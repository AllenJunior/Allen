// Dlg_NUC103.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_NUC103.h"
#include "afxdialogex.h"
#include "../common/prog_info.h"
#include "numeric_conversion.h"

// Dlg_NUC103 dialog

IMPLEMENT_DYNAMIC(CDlg_NUC103, CMFCPropertyPage)

CDlg_NUC103::CDlg_NUC103(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_NUC103::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen),
m_Len(0)
{

}

CDlg_NUC103::~CDlg_NUC103()
{
}

void CDlg_NUC103::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_cfg0);
	DDX_Control(pDX, IDC_EDIT2, m_cfg1);
	DDX_Control(pDX, IDC_COMBO1, m_lock);
	DDX_Control(pDX, IDC_COMBO2, m_ClkSource);
	DDX_Control(pDX, IDC_COMBO3, m_WDTEn);
	DDX_Control(pDX, IDC_COMBO4, m_WDTPD);
	DDX_Control(pDX, IDC_COMBO5, m_BroDet);
	DDX_Control(pDX, IDC_COMBO6, m_BroVolt);
	DDX_Control(pDX, IDC_COMBO7, m_BroRst);
	DDX_Control(pDX, IDC_COMBO8, m_boot);
	DDX_Control(pDX, IDC_COMBO9, m_DataFlash);
}


BEGIN_MESSAGE_MAP(CDlg_NUC103, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_NUC103::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_NUC103::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_NUC103::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_NUC103::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_NUC103::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDlg_NUC103::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDlg_NUC103::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CDlg_NUC103::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO9, &CDlg_NUC103::OnCbnSelchangeCombo)
	ON_EN_CHANGE(IDC_EDIT3, &CDlg_NUC103::OnEnChangeEdit3)
END_MESSAGE_MAP()


