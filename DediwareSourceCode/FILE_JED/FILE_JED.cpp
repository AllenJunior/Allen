// FILE_NVCM.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include <tchar.h>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include "FILE_JED.h"
#include "../loadfile_include/data_file.h"
#include "../error_no/error_no.h"
#include "../common/prog_info.h"
#include <assert.h>
/////////////////////////////////////////////////
#define MAX_LINE_SIZE 512
static unsigned char *g_JED_data = NULL;
static unsigned int   g_JED_size = 0;
static unsigned int   g_data_len = 0;
static unsigned long long g_ptn_addr = 0;
static unsigned long long g_file_offset = 0;
using namespace std;

std::string family_str;
unsigned char blank_value;
unsigned int g_column;
unsigned int g_row;
unsigned int g_jedec_size;
unsigned int g_chipinfo_chipsize;
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
ANALYS_FILE_API_C int analys_init(const struct chip_info_c *ChipInfo)
{
		char family[MAX_LINE_SIZE];
		wcstombs(family, ChipInfo->description, MAX_LINE_SIZE);
		std::string description_str(family);
		family_str = description_str;
		blank_value = (ChipInfo->partition_info[0].blank_value & 0x01);
		///Column number
		if ((ChipInfo->partition_info[0].program_parameter) != 0)
		{
			g_column =((((ChipInfo->partition_info[0].page_size_in_byte)/2) - 1) * 16) + ChipInfo->partition_info[0].program_parameter;
		}
		else if ((ChipInfo->partition_info[0].program_parameter) == 0)
		{
			g_column = (ChipInfo->partition_info[0].page_size_in_byte/2) * 16;
		}
		//Row number
		g_row = ChipInfo->partition_info[0].read_parameter;
		g_jedec_size = g_column*g_row;
		g_chipinfo_chipsize = ChipInfo->partition_info[0].size_in_bytes;
	return analys_release();
}
ANALYS_FILE_API_C int analys_file(const wchar_t *path, unsigned long long ptn_addr, unsigned long long file_offset, std::vector<unsigned char> *vAnalysRes)
{
	assert(path);
	assert(vAnalysRes);
	int err = E_OK; // E_OK = 0 = True;
	FILE *hFile = NULL;
	char Line_str[MAX_LINE_SIZE];
	char temp_str[MAX_LINE_SIZE];
	unsigned int Line_len = 0;
	unsigned int addr_index = 0;
	//Open vector
	std::vector<unsigned char> vMemBuf(g_jedec_size);
	std::fill(vMemBuf.begin(), vMemBuf.end(), blank_value);

	ANALYS_FILE_RES file_info;
	analys_release();
	vAnalysRes->clear();
	///To get file's informations
	_stati64	st;
	if (_tstat64(path, &st) == -1){
		err = E_FILE_NO_EXIST;
		goto file_end;
	}
	///To define the file size and build memory for data
	g_JED_size = (unsigned int)st.st_size;
	g_JED_data = (unsigned char*)malloc(g_JED_size);
	///Checking the memory range
	assert(g_JED_data);
	if (!g_JED_data)
	{
		err = E_ALLOC_MEMORY_FAIL;
		goto file_end;
	}
	//To open the file from the path
	hFile = _wfopen(path, _T("r"));
	//Check File empty???
	if (hFile == NULL)
	{
		err = E_OPEN_FILE_FAIL;
		goto file_end;
	}
	//Analyze JED File
	unsigned int addr = 0;
	unsigned file_length = 0;
	//Find the .JED File includes ( " L " )
	do{
		fgets(temp_str, MAX_LINE_SIZE, hFile);
		unsigned int temp_len = strlen(temp_str);
		if (((temp_str[0] == 'C') && (temp_len < 8))
			|| (temp_str[0] == 'E'))
		{
			break;
		}
		if ((temp_str[0] == 'N') && (temp_str[1] == 'O') && (temp_str[2] == 'T') && (temp_str[3] == 'E')
			&& (temp_str[5] == 'E') && (temp_str[6] == 'N') && (temp_str[7] == 'D')
			&& (temp_str[9] == 'C') && (temp_str[10] == 'O') && (temp_str[11] == 'N')
			&& (temp_str[12] == 'F') && (temp_str[13] == 'I') && (temp_str[14] == 'G'))
		{
			break;
		}
		if (
			((temp_str[0] == 'L') && (temp_len > 1))
			|| ((temp_str[0] == ' ') && (temp_len > 1))
			|| ((temp_str[0] == '0') && (temp_len > 1))
			|| ((temp_str[0] == '1') && (temp_len > 1))
			)
		{
			if ((temp_str[0] == 'L') && (temp_len < 10))
			{
				std::string addr_str(temp_str);
				string_replace(addr_str, "L", "");
				string_replace(addr_str, " ", "");
				string_replace(addr_str, "\n", "");
				addr = strtol(addr_str.c_str(), NULL, 10);
				continue;
			}
			if (((temp_str[0] == ' ') && (temp_len > 10))
				|| ((temp_str[0] == '0') && (temp_len > 10))
				|| ((temp_str[0] == '1') && (temp_len > 10)))
			{
				std::string data_str(temp_str);
				string_replace(data_str, " ", "");
				string_replace(data_str, "*", "");
				string_replace(data_str, "\n", "");
				unsigned int data_len = data_str.length();
				memcpy(&vMemBuf.at(addr), data_str.c_str(), data_len);
				addr += data_len;
				file_length += data_len;
			}
			if ((temp_str[0] == 'L') && (temp_len > 10))
			{
				int space = 0;
				for (space = 0; space < 10; space++)
				{
					if (temp_str[space] == ' ')
					{
						space += 1; // "L00080" 's length
						break;
					}
				}
				char tempaddr_str[10];
				std::string fix_str(temp_str);
				//address fix
				strncpy(tempaddr_str, fix_str.c_str(), space);
				if (tempaddr_str[space - 1] == ' ')
				{
					tempaddr_str[space - 1] = '\0';
				}
				std::string addr_str(tempaddr_str);
				string_replace(addr_str, "L", "");
				string_replace(addr_str, " ", "");
				string_replace(addr_str, "\n", "");
				addr = strtol(addr_str.c_str(), NULL, 10);
				//data fix
				std::string data_str = fix_str.erase(0, space);
				string_replace(data_str, " ", "");
				string_replace(data_str, "*", "");
				string_replace(data_str, "\n", "");
				unsigned int data_len = data_str.length();
				memcpy(&vMemBuf.at(addr), data_str.c_str(), data_len);
				addr += data_len;
				file_length += data_len;
			}
		}
	} while (!feof(hFile));
	//for Lattice LC4XXX/LCMXO/LCMXO2/LCMXO3L/Powr60X
	for (int x = 0; x < (file_length / g_column); x++)
	{
		char column_str[4800] = { '\0' };
		memcpy(&column_str[0], &vMemBuf.at(x*g_column), g_column);

		std::string datacolumn_str(column_str);
		unsigned temp_datalen = datacolumn_str.length();
		unsigned insert_length = (temp_datalen % 16) ? (16 - temp_datalen % 16) : 0;
		for (int i = 0; i < insert_length; i++)
		{
			datacolumn_str.insert(datacolumn_str.end(), _T('0'));
		}
		unsigned int Data_len = datacolumn_str.length();
		for (int j = 0; j < (Data_len / 8); j++)
		{
			char temp_data[8];
			strncpy(temp_data, datacolumn_str.c_str(), 8);
			unsigned char data = strtol(temp_data, NULL, 2);
			memcpy(g_JED_data + g_data_len, &data, sizeof(data));
			datacolumn_str.erase(0, 8);
			g_data_len += 1;
		}
	}
	if (g_data_len)
	{
		file_info.addr = addr_index;
		file_info.len = (unsigned long long)g_data_len;
		file_info.offset_binary = 0;
		vAnalysRes->resize(sizeof(file_info));
		memcpy(&(vAnalysRes->at(0)), &file_info, sizeof(file_info));
	}
	////////////////////////////////////////////////////////////////////////////////
file_end : 
	if (hFile)
	{
		fclose(hFile);	
	}
	return err;
}
ANALYS_FILE_API_C int analys_release()
{
	if (g_JED_data)
		free(g_JED_data);
	g_JED_data = NULL;
	g_data_len = 0;
	g_JED_size = 0;
	g_ptn_addr = 0;
	g_file_offset = 0;
	return E_OK;
}
ANALYS_FILE_API_C unsigned long analys_checksum(unsigned long(*checksum_func)(int first, unsigned long init, const unsigned char *data, unsigned long len))
{ 
		return checksum_func(1, 0, g_JED_data, g_data_len);
}
ANALYS_FILE_API_C unsigned long analys_read(unsigned char *buf_out, unsigned long buf_len, unsigned long long ptn_addr, unsigned long long *ptn_offset)
{
	if (!buf_out)
		return 0;
	*ptn_offset = 0;
 	unsigned long lenRead = min(ptn_addr >g_data_len ? 0 : (g_data_len - (unsigned int)ptn_addr), buf_len);
	//unsigned long lenRead = buf_len;
	if (lenRead == 0)
		return 0;
	memcpy(buf_out, g_JED_data + ptn_addr, lenRead);
	return lenRead;
}