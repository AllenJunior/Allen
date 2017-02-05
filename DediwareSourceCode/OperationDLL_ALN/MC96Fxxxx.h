#include "stdafx.h"
#include <assert.h>
#include <vector>
#include "stdio.h"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <tchar.h>
#include "../Log/log.h"
#include "../common/comm_type.h"
#include "../common/loadfile_info.h"
#include "../common/prog_info.h"
#include "../vm_mgr/vm_mgr.h"

int MC96F6632(
	const struct chip_info_c                *chip,                  /* chip info structure */
	std::vector<struct loadfile_du>         &du_header,             /* du header */
	vm_ptr_t                                buff,
	loadfile_item                           **reg,
	unsigned long                           *chip_checksum,         /* loadfile --> chipchecksum */
	std::vector<struct loadfile_fcs>        &file_checksum,          /* file checksum */
	const struct loadfile_funcs				*funcs
	)
{
	//if the file item is not .HPO
	std::wstring EndOfPath = &du_header.at(0).image_file_path[0];
	int pathlength = EndOfPath.length();
	EndOfPath.erase(0, pathlength - 4);
	const wchar_t *path_t = EndOfPath.c_str();
	const wchar_t *FileItem_CMP1 = L".HPO";
	const wchar_t *FileItem_CMP2 = L".hpo";
	if ((wcscmp(path_t, FileItem_CMP1) != 0)
		&& (wcscmp(path_t, FileItem_CMP2) != 0))
	{
		//MessageBox(NULL, L"The File be loaded is not in (.HPO) Format !", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
		return 0;
	}
	//Setting reg buff
	loadfile_item *pStruct_Buffer;
	pStruct_Buffer = (loadfile_item*)malloc(sizeof(loadfile_item)+chip->partition_info[1].size_in_bytes);
	memset(pStruct_Buffer->buff, 0, chip->partition_info[1].size_in_bytes);
	const wchar_t *path = &du_header.at(0).image_file_path[0];
	std::ifstream input(path);
	std::string Config_str;
	char configData[4];
	char strtemp_cpy[2];
	char tempdata;
	int r;
	memset(configData, 0x00, 4);
	//Find ConfigData
	do{
		std::getline(input, Config_str);
		r = Config_str.find_first_of(_T('O'));
	} while (r != 0);
	unsigned int length = Config_str.length();
	//Option1
	Config_str.erase(0, 1);
	strncpy(strtemp_cpy, Config_str.c_str(), 2);
	tempdata = strtol(strtemp_cpy, NULL, 16);
	memcpy(configData, &tempdata, 1);
	//Option2
	Config_str.erase(0, 3);
	strncpy(strtemp_cpy, Config_str.c_str(), 2);
	tempdata = strtol(strtemp_cpy, NULL, 16);
	memcpy(configData + 1, &tempdata, 1);
	memcpy(pStruct_Buffer->buff, &configData, sizeof(configData));
	//CheckButton Initial
	int Checkint = 0x01;
	memcpy(pStruct_Buffer->buff + sizeof(configData), &Checkint, sizeof(Checkint));
	//Setting Buff temp
	std::vector<unsigned char> vMemBuf_temp((unsigned int)chip->partition_info[0].size_in_bytes);
	memset(&vMemBuf_temp.at(0), chip->partition_info[0].blank_value, sizeof(vMemBuf_temp));
	//Find, Unique key Area
	vm_read(&vMemBuf_temp.at(0), buff + du_header.at(0).data_offset_in_buff, du_header.at(0).image_length);
	unsigned int end_point = 0;
	unsigned int begin_point = 0;
	unsigned int p_keylength = 0;
	unsigned int addr_pointer = chip->partition_info[0].size_in_bytes - 1; //address pointer
	for (addr_pointer > 0; addr_pointer--;)
	{
		if (vMemBuf_temp.at(addr_pointer) == 0xD5)
		{
			end_point = addr_pointer;
			for (addr_pointer > 0; addr_pointer--;)
			{
				if (vMemBuf_temp.at(addr_pointer) == 0xFE)
				{
					begin_point = addr_pointer;
					//transfer the buffer for unique area
					//keylength = between 0xFE & 0xD5 " end - start - 2(begin & end) 
					unsigned int keylength = (end_point - begin_point + 1) - 2;
					if (keylength > 6)
					{
						return FALSE;
						break;
					}
					//check the area value (==0x00) || (== 0xFF)
					for (unsigned int k = 1; k <= keylength; k++)
					{
						if ((vMemBuf_temp.at(begin_point + k) == 0xFF) || (vMemBuf_temp.at(begin_point + k) == 0x00))
							continue;
						else
							//MessageBox(NULL, L"Please check the file for the Unique Key area !!!", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
							goto skip_unique;
					}
					for (unsigned int j = 1; j <= keylength; j++)
					{
						vMemBuf_temp.at(begin_point + j) &= 0x00;
					}
					p_keylength = keylength;
					vm_write(buff, &vMemBuf_temp.at(0), du_header.at(0).image_length);
					du_header.at(0).data_crc_value = funcs->du_crc(&vMemBuf_temp.at(0), (unsigned long)du_header.at(0).program_length);
					du_header.at(0).data_checksum_value = funcs->du_checksum(&vMemBuf_temp.at(0), (unsigned long)du_header.at(0).program_length);
					du_header.at(0).du_crc = funcs->du_crc(&vMemBuf_temp.at(0), (unsigned long)du_header.at(0).image_length);
				skip_unique:
					break;
				}
			}
			break;
		}
	}
	//Address save to register
	unsigned int pointaddress = (begin_point + 1);
	memcpy(pStruct_Buffer->buff + sizeof(configData)+sizeof(Checkint), &pointaddress, sizeof(pointaddress));
	//CRC value to register
	unsigned int CRC = (*chip_checksum)&0xFFFF;
	memcpy(pStruct_Buffer->buff + sizeof(configData)+sizeof(Checkint)+sizeof(pointaddress), &CRC, sizeof(CRC));
	//Initial UserID Value
	char UserID_config[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	memcpy(pStruct_Buffer->buff + sizeof(configData)+sizeof(Checkint)+sizeof(pointaddress)+sizeof(CRC), &UserID_config, sizeof(UserID_config));
	pStruct_Buffer->len = chip->partition_info[1].size_in_bytes;
	*reg = pStruct_Buffer;
	return 0;
}

	int MC96F6432S(
		const struct chip_info_c                *chip,                  /* chip info structure */
		std::vector<struct loadfile_du>         &du_header,             /* du header */
		vm_ptr_t                                buff,
		loadfile_item                           **reg,
		unsigned long                           *chip_checksum,         /* loadfile --> chipchecksum */
		std::vector<struct loadfile_fcs>        &file_checksum,          /* file checksum */
		const struct loadfile_funcs				*funcs
		)
	{
		std::wstring EndOfPath = &du_header.at(0).image_file_path[0];
		int pathlength = EndOfPath.length();
		EndOfPath.erase(0, pathlength - 4);
		const wchar_t *path_t = EndOfPath.c_str();
		const wchar_t *FileItem_CMP1 = L".HPO";
		const wchar_t *FileItem_CMP2 = L".hpo";
		if ((wcscmp(path_t, FileItem_CMP1) != 0)
			&& (wcscmp(path_t, FileItem_CMP2) != 0))
		{
			//MessageBox(NULL, L"The File be loaded is not in (.HPO) Format !", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
			return FALSE;
		}
		//Setting option temp
		loadfile_item *pStruct_Buffer;
		//unsigned int reg_len = chip->partition_info[1].size_in_bytes;
		unsigned int p_keylength = 0;
		pStruct_Buffer = (loadfile_item*)malloc(sizeof(loadfile_item)+chip->partition_info[1].size_in_bytes);
		memset(pStruct_Buffer->buff, 0, chip->partition_info[1].size_in_bytes);
		//Setting Buff temp
		std::vector<unsigned char> vMemBuf_temp((unsigned int)chip->partition_info[0].size_in_bytes);
		memset(&vMemBuf_temp.at(0), chip->partition_info[0].blank_value, sizeof(vMemBuf_temp));
		//Find, Unique key Area
		vm_read(&vMemBuf_temp.at(0), buff + du_header.at(0).data_offset_in_buff, du_header.at(0).image_length);
		unsigned int end_point = 0;
		unsigned int begin_point = 0;
		unsigned int addr_pointer = chip->partition_info[0].size_in_bytes - 1; //address pointer

		for (addr_pointer > 0; addr_pointer--;)
		{
			if (vMemBuf_temp.at(addr_pointer) == 0xD5)
			{
				end_point = addr_pointer;
				for (addr_pointer > 0; addr_pointer--;)
				{
					if (vMemBuf_temp.at(addr_pointer) == 0xFE)
					{
						begin_point = addr_pointer;
						//transfer the buffer for unique area
						//keylength = between 0xFE & 0xD5 " end - start - 2(begin & end) 
						unsigned int keylength = (end_point - begin_point + 1) - 2;
						//check the area value (==0x00) || (== 0xFF)
						for (unsigned int k = 1; k <= keylength; k++)
						{
							if ((vMemBuf_temp.at(begin_point + k) == 0xFF) || (vMemBuf_temp.at(begin_point + k) == 0x00))
								continue;
							else
								//MessageBox(NULL, L"Please check the file for the Unique Key area !!!", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
								goto skip_unique;
						}
						for (unsigned int j = 1; j <= keylength; j++)
						{
							vMemBuf_temp.at(begin_point + j) &= 0x00;
						}
						p_keylength = keylength;
						vm_write(buff, &vMemBuf_temp.at(0), du_header.at(0).image_length);
						du_header.at(0).data_crc_value = funcs->du_crc(&vMemBuf_temp.at(0), (unsigned long)du_header.at(0).program_length);
						du_header.at(0).data_checksum_value = funcs->du_checksum(&vMemBuf_temp.at(0), (unsigned long)du_header.at(0).program_length);
						du_header.at(0).du_crc = funcs->du_crc(&vMemBuf_temp.at(0), (unsigned long)du_header.at(0).image_length);
					skip_unique:
						break;
					}
				}
				break;
			}
		}
		//Getting Option setting
		const wchar_t *path = &du_header.at(0).image_file_path[0];
		std::ifstream input(path);
		std::string Config_str;
		unsigned int r, length;
		unsigned char configdata[4] = { 0x00 };
		char strtemp_cpy[2];
		char tempdata;
		do{
			std::getline(input, Config_str);
			r = Config_str.find_first_of(_T('O'));
		} while (r != 0);
		length = Config_str.length();
		//Option1
		Config_str.erase(0, 1);
		strncpy(strtemp_cpy, Config_str.c_str(), 2);
		tempdata = strtol(strtemp_cpy, NULL, 16);
		memcpy(configdata, &tempdata, 1);
		//Option2
		Config_str.erase(0, 3);
		strncpy(strtemp_cpy, Config_str.c_str(), 2);
		tempdata = strtol(strtemp_cpy, NULL, 16);
		memcpy(configdata + 1, &tempdata, 1);
		memcpy(pStruct_Buffer->buff, &configdata, sizeof(configdata));
		if (wcscmp(chip->description, L"MC96F6432S") == 0)
		{
			//CRC checkbox
			int initcheck = 0x01;
			memcpy(pStruct_Buffer->buff + sizeof(configdata), &initcheck, sizeof(initcheck));
			//checksum
			int checksum = *chip_checksum;
			checksum &= 0xFFFF;
			memcpy(pStruct_Buffer->buff + sizeof(configdata)+sizeof(initcheck), &checksum, sizeof(checksum));
			pStruct_Buffer->len = chip->partition_info[1].size_in_bytes;
			*reg = pStruct_Buffer;
		}
		if (wcscmp(chip->description, L"MC96F6432") == 0)
		{
			//CRC checkbox
			int initcheck = 0x01;
			memcpy(pStruct_Buffer->buff + sizeof(configdata), &initcheck, sizeof(initcheck));
			//checksum
			int checksum = *chip_checksum;
			checksum &= 0xFFFF;
			memcpy(pStruct_Buffer->buff + sizeof(configdata)+sizeof(initcheck), &checksum, sizeof(checksum));
			//ID checkbox
			unsigned int id_lentgh_cmp = 6;
			int idcheck = !(memcmp(&p_keylength, &id_lentgh_cmp, 1));
			memcpy(pStruct_Buffer->buff + sizeof(configdata)+sizeof(initcheck)+sizeof(checksum), &idcheck, sizeof(idcheck));
			//Address
			unsigned int pointaddress = (begin_point + 1);
			memcpy(pStruct_Buffer->buff + sizeof(configdata)+sizeof(initcheck)+sizeof(checksum)+sizeof(idcheck), &pointaddress, sizeof(pointaddress));
			//temp1
			char UserID_config[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
			memcpy(pStruct_Buffer->buff + sizeof(configdata)+sizeof(initcheck)+sizeof(checksum)+sizeof(idcheck)+sizeof(pointaddress), &UserID_config, sizeof(UserID_config));
			pStruct_Buffer->len = chip->partition_info[1].size_in_bytes;
			*reg = pStruct_Buffer;
		}
		return 0;
	}