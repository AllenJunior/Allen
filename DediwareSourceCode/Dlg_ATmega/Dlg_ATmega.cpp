// Dlg_ATmega.cpp : Defines the initialization routines for the DLL.
//
#include "stdafx.h"
#include "../common/prog_info.h"
#include "Dlg_ATmega8.h"
#include "Dlg_ATmega16.h"
#include "Dlg_ATmega128.h"
#include "Dlg_ATmegaU2.h"
#include "Dlg_ATmegaM1.h"
#include"Dlg_ATmega48.h"
#include "Dlg_ATmega88.h"
#include "Dlg_ATmega328.h"
#include "Dlg_ATmega162.h"
#include "Dlg_ATmegaX0X4.h"
#include "Dlg_ATmegaX5X9.h"
#include "Dlg_AT90CAN.h"
#include "Dlg_AT90PWMX.h"
#include "Dlg_AT90PWMX1.h"
#include "Dlg_AT90USB.h"
#include "Dlg_ATmegaHVA.h"
#include "Dlg_ATmegaHVB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern "C" __declspec(dllexport) CMFCPropertyPage * GetPropertyPage(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen)
{
	CMFCPropertyPage *d_page = NULL;
	//ATmega8/85X5
	if (wcscmp(_T("ATmega8"), ChipInfo->description) == 0
		|| wcscmp(_T("ATmega85X5"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_ATmega8(ChipInfo, RegisterBuff, BuffLen, pDataLen);
	}
	//ATmega16/32
	if (wcscmp(_T("ATmega16"), ChipInfo->description) == 0
		|| wcscmp(_T("ATmega32"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_ATmega16(ChipInfo, RegisterBuff, BuffLen, pDataLen);
	}
	//ATmega 64/128
	if (wcscmp(_T("ATmega64"), ChipInfo->description) == 0
		|| wcscmp(_T("ATmega128"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_ATmega128(ChipInfo, RegisterBuff, BuffLen, pDataLen);
	}
	//ATmegaU2/U4
	if (wcscmp(_T("ATmega_U2"), ChipInfo->description) == 0
		|| wcscmp(_T("ATmega_U4"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_ATmegaU2(ChipInfo, RegisterBuff, BuffLen, pDataLen);
	}
	//ATmegaM1
	if (wcscmp(_T("ATmega_M1"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_ATmegaM1(ChipInfo, RegisterBuff, BuffLen, pDataLen);
	}
	//ATmega48
	if (wcscmp(_T("ATmega48"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_ATmega48(ChipInfo, RegisterBuff, BuffLen, pDataLen);
	}
	//ATmega88_168
	if (wcscmp(_T("ATmegaX8"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_ATmega88(ChipInfo, RegisterBuff, BuffLen, pDataLen);
	}
	//ATmega328
	if (wcscmp(_T("ATmega328"), ChipInfo->description) == 0
		|| (wcscmp(_T("ATmega328PB"), ChipInfo->description) == 0))
	{
		d_page = new CDlg_ATmega328(ChipInfo, RegisterBuff, BuffLen, pDataLen);
	}
	//ATmega162
	if (wcscmp(_T("ATmega162"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_ATmega162(ChipInfo, RegisterBuff, BuffLen, pDataLen);
	}
	//ATmegaX0/ATmegaX4
	if (wcscmp(_T("ATmegaX0"), ChipInfo->description) == 0
		|| wcscmp(_T("ATmegaX4"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_ATmegaX0X4(ChipInfo, RegisterBuff, BuffLen, pDataLen);
	}
	//ATmegaX5/ATmegaX9
	if (wcscmp(_T("ATmegaX5"), ChipInfo->description) == 0
		|| wcscmp(_T("ATmegaX9"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_ATmegaX5X9(ChipInfo, RegisterBuff, BuffLen, pDataLen);
	}
	//AT90CAN
	if (wcscmp(_T("AT90CAN"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_AT90CAN(ChipInfo, RegisterBuff, BuffLen, pDataLen);
	}
	//AT90PWMX
	if ( wcscmp(_T("AT90PWMX"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_AT90PWMX(ChipInfo, RegisterBuff, BuffLen, pDataLen);
	}
	//AT90PWMX1
	if (wcscmp(_T("AT90PWMX1"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_AT90PWMX1(ChipInfo, RegisterBuff, BuffLen, pDataLen);
	}
	//AT90USBX2 / AT90USB64X / AT90USB128X
	if (wcscmp(_T("AT90USBX2"), ChipInfo->description) == 0
		|| wcscmp(_T("AT90USB64X"), ChipInfo->description) == 0
		|| wcscmp(_T("AT90USB128X"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_AT90USB(ChipInfo, RegisterBuff, BuffLen, pDataLen);
	}
	//ATmegaHVA
	if (wcscmp(_T("ATmegaHVA"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_ATmegaHVA(ChipInfo, RegisterBuff, BuffLen, pDataLen);
	}
	//ATmegaHVB
	if (wcscmp(_T("ATmegaHVB"), ChipInfo->description) == 0)
	{
		d_page = new CDlg_ATmegaHVB(ChipInfo, RegisterBuff, BuffLen, pDataLen);
	}
	return d_page;
}