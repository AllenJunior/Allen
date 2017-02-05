// Dlg_M0519.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_M0519.h"
#include "afxdialogex.h"
#include "../common/prog_info.h"
#include "numeric_conversion.h"

// Dlg_M0519 dialog

IMPLEMENT_DYNAMIC(CDlg_M0519, CMFCPropertyPage)

CDlg_M0519::CDlg_M0519(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_M0519::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_M0519::~CDlg_M0519()
{
}

void CDlg_M0519::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_cfg0);
	DDX_Control(pDX, IDC_EDIT2, m_cfg1);
	DDX_Control(pDX, IDC_COMBO1, m_CWDTEN);
	DDX_Control(pDX, IDC_COMBO2, m_CWDTPDEN);
	DDX_Control(pDX, IDC_COMBO3, m_CFOSC);
	DDX_Control(pDX, IDC_COMBO4, m_CBODEN);
	DDX_Control(pDX, IDC_COMBO5, m_CBOV);
	DDX_Control(pDX, IDC_COMBO6, m_CBORST);
	DDX_Control(pDX, IDC_COMBO7, m_CHZ_BPWM);
	DDX_Control(pDX, IDC_COMBO8, m_CHZ_Odd1);
	DDX_Control(pDX, IDC_COMBO9, m_CHZ_Even1);
	DDX_Control(pDX, IDC_COMBO10, m_CHZ_Odd0);
	DDX_Control(pDX, IDC_COMBO11, m_CHZ_Even0);
	DDX_Control(pDX, IDC_COMBO12, m_CBS);
	DDX_Control(pDX, IDC_COMBO13, m_LOCK);
	DDX_Control(pDX, IDC_COMBO14, m_DFEN);
	DDX_Control(pDX, IDC_EDIT3, m_Addr);
}


BEGIN_MESSAGE_MAP(CDlg_M0519, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_M0519::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_M0519::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_M0519::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_M0519::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_M0519::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDlg_M0519::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDlg_M0519::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CDlg_M0519::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO9, &CDlg_M0519::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO10, &CDlg_M0519::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO11, &CDlg_M0519::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO12, &CDlg_M0519::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO13, &CDlg_M0519::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO14, &CDlg_M0519::OnCbnSelchangeCombo)
	ON_EN_CHANGE(IDC_EDIT3, &CDlg_M0519::OnEnChangeAddr)
END_MESSAGE_MAP()


