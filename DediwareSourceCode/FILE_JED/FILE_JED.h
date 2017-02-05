// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FILE_JED_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FILE_JED_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once
#define ANALYS_FILE_API __declspec(dllexport)   
#define ANALYS_FILE_API_C extern "C" __declspec(dllexport)

ANALYS_FILE_API_C int analys_init(const struct chip_info_c *ChipInfo);
ANALYS_FILE_API_C int analys_file(const wchar_t *path, unsigned long long ptn_addr, unsigned long long file_offset, std::vector<unsigned char> *vAnalysRes);
ANALYS_FILE_API_C int analys_release();
ANALYS_FILE_API_C unsigned long analys_checksum(unsigned long(*checksum_func)(int first, unsigned long init, const unsigned char *data, unsigned long len));
ANALYS_FILE_API_C unsigned long analys_read(unsigned char *buf_out, unsigned long buf_len, unsigned long long ptn_addr, unsigned long long *ptn_offset);
//using namespace std;
//void string_replace(string & strBig, const string & strsrc, const string &strdst);