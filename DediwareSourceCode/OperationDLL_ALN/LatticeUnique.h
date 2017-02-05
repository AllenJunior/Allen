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
#define MAX_LINE_SIZE 512
using namespace std;
void string_replace(string & strBig, const string & strsrc, const string &strdst)
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
unsigned int JEDformat1_checksum(char *temp_str, unsigned int temp_len, FILE *hFile)
{
	std::string crc_str;
	char tempdata[8];
	unsigned int checksum = 0;
	//unsigned int checksum = 0;
	do{
		if (temp_str[0] == 'C')
		{
			goto end_crc;
		}
		if ((temp_str[0] == 'L') && (temp_str[temp_len - 1] == '*'))
		{
			temp_str[temp_len - 1] = '\0';
			for (int i = 0; i < 16; i++)
			{
				if (temp_str[i] == ' ')
				{
					std::string data_strLine(temp_str);
					data_strLine.erase(0, (i + 1));
					crc_str += data_strLine;
					break;
				}
			}
		}
		fgets(temp_str, MAX_LINE_SIZE, hFile);
	} while (!feof(hFile));
end_crc:
	unsigned int crc_strlen = crc_str.length();
	//caulate the string length if(Line_len / 16 != 0)
	unsigned int insert_length = (crc_strlen % 8) ? (8 - crc_strlen % 8) : 0;
	for (int i = 0; i < insert_length; i++)
	{
		crc_str.insert(crc_str.end(), _T('0'));
	}
	crc_strlen = crc_str.length();
	//Caculate CRC
	for (int i = 0; i < (crc_strlen / 8); i++)
	{
		strncpy(tempdata, crc_str.c_str(), 8);
		unsigned char data = strtol(tempdata, NULL, 2);
		checksum += MsbToLsb(data);
		crc_str.erase(0, 8);
	}
	checksum &= 0xFFFF;
	return checksum;
}
unsigned int JEDformat2_checksum(char *temp_str, unsigned int temp_len, FILE *hFile)
{
	std::string crc_str;
	char tempdata[8];
	unsigned int checksum = 0;
	do{
		if (temp_str[0] == 'C')
		{
			goto end_crc;
		}
		if (temp_str[0] == '*')
		{
			fgets(temp_str, MAX_LINE_SIZE, hFile);
			break;
		}
		temp_len = strlen(temp_str) - 1;
		if (temp_str[temp_len] == '\n')
		{
			temp_str[temp_len] = '\0';
		}
		std::string temp_strline(temp_str);
		crc_str += temp_strline;
		fgets(temp_str, MAX_LINE_SIZE, hFile);
	} while (!feof(hFile));
end_crc:
	unsigned int crc_strlen = crc_str.length();
	unsigned int insert_length = (crc_strlen % 8) ? (8 - crc_strlen % 8) : 0;
	for (int i = 0; i < insert_length; i++)
	{
		crc_str.insert(crc_str.end(), _T('0'));
	}
	//Caculate CRC
	crc_strlen = crc_str.length();
	for (int i = 0; i < (crc_strlen / 8); i++)
	{
		strncpy(tempdata, crc_str.c_str(), 8);
		unsigned char data = strtol(tempdata, NULL, 2);
		checksum += MsbToLsb(data);
		crc_str.erase(0, 8);
	}
	checksum &= 0xFFFF;
	return checksum;
}
unsigned int JEDformat3_checksum(char *temp_str, unsigned int temp_len, FILE *hFile)
{
	char *Data_str = NULL;
	unsigned int checksum = 0;
	std::string crc_str;
	do {
		unsigned int Line_len = strlen(temp_str);	//temp_str Length -1 = pointer
		if ((temp_str[0] == 'N') || ((temp_str[0] == 'L') && (Line_len<10)))
		{
			fgets(temp_str, MAX_LINE_SIZE, hFile);
			continue;
		}
		if (temp_str[0] == 'C')
		{
			break;
		}

		///String Adding...
		if (temp_str[0] == 'L')
		{
			for (int i = 0; i < 10; i++)
			{
				//find " "
				if (temp_str[i] == ' ')
				{
					Data_str = temp_str + (i + 1);
					break;
				}
			}
		}
		else if (temp_str[0] == ' ')
		{
			Data_str = temp_str;
		}
		std::string temp_line(Data_str);
		string_replace(temp_line, "\n", "");
		string_replace(temp_line, "*", "");
		string_replace(temp_line, " ", "");
		string_replace(temp_line, "E", "");
		crc_str += temp_line;
		fgets(temp_str, MAX_LINE_SIZE, hFile);
	} while (!feof(hFile));
end_crc:
	unsigned int crcstr_len = crc_str.length();
	unsigned int insert_length = (crcstr_len % 8) ? (8 - crcstr_len % 8) : 0;
	for (int i = 0; i < insert_length; i++)
	{
		crc_str.insert(crc_str.end(), _T('0'));
	}
	crcstr_len = crc_str.length();
	//Caculate CRC
	for (int i = 0; i < (crcstr_len / 8); i++)
	{
		char tempdata[8];
		strncpy(tempdata, crc_str.c_str(), 8);
		unsigned char data = strtol(tempdata, NULL, 2);
		checksum += MsbToLsb(data);
		crc_str.erase(0, 8);
	}
	checksum &= 0xFFFF;
	return checksum;
}
int iCE40Feature(
	const struct chip_info_c                *chip,                  /* chip info structure */
	std::vector<struct loadfile_du>         &du_header,             /* du header */
	vm_ptr_t                                buff,
	loadfile_item                           **reg,
	unsigned long                           *chip_checksum,         /* loadfile --> chipchecksum */
	std::vector<struct loadfile_fcs>        &file_checksum          /* file checksum */
	)
{
	//MessageBox(NULL, L"Calculating .nvcm File Checksum", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
	loadfile_item *pStruct_Buffer;
	unsigned int reg_len = 4;
	pStruct_Buffer = (loadfile_item*)malloc(sizeof(loadfile_item)+reg_len);
	memset(pStruct_Buffer->buff, 0, reg_len);
	const wchar_t *path = &du_header.at(0).image_file_path[0];
	FILE *hFile = NULL;
	char temp_str[64] = { 0 };
	char *crc_str = NULL;
	char *security_str = NULL;
	unsigned int Line_len;
	hFile = _wfopen(path, _T("r"));
	if (hFile == NULL)
		return 0;
	//Catch NVCM Checksum
	do{
		if (fgets(temp_str, 64, hFile) == NULL)
			continue;
		// "\n ->\0"
		Line_len = strlen(temp_str) - 1;
		if (temp_str[Line_len] == '\n') // 換行
			temp_str[Line_len] = '\0';	// 空格
		if ((temp_str[0] == '#') && (temp_str[1] == 'D') && (temp_str[2] == 'C'))
			break;
	} while (!feof(hFile));
	crc_str = temp_str + 4;
	int CRCValue = strtol(crc_str, NULL, 16);
	*chip_checksum = CRCValue;
	/*
	//Security Check
	do
	{
		if (fgets(temp_str, 64, hFile) == NULL)
			continue;
		// "\n ->\0"
		Line_len = strlen(temp_str) - 1;
		if (temp_str[Line_len] == '\n') // 換行
			temp_str[Line_len] = '\0';	// 空格
		if ((temp_str[0] == '#') && (temp_str[1] == 'O') && (temp_str[2] == 'T'))
			break;
	} while (!feof(hFile));
	security_str = temp_str + 4;
	int SecurityValue = strtol(security_str, NULL, 16);
	*/
	unsigned int SecurityValue = 0;
	memcpy(pStruct_Buffer->buff, &SecurityValue, sizeof(SecurityValue));
	pStruct_Buffer->len = reg_len;
	*reg = pStruct_Buffer;
	MessageBox(NULL, L"Please refer to Chip-Checksum", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
	fclose(hFile);
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
	int pathlength = EndOfPath.length();
	EndOfPath.erase(0, pathlength - 4);
	const wchar_t *path_t = EndOfPath.c_str();
	const wchar_t *FileItem_CMP1 = L".jed";
	const wchar_t *FileItem_CMP2 = L".JED";
	if (wcscmp(path_t, FileItem_CMP1) != 0
		&& wcscmp(path_t, FileItem_CMP2) != 0
		)
	{
			MessageBox(NULL, L"The Path of File is too long !", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
			return 1;
	}
	//MessageBox(NULL, L"Calculating .JED File Checksum.", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
	const wchar_t *path = &du_header.at(0).image_file_path[0];
	std::ifstream input(path);
	std::string line, line2, Templine, crcvalue;
	FILE *hFile = NULL;
	hFile = _wfopen(path, _T("r"));
	unsigned int checksum = 0;
	////////////////////////For XO2 / XO3L Series Calculate with Buffer Data
	if (
		wcscmp(chip->description, L"ispMachXO2") == 0
		|| wcscmp(chip->description, L"ispMachXO3L") == 0
		)
	{
		std::vector<unsigned char> vMemBuf(du_header.at(0).program_length);
		std::fill(vMemBuf.begin(), vMemBuf.end(), 0xFF);
		vm_read((unsigned char*)&vMemBuf[0] + du_header.at(0).ptn_addr, buff + du_header.at(0).data_offset_in_buff, du_header.at(0).program_length);
		unsigned char CRCdata = 0x0;
		for (int a = 0; a < du_header.at(0).program_length; a++)
		{
			memcpy(&CRCdata, &vMemBuf[a], sizeof(CRCdata));
			checksum += MsbToLsb(CRCdata);
		}
		checksum &= 0xFFFF;
		///Find 'LXXXX, for skip information
		char temp_str[MAX_LINE_SIZE];
		do{
			if (fgets(temp_str, MAX_LINE_SIZE, hFile) == NULL)
				continue;
		} while ((temp_str[0] != 'L') && !feof(hFile));
		///Find 'CXXXX
		do{
			if (fgets(temp_str, MAX_LINE_SIZE, hFile) == NULL)
				continue;
		} while ((temp_str[0] != 'C') && !feof(hFile));
		////////////////////Find the "CXXXX"//////////////////////
		std:string crcvalue(temp_str);
		crcvalue.erase(0, 1);
		crcvalue.erase(4, 2);
		unsigned int checsum_cmp = strtol(crcvalue.c_str(), NULL, 16);
		if (checksum == 0x00)
		{
			*chip_checksum = 0x00;
			MessageBox(NULL, L"CheckSum Different !", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
			return -1;
		}
		if (memcmp(&checsum_cmp, &checksum, sizeof(checksum)) == 0x0)
		{
			*chip_checksum = checksum;
			MessageBox(NULL, L"Please refer to Chip-Checksum.", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
			return 0;
		}
		else if (memcmp(&checsum_cmp, &checksum, sizeof(checksum)) != 0x0)
		{
			*chip_checksum = 0x00;
			MessageBox(NULL, L"CheckSum Different !", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
			return -1;
		}
	}
	////////////////////////For LC4xxx / LCMXO / POWRXXX Caculate with catching the file string
	else if (
					wcscmp(chip->description, L"ispMach40xx") == 0
					|| wcscmp(chip->description, L"ispMACHXO") == 0
					|| wcscmp(chip->description, L"ispPAC") == 0
					//|| wcscmp(chip->description, L"M4A3") == 0
				)
	{
		char temp_str[MAX_LINE_SIZE];
		char Data_str[MAX_LINE_SIZE];
		unsigned int temp_len = 0;
		do{
			if (fgets(temp_str, MAX_LINE_SIZE, hFile) == NULL)
				continue;
		} while ((temp_str[0] != 'L') && !feof(hFile));
		temp_len = strlen(temp_str) - 1;
		//Format1 L0000 1010010111001*		
		if ((temp_str[0] == 'L') && (temp_str[temp_len - 1] == '*'))
		{
			checksum = JEDformat1_checksum(temp_str, temp_len, hFile);
		}
		//Format2 L00000 & Format3 L0000
		if ((temp_str[0] == 'L') && (temp_str[temp_len-1] != '*'))
		{
			fgets(temp_str, MAX_LINE_SIZE, hFile);
			temp_len = strlen(temp_str) - 1;
			if ((temp_str[0] =='0')|| (temp_str[0] =='1'))
			{
				checksum = JEDformat2_checksum(temp_str, temp_len, hFile);
			}
			/*
			//For M4A3
			else if ((temp_str[0] == ' ') || ((temp_str[1] == 'L') && (temp_len > 10)))
			{
				checksum = JEDformat3_checksum(temp_str, temp_len, hFile);				
			}
			*/
		}
		////////////////////Find the "CXXXX"//////////////////////
		string crcvalue(temp_str);
		crcvalue.erase(0, 1);
		crcvalue.erase(4, 2);
		unsigned int checsum_cmp = strtol(crcvalue.c_str(), NULL, 16);
		if (checksum == 0x00)
		{
			*chip_checksum = 0x00;
			MessageBox(NULL, L"CheckSum Different !", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
			return -1;
		}
		if (memcmp(&checsum_cmp, &checksum, sizeof(checksum)) == 0x0)
		{
			*chip_checksum = checksum;
			MessageBox(NULL, L"Please refer to  Chip-Checksum.", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
			return 0;
		}
		else if ((memcmp(&checsum_cmp, &checksum, sizeof(checksum)) != 0x0))
		{
			*chip_checksum = 0x00;
			MessageBox(NULL, L"CheckSum Different !", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
			return -1;
		}		
	}
}

int XOFeature(
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
	const wchar_t *FileItem_CMP1 = L".jed";
	const wchar_t *FileItem_CMP2 = L".JED";
	if (wcscmp(path_t, FileItem_CMP1) != 0
		&& wcscmp(path_t, FileItem_CMP2) != 0
		)
	{
		MessageBox(NULL, L"The Path of File is too long !", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
		return 0;
	}
	const wchar_t *path = &du_header.at(0).image_file_path[0];
	std::ifstream input(path);
	//std::string ucode, securstr;
	FILE *hFile = NULL;
	hFile = _wfopen(path, _T("r"));
	char temp_str[64];
	unsigned int security_value = 0x00;
	char usercode[4];
	unsigned int BufferLen = chip->partition_info[1].size_in_bytes;
	pStruct_Buffer = (loadfile_item*)malloc(sizeof(loadfile_item)+BufferLen);
	memset(pStruct_Buffer->buff, 0, BufferLen);
	//Find G
	do{
		if (fgets(temp_str, 64, hFile) == NULL)
			continue;
	} while ((temp_str[0] != 'G') && !feof(hFile));
	if (temp_str[1] == '0')
		security_value = 0x00;
	else if (temp_str[1] == '1')
		security_value = 0x01;
	//Find U
	do{
		if (fgets(temp_str, 64, hFile) == NULL)
		if (strlen(temp_str) == 0)
		{			
			break;
		}
			continue;
	} while ((temp_str[0] != 'U') && !feof(hFile));
	if ((temp_str[0] == 'U') && temp_str[1] == 'H')
	{
		std::string temp_line(temp_str);
		string_replace(temp_line, "*", "");
		string_replace(temp_line, "\n", "");
		string_replace(temp_line, "U", "");
		string_replace(temp_line, "H", "");
		char usercode[4];
		for (int i = 0; i < 4; i++)
		{
			char temp[2];
			strncpy(temp, temp_line.c_str(), 2);
			usercode[3 - i] = strtol(temp, NULL, 16);
			temp_line.erase(0, 2);
		}
		memcpy(pStruct_Buffer->buff, &usercode, sizeof(usercode));
		memcpy(pStruct_Buffer->buff + sizeof(usercode), &security_value, sizeof(security_value));
		pStruct_Buffer->len = BufferLen;
		*reg = pStruct_Buffer;
	}
	else if((temp_str[0] == 'U') && temp_str[1] == 'A')
	{
		char usercode[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
		unsigned int temp_len = strlen(temp_str) - 1;
		if (temp_str[temp_len] == '\n')
		{
			temp_str[temp_len] = '\0';
			if (temp_str[temp_len - 1] == '*')
			{
				temp_str[temp_len - 1] = '\0';
			}
		}
		temp_len = strlen(temp_str) - 2; //length - 2 "UA"
		for (int i = 0; i < temp_len; i++)
		{
			usercode[3 - i] = temp_str[2 + i];
		}
		memcpy(pStruct_Buffer->buff, &usercode, sizeof(usercode));
		memcpy(pStruct_Buffer->buff + sizeof(usercode), &security_value, sizeof(security_value));
		pStruct_Buffer->len = BufferLen;
		*reg = pStruct_Buffer;
	}
	else if ((temp_str[0] == 'U') && (temp_str[1] != 'A') && (temp_str[1] != 'H'))
		{
			char usercode[4];
			std::string temp_line(temp_str);
			string_replace(temp_line, "*", "");
			string_replace(temp_line, "\n", "");
			string_replace(temp_line, "U", "");
			char tempdata[8];
			unsigned int data_len = temp_line.length();
			for (int i = 0; i < (data_len / 8); i++)
			{
				strncpy(tempdata, temp_line.c_str(), 8);
				usercode[3 - i] = strtol(tempdata, NULL, 2);
				temp_line.erase(0, 8);
			}
			memcpy(pStruct_Buffer->buff, &usercode, sizeof(usercode));
			memcpy(pStruct_Buffer->buff + sizeof(usercode), &security_value, sizeof(security_value));
			pStruct_Buffer->len = BufferLen;
			*reg = pStruct_Buffer;
		}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			usercode[i] = 0xFF;
		}
		memcpy(pStruct_Buffer->buff, &usercode, sizeof(usercode));
		memcpy(pStruct_Buffer->buff + sizeof(usercode), &security_value, sizeof(security_value));
		pStruct_Buffer->len = BufferLen;
		*reg = pStruct_Buffer;
	}
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
		const wchar_t *FileItem_CMP1 = L".jed";
		const wchar_t *FileItem_CMP2 = L".JED";
		if (wcscmp(path_t, FileItem_CMP1) != 0
			&& wcscmp(path_t, FileItem_CMP2) != 0
			)
		{
			MessageBox(NULL, L"The Path of File is too long !", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
			return 0;
		}
		const wchar_t *path = &du_header.at(0).image_file_path[0];
		std::ifstream input(path);
		//std::string rowstr, bitstr, ucode,securstr;
		FILE *hFile = NULL;
		hFile = _wfopen(path, _T("r"));
		char temp_str[72];
		char Fearture_Row[8] = { 0 };
		char Fearture_Bit[4] = { 0 };
		char usercode[4];
		unsigned int temp_len = 0;
		unsigned int security_value;
		//unsigned char regdata[20]; //16
		//int r, p, q, w, s, t;
		unsigned int BufferLen = chip->partition_info[1].size_in_bytes;
		pStruct_Buffer = (loadfile_item*)malloc(sizeof(loadfile_item)+BufferLen);
		memset(pStruct_Buffer->buff, 0, BufferLen);
		//Find G
		do{
			if (fgets(temp_str, 64, hFile) == NULL)
				continue;
		} while ((temp_str[0] != 'G') && !feof(hFile));
		if (temp_str[1] == '0')
			security_value = 0x00;
		else if (temp_str[1] == '1')
			security_value = 0x01;
		//Find E
		do{
			if (fgets(temp_str, 72, hFile) == NULL)
				continue;
		} while ((temp_str[0] != 'E') && !feof(hFile));
		char tempdata[8];
		//Feature Row
		std::string FeatureRow_str(temp_str);
		string_replace(FeatureRow_str, "E", "");
		string_replace(FeatureRow_str, "\n", "");
		temp_len = FeatureRow_str.length();
		for (int i = 0; i < (temp_len / 8); i++)
		{
			char tempdata[8];
			strncpy(tempdata, FeatureRow_str.c_str(), 8);
			Fearture_Row[i] = strtol(tempdata, NULL, 2);
			FeatureRow_str.erase(0, 8);
		}
		//Feature Bit
		fgets(temp_str, 64, hFile);
		std::string FeatureBit_str(temp_str);
		string_replace(FeatureBit_str, "*", "");
		string_replace(FeatureBit_str, "\n", "");
		temp_len = FeatureBit_str.length();
		for (int i = 0; i < (temp_len / 8); i++)
		{
			char tempdata[8];
			strncpy(tempdata, FeatureBit_str.c_str(), 8);
			Fearture_Bit[1-i] = strtol(tempdata, NULL, 2);
			FeatureBit_str.erase(0, 8);
		}
		//Find U
		do{
			if (fgets(temp_str, 64, hFile) == NULL)
			if (strlen(temp_str) == 0)
			{
				break;
			}
				continue;
		} while ((temp_str[0] != 'U') && !feof(hFile));
		if ((temp_str[0]== 'U') && (temp_str[1] == 'H'))
		{
			std::string temp_line(temp_str);
			string_replace(temp_line, "*", "");
			string_replace(temp_line, "\n", "");
			string_replace(temp_line, "U", "");
			string_replace(temp_line, "H", "");
			char usercode[4];
			for (int i = 0; i < 4; i++)
			{
				char temp[2];
				strncpy(temp, temp_line.c_str(), 2);
				usercode[3 - i] = strtol(temp, NULL, 16);
				temp_line.erase(0, 2);
			}
			memcpy(pStruct_Buffer->buff, &Fearture_Row, sizeof(Fearture_Row));
			memcpy(pStruct_Buffer->buff + sizeof(Fearture_Row), &Fearture_Bit, sizeof(Fearture_Bit));
			memcpy(pStruct_Buffer->buff + sizeof(Fearture_Row)+sizeof(Fearture_Bit), &usercode, sizeof(usercode));
			memcpy(pStruct_Buffer->buff + sizeof(Fearture_Row)+sizeof(Fearture_Bit)+sizeof(usercode), &security_value, sizeof(security_value));
			pStruct_Buffer->len = BufferLen;
			*reg = pStruct_Buffer;
		}
		else if((temp_str[0] == 'U') && (temp_str[1] == 'A'))
		{
			char usercode[4] = {0xFF,0xFF,0xFF,0xFF};
			unsigned int temp_len = strlen(temp_str) - 1;
			if (temp_str[temp_len] == '\n')
			{
				temp_str[temp_len] = '\0';
				if (temp_str[temp_len-1] == '*')
				{
					temp_str[temp_len - 1] = '\0';
				}
			}
			temp_len = strlen(temp_str) - 2; //length - 2 "UA"
			for (int i = 0; i < temp_len; i++)
			{
				usercode[3 - i] = temp_str[2 + i];
			}
			memcpy(pStruct_Buffer->buff, &Fearture_Row, sizeof(Fearture_Row));
			memcpy(pStruct_Buffer->buff + sizeof(Fearture_Row), &Fearture_Bit, sizeof(Fearture_Bit));
			memcpy(pStruct_Buffer->buff + sizeof(Fearture_Row)+sizeof(Fearture_Bit), &usercode, sizeof(usercode));
			memcpy(pStruct_Buffer->buff + sizeof(Fearture_Row)+sizeof(Fearture_Bit)+sizeof(usercode), &security_value, sizeof(security_value));
			pStruct_Buffer->len = BufferLen;
			*reg = pStruct_Buffer;
		}
		else if ((temp_str[0] == 'U') && (temp_str[1] != 'A') && (temp_str[1] != 'H'))
		{
			char usercode[4];
			std::string temp_line(temp_str);
			string_replace(temp_line, "*", "");
			string_replace(temp_line, "\n", "");
			string_replace(temp_line, "U", "");
			char tempdata[8];
			unsigned int data_len = temp_line.length();
			for (int i = 0; i < (data_len / 8); i++)
			{
				strncpy(tempdata, temp_line.c_str(), 8);
				usercode[3 - i] = strtol(tempdata, NULL, 2);
				temp_line.erase(0, 8);
			}
			memcpy(pStruct_Buffer->buff, &Fearture_Row, sizeof(Fearture_Row));
			memcpy(pStruct_Buffer->buff + sizeof(Fearture_Row), &Fearture_Bit, sizeof(Fearture_Bit));
			memcpy(pStruct_Buffer->buff + sizeof(Fearture_Row)+sizeof(Fearture_Bit), &usercode, sizeof(usercode));
			memcpy(pStruct_Buffer->buff + sizeof(Fearture_Row)+sizeof(Fearture_Bit)+sizeof(usercode), &security_value, sizeof(security_value));
			pStruct_Buffer->len = BufferLen;
			*reg = pStruct_Buffer;
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				usercode[i] = 0xFF;
			}
			memcpy(pStruct_Buffer->buff, &Fearture_Row, sizeof(Fearture_Row));
			memcpy(pStruct_Buffer->buff + sizeof(Fearture_Row), &Fearture_Bit, sizeof(Fearture_Bit));
			memcpy(pStruct_Buffer->buff + sizeof(Fearture_Row)+sizeof(Fearture_Bit), &usercode, sizeof(usercode));
			memcpy(pStruct_Buffer->buff + sizeof(Fearture_Row)+sizeof(Fearture_Bit)+sizeof(usercode), &security_value, sizeof(security_value));
			pStruct_Buffer->len = BufferLen;
			*reg = pStruct_Buffer;
		}
	}

	///OLD
		/*
		{
			char byte[8];
			char Frow[8];
			char Fbit[4];
			char Ucode[4];
			char Security[4];
			unsigned char regdata[20]; //16
			int r, p, q, w, s, t;
			unsigned int BufferLen = chip->partition_info[1].size_in_bytes;
			pStruct_Buffer = (loadfile_item*)malloc(sizeof(loadfile_item)+BufferLen);
			memset(pStruct_Buffer->buff, 0, BufferLen);
			unsigned int BufferLen = 20; //16
			pStruct_Buffer = (loadfile_item*)malloc(sizeof(loadfile_item)+BufferLen);
			memset(pStruct_Buffer->buff, 0, BufferLen);
			//Find the Security bit
			do {
				std::getline(input, securstr);
				s = securstr.find("G");
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
				r = rowstr.find("E");
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
				q = ucode.find("U");
				r = ucode.length();
				if (r = 0)
				{
					break;
				}
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
		*/



	