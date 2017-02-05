// OperationDLL.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include <assert.h>
#include "tinyxml.h"
#include "tinystr.h"
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
//Manufacture Function (.h)
#include "IR35XX.h"
#include "MC96Fxxxx.h"
#include "LatticeUnique.h"
#include "iML7975.h"
#include "ST_PM67xxCRC.h"
#include "TPS40X22.h"
#include "TPS536X1.h"
#include "TPS536X9.h"
#include "TPS40428.h"
#include "Amiccom.h"
#include "LP8860.h"
#include "PrimarionFile.h"

int Calu_du_crc(std::vector<struct loadfile_du> &du_header, vm_ptr_t buff, const struct loadfile_funcs *funcs)
{
	int status;
	unsigned int ProgramLength = (unsigned int)du_header.at(0).program_length;
	unsigned int ImageLength = (unsigned int)du_header.at(0).image_length;
	unsigned char *pProgramFlash = (unsigned char*)malloc(ProgramLength);
	unsigned char *pImageFlash = (unsigned char*)malloc(ImageLength);

	memset(pProgramFlash, 0xFF, ProgramLength);
	status = vm_read(pProgramFlash, buff, ProgramLength);
	if (status != 0)return status;
	du_header.at(0).data_crc_value = funcs->du_crc(pProgramFlash, ProgramLength);


	memset(pImageFlash, 0xFF, ImageLength);
	status = vm_read(pImageFlash, buff, ImageLength);
	if (status != 0)return status;
	du_header.at(0).data_checksum_value = funcs->du_checksum(pImageFlash, ImageLength);
	du_header.at(0).du_crc = funcs->du_crc(pImageFlash, ImageLength);

	free(pProgramFlash);
	free(pImageFlash);

	return 0;
}