// Dlg_NUC103 message handlers
BOOL CDlg_NUC103::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	Val_Cfg0 = 0xFFFFFFFF;
	Val_Cfg1 = 0xFFFFFFFF;
	m_cfg0.SetWindowTextW(L"FFFFFFFF");
	m_cfg1.SetWindowTextW(L"FFFFFFFF");
	GetDlgItem(IDC_EDIT3)->SetWindowTextW(L"FFFFFFFF");
	m_cfg0.EnableWindow(false);
	m_cfg1.EnableWindow(false);
	m_lock.InsertString(m_lock.GetCount(), L"0 = Lock Enable");
	m_lock.InsertString(m_lock.GetCount(), L"1 = Lock Disable");
	m_ClkSource.InsertString(m_ClkSource.GetCount(), L"0 = External XTAL");
	m_ClkSource.InsertString(m_ClkSource.GetCount(), L"1 = Internal OSC 22.1184MHz");
	m_WDTEn.InsertString(m_WDTEn.GetCount(), L"0 = Enable");
	m_WDTEn.InsertString(m_WDTEn.GetCount(), L"1 = Disable");
	m_WDTPD.InsertString(m_WDTPD.GetCount(), L"0 = Keep Enable");
	m_WDTPD.InsertString(m_WDTPD.GetCount(), L"1 = Controlled by OSC 10K");
	m_BroDet.InsertString(m_BroDet.GetCount(), L"0 = Enable");
	m_BroDet.InsertString(m_BroDet.GetCount(), L"1 = Disable");
	m_BroVolt.InsertString(m_BroVolt.GetCount(), L"00 = 2.2V");
	m_BroVolt.InsertString(m_BroVolt.GetCount(), L"01 = 2.7V");
	m_BroVolt.InsertString(m_BroVolt.GetCount(), L"10 = 3.8V");
	m_BroVolt.InsertString(m_BroVolt.GetCount(), L"11 = 4.5V");
	m_BroRst.InsertString(m_BroRst.GetCount(), L"0 = Enable");
	m_BroRst.InsertString(m_BroRst.GetCount(), L"1 = Disable");
	m_boot.InsertString(m_boot.GetCount(), L"00 = Boot from LDROM with IAP");
	m_boot.InsertString(m_boot.GetCount(), L"01 = Boot from LDROM");
	m_boot.InsertString(m_boot.GetCount(), L"10 = Boot from APROM with IAP");
	m_boot.InsertString(m_boot.GetCount(), L"11 = Boot from APROM");
	m_DataFlash.InsertString(m_DataFlash.GetCount(), L"0X0 = APROM and DataFlash share flash");
	m_DataFlash.InsertString(m_DataFlash.GetCount(), L"0X1 = DataFlash Disable");
	m_DataFlash.InsertString(m_DataFlash.GetCount(), L"1X1 = DataFlash = 4KB");
	m_lock.SetCurSel(1);
	m_ClkSource.SetCurSel(1);
	m_WDTEn.SetCurSel(1);
	m_WDTPD.SetCurSel(1);
	m_BroDet.SetCurSel(1);
	m_BroVolt.SetCurSel(3);
	m_BroRst.SetCurSel(1);
	m_boot.SetCurSel(3);
	m_DataFlash.SetCurSel(2);
	GetDlgItem(IDC_EDIT3)->EnableWindow(false);
	return TRUE;
}
BOOL CDlg_NUC103::OnSetActive()
{
	CString CFG0;
	CString CFG1;
	if (*m_pDataLen != NULL)
	{
		memcpy((void*)&Val_Cfg0, m_RegBuff, 4);
		memcpy((void*)&Val_Cfg1, m_RegBuff + 4, 4);
		//CFG0
		CFG0.Format(_T("%08X"), Val_Cfg0);
		GetDlgItem(IDC_EDIT1)->SetWindowTextW(CFG0);
		//CFG1
		CFG1.Format(_T("%08X"), Val_Cfg1);
		GetDlgItem(IDC_EDIT2)->SetWindowTextW(CFG1);
		GetDlgItem(IDC_EDIT3)->SetWindowTextW(CFG1);
		//Code Protect
		if ((Val_Cfg0 & 0x00000002) == 0x02)
			m_lock.SetCurSel(1);
		else if ((Val_Cfg0 & 0x00000002) == 0x00)
			m_lock.SetCurSel(0);
		//Clock Source Selection
		if ((Val_Cfg0 & 0x07000000) == 0x07000000)
			m_ClkSource.SetCurSel(1);
		else if ((Val_Cfg0 & 0x07000000) == 0x00)
			m_ClkSource.SetCurSel(0);
		//WatchDog Timer Enable
		if ((Val_Cfg0 & 0x80000000) == 0x80000000)
			m_WDTEn.SetCurSel(1);
		else if ((Val_Cfg0 & 0x80000000) == 0x0)
			m_WDTEn.SetCurSel(0);
		//WatchDog Clock Power Down
		if ((Val_Cfg0 & 0x40000000) == 0x40000000)
			m_WDTPD.SetCurSel(1);
		else if ((Val_Cfg0 & 0x40000000) == 0x0)
			m_WDTPD.SetCurSel(0);
		//Brown-Out Detection
		if ((Val_Cfg0 & 0x00800000) == 0x00800000)
			m_BroDet.SetCurSel(1);
		else if ((Val_Cfg0 & 0x00800000) == 0x00)
			m_BroDet.SetCurSel(0);
		//Brown-Out Voltage
		if ((Val_Cfg0 & 0x00600000) == 0x00600000)
			m_BroVolt.SetCurSel(3);
		else if ((Val_Cfg0 & 0x00600000) == 0x00400000)
			m_BroVolt.SetCurSel(2);
		else if ((Val_Cfg0 & 0x00600000) == 0x00200000)
			m_BroVolt.SetCurSel(1);
		else if ((Val_Cfg0 & 0x00600000) == 0x00)
			m_BroVolt.SetCurSel(0);
		//Brown-Out Reset
		if ((Val_Cfg0 & 0x00100000) == 0x00100000)
			m_BroRst.SetCurSel(1);
		else if ((Val_Cfg0 & 0x00600000) == 0x00)
			m_BroRst.SetCurSel(0);
		//Boot Select
		if ((Val_Cfg0 & 0x000000C0) == 0xC0)
			m_boot.SetCurSel(3);
		else if ((Val_Cfg0 & 0x000000C0) == 0x80)
			m_boot.SetCurSel(2);
		else if ((Val_Cfg0 & 0x000000C0) == 0x40)
			m_boot.SetCurSel(1);
		else if ((Val_Cfg0 & 0x000000C0) == 0x00)
			m_boot.SetCurSel(0);
		//DataFlash Enable
		if ((Val_Cfg0 & 0x00000005) == 0x05)
		{
			m_DataFlash.SetCurSel(2);
			GetDlgItem(IDC_EDIT3)->EnableWindow(false);
		}
		else if ((Val_Cfg0 & 0x00000005) == 0x01)
		{
			m_DataFlash.SetCurSel(1);
			GetDlgItem(IDC_EDIT3)->EnableWindow(false);
		}
		else if ((Val_Cfg0 & 0x00000005) == 0x00)
		{
			m_DataFlash.SetCurSel(0);
			GetDlgItem(IDC_EDIT3)->EnableWindow(true);
		}

	}
	return CMFCPropertyPage::OnSetActive();
}
void CDlg_NUC103::OnCbnSelchangeCombo()
{
	CString strTemp; //cfg0
	CString strTemp2; //cfg1
	Val_Cfg0 = 0xFFFFFFFF;
	Val_Cfg1 = 0xFFFFFFFF;
	//Code Protect
	Val_Cfg0 &= 0xFFFFFFFD;
	if (m_lock.GetCurSel() == 1)
		Val_Cfg0 |= 0x02;
	else if (m_lock.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(strTemp);
	//CLK Source
	Val_Cfg0 &= 0xF8FFFFFF;
	if (m_ClkSource.GetCurSel() == 1)
		Val_Cfg0 |= 0x07000000;
	else if (m_ClkSource.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(strTemp);
	//WDT Clock
	Val_Cfg0 &= 0x7FFFFFFF;
	if (m_WDTEn.GetCurSel() == 1)
		Val_Cfg0 |= 0x80000000;
	else if (m_WDTEn.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(strTemp);
	//Watch dog Power Down
	Val_Cfg0 &= 0xBFFFFFFF;
	if (m_WDTPD.GetCurSel() == 1)
		Val_Cfg0 |= 0x40000000;
	else if (m_WDTPD.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(strTemp);
	//Brown-Out Detection
	Val_Cfg0 &= 0xFF7FFFFF;
	if (m_BroDet.GetCurSel() == 1)
		Val_Cfg0 |= 0x00800000;
	else if (m_BroDet.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(strTemp);
	//Brown-Out Voltage
	Val_Cfg0 &= 0xFF9FFFFF;
	switch (m_BroVolt.GetCurSel())
	{
	case 0:
		Val_Cfg0 |= 0x000000;
		break;
	case 1:
		Val_Cfg0 |= 0x200000;
		break;
	case 2:
		Val_Cfg0 |= 0x400000;
		break;
	case 3:
		Val_Cfg0 |= 0x600000;
		break;
	}
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(strTemp);
	//Brown-Out Reset
	Val_Cfg0 &= 0xFFEFFFFF;
	if (m_BroRst.GetCurSel() == 1)
		Val_Cfg0 |= 0x100000;
	else if (m_BroRst.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(strTemp);
	//Boot Select
	Val_Cfg0 &= 0xFFFFFF3F;
	switch (m_boot.GetCurSel())
	{
	case 0:
		Val_Cfg0 |= 0x00;
		break;
	case 1:
		Val_Cfg0 |= 0x40;
		break;
	case 2:
		Val_Cfg0 |= 0x80;
		break;
	case 3:
		Val_Cfg0 |= 0xC0;
		break;
	}
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(strTemp);
	//DataFlash
	Val_Cfg0 &= 0xFFFFFFFA;
	switch (m_DataFlash.GetCurSel())
	{
	case 0:
		Val_Cfg0 |= 0x00;
		GetDlgItem(IDC_EDIT3)->EnableWindow(true);
		break;
	case 1:
		Val_Cfg0 |= 0x01;
		Val_Cfg1 = 0xFFFFFFFF;
		strTemp2.Format(_T("%08X"), Val_Cfg1);
		m_cfg1.SetWindowTextW(strTemp2);
		GetDlgItem(IDC_EDIT3)->SetWindowTextW(strTemp2);
		GetDlgItem(IDC_EDIT3)->EnableWindow(false);
		break;
	case 2:
		Val_Cfg0 |= 0x05;
		Val_Cfg1 = 0xFFFFFFFF;
		strTemp2.Format(_T("%08X"), Val_Cfg1);
		m_cfg1.SetWindowTextW(strTemp2);
		GetDlgItem(IDC_EDIT3)->SetWindowTextW(strTemp2);
		GetDlgItem(IDC_EDIT3)->EnableWindow(false);
		break;
	}
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(strTemp);

}
void CDlg_NUC103::OnOK()
{
	CString temp;
	m_Len = 8;
	*m_pDataLen = m_Len;
	m_cfg0.GetWindowTextW(temp);
	Val_Cfg0 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff, &Val_Cfg0, 4);
	m_cfg1.GetWindowTextW(temp);
	Val_Cfg1 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff + 4, &Val_Cfg1, 4);
	CMFCPropertyPage::OnOK();
}


LRESULT CDlg_NUC103::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();
}
LRESULT CDlg_NUC103::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}
void CDlg_NUC103::OnEnChangeEdit3()
{
	CString Addr;
	if (m_DataFlash.GetCurSel() == 0)
	{
		GetDlgItem(IDC_EDIT3)->GetWindowTextW(Addr);
		m_cfg1.SetWindowTextW(Addr);
		Val_Cfg1 = numeric_conversion::hexstring_to_size_t(Addr.GetString());
	}
}
