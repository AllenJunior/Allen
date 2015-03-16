// Dlg_N78E517.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_N78E517.h"
#include "afxdialogex.h"
#include "../common/prog_info.h"
#include "numeric_conversion.h"

// Dlg_N78E517 dialog

IMPLEMENT_DYNAMIC(CDlg_N78E517, CMFCPropertyPage)

CDlg_N78E517::CDlg_N78E517(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_N78E517::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen),
m_Len(0)
{

}

CDlg_N78E517::~CDlg_N78E517()
{
}

void CDlg_N78E517::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_cfg0);
	DDX_Control(pDX, IDC_EDIT2, m_cfg1);
	DDX_Control(pDX, IDC_EDIT3, m_cfg2);
	DDX_Control(pDX, IDC_EDIT4, m_cfg3);
	DDX_Control(pDX, IDC_EDIT5, m_cfg1);
	DDX_Control(pDX, IDC_COMBO1, m_boot);
	DDX_Control(pDX, IDC_COMBO2, m_lock);
	DDX_Control(pDX, IDC_COMBO3, m_DataFlash);
	DDX_Control(pDX, IDC_COMBO4, m_BroDet);
	DDX_Control(pDX, IDC_COMBO5, m_BroVolt);
	DDX_Control(pDX, IDC_COMBO6, m_BroRst);
	DDX_Control(pDX, IDC_COMBO7, m_WTEn);
	DDX_Control(pDX, IDC_COMBO8, m_6TMEn);
	DDX_Control(pDX, IDC_COMBO9, m_Extal);
	DDX_Control(pDX, IDC_COMBO10, m_ClkFilter);
	DDX_Control(pDX, IDC_COMBO11, m_Osc);
	DDX_Control(pDX, IDC_COMBO12, m_ClkSource);
	
}


BEGIN_MESSAGE_MAP(CDlg_N78E517, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_N78E517::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_N78E517::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_N78E517::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_N78E517::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_N78E517::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDlg_N78E517::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDlg_N78E517::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CDlg_N78E517::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO9, &CDlg_N78E517::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO10, &CDlg_N78E517::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO11, &CDlg_N78E517::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO12, &CDlg_N78E517::OnCbnSelchangeCombo)
	ON_EN_CHANGE(IDC_EDIT5, &CDlg_N78E517::OnEnChangeEdit5)
END_MESSAGE_MAP()


