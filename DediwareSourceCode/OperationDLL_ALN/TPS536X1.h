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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int TPS536X1_XMLanalyze(
	const struct chip_info_c                *chip,                  /* chip info structure */
	std::vector<struct loadfile_du>         &du_header,             /* du header */
	vm_ptr_t                                buff,
	loadfile_item                           **reg,
	unsigned long                           *chip_checksum,         /* loadfile --> chipchecksum */
	std::vector<struct loadfile_fcs>        &file_checksum,          /* file checksum */
	const struct loadfile_funcs				*funcs
	)
{
	wchar_t path_str = du_header.at(0).image_file_path[0];
	std::wstring filename_str = &du_header.at(0).image_file_path[0];
	int path_length = filename_str.length();
	filename_str.erase(0, path_length - 4);
	const wchar_t *path_t = filename_str.c_str();
	const wchar_t *filecmp_str1 = L".xml";
	const wchar_t *filecmp_str2 = L".XML";
	if (wcscmp(path_t, filecmp_str1) != 0
		&& wcscmp(path_t, filecmp_str2) != 0)
	{
		MessageBox(NULL, L"The File be loaded is not in TI XML Format !", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
		return 0;
	}
	//MessageBox(NULL, L"Converting the TI-XML File", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
	std::vector<unsigned char> vMemBuf((unsigned int)chip->partition_info[0].size_in_bytes);
	std::fill(vMemBuf.begin(), vMemBuf.end(), 0xFF);
	const wchar_t *path = &du_header.at(0).image_file_path[0];
	std::ifstream input(path);
	std::string temp1_str, temp2_str, end_str;
	const char *end_point = "</ProjectData>";
	FILE *hFile = NULL;
	hFile = _wfopen(path, _T("r"));
	int addr;
	short int  data = 0xFFFF;
	int i, j, k;
	unsigned int str_length;
	unsigned int checksum = 0;
	//Find addr
	do{
		do{
			std::getline(input, temp1_str);
			if ((strcmp(temp1_str.c_str(), end_point)) == 0)
				goto end;
			temp1_str.erase(0, 10);
			i = temp1_str.find("<Code>");
		} while (i != 0);
		temp1_str.erase(0, 6);	//<Code>32</Code>
		str_length = temp1_str.length();
		temp1_str.erase(str_length - 7, str_length);
		addr = strtol(temp1_str.c_str(), NULL, 10);  // Dec to Hex
		//Find data
		do {
			std::getline(input, temp2_str);
			temp2_str.erase(0, 10);
			i = temp2_str.find("<ValueEncoded");
		} while (i != 0);
		str_length = temp2_str.length();
		temp2_str.erase(0, 42);
		str_length = temp2_str.length();
		temp2_str.erase(str_length - 4, str_length);
		data = strtol(temp2_str.c_str(), NULL, 16);
		memcpy(&vMemBuf.at((addr * 2) - 2), &data, sizeof(data));
		checksum += data;
	} while (!feof(hFile));
end:
	*chip_checksum = checksum & 0xFFFF;
	if (checksum == 0x0)
	{
		MessageBox(NULL, L"Please Check the File", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
		return -1;
	}
	du_header.at(0).program_length = (unsigned int)chip->partition_info[0].size_in_bytes;
	vm_write(buff, &vMemBuf.at(0), du_header.at(0).program_length);
	MessageBox(NULL, L"Please refer to Chip-Checksum", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
	return 0;
}
