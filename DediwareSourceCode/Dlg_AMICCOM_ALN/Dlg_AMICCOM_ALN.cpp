// Dlg_AMICCOM_ALN.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "../common/prog_info.h"
#include "Dlg_A8XXX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
extern "C" __declspec(dllexport) CMFCPropertyPage * GetPropertyPage(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen)
{

	CMFCPropertyPage *d_page = NULL;
	if (wcscmp(_T("A8XXX"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_A8XXX(ChipInfo, RegisterBuff, BuffLen, pDataLen);//12Bytes
	}

	return d_page;
}

