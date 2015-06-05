// OperationDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <assert.h>
#include <vector>
#include "../common/comm_type.h"
#include "../common/loadfile_info.h"
#include "../common/prog_info.h"
#include "../vm_mgr/vm_mgr.h"
#include "stdio.h"
#include <iostream>
#include <fstream>
#include <string>
#include <tchar.h>
#include "../Log/log.h"
#include <string.h>

int Calu_du_crc(std::vector<struct loadfile_du> &du_header, vm_ptr_t buff, const struct loadfile_funcs *funcs)
{
	int status;
	unsigned int ProgramLength = (unsigned int)du_header.at(0).program_length;
	unsigned int ImageLength = (unsigned int)du_header.at(0).image_length;
	unsigned char *pProgramFlash = (unsigned char*)malloc(ProgramLength);
	unsigned char *pImageFlash = (unsigned char*)malloc(ImageLength);

	memset(pProgramFlash, 0xFF, ProgramLength);
	status = vm_read(pProgramFlash, buff, ProgramLength);
	if (status != 0)return status;
	du_header.at(0).data_crc_value = funcs->du_crc(pProgramFlash, ProgramLength);


	memset(pImageFlash, 0xFF, ImageLength);
	status = vm_read(pImageFlash, buff, ImageLength);
	if (status != 0)return status;
	du_header.at(0).data_checksum_value = funcs->du_checksum(pImageFlash, ImageLength);
	du_header.at(0).du_crc = funcs->du_crc(pImageFlash, ImageLength);

	free(pProgramFlash);
	free(pImageFlash);

	return 0;
}


