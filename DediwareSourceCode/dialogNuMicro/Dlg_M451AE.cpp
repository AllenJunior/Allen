// Dlg_M451AE.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_M451AE.h"
#include "afxdialogex.h"
#include "../common/prog_info.h"
#include "numeric_conversion.h"

// Dlg_M451AE dialog

IMPLEMENT_DYNAMIC(CDlg_M451AE, CMFCPropertyPage)

CDlg_M451AE::CDlg_M451AE(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_M451AE::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_M451AE::~CDlg_M451AE()
{
}

void CDlg_M451AE::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Cfg0);
	DDX_Control(pDX, IDC_EDIT2, m_Cfg1);
	DDX_Control(pDX, IDC_COMBO1, m_CWDTEN);
	DDX_Control(pDX, IDC_COMBO2, m_CWDTPDEN);
	DDX_Control(pDX, IDC_COMBO3, m_CFGXT1);
	DDX_Control(pDX, IDC_COMBO4, m_CFOSC);
	DDX_Control(pDX, IDC_COMBO5, m_CBODEN);
	DDX_Control(pDX, IDC_COMBO6, m_CBOV);
	DDX_Control(pDX, IDC_COMBO7, m_CBORST);
	DDX_Control(pDX, IDC_COMBO8, m_CIOINI);
	DDX_Control(pDX, IDC_COMBO9, m_CBS);
	DDX_Control(pDX, IDC_COMBO10, m_MBS);
	DDX_Control(pDX, IDC_COMBO11, m_LOCK);
	DDX_Control(pDX, IDC_COMBO12, m_DFEN);
	DDX_Control(pDX, IDC_EDIT3, m_Addr);
}


BEGIN_MESSAGE_MAP(CDlg_M451AE, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_M451AE::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_M451AE::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_M451AE::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_M451AE::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_M451AE::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDlg_M451AE::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDlg_M451AE::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CDlg_M451AE::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO9, &CDlg_M451AE::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO10, &CDlg_M451AE::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO11, &CDlg_M451AE::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO12, &CDlg_M451AE::OnCbnSelchangeCombo)
	ON_EN_CHANGE(IDC_EDIT3, &CDlg_M451AE::OnEnChangeAddr)
END_MESSAGE_MAP()


