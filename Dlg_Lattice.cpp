// Dlg_Lattice.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Dlg_ispLattice.h"
#include "ispMachXO2.h"
#include "../common/prog_info.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
extern "C" __declspec(dllexport) CMFCPropertyPage * GetPropertyPage(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen)
{
	CMFCPropertyPage *d_page = NULL;
	if (wcscmp(_T("ispMachXO2"), ChipInfo->description) == 0)
	{
		d_page = new Dlg_ispMachXO2(ChipInfo, RegisterBuff, BuffLen, pDataLen);
	}
	else // ispMACH4XXX ; ispMACHXO ;ispPAC
		d_page = new CDlg_ispLattice(ChipInfo, RegisterBuff, BuffLen, pDataLen);

	return d_page;
	
}