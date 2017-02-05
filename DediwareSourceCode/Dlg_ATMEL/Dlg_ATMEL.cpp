// Dlg_ATMEL.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Dlg_ATxmegaA.h"		// ATxmega-A series
#include "Dlg_Atxmega_A3B.h"	// ATxmega-AU series
#include "Dlg_ATxmegaBCD.h"		// ATxmega-B/C/D series
#include "Dlg_ATxmegaE.h"		// ATxmega-E series
#include "Dlg_ATtinyX.h"		// ATtiny 4/5/9/10
#include "Dlg_ATSAMD.h"			// ATSAMD series
#include "Dlg_SAM3S.h"
#include "Dlg_SAMG5X.h"
#include "Dlg_ATtinyISP_1.h"
#include "Dlg_ATtinyISP_2.h"
#include "Dlg_ATtiny13.h"
#include "Dlg_ATtiny26.h"
#include "Dlg_ATtinyX41.h"
#include "Dlg_ATtiny828.h"
#include "Dlg_AT89S.h"
#include "../common/prog_info.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern "C" __declspec(dllexport) CMFCPropertyPage * GetPropertyPage(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen)
{

	CMFCPropertyPage *d_page = NULL;
	
	//ATxmega-A series
	if (wcscmp(_T("ATxmegaA"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_ATxmegaA(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	//ATxmega-AU series
	if (wcscmp(_T("ATxmegaAU"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_Atxmega_A3B(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	//ATxmega-B/C/D series
	if (wcscmp(_T("ATxmegaB"), ChipInfo->description) == 0
		|| wcscmp(_T("ATxmegaC"), ChipInfo->description) == 0
		|| wcscmp(_T("ATxmegaD"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_ATxmegaBCD(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	//ATxmega-E series
	if (wcscmp(_T("ATxmegaE"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_ATxmegaE(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	//ATtiny 4/5/9/10/20/40
	if (wcscmp(_T("ATtinyX"), ChipInfo->description) == 0 
		|| wcscmp(_T("ATtiny1X"), ChipInfo->description) == 0
		)
	{
		d_page = new CDlg_ATtinyX(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	//ATtiny ISP dialog type 1(ATtiny2313A)
	if (wcscmp(_T("ATtinyISP1"), ChipInfo->description) == 0){
		d_page = new CDlg_ATtinyISP_1(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	//ATtiny ISP dialog type 2
	if (wcscmp(_T("ATtinyISP2_1"), ChipInfo->description) == 0			//ATtiny24/44/84
		|| wcscmp(_T("ATtinyISP2_2"), ChipInfo->description) == 0      //ATtiny25/45/85
		|| wcscmp(_T("ATtinyISP2_3"), ChipInfo->description) == 0      //ATtiny43
		|| wcscmp(_T("ATtinyISP2_4"), ChipInfo->description) == 0      //ATtiny261/461/861
		|| wcscmp(_T("ATtinyISP2_5"), ChipInfo->description) == 0      //ATtiny87/167
		|| wcscmp(_T("ATtinyISP2_6"), ChipInfo->description) == 0      //ATtiny48/88
		)
	{
		d_page = new CDlg_ATtinyISP_2(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	//ATtiny 13V
	if (wcscmp(_T("ATtiny13"), ChipInfo->description) == 0){
		d_page = new CDlg_ATtiny13(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	//ATtiny26
	if (wcscmp(_T("ATtiny26"), ChipInfo->description) == 0){
		d_page = new CDlg_ATtiny26(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	//ATtinyX41
	if (wcscmp(_T("ATtinyX41"), ChipInfo->description) == 0
		|| wcscmp(_T("ATtiny1634"), ChipInfo->description) == 0){
		d_page = new CDlg_ATtinyX41(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	//ATtiny828
	if (wcscmp(_T("ATtiny828"), ChipInfo->description) == 0){
		d_page = new CDlg_ATtiny828(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	//ATSAM C/D/L/R  CortexM0+
	if (wcscmp(_T("SAMCXX"), ChipInfo->description) == 0
		|| wcscmp(_T("SAMDXX"), ChipInfo->description) == 0
		|| wcscmp(_T("SAMLXX"), ChipInfo->description) == 0
		|| wcscmp(_T("SAMRXX"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_ATSAMD(ChipInfo, RegisterBuff, BuffLen, pDataLen);//12Bytes
	}
	//ATSAM3S CortexM3
	if (wcscmp(_T("SAM3S"), ChipInfo->description) == 0
		|| wcscmp(_T("SAM3N"), ChipInfo->description) == 0
		|| wcscmp(_T("SAM3U"), ChipInfo->description) == 0
		|| wcscmp(_T("SAM3A/X"), ChipInfo->description) == 0
		){
		d_page = new CDlg_SAM3S(ChipInfo, RegisterBuff, BuffLen, pDataLen);//8Bytes
	}
	//ATSAMG55 CortexM4
	if (wcscmp(_T("SAMG5X"), ChipInfo->description) == 0){
		d_page = new CDlg_SAMG5X(ChipInfo, RegisterBuff, BuffLen, pDataLen);//12Bytes
	}
	if (wcscmp(_T("AT89S"), ChipInfo->description) == 0){
		d_page = new CDlg_AT89S(ChipInfo, RegisterBuff, BuffLen, pDataLen);//12Bytes
	}
	return d_page;
}