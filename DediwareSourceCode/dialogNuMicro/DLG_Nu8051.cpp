// DLG_Nu8051.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_Nu8051.h"
#include "afxdialogex.h"
#include "../common/prog_info.h"
#include "numeric_conversion.h"

// DLG_Nu8051 dialog

IMPLEMENT_DYNAMIC(CDLG_Nu8051, CMFCPropertyPage)

CDLG_Nu8051::CDLG_Nu8051(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDLG_Nu8051::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDLG_Nu8051::~CDLG_Nu8051()
{
}

void CDLG_Nu8051::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_BootSelect);
	DDX_Control(pDX, IDC_COMBO2, m_Lock);
	DDX_Control(pDX, IDC_COMBO3, m_BroDect);
	DDX_Control(pDX, IDC_COMBO4, m_BroVol);
	DDX_Control(pDX, IDC_COMBO5, m_BroReset);
	DDX_Control(pDX, IDC_COMBO6, m_watchdog);
	DDX_Control(pDX, IDC_COMBO7, m_CLKFilter);
	DDX_Control(pDX, IDC_COMBO8, m_InternalRC);
	DDX_Control(pDX, IDC_COMBO9, m_CLKSource);
	DDX_Control(pDX, IDC_COMBO10, m_DataFlash);
	DDX_Control(pDX, IDC_EDIT1, m_Cfg0);
	DDX_Control(pDX, IDC_EDIT2, m_Cfg1);
	DDX_Control(pDX, IDC_EDIT3, m_Cfg2);
	DDX_Control(pDX, IDC_EDIT4, m_Cfg3);
	DDX_Control(pDX, IDC_EDIT5, m_Cfg1);
}


BEGIN_MESSAGE_MAP(CDLG_Nu8051, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDLG_Nu8051::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDLG_Nu8051::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDLG_Nu8051::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDLG_Nu8051::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDLG_Nu8051::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDLG_Nu8051::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDLG_Nu8051::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CDLG_Nu8051::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO9, &CDLG_Nu8051::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO10, &CDLG_Nu8051::OnCbnSelchangeCombo)
	ON_EN_CHANGE(IDC_EDIT5, &CDLG_Nu8051::OnEnChangeEdit5)
END_MESSAGE_MAP()

BOOL CDLG_Nu8051::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	Val_Cfg0 = 0xFF;
	Val_Cfg1 = 0xFF;
	Val_Cfg2 = 0xFF;
	Val_Cfg3 = 0xFF;
	m_Cfg0.SetWindowTextW(L"FF");
	m_Cfg1.SetWindowTextW(L"FF");
	m_Cfg2.SetWindowTextW(L"FF");
	m_Cfg3.SetWindowTextW(L"FF");
	GetDlgItem(IDC_EDIT5)->SetWindowTextW(L"FF");
	m_Lock.InsertString(m_Lock.GetCount(), L"0 = Chip is Locked");
	m_Lock.InsertString(m_Lock.GetCount(), L"1 = Chip is unLocked");
	m_BootSelect.InsertString(m_BootSelect.GetCount(), L"0 = MCU Boot from LDROM");
	m_BootSelect.InsertString(m_BootSelect.GetCount(), L"1 = MCU Boot from APROM");
	m_BroDect.InsertString(m_BroDect.GetCount(), L"0 = Enable BOD detection");
	m_BroDect.InsertString(m_BroDect.GetCount(), L"1 = Disable BOD detection");
	m_BroVol.InsertString(m_BroVol.GetCount(), L"0 = 3.8V");
	m_BroVol.InsertString(m_BroVol.GetCount(), L"1 = 2.7V");
	m_BroReset.InsertString(m_BroReset.GetCount(), L"0 = Disable BOD Reset when VDD drops below VBOD");
	m_BroReset.InsertString(m_BroReset.GetCount(), L"1 = Enable BOD Reset when VDD drops below VBOD");
	m_watchdog.InsertString(m_watchdog.GetCount(), L"0 = Enable Watchdog Timer after all resets");
	m_watchdog.InsertString(m_watchdog.GetCount(), L"1 = Disable Watchdog Timer after all resets");
	m_CLKFilter.InsertString(m_CLKFilter.GetCount(), L"0 = Disable Clock Filter");
	m_CLKFilter.InsertString(m_CLKFilter.GetCount(), L"1 = Enable Clock Filter");
	m_InternalRC.InsertString(m_InternalRC.GetCount(), L"0 = 11.0592MHz");
	m_InternalRC.InsertString(m_InternalRC.GetCount(), L"1 = 22.1184MHz");
	m_CLKSource.InsertString(m_CLKSource.GetCount(), L"00 = External Crystal,4MHz~24MHz");
	m_CLKSource.InsertString(m_CLKSource.GetCount(), L"10 = Reserved");
	m_CLKSource.InsertString(m_CLKSource.GetCount(), L"11 = Internal RC oscillator");
	//N79E845 Only
	m_DataFlash.InsertString(m_DataFlash.GetCount(), L"0 = DataFlash exists");
	m_DataFlash.InsertString(m_DataFlash.GetCount(), L"1 = There's no DataFlash");
	m_DataFlash.SetCurSel(1);
	if (
		   wcscmp(_T("N79E845A[TSSOP20]"), m_ChipInfo->part_name) == 0
		|| wcscmp(_T("N79E855A[TSSOP28]"), m_ChipInfo->part_name) == 0
		|| wcscmp(_T("N79E815A[TSSOP20]"), m_ChipInfo->part_name) == 0
		|| wcscmp(_T("N79E815A[SOP20]"), m_ChipInfo->part_name) == 0
		|| wcscmp(_T("N79E815A[TSSOP28]"), m_ChipInfo->part_name) == 0
		|| wcscmp(_T("N79E815A[SOP28]"), m_ChipInfo->part_name) == 0
		)
	{
		GetDlgItem(IDC_COMBO10)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_COMBO10)->EnableWindow(false);
	}
	GetDlgItem(IDC_EDIT5)->EnableWindow(false);
	//
	m_Lock.SetCurSel(1);
	m_BootSelect.SetCurSel(1);
	m_BroDect.SetCurSel(1);
	m_BroVol.SetCurSel(1);
	m_BroReset.SetCurSel(1);
	m_watchdog.SetCurSel(1);
	m_CLKFilter.SetCurSel(1);
	m_InternalRC.SetCurSel(1);
	m_CLKSource.SetCurSel(2);
	return TRUE;
}

