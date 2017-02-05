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
int ComancheFile(
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
	const wchar_t *FileItem_CMP1 = L".txt";
	const wchar_t *FileItem_CMP2 = L".TXT";
	if (wcscmp(path_t, FileItem_CMP1) != 0
		&& wcscmp(path_t, FileItem_CMP2) != 0)
	{
		MessageBox(NULL, L"Your File is not .TXT file !!!", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
		return 0;
	}
	//MessageBox(NULL, L"Converting the IOR .txt File", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
	std::vector<unsigned char> vMemBuf((unsigned int)chip->partition_info[0].size_in_bytes);
	std::fill(vMemBuf.begin(), vMemBuf.end(), 0xFF);
	const wchar_t *path = &du_header.at(0).image_file_path[0];
	std::ifstream input(path);
	std::string addr_str, data_str, mask_str,temp_str;
	const char *mask_cmp = "00";
	const char *addr_10 = "10";
	const char *addr_11 = "11";
	const char *addr_12 = "12";
	const char *addr_13 = "13";
	const char *addr_14 = "14";
	const char *addr_15 = "15";
	const char *addr_16 = "16";
	const char *addr_17 = "17";
	const char *addr_18 = "18";
	const char *addr_19 = "19";
	const char *addr_1A = "1A";
	const char *addr_1B = "1B";
	const char *addr_1C = "1C";
	const char *addr_1D = "1D";
	const char *addr_1E = "1E";
	const char *addr_1F = "1F";
	const char *addr_20 = "20";
	const char *addr_21 = "21";
	const char *addr_22 = "22";
	const char *addr_23 = "23";
	const char *addr_24 = "24";
	const char *addr_25 = "25";
	const char *addr_26 = "26";
	const char *addr_27 = "27";
	const char *addr_28 = "28";
	const char *addr_29 = "29";
	const char *addr_2A = "2A";
	const char *addr_2B = "2B";
	const char *addr_2C = "2C";
	const char *addr_2D = "2D";
	const char *addr_2E = "2E";
	const char *addr_2F = "2F";
	const char *addr_30 = "30";
	const char *addr_31 = "31";
	const char *addr_32 = "32";
	const char *addr_33 = "33";
	const char *addr_34 = "34";
	const char *addr_35 = "35";
	const char *addr_36 = "36";
	const char *addr_37 = "37";
	const char *addr_38 = "38";
	const char *addr_39 = "39";
	const char *addr_3A = "3A";
	const char *addr_3B = "3B";
	const char *addr_3C = "3C";
	const char *addr_3D = "3D";
	const char *addr_3E = "3E";
	const char *addr_3F = "3F";
	const char *addr_40 = "40";
	const char *addr_41 = "41";
	const char *addr_42 = "42";
	const char *addr_43 = "43";
	const char *addr_44 = "44";
	const char *addr_45 = "45";
	const char *addr_46 = "46";
	const char *addr_47 = "47";
	const char *addr_48 = "48";
	const char *addr_49 = "49";
	const char *addr_4A = "4A";
	const char *addr_4B = "4B";
	const char *addr_4C = "4C";
	const char *addr_4D = "4D";
	const char *addr_4E = "4E";
	const char *addr_4F = "4F";
	const char *addr_50 = "50";
	const char *addr_51 = "51";
	const char *addr_52 = "52";
	const char *addr_53 = "53";
	const char *addr_54 = "54";
	const char *addr_55 = "55";
	const char *addr_56 = "56";
	const char *addr_57 = "57";
	const char *addr_58 = "58";
	const char *addr_59 = "59";
	const char *addr_5A = "5A";
	const char *addr_5B = "5B";
	const char *addr_5C = "5C";
	const char *addr_5D = "5D";
	const char *addr_5E = "5E";
	const char *addr_5F = "5F";
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
	const char *addr_79 = "79";
	const char *addr_7A = "7A";
	const char *addr_7B = "7B";
	const char *addr_7C = "7C";
	const char *addr_7D = "7D";
	const char *addr_7E = "7E";
	const char *addr_7F = "7F";
	const char *addr_80 = "80";
	const char *addr_81 = "81";
	const char *addr_82 = "82";
	const char *addr_83 = "83";
	const char *addr_84 = "84";
	const char *addr_85 = "85";
	const char *addr_86 = "86";
	const char *addr_87 = "87";
	const char *addr_88 = "88";
	const char *addr_89 = "89";
	const char *addr_8A = "8A";
	const char *addr_8B = "8B";
	const char *addr_8C = "8C";
	const char *addr_8D = "8D";
	const char *addr_8E = "8E";
	const char *addr_8F = "8F";
	const char *addr_90 = "90";

	unsigned char addr = 0x00;
	unsigned char data = 0x00;
	unsigned char mask = 0x00;
	unsigned int checksum = 0x00;
	unsigned int file_length = 0;
	unsigned int str_length = 0;
	//Analyzing the file
	for (int cnt = 0; cnt < (du_header.at(0).program_length / 3); cnt++)
	{
		std::getline(input, temp_str);
		if (temp_str.length() == NULL)
		{
			break;
		}
		addr_str = temp_str;
		data_str = temp_str;
		mask_str = temp_str;
		//Address
		str_length = addr_str.length();
		addr_str.erase(str_length - 6, str_length);
		//Data
		str_length = data_str.length();
		data_str.erase(str_length - 3, str_length);
		str_length = data_str.length();
		data_str.erase(0, str_length - 2);
		//Mask
		str_length = mask_str.length();
		mask_str.erase(0, str_length - 2);
		///////////Converting
		if (strcmp(addr_str.c_str(), addr_10) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_11) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_12) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_13) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_14) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_15) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_16) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_17) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_18) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_19) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_1A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_1B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_1C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_1D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_1E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_1F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_20) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_21) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_22) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_23) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_24) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_25) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_26) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_27) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_28) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_29) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_30) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_31) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_32) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_33) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_34) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_35) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_36) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_37) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_38) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_39) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_40) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_41) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_42) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_43) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_44) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_45) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_46) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_47) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_48) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_49) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_50) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_51) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_52) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_53) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_54) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_55) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_56) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_57) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_58) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_59) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_60) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_61) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_62) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_63) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_64) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_65) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_66) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_67) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_68) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_69) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_70) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_71) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_72) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_73) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_74) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_75) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_76) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_77) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_78) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_79) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_80) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_81) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_82) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_83) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_84) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_85) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_86) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_87) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_88) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_89) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_8A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_8B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_8C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_8D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_8E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_8F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_90) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			)
		{
			//Load Address
			addr = strtol(addr_str.c_str(), NULL, 16);
			memcpy(&vMemBuf.at(file_length), &addr, sizeof(addr));
			file_length += 1;
			//Load Data
			data = strtol(data_str.c_str(), NULL, 16);
			memcpy(&vMemBuf.at(file_length), &data, sizeof(data));
			file_length += 1;
			//Load Mask
			mask = strtol(mask_str.c_str(), NULL, 16);
			memcpy(&vMemBuf.at(file_length), &mask, sizeof(mask));
			file_length += 1;
			//Load Fix
			char fix = 0xFF;
			memcpy(&vMemBuf.at(file_length), &fix, sizeof(fix));
			file_length += 1;
			checksum += data;
		}
	}