// Dlg_M451AE message handlers
BOOL CDlg_M451AE::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	Val_Cfg0 = 0xFFFFFFFF;
	Val_Cfg1 = 0xFFFFFFFF;
	m_Cfg0.SetWindowTextW(L"FFFFFFFF");
	m_Cfg1.SetWindowTextW(L"FFFFFFFF");
	m_Addr.SetWindowTextW(L"FFFFFFFF");
	m_Cfg0.EnableWindow(false);
	m_Cfg1.EnableWindow(false);
	m_Addr.EnableWindow(false);

	m_CWDTEN.InsertString(m_CWDTEN.GetCount(), L"000 = WDT is active, WDT CLK is always on");
	m_CWDTEN.InsertString(m_CWDTEN.GetCount(), L"011 = WDT is active, WDT CLK is controlled by CWDTEN in Power-down mode");
	m_CWDTEN.InsertString(m_CWDTEN.GetCount(), L"111 = WDT is inactive");

	m_CWDTPDEN.InsertString(m_CWDTPDEN.GetCount(), L"0 = WDT Timer CLK keep enabled when chip enters Power-down");
	m_CWDTPDEN.InsertString(m_CWDTPDEN.GetCount(), L"1 = WDT Timer CLK is Controlled by LIRCEN when chip enters Power-down");

	m_CFGXT1.InsertString(m_CFGXT1.GetCount(), L"0 = PF[4:3] pins are configured as GPIO pins");
	m_CFGXT1.InsertString(m_CFGXT1.GetCount(), L"1 = PF[4:3] pins are configured as external 4~20MHz high speed crystal OSC");

	m_CFOSC.InsertString(m_CFOSC.GetCount(), L"0 = 4~20MHz external high speed crystal OSC");
	m_CFOSC.InsertString(m_CFOSC.GetCount(), L"1 = 22.1184MHz internal high speed RC OSC");

	m_CBODEN.InsertString(m_CBODEN.GetCount(), L"0 =Brown-out Detect enabled");
	m_CBODEN.InsertString(m_CBODEN.GetCount(), L"1 =Brown-out Detect disabled");

	m_CBOV.InsertString(m_CBOV.GetCount(), L"00 = Brown-out Voltage 2.2V");
	m_CBOV.InsertString(m_CBOV.GetCount(), L"01 = Brown-out Voltage 2.7V");
	m_CBOV.InsertString(m_CBOV.GetCount(), L"10 = Brown-out Voltage 3.7V");
	m_CBOV.InsertString(m_CBOV.GetCount(), L"11 = Brown-out Voltage 4.5V");

	m_CBORST.InsertString(m_CBORST.GetCount(), L"0 = Brown-out RST enabled");
	m_CBORST.InsertString(m_CBORST.GetCount(), L"1 = Brown-out RST disabled");

	m_CIOINI.InsertString(m_CIOINI.GetCount(), L"0 = All GPIO set as Quasi-bidirectional mode");
	m_CIOINI.InsertString(m_CIOINI.GetCount(), L"1 = All GPIO set as Input Tri-state mode");

	m_CBS.InsertString(m_CBS.GetCount(), L"00 = Boot from LDROM with IAP mode");
	m_CBS.InsertString(m_CBS.GetCount(), L"01 = Boot from LDROM without IAP mode");
	m_CBS.InsertString(m_CBS.GetCount(), L"10 = Boot from APROM with IAP mode");
	m_CBS.InsertString(m_CBS.GetCount(), L"11 = Boot from APROM without IAP mode");

	m_MBS.InsertString(m_MBS.GetCount(), L"0 = Booting from Boot Loader, and ignored CBS setting");
	m_MBS.InsertString(m_MBS.GetCount(), L"1 = Booting from APROM/LDROM, depended on CBS setting");

	m_LOCK.InsertString(m_LOCK.GetCount(), L"0 = Locked");
	m_LOCK.InsertString(m_LOCK.GetCount(), L"1 = Unlocked");

	m_DFEN.InsertString(m_DFEN.GetCount(), L"0 = DataFlash enabled");
	m_DFEN.InsertString(m_DFEN.GetCount(), L"1 = DataFlash disabled");

	m_CWDTEN.SetCurSel(2);
	m_CWDTPDEN.SetCurSel(1);
	m_CFGXT1.SetCurSel(1);
	m_CFOSC.SetCurSel(1);
	m_CBODEN.SetCurSel(1);
	m_CBOV.SetCurSel(3);
	m_CBORST.SetCurSel(1);
	m_CIOINI.SetCurSel(1);
	m_CBS.SetCurSel(3);
	m_MBS.SetCurSel(1);
	m_LOCK.SetCurSel(1);
	m_DFEN.SetCurSel(1);

	return TRUE;
}

