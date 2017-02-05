// Dlg_M05XD.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_M05XD.h"
#include "afxdialogex.h"
#include "../common/prog_info.h"
#include "numeric_conversion.h"


// Dlg_M05XD dialog

IMPLEMENT_DYNAMIC(CDlg_M05XD, CMFCPropertyPage)

CDlg_M05XD::CDlg_M05XD(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_M05XD::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_M05XD::~CDlg_M05XD()
{
}

void CDlg_M05XD::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Cfg0);
	DDX_Control(pDX, IDC_COMBO1, m_CWDTEN);
	DDX_Control(pDX, IDC_COMBO2, m_CWDTPDEN);
	DDX_Control(pDX, IDC_COMBO3, m_CFOSC);
	DDX_Control(pDX, IDC_COMBO4, m_CBODEN);
	DDX_Control(pDX, IDC_COMBO5, m_CBOV);
	DDX_Control(pDX, IDC_COMBO6, m_CBORST);
	DDX_Control(pDX, IDC_COMBO7, m_CIOINI);
	DDX_Control(pDX, IDC_COMBO8, m_CBS);
	DDX_Control(pDX, IDC_COMBO9, m_Lock);
}


BEGIN_MESSAGE_MAP(CDlg_M05XD, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_M05XD::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_M05XD::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_M05XD::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_M05XD::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_M05XD::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDlg_M05XD::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDlg_M05XD::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CDlg_M05XD::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO9, &CDlg_M05XD::OnCbnSelchangeCombo)
END_MESSAGE_MAP()


// Dlg_M05XD message handlers
BOOL CDlg_M05XD::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	Val_Cfg0 = 0xFFFFFFFF;
	m_Cfg0.SetWindowTextW(L"FFFFFFFF");
	m_CWDTEN.InsertString(m_CWDTEN.GetCount(), L"0 = Watchdog Timer Enabled");
	m_CWDTEN.InsertString(m_CWDTEN.GetCount(), L"1 = Watchdog Timer Disabled");
	m_CWDTPDEN.InsertString(m_CWDTPDEN.GetCount(), L"0 = OSC10K WDT CLK Source Enabled");
	m_CWDTPDEN.InsertString(m_CWDTPDEN.GetCount(), L"1 = OSC10K WDT CLK Source Controlled by OSC10K_EN");
	m_CFOSC.InsertString(m_CFOSC.GetCount(), L"000 = External crystal clock (4~24MHz)");
	m_CFOSC.InsertString(m_CFOSC.GetCount(), L"111 = Internal RC 22.1184 MHz oscillator clock");
	m_CBODEN.InsertString(m_CBODEN.GetCount(), L"0 = Brown-out detect Enabled");
	m_CBODEN.InsertString(m_CBODEN.GetCount(), L"1 = Brown-out detect Disabled");
	m_CBOV.InsertString(m_CBOV.GetCount(), L"00 = 2.2V");
	m_CBOV.InsertString(m_CBOV.GetCount(), L"01 = 2.7V");
	m_CBOV.InsertString(m_CBOV.GetCount(), L"10 = 3.7V");
	m_CBOV.InsertString(m_CBOV.GetCount(), L"11 = 4.4V");
	m_CBORST.InsertString(m_CBORST.GetCount(), L"0 = Brown-out reset Enabled");
	m_CBORST.InsertString(m_CBORST.GetCount(), L"1 = Brown-out reset Disabled");
	m_CIOINI.InsertString(m_CIOINI.GetCount(), L"0 = input tri-state mode");
	m_CIOINI.InsertString(m_CIOINI.GetCount(), L"1 = Quasi-bidirectional mode");
	m_CBS.InsertString(m_CBS.GetCount(), L"00 = Boot from LDROM with IAP function");
	m_CBS.InsertString(m_CBS.GetCount(), L"01 = Boot from LDROM");
	m_CBS.InsertString(m_CBS.GetCount(), L"10 = Boot from APROM with IAP function");
	m_CBS.InsertString(m_CBS.GetCount(), L"11 = Boot from APROM");
	m_Lock.InsertString(m_Lock.GetCount(), L"0 = DataFlash is Locked");
	m_Lock.InsertString(m_Lock.GetCount(), L"1 = DataFlash is not Locked");
	m_CWDTEN.SetCurSel(1);
	m_CWDTPDEN.SetCurSel(1);
	m_CFOSC.SetCurSel(1);
	m_CBODEN.SetCurSel(1);
	m_CBOV.SetCurSel(3);
	m_CBORST.SetCurSel(1);
	m_CIOINI.SetCurSel(1);
	m_CBS.SetCurSel(3);
	m_Lock.SetCurSel(1);
	m_Cfg0.EnableWindow(false);
	return TRUE;
}


