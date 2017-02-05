// File_NVM.cpp : Defines the exported functions for the DLL application.
#include "stdafx.h"
#include <tchar.h>
#include <vector>
#include "FILE_JED.h"
#include "../loadfile_include/data_file.h"
#include "../error_no/error_no.h"
#include <assert.h>
#define TEMP_FILE_NAME _T("DediJEDToBin.tmp")
#define MAX_LINE_SIZE 512
#define PER_WIN_SIZE          (64*1024*1024UL)
#define ALIGN_SIZE            (64 * 1024)

static HANDLE temp_file = NULL;                /* handle of temp file */
static HANDLE map_file = NULL;                 /* handle of map file */
static LPVOID lpmap_buf = NULL;                /* handle of map buf  */
static std::vector<ANALYS_FILE_RES> g_vAnalysRes;
static unsigned long long g_ptn_addr = 0;
static unsigned long long g_file_offset = 0;
static unsigned long long g_last_winoffset = 0;
static unsigned long long g_file_len = 0;
static unsigned long long g_last_maplen = 0;
static unsigned char *scroll_win(unsigned long long file_offset, unsigned long *length);
static int create_temp_file();
static int analys_jed_file(const wchar_t *path);
static int write_to_bin(const unsigned char *buff, ANALYS_FILE_RES *pRecord);
static void MergeRecord();


