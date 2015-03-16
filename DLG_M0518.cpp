// DLG_M0518.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_M0518.h"
#include "afxdialogex.h"
#include "../common/prog_info.h"
#include "numeric_conversion.h"

// DLG_M0518 dialog

IMPLEMENT_DYNAMIC(CDLG_M0518, CMFCPropertyPage)

CDLG_M0518::CDLG_M0518(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
	: CMFCPropertyPage(CDLG_M0518::IDD),
	m_ChipInfo(ChipInfo),
	m_RegBuff(RegisterBuff),
	m_BuffLen(BuffLen),
	m_pDataLen(pDataLen),
	m_Len(0)
{

}

CDLG_M0518::~CDLG_M0518()
{
}

void CDLG_M0518::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_WDTEN);
	DDX_Control(pDX, IDC_COMBO2, m_WDTPDEN);
	DDX_Control(pDX, IDC_COMBO3, m_GPFMFP);
	DDX_Control(pDX, IDC_COMBO4, m_OSC);
	DDX_Control(pDX, IDC_COMBO5, m_BroDet);
	DDX_Control(pDX, IDC_COMBO6, m_BroVol);
	DDX_Control(pDX, IDC_COMBO7, m_BroRst);
	DDX_Control(pDX, IDC_COMBO8, m_IOinit);
	DDX_Control(pDX, IDC_COMBO9, m_Boot);
	DDX_Control(pDX, IDC_COMBO10, m_lock);
	DDX_Control(pDX, IDC_COMBO11, m_DataFlashEN);
	DDX_Control(pDX, IDC_EDIT1, m_cfg0);
	DDX_Control(pDX, IDC_EDIT2, m_cfg1);
}


BEGIN_MESSAGE_MAP(CDLG_M0518, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDLG_M0518::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDLG_M0518::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDLG_M0518::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDLG_M0518::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDLG_M0518::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDLG_M0518::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDLG_M0518::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CDLG_M0518::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO9, &CDLG_M0518::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO10, &CDLG_M0518::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO11, &CDLG_M0518::OnCbnSelchangeCombo)
	ON_EN_CHANGE(IDC_EDIT3, &CDLG_M0518::OnEnChangeEdit3)
	ON_BN_CLICKED(IDC_BUTTON1, &CDLG_M0518::OnBnClickedButton1)
END_MESSAGE_MAP()


// DLG_M0518 message handlers
BOOL CDLG_M0518::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	Val_Cfg0 = 0xFFFFFFFF;
	Val_Cfg1 = 0xFFFFFFFF;
	m_cfg0.SetWindowTextW(L"FFFFFFFF");
	m_cfg1.SetWindowTextW(L"FFFFFFFF");
	GetDlgItem(IDC_EDIT3)->SetWindowTextW(L"FFFFFFFF");
	m_cfg0.EnableWindow(false);
	m_cfg1.EnableWindow(false);
	m_WDTEN.InsertString(m_WDTEN.GetCount(), L"000 = WDT is active and WDT Clock is always on");
	m_WDTEN.InsertString(m_WDTEN.GetCount(), L"011 = WDT is active and WDT Clock is controlled by WDT CLK Power Down");
	m_WDTEN.InsertString(m_WDTEN.GetCount(), L"111 = WDT is inactive");

	m_WDTPDEN.InsertString(m_WDTPDEN.GetCount(), L"0 = Keep Enable");
	m_WDTPDEN.InsertString(m_WDTPDEN.GetCount(), L"1 = Controlled by OSC 10K");

	m_GPFMFP.InsertString(m_GPFMFP.GetCount(), L"0 = GPIO");
	m_GPFMFP.InsertString(m_GPFMFP.GetCount(), L"1 = 4~24MHz External Crystal Pin");

	m_OSC.InsertString(m_OSC.GetCount(), L"000 = External XTAL");
	m_OSC.InsertString(m_OSC.GetCount(), L"111 = Internal OSC(22.1184MHz)");

	m_BroDet.InsertString(m_BroDet.GetCount(), L"0 = Enabled Brown-Out detect after power on");
	m_BroDet.InsertString(m_BroDet.GetCount(), L"1 = Diaable Brown-Out detect after Power on");

	m_BroVol.InsertString(m_BroVol.GetCount(), L"00 = 2.2V");
	m_BroVol.InsertString(m_BroVol.GetCount(), L"01 = 2.7V");
	m_BroVol.InsertString(m_BroVol.GetCount(), L"10 = 3.7V");
	m_BroVol.InsertString(m_BroVol.GetCount(), L"11 = 4.4V");

	m_BroRst.InsertString(m_BroRst.GetCount(), L"0 = Enable Brown Out Reset after Power on");
	m_BroRst.InsertString(m_BroRst.GetCount(), L"1 = Disable Brown Out Reset after Power on");

	m_IOinit.InsertString(m_IOinit.GetCount(), L"0 = Input Tri-State Mode");
	m_IOinit.InsertString(m_IOinit.GetCount(), L"1 = Quasi-bidirectional Mode");

	m_Boot.InsertString(m_Boot.GetCount(), L"00 = Boot from LDROM with IAP");
	m_Boot.InsertString(m_Boot.GetCount(), L"01 = Boot from LDROM");
	m_Boot.InsertString(m_Boot.GetCount(), L"10 = Boot from APROM with IAP");
	m_Boot.InsertString(m_Boot.GetCount(), L"11 = Boot from APROM");

	m_lock.InsertString(m_lock.GetCount(), L"0 = Lock Enable");
	m_lock.InsertString(m_lock.GetCount(), L"1 = Lock Disable");

	m_DataFlashEN.InsertString(m_DataFlashEN.GetCount(), L"0X0 = APROM and DataFlash share flash");
	m_DataFlashEN.InsertString(m_DataFlashEN.GetCount(), L"0X1 = DataFlash Disable");
	m_DataFlashEN.InsertString(m_DataFlashEN.GetCount(), L"1X1 = DataFlash = 4KB");

	m_WDTEN.SetCurSel(2);
	m_WDTPDEN.SetCurSel(1);
	m_GPFMFP.SetCurSel(1);
	m_OSC.SetCurSel(1);
	m_BroDet.SetCurSel(1);
	m_BroVol.SetCurSel(3);
	m_BroRst.SetCurSel(1);
	m_IOinit.SetCurSel(1);
	m_Boot.SetCurSel(3);
	m_lock.SetCurSel(1);
	m_DataFlashEN.SetCurSel(2);
	GetDlgItem(IDC_EDIT3)->EnableWindow(false);
	return TRUE;
}