end:
	du_header.at(0).program_length = file_length;  //length will be 0x1F8
	vm_write(buff, &vMemBuf.at(0), du_header.at(0).program_length);
	checksum &= 0xFFFF;
	if (checksum == 0x0)
	{
		MessageBox(NULL, L"Please Check the File", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
		return -1;
	}
	*chip_checksum = checksum;
	MessageBox(NULL, L"Please refer to Chip-Checksum", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
	return 0;
}

int BaxterFile(
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
	const wchar_t *FileItem_CMP1 = L".txt";
	const wchar_t *FileItem_CMP2 = L".TXT";
	if (wcscmp(path_t, FileItem_CMP1) != 0
		&& wcscmp(path_t, FileItem_CMP2) != 0)
	{
		MessageBox(NULL, L"Your File is not .TXT file !!!", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
		return 0;
	}
	//MessageBox(NULL, L"Converting the IOR .txt File", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
	std::vector<unsigned char> vMemBuf((unsigned int)chip->partition_info[0].size_in_bytes);
	std::fill(vMemBuf.begin(), vMemBuf.end(), 0xFF);
	const wchar_t *path = &du_header.at(0).image_file_path[0];
	std::ifstream input(path);
	std::string addr_str, data_str, mask_str, temp_str;
	const char *mask_cmp = "00";
	const char *addr_20 = "20";
	const char *addr_21 = "21";
	const char *addr_22 = "22";
	const char *addr_23 = "23";
	const char *addr_24 = "24";
	const char *addr_25 = "25";
	const char *addr_26 = "26";
	const char *addr_27 = "27";
	const char *addr_28 = "28";
	const char *addr_29 = "29";
	const char *addr_2A = "2A";
	const char *addr_2B = "2B";
	const char *addr_2C = "2C";
	const char *addr_2D = "2D";
	const char *addr_2E = "2E";
	const char *addr_2F = "2F";
	const char *addr_30 = "30";
	const char *addr_31 = "31";
	const char *addr_32 = "32";
	const char *addr_33 = "33";
	const char *addr_34 = "34";
	const char *addr_35 = "35";
	const char *addr_36 = "36";
	const char *addr_37 = "37";
	const char *addr_38 = "38";
	const char *addr_39 = "39";
	const char *addr_3A = "3A";
	const char *addr_3B = "3B";
	const char *addr_3C = "3C";
	const char *addr_3D = "3D";
	const char *addr_3E = "3E";
	const char *addr_3F = "3F";
	const char *addr_40 = "40";
	const char *addr_41 = "41";
	const char *addr_42 = "42";
	const char *addr_43 = "43";
	const char *addr_44 = "44";
	const char *addr_45 = "45";
	const char *addr_46 = "46";
	const char *addr_47 = "47";
	const char *addr_48 = "48";
	const char *addr_49 = "49";
	const char *addr_4A = "4A";
	const char *addr_4B = "4B";
	const char *addr_4C = "4C";
	const char *addr_4D = "4D";
	const char *addr_4E = "4E";
	const char *addr_4F = "4F";
	const char *addr_50 = "50";
	const char *addr_51 = "51";
	const char *addr_52 = "52";
	const char *addr_53 = "53";
	const char *addr_54 = "54";
	const char *addr_55 = "55";
	const char *addr_56 = "56";
	const char *addr_57 = "57";
	const char *addr_58 = "58";
	const char *addr_59 = "59";
	const char *addr_5A = "5A";
	const char *addr_5B = "5B";
	const char *addr_5C = "5C";
	const char *addr_5D = "5D";
	const char *addr_5E = "5E";
	const char *addr_5F = "5F";
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
	const char *addr_79 = "79";
	const char *addr_7A = "7A";
	const char *addr_7B = "7B";
	const char *addr_7C = "7C";
	const char *addr_7D = "7D";
	const char *addr_7E = "7E";
	const char *addr_7F = "7F";
	const char *addr_80 = "80";
	const char *addr_81 = "81";
	const char *addr_82 = "82";
	const char *addr_83 = "83";
	const char *addr_84 = "84";
	const char *addr_85 = "85";
	const char *addr_86 = "86";
	const char *addr_87 = "87";
	const char *addr_88 = "88";
	const char *addr_89 = "89";
	const char *addr_8A = "8A";
	const char *addr_8B = "8B";
	const char *addr_8C = "8C";
	const char *addr_90 = "90";
	const char *addr_91 = "91";
	const char *addr_92 = "92";
	const char *addr_93 = "93";
	const char *addr_94 = "94";
	const char *addr_95 = "95";
	const char *addr_96 = "96";
	const char *addr_97 = "97";
	const char *addr_98 = "98";
	const char *addr_99 = "99";
	const char *addr_9A = "9A";
	const char *addr_9B = "9B";
	const char *addr_9C = "9C";
	const char *addr_9D = "9D";
	const char *addr_9E = "9E";
	const char *addr_9F = "9F";
	const char *addr_A0 = "A0";
	const char *addr_A1 = "A1";
	const char *addr_A2 = "A2";
	const char *addr_A3 = "A3";

	unsigned char addr = 0x00;
	unsigned char data = 0x00;
	unsigned char mask = 0x00;
	unsigned int checksum = 0x00;
	unsigned int file_length = 0;
	unsigned int str_length = 0;

	for (int cnt = 0; cnt < (du_header.at(0).program_length / 3); cnt++)
	{
		std::getline(input, temp_str);
		if (temp_str.length() == NULL)
		{
			break;
		}
		addr_str = temp_str;
		data_str = temp_str;
		mask_str = temp_str;
		//Address
		str_length = addr_str.length();
		addr_str.erase(str_length - 6, str_length);
		//Data
		str_length = data_str.length();
		data_str.erase(str_length - 3, str_length);
		str_length = data_str.length();
		data_str.erase(0, str_length - 2);
		//Mask
		str_length = mask_str.length();
		mask_str.erase(0, str_length - 2);
		///////////Converting
		if (strcmp(addr_str.c_str(), addr_20) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_21) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_22) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_23) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_24) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_25) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_26) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_27) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_28) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_29) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_30) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_31) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_32) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_33) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_34) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_35) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_36) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_37) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_38) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_39) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_40) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_41) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_42) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_43) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_44) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_45) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_46) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_47) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_48) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_49) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_50) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_51) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_52) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_53) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_54) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_55) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_56) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_57) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_58) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_59) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_60) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_61) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_62) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_63) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_64) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_65) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_66) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_67) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_68) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_69) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_70) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_71) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_72) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_73) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_74) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_75) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_76) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_77) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_78) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_79) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_80) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_81) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_82) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_83) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_84) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_85) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_86) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_87) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_88) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_89) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_8A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_8B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_8C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_90) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_91) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_92) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_93) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_94) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_95) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_96) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_97) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_98) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_99) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_9A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_9B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_9C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_9D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_9E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_9F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_A0) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_A1) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_A2) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_A3) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			)
		{
			//Load Address
			addr = strtol(addr_str.c_str(), NULL, 16);
			memcpy(&vMemBuf.at(file_length), &addr, sizeof(addr));
			file_length += 1;
			//Load Data
			data = strtol(data_str.c_str(), NULL, 16);
			memcpy(&vMemBuf.at(file_length), &data, sizeof(data));
			file_length += 1;
			//Load Mask
			mask = strtol(mask_str.c_str(), NULL, 16);
			memcpy(&vMemBuf.at(file_length), &mask, sizeof(mask));
			file_length += 1;
			//Load Fix
			char fix = 0xFF;
			memcpy(&vMemBuf.at(file_length), &fix, sizeof(fix));
			file_length += 1;
			checksum += data;
		}
	}