// Dlg_M0519 message handlers
BOOL CDlg_M0519::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	Val_Cfg0 = 0xFFFFFFFF;
	Val_Cfg1 = 0xFFFFFFFF;
	m_cfg0.SetWindowTextW(L"FFFFFFFF");
	m_cfg1.SetWindowTextW(L"FFFFFFFF");
	m_Addr.SetWindowTextW(L"FFFFFFFF");
	m_CWDTEN.InsertString(m_CWDTEN.GetCount(), L"0 = Watchdog Timer Enable");
	m_CWDTEN.InsertString(m_CWDTEN.GetCount(), L"1 = Watchdog Timer Disable");
	m_CWDTPDEN.InsertString(m_CWDTPDEN.GetCount(), L"0 = OSC10K Watchdog Timer Enable");
	m_CWDTPDEN.InsertString(m_CWDTPDEN.GetCount(), L"1 = OSC10K Watchdog Timer Disable");
	m_CFOSC.InsertString(m_CFOSC.GetCount(), L"000 = 4~24MHz External Clock Source");
	m_CFOSC.InsertString(m_CFOSC.GetCount(), L"111 = 22.1184MHz Internal Clock Source");
	m_CBODEN.InsertString(m_CBODEN.GetCount(), L"0 = Brown-out Detector Enable");
	m_CBODEN.InsertString(m_CBODEN.GetCount(), L"1 = Brown-out Detector Disable");
	m_CBOV.InsertString(m_CBOV.GetCount(), L"00 = Brown-out Voltage 2.2V");
	m_CBOV.InsertString(m_CBOV.GetCount(), L"01 = Brown-out Voltage 2.7V");
	m_CBOV.InsertString(m_CBOV.GetCount(), L"10 = Brown-out Voltage 3.7V");
	m_CBOV.InsertString(m_CBOV.GetCount(), L"11 = Brown-out Voltage 4.4V");
	m_CBORST.InsertString(m_CBORST.GetCount(), L"0 = Brown-out Reset Enable");
	m_CBORST.InsertString(m_CBORST.GetCount(), L"1 = Brown-out Reset Disable");
	m_CHZ_BPWM.InsertString(m_CHZ_BPWM.GetCount(), L"0 = Controlled by GPIO");
	m_CHZ_BPWM.InsertString(m_CHZ_BPWM.GetCount(), L"1 = Forced in Tri-state");
	m_CHZ_Odd1.InsertString(m_CHZ_Odd1.GetCount(), L"0 = Controlled by GPIO");
	m_CHZ_Odd1.InsertString(m_CHZ_Odd1.GetCount(), L"1 = Forced in Tri-state");
	m_CHZ_Even1.InsertString(m_CHZ_Even1.GetCount(), L"0 = Controlled by GPIO");
	m_CHZ_Even1.InsertString(m_CHZ_Even1.GetCount(), L"1 = Forced in Tri-state");
	m_CHZ_Odd0.InsertString(m_CHZ_Odd0.GetCount(), L"0 = Controlled by GPIO");
	m_CHZ_Odd0.InsertString(m_CHZ_Odd0.GetCount(), L"1 = Forced in Tri-state");
	m_CHZ_Even0.InsertString(m_CHZ_Even0.GetCount(), L"0 = Controlled by GPIO");
	m_CHZ_Even0.InsertString(m_CHZ_Even0.GetCount(), L"1 = Forced in Tri-state");
	m_CBS.InsertString(m_CBS.GetCount(), L"00 = Boot from LDROM with IAP");
	m_CBS.InsertString(m_CBS.GetCount(), L"01 = Boot from LDROM");
	m_CBS.InsertString(m_CBS.GetCount(), L"10 = Boot from APROM with IAP");
	m_CBS.InsertString(m_CBS.GetCount(), L"11 = Boot from APROM");
	m_LOCK.InsertString(m_LOCK.GetCount(), L"0 = Lock Enable");
	m_LOCK.InsertString(m_LOCK.GetCount(), L"1 = Lock Disable");
	m_DFEN.InsertString(m_DFEN.GetCount(), L"0 = DataFlash Enable");
	m_DFEN.InsertString(m_DFEN.GetCount(), L"1 = DataFlash Enable");

	m_CWDTEN.SetCurSel(1);
	m_CWDTPDEN.SetCurSel(1);
	m_CFOSC.SetCurSel(1);
	m_CBODEN.SetCurSel(1);
	m_CBOV.SetCurSel(3);
	m_CBORST.SetCurSel(1);
	m_CHZ_BPWM.SetCurSel(1);
	m_CHZ_Odd1.SetCurSel(1);
	m_CHZ_Even1.SetCurSel(1);
	m_CHZ_Odd0.SetCurSel(1);
	m_CHZ_Even0.SetCurSel(1);
	m_CBS.SetCurSel(3);
	m_LOCK.SetCurSel(1);
	m_DFEN.SetCurSel(1);
	m_Addr.EnableWindow(FALSE);

	return TRUE;
}
BOOL CDlg_M0519::OnSetActive()
{
	CString CFG0;
	CString CFG1;
	if (*m_pDataLen != NULL)
	{
		memcpy((void*)&Val_Cfg0, m_RegBuff, sizeof(Val_Cfg0));
		memcpy((void*)&Val_Cfg1, m_RegBuff + sizeof(Val_Cfg0), sizeof(Val_Cfg1));
		//Config0
		CFG0.Format(_T("%08X"), Val_Cfg0);
		m_cfg0.SetWindowTextW(CFG0);
		//COnfig1
		CFG1.Format(_T("%08X"), Val_Cfg1);
		m_cfg1.SetWindowTextW(CFG1);
		m_Addr.SetWindowTextW(CFG1);
		//Watchdog Timer Enable
		if ((Val_Cfg0 & 0x80000000) == 0x80000000)
			m_CWDTEN.SetCurSel(1);
		else if ((Val_Cfg0 & 0x80000000) == 0x00)
			m_CWDTEN.SetCurSel(0);
		//Watchdog Power down
		if ((Val_Cfg0 & 0x40000000) == 0x40000000)
			m_CWDTPDEN.SetCurSel(1);
		else if ((Val_Cfg0 & 0x40000000) == 0x00)
			m_CWDTPDEN.SetCurSel(0);
		//Clock Source Selection
		if ((Val_Cfg0 & 0x07000000) == 0x07000000)
			m_CFOSC.SetCurSel(1);
		else if ((Val_Cfg0 & 0x07000000) == 0x00)
			m_CFOSC.SetCurSel(0);
		//Brown-Out Detection
		if ((Val_Cfg0 & 0x800000) == 0x800000)
			m_CBODEN.SetCurSel(1);
		else if ((Val_Cfg0 & 0x800000) == 0x00)
			m_CBODEN.SetCurSel(0);
		//Brown-Out Voltage
		if ((Val_Cfg0 & 0x600000) == 0x600000)
			m_CBOV.SetCurSel(3);
		else if ((Val_Cfg0 & 0x600000) == 0x400000)
			m_CBOV.SetCurSel(2);
		else if ((Val_Cfg0 & 0x600000) == 0x200000)
			m_CBOV.SetCurSel(1);
		else if ((Val_Cfg0 & 0x600000) == 0x00)
			m_CBOV.SetCurSel(0);
		//Brown-Out Reset
		if ((Val_Cfg0 & 0x100000) == 0x100000)
			m_CBORST.SetCurSel(1);
		else if ((Val_Cfg0 & 0x100000) == 0x00)
			m_CBORST.SetCurSel(0);
		//Basic PWM Ports
		if ((Val_Cfg0 & 0x1000) == 0x1000)
			m_CHZ_BPWM.SetCurSel(1);
		else if ((Val_Cfg0 & 0x1000) == 0x00)
			m_CHZ_BPWM.SetCurSel(0);
		//PWM Odd1
		if ((Val_Cfg0 & 0x0800) == 0x0800)
			m_CHZ_Odd1.SetCurSel(1);
		else if ((Val_Cfg0 & 0x0800) == 0x00)
			m_CHZ_Odd1.SetCurSel(0);
		//PWM Even1
		if ((Val_Cfg0 & 0x0400) == 0x0400)
			m_CHZ_Even1.SetCurSel(1);
		else if ((Val_Cfg0 & 0x0400) == 0x00)
			m_CHZ_Even1.SetCurSel(0);
		//PWM Odd0
		if ((Val_Cfg0 & 0x0200) == 0x0200)
			m_CHZ_Odd0.SetCurSel(1);
		else if ((Val_Cfg0 & 0x0200) == 0x00)
			m_CHZ_Odd0.SetCurSel(0);
		//PWM Even0
		if ((Val_Cfg0 & 0x0100) == 0x0100)
			m_CHZ_Even0.SetCurSel(1);
		else if ((Val_Cfg0 & 0x0100) == 0x00)
			m_CHZ_Even0.SetCurSel(0);
		//Boot Select
		if ((Val_Cfg0 & 0xC0) == 0xC0)
			m_CBS.SetCurSel(3);
		else if ((Val_Cfg0 & 0xC0) == 0x80)
			m_CBS.SetCurSel(2);
		else if ((Val_Cfg0 & 0xC0) == 0x40)
			m_CBS.SetCurSel(1);
		else if ((Val_Cfg0 & 0xC0) == 0x00)
			m_CBS.SetCurSel(0);
		//Code Protect
		if ((Val_Cfg0 & 0x02) == 0x02)
			m_LOCK.SetCurSel(1);
		else if ((Val_Cfg0 & 0x02) == 0x00)
			m_LOCK.SetCurSel(0);
		//Data Flash Enable
		if ((Val_Cfg0 & 0x01) == 0x01)
		{
			m_DFEN.SetCurSel(1);
			m_Addr.EnableWindow(FALSE);
		}
		else if ((Val_Cfg0 & 0x01) == 0x00)
		{
			m_DFEN.SetCurSel(0);
			m_Addr.EnableWindow(TRUE);
		}
			
	}
	return CMFCPropertyPage::OnSetActive();
}
void CDlg_M0519::OnCbnSelchangeCombo()
{
	CString temp_str;
	Val_Cfg0 = 0xFFFFFFFF;
	Val_Cfg1 = 0xFFFFFFFF;
	//Watchdog Timer Enable
	Val_Cfg0 &= 0x7FFFFFFF;
	if (m_CWDTEN.GetCurSel() == 1)
		Val_Cfg0 |= 0x80000000;
	else if (m_CWDTEN.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	temp_str.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(temp_str);
	//Watchdog Power down enable
	Val_Cfg0 &= 0xBFFFFFFF;
	if (m_CWDTPDEN.GetCurSel() == 1)
		Val_Cfg0 |= 0x40000000;
	else if (m_CWDTPDEN.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	temp_str.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(temp_str);
	//Clock Source
	Val_Cfg0 &= 0xF8FFFFFF;
	if (m_CFOSC.GetCurSel() == 1)
		Val_Cfg0 |= 0x07000000;
	else if (m_CFOSC.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	temp_str.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(temp_str);
	//Brown-out Detector
	Val_Cfg0 &= 0xFF7FFFFF;
	if (m_CBODEN.GetCurSel() == 1)
		Val_Cfg0 |= 0x800000;
	else if (m_CBODEN.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	temp_str.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(temp_str);
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
	temp_str.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(temp_str);
	//Brown-Out Reset
	Val_Cfg0 &= 0xFFEFFFFF;
	if (m_CBORST.GetCurSel() == 1)
		Val_Cfg0 |= 0x100000;
	else if (m_CBORST.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	temp_str.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(temp_str);
	//Basic PWM
	Val_Cfg0 &= 0xFFFFEFFF;
	if (m_CHZ_BPWM.GetCurSel() == 1)
		Val_Cfg0 |= 0x1000;
	else if (m_CHZ_BPWM.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	temp_str.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(temp_str);
	//PWM Odd1
	Val_Cfg0 &= 0xFFFFF7FF;
	if (m_CHZ_Odd1.GetCurSel() == 1)
		Val_Cfg0 |= 0x0800;
	else if (m_CHZ_Odd1.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	temp_str.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(temp_str);
	//PWM Even1
	Val_Cfg0 &= 0xFFFFFBFF;
	if (m_CHZ_Even1.GetCurSel() == 1)
		Val_Cfg0 |= 0x0400;
	else if (m_CHZ_Even1.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	temp_str.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(temp_str);
	//PWM Odd0
	Val_Cfg0 &= 0xFFFFFDFF;
	if (m_CHZ_Odd0.GetCurSel() == 1)
		Val_Cfg0 |= 0x0200;
	else if (m_CHZ_Odd0.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	temp_str.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(temp_str);
	//PWM Even0
	Val_Cfg0 &= 0xFFFFFEFF;
	if (m_CHZ_Even0.GetCurSel() == 1)
		Val_Cfg0 |= 0x0100;
	else if (m_CHZ_Even0.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	temp_str.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(temp_str);
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
	temp_str.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(temp_str);
	//Security Lock
	Val_Cfg0 &= 0xFFFFFFFD;
	if (m_LOCK.GetCurSel() == 1)
		Val_Cfg0 |= 0x02;
	else if (m_LOCK.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	temp_str.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(temp_str);
	//Data Flash
	Val_Cfg0 &= 0xFFFFFFFE;
	if (m_DFEN.GetCurSel() == 1)
	{
		Val_Cfg0 |= 0x01;
		Val_Cfg1 = 0xFFFFFFFF;
		m_cfg1.SetWindowTextW(L"FFFFFFFF");
		m_Addr.SetWindowTextW(L"FFFFFFFF");
		m_Addr.EnableWindow(FALSE);
	}
	else if (m_DFEN.GetCurSel() == 0)
	{
		Val_Cfg0 |= 0x00; 
		m_Addr.EnableWindow(TRUE);
	}
	temp_str.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(temp_str);
}

void CDlg_M0519::OnOK()
{
	CString temp;
	m_cfg0.GetWindowTextW(temp);
	Val_Cfg0 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff, &Val_Cfg0, sizeof(Val_Cfg0));
	m_cfg1.GetWindowTextW(temp);
	Val_Cfg1 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff + sizeof(Val_Cfg0), &Val_Cfg1, sizeof(Val_Cfg1));
	*m_pDataLen = sizeof(Val_Cfg0)+sizeof(Val_Cfg1);
	CMFCPropertyPage::OnOK();
}

LRESULT CDlg_M0519::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();
}
LRESULT CDlg_M0519::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}


void CDlg_M0519::OnEnChangeAddr()
{
	CString Addr;
	if (m_DFEN.GetCurSel() == 0)
	{
		m_Addr.GetWindowTextW(Addr);
		m_cfg1.SetWindowTextW(Addr);
		Val_Cfg1 = numeric_conversion::hexstring_to_size_t(Addr.GetString());
	}
}