BOOL CDlg_M451AE::OnSetActive()
{
	CString CFG0;
	CString CFG1;
	if (*m_pDataLen != NULL)
	{
		memcpy((void*)&Val_Cfg0, m_RegBuff, sizeof(Val_Cfg0));
		memcpy((void*)&Val_Cfg1, m_RegBuff + sizeof(Val_Cfg0), sizeof(Val_Cfg1));
		//CFG0
		CFG0.Format(_T("%08X"), Val_Cfg0);
		m_Cfg0.SetWindowTextW(CFG0);
		//CFG1
		CFG1.Format(_T("%08X"), Val_Cfg1);
		m_Cfg1.SetWindowTextW(CFG1);
		m_Addr.SetWindowTextW(CFG1);
		//WDT Timer HW enable
		if ((Val_Cfg0 & 0x80000018) == 0x80000018)
			m_CWDTEN.SetCurSel(2);
		else if ((Val_Cfg0 & 0x80000018) == 0x00000018)
			m_CWDTEN.SetCurSel(1);
		else if ((Val_Cfg0 & 0x80000018) == 0x00)
			m_CWDTEN.SetCurSel(0);
		//WDT CLK PWR down enable
		if ((Val_Cfg0 & 0x40000000) == 0x40000000)
			m_CWDTPDEN.SetCurSel(1);
		else if ((Val_Cfg0 & 0x40000000) == 0x00)
			m_CWDTPDEN.SetCurSel(0);
		//Multi-Function Select
		if ((Val_Cfg0 & 0x08000000) == 0x08000000)
			m_CWDTPDEN.SetCurSel(1);
		else if ((Val_Cfg0 & 0x08000000) == 0x00)
			m_CWDTPDEN.SetCurSel(0);
		//CPU CLK Source
		if ((Val_Cfg0 & 0x04000000) == 0x04000000)
			m_CFOSC.SetCurSel(1);
		else if ((Val_Cfg0 & 0x04000000) == 0x00)
			m_CFOSC.SetCurSel(0);
		//Brown-out Detection
		if ((Val_Cfg0 & 0x800000) == 0x800000)
			m_CBODEN.SetCurSel(1);
		else if ((Val_Cfg0 & 0x800000) == 0x00)
			m_CBODEN.SetCurSel(0);
		//Brown-out Voltage
		if ((Val_Cfg0 & 0x600000) == 0x600000)
			m_CBOV.SetCurSel(3);
		else if ((Val_Cfg0 & 0x600000) == 0x400000)
			m_CBOV.SetCurSel(2);
		else if ((Val_Cfg0 & 0x600000) == 0x200000)
			m_CBOV.SetCurSel(1);
		else if ((Val_Cfg0 & 0x600000) == 0x00)
			m_CBOV.SetCurSel(0);
		//Brown-out RST
		if ((Val_Cfg0 & 0x100000) == 0x100000)
			m_CBORST.SetCurSel(1);
		else if ((Val_Cfg0 & 0x100000) == 0x00)
			m_CBORST.SetCurSel(0);
		//IO Initial state
		if ((Val_Cfg0 & 0x0400) == 0x0400)
			m_CIOINI.SetCurSel(1);
		else if ((Val_Cfg0 & 0x0400) == 0x00)
			m_CIOINI.SetCurSel(0);
		//Boot Selection
		if ((Val_Cfg0 & 0xC0) == 0xC0)
			m_CBS.SetCurSel(3);
		else if ((Val_Cfg0 & 0xC0) == 0x80)
			m_CBS.SetCurSel(2);
		else if ((Val_Cfg0 & 0xC0) == 0x40)
			m_CBS.SetCurSel(1);
		else if ((Val_Cfg0 & 0xC0) == 0x00)
			m_CBS.SetCurSel(0);
		//Boot Loader Selection
		if ((Val_Cfg0 & 0x20) == 0x20)
			m_MBS.SetCurSel(1);
		else if ((Val_Cfg0 & 0x20) == 0x00)
			m_MBS.SetCurSel(0);
		//Security Lock
		if ((Val_Cfg0 & 0x02) == 0x02)
			m_LOCK.SetCurSel(1);
		else if ((Val_Cfg0 & 0x02) == 0x00)
			m_LOCK.SetCurSel(0);
		//Data Flash Enable
		if ((Val_Cfg0 & 0x01) == 0x01)
		{
			m_DFEN.SetCurSel(1);
			m_Addr.EnableWindow(false);
		}
		else if ((Val_Cfg0 & 0x01) == 0x00)
		{
			m_DFEN.SetCurSel(0);
			m_Addr.EnableWindow(true);
		}

	}
	return CMFCPropertyPage::OnSetActive();

}

