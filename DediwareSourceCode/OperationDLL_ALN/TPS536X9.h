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
void string_replace_TPSX9(string & strBig, const string & strsrc, const string &strdst)
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int TPS536X9_XMLanalyze(
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
	const wchar_t *filecmp_str3 = L"ifsp";
	const wchar_t *filecmp_str4 = L"IFSP";

	if (wcscmp(path_t, filecmp_str1) != 0
		&& wcscmp(path_t, filecmp_str2) != 0
		&& wcscmp(path_t, filecmp_str3) != 0
		&& wcscmp(path_t, filecmp_str4) != 0)
	{
		MessageBox(NULL, L"The File be loaded is not in TI specific Format !", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
		return 0;
	}
	//MessageBox(NULL, L"Converting the TI-XML File", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
	std::vector<unsigned char> vMemBuf((unsigned int)chip->partition_info[0].size_in_bytes);
	std::fill(vMemBuf.begin(), vMemBuf.end(), 0xFF);
	const wchar_t *path = &du_header.at(0).image_file_path[0];
	std::ifstream input(path);
	std::string addr_str, data_str, cmp_str, end_str,temp_str;
	const char *end_point1 = "</ProjectData>";
	const char *end_point2 = "</MultiProjectsData>";
	const char *rail1_cmp = "Rail#1";
	const char *rail2_cmp = "Rail#2";
	const char *addr_B0 = "176";	//2
	const char *addr_B1 = "177";	//3
	const char *addr_B2 = "178"; //4
	const char *addr_B3 = "179"; //5
	const char *addr_B4 = "180"; //6
	const char *addr_B5 = "181"; //7
	const char *addr_B6 = "182"; //8
	const char *addr_B7 = "183"; //9
	const char *addr_B8 = "184"; //10
	const char *addr_B9 = "185"; //11
	const char *addr_BA = "186"; //12
	const char *addr_BB = "187"; //13
	const char *addr_BC = "188"; //13
	const char *addr_24 = "36"; //14 & 15

	FILE *hFile = NULL;
	hFile = _wfopen(path, _T("r"));
	unsigned int addr = 0;
	unsigned int datalength = 0;
	unsigned int checksum = 0;
	short int data1 = 0xFFFF;	//word format
	char data2 = 0xFF;	//byte format
	unsigned int str_length;
	int i, j;
	do{
		do{
			std::getline(input, addr_str);
			if ((strcmp(addr_str.c_str(), end_point1)) == 0
				|| (strcmp(addr_str.c_str(), end_point2)) == 0 )
				goto end;
			//addr_str.erase(0, 10);
			string_replace_TPSX9(addr_str, " ", "");
			i = addr_str.find("<Code>");
		} while (i != 0);
		//addr_str.erase(0, 6);	//<Code>20</Code>
		//
		//addr_str.erase(str_length - 7, str_length);
		string_replace_TPSX9(addr_str, "<Code>", "");
		string_replace_TPSX9(addr_str, "</Code>", "");
		string_replace_TPSX9(addr_str, "\r", "");
		string_replace_TPSX9(addr_str, "\n", "");
		str_length = addr_str.length();
		//-----------------------0xB0 ~ 0xBC-----------------------
		if (
			(strcmp(addr_str.c_str(), addr_B0)) == 0
			|| (strcmp(addr_str.c_str(), addr_B1)) == 0
			|| (strcmp(addr_str.c_str(), addr_B2)) == 0
			|| (strcmp(addr_str.c_str(), addr_B3)) == 0
			|| (strcmp(addr_str.c_str(), addr_B4)) == 0
			|| (strcmp(addr_str.c_str(), addr_B5)) == 0
			|| (strcmp(addr_str.c_str(), addr_B6)) == 0
			|| (strcmp(addr_str.c_str(), addr_B7)) == 0
			|| (strcmp(addr_str.c_str(), addr_B8)) == 0
			|| (strcmp(addr_str.c_str(), addr_B9)) == 0
			|| (strcmp(addr_str.c_str(), addr_BA)) == 0
			|| (strcmp(addr_str.c_str(), addr_BB)) == 0
			|| (strcmp(addr_str.c_str(), addr_BC)) == 0
			)
		{
			//Find data <IDAndCode>
			do{
				std::getline(input, temp_str);
				string_replace_TPSX9(temp_str, "\n", "");
				string_replace_TPSX9(temp_str, "\r", "");
				string_replace_TPSX9(temp_str, " ", "");
				i = temp_str.find("<IDAndCode>");
			} while (i != 0);
			string_replace_TPSX9(temp_str, "<IDAndCode>", "");
			string_replace_TPSX9(temp_str, "</IDAndCode>", "");
			str_length = temp_str.length();
			temp_str.erase(0, str_length - 7);
			str_length = temp_str.length();
			temp_str.erase(str_length-1, str_length);
			if ((strcmp(temp_str.c_str(), rail1_cmp)) == 0)
			{
				do {
					std::getline(input, data_str);
					//data_str.erase(0, 10);
					string_replace_TPSX9(data_str, " ", "");
					i = data_str.find("<ValueText>");
				} while (i != 0);
				string_replace_TPSX9(data_str, "<ValueText>", "");
				string_replace_TPSX9(data_str, "</ValueText>", "");
				//////////6 bytes /////////////
				for (int k = 0; k < 12;)
				{
					temp_str = data_str;
					str_length = temp_str.length();
					temp_str.erase(str_length + k - 10, str_length);
					str_length = data_str.length();
					data_str.erase(0, str_length + k - 10);
					data2 = strtol(temp_str.c_str(), NULL, 16);
					memcpy(&vMemBuf.at(addr), &data2, sizeof(data2));
					addr += 1;
					k += 2;
					checksum += data2;
				}
				datalength += 6;
			}
		}
		// 0x24
		if ((strcmp(addr_str.c_str(), addr_24)) == 0)
		{
			//Find data <IDAndCode>
			do{
				std::getline(input, temp_str);
				string_replace_TPSX9(temp_str, "\n", "");
				string_replace_TPSX9(temp_str, "\r", "");
				string_replace_TPSX9(temp_str, " ", "");
				i = temp_str.find("<IDAndCode>");
			} while (i != 0);
			string_replace_TPSX9(temp_str, "<IDAndCode>", "");
			string_replace_TPSX9(temp_str, "</IDAndCode>", "");
			str_length = temp_str.length();
			temp_str.erase(0, str_length - 7);
			str_length = temp_str.length();
			temp_str.erase(str_length - 1, str_length);
			// Reg 0x24 Rail1 or Rail2
			if ((strcmp(temp_str.c_str(), rail1_cmp)) == 0 || (strcmp(temp_str.c_str(), rail2_cmp)) == 0)
			{
				do {
					std::getline(input, data_str);
					string_replace_TPSX9(data_str, " ", "");
					i = data_str.find("<ValueEncoded");
				} while (i != 0);
				str_length = data_str.length();
				data_str.erase(0, str_length - 7);
				str_length = data_str.length();
				data_str.erase(str_length - 3, str_length);
				data1 = strtol(data_str.c_str(), NULL, 16);
				memcpy(&vMemBuf.at(addr), &data1, sizeof(data1));
				addr += 2;
				//Fix 3 word
				short int fix3word = 0xFFFF;
				memcpy(&vMemBuf.at(addr), &fix3word, sizeof(fix3word));
				addr += 2;
				memcpy(&vMemBuf.at(addr), &fix3word, sizeof(fix3word));
				addr += 2;
				checksum += data1;
			}
			datalength += 6;
		}
	} while (!feof(hFile));
end:
	*chip_checksum = checksum&0xFFFF;
	if ((checksum == 0x0) || (datalength<(unsigned int)chip->partition_info[0].size_in_bytes))
	{
		chip_checksum = 0x0000;
		MessageBox(NULL, L"Please Check the File", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
		return -1;
	}
	du_header.at(0).program_length = datalength;
	vm_write(buff, &vMemBuf.at(0), du_header.at(0).program_length);
	MessageBox(NULL, L"Please refer to Chip-Checksum", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
	return 0;
}