int create_temp_file()
{
	assert(!temp_file);
	TCHAR tempPath[MAX_PATH];
	GetTempPath(MAX_PATH, tempPath);
#ifndef _DEBUG
	temp_file = CreateFile((std::wstring(tempPath) + std::wstring(TEMP_FILE_NAME)).c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN | FILE_FLAG_DELETE_ON_CLOSE, NULL);
#else
	temp_file = CreateFile((std::wstring(tempPath) + std::wstring(TEMP_FILE_NAME)).c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN | FILE_FLAG_DELETE_ON_CLOSE, NULL);
#endif
	if (temp_file == INVALID_HANDLE_VALUE)
		return E_CREATE_FILE_FAIL;
	return E_OK;
}
bool CheckHexString(std::string &csr)
{
	if (csr[csr.length() - 1] == '\n')
		csr.erase(csr.end() - 1, csr.end());
	for (int i = 1; i<csr.length(); i++)
	{
		if ( !(csr[i] >= '0' && csr[i] <= '9') 
			&& !(csr[i] >= 'a' && csr[i] <= 'f') 
			&& !(csr[i] >= 'A' && csr[i] <= 'F')
			)
			return false;
	}
	return true;
}
int analys_jed_file(const wchar_t *path)
{
	int err = E_OK;
	FILE *hFile = NULL;
	ANALYS_FILE_RES Record;
	g_vAnalysRes.clear();

	CHAR  mLine_str[MAX_LINE_SIZE];
	TCHAR wLine_str[MAX_LINE_SIZE];
	std::vector<unsigned char> vData;

	unsigned int  Line_len = 0;
	unsigned int  Data_len = 0;
	unsigned long Phys_addr = 0;
	unsigned long Phys_addr2 = 0;

	hFile = _wfopen(path, _T("r"));
	if (hFile == NULL)
	{
		err = E_OPEN_FILE_FAIL;
		goto end;
	}
	//Searching LXXXXX
	do
	{
		if (fgets(mLine_str, MAX_LINE_SIZE, hFile) == NULL)
			continue;
	} while (mLine_str[0] != 'L' && !feof(hFile));
	if (mLine_str[0] != 'L' || CheckHexString(std::string(mLine_str + 1)) != true)
		goto end;

	mbstowcs(wLine_str, mLine_str + 1, MAX_LINE_SIZE);
	Phys_addr = _tcstoul(wLine_str, NULL, 16);
	//start translate
	do
	{
		if (fgets(mLine_str, MAX_LINE_SIZE, hFile) == NULL)
			continue;
		mbstowcs(wLine_str, mLine_str, MAX_LINE_SIZE);
		// Delete the '\n'
		std::wstring strLine(wLine_str);
		if (strLine[strLine.length() - 1] == _T('\n'))
			strLine.erase(strLine.end() - 1, strLine.end());
		// if Find the string = *
		Line_len = strLine.length();
		if (strLine.find(_T('*')) != std::wstring::npos)
			goto second_range;
		// Insert the data
		Data_len = (Line_len % 16) ? (16 - Line_len % 16) : 0;
		for (int i = 0; i < Data_len; i++)
			strLine.insert(strLine.end(), _T('0'));

		while (strLine.length())
		{
			unsigned char data = 0;
			for (int j = 0; j < 8; j++)
			{
				data = data << 1;
				if (strLine[j] == _T('1'))
					data |= 1;
			}
			strLine.erase(strLine.begin(), strLine.begin() + 8);
			vData.push_back(data);
		}
	} while (!feof(hFile) && err == E_OK);
	///////////////////////////////////////////////////////////////////////////
second_range:
	// if the doesn't have the second range
	fgets(mLine_str, MAX_LINE_SIZE, hFile) ;
	if (mLine_str[0] == 'C')
	{
		Record.addr = Phys_addr;
		Record.len = vData.size();
		Record.offset_binary = g_file_len;
		err = write_to_bin(&vData[0], &Record);
		goto end;
	}
	do
	{
		if (fgets(mLine_str, MAX_LINE_SIZE, hFile) == NULL)
			continue;
	} while (mLine_str[0] != 'L' && !feof(hFile));
	do
	{
		if (fgets(mLine_str, MAX_LINE_SIZE, hFile) == NULL)
			continue;
		mbstowcs(wLine_str, mLine_str, MAX_LINE_SIZE);
		//Delete the '\n'
		std::wstring strLine(wLine_str);
		if (strLine[strLine.length() - 1] == _T('\n'))
			strLine.erase(strLine.end() - 1, strLine.end());
		Line_len = strLine.length();
		if (strLine.find(_T('*')) != std::wstring::npos)
		{
				Record.addr = Phys_addr;
				Record.len = vData.size();
				Record.offset_binary = g_file_len;
				err = write_to_bin(&vData[0], &Record);
				break;
		}
		//Insert the data
		Data_len = (Line_len % 16) ? (16 - Line_len % 16) : 0;
		for (int i = 0; i < Data_len; i++)
			strLine.insert(strLine.end(), _T('0'));

		while (strLine.length())
		{
			unsigned char data = 0;
			for (int j = 0; j < 8; j++)
			{
				data = data << 1;
				if (strLine[j] == _T('1'))
					data |= 1;
			}
			strLine.erase(strLine.begin(), strLine.begin() + 8);
			vData.push_back(data);
		}
	} while (!feof(hFile) && err == E_OK);
end:
	if (hFile)
		fclose(hFile);
	return err;
}
int write_to_bin(const unsigned char *buff, ANALYS_FILE_RES *pRecord)
{
	assert(temp_file);
	assert(buff);
	assert(pRecord);

	int err = E_OK;
	unsigned long ret_len = 0;
	ANALYS_FILE_RES oRecordItem;
	ANALYS_FILE_RES nRecordItem;

	nRecordItem.addr = pRecord->addr;
	nRecordItem.len = pRecord->len;
	nRecordItem.offset_binary = pRecord->offset_binary;
	//max record count is 4g,data len is 1T	
	for (int i = 0; i < g_vAnalysRes.size(); i++)
	{
		oRecordItem = g_vAnalysRes[i];
		if (oRecordItem.addr == nRecordItem.addr)
		{
			err = E_FILE_ANALYSE_FILE;
			goto end;
		}
		if ((nRecordItem.addr < oRecordItem.addr && (nRecordItem.addr + nRecordItem.len > oRecordItem.addr)) ||
			(oRecordItem.addr < nRecordItem.addr && (oRecordItem.addr + oRecordItem.len > nRecordItem.addr))
			)
		{
			err = E_FILE_ANALYSE_FILE;
			goto end;
		}
		if (i == g_vAnalysRes.size() - 1)
		{

			if (oRecordItem.addr + oRecordItem.len == nRecordItem.addr)
			{
				if (oRecordItem.offset_binary + oRecordItem.len == nRecordItem.offset_binary)
				{
					g_vAnalysRes.erase(g_vAnalysRes.begin() + i);
					oRecordItem.len += nRecordItem.len;
					g_vAnalysRes.push_back(oRecordItem);
				}
				else
					g_vAnalysRes.push_back(nRecordItem);
				break;
			}
			if (oRecordItem.addr + oRecordItem.len < nRecordItem.addr)
			{
				g_vAnalysRes.push_back(nRecordItem);
				break;
			}
			if (nRecordItem.addr + nRecordItem.len <= oRecordItem.addr)
			{
				g_vAnalysRes.insert(g_vAnalysRes.begin() + i, nRecordItem);
				break;
			}

			break;
		}
		if (i < g_vAnalysRes.size() - 1)
		{
			if (nRecordItem.addr < g_vAnalysRes[i + 1].addr && (nRecordItem.addr + nRecordItem.len > g_vAnalysRes[i + 1].addr))
			{
				err = E_FILE_ANALYSE_FILE;
				goto end;
			}
			if (oRecordItem.addr + oRecordItem.len == nRecordItem.addr && (oRecordItem.offset_binary + oRecordItem.len == nRecordItem.offset_binary))
			{
				g_vAnalysRes[i].len += nRecordItem.len;
				break;
			}
			if (oRecordItem.addr + oRecordItem.len <= nRecordItem.addr && g_vAnalysRes[i + 1].addr > nRecordItem.addr)
			{
				g_vAnalysRes.insert(g_vAnalysRes.begin() + i + 1, nRecordItem);
				break;
			}
			if (nRecordItem.addr + nRecordItem.len <= oRecordItem.addr)
			{
				g_vAnalysRes.insert(g_vAnalysRes.begin() + i, nRecordItem);
				break;
			}
		}
	}
	if (g_vAnalysRes.size() == 0)
		g_vAnalysRes.push_back(nRecordItem);
	WriteFile(temp_file, buff, pRecord->len, &ret_len, NULL);
	if (ret_len != pRecord->len)
	{
		err = E_WRITE_FILE_FAIL;
		goto end;
	}
	g_file_len += pRecord->len;

end:
	return err;
}
void MergeRecord()
{
	ANALYS_FILE_RES RecordItem;
	unsigned int  RecordCount = 0;
	unsigned long long read_len = 0;
	unsigned long ret_read = 0;
	unsigned long long write_len = 0;
	unsigned long ret_write = 0;
	std::vector<unsigned char> vBinRecord;
	BOOL bFileIO = FALSE;

	RecordCount = g_vAnalysRes.size();
	if (RecordCount == 0 || !g_file_len)
		return;

	vBinRecord.resize(g_file_len);
	SetFilePointer(temp_file, 0, 0, FILE_BEGIN);
	while (read_len < g_file_len)
	{
		bFileIO = ReadFile(temp_file, &vBinRecord[0] + read_len, (unsigned long)(g_file_len - read_len), &ret_read, NULL);
		assert(bFileIO);
		if (bFileIO == FALSE)
			return;
		read_len += ret_read;
	}

	if (temp_file)
	{
		CloseHandle(temp_file);
		temp_file = NULL;
	}
	if (create_temp_file() != E_OK)
		return;
	assert(temp_file);

	for (unsigned int i = 0; i < RecordCount; i++)
	{
		if (i == 0)
		{
			RecordItem = g_vAnalysRes.at(i);
			bFileIO = WriteFile(temp_file, &vBinRecord[0] + RecordItem.offset_binary, RecordItem.len, &ret_write, NULL);
			assert(bFileIO);

			RecordItem.offset_binary = write_len;
			write_len += ret_write;

			g_vAnalysRes.erase(g_vAnalysRes.begin() + i);
			g_vAnalysRes.insert(g_vAnalysRes.begin() + i, RecordItem);

		}
		else
		{
			RecordItem = g_vAnalysRes.at(i);
			bFileIO = WriteFile(temp_file, &vBinRecord[0] + RecordItem.offset_binary, RecordItem.len, &ret_write, NULL);
			assert(bFileIO);

			RecordItem.offset_binary = write_len;
			write_len += ret_write;

			if (g_vAnalysRes[i - 1].addr + g_vAnalysRes[i - 1].len == RecordItem.addr)
			{
				RecordItem.addr = g_vAnalysRes[i - 1].addr;
				RecordItem.len += g_vAnalysRes[i - 1].len;
				RecordItem.offset_binary = g_vAnalysRes[i - 1].offset_binary;

				g_vAnalysRes.erase(g_vAnalysRes.begin() + i - 1);
				g_vAnalysRes.insert(g_vAnalysRes.begin() + i - 1, RecordItem);
				g_vAnalysRes.erase(g_vAnalysRes.begin() + i);

				RecordCount--;
				i--;
			}
			else
			{
				g_vAnalysRes.erase(g_vAnalysRes.begin() + i);
				g_vAnalysRes.insert(g_vAnalysRes.begin() + i, RecordItem);
			}
		}
	}
	assert(write_len == g_file_len);
}
unsigned char *scroll_win(unsigned long long file_offset, unsigned long *length)
{
	unsigned long long lenRemain;
	unsigned long long win_offset;
	unsigned long long lenMap = 0;
	unsigned long  win_in_offset;

	win_offset = file_offset / PER_WIN_SIZE *PER_WIN_SIZE;
	win_in_offset = file_offset - win_offset;

	lenRemain = g_file_len - min(g_file_len, file_offset);
	*length = min(min(*length, lenRemain), PER_WIN_SIZE - win_in_offset);
	if (*length == 0)
		return NULL;

	if (lpmap_buf && g_last_maplen &&
		(win_offset + win_in_offset >= g_last_winoffset) &&
		(win_offset + win_in_offset + *length <= g_last_winoffset + g_last_maplen)
		)
	{
		return (unsigned char*)(lpmap_buf)+win_offset + win_in_offset - g_last_winoffset;
	}
	else
	{
		if (lpmap_buf)
			UnmapViewOfFile(lpmap_buf);
		lpmap_buf = NULL;
	}


	if (lpmap_buf == NULL)//first map
	{
		lenMap = g_file_len - min(g_file_len, win_offset);
		lenMap = lenMap > PER_WIN_SIZE ? PER_WIN_SIZE : lenMap;
		*length = min(lenMap - win_in_offset, *length);
		g_last_winoffset = win_offset;
		g_last_maplen = lenMap;
	}

	//scroll window
	if (lpmap_buf == NULL)
	{
		assert(lenMap <= PER_WIN_SIZE);
		lpmap_buf = MapViewOfFile(map_file, FILE_MAP_READ, (DWORD)(win_offset >> 32), (DWORD)(win_offset & 0xffffffff), lenMap);
		if (lpmap_buf == NULL)
		{
			return NULL;
		}
	}
	return (unsigned char*)(lpmap_buf)+win_in_offset;
	//win_offset = file_offset;
	//lenRemain = g_file_len - min(g_file_len, win_offset);
	//*length = min((unsigned long long)*length, lenRemain);


	//win_offset = win_offset / PER_WIN_SIZE*PER_WIN_SIZE;
	//win_in_offset = (unsigned long)(file_offset - win_offset);
	//if (win_in_offset + *length > PER_WIN_SIZE || win_offset != g_last_winoffset)
	//{
	//	win_offset = win_offset + win_in_offset;
	//	win_offset = win_offset / ALIGN_SIZE * ALIGN_SIZE;
	//	win_in_offset = (unsigned long)(file_offset - win_offset);
	//	if (lpmap_buf)
	//	{
	//		UnmapViewOfFile(lpmap_buf);
	//		lpmap_buf = NULL;
	//	}
	//}
	//else
	//{
	//	win_in_offset = (unsigned long)(file_offset - g_last_winoffset);
	//}

	//if (g_last_maplen && (win_in_offset >= g_last_maplen || (g_last_maplen - win_in_offset < *length)))
	//{
	//	if (lpmap_buf)
	//	{
	//		UnmapViewOfFile(lpmap_buf);
	//		lpmap_buf = NULL;
	//	}
	//}

	//lenMap = win_in_offset + *length;
	////scroll window
	//if (lpmap_buf == NULL)
	//{
	//	g_last_winoffset = win_offset;

	//	lpmap_buf = MapViewOfFile(map_file, FILE_MAP_READ, (DWORD)(win_offset >> 32), (DWORD)(win_offset & 0xffffffff), PER_WIN_SIZE > lenMap ? lenMap : PER_WIN_SIZE);
	//	if (lpmap_buf == NULL)
	//	{
	//		return NULL;
	//	}
	//	g_last_maplen = lenMap;
	//}
	//return (unsigned char*)(lpmap_buf)+win_in_offset;
}
ANALYS_FILE_API_C int analys_init()
{
	return analys_release();

}
ANALYS_FILE_API_C int analys_file(const wchar_t *path, unsigned long long ptn_addr, unsigned long long file_offset, std::vector<unsigned char> *vAnalysRes)
{
	int err = E_OK;
	if (!path || !vAnalysRes)
	{
		err = E_INVALID_PARAM;
		goto end;
	}

	analys_release();
	err = create_temp_file();
	if (err != E_OK)
		goto end;

	err = analys_jed_file(path);
	if (err != E_OK)
		goto end;

	MergeRecord();
	vAnalysRes->clear();


	if (g_vAnalysRes.size())
	{
		vAnalysRes->resize(sizeof(ANALYS_FILE_RES)*g_vAnalysRes.size());

		memcpy(&(vAnalysRes->at(0)), &(g_vAnalysRes.at(0)), sizeof(ANALYS_FILE_RES)*g_vAnalysRes.size());
	}

	g_file_offset = (file_offset == (unsigned long long) - 1 ? 0 : file_offset);
	g_ptn_addr = (ptn_addr == (unsigned long long) - 1 ? 0 : ptn_addr);

end:
	if (err != E_OK)
		analys_release();
	return err;
}
ANALYS_FILE_API_C int analys_release()
{
	int err = E_OK;
	if (lpmap_buf)
		UnmapViewOfFile(lpmap_buf);
	if (map_file)
		CloseHandle(map_file);
	if (temp_file && temp_file != INVALID_HANDLE_VALUE)
		CloseHandle(temp_file);

	temp_file = NULL;
	map_file = NULL;
	lpmap_buf = NULL;

	g_vAnalysRes.clear();
	g_last_winoffset = 0;
	g_last_maplen = 0;
	g_file_len = 0;
	g_ptn_addr = 0;
	g_file_offset = 0;
	return err;
}
ANALYS_FILE_API_C unsigned long analys_checksum(unsigned long(*checksum_func)(int first, unsigned long init, const unsigned char *data, unsigned long len))
{
	assert(checksum_func);
	unsigned long  cks_value = 0;
	unsigned char  *buff = NULL;
	unsigned long  data_len = 0;
        int first = 1;
	if (map_file == NULL)
	{
		map_file = CreateFileMapping(temp_file, NULL, PAGE_READWRITE, g_file_len >> 32, g_file_len & 0xffffffff, NULL);
		if (map_file == NULL)
		{
			errno = E_FILE_ANALYSE_FILE;

			return 0;
		}
	}
	for (int i = 0; i < g_vAnalysRes.size(); i++)
	{
		data_len = (unsigned long)g_vAnalysRes[i].len;
		buff = scroll_win(g_vAnalysRes[i].offset_binary, &data_len);
		if (buff == NULL)
			break;
		if (buff && checksum_func)
			cks_value += checksum_func(first, cks_value, buff, data_len);

                first = 0;
	}
	if (map_file)
	{
		CloseHandle(map_file);
		map_file = NULL;
	}
	return cks_value;
}
ANALYS_FILE_API_C unsigned long analys_read(unsigned char *buf_out, unsigned long buf_len, unsigned long long ptn_addr, unsigned long long *ptn_offset)
{
	assert(buf_out);
	assert(buf_len);
	assert(ptn_offset);
	assert(g_vAnalysRes.size());

	unsigned char *buf_map = NULL;
	unsigned long read_len = buf_len;
	unsigned long copy_len = 0;
	unsigned long ret_len = 0;

	*ptn_offset = 0;

	if (!buf_out || !buf_len || !ptn_offset)
		return 0;
	if (map_file == NULL)
	{
		map_file = CreateFileMapping(temp_file, NULL, PAGE_READWRITE, g_file_len >> 32, g_file_len & 0xffffffff, NULL);
		if (map_file == NULL)
		{
			errno = E_FILE_ANALYSE_FILE;

			return 0;
		}
	}
	int i;
	int last_index = 0;

	if (g_vAnalysRes[0].addr >= (ptn_addr + read_len))
	{
		*ptn_offset = g_vAnalysRes[0].addr - ptn_addr;
		goto end;
	}

	while (copy_len < read_len)
	{
		ret_len = 0;
		for (i = last_index; i < g_vAnalysRes.size(); i++)
		{
			if (i == g_vAnalysRes.size() - 1)
			{
				if (g_vAnalysRes[i].addr <= (ptn_addr + copy_len) &&
					g_vAnalysRes[i].addr + g_vAnalysRes[i].len >(ptn_addr + copy_len))
				{
					ret_len = min(g_vAnalysRes[i].len - (ptn_addr + copy_len - g_vAnalysRes[i].addr), read_len - copy_len);
					last_index = i;
					break;
				}
				else
				{
					goto end;
				}
			}
			if (i < g_vAnalysRes.size() - 1)
			{
				if ((g_vAnalysRes[i].addr + g_vAnalysRes[i].len <= ptn_addr + copy_len) &&
					(g_vAnalysRes[i + 1].addr > ptn_addr + copy_len)
					)
				{
					*ptn_offset = g_vAnalysRes[i + 1].addr - (ptn_addr + (unsigned long long)copy_len);
					goto end;
				}
				if ((g_vAnalysRes[i].addr <= (ptn_addr + copy_len)) &&
					(g_vAnalysRes[i].addr + g_vAnalysRes[i].len >(ptn_addr + copy_len))
					)
				{
					ret_len = min(g_vAnalysRes[i].len - (ptn_addr + copy_len - g_vAnalysRes[i].addr), read_len - copy_len);
					last_index = i;
					break;
				}
			}
		}
		buf_map = scroll_win(g_vAnalysRes[i].offset_binary + ptn_addr + copy_len - g_vAnalysRes[i].addr, &ret_len);

		assert(buf_map);
		if (buf_map == NULL)
			goto end;

		memcpy(buf_out + copy_len, buf_map, ret_len);
		copy_len += ret_len;
	}
end:
	return copy_len;
}