void CDlg_M451AE::OnCbnSelchangeCombo()
{
	CString strTemp;
	CString strTemp2;
	Val_Cfg0 = 0xFFFFFFFF;
	Val_Cfg1 = 0xFFFFFFFF;
	//WDT Timer HW enable
	Val_Cfg0 &= 0x7FFFFFE7;
	if (m_CWDTEN.GetCurSel() == 2)
		Val_Cfg0 |= 0x80000018;
	else if (m_CWDTEN.GetCurSel() == 1)
		Val_Cfg0 |= 0x00000018;
	else if (m_CWDTEN.GetCurSel() == 0)
		Val_Cfg0 |= 0x0;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
	// WDT CLK Power-down enable
	Val_Cfg0 &= 0xBFFFFFFF;
	if (m_CWDTPDEN.GetCurSel() == 1)
		Val_Cfg0 |= 0x40000000;
	else if (m_CWDTPDEN.GetCurSel() == 0)
		Val_Cfg0 |= 0x0;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
	//Multi-Function Select
	Val_Cfg0 &= 0xF7FFFFFF;
	if (m_CFGXT1.GetCurSel() == 1)
		Val_Cfg0 |= 0x08000000;
	else if (m_CFGXT1.GetCurSel() == 0)
		Val_Cfg0 |= 0x0;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
	//CPU CLK Source Selection after RST
	Val_Cfg0 &= 0xFBFFFFFF;
	if (m_CFOSC.GetCurSel() == 1)
		Val_Cfg0 |= 0x04000000;
	else if (m_CFOSC.GetCurSel() == 0)
		Val_Cfg0 |= 0x0;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
	//Brown-out Detection
	Val_Cfg0 &= 0xFF7FFFFF;
	if (m_CBODEN.GetCurSel() == 1)
		Val_Cfg0 |= 0x800000;
	else if (m_CBODEN.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
	//Brown-out Voltage
	Val_Cfg0 &= 0xFF9FFFFF;
	if (m_CBOV.GetCurSel() == 3)
		Val_Cfg0 |= 0x600000;
	else if (m_CBOV.GetCurSel() == 2)
		Val_Cfg0 |= 0x400000;
	else if (m_CBOV.GetCurSel() == 1)
		Val_Cfg0 |= 0x200000;
	else if (m_CBOV.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
	//Brown-out RST
	Val_Cfg0 &= 0xFFEFFFFF;
	if (m_CBORST.GetCurSel() == 1)
		Val_Cfg0 |= 0x100000;
	else if (m_CBORST.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
	//IO Initial state
	Val_Cfg0 &= 0xFFFFFBFF;
	if (m_CIOINI.GetCurSel() == 1)
		Val_Cfg0 |= 0x0400;
	else if (m_CIOINI.GetCurSel() == 0)
		Val_Cfg0 |= 0x0;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
	//Boot Selection
	Val_Cfg0 &= 0xFFFFFF3F;
	if (m_CBS.GetCurSel() == 3)
		Val_Cfg0 |= 0xC0;
	else if (m_CBS.GetCurSel() == 2)
		Val_Cfg0 |= 0x80;
	else if (m_CBS.GetCurSel() == 1)
		Val_Cfg0 |= 0x40;
	else if (m_CBS.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
	//Boot Loader Selection
	Val_Cfg0 &= 0xFFFFFFDF;
	if (m_MBS.GetCurSel() == 1)
		Val_Cfg0 |= 0x20;
	else if (m_MBS.GetCurSel() == 0)
		Val_Cfg0 |= 0x0;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
	//Security Lock
	Val_Cfg0 &= 0xFFFFFFFD;
	if (m_LOCK.GetCurSel() == 1)
		Val_Cfg0 |= 0x02;
	else if (m_LOCK.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
	//DataFlash Enable
	Val_Cfg0 &= 0xFFFFFFFE;
	if (m_DFEN.GetCurSel() == 1)
	{
		Val_Cfg0 |= 0x01;
		Val_Cfg1 = 0xFFFFFFFF;
		strTemp2.Format(_T("%08X"), Val_Cfg1);
		m_Cfg1.SetWindowTextW(strTemp2);
		m_Addr.SetWindowTextW(strTemp2);
		m_Addr.EnableWindow(false);
	}
	else if (m_DFEN.GetCurSel() == 0)
	{
		Val_Cfg0 |= 0x00;
		m_Addr.EnableWindow(true);
	}
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
}

void CDlg_M451AE::OnOK()
{
	CString temp;
	m_Cfg0.GetWindowTextW(temp);
	Val_Cfg0 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff, &Val_Cfg0, sizeof(Val_Cfg0));
	m_Cfg1.GetWindowTextW(temp);
	Val_Cfg1 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff + sizeof(Val_Cfg0), &Val_Cfg1, sizeof(Val_Cfg1));
	*m_pDataLen = sizeof(Val_Cfg0)+sizeof(Val_Cfg1);
	CMFCPropertyPage::OnOK();

}

LRESULT CDlg_M451AE::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();
}


LRESULT CDlg_M451AE::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}

void CDlg_M451AE::OnEnChangeAddr()
{
	CString Addr;
	if (m_DFEN.GetCurSel() == 0)
	{
		m_Addr.GetWindowTextW(Addr);
		m_Cfg1.SetWindowTextW(Addr);
		Val_Cfg1 = numeric_conversion::hexstring_to_size_t(Addr.GetString());
	}
}