BOOL CDlg_M05XD::OnSetActive()
{
	CString CFG0;
	if (*m_pDataLen != NULL)
	{
		memcpy((void*)&Val_Cfg0, m_RegBuff, sizeof(Val_Cfg0));
		//CFG0
		CFG0.Format(_T("%08X"), Val_Cfg0);
		m_Cfg0.SetWindowTextW(CFG0);
		//Watchdog Enable Control
		if ((Val_Cfg0 & 0x80000000) == 0x80000000)
			m_CWDTEN.SetCurSel(1);
		else if ((Val_Cfg0 & 0x80000000) == 0x00)
			m_CWDTEN.SetCurSel(0);
		//Watchdog Clock Power-Down Enable Control
		if ((Val_Cfg0 & 0x40000000) == 0x40000000)
			m_CWDTPDEN.SetCurSel(1);
		else if ((Val_Cfg0 & 0x40000000) == 0x00)
			m_CWDTPDEN.SetCurSel(0);
		//CPU Clock Source Selection after Reset
		if ((Val_Cfg0 & 0x07000000) == 0x07000000)
			m_CFOSC.SetCurSel(1);
		else if ((Val_Cfg0 & 0x07000000) == 0x00)
			m_CFOSC.SetCurSel(0);
		//Brown-out Dectector
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
		//Brown-out Reset Enable
		if ((Val_Cfg0 & 0x100000) == 0x100000)
			m_CBORST.SetCurSel(1);
		else if ((Val_Cfg0 & 0x100000) == 0x00)
			m_CBORST.SetCurSel(0);
		//I/O Initial State Selection
		if ((Val_Cfg0 & 0x0400) == 0x0400)
			m_CIOINI.SetCurSel(1);
		else if ((Val_Cfg0 & 0x0400) == 0x00)
			m_CIOINI.SetCurSel(0);
		//Chip Boot Selection
		if ((Val_Cfg0 & 0xC0) == 0xC0)
			m_CBS.SetCurSel(3);
		else if ((Val_Cfg0 & 0xC0) == 0x80)
			m_CBS.SetCurSel(2);
		else if ((Val_Cfg0 & 0xC0) == 0x40)
			m_CBS.SetCurSel(1);
		else if ((Val_Cfg0 & 0xC0) == 0x00)
			m_CBS.SetCurSel(0);
		//Security Lock Control
		if ((Val_Cfg0 & 0x02) == 0x02)
			m_Lock.SetCurSel(1);
		else if ((Val_Cfg0 & 0x02) == 0x00)
			m_Lock.SetCurSel(0);
	}
	return CMFCPropertyPage::OnSetActive();
}

void CDlg_M05XD::OnCbnSelchangeCombo()
{
	CString strTemp;
	Val_Cfg0 = 0xFFFFFFFF;
	//Watchdog Enable control
	Val_Cfg0 &= 0x7FFFFFFF;
	if (m_CWDTEN.GetCurSel() == 1)
		Val_Cfg0 |= 0x80000000;
	else if (m_CWDTEN.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
	//Watchdog Clock Power Down Enable control
	Val_Cfg0 &= 0xBFFFFFFF;
	if (m_CWDTPDEN.GetCurSel() == 1)
		Val_Cfg0 |= 0x40000000;
	else if (m_CWDTPDEN.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
	//CPU Clock Source selection after reset
	Val_Cfg0 &= 0xF8FFFFFF;
	if (m_CFOSC.GetCurSel() == 1)
		Val_Cfg0 |= 0x07000000;
	else if (m_CFOSC.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
	//Brown-out Detector Enable Control
	Val_Cfg0 &= 0xFF7FFFFF;
	if (m_CBODEN.GetCurSel() == 1)
		Val_Cfg0 |= 0x800000;
	else if (m_CBODEN.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
	//Brown-out Voltage
	Val_Cfg0 &= 0xFF9FFFFF;
	switch (m_CBOV.GetCurSel())
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
	m_Cfg0.SetWindowTextW(strTemp);
	//Brown-out Reset Enable
	Val_Cfg0 &= 0xFFEFFFFF;
	if (m_CBORST.GetCurSel() == 1)
		Val_Cfg0 |= 0x100000;
	else if (m_CBORST.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
	//I/O Initial State Selection
	Val_Cfg0 &= 0xFFFFFBFF;
	if (m_CIOINI.GetCurSel() == 1)
		Val_Cfg0 |= 0x0400;
	else if (m_CIOINI.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
	//Chip Boot Selection
	Val_Cfg0 &= 0xFFFFFF3F;
	switch (m_CBS.GetCurSel())
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
	m_Cfg0.SetWindowTextW(strTemp);
	//Security Lock control
	Val_Cfg0 &= 0xFFFFFFFD;
	if (m_Lock.GetCurSel() == 1)
		Val_Cfg0 |= 0x02;
	else if (m_Lock.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
}

void CDlg_M05XD::OnOK()
{
	CString temp;
	m_Cfg0.GetWindowTextW(temp);
	Val_Cfg0 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	*m_pDataLen = sizeof(Val_Cfg0);
	memcpy(m_RegBuff, &Val_Cfg0, sizeof(Val_Cfg0));
	CMFCPropertyPage::OnOK();

}

LRESULT CDlg_M05XD::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();
}


LRESULT CDlg_M05XD::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}