unsigned char MsbToLsb(unsigned char Value)
{
	unsigned char num = Value;   // 1010 1010 (aa) -> 0101 0101 (55)
	int s = sizeof(num)* 8;    // get number of bits
	int i, x, y, p;
	int var = 0;                // make var data type to be equal or larger than num

	for (i = 0; i < (s / 2); i++)
	{
		// extract bit on the left, from MSB
		p = s - i - 1;
		x = num & (1 << p);
		x = x >> p;
		//printf("x: %d\n", x);

		// extract bit on the right, from LSB
		y = num & (1 << i);
		y = y >> i;
		// printf("y: %d\n", y);

		var = var | (x << i);       // apply x
		var = var | (y << p);       // apply y
	}
	return var;
}
int IR35XXX(
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
	const wchar_t *FileItem_CMP = L".txt";
	int pck = wcscmp(path_t, FileItem_CMP);
	if (pck != 0)
	{
		MessageBox(NULL, L"Your File is not .TXT file !!!", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
		return 0;
	}
	if ((du_header.at(0).program_length / 3) * 4 > du_header.at(0).image_length)
		return 0;

	unsigned char *bufferCopy = (unsigned char *)malloc(du_header.at(0).image_length);
	if (bufferCopy == 0) return 0;
	unsigned char *bufferTemp = (unsigned char *)malloc(du_header.at(0).image_length);
	if (bufferTemp == 0) return 0;


	vm_read(bufferCopy, buff + du_header.at(0).data_offset_in_buff, du_header.at(0).image_length);
	unsigned int i = 0x00, j = 0x00;
	while (i < du_header.at(0).image_length) {
		bufferTemp[i++] = bufferCopy[j++];
	}
	du_header.at(0).program_length = ((du_header.at(0).program_length) / 3) * 4;
	i = 0x00, j = 0x00;
	while (i < du_header.at(0).program_length) {
		bufferCopy[i++] = bufferTemp[j++];
		bufferCopy[i++] = bufferTemp[j++];
		bufferCopy[i++] = bufferTemp[j++];
		bufferCopy[i++] = 0xFF;
		//j++;
	};
	vm_write(buff + du_header.at(0).data_offset_in_buff, bufferCopy, du_header.at(0).image_length);

	/* call checksum func*/
	du_header.at(0).data_crc_value = funcs->du_crc(bufferCopy, du_header.at(0).program_length);
	du_header.at(0).data_checksum_value = funcs->du_checksum(bufferCopy, du_header.at(0).program_length);
	du_header.at(0).du_crc = funcs->du_crc(bufferCopy, du_header.at(0).image_length);
	free(bufferCopy);
	free(bufferTemp);

}
int MC96Fxxxx(
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
	const wchar_t *FileItem_CMP = L".HPO";
	int pck = wcscmp(path_t, FileItem_CMP);
	if (pck != 0)
	{
		MessageBox(NULL, L"Your File is not .HPO file !!!", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
		return 0;
	}
	//Find ConfigData
	loadfile_item *pStruct_Buffer;
	unsigned int BufferLen = 24;
	pStruct_Buffer = (loadfile_item*)malloc(sizeof(loadfile_item)+BufferLen);
	memset(pStruct_Buffer->buff, 0, BufferLen);
	const wchar_t *path = &du_header.at(0).image_file_path[0];
	std::ifstream input(path);
	std::string Config_str;
	char configData[4];
	char strtemp_cpy[2];
	char tempdata;
	int r, length;
	memset(configData, 0x00, 4);
	do{
		std::getline(input, Config_str);
		r = Config_str.find_first_of(_T('O'));
	} while (r != 0);
	length = Config_str.length();
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
	int Checkint = 0x00000000;
	memcpy(pStruct_Buffer->buff + sizeof(configData), &Checkint, sizeof(Checkint));
	//Find Unique ID - Address and Mask the value to 0x00
	unsigned int i = 0;
	unsigned char *pBufferCopy = (unsigned char *)malloc((unsigned long)du_header.at(i).image_length);
	if (pBufferCopy == NULL) {
		return 1;
	}
	char UserID[8] = { 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD5 };
	char UID_Cmp[8];
	int CmpValue = 0;
	int K = 0;
	vm_read(pBufferCopy, buff + du_header.at(i).data_offset_in_buff, du_header.at(i).image_length);
	do{
		memcpy(UID_Cmp, &pBufferCopy[K], 8);
		CmpValue = memcmp(UserID, UID_Cmp, 8);
		K += 1;
	} while (CmpValue != 0);
	for (int c = 1; c < 7; c++)
	{
		UID_Cmp[c] &= 0x00;
	}
	K -= 1; //address
	memcpy(&pBufferCopy[K], UID_Cmp, 8);
	vm_write(buff, pBufferCopy, du_header.at(0).program_length);
	/* call checksum func*/
	du_header.at(i).data_crc_value = funcs->du_crc(pBufferCopy, (unsigned long)du_header.at(i).program_length);
	du_header.at(i).data_checksum_value = funcs->du_checksum(pBufferCopy, (unsigned long)du_header.at(i).program_length);
	du_header.at(i).du_crc = funcs->du_crc(pBufferCopy, (unsigned long)du_header.at(i).image_length);
	/* 如果没有Register partion，ICSetting Dilog, 默认Load file要更改buffer，就把下面的示例代码写道这里 */
	//Address save to register
	K += 1;
	memcpy(pStruct_Buffer->buff + sizeof(configData)+sizeof(Checkint), &K, sizeof(K));
	//CRC value to register
	int CRC = *chip_checksum;
	CRC &= 0x00FFFF;
	memcpy(pStruct_Buffer->buff + sizeof(configData)+sizeof(Checkint)+sizeof(K), &CRC, sizeof(CRC));
	//Initial UserID Value
	char UserID_config[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	memcpy(pStruct_Buffer->buff + sizeof(configData)+sizeof(Checkint)+sizeof(K)+sizeof(CRC), &UserID_config, sizeof(UserID_config));
	free(pBufferCopy);
	pStruct_Buffer->len = BufferLen;
	*reg = pStruct_Buffer;
	return 0;

}
int iML7975_Special(
	const struct chip_info_c                *chip,                  /* chip info structure */
	std::vector<struct loadfile_du>         &du_header,             /* du header */
	vm_ptr_t                                buff,
	loadfile_item                           **reg,
	unsigned long                           *chip_checksum,         /* loadfile --> chipchecksum */
	std::vector<struct loadfile_fcs>        &file_checksum,          /* file checksum */
	const struct loadfile_funcs				*funcs
	)
{
	unsigned char temp[2];
	loadfile_item *pStruct_Buffer;
	std::vector<unsigned char> vMemBuf((unsigned int)chip->partition_info[0].size_in_bytes);
	std::fill(vMemBuf.begin(), vMemBuf.end(), 0xFF);
	vm_read((unsigned char*)&vMemBuf[0] + du_header.at(0).ptn_addr, buff + du_header.at(0).data_offset_in_buff, du_header.at(0).program_length);
	memcpy(temp, &vMemBuf.at(0), 2);
	temp[1] |= 0xFF;
	vm_write(buff, temp, du_header.at(0).program_length);
	int checksum_temp = temp[0];
	*chip_checksum = checksum_temp;
	Calu_du_crc(du_header, buff, funcs);
	return 0;
}


int Cal_JEDChecksum(
	const struct chip_info_c                *chip,                  /* chip info structure */
	std::vector<struct loadfile_du>         &du_header,             /* du header */
	vm_ptr_t                                buff,
	loadfile_item                           **reg,
	unsigned long                           *chip_checksum,         /* loadfile --> chipchecksum */
	std::vector<struct loadfile_fcs>        &file_checksum          /* file checksum */
	)
{
	wchar_t strpath = du_header.at(0).image_file_path[0];
	std::wstring EndOfPath = &du_header.at(0).image_file_path[0];
	int Length = du_header.at(0).program_length;
	//
	int pathlength = EndOfPath.length();
	EndOfPath.erase(0, pathlength - 4);
	const wchar_t *path_t = EndOfPath.c_str();
	const wchar_t *FileItem_CMP = L".jed";
	int pck = wcscmp(path_t, FileItem_CMP);
	//
	//int pck = EndOfPath.find(_T('.jed'));
	if (pck != 0)
	{
		MessageBox(NULL, L"The Path of File is too long !", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
		return 0;
	}
	const wchar_t *path = &du_header.at(0).image_file_path[0];
	std::ifstream input(path);
	std::string line, line2, Templine, crcvalue;
	unsigned int checksum = 0;
	char tempdata[8];
	int i, j, t, k, L;
	//Find the "L0000"
	do{
		std::getline(input, line);
		line2 = line;
		i = line.find_first_of(_T('L'));
	} while (i != 0);	//Find "L"
	//String Adding
	do{
		std::getline(input, line);
		Templine += line;	//Row + Row...
		j = line.find_first_of(_T('*'));
	} while (j != 0);	//FindEnd"*"
	int x, y;
	x = Templine.length();
	Templine.erase(x - 1, 1);
	y = 8 - ((x - 1) % 8);
	if (y < 8)
	{
		for (int z = 0; z < y; z++)
		{
			std::string a = "0";
			Templine += a;
		}
	}
	//Calculate the CRC value
	for (int k = 0; k < Length; k++)
	{
		strncpy(tempdata, Templine.c_str(), 8);
		int data = strtol(tempdata, NULL, 2);
		checksum += MsbToLsb(data);
		Templine.erase(0, 8);
	}
	//Find the "CXXXX"
	do{
		std::getline(input, line);
		crcvalue = line;
		j = line.find_first_of(_T('C'));
	} while (j != 0);
	crcvalue.erase(0, 1);
	crcvalue.erase(4, 1);
	int c = strtol(crcvalue.c_str(), NULL, 16);
	checksum &= 0xFFFF;
	t = memcmp(&c, &checksum, sizeof(checksum));
	if (t == 0x0){
		*chip_checksum = checksum;
	}
	else
		MessageBox(NULL, L"CheckSum Different !", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
	return 0;
}

	int XO2Feature(
		const struct chip_info_c                *chip,                  /* chip info structure */
		std::vector<struct loadfile_du>         &du_header,             /* du header */
		vm_ptr_t                                buff,
		loadfile_item                           **reg,
		unsigned long                           *chip_checksum,         /* loadfile --> chipchecksum */
		std::vector<struct loadfile_fcs>        &file_checksum          /* file checksum */
		)
	{
		wchar_t featurerow = du_header.at(0).image_file_path[0];
		std::wstring EndOfPath = &du_header.at(0).image_file_path[0];
		loadfile_item *pStruct_Buffer;
		int pathlength = EndOfPath.length();
		EndOfPath.erase(0, pathlength - 4);
		const wchar_t *path_t = EndOfPath.c_str();
		const wchar_t *FileItem_CMP = L".jed";
		int pcmp = wcscmp(path_t, FileItem_CMP);
		if (pcmp != 0)
		{
			MessageBox(NULL, L"The Path of File is too long !", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
			return 0;
		}
		const wchar_t *path = &du_header.at(0).image_file_path[0];
		std::ifstream input(path);
		std::string rowstr, bitstr, ucode,securstr;
		char byte[8];
		char Frow[8];
		char Fbit[4];
		char Ucode[4];
		char Security[4];
		unsigned char regdata[20]; //16
		int r, p, q, w, s, t;
		if (wcscmp(chip->description, L"ispMachXO2") == 0)
		{
			unsigned int BufferLen = 20; //16
			pStruct_Buffer = (loadfile_item*)malloc(sizeof(loadfile_item)+BufferLen);
			memset(pStruct_Buffer->buff, 0, BufferLen);
			//Find the Security bit
			do {
				std::getline(input, securstr);
				s = securstr.find_first_of(_T('G'));
			} while (s != 0);
			memset(Security, 0, 4);
			securstr.erase(0, 1);
			securstr.erase(1, 1);
			const char *s_cmp = "0";
			const char *s_en = securstr.c_str();
			t = strcmp(s_cmp, s_en);
			//int t = 0;
			if (t == 0x0)
				Security[0] = 0x00;
			else
				Security[0] = 0x01;
			//Find the FeatureRow(8 Bytes)
			do{
				std::getline(input, rowstr);
				r = rowstr.find_first_of(_T('E'));
			} while (r != 0);
			rowstr.erase(0, 1);//Erase ("E")
			for (int i = 0; i < 8; i++)
			{
				strncpy(byte, rowstr.c_str(), 8);
				char row = strtol(byte, NULL, 2);
				memcpy(Frow + i, &row, 1);
				rowstr.erase(0, 8);
			}
			//Get the Feature Bit(2 Bytes)
			std::getline(input, bitstr);
			p = bitstr.length();
			bitstr.erase(p - 1, 1);
			memset(Fbit, 0, 4); //Fbit[4Bytes]
			for (int j = 0; j < 2; j++)
			{
				strncpy(byte, bitstr.c_str(), 8);
				char bit = strtol(byte, NULL, 2);
				if (j == 0)
				{
					memcpy(Fbit + 1, &bit, 1);
				}
				else
					memcpy(Fbit, &bit, 1);
				bitstr.erase(0, 8);
			}
			//Find Usercode
			do{
				std::getline(input, ucode);
				q = ucode.find_first_of(_T('UH'));
			} while (q != 0);
			w = ucode.length();
			ucode.erase(w - 1, 1);
			ucode.erase(0, 2);
			std::string tempucode;
			tempucode = ucode;
			tempucode.erase(4, 4);
			short int Ucode1 = strtol(tempucode.c_str(), NULL, 16);
			ucode.erase(0, 4);
			short int Ucode2 = strtol(ucode.c_str(), NULL, 16);
			memcpy(regdata, Frow, sizeof(Frow));
			memcpy(regdata + sizeof(Frow), Fbit, sizeof(Fbit));
			memcpy(regdata + sizeof(Frow)+sizeof(Fbit), &Ucode2, sizeof(Ucode2));
			memcpy(regdata + sizeof(Frow)+sizeof(Fbit)+sizeof(Ucode2), &Ucode1, sizeof(Ucode1));
			memcpy(regdata + sizeof(Frow)+sizeof(Fbit)+sizeof(Ucode2)+sizeof(Ucode1), &Security, sizeof(Security));
			memcpy(pStruct_Buffer->buff, &regdata, sizeof(regdata));
			pStruct_Buffer->len = BufferLen;
			*reg = pStruct_Buffer;
		}
		if (wcscmp(chip->description, L"ispMACHXO") == 0 || wcscmp(chip->description, L"ispPAC") == 0)
		{
			unsigned int BufferLen = 8;
			pStruct_Buffer = (loadfile_item*)malloc(sizeof(loadfile_item)+BufferLen);
			memset(pStruct_Buffer->buff, 0, BufferLen);
			//Find Security Setting
			do {
				std::getline(input, securstr);
				s = securstr.find_first_of(_T('G'));
			} while (s != 0);
			memset(Security, 0, 4);
			securstr.erase(0, 1);
			securstr.erase(1, 1);
			const char *s_cmp = "0";
			const char *s_en = securstr.c_str();
			t = strcmp(s_cmp, s_en);
			//int t = 0;
			if (t == 0x0)
				Security[0] = 0x00;
			else
				Security[0] = 0x01;
			//Fing UserCode
			do{
				std::getline(input, ucode);
				q = ucode.find_first_of(_T('U'));
			} while (q != 0);
			w = ucode.length();
			ucode.erase(w - 1, 1);
			ucode.erase(0, 1);
			for (int i = 0; i < 4; i++)
			{
				strncpy(byte, ucode.c_str(), 8);
				char tempucode = strtol(byte, NULL, 2);
				Ucode[3 - i] = tempucode;
				ucode.erase(0, 8);
			}
			memcpy(pStruct_Buffer->buff, &Ucode, sizeof(Ucode));
			memcpy(pStruct_Buffer->buff + sizeof(Ucode), &Security, sizeof(Security));
			pStruct_Buffer->len = BufferLen;
			*reg = pStruct_Buffer;
		}
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport)  
int du_to_reg(
                const struct chip_info_c                *chip,                  /* chip info structure */
                std::vector<struct loadfile_du>         &du_header,             /* du header */
                vm_ptr_t                                buff,
                loadfile_item                           **reg,
                unsigned long                           *chip_checksum,         /* loadfile --> chipchecksum */
                std::vector<struct loadfile_fcs>        &file_checksum,          /* file checksum */
				const struct loadfile_funcs				*funcs
                )
{
        assert(chip);
        assert(reg);
        assert(chip_checksum);

		//check du_header NULL?	 
		if (du_header.empty())
			return 0;
		if (wcscmp(chip->manufacturer, L"International Rectifier") == 0)
		{
			IR35XXX(chip, du_header, buff, reg, chip_checksum, file_checksum, funcs);
		}

		if (wcscmp(chip->manufacturer, L"ABOV Semiconductor") == 0)
		{
			//Only for Midea
			if (wcscmp(chip->part_name, L"MC96F6632[QFP64]") == 0
				|| wcscmp(chip->part_name, L"MC96F6832L/Q[MQFP80]") == 0
				)
			{
				MC96Fxxxx(chip, du_header, buff, reg, chip_checksum, file_checksum, funcs);
			}
		}
		
		if (wcscmp(chip->part_name, L"iML7975[DFN10]") == 0)
		{
			iML7975_Special(chip, du_header, buff, reg, chip_checksum, file_checksum, funcs);
		}

		if (wcscmp(chip->description, L"ispMachXO2") == 0|| wcscmp(chip->description,L"ispMACHXO") ==0 
			|| wcscmp(chip->description, L"ispPAC") == 0)
		{
			XO2Feature(chip, du_header, buff, reg, chip_checksum, file_checksum);
		}
		
		if (wcscmp(chip->manufacturer, L"LATTICE") == 0)
		{
			Cal_JEDChecksum(chip, du_header, buff, reg, chip_checksum, file_checksum);
		}
		else
			return 0;

		return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport)
int final_du(
			const struct chip_info_c                *chip,                  /* chip info structure */
			std::vector<struct loadfile_du>         &du_header,             /* du header */
			vm_ptr_t                                buff,
			loadfile_item                           **reg,
			unsigned long                           *chip_checksum,         /* loadfile --> chipchecksum */
			std::vector<struct loadfile_fcs>        &file_checksum          /* file checksum */
)
{
	assert(chip);
	assert(reg);
	assert(chip_checksum);

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport)
int read_reg(
				const struct chip_info_c        *chip,
				unsigned char                   *reg,
				int                             reg_len,
				int(*read)(
							const struct chip_info_c  *chip,
							int             ptn_index,
							unsigned long long start,
							unsigned char   *buff,
							int             len,
							int             timeout,
							void            *param
				),
				void                            *read_param
)
{
	assert(chip);
	assert(reg);
	assert(read);
	assert(read_param);

	//LCMXO2 Read to register
	if (wcscmp(chip->description, L"ispMachXO2") == 0)
	{
		unsigned char buffer[14];
		unsigned char regdata[16];
		char FeatureRow[8];
		char FeatureBit[4];
		char Usercode[4];
		read(chip, 1, 0, buffer, 14, 1000, read_param); 
		memcpy(FeatureRow, buffer, 8);
		memset(FeatureBit, 0, 4);
		for (int i = 0; i < 2; i++)
		{
			memcpy(FeatureBit + i, buffer + sizeof(FeatureRow) + 1 - i, 1); //reverse take
		}
		for (int j = 0; j < 4; j++)
		{
			memcpy(Usercode + j, buffer + sizeof(FeatureRow) + sizeof(FeatureBit) + 1 - j, 1);
		}
		memcpy(regdata, FeatureRow, sizeof(FeatureRow));
		memcpy(regdata + sizeof(FeatureRow), FeatureBit, sizeof(FeatureBit));
		memcpy(regdata + sizeof(FeatureRow)+sizeof(FeatureBit), Usercode, sizeof(Usercode));
		memcpy(reg, &regdata, 16);
		return reg_len;
	}
	//LC4XXX,LCMXO,Powr6xx read to register
	if (wcscmp(chip->description, L"ispMach40xx") == 0 || wcscmp(chip->description, L"ispMACHXO") == 0
		|| wcscmp(chip->description, L"ispPAC") == 0)
	{
		unsigned char buffer[4];
		char Usercode[4];
		read(chip, 1, 0, buffer, 4, 1000, read_param);
		for (int j = 0; j < 4; j++)
		{
			memcpy(Usercode + j, buffer + 3 - j, 1);
		}
		memcpy(reg, &Usercode, 4);
		return reg_len;
	}
	//Nuvoton read to register
	if (wcscmp(chip->manufacturer, L"Nuvoton") == 0)
	{
		if (wcscmp(chip->description, L"Nu8051") == 0)
		{
			unsigned char buffer[4];
			read(chip, 2, 0, buffer, 4, 1000, read_param);
			memcpy(reg, &buffer, 4);
			return reg_len; 
		}

		if (wcscmp(chip->description, L"M05X") == 0)
		{
			unsigned char buffer[4];
			read(chip, 3, chip->partition_info[3].start_program_addr, buffer, 4, 1000, read_param);
			memcpy(reg, &buffer, 4);
			return reg_len;
		}

		if (
				wcscmp(chip->description, L"Mini51") == 0
				|| wcscmp(chip->description, L"M0518") == 0
				|| wcscmp(chip->description, L"NUC1X0ACN") == 0
				|| wcscmp(chip->description, L"ISD9XXX") == 0
				|| wcscmp(chip->description, L"NUC103") == 0
				|| wcscmp(chip->description, L"NUC100DN") == 0
			)
		{
			unsigned char buffer[8];
			read(chip, 3, chip->partition_info[3].start_program_addr, buffer, 8, 1000, read_param);
			memcpy(reg, &buffer, 8);
			return reg_len;
		}

	}
	//ATMEL ATxmega256A3Bu
	if (wcscmp(chip->manufacturer, L"Atmel") == 0)
	{
		if (wcscmp(chip->description, L"ATxmegaA3BU") == 0)
		{
			unsigned char buffer[6];
			read(chip, chip->partition_cnt-1, 0, buffer, chip->partition_info[4].size_in_bytes, 1000, read_param);
			memcpy(reg, &buffer, 6);
			return reg_len;
		}

	}
	//ABOV MC96F6632
	if (wcscmp(chip->manufacturer, L"ABOV Semiconductor") == 0)
	{
		//Only for Midea
		if (wcscmp(chip->part_name, L"MC96F6632[QFP64]") == 0
			|| wcscmp(chip->part_name, L"MC96F6832L/Q[MQFP80]") == 0
			)
		{
			unsigned char temp[10];
			unsigned char configdata[4];
			unsigned char UserID[6];
			unsigned char CRC[2];
			unsigned char button_CMP[2] = { 0x00, 0x00 };
			unsigned char buffer[24];
			memset(configdata, 0x00, 4);
			memset(temp, 0x00, 10);
			memset(buffer, 0x00, 24);
			read(chip, 1, 0, temp, 10, 1000, read_param);
			//configData
			memcpy(configdata, temp, 2);
			memcpy(buffer, configdata, sizeof(configdata));
			//checkbutton
			memcpy(CRC, temp + 8, 2);
			int check = memcmp(button_CMP, CRC, 2);
			if (check == 0)
			{
				memcpy(buffer + sizeof(configdata), &check, sizeof(check));
			}	
			else if (check != 0)
			{
				int check = 0x01;
				memcpy(buffer + sizeof(configdata), &check, sizeof(check));
			}
			//UserID
			for (int i = 0;  i < 6 ; i++)
			{
				memcpy(&UserID[5-i], temp + 2 + i, 1);
			}
			memcpy(buffer + 18, UserID, sizeof(UserID));
			//CRC
			memcpy(buffer + 12, temp + 8, 2);
			memcpy(reg, &buffer, 24);
			return reg_len;
		}
		
		if (wcscmp(chip->part_name, L"MC96FC664AL[QFP64]") == 0
			|| wcscmp(chip->part_name, L"MC96FC864AL[QFP80]") == 0
			)
		{
			unsigned char buffer[4];
			unsigned char temp[2];
			memset(buffer, 0x00, 4);
			read(chip, 1, 0, temp, 2, 1000, read_param);
			memcpy(buffer, &temp[0], 1);
			memcpy(reg, &buffer, 4);
			return reg_len;
		}
		
	}
	
}
