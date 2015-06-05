// Dlg_ATMEL.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Dlg_Atxmega_A3B.h"
#include "../common/prog_info.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern "C" __declspec(dllexport) CMFCPropertyPage * GetPropertyPage(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen)
{

	CMFCPropertyPage *d_page = NULL;

	if (wcscmp(_T("ATxmegaA3BU"), ChipInfo->description) == 0){
		d_page = new CDlg_Atxmega_A3B(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}

	return d_page;
}