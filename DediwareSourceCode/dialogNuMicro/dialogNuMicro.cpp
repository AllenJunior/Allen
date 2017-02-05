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
#include "Dlg_Nano1X0AN.h"
#include "Dlg_Nano1X0BN.h"
#include "Dlg_Mini58.h"
#include "Dlg_M05XD.h"
#include "Dlg_Mini51D.h"
#include "Dlg_M451AE.h"
#include"Dlg_N79E7X.h"
#include "Dlg_M0519.h"
#include "Dlg_Nano102.h"

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
	else if(wcscmp(_T("Mini51D"), ChipInfo->description) == 0){
		d_page = new CDlg_Mini51D(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	else if (wcscmp(_T("Mini58"), ChipInfo->description) == 0){
		d_page = new CDlg_Mini58(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	else if (wcscmp(_T("M05X"), ChipInfo->description) == 0){
		d_page = new CDLG_M05X(ChipInfo, RegisterBuff, BuffLen, pDataLen);//4Bytes
	}
	else if (wcscmp(_T("M05XD"), ChipInfo->description) == 0){
		d_page = new CDlg_M05XD(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
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
	else if (wcscmp(_T("M0519"), ChipInfo->description) == 0){
		d_page = new CDlg_M0519(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
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
	else if (wcscmp(_T("Nano1x0AN"), ChipInfo->description) == 0){
		d_page = new CDlg_Nano1X0AN(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	else if (wcscmp(_T("Nano1x0BN"), ChipInfo->description) == 0){
		d_page = new CDlg_Nano1X0BN(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	else if ((wcscmp(_T("Nano102"), ChipInfo->description) == 0)
		|| (wcscmp(_T("NVT8899"), ChipInfo->description) == 0)){
		d_page = new CDlg_Nano102(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	else if (wcscmp(_T("M451AE"), ChipInfo->description) == 0){
		d_page = new CDlg_M451AE(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	else if (wcscmp(_T("N79E7X"), ChipInfo->description) == 0){
		d_page = new CDlg_N79E7X(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	return d_page;
}
