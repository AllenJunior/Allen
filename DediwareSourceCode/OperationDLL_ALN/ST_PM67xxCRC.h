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

unsigned short CRC16_CCITT(unsigned char *s, unsigned char len)
{
	unsigned char i, j;
	unsigned short crc = 0x1D0F;   //Preset Value
	for (i = 0; i<len; i++)
	for (j = 0x80; j != 0; j >>= 1)
	{
		if ((crc & 0x8000) != 0)
		{
			crc <<= 1;
			crc ^= 0x1021;
		}
		else
			crc <<= 1;
		if ((s[i] & j) != 0)
		{
			crc ^= 0x1021;
		}
	}
	return crc;
}

int ST_CRC(
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
	const wchar_t *FileItem_CMP1 = L".nvm";
	const wchar_t *FileItem_CMP2 = L".NVM";
	if (wcscmp(path_t, FileItem_CMP1) != 0
		&& wcscmp(path_t, FileItem_CMP2) != 0)
	{
		MessageBox(NULL, L"Your File is not .NVM file !!!", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
		return 1;
	}
	//MessageBox(NULL, L"Reanalyze File & Calculating Checksum", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
	//loadfile_item *pStruct_Buffer;
	std::vector<unsigned char> vMemBuf((unsigned int)chip->partition_info[0].size_in_bytes);
	std::fill(vMemBuf.begin(), vMemBuf.end(), 0xFF);
	const wchar_t *path = &du_header.at(0).image_file_path[0];
	std::ifstream input(path);
	std::string data_str, temp_str;
	const char *Row_1 = "#1";
	const char *Row_2 = "#2";
	const char *Row_3 = "#3";
	const char *Row_4 = "#4";
	const char *Row_5 = "#5";
	const char *Row_6 = "#6";
	const char *Row_7 = "#7";
	const char *Row_8 = "#8";
	const char *Row_9 = "#9";
	const char *Row_A = "#A";
	const char *Row_B = "#B";
	const char *Row_C = "#C";
	const char *Row_D = "#D";
	const char *Row_E = "#E";
	FILE *hFile = NULL;
	hFile = _wfopen(path, _T("r"));
	unsigned char data;
	unsigned int file_length = 0;
	do{
		std::getline(input, temp_str);
		if (temp_str.length() == 0)
			goto end;
		data_str = temp_str;
		unsigned int temp_length = temp_str.length();
		temp_str.erase(temp_length - 33, temp_length);
		data_str.erase(0, 3);
		if ((strcmp(temp_str.c_str(), Row_1) == 0)
			|| (strcmp(temp_str.c_str(), Row_2) == 0)
			|| (strcmp(temp_str.c_str(), Row_3) == 0)
			|| (strcmp(temp_str.c_str(), Row_4) == 0)
			|| (strcmp(temp_str.c_str(), Row_5) == 0)
			|| (strcmp(temp_str.c_str(), Row_6) == 0)
			|| (strcmp(temp_str.c_str(), Row_7) == 0)
			|| (strcmp(temp_str.c_str(), Row_8) == 0)
			|| (strcmp(temp_str.c_str(), Row_9) == 0)
			|| (strcmp(temp_str.c_str(), Row_A) == 0)
			|| (strcmp(temp_str.c_str(), Row_B) == 0)
			|| (strcmp(temp_str.c_str(), Row_C) == 0)
			|| (strcmp(temp_str.c_str(), Row_D) == 0)
			|| (strcmp(temp_str.c_str(), Row_E) == 0)
			)
		{
			unsigned int data_length = data_str.length();
			for (int i = 0; i < (data_length / 2); i++)
			{
				temp_str = data_str;
				unsigned int temp_length = temp_str.length();
				temp_str.erase(2, temp_length);
				data = strtol(temp_str.c_str(), NULL, 16);
				memcpy(&vMemBuf.at(file_length), &data, sizeof(data));
				data_str.erase(0, 2);
				file_length += 1;
			}
		}
	} while (!feof(hFile));
end:
	du_header.at(0).program_length = file_length;
	unsigned short cks_cmp = 0x00;
	unsigned short checksum = CRC16_CCITT(&vMemBuf.at(0), file_length - 2);
	memcpy(&cks_cmp, &vMemBuf[file_length - 2], 2);
	vm_write(buff, &vMemBuf.at(0), du_header.at(0).program_length);
	if ((memcmp(&cks_cmp, &checksum, sizeof(checksum))) != 0)
	{
		*chip_checksum = 0x0000;
		MessageBox(NULL, L"Checksum Different!!!!!, Please check the file.", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
		return 1;
	}
	if ((memcmp(&cks_cmp, &checksum, sizeof(checksum))) == 0)
	{
		*chip_checksum = checksum;
		MessageBox(NULL, L"Please refer to Chip-Checksum.", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
		return 0;
	}
}