BOOL CDLG_Nu8051::OnSetActive()
{
	CString Cfg0;
	CString Cfg1;
	CString Cfg2;
	CString Cfg3; 
	if (*m_pDataLen != NULL){
		memcpy((void*)&Val_Cfg0, m_RegBuff, 1);
		memcpy((void*)&Val_Cfg1, m_RegBuff + 1, 1);
		memcpy((void*)&Val_Cfg2, m_RegBuff + 2, 1);
		memcpy((void*)&Val_Cfg3, m_RegBuff + 3, 1);

		//CFG0
		Cfg0.Format(_T("%02X"), Val_Cfg0);
		GetDlgItem(IDC_EDIT1)->SetWindowTextW(Cfg0);
		//CFG1
		Cfg1.Format(_T("%02X"), Val_Cfg1);
		GetDlgItem(IDC_EDIT2)->SetWindowTextW(Cfg1);
		GetDlgItem(IDC_EDIT5)->SetWindowTextW(Cfg1);
		//CFG2
		Cfg2.Format(_T("%02X"), Val_Cfg2);
		GetDlgItem(IDC_EDIT3)->SetWindowTextW(Cfg2);
		//CFG3
		Cfg3.Format(_T("%02X"), Val_Cfg3);
		GetDlgItem(IDC_EDIT4)->SetWindowTextW(Cfg3);

		//Lock
		if ((Val_Cfg0 & 0x02) == 0x02)
			m_Lock.SetCurSel(1);
		else if ((Val_Cfg0 & 0x02) == 0x00)
			m_Lock.SetCurSel(0);
		//Boot Select
		if ((Val_Cfg0 & 0x80) == 0x80)
			m_BootSelect.SetCurSel(1);
		else if ((Val_Cfg0 & 0x80) == 0x00)
			m_BootSelect.SetCurSel(0);
		//DataFlash_N79E845_Only----------------
		if (
			wcscmp(_T("N79E845A[TSSOP20]"), m_ChipInfo->part_name) == 0
			|| wcscmp(_T("N79E855A[TSSOP28]"), m_ChipInfo->part_name) == 0
			|| wcscmp(_T("N79E815A[TSSOP20]"), m_ChipInfo->part_name) == 0
			|| wcscmp(_T("N79E815A[SOP20]"), m_ChipInfo->part_name) == 0
			|| wcscmp(_T("N79E815A[TSSOP28]"), m_ChipInfo->part_name) == 0
			|| wcscmp(_T("N79E815A[SOP28]"), m_ChipInfo->part_name) == 0
			)
		{
			if ((Val_Cfg0 &= 0x01) == 0x01)
			{
				m_DataFlash.SetCurSel(1);
				GetDlgItem(IDC_EDIT5)->EnableWindow(false);
			}
			else if ((Val_Cfg0 &= 0x00) == 0x00)
			{
				m_DataFlash.SetCurSel(0);
				GetDlgItem(IDC_EDIT5)->EnableWindow(true);
			}
		}
		else
		{
			m_DataFlash.SetCurSel(1);
			GetDlgItem(IDC_EDIT5)->EnableWindow(false);
			GetDlgItem(IDC_COMBO10)->EnableWindow(false);
		}
		//--------------------------------------

		//BOD Detect
		if ((Val_Cfg2 & 0x80) == 0x80)
			m_BroDect.SetCurSel(1);
		else if ((Val_Cfg2 & 0x80) == 0x00)
			m_BroDect.SetCurSel(0);
		//BOD Voltage
		if ((Val_Cfg2 & 0x40) == 0x40)
			m_BroVol.SetCurSel(1);
		else if ((Val_Cfg2 & 0x40) == 0x00)
			m_BroVol.SetCurSel(0);
		//BOD Reset
		if ((Val_Cfg2 & 0x10) == 0x10)
			m_BroReset.SetCurSel(1);
		else if ((Val_Cfg2 & 0x10) == 0x00)
			m_BroReset.SetCurSel(0);
		//WatchDog
		if ((Val_Cfg3 & 0x80) == 0x80)
			m_watchdog.SetCurSel(1);
		else if ((Val_Cfg3 & 0x80) == 0x00)
			m_watchdog.SetCurSel(0);
		//CLK Filter
		if ((Val_Cfg3 & 0x10) == 0x10)
			m_CLKFilter.SetCurSel(1);
		else if ((Val_Cfg3 & 0x10) == 0x00)
			m_CLKFilter.SetCurSel(0);
		//Inernal RC
		if ((Val_Cfg3 & 0x08) == 0x08)
			m_InternalRC.SetCurSel(1);
		else if ((Val_Cfg3 & 0x08) == 0x00)
			m_InternalRC.SetCurSel(0);
		//CLK Source
		if ((Val_Cfg3 & 0x03) == 0x03)
			m_CLKSource.SetCurSel(2);
		else if ((Val_Cfg3 & 0x03) == 0x02)
			m_CLKSource.SetCurSel(1);
		else if ((Val_Cfg3 & 0x03) == 0x00)
			m_CLKSource.SetCurSel(0);
	}
	return CMFCPropertyPage::OnSetActive();
}

void CDLG_Nu8051::OnCbnSelchangeCombo()
{
	CString cfg0;
	CString cfg1;
	CString cfg2;
	CString cfg3;
	Val_Cfg0 = 0xFF;
	Val_Cfg1 = 0xFF;
	Val_Cfg2 = 0xFF;
	Val_Cfg3 = 0xFF;
	//DataFlash_N79E845_Only----------------
	if (
		wcscmp(_T("N79E845A[TSSOP20]"), m_ChipInfo->part_name) == 0
		|| wcscmp(_T("N79E855A[TSSOP28]"), m_ChipInfo->part_name) == 0
		|| wcscmp(_T("N79E815A[TSSOP20]"), m_ChipInfo->part_name) == 0
		|| wcscmp(_T("N79E815A[SOP20]"), m_ChipInfo->part_name) == 0
		|| wcscmp(_T("N79E815A[TSSOP28]"), m_ChipInfo->part_name) == 0
		|| wcscmp(_T("N79E815A[SOP28]"), m_ChipInfo->part_name) == 0
		)
	{
		Val_Cfg0 &= 0xFE;
		if (m_DataFlash.GetCurSel() == 1)
		{
			Val_Cfg0 |= 0x01;
			Val_Cfg1 = 0xFF;
			cfg1.Format(_T("%02X"), Val_Cfg1);
			GetDlgItem(IDC_EDIT5)->EnableWindow(false);
			GetDlgItem(IDC_EDIT5)->SetWindowTextW(cfg1);
			GetDlgItem(IDC_EDIT2)->SetWindowTextW(cfg1);
		}
		else if (m_DataFlash.GetCurSel() == 0)
		{
			Val_Cfg0 |= 0x00;
			GetDlgItem(IDC_EDIT5)->EnableWindow(true);
		}
		cfg0.Format(_T("%02X"), Val_Cfg0);
		m_Cfg0.SetWindowTextW(cfg0);
	}
	else
	{
		Val_Cfg0 |= 0x01;
		Val_Cfg1 = 0xFF;
		cfg1.Format(_T("%02X"), Val_Cfg1);
		GetDlgItem(IDC_EDIT5)->EnableWindow(false);
		GetDlgItem(IDC_COMBO10)->EnableWindow(false);
		GetDlgItem(IDC_EDIT5)->SetWindowTextW(cfg1);
		GetDlgItem(IDC_EDIT2)->SetWindowTextW(cfg1);
	}
	//--------------------------------------

	//Code Protect
	Val_Cfg0 &= 0xFD;
	if (m_Lock.GetCurSel() == 1)
		Val_Cfg0 |= 0x02;
	else if (m_Lock.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	cfg0.Format(_T("%02X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(cfg0);
	
	//Boot Select
	Val_Cfg0 &= 0x7F;
	if (m_BootSelect.GetCurSel() == 1)
		Val_Cfg0 |= 0x80;
	else if (m_BootSelect.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	cfg0.Format(_T("%02X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(cfg0);

	//BOD Detect
	Val_Cfg2 &= 0x7F;
	if (m_BroDect.GetCurSel() == 1)
		Val_Cfg2 |= 0x80;
	else if (m_BroDect.GetCurSel() == 0)
		Val_Cfg2 |= 0x00;
	cfg2.Format(_T("%02X"), Val_Cfg2);
	m_Cfg2.SetWindowTextW(cfg2);

	//BOD Voltage
	Val_Cfg2 &= 0xBF;
	if (m_BroVol.GetCurSel() == 1)
		Val_Cfg2 |= 0x40;
	else if (m_BroVol.GetCurSel() == 0)
		Val_Cfg2 |= 0x00;
	cfg2.Format(_T("%02X"), Val_Cfg2);
	m_Cfg2.SetWindowTextW(cfg2);

	//BOD Reset
	Val_Cfg2 &= 0xEF;
	if (m_BroReset.GetCurSel() == 1)
		Val_Cfg2 |= 0x10;
	else if (m_BroReset.GetCurSel() == 0)
		Val_Cfg2 |= 0x00;
	cfg2.Format(_T("%02X"), Val_Cfg2);
	m_Cfg2.SetWindowTextW(cfg2);

	//WatchDog
	Val_Cfg3 &= 0x7F;
	if (m_watchdog.GetCurSel() == 1)
		Val_Cfg3 |= 0x80;
	else if (m_watchdog.GetCurSel() == 0)
		Val_Cfg3 |= 0x00;
	cfg3.Format(_T("%02X"), Val_Cfg3);
	m_Cfg3.SetWindowTextW(cfg3);

	//CLK Filter
	Val_Cfg3 &= 0xEF;
	if (m_CLKFilter.GetCurSel() == 1)
		Val_Cfg3 |= 0x10;
	else if (m_CLKFilter.GetCurSel() == 0)
		Val_Cfg3 |= 0x00;
	cfg3.Format(_T("%02X"), Val_Cfg3);
	m_Cfg3.SetWindowTextW(cfg3);

	//Internal RC
	Val_Cfg3 &= 0xF7;
	if (m_InternalRC.GetCurSel() == 1)
		Val_Cfg3 |= 0x08;
	else if (m_InternalRC.GetCurSel() == 0)
		Val_Cfg3 |= 0x00;
	cfg3.Format(_T("%02X"), Val_Cfg3);
	m_Cfg3.SetWindowTextW(cfg3);

	//CLK Source
	Val_Cfg3 &= 0xFC;
	if (m_CLKSource.GetCurSel() == 2)
		Val_Cfg3 |= 0x03;
	else if (m_CLKSource.GetCurSel() == 1)
		Val_Cfg3 |= 0x02;
	else if (m_CLKSource.GetCurSel() == 0)
		Val_Cfg3 |= 0x00;
	cfg3.Format(_T("%02X"), Val_Cfg3);
	m_Cfg3.SetWindowTextW(cfg3);

}

void CDLG_Nu8051::OnOK()
{
	CString temp;
	m_Len = 4;
	*m_pDataLen = m_Len;
	m_Cfg0.GetWindowTextW(temp);
	Val_Cfg0 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff, &Val_Cfg0, sizeof(Val_Cfg0));

	m_Cfg1.GetWindowTextW(temp);
	Val_Cfg1 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff + sizeof(Val_Cfg0), &Val_Cfg1, sizeof(Val_Cfg1));

	m_Cfg2.GetWindowTextW(temp);
	Val_Cfg2 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff + sizeof(Val_Cfg0)+sizeof(Val_Cfg1), &Val_Cfg2, sizeof(Val_Cfg2));

	m_Cfg3.GetWindowTextW(temp);
	Val_Cfg3 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff + sizeof(Val_Cfg0)+sizeof(Val_Cfg1)+sizeof(Val_Cfg2), &Val_Cfg3, sizeof(Val_Cfg3));

	CMFCPropertyPage::OnOK();
}

LRESULT CDLG_Nu8051::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();
}


LRESULT CDLG_Nu8051::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}

// DLG_Nu8051 message handlers


void CDLG_Nu8051::OnEnChangeEdit5()
{
	CString Addr;
	if (m_DataFlash.GetCurSel() == 0)
	{
		GetDlgItem(IDC_EDIT5)->GetWindowText(Addr);
		m_Cfg1.SetWindowTextW(Addr);
		Val_Cfg1 = numeric_conversion::hexstring_to_size_t(Addr.GetString());
	}

}
