// dialogNuMicro.cpp : Defines the initialization routines for the DLL.
//
#include "stdafx.h"
#include "DlgMini51.h"
#include "Dlg_M05X.h"
#include "DLG_NUC1X0ACN.h"
#include "DLG_Nu8051.h"
#include "DLG_M0518.h"
#include "DLG_ISD9XXX.h"
#include "Dlg_N78E517.h"
#include "Dlg_NUC120DN.h"
#include "Dlg_NUC103.h"
#include "stdafx.h"
#include "../common/prog_info.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif 


extern "C" __declspec(dllexport) CMFCPropertyPage * GetPropertyPage(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen)
{
	CMFCPropertyPage *d_page = NULL;

	if (wcscmp(_T("Mini51"), ChipInfo->description) == 0){
		d_page = new CDlgMini51(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	else if (wcscmp(_T("M05X"), ChipInfo->description) == 0){
		d_page = new CDLG_M05X(ChipInfo, RegisterBuff, BuffLen, pDataLen);//4Bytes
	}
	else if (wcscmp(_T("NUC1X0ACN"), ChipInfo->description) == 0){
		d_page = new CDLG_NUC1X0ACN(ChipInfo, RegisterBuff, BuffLen, pDataLen);//4Bytes//NUC100
	}
	else if (wcscmp(_T("Nu8051"), ChipInfo->description) == 0){
		d_page = new CDLG_Nu8051(ChipInfo, RegisterBuff, BuffLen, pDataLen);//4Bytes
	}
	else if (wcscmp(_T("M0518"), ChipInfo->description) == 0){
		d_page = new CDLG_M0518(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	else if (wcscmp(_T("ISD9XXX"), ChipInfo->description) == 0){
		d_page = new CDLG_ISD9XXX(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	else if (wcscmp(_T("N78E"), ChipInfo->description) == 0){
		d_page = new CDlg_N78E517(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	else if (wcscmp(_T("NUC103"), ChipInfo->description) == 0){
		d_page = new CDlg_NUC103(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	else if (wcscmp(_T("NUC100DN"), ChipInfo->description) == 0){
		d_page = new CDlg_NUC120DN(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	return d_page;
}
