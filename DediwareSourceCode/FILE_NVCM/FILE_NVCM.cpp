// FILE_NVCM.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include <tchar.h>
#include <vector>
#include "FILE_NVCM.h"
#include "../loadfile_include/data_file.h"
#include "../error_no/error_no.h"
#include <assert.h>
/////////////////////////////////////////////////
static unsigned char *g_nvcm_data = NULL;
static unsigned int   g_nvcm_size = 0;
static unsigned int   g_data_len = 0;
static unsigned long long g_ptn_addr = 0;
static unsigned long long g_file_offset = 0;
ANALYS_FILE_API_C int analys_init()
{
	return analys_release();
}

ANALYS_FILE_API_C int analys_file(const wchar_t *path, unsigned long long ptn_addr, unsigned long long file_offset, std::vector<unsigned char> *vAnalysRes)
{
	assert(path);
	assert(vAnalysRes);

	int err = E_OK; // E_OK = 0 = True;
	FILE *hFile = NULL;
	CONST INT MAX_LINE_SIZE = 64;
	char Line_str[MAX_LINE_SIZE] = { 0 };
	char temp_str[MAX_LINE_SIZE] = { 0 };
	char *Data_str = NULL;
	unsigned int Data_len = 0;
	unsigned int Line_len = 0;
	ANALYS_FILE_RES file_info;
	//
	analys_release();
	vAnalysRes->clear();

	///To get file's informations
	_stati64	st;
	if (_tstat64(path, &st) == -1){
		err = E_FILE_NO_EXIST;
		goto end;
	}
	///To define the file size and build memory for data
	g_nvcm_size = (unsigned int)st.st_size;
	g_nvcm_data = (unsigned char*)malloc(g_nvcm_size);
	memset(g_nvcm_data, 0x00, g_nvcm_size);
	///Checking the memory range
	assert(g_nvcm_data);
	if (!g_nvcm_data){
		err = E_ALLOC_MEMORY_FAIL;
			goto end;
	}
	//To open the file from the path
	hFile = _wfopen(path, _T("r"));
	
	if (hFile == NULL)
	{
		err = E_OPEN_FILE_FAIL;
		goto end;
	}
	
	///----------Analyze the file
	///----------Find the .NVCM File includes ( " 06 " )
	do{
		if (fgets(temp_str, MAX_LINE_SIZE, hFile) == NULL)
			continue;
	} while ((temp_str[0] != '0' && temp_str[1] != '6') && !feof(hFile));
	
	//-----------
	do{
		if (fgets(Line_str, MAX_LINE_SIZE, hFile) == NULL)
			continue;
		//End_pointer '04'
		if ((Line_str[0] == '0') && (Line_str[1] == '4'))
			break;
		// "\n ->\0"
		Line_len = strlen(Line_str) - 1;
		if (Line_str[Line_len] == '\n') // ´«¦æ
			Line_str[Line_len] = '\0';	// ªÅ®æ
		
		if ((Line_str[0] == '0') && (Line_str[1] == '2'))
			Line_str[1] = '0';

		//Data translate
		for (int i = 0; i < (Line_len/3) ; i++)
		{
			sscanf((Line_str + (i * 3)), "%3x", g_nvcm_data + g_data_len + i);
		}
		g_data_len += Line_len / 3;
	} while (!feof(hFile));
		if (g_data_len)
		{
			//file_info.addr = (unsigned long long) - 1;
			file_info.addr = 0;
			file_info.len = (unsigned long long)g_data_len;
			file_info.offset_binary = 0;
			vAnalysRes->resize(sizeof(file_info));
			memcpy(&(vAnalysRes->at(0)), &file_info, sizeof(file_info));
		}
	end:
		if (hFile)
		{
			fclose(hFile);	
		}
		return err;
}
ANALYS_FILE_API_C int analys_release()
{
	if (g_nvcm_data)
		free(g_nvcm_data);
	g_nvcm_data = NULL;
	g_data_len = 0;
	g_nvcm_size = 0;
	g_ptn_addr = 0;
	g_file_offset = 0;
	return E_OK;
}
ANALYS_FILE_API_C unsigned long analys_checksum(unsigned long(*checksum_func)(int first, unsigned long init, const unsigned char *data, unsigned long len))
{
	return checksum_func(1, 0, g_nvcm_data, g_data_len);
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
	memcpy(buf_out, g_nvcm_data + ptn_addr, lenRead);

	return lenRead;
}