BOOL CDLG_M0518::OnSetActive()
{
	CString CFG0;
	CString CFG1;
	if (*m_pDataLen != NULL)
	{
		memcpy((void*)&Val_Cfg0, m_RegBuff, 4);
		memcpy((void*)&Val_Cfg1, m_RegBuff + 4 , 4);
		//CFG0
		CFG0.Format(_T("%08X"), Val_Cfg0);
		GetDlgItem(IDC_EDIT1)->SetWindowTextW(CFG0);
		//CFG1
		CFG1.Format(_T("%08X"), Val_Cfg1);
		GetDlgItem(IDC_EDIT2)->SetWindowTextW(CFG1);
		GetDlgItem(IDC_EDIT3)->SetWindowTextW(CFG1);
		//WatchDog Timer Enable
		if ((Val_Cfg0 & 0x80000018) == 0x80000018)
			m_WDTEN.SetCurSel(2);
		else if ((Val_Cfg0 & 0x80000018) == 0x00000018)
			m_WDTEN.SetCurSel(1);
		else if ((Val_Cfg0 & 0x80000018) == 0x0)
			m_WDTEN.SetCurSel(0);
		//WatchDog Clock Power Down
		if ((Val_Cfg0 & 0x40000000) == 0x40000000)
			m_WDTPDEN.SetCurSel(1);
		else if ((Val_Cfg0 & 0x40000000) == 0x0)
			m_WDTPDEN.SetCurSel(0);
		//XT_IN/OUT Function
		if ((Val_Cfg0 & 0x08000000) == 0x08000000)
			m_GPFMFP.SetCurSel(1);
		else if ((Val_Cfg0 & 0x08000000) == 0x0)
			m_GPFMFP.SetCurSel(0);
		//Clock Source Selection
		if ((Val_Cfg0 & 0x07000000) == 0x07000000)
			m_OSC.SetCurSel(1);
		else if ((Val_Cfg0 & 0x07000000) == 0x00)
			m_OSC.SetCurSel(0);
		//Brown-Out Detection
		if ((Val_Cfg0 & 0x00800000) == 0x00800000)
			m_BroDet.SetCurSel(1);
		else if ((Val_Cfg0 & 0x00800000) == 0x00)
			m_BroDet.SetCurSel(0);
		//Brown-Out Voltage
		if ((Val_Cfg0 & 0x00600000) == 0x00600000)
			m_BroVol.SetCurSel(3);
		else if ((Val_Cfg0 & 0x00600000) == 0x00400000)
			m_BroVol.SetCurSel(2);
		else if ((Val_Cfg0 & 0x00600000) == 0x00200000)
			m_BroVol.SetCurSel(1);
		else if ((Val_Cfg0 & 0x00600000) == 0x00)
			m_BroVol.SetCurSel(0);
		//Brown-Out Reset
		if ((Val_Cfg0 & 0x00100000) == 0x00100000)
			m_BroRst.SetCurSel(1);
		else if ((Val_Cfg0 & 0x00600000) == 0x00)
			m_BroRst.SetCurSel(0);
		//IO Initial State
		if ((Val_Cfg0 & 0x00000400) == 0x00000400)
			m_IOinit.SetCurSel(1);
		else if ((Val_Cfg0 & 0x00000400) == 0x00)
			m_IOinit.SetCurSel(0);
		//Boot Select
		if ((Val_Cfg0 & 0x000000C0) == 0xC0)
			m_Boot.SetCurSel(3);
		else if ((Val_Cfg0 & 0x000000C0) == 0x80)
			m_Boot.SetCurSel(2);
		else if ((Val_Cfg0 & 0x000000C0) == 0x40)
			m_Boot.SetCurSel(1);
		else if ((Val_Cfg0 & 0x000000C0) == 0x00)
			m_Boot.SetCurSel(0);
		//Code Protect
		if ((Val_Cfg0 & 0x00000002) == 0x02)
			m_lock.SetCurSel(1);
		else if ((Val_Cfg0 & 0x00000002) == 0x00)
			m_lock.SetCurSel(0);
		//DataFlash Enable
		if ((Val_Cfg0 & 0x00000005) == 0x05)
		{
			m_DataFlashEN.SetCurSel(2);
			GetDlgItem(IDC_EDIT3)->EnableWindow(false);
		}
		else if ((Val_Cfg0 & 0x00000005) == 0x01)
		{
			m_DataFlashEN.SetCurSel(1);
			GetDlgItem(IDC_EDIT3)->EnableWindow(false);
		}
		else if ((Val_Cfg0 & 0x00000005) == 0x00)
		{
			m_DataFlashEN.SetCurSel(0);
			GetDlgItem(IDC_EDIT3)->EnableWindow(true);
		}
			
	}
	return CMFCPropertyPage::OnSetActive();
}
void CDLG_M0518::OnCbnSelchangeCombo()
{
	CString strTemp; //cfg0
	CString strTemp2; //cfg1
	Val_Cfg0 = 0xFFFFFFFF;
	Val_Cfg1 = 0xFFFFFFFF;
	//Watch dog Enable
	Val_Cfg0 &= 0x7FFFFFE7;
	switch (m_WDTEN.GetCurSel())
	{
		case 0:
			Val_Cfg0 |= 0x00000000;
			break;
		case 1:
			Val_Cfg0 |= 0x00000018;
			break;
		case 2:
			Val_Cfg0 |= 0x80000018;
			break;
	}
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(strTemp);
	//Watch dog Power Down
	Val_Cfg0 &= 0xBFFFFFFF;
	if (m_WDTPDEN.GetCurSel() == 1)
		Val_Cfg0 |= 0x40000000;
	else if (m_WDTPDEN.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(strTemp);
	//XT1_IN/OUT Selection
	Val_Cfg0 &= 0xF7FFFFFF;
	if (m_GPFMFP.GetCurSel() == 1)
		Val_Cfg0 |= 0x08000000;
	else if (m_GPFMFP.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(strTemp);
	//CLK Source
	Val_Cfg0 &= 0xF8FFFFFF;
	if (m_OSC.GetCurSel() == 1)
		Val_Cfg0 |= 0x07000000;
	else if (m_OSC.GetCurSel() == 0)
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
	switch (m_BroVol.GetCurSel())
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
	//IO Initial
	Val_Cfg0 &= 0xFFFFFBFF;
	if (m_IOinit.GetCurSel() == 1)
		Val_Cfg0 |= 0x0400;
	else if (m_IOinit.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(strTemp);
	//Boot Select
	Val_Cfg0 &= 0xFFFFFF3F;
	switch (m_Boot.GetCurSel())
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
	//Code Protect
	Val_Cfg0 &= 0xFFFFFFFD;
	if (m_lock.GetCurSel() == 1)
		Val_Cfg0 |= 0x02;
	else if (m_lock.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(strTemp);
	//DataFlash
	Val_Cfg0 &= 0xFFFFFFFA;
	switch (m_DataFlashEN.GetCurSel())
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

void CDLG_M0518::OnOK()
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

void CDLG_M0518::OnEnChangeEdit3()
{
	CString Addr;
	if (m_DataFlashEN.GetCurSel() == 0)
	{
		GetDlgItem(IDC_EDIT3)->GetWindowTextW(Addr);
		m_cfg1.SetWindowTextW(Addr);
		Val_Cfg1 = numeric_conversion::hexstring_to_size_t(Addr.GetString());
	}
}

void CDLG_M0518::OnBnClickedButton1()
{
	m_cfg0.EnableWindow(true);
	m_cfg1.EnableWindow(true);
}

LRESULT CDLG_M0518::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();
}
LRESULT CDLG_M0518::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}