end:
	du_header.at(0).program_length = file_length;	//length will be 0x1B0
	vm_write(buff, &vMemBuf.at(0), du_header.at(0).program_length);
	checksum &= 0xFFFF;
	if (checksum == 0x0)
	{
		MessageBox(NULL, L"Please Check the File", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
		return -1;
	}
	*chip_checksum = checksum;
	MessageBox(NULL, L"Please refer to Chip-Checksum", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
	return 0;
}

int SalemFile(
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
	const wchar_t *FileItem_CMP1 = L".txt";
	const wchar_t *FileItem_CMP2 = L".TXT";
	if (wcscmp(path_t, FileItem_CMP1) != 0
		&& wcscmp(path_t, FileItem_CMP2) != 0)
	{
		MessageBox(NULL, L"Your File is not .TXT file !!!", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
		return 0;
	}
	//MessageBox(NULL, L"Converting the IOR .txt File", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
	std::vector<unsigned char> vMemBuf((unsigned int)chip->partition_info[0].size_in_bytes);
	std::fill(vMemBuf.begin(), vMemBuf.end(), 0xFF);
	const wchar_t *path = &du_header.at(0).image_file_path[0];
	std::ifstream input(path);
	std::string addr_str, data_str, mask_str, temp_str;
	const char *mask_cmp = "00";
	const char *addr_24 = "24";
	const char *addr_25 = "25";
	const char *addr_26 = "26";
	const char *addr_27 = "27";
	const char *addr_28 = "28";
	const char *addr_29 = "29";
	const char *addr_2A = "2A";
	const char *addr_2B = "2B";
	const char *addr_2C = "2C";
	const char *addr_2D = "2D";
	const char *addr_2E = "2E";
	const char *addr_2F = "2F";
	const char *addr_30 = "30";
	const char *addr_31 = "31";
	const char *addr_32 = "32";
	const char *addr_33 = "33";
	const char *addr_34 = "34";
	const char *addr_35 = "35";
	const char *addr_36 = "36";
	const char *addr_37 = "37";
	const char *addr_38 = "38";
	const char *addr_39 = "39";
	const char *addr_3A = "3A";
	const char *addr_3B = "3B";
	const char *addr_3C = "3C";
	const char *addr_3D = "3D";
	const char *addr_3E = "3E";
	const char *addr_3F = "3F";
	const char *addr_40 = "40";
	const char *addr_41 = "41";
	const char *addr_42 = "42";
	const char *addr_43 = "43";
	const char *addr_44 = "44";
	const char *addr_45 = "45";
	const char *addr_46 = "46";
	const char *addr_47 = "47";
	const char *addr_48 = "48";
	const char *addr_49 = "49";
	const char *addr_4A = "4A";
	const char *addr_4B = "4B";
	const char *addr_4C = "4C";
	const char *addr_4D = "4D";
	const char *addr_4E = "4E";
	const char *addr_4F = "4F";
	const char *addr_50 = "50";
	const char *addr_51 = "51";
	const char *addr_52 = "52";
	const char *addr_53 = "53";
	const char *addr_54 = "54";
	const char *addr_55 = "55";
	const char *addr_56 = "56";
	const char *addr_57 = "57";
	const char *addr_58 = "58";
	const char *addr_59 = "59";
	const char *addr_5A = "5A";
	const char *addr_5B = "5B";
	const char *addr_5C = "5C";
	const char *addr_5D = "5D";
	const char *addr_5E = "5E";
	const char *addr_5F = "5F";
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
	const char *addr_79 = "79";
	const char *addr_7A = "7A";
	const char *addr_7B = "7B";
	const char *addr_7C = "7C";
	const char *addr_7D = "7D";
	const char *addr_7E = "7E";
	const char *addr_7F = "7F";
	const char *addr_80 = "80";
	const char *addr_81 = "81";
	const char *addr_82 = "82";
	const char *addr_83 = "83";
	const char *addr_84 = "84";
	const char *addr_85 = "85";
	const char *addr_86 = "86";
	const char *addr_87 = "87";
	const char *addr_88 = "88";
	const char *addr_89 = "89";
	const char *addr_8A = "8A";
	const char *addr_8B = "8B";
	const char *addr_8C = "8C";
	const char *addr_8D = "8D";
	const char *addr_8E = "8E";
	const char *addr_8F = "8F";
	const char *addr_90 = "90";
	const char *addr_91 = "91";
	const char *addr_92 = "92";
	const char *addr_93 = "93";
	const char *addr_94 = "94";
	const char *addr_95 = "95";
	const char *addr_96 = "96";
	const char *addr_98 = "98";
	const char *addr_99 = "99";
	const char *addr_9A = "9A";
	const char *addr_9B = "9B";
	const char *addr_9C = "9C";
	const char *addr_9D = "9D";
	const char *addr_9E = "9E";
	const char *addr_9F = "9F";
	const char *addr_A0 = "A0";
	const char *addr_A1 = "A1";
	const char *addr_A2 = "A2";
	const char *addr_A3 = "A3";
	const char *addr_A4 = "A4";
	const char *addr_A5 = "A5";

	unsigned char addr = 0x00;
	unsigned char data = 0x00;
	unsigned char mask = 0x00;
	unsigned int checksum = 0x00;
	unsigned int file_length = 0;
	unsigned int str_length = 0;

	for (int cnt = 0; cnt < (du_header.at(0).program_length / 3); cnt++)
	{
		std::getline(input, temp_str);
		if (temp_str.length() == NULL)
		{
			break;
		}
		addr_str = temp_str;
		data_str = temp_str;
		mask_str = temp_str;
		//Address
		str_length = addr_str.length();
		addr_str.erase(str_length - 6, str_length);
		//Data
		str_length = data_str.length();
		data_str.erase(str_length - 3, str_length);
		str_length = data_str.length();
		data_str.erase(0, str_length - 2);
		//Mask
		str_length = mask_str.length();
		mask_str.erase(0, str_length - 2);
		///////////Converting
		if (strcmp(addr_str.c_str(), addr_24) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_25) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_26) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_27) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_28) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_29) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_30) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_31) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_32) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_33) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_34) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_35) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_36) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_37) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_38) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_39) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_40) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_41) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_42) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_43) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_44) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_45) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_46) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_47) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_48) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_49) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_50) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_51) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_52) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_53) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_54) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_55) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_56) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_57) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_58) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_59) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_60) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_61) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_62) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_63) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_64) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_65) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_66) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_67) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_68) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_69) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_70) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_71) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_72) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_73) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_74) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_75) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_76) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_77) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_78) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_79) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_80) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_81) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_82) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_83) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_84) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_85) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_86) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_87) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_88) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_89) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_8A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_8B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_8C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_8D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_8E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_8F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_90) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_91) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_92) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_93) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_94) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_95) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_96) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_98) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_99) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_9A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_9B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_9C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_9D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_9E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_9F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_A0) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_A1) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_A2) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_A3) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_A4) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_A5) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			)
		{
			//Load Address
			addr = strtol(addr_str.c_str(), NULL, 16);
			memcpy(&vMemBuf.at(file_length), &addr, sizeof(addr));
			file_length += 1;
			//Load Data
			data = strtol(data_str.c_str(), NULL, 16);
			memcpy(&vMemBuf.at(file_length), &data, sizeof(data));
			file_length += 1;
			//Load Mask
			mask = strtol(mask_str.c_str(), NULL, 16);
			memcpy(&vMemBuf.at(file_length), &mask, sizeof(mask));
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
	MessageBox(NULL, L"Please refer to Chip-Checksum", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
	return 0;
}
int ManhattanFile(
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
	const wchar_t *FileItem_CMP1 = L".txt";
	const wchar_t *FileItem_CMP2 = L".TXT";
	if (wcscmp(path_t, FileItem_CMP1) != 0
		&& wcscmp(path_t, FileItem_CMP2) != 0)
	{
		MessageBox(NULL, L"Your File is not .TXT file !!!", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
		return 0;
	}
	//MessageBox(NULL, L"Converting the IOR .txt File", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
	std::vector<unsigned char> vMemBuf((unsigned int)chip->partition_info[0].size_in_bytes);
	std::fill(vMemBuf.begin(), vMemBuf.end(), 0xFF);
	const wchar_t *path = &du_header.at(0).image_file_path[0];
	std::ifstream input(path);
	std::string addr_str, data_str, mask_str, temp_str;
	const char *mask_cmp = "00";
	const char *addr_20 = "20";
	const char *addr_21 = "21";
	const char *addr_22 = "22";
	const char *addr_23 = "23";
	const char *addr_24 = "24";
	const char *addr_25 = "25";
	const char *addr_26 = "26";
	const char *addr_27 = "27";
	const char *addr_28 = "28";
	const char *addr_29 = "29";
	const char *addr_2A = "2A";
	const char *addr_2B = "2B";
	const char *addr_2C = "2C";
	const char *addr_2D = "2D";
	const char *addr_2E = "2E";
	const char *addr_2F = "2F";
	const char *addr_30 = "30";
	const char *addr_31 = "31";
	const char *addr_32 = "32";
	const char *addr_33 = "33";
	const char *addr_34 = "34";
	const char *addr_35 = "35";
	const char *addr_36 = "36";
	const char *addr_37 = "37";
	const char *addr_38 = "38";
	const char *addr_39 = "39";
	const char *addr_3A = "3A";
	const char *addr_3B = "3B";
	const char *addr_3C = "3C";
	const char *addr_3D = "3D";
	const char *addr_3E = "3E";
	const char *addr_3F = "3F";
	const char *addr_40 = "40";
	const char *addr_41 = "41";
	const char *addr_42 = "42";
	const char *addr_43 = "43";
	const char *addr_44 = "44";
	const char *addr_45 = "45";
	const char *addr_46 = "46";
	const char *addr_47 = "47";
	const char *addr_48 = "48";
	const char *addr_49 = "49";
	const char *addr_4A = "4A";
	const char *addr_4B = "4B";
	const char *addr_4C = "4C";
	const char *addr_4D = "4D";
	const char *addr_4E = "4E";
	const char *addr_4F = "4F";
	const char *addr_50 = "50";
	const char *addr_51 = "51";
	const char *addr_52 = "52";
	const char *addr_53 = "53";
	const char *addr_54 = "54";
	const char *addr_55 = "55";
	const char *addr_56 = "56";
	const char *addr_57 = "57";
	const char *addr_58 = "58";
	const char *addr_59 = "59";
	const char *addr_5A = "5A";
	const char *addr_5B = "5B";
	const char *addr_5C = "5C";
	const char *addr_5D = "5D";
	const char *addr_5E = "5E";
	const char *addr_5F = "5F";
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
	//const char *addr_6E = "6E";
	//const char *addr_6F = "6F";
	const char *addr_70 = "70";
	//const char *addr_71 = "71";
	//const char *addr_72 = "72";
	const char *addr_80 = "80";
	const char *addr_81 = "81";
	const char *addr_82 = "82";

	//FILE *hFile = NULL;
	//hFile = _wfopen(path, _T("r"));
	unsigned char addr = 0x00;
	unsigned char data = 0x00;
	unsigned char mask = 0x00;
	unsigned int checksum = 0x00;
	unsigned int file_length = 0;
	unsigned int str_length = 0;

	for (int cnt = 0; cnt < (du_header.at(0).program_length / 3); cnt++)
	{
		std::getline(input, temp_str);
		if (temp_str.length() == NULL)
		{
			break;
		}
		addr_str = temp_str;
		data_str = temp_str;
		mask_str = temp_str;
		//Address
		str_length = addr_str.length();
		addr_str.erase(str_length - 6, str_length);
		//Data
		str_length = data_str.length();
		data_str.erase(str_length - 3, str_length);
		str_length = data_str.length();
		data_str.erase(0, str_length - 2);
		//Mask
		str_length = mask_str.length();
		mask_str.erase(0, str_length - 2);
		///////////Converting
		if (strcmp(addr_str.c_str(), addr_20) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_21) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_22) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_23) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_24) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_25) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_26) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_27) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_28) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_29) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_30) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_31) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_32) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_33) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_34) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_35) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_36) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_37) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_38) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_39) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_40) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_41) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_42) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_43) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_44) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_45) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_46) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_47) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_48) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_49) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_50) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_51) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_52) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_53) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_54) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_55) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_56) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_57) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_58) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_59) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_60) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_61) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_62) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_63) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_64) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_65) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_66) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_67) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_68) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_69) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			//|| strcmp(addr_str.c_str(), addr_6E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			//|| strcmp(addr_str.c_str(), addr_6F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_70) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			//|| strcmp(addr_str.c_str(), addr_71) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			//|| strcmp(addr_str.c_str(), addr_72) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_80) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_81) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_82) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			)
		{
			//Load Address
			addr = strtol(addr_str.c_str(), NULL, 16);
			memcpy(&vMemBuf.at(file_length), &addr, sizeof(addr));
			file_length += 1;
			//Load Data
			data = strtol(data_str.c_str(), NULL, 16);
			memcpy(&vMemBuf.at(file_length), &data, sizeof(data));
			file_length += 1;
			//Load Mask
			mask = strtol(mask_str.c_str(), NULL, 16);
			memcpy(&vMemBuf.at(file_length), &mask, sizeof(mask));
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
	MessageBox(NULL, L"Please refer to Chip-Checksum", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
	return 0;

}

int LucasFile(
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
	const wchar_t *FileItem_CMP1 = L".txt";
	const wchar_t *FileItem_CMP2 = L".TXT";
	if (wcscmp(path_t, FileItem_CMP1) != 0
		&& wcscmp(path_t, FileItem_CMP2) != 0)
	{
		MessageBox(NULL, L"Your File is not .TXT file !!!", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
		return 0;
	}
	//MessageBox(NULL, L"Converting the IOR .txt File", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
	std::vector<unsigned char> vMemBuf((unsigned int)chip->partition_info[0].size_in_bytes);
	std::fill(vMemBuf.begin(), vMemBuf.end(), 0xFF);
	const wchar_t *path = &du_header.at(0).image_file_path[0];
	std::ifstream input(path);
	std::string addr_str, data_str, mask_str, temp_str;
	const char *mask_cmp = "00";
	const char *addr_18 = "18";
	const char *addr_19 = "19";
	const char *addr_1A = "1A";
	const char *addr_1B = "1B";
	const char *addr_1C = "1C";
	const char *addr_1D = "1D";
	const char *addr_1E = "1E";
	const char *addr_1F = "1F";
	const char *addr_20 = "20";
	const char *addr_21 = "21";
	const char *addr_22 = "22";
	const char *addr_23 = "23";
	const char *addr_24 = "24";
	const char *addr_25 = "25";
	const char *addr_26 = "26";
	const char *addr_27 = "27";
	const char *addr_28 = "28";
	const char *addr_29 = "29";
	const char *addr_2A = "2A";
	const char *addr_2B = "2B";
	const char *addr_2C = "2C";
	const char *addr_2D = "2D";
	const char *addr_2E = "2E";
	const char *addr_2F = "2F";
	const char *addr_30 = "30";
	const char *addr_31 = "31";
	const char *addr_32 = "32";
	const char *addr_33 = "33";
	const char *addr_34 = "34";
	const char *addr_35 = "35";
	const char *addr_36 = "36";
	const char *addr_37 = "37";
	const char *addr_38 = "38";
	const char *addr_39 = "39";
	const char *addr_3A = "3A";
	const char *addr_3B = "3B";
	const char *addr_3C = "3C";
	const char *addr_3D = "3D";
	const char *addr_3E = "3E";
	const char *addr_3F = "3F";
	const char *addr_40 = "40";
	const char *addr_41 = "41";
	const char *addr_42 = "42";
	const char *addr_43 = "43";
	const char *addr_44 = "44";
	const char *addr_45 = "45";
	const char *addr_46 = "46";
	const char *addr_47 = "47";
	const char *addr_48 = "48";
	const char *addr_49 = "49";
	const char *addr_4A = "4A";
	const char *addr_4B = "4B";
	const char *addr_4C = "4C";
	const char *addr_4D = "4D";
	const char *addr_4E = "4E";
	const char *addr_4F = "4F";
	const char *addr_50 = "50";
	const char *addr_51 = "51";
	const char *addr_52 = "52";
	const char *addr_53 = "53";
	const char *addr_54 = "54";
	const char *addr_55 = "55";
	const char *addr_56 = "56";
	const char *addr_57 = "57";
	const char *addr_58 = "58";
	const char *addr_59 = "59";
	const char *addr_5A = "5A";
	const char *addr_5B = "5B";
	const char *addr_5C = "5C";
	const char *addr_5D = "5D";
	const char *addr_5E = "5E";
	const char *addr_5F = "5F";
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
	const char *addr_78 = "78";
	const char *addr_79 = "79";
	const char *addr_7A = "7A";
	const char *addr_7B = "7B";
	const char *addr_7C = "7C";
	const char *addr_7D = "7D";
	const char *addr_7E = "7E";
	const char *addr_7F = "7F";
	const char *addr_80 = "80";
	const char *addr_81 = "81";
	const char *addr_82 = "82";
	const char *addr_83 = "83";
	const char *addr_84 = "84";
	const char *addr_85 = "85";
	const char *addr_86 = "86";
	const char *addr_87 = "87";
	const char *addr_88 = "88";
	const char *addr_89 = "89";
	const char *addr_8A = "8A";
	
	unsigned char addr = 0x00;
	unsigned char data = 0x00;
	unsigned char mask = 0x00;
	unsigned int checksum = 0x00;
	unsigned int file_length = 0;
	unsigned int str_length = 0;
	//Analyzing the file
	for (int cnt = 0; cnt < (du_header.at(0).program_length / 3); cnt++)
	{
		std::getline(input, temp_str);
		if (temp_str.length() == NULL)
		{
			break;
		}
		addr_str = temp_str;
		data_str = temp_str;
		mask_str = temp_str;
		//Address
		str_length = addr_str.length();
		addr_str.erase(str_length - 6, str_length);
		//Data
		str_length = data_str.length();
		data_str.erase(str_length - 3, str_length);
		str_length = data_str.length();
		data_str.erase(0, str_length - 2);
		//Mask
		str_length = mask_str.length();
		mask_str.erase(0, str_length - 2);
		///////////Converting
		if (strcmp(addr_str.c_str(), addr_18) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_19) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_1A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_1B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_1C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_1D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_1E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_1F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_20) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_21) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_22) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_23) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_24) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_25) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_26) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_27) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_28) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_29) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_2F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_30) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_31) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_32) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_33) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_34) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_35) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_36) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_37) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_38) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_39) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_3F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_40) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_41) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_42) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_43) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_44) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_45) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_46) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_47) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_48) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_49) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_4F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_50) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_51) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_52) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_53) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_54) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_55) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_56) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_57) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_58) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_59) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_5F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_60) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_61) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_62) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_63) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_64) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_65) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_66) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_67) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_68) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_69) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_6F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_70) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_71) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_72) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_73) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_74) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_75) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_76) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_78) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_79) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7B) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7C) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7D) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7E) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_7F) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_80) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_81) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_82) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_83) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_84) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_85) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_86) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_87) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_88) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_89) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			|| strcmp(addr_str.c_str(), addr_8A) == 0 && strcmp(mask_str.c_str(), mask_cmp) != 0
			)
		{
			//Load Address
			addr = strtol(addr_str.c_str(), NULL, 16);
			memcpy(&vMemBuf.at(file_length), &addr, sizeof(addr));
			file_length += 1;
			//Load Data
			data = strtol(data_str.c_str(), NULL, 16);
			memcpy(&vMemBuf.at(file_length), &data, sizeof(data));
			file_length += 1;
			//Load Mask
			mask = strtol(mask_str.c_str(), NULL, 16);
			memcpy(&vMemBuf.at(file_length), &mask, sizeof(mask));
			file_length += 1;
			//Load Fix
			char fix = 0xFF;
			memcpy(&vMemBuf.at(file_length), &fix, sizeof(fix));
			file_length += 1;
			checksum += data;
		}
	}
end:
	du_header.at(0).program_length = file_length;  //length will be 0x1F8
	vm_write(buff, &vMemBuf.at(0), du_header.at(0).program_length);
	checksum &= 0xFFFF;
	if (checksum == 0x0)
	{
		MessageBox(NULL, L"Please Check the File", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
		return -1;
	}
	*chip_checksum = checksum;
	MessageBox(NULL, L"Please refer to Chip-Checksum", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
	return 0;
}