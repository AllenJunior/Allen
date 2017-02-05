#include "../Log/log.h"
#include "../common/comm_type.h"
#include "../common/loadfile_info.h"
#include "../common/prog_info.h"
#include "../vm_mgr/vm_mgr.h"


int A8XXX_Chechsum(
	const struct chip_info_c                *chip,                  /* chip info structure */
	std::vector<struct loadfile_du>         &du_header,             /* du header */
	vm_ptr_t                                buff,
	loadfile_item                           **reg,
	unsigned long                           *chip_checksum,         /* loadfile --> chipchecksum */
	std::vector<struct loadfile_fcs>        &file_checksum,          /* file checksum */
	const struct loadfile_funcs				*funcs
	)
{
	unsigned char checkbox = 0x00;
	unsigned char bank = 0x00;
	unsigned short int Addr = 0x0000;
	unsigned short int checksum = 0x0000;
	unsigned short int fix = 0x0000;
	loadfile_item *pStruct_Buffer;
	unsigned reg_len = sizeof(bank)+sizeof(Addr)+sizeof(checksum)+sizeof(checkbox)+sizeof(fix);
	pStruct_Buffer = (loadfile_item*)malloc(sizeof(loadfile_item)+reg_len);
	checksum = *chip_checksum;
	checksum &= 0xFFFF;
	memcpy(pStruct_Buffer->buff, &checkbox, sizeof(checkbox));
	memcpy(pStruct_Buffer->buff + sizeof(checkbox), &bank, sizeof(bank));
	memcpy(pStruct_Buffer->buff + sizeof(checkbox)+sizeof(bank), &Addr, sizeof(Addr));
	memcpy(pStruct_Buffer->buff + sizeof(checkbox)+sizeof(bank)+sizeof(Addr), &checksum, sizeof(checksum));
	memcpy(pStruct_Buffer->buff + sizeof(checkbox)+sizeof(bank)+sizeof(Addr)+sizeof(checksum), &fix, sizeof(fix));
	pStruct_Buffer->len = reg_len;
	*reg = pStruct_Buffer;
	return 0;
}