extern "C" __declspec(dllexport)  
int du_to_reg(
                const struct chip_info_c                *chip,                  /* chip info structure */
                std::vector<struct loadfile_du>         &du_header,             /* du header */
                vm_ptr_t                                buff,
				struct loadfile_item                           **reg,
                unsigned long                           *chip_checksum,         /* loadfile --> chipchecksum */
                std::vector<struct loadfile_fcs>        &file_checksum,          /* file checksum */
				const struct loadfile_funcs				*funcs
                )
{
        assert(chip);
        assert(reg);
        assert(chip_checksum);

		//check du_header NULL?
		if (du_header.empty())
			return 0;
		//AMICCOM
		if (wcscmp(chip->manufacturer, L"AMICCOM") == 0)
		{
			if (wcscmp(chip->description, L"A8XXX") == 0)
			{
				A8XXX_Chechsum(chip, du_header, buff, reg, chip_checksum, file_checksum, funcs);
			}
		}
		//iML7975 CRC 1Byte
		if (wcscmp(chip->part_name, L"iML7975[DFN10]") == 0)
		{
			iML7975_Special(chip, du_header, buff, reg, chip_checksum, file_checksum, funcs);
			Calu_du_crc(du_header, buff, funcs);
		}
		/////////ST MicroController
		if (wcscmp(chip->manufacturer, L"ST") == 0)
		{
			if (wcscmp(chip->description, L"PowerManager") == 0)
			{
				ST_CRC(chip, du_header, buff, reg, chip_checksum, file_checksum, funcs);
				Calu_du_crc(du_header, buff, funcs);
			}
		}
		//TI TPS536X1 xml Format
		if (wcscmp(chip->manufacturer, L"Texas Instruments") == 0)
		{
			if (wcscmp(chip->description, L"TPS536X1") == 0)
			{
				TPS536X1_XMLanalyze(chip, du_header, buff, reg, chip_checksum, file_checksum, funcs);
				Calu_du_crc(du_header, buff, funcs);
			}
			if (wcscmp(chip->description, L"TPS536X9") == 0)
			{
				TPS536X9_XMLanalyze(chip, du_header, buff, reg, chip_checksum, file_checksum, funcs);
				Calu_du_crc(du_header, buff, funcs);
			}
			if (wcscmp(chip->description, L"TPS40X22") == 0)
			{
				TPS40X22_XMLanalyze(chip, du_header, buff, reg, chip_checksum, file_checksum, funcs);
				Calu_du_crc(du_header, buff, funcs);
			}
			if (wcscmp(chip->description, L"TPS40428") == 0)
			{
				TPS40428_XMLanalyze(chip, du_header, buff, reg, chip_checksum, file_checksum, funcs);
				Calu_du_crc(du_header, buff, funcs);
			}
			if (wcscmp(chip->description, L"LP8860") == 0)
			{
				LP8860_EEPFile(chip, du_header, buff, reg, chip_checksum, file_checksum, funcs);
				Calu_du_crc(du_header, buff, funcs);
			}
		}
		/////////International Rectifier
		if ((wcscmp(chip->manufacturer, L"International Rectifier") == 0)
			|| (wcscmp(chip->manufacturer, L"Infineon") == 0))
		{
			if (
				(wcscmp(chip->description, L"C") == 0)
				|| (wcscmp(chip->description, L"E") == 0)
				|| (wcscmp(chip->description, L"M") == 0)
				)
			{
				PrimarionFile(chip, du_header, buff, reg, chip_checksum, file_checksum, funcs);
				Calu_du_crc(du_header, buff, funcs);
			}
			if (wcscmp(chip->description, L"Comanche") == 0)
			{
				ComancheFile(chip, du_header, buff, reg, chip_checksum, file_checksum, funcs);
				Calu_du_crc(du_header, buff, funcs);
			}
			if (wcscmp(chip->description, L"Baxter") == 0)
			{
				BaxterFile(chip, du_header, buff, reg, chip_checksum, file_checksum, funcs);
				Calu_du_crc(du_header, buff, funcs);
			}
			if (wcscmp(chip->description, L"Salem") == 0)
			{
				SalemFile(chip, du_header, buff, reg, chip_checksum, file_checksum, funcs);
				Calu_du_crc(du_header, buff, funcs);
			}
			if (wcscmp(chip->description, L"Manhattan") == 0)
			{
				ManhattanFile(chip, du_header, buff, reg, chip_checksum, file_checksum, funcs);
				Calu_du_crc(du_header, buff, funcs);
			}
			if (wcscmp(chip->description, L"Lucas") == 0)
			{
				LucasFile(chip, du_header, buff, reg, chip_checksum, file_checksum, funcs);
				Calu_du_crc(du_header, buff, funcs);
			}
		}
		/////////ABOV Semiconductor
		if (wcscmp(chip->manufacturer, L"ABOV Semiconductor") == 0)
		{
			//Only for Midea (MC96F6632 / MC96F6832L/Q)
			if (wcscmp(chip->description, L"MC96F6632") == 0)
			{
				MC96F6632(chip, du_header, buff, reg, chip_checksum, file_checksum, funcs);
			}
			//Only for Midea MC96F6432S
			if (
				(wcscmp(chip->description, L"MC96F6432") == 0)
				||	(wcscmp(chip->description, L"MC96F6432S") == 0)
				)
			{
				MC96F6432S(chip, du_header, buff, reg, chip_checksum, file_checksum, funcs);
			}
		}
		/////////LATTICE
		if (wcscmp(chip->manufacturer, L"LATTICE") == 0)
		{
			//Lattice FeatureRow/ Featurebit / userword
			if (
				(wcscmp(chip->description, L"ispMACHXO") == 0 )
				|| (wcscmp(chip->description, L"ispPAC") == 0)
				|| wcscmp(chip->description, L"ispMach40xx") == 0
				)
			{
				XOFeature(chip, du_header, buff, reg, chip_checksum, file_checksum);
			}
			if (
				(wcscmp(chip->description, L"ispMachXO2") == 0)
				|| (wcscmp(chip->description, L"ispMachXO3L") == 0)
				)
			{
				XO2Feature(chip, du_header, buff, reg, chip_checksum, file_checksum);
			}
			//Lattice Checksum
			if (
				wcscmp(chip->description, L"ispMachXO2") == 0
				|| wcscmp(chip->description, L"ispMACHXO") == 0
				|| wcscmp(chip->description, L"ispPAC") == 0
				|| wcscmp(chip->description, L"ispMachXO3L") == 0
				|| wcscmp(chip->description, L"ispMach40xx") == 0
				)
			{
				Cal_JEDChecksum(chip, du_header, buff, reg, chip_checksum, file_checksum);
			}
			// iCE40 Security Check / Catch Checksum
			if (wcscmp(chip->description, L"iCE40") == 0)
			{
				iCE40Feature(chip, du_header, buff, reg, chip_checksum, file_checksum);
			}
		}
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

	if (wcscmp(chip->manufacturer, L"AMICCOM") == 0)
	{
		if (wcscmp(chip->description, L"A8XXX") == 0)
		{
			unsigned char buffer[8];
			read(chip, chip->partition_cnt - 1, 0, buffer, chip->partition_info[4].size_in_bytes, 2000, read_param);
			memcpy(reg, &buffer, 8);
			return reg_len;
		}
	}

	if (wcscmp(chip->manufacturer, L"LATTICE") == 0)
	{
		//iCE40 Read to register
		if (wcscmp(chip->description, L"iCE40") == 0)
		{
			unsigned char temp[4];
			unsigned int buffer = 0x00;
			read(chip, 1, 0, temp, 4, 2000, read_param);
			if (temp[0] == 0x00)
			{
				buffer = 0x0;
			}
			else if (temp[0] == 0x30)
			{
				buffer = 0x1;
			}
			memcpy(reg, &buffer, 4);
			return reg_len;
		}
		//LCMXO2 Read to register
		if (wcscmp(chip->description, L"ispMachXO2") == 0
			|| wcscmp(chip->description, L"ispMachXO3L") == 0)
		{
			unsigned char buffer[14];
			unsigned char regdata[16];
			char FeatureRow[8];
			char FeatureBit[4];
			char Usercode[4];
			read(chip, 1, 0, buffer, 14, 2000, read_param);
			memcpy(FeatureRow, buffer, 8);
			memset(FeatureBit, 0, 4);
			for (int i = 0; i < 2; i++)
			{
				memcpy(FeatureBit + i, buffer + sizeof(FeatureRow)+1 - i, 1); //reverse take
			}
			for (int j = 0; j < 4; j++)
			{
				memcpy(Usercode + j, buffer + sizeof(FeatureRow)+sizeof(FeatureBit)+1 - j, 1);
			}
			memcpy(regdata, FeatureRow, sizeof(FeatureRow));
			memcpy(regdata + sizeof(FeatureRow), FeatureBit, sizeof(FeatureBit));
			memcpy(regdata + sizeof(FeatureRow)+sizeof(FeatureBit), Usercode, sizeof(Usercode));
			memcpy(reg, &regdata, 16);
			//Disable Security checkbox
			unsigned int fixint = 0x00;
			memcpy(reg + sizeof(regdata), &fixint, sizeof(fixint));
			return reg_len;
		}
		//LC4XXX,LCMXO,Powr6xx read to register
		if (wcscmp(chip->description, L"ispMach40xx") == 0 || wcscmp(chip->description, L"ispMACHXO") == 0
			|| wcscmp(chip->description, L"ispPAC") == 0)
		{
			unsigned char buffer[4];
			char Usercode[4];
			read(chip, 1, 0, buffer, 4, 2000, read_param);
			for (int j = 0; j < 4; j++)
			{
				memcpy(Usercode + j, buffer + 3 - j, 1);
			}
			memcpy(reg, &Usercode, 4);
			//Disable Security checkbox
			unsigned int fixint = 0x00;
			memcpy(reg + sizeof(Usercode), &fixint, sizeof(fixint));
			return reg_len;
		}
	}

	//Nuvoton read to register
	if (wcscmp(chip->manufacturer, L"Nuvoton") == 0)
	{
		if (wcscmp(chip->description, L"Nu8051") == 0)
		{
			unsigned char buffer[4];
			read(chip, 2, 0, buffer, 4, 2000, read_param);
			memcpy(reg, &buffer, 4);
			return reg_len; 
		}
		if (wcscmp(chip->description, L"N79E7X") == 0)
		{
			unsigned char buffer[8];
			read(chip, 2, 0, buffer, 8, 2000, read_param);
			memcpy(reg, &buffer, 8);
			return reg_len;
		}
		if (wcscmp(chip->description, L"M05X") == 0
			|| wcscmp(chip->description, L"M05XD") == 0)
		{
			unsigned char buffer[4];
			read(chip, 3, chip->partition_info[3].start_program_addr, buffer, 4, 2000, read_param);
			memcpy(reg, &buffer, 4);
			return reg_len;
		}
		if (
				wcscmp(chip->description, L"Mini51") == 0
				|| wcscmp(chip->description, L"Mini51D") == 0
				|| wcscmp(chip->description, L"Mini58") == 0
				|| wcscmp(chip->description, L"M0518") == 0
				|| wcscmp(chip->description, L"M0519") == 0
				|| wcscmp(chip->description, L"NUC1X0ACN") == 0
				|| wcscmp(chip->description, L"ISD9XXX") == 0
				|| wcscmp(chip->description, L"NUC103") == 0
				|| wcscmp(chip->description, L"NUC100DN") == 0
				|| wcscmp(chip->description, L"Nano1x0AN") == 0
				|| wcscmp(chip->description, L"Nano1x0BN") == 0
				|| wcscmp(chip->description, L"Nano102") == 0
				|| wcscmp(chip->description, L"NVT8899") == 0
				|| wcscmp(chip->description, L"M451AE") == 0
			)
		{
			unsigned char buffer[8];
			read(chip, 3, chip->partition_info[3].start_program_addr, buffer, 8, 2000, read_param);
			memcpy(reg, &buffer, 8);
			return reg_len;
		}
		if (
			wcscmp(chip->description, L"NUC400") == 0
			)
		{
			unsigned char buffer[16];
			read(chip, 3, chip->partition_info[3].start_program_addr, buffer, 16, 2000, read_param);
			memcpy(reg, &buffer, 16);
			return reg_len;
		}
	}
	//ATMEL
	if ((wcscmp(chip->manufacturer, L"Atmel") == 0) || (wcscmp(chip->manufacturer, L"BOSCH") == 0))
	{
		if (wcscmp(chip->description, L"AT89S") == 0)
		{
			unsigned char buffer[4] = {0x00};
			read(chip, chip->partition_cnt - 1, 0, buffer, chip->partition_info[1].size_in_bytes, 2000, read_param);
			if (buffer[0] == 0x00)
				buffer[0] = 0x00;
			else if (buffer[0] == 0x04)
				buffer[0] = 0x01;
			else if (buffer[0] == 0x08)
				buffer[0] = 0x02;
			else if (buffer[0] == 0x10)
				buffer[0] = 0x03;
			memcpy(reg, &buffer, sizeof(buffer));
			return reg_len;
		}
		if ( wcscmp(chip->description, L"ATxmegaAU") == 0
			|| wcscmp(chip->description, L"ATxmegaA") == 0
			|| wcscmp(chip->description, L"ATxmegaB") == 0
			|| wcscmp(chip->description, L"ATxmegaC") == 0
			|| wcscmp(chip->description, L"ATxmegaD") == 0
			|| wcscmp(chip->description, L"ATxmegaE") == 0
			)
		{
			unsigned char buffer[6];
			read(chip, chip->partition_cnt - 1, 0, buffer, chip->partition_info[4].size_in_bytes, 2000, read_param);
			memcpy(reg, &buffer, 6);
			return reg_len;
		}
		if (
			wcscmp(chip->description, L"ATtinyX") == 0
			|| wcscmp(chip->description, L"ATtiny1X") == 0
			)
		{
			unsigned char buffer[4];
			read(chip, chip->partition_cnt - 1, 0, buffer, chip->partition_info[1].size_in_bytes, 2000, read_param);
			memcpy(reg, &buffer, 4);
			return reg_len;
		}
		if (
			//ATtiny Series
			wcscmp(chip->description, L"ATtinyISP1") == 0          //ATtiny2313A
			|| wcscmp(chip->description, L"ATtinyISP2_1") == 0   // ATtiny24/44/84
			|| wcscmp(chip->description, L"ATtinyISP2_2") == 0   // ATtiny25/45/85
			|| wcscmp(chip->description, L"ATtinyISP2_3") == 0   // ATtiny43U
			|| wcscmp(chip->description, L"ATtinyISP2_4") == 0   // ATtiny261/461/861
			|| wcscmp(chip->description, L"ATtinyISP2_5") == 0   // ATtiny87/167
			|| wcscmp(chip->description, L"ATtinyISP2_6") == 0   // ATtiny48/88
			|| wcscmp(chip->description, L"ATtiny13") == 0			 // ATtiny13
			|| wcscmp(chip->description, L"ATtiny26") == 0			// ATtiny26
			|| wcscmp(chip->description, L"ATtinyX41") == 0		// ATtinyX41
			|| wcscmp(chip->description, L"ATtiny828") == 0		// ATtiny828
			|| wcscmp(chip->description, L"ATtiny1634") == 0		// ATtiny1634
			//ATmega Series
			//ATmegaV1
			|| wcscmp(chip->description, L"ATmega64") == 0
			|| wcscmp(chip->description, L"ATmega128") == 0
			//ATmegaV2
			|| wcscmp(chip->description, L"ATmega8") == 0
			|| wcscmp(chip->description, L"ATmega16") == 0
			|| wcscmp(chip->description, L"ATmega32") == 0
			|| wcscmp(chip->description, L"ATmega85X5") == 0
			|| wcscmp(chip->description, L"ATmegaHVA") == 0
			|| wcscmp(chip->description, L"ATmegaHVB") == 0
			//ATmegaV3
			|| wcscmp(chip->description, L"ATmega_U2") == 0
			|| wcscmp(chip->description, L"ATmega_U4") == 0
			|| wcscmp(chip->description, L"ATmega_M1") == 0
			|| wcscmp(chip->description, L"ATmega48") == 0
			|| wcscmp(chip->description, L"ATmegaX8") == 0
			|| wcscmp(chip->description, L"ATmega328") == 0
			|| wcscmp(chip->description, L"ATmega328PB") == 0
			|| wcscmp(chip->description, L"ATmega162") == 0
			|| wcscmp(chip->description, L"ATmegaX0") == 0
			|| wcscmp(chip->description, L"ATmegaX4") == 0
			|| wcscmp(chip->description, L"ATmegaX5") == 0
			|| wcscmp(chip->description, L"ATmegaX9") == 0
			//AT90 Series
			|| wcscmp(chip->description, L"AT90CAN") == 0
			|| wcscmp(chip->description, L"AT90PWMX") == 0
			|| wcscmp(chip->description, L"AT90PWMX1") == 0
			|| wcscmp(chip->description, L"AT90USBX2") == 0
			|| wcscmp(chip->description, L"AT90USB64X") == 0
			|| wcscmp(chip->description, L"AT90USB128X") == 0
			)
		{
			unsigned char buffer[4];
			read(chip, chip->partition_cnt - 1, 0, buffer, chip->partition_info[2].size_in_bytes, 2000, read_param);
			memcpy(reg, &buffer, 4);
			return reg_len;
		}
		if (wcscmp(chip->description, L"SAMCXX") == 0
			|| wcscmp(chip->description, L"SAMDXX") == 0
			|| wcscmp(chip->description, L"SAMLXX") == 0
			|| wcscmp(chip->description, L"SAMRXX") == 0
			|| wcscmp(chip->description, L"SAMG5X") == 0
			)
		{
			unsigned char buffer[12];
			read(chip, chip->partition_cnt - 1, 0, buffer, chip->partition_info[1].size_in_bytes, 2000, read_param);
			memcpy(reg, &buffer, 12);
			return reg_len;
		}
		if (wcscmp(chip->description, L"SAM3S") == 0
			|| wcscmp(chip->description, L"SAM3N") == 0
			|| wcscmp(chip->description, L"SAM3U") == 0
			|| wcscmp(chip->description, L"SAM3A/X") == 0
			)
		{
			unsigned char buffer[8];
			read(chip, chip->partition_cnt - 1, 0, buffer, chip->partition_info[1].size_in_bytes, 2000, read_param);
			memcpy(reg, &buffer, 8);
			return reg_len;
		}
	}
	//ABOV
	if (wcscmp(chip->manufacturer, L"ABOV Semiconductor") == 0)
	{
		if (wcscmp(chip->description, L"MC96F6432") == 0)
		{
			unsigned char temp[28] = { 0x00 };
			unsigned int crc_enable = 0x00;
			unsigned int id_enable = 0x00;
			unsigned int checksum = 0x00;
			unsigned char uid[6] = { 0x00 };
			unsigned char to_register[28] = { 0x00 };
			read(chip, 1, 0, temp, chip->partition_info[1].size_in_bytes, 2000, read_param);
			//config
			memcpy(to_register, temp, 4);
			//crc checkbox
			memcpy(&checksum, temp + 8, 4);
			if (checksum == 0)
				crc_enable = 0;
			else if (checksum != 0)
				crc_enable = 1;
			memcpy(to_register + 4, &crc_enable, sizeof(crc_enable));
			//checksum
			memcpy(to_register + 4 + sizeof(crc_enable), &checksum, sizeof(checksum));
			//uid
			memcpy(uid, temp + 4 + sizeof(crc_enable)+sizeof(checksum)+8, sizeof(uid));
			for (unsigned int i = 0; i < 6; i++)
			{
				if (uid[i] == 0x00)
				{
					id_enable = 0;
					continue;
				}
				else if (uid[i] != 0x00)
				{
					id_enable = 1;
					break;
				}
			}
			memcpy(to_register + 4 + sizeof(crc_enable)+sizeof(checksum), &id_enable, sizeof(id_enable));
			memcpy(to_register + 4 + sizeof(crc_enable)+sizeof(checksum)+sizeof(id_enable)+4, &uid, sizeof(uid));
			memcpy(reg, &to_register, sizeof(to_register));
			return reg_len;
		}
		if (wcscmp(chip->description, L"MC96F6432A") == 0)
		{
			unsigned char buffer[4] = {0x00};
			//unsigned char temp[2];
			//memset(buffer, 0x00, 4);
			//memcpy(buffer, &temp[0], 1);
			read(chip, 1, 0, buffer, 2, 2000, read_param);
			memcpy(reg, &buffer, 4);
			return reg_len;
		}
		//For MC96F6432S
		if (wcscmp(chip->description, L"MC96F6432S") == 0)
		{
			unsigned char temp[12] = { 0x00 };
			unsigned int enable_check = 0x00;
			unsigned int checksum = 0x00;
			unsigned char toregister[12] = { 0x00 };
			read(chip, 1, 0, temp, chip->partition_info[1].size_in_bytes, 2000, read_param);
			//configdata
			memcpy(toregister, temp, 4);
			memcpy(&checksum, temp + 8, 4);
			if (checksum == 0)
				enable_check = 0;
			else if (checksum != 0)
				enable_check = 1;
			memcpy(toregister + 4, &enable_check, sizeof(enable_check));
			memcpy(toregister + 4 + sizeof(enable_check), &checksum, sizeof(checksum));
			memcpy(reg, &toregister, sizeof(toregister));
			return reg_len;
		}
		//Only for Midea
		if (wcscmp(chip->description, L"MC96F6632") == 0)
		{
			unsigned char temp[10];
			unsigned char configdata[4];
			unsigned char UserID[6];
			unsigned char CRC[2];
			unsigned char button_CMP[2] = { 0x00, 0x00 };
			unsigned char buffer[24];
			memset(configdata, 0x00, 4);
			memset(temp, 0x00, 10);
			memset(buffer, 0x00, 24);
			read(chip, 1, 0, temp, 10, 1000, read_param);
			//configData
			memcpy(configdata, temp, 2);
			memcpy(buffer, configdata, sizeof(configdata));
			//checkbutton
			memcpy(CRC, temp + 8, 2);
			int check = memcmp(button_CMP, CRC, 2);
			if (check == 0)
			{
				memcpy(buffer + sizeof(configdata), &check, sizeof(check));
			}
			else if (check != 0)
			{
				int check = 0x01;
				memcpy(buffer + sizeof(configdata), &check, sizeof(check));
			}
			//UserID
			for (int i = 0; i < 6; i++)
			{
				memcpy(&UserID[5 - i], temp + 2 + i, 1);
			}
			memcpy(buffer + 18, UserID, sizeof(UserID));
			//CRC
			memcpy(buffer + 12, temp + 8, 2);
			memcpy(reg, &buffer, 24);
			return reg_len;
		}
	//For ABOV MC96FC664AL
		if (wcscmp(chip->description, L"MC96FC664AL") == 0)
		{
			unsigned char buffer[4];
			unsigned char temp[2];
			memset(buffer, 0x00, 4);
			read(chip, 1, 0, temp, 2, 2000, read_param);
			memcpy(buffer, &temp[0], 1);
			memcpy(reg, &buffer, 4);
			return reg_len;
		}
	}
}
