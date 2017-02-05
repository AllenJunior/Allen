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
using namespace std;
void string_replace_LP(string & strBig, const string & strsrc, const string &strdst)
{
	string::size_type pos = 0;
	string::size_type srclen = strsrc.size();
	string::size_type dstlen = strdst.size();
	while ((pos = strBig.find(strsrc, pos)) != string::npos)
	{
		strBig.replace(pos, srclen, strdst);
		pos += dstlen;
	}
}
int LP8860_EEPFile(
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
	const wchar_t *FileItem_CMP1 = L".eep";
	const wchar_t *FileItem_CMP2 = L".EEP";
	if (wcscmp(path_t, FileItem_CMP1) != 0
		&& wcscmp(path_t, FileItem_CMP2) != 0)
	{
		MessageBox(NULL, L"Your File is not .eep file !!!", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
		return 0;
	}
	//MessageBox(NULL, L"Converting the TI .eep File", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
	std::vector<unsigned char> vMemBuf((unsigned int)chip->partition_info[0].size_in_bytes);
	std::fill(vMemBuf.begin(), vMemBuf.end(), chip->partition_info[0].blank_value);
	const wchar_t *path = &du_header.at(0).image_file_path[0];
	std::ifstream input(path);
	std::string addr_str, data_str,temp_str;
	const char *addr_60 = "60";
	const char *addr_61 = "61";
	const char *addr_62 = "62";
	const char *addr_63 = "63";
	const char *addr_64 = "64";
	const char *addr_65 = "65";
	const char *addr_66 = "66";
	const char *addr_67 = "67";
	const char *addr_68 = "68";
	const char *addr_69 = "69";
	const char *addr_6A = "6A";
	const char *addr_6B = "6B";
	const char *addr_6C = "6C";
	const char *addr_6D = "6D";
	const char *addr_6E = "6E";
	const char *addr_6F = "6F";
	const char *addr_70 = "70";
	const char *addr_71 = "71";
	const char *addr_72 = "72";
	const char *addr_73 = "73";
	const char *addr_74 = "74";
	const char *addr_75 = "75";
	const char *addr_76 = "76";
	const char *addr_77 = "77";
	const char *addr_78 = "78";

	unsigned char addr = 0x00;
	unsigned char data = 0x00;
	unsigned int checksum = 0x00;
	unsigned int file_length = 0;
	unsigned int str_length = 0;
	//Analyzing the file
	for (int cnt = 0; cnt < (du_header.at(0).program_length / 2); cnt++)
	{
		std::getline(input, temp_str);
		if (temp_str.length() == NULL)
		{
			break;
		}
		string_replace_LP(temp_str, " ", "");
		addr_str = temp_str;
		data_str = temp_str;
		//Address
		str_length = addr_str.length();
		addr_str.erase(str_length - 2, str_length);
		//Data
		str_length = data_str.length();
		data_str.erase(0, str_length - 2);
		///////////Converting
		if (	(strcmp(addr_str.c_str(), addr_60) == 0 )
			|| (strcmp(addr_str.c_str(), addr_61) == 0)
			|| (strcmp(addr_str.c_str(), addr_62) == 0)
			|| (strcmp(addr_str.c_str(), addr_63) == 0)
			|| (strcmp(addr_str.c_str(), addr_64) == 0)
			|| (strcmp(addr_str.c_str(), addr_65) == 0)
			|| (strcmp(addr_str.c_str(), addr_66) == 0)
			|| (strcmp(addr_str.c_str(), addr_67) == 0)
			|| (strcmp(addr_str.c_str(), addr_68) == 0)
			|| (strcmp(addr_str.c_str(), addr_69) == 0)
			|| (strcmp(addr_str.c_str(), addr_6A) == 0)
			|| (strcmp(addr_str.c_str(), addr_6B) == 0)
			|| (strcmp(addr_str.c_str(), addr_6C) == 0)
			|| (strcmp(addr_str.c_str(), addr_6D) == 0)
			|| (strcmp(addr_str.c_str(), addr_6E) == 0)
			|| (strcmp(addr_str.c_str(), addr_6F) == 0)
			|| (strcmp(addr_str.c_str(), addr_70) == 0)
			|| (strcmp(addr_str.c_str(), addr_71) == 0)
			|| (strcmp(addr_str.c_str(), addr_72) == 0)
			|| (strcmp(addr_str.c_str(), addr_73) == 0)
			|| (strcmp(addr_str.c_str(), addr_74) == 0)
			|| (strcmp(addr_str.c_str(), addr_75) == 0)
			|| (strcmp(addr_str.c_str(), addr_76) == 0)
			|| (strcmp(addr_str.c_str(), addr_77) == 0)
			|| (strcmp(addr_str.c_str(), addr_78) == 0)
			)
		{
			//Load Data
			data = strtol(data_str.c_str(), NULL, 16);
			memcpy(&vMemBuf.at(file_length), &data, sizeof(data));
			file_length += 1;
			//Load Fix
			char fix = 0xFF;
			memcpy(&vMemBuf.at(file_length), &fix, sizeof(fix));
			file_length += 1;
			checksum += data;
		}
	}
end:
	du_header.at(0).program_length = file_length;  
	vm_write(buff, &vMemBuf.at(0), du_header.at(0).program_length);
	checksum &= 0xFFFF;
	if (checksum == 0x0)
	{
		MessageBox(NULL, L"Please Check the File", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
		return -1;
	}
	*chip_checksum = checksum;
	MessageBox(NULL, L"Please refer to  Chip-Checksum", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
	return 0;
}
