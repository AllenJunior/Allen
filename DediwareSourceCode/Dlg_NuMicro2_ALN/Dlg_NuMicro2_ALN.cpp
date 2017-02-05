// Dlg_NuMicro2_ALN.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "../common/prog_info.h"
#include "Dlg_NUC505.h"
#include "Dlg_NUC400.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern "C" __declspec(dllexport) CMFCPropertyPage * GetPropertyPage(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen)
{

	CMFCPropertyPage *d_page = NULL;
	if (wcscmp(_T("NUC505"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_NUC505(ChipInfo, RegisterBuff, BuffLen, pDataLen);
	}
	if (wcscmp(_T("NUC400"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_NUC400(ChipInfo, RegisterBuff, BuffLen, pDataLen);
	}

	return d_page;
}
