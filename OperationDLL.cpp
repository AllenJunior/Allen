// OperationDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <assert.h>
#include <vector>
#include "../common/comm_type.h"
#include "../common/loadfile_info.h"
#include "../common/prog_info.h"
#include "../vm_mgr/vm_mgr.h"
#include "stdio.h"
#include <iostream>
#include <fstream>
#include <string>
#include <tchar.h>
#include "../Log/log.h"

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
	int pck = EndOfPath.find(_T('.jed'));
	if (pck < 0)
	{
		MessageBox(NULL, L"The Path of File is too long !", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
		return 0;
	}
	const wchar_t *path = &du_header.at(0).image_file_path[0];
	std::ifstream input(path);
	std::string line, line2, Templine, crcvalue;
	unsigned int checksum = 0;
	char tempdata[8];
	int i, j, t, k, L;
	//Find the "L0000"
	do{
		std::getline(input, line);
		i = line.find_first_of(_T('L'));
	} while (i != 0);	//Find "L"
	//String Adding
	do{
		std::getline(input, line);
		Templine += line;	//Row + Row...
		j = line.find_first_of(_T('*'));
	} while (j != 0);	//FindEnd"*"
	///////////////////////////////////////////////////////////////
	//If the File had two part "L"
	/*
	do{
	std::getline(input, line2);
	std::getline(input, line2);
	L = line2.find_first_of(_T('L'));
	} while (L != 0);
	if (L == 0)
	{
	int relength = Templine.length();
	Templine.erase(relength - 1, 1);
	do{
	std::getline(input, line2);
	Templine += line2;
	j = line2.find_first_of(_T('*'));
	} while (j != 0);
	}
	*/
	///////////////////////////////////////////////////////////////
	//Determine the last bit
	int x, y;
	x = Templine.length();
	Templine.erase(x - 1, 1);
	y = 8 - ((x - 1) % 8);
	if (y < 8)
	{
		for (int z = 0; z < y; z++)
		{
			std::string a = "0";
			Templine += a;
		}
	}
	//Calculate the CRC value
	for (int k = 0; k < Length; k++)
	{
		strncpy(tempdata, Templine.c_str(), 8);
		int data = strtol(tempdata, NULL, 2);
		checksum += MsbToLsb(data);
		Templine.erase(0, 8);
	}
	//Find the "CXXXX"
	do{
		std::getline(input, line);
		crcvalue = line;
		j = line.find_first_of(_T('C'));
	} while (j != 0);
	crcvalue.erase(0, 1);
	crcvalue.erase(4, 1);
	int c = strtol(crcvalue.c_str(), NULL, 16);
	checksum &= 0xFFFF;
	t = memcmp(&c, &checksum, sizeof(checksum));
	if (t == 0x0){
		*chip_checksum = checksum;
	}
	else
		MessageBox(NULL, L"CheckSum Different !", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
	return 0;
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
		unsigned int BufferLen = 16;
		int pcmp = EndOfPath.find(_T('.jed'));
		if (pcmp < 0)
		{
			MessageBox(NULL, L"The Path of File is too long !", L"", MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
			return 0;
		}
		const wchar_t *path = &du_header.at(0).image_file_path[0];
		std::ifstream input(path);
		std::string rowstr, bitstr, ucode;
		char byte[8];
		char Frow[8];
		char Fbit[4];
		char Ucode[4];
		unsigned char regdata[16];
		int r, p, q, w;
		if (wcscmp(chip->description, L"ispMachXO2") == 0)
		{
			//Find the FeatureRow(8 Bytes)
			do{
				std::getline(input, rowstr);
				r = rowstr.find_first_of(_T('E'));
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
				q = ucode.find_first_of(_T('UH'));
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
			pStruct_Buffer = (loadfile_item*)malloc(sizeof(loadfile_item)+BufferLen);
			memset(pStruct_Buffer->buff, 0, BufferLen);
			memcpy(pStruct_Buffer->buff, &regdata, sizeof(regdata));
			pStruct_Buffer->len = BufferLen;
			*reg = pStruct_Buffer;
		}
		if (wcscmp(chip->description, L"ispMACHXO") == 0 || wcscmp(chip->description, L"ispPAC") == 0)
		{
			unsigned int BufferLen = 4;
			pStruct_Buffer = (loadfile_item*)malloc(sizeof(loadfile_item)+BufferLen);
			memset(pStruct_Buffer->buff, 0, BufferLen);
			do{
				std::getline(input, ucode);
				q = ucode.find_first_of(_T('U'));
			} while (q != 0);
			w = ucode.length();
			ucode.erase(w - 1, 1);
			ucode.erase(0, 1);
			for (int i = 0; i < 4; i++)
			{
				strncpy(byte, ucode.c_str(), 8);
				char tempucode = strtol(byte, NULL, 2);
				Ucode[3 - i] = tempucode;
				ucode.erase(0, 8);
			}
			memcpy(pStruct_Buffer->buff, &Ucode, sizeof(Ucode));
			pStruct_Buffer->len = BufferLen;
			*reg = pStruct_Buffer;
		}
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport)  
int du_to_reg(
                const struct chip_info_c                *chip,                  /* chip info structure */
                std::vector<struct loadfile_du>         &du_header,             /* du header */
                vm_ptr_t                                buff,
                loadfile_item                           **reg,
                unsigned long                           *chip_checksum,         /* loadfile --> chipchecksum */
                std::vector<struct loadfile_fcs>        &file_checksum          /* file checksum */
                )
{
        assert(chip);
        assert(reg);
        assert(chip_checksum);

		//check du_header NULL?	 
		if (du_header.empty())
			return 0;
		if (wcscmp(chip->description, L"ispMachXO2") == 0|| wcscmp(chip->description,L"ispMACHXO") ==0 
			|| wcscmp(chip->description, L"ispPAC") == 0)
		{
			XO2Feature(chip, du_header, buff, reg, chip_checksum, file_checksum);
		}
		
		if (wcscmp(chip->manufacturer, L"LATTICE") == 0)
		{
			Cal_JEDChecksum(chip, du_header, buff, reg, chip_checksum, file_checksum);
		}
		else
			return 0;

		return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport)
int final_du(
			const struct chip_info_c                *chip,                  /* chip info structure */
			std::vector<struct loadfile_du>         &du_header,             /* du header */
			vm_ptr_t                                buff,
			loadfile_item                           **reg,
			unsigned long                           *chip_checksum,         /* loadfile --> chipchecksum */
			std::vector<struct loadfile_fcs>        &file_checksum          /* file checksum */
)
{
	assert(chip);
	assert(reg);
	assert(chip_checksum);

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport)
int read_reg(
				const struct chip_info_c        *chip,
				unsigned char                   *reg,
				int                             reg_len,
				int(*read)(
							const struct chip_info_c  *chip,
							int             ptn_index,
							unsigned long long start,
							unsigned char   *buff,
							int             len,
							int             timeout,
							void            *param
				),
				void                            *read_param
)
{
	assert(chip);
	assert(reg);
	assert(read);
	assert(read_param);

	//LCMXO2 Read to register
	if (wcscmp(chip->description, L"ispMachXO2") == 0)
	{
		unsigned char buffer[14];
		unsigned char regdata[16];
		char FeatureRow[8];
		char FeatureBit[4];
		char Usercode[4];
		read(chip, 1, 0, buffer, 14, 1000, read_param); 
		memcpy(FeatureRow, buffer, 8);
		memset(FeatureBit, 0, 4);
		for (int i = 0; i < 2; i++)
		{
			memcpy(FeatureBit + i, buffer + sizeof(FeatureRow) + 1 - i, 1); //reverse take
		}
		for (int j = 0; j < 4; j++)
		{
			memcpy(Usercode + j, buffer + sizeof(FeatureRow) + sizeof(FeatureBit) + 1 - j, 1);
		}
		memcpy(regdata, FeatureRow, sizeof(FeatureRow));
		memcpy(regdata + sizeof(FeatureRow), FeatureBit, sizeof(FeatureBit));
		memcpy(regdata + sizeof(FeatureRow)+sizeof(FeatureBit), Usercode, sizeof(Usercode));
		memcpy(reg, &regdata, 16);
		return reg_len;
	}
	//LC4XXX,LCMXO,Powr6xx read to register
	if (wcscmp(chip->description, L"ispMach40xx") == 0 || wcscmp(chip->description, L"ispMACHXO") == 0
		|| wcscmp(chip->description, L"ispPAC") == 0)
	{
		unsigned char buffer[4];
		char Usercode[4];
		read(chip, 1, 0, buffer, 4, 1000, read_param);
		for (int j = 0; j < 4; j++)
		{
			memcpy(Usercode + j, buffer + 3 - j, 1);
		}
		memcpy(reg, &Usercode, 4);
		return reg_len;
	}
	//Nuvoton read to register
	if (wcscmp(chip->manufacturer, L"Nuvoton") == 0)
	{
		if (wcscmp(chip->description, L"Nu8051") == 0)
		{
			unsigned char buffer[4];
			read(chip, 2, 0, buffer, 4, 1000, read_param);
			memcpy(reg, &buffer, 4);
			return reg_len; 
		}

		if (wcscmp(chip->description, L"M05X") == 0)
		{
			unsigned char buffer[4];
			read(chip, 3, chip->partition_info[3].start_program_addr, buffer, 4, 1000, read_param);
			memcpy(reg, &buffer, 4);
			return reg_len;
		}

		if (
				wcscmp(chip->description, L"Mini51") == 0
				|| wcscmp(chip->description, L"M0518") == 0
				|| wcscmp(chip->description, L"NUC1X0ACN") == 0
				|| wcscmp(chip->description, L"ISD9XXX") == 0
				|| wcscmp(chip->description, L"NUC103") == 0
				|| wcscmp(chip->description, L"NUC100DN") == 0
			)
		{
			unsigned char buffer[8];
			read(chip, 3, chip->partition_info[3].start_program_addr, buffer, 8, 1000, read_param);
			memcpy(reg, &buffer, 8);
			return reg_len;
		}

	}


	
}
