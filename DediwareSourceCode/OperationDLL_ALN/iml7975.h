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


int iML7975_Special(
	const struct chip_info_c                *chip,                  /* chip info structure */
	std::vector<struct loadfile_du>         &du_header,             /* du header */
	vm_ptr_t                                buff,
	loadfile_item                           **reg,
	unsigned long                           *chip_checksum,         /* loadfile --> chipchecksum */
	std::vector<struct loadfile_fcs>        &file_checksum,          /* file checksum */
	const struct loadfile_funcs				*funcs
	)
{
	unsigned char temp[2];
	loadfile_item *pStruct_Buffer;
	std::vector<unsigned char> vMemBuf((unsigned int)chip->partition_info[0].size_in_bytes);
	std::fill(vMemBuf.begin(), vMemBuf.end(), 0xFF);
	vm_read((unsigned char*)&vMemBuf[0] + du_header.at(0).ptn_addr, buff + du_header.at(0).data_offset_in_buff, du_header.at(0).program_length);
	memcpy(temp, &vMemBuf.at(0), 2);
	temp[1] |= 0xFF;
	vm_write(buff, temp, du_header.at(0).program_length);
	int checksum_temp = temp[0];
	*chip_checksum = checksum_temp;
	return 0;
}