// Dlg_N78E517 message handlers
BOOL CDlg_N78E517::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	Val_Cfg0 = 0x7F;
	Val_Cfg1 = 0xFF;
	Val_Cfg2 = 0xFF;
	Val_Cfg3 = 0xFF;
	m_cfg0.SetWindowTextW(L"7F");
	m_cfg1.SetWindowTextW(L"FF");
	m_cfg2.SetWindowTextW(L"FF");
	m_cfg3.SetWindowTextW(L"FF");
	//config0
	m_boot.InsertString(m_boot.GetCount(), L"0 = MCU will boot from LDROM");
	m_boot.InsertString(m_boot.GetCount(), L"1 = MCU will boot from APROM");
	m_lock.InsertString(m_lock.GetCount(), L"0 = Locked");
	m_lock.InsertString(m_lock.GetCount(), L"1 = Unlocked");
	m_DataFlash.InsertString(m_DataFlash.GetCount(), L"0 = DataFlash Enabled");
	m_DataFlash.InsertString(m_DataFlash.GetCount(), L"1 = DataFlash Disabled");
	//config2
	m_BroDet.InsertString(m_BroDet.GetCount(), L"0 = Disable Brown-Out");
	m_BroDet.InsertString(m_BroDet.GetCount(), L"1 = Enable Brown-Out");
	m_BroVolt.InsertString(m_BroVolt.GetCount(), L"00 = 4.5V" );
	m_BroVolt.InsertString(m_BroVolt.GetCount(), L"01 = 3.8V");
	m_BroVolt.InsertString(m_BroVolt.GetCount(), L"10 = 2.7V");
	m_BroVolt.InsertString(m_BroVolt.GetCount(), L"11 = 2.2V");
	m_BroRst.InsertString(m_BroRst.GetCount(), L"0 = Brown-Out interrupt");
	m_BroRst.InsertString(m_BroRst.GetCount(), L"1 = Brown-Out Reset");
	//config3
	m_WTEn.InsertString(m_WTEn.GetCount(), L"0 = Enable Watchdog Timer");
	m_WTEn.InsertString(m_WTEn.GetCount(), L"1 = Disable Watchdog Timer");
	m_6TMEn.InsertString(m_6TMEn.GetCount(), L"0 = 6T");
	m_6TMEn.InsertString(m_6TMEn.GetCount(), L"1 = 12T");
	m_Extal.InsertString(m_Extal.GetCount(), L"0 = 4MHz ~ 24MHz");
	m_Extal.InsertString(m_Extal.GetCount(), L"1 = 24MHz ~ 48MHz");
	m_ClkFilter.InsertString(m_ClkFilter.GetCount(), L"0 = Disable Clock Filter");
	m_ClkFilter.InsertString(m_ClkFilter.GetCount(), L"1 = Enable Clock Filter");
	m_Osc.InsertString(m_Osc.GetCount(), L"0 = 11.0592MHz");
	m_Osc.InsertString(m_Osc.GetCount(), L"1 = 22.1184MHz");
	m_ClkSource.InsertString(m_ClkSource.GetCount(), L"0 = Internal RC");
	m_ClkSource.InsertString(m_ClkSource.GetCount(), L"1 = External XTAL");
	//Initial status
	m_boot.SetCurSel(0);
	m_lock.SetCurSel(1);
	m_DataFlash.SetCurSel(1);
	m_BroDet.SetCurSel(1);
	m_BroVolt.SetCurSel(3);
	m_BroRst.SetCurSel(1);
	m_WTEn.SetCurSel(1);
	m_6TMEn.SetCurSel(1);
	m_ClkSource.SetCurSel(1);
	m_ClkFilter.SetCurSel(1);
	m_Osc.SetCurSel(1);
	m_Extal.SetCurSel(1);
	GetDlgItem(IDC_EDIT5)->SetWindowTextW(L"FF");
	GetDlgItem(IDC_EDIT5)->EnableWindow(false);
	GetDlgItem(IDC_COMBO1)->EnableWindow(false);
	return true;

}
BOOL CDlg_N78E517::OnSetActive()
{
	CString Cfg0;
	CString Cfg1;
	CString Cfg2;
	CString Cfg3;
	if (*m_pDataLen != NULL)
	{
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

		//Boot Select
		if ((Val_Cfg0 & 0x80) == 0x80)
			m_boot.SetCurSel(1);
		else if ((Val_Cfg0 & 0x80) == 0x00)
			m_boot.SetCurSel(0);
		//Lock
		if ((Val_Cfg0 & 0x02) == 0x02)
			m_lock.SetCurSel(1);
		else if ((Val_Cfg0 & 0x02) == 0x00)
			m_lock.SetCurSel(0);
		//DataFlash
		if ((Val_Cfg0 & 0x01) == 0x01)
		{
			m_DataFlash.SetCurSel(1);
			GetDlgItem(IDC_EDIT5)->EnableWindow(false);
		}
		else if ((Val_Cfg0 & 0x01) == 0x00)
		{
			m_DataFlash.SetCurSel(0);
			GetDlgItem(IDC_EDIT5)->EnableWindow(true);
		}
		//Brown-Out Detect
		if ((Val_Cfg2 & 0x80) == 0x80)
			m_BroDet.SetCurSel(1);
		else if ((Val_Cfg2 & 0x80) == 0x00)
			m_BroDet.SetCurSel(0);
		//Brown-Out Voltage
		if ((Val_Cfg2 & 0x60) == 0x60)
			m_BroVolt.SetCurSel(3);
		else if ((Val_Cfg2 & 0x60) == 0x40)
			m_BroVolt.SetCurSel(2);
		else if ((Val_Cfg2 & 0x60) == 0x20)
			m_BroVolt.SetCurSel(1);
		else if ((Val_Cfg2 & 0x60) == 0x00)
			m_BroVolt.SetCurSel(0);
		//Brown-Out Reset
		if ((Val_Cfg2 & 0x10) == 0x10)
			m_BroRst.SetCurSel(1);
		else if ((Val_Cfg2 & 0x10) == 0x00)
			m_BroRst.SetCurSel(0);
		//Watchdog Timer
		if ((Val_Cfg3 & 0x80) == 0x80)
			m_WTEn.SetCurSel(1);
		else if ((Val_Cfg3 & 0x80) == 0x00)
			m_WTEn.SetCurSel(0);
		//Enable 6T Mode
		if ((Val_Cfg3 & 0x40) == 0x40)
			m_6TMEn.SetCurSel(1);
		else if ((Val_Cfg3 & 0x40) == 0x00)
			m_6TMEn.SetCurSel(0);
		//External XTAL
		if ((Val_Cfg3 & 0x20) == 0x20)
			m_Extal.SetCurSel(1);
		else if ((Val_Cfg3 & 0x20) == 0x00)
			m_Extal.SetCurSel(0);
		//Clock Filter
		if ((Val_Cfg3 & 0x10) == 0x10)
			m_ClkFilter.SetCurSel(1);
		else if ((Val_Cfg3 & 0x10) == 0x00)
			m_ClkFilter.SetCurSel(0);
		//Internal RC
		if ((Val_Cfg3 & 0x08) == 0x08)
			m_Osc.SetCurSel(1);
		else if ((Val_Cfg3 & 0x08) == 0x00)
			m_Osc.SetCurSel(0);
		//Clock Source
		if ((Val_Cfg3 & 0x02) == 0x02)
			m_ClkSource.SetCurSel(1);
		else if ((Val_Cfg3 & 0x02) == 0x00)
			m_ClkSource.SetCurSel(0);

	}
	return CMFCPropertyPage::OnSetActive();
}

