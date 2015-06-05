// Dlg_ABOV_ALN.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Dlg_MC96F6632.h"
#include "Dlg_MC96FC664A.h"
#include "../common/prog_info.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern "C" __declspec(dllexport) CMFCPropertyPage * GetPropertyPage(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen)
{

	CMFCPropertyPage *d_page = NULL;
	if (wcscmp(_T("MC96F6632[QFP64]"), ChipInfo->part_name) == 0
		|| wcscmp(_T("MC96F6832L/Q[MQFP80]"), ChipInfo->part_name) == 0)
	{
		d_page = new CDlg_MC96F6632(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	if (wcscmp(_T("MC96FC664AL[QFP64]"), ChipInfo->part_name) == 0
		|| wcscmp(_T("MC96FC864AL[QFP80]"), ChipInfo->part_name) == 0)
	{
		d_page = new CDlg_MC96FC664A(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}

	return d_page;
}