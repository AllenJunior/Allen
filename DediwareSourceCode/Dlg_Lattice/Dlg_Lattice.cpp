// Dlg_Lattice.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Dlg_ispLattice.h"
#include "ispMachXO2.h"
#include "Dlg_iCE40.h"
#include"afxpropertypage.h"
#include "../common/prog_info.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
extern "C" __declspec(dllexport) CMFCPropertyPage * GetPropertyPage(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen)
{
	CMFCPropertyPage *d_page = NULL;

	if (wcscmp(_T("ispMachXO2"), ChipInfo->description) == 0
		|| wcscmp(_T("ispMachXO3L"), ChipInfo->description) == 0)
	{
		d_page = new Dlg_ispMachXO2(ChipInfo, RegisterBuff, BuffLen, pDataLen);
	}

	if (wcscmp(_T("ispPAC"), ChipInfo->description) == 0
		|| wcscmp(_T("ispMach40xx"), ChipInfo->description) == 0
		|| wcscmp(_T("ispMACHXO"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_ispLattice(ChipInfo, RegisterBuff, BuffLen, pDataLen);
	}

	if (wcscmp(_T("iCE40"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_iCE40(ChipInfo, RegisterBuff, BuffLen, pDataLen);
	}
		

	return d_page;
	
}