void CDlg_N78E517::OnCbnSelchangeCombo()
{
	CString cfg0;
	CString cfg1;
	CString cfg2;
	CString cfg3;
	Val_Cfg0 = 0xFF;
	Val_Cfg1 = 0xFF;
	Val_Cfg2 = 0xFF;
	Val_Cfg3 = 0xFF;
	//Boot Select
	Val_Cfg0 &= 0x7F;
	if (m_boot.GetCurSel() == 1)
		Val_Cfg0 |= 0x80;
	else if (m_boot.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	cfg0.Format(_T("%02X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(cfg0);
	//Lock
	Val_Cfg0 &= 0xFD;
	if (m_lock.GetCurSel() == 1)
		Val_Cfg0 |= 0x02;
	else if (m_lock.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	cfg0.Format(_T("%02X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(cfg0);
	//DataFlash
	Val_Cfg0 &= 0xFE;
	if (m_DataFlash.GetCurSel() == 1)
	{
		Val_Cfg0 |= 0x01;
		Val_Cfg1 = 0xFF;
		cfg1.Format(_T("%02X"), Val_Cfg1);
		GetDlgItem(IDC_EDIT2)->SetWindowTextW(cfg1);
		GetDlgItem(IDC_EDIT5)->SetWindowTextW(cfg1);
		GetDlgItem(IDC_EDIT5)->EnableWindow(false);
	}
	else if (m_DataFlash.GetCurSel() == 0)
	{
		Val_Cfg0 |= 0x00;
		GetDlgItem(IDC_EDIT5)->EnableWindow(true);
	}
	cfg0.Format(_T("%02X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(cfg0);
	//Brown-Out Detect
	Val_Cfg2 &= 0x7F;
	if (m_BroDet.GetCurSel() == 1)
		Val_Cfg2 |= 0x80;
	else if (m_BroDet.GetCurSel() == 0)
		Val_Cfg2 |= 0x00;
	cfg2.Format(_T("%02X"), Val_Cfg2);
	m_cfg2.SetWindowTextW(cfg2);
	//Brown-Out Voltage
	Val_Cfg2 &= 0x9F;
	if (m_BroVolt.GetCurSel() == 3)
		Val_Cfg2 |= 0x60;
	else if (m_BroVolt.GetCurSel() == 2)
		Val_Cfg2 |= 0x40;
	else if (m_BroVolt.GetCurSel() == 1)
		Val_Cfg2 |= 0x20;
	else if (m_BroVolt.GetCurSel() == 0)
		Val_Cfg2 |= 0x00;
	cfg2.Format(_T("%02X"), Val_Cfg2);
	m_cfg2.SetWindowTextW(cfg2);
	//Brown-Out Reset
	Val_Cfg2 &= 0xEF;
	if (m_BroRst.GetCurSel() == 1)
		Val_Cfg2 |= 0x10;
	else if (m_BroRst.GetCurSel() == 0)
		Val_Cfg2 |= 0x00;
	cfg2.Format(_T("%02X"), Val_Cfg2);
	m_cfg2.SetWindowTextW(cfg2);
	//Watchdog Timer
	Val_Cfg3 &= 0x7F;
	if (m_WTEn.GetCurSel() == 1)
		Val_Cfg3 |= 0x80;
	else if (m_WTEn.GetCurSel() == 0)
		Val_Cfg3 |= 0x00;
	cfg3.Format(_T("%02X"), Val_Cfg3);
	m_cfg3.SetWindowTextW(cfg3);
	//Enable 6T Mode
	Val_Cfg3 &= 0xBF;
	if (m_6TMEn.GetCurSel() == 1)
		Val_Cfg3 |= 0x40;
	else if (m_6TMEn.GetCurSel() == 0)
		Val_Cfg3 |= 0x00;
	cfg3.Format(_T("%02X"), Val_Cfg3);
	m_cfg3.SetWindowTextW(cfg3);
	//External XTAL
	Val_Cfg3 &= 0xDF;
	if (m_Extal.GetCurSel() == 1)
		Val_Cfg3 |= 0x20;
	else if (m_Extal.GetCurSel() == 0)
		Val_Cfg3 |= 0x00;
	cfg3.Format(_T("%02X"), Val_Cfg3);
	m_cfg3.SetWindowTextW(cfg3);
	//Clock Filter
	Val_Cfg3 &= 0xEF;
	if (m_ClkFilter.GetCurSel() == 1)
		Val_Cfg3 |= 0x10;
	else if (m_ClkFilter.GetCurSel() == 0)
		Val_Cfg3 |= 0x00;
	cfg3.Format(_T("%02X"), Val_Cfg3);
	m_cfg3.SetWindowTextW(cfg3);
	//Internal RC
	Val_Cfg3 &= 0xF7;
	if (m_Osc.GetCurSel() == 1)
		Val_Cfg3 |= 0x08;
	else if (m_Osc.GetCurSel() == 0)
		Val_Cfg3 |= 0x00;
	cfg3.Format(_T("%02X"), Val_Cfg3);
	m_cfg3.SetWindowTextW(cfg3);
	//Clock Source
	Val_Cfg3 &= 0xFD;
	if (m_ClkSource.GetCurSel() == 1)
		Val_Cfg3 |= 0x02;
	else if (m_ClkSource.GetCurSel() == 0)
		Val_Cfg3 |= 0x00;
	cfg3.Format(_T("%02X"), Val_Cfg3);
	m_cfg3.SetWindowTextW(cfg3);

}

void CDlg_N78E517::OnOK()
{
	CString temp;
	m_Len = 4;
	*m_pDataLen = m_Len;
	m_cfg0.GetWindowTextW(temp);
	Val_Cfg0 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff, &Val_Cfg0, 1);

	m_cfg1.GetWindowTextW(temp);
	Val_Cfg1 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff + 1, &Val_Cfg1, 1);

	m_cfg2.GetWindowTextW(temp);
	Val_Cfg2 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff + 2, &Val_Cfg2, 1);

	m_cfg3.GetWindowTextW(temp);
	Val_Cfg3 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff + 3, &Val_Cfg3, 1);

	CMFCPropertyPage::OnOK();
}

LRESULT CDlg_N78E517::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();
}


LRESULT CDlg_N78E517::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}

void CDlg_N78E517::OnEnChangeEdit5()
{
	CString Addr;
	if (m_DataFlash.GetCurSel() == 0)
	{
		GetDlgItem(IDC_EDIT5)->GetWindowText(Addr);
		m_cfg1.SetWindowTextW(Addr);
		Val_Cfg1 = numeric_conversion::hexstring_to_size_t(Addr.GetString());
	}

}
