// Dlg_N79E7X.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_N79E7X.h"
#include "afxdialogex.h"
#include "../common/prog_info.h"
#include "numeric_conversion.h"

// Dlg_N79E7X dialog

IMPLEMENT_DYNAMIC(CDlg_N79E7X, CMFCPropertyPage)

CDlg_N79E7X::CDlg_N79E7X(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_N79E7X::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_N79E7X::~CDlg_N79E7X()
{
}

void CDlg_N79E7X::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Cfg0);
	DDX_Control(pDX, IDC_EDIT2, m_Cfg1);
	DDX_Control(pDX, IDC_EDIT3, m_Cfg2);
	DDX_Control(pDX, IDC_EDIT4, m_Cfg3);
	DDX_Control(pDX, IDC_EDIT5, m_Cfg4);
	DDX_Control(pDX, IDC_EDIT6, m_addr);
	DDX_Control(pDX, IDC_COMBO1, m_CBS);
	DDX_Control(pDX, IDC_COMBO2, m_LOCK);
	DDX_Control(pDX, IDC_COMBO3, m_DFEN);
	DDX_Control(pDX, IDC_COMBO4, m_CBODEN);
	DDX_Control(pDX, IDC_COMBO5, m_CBOV);
	DDX_Control(pDX, IDC_COMBO6, m_CBORST);
	DDX_Control(pDX, IDC_COMBO7, m_CWDTEN);
	DDX_Control(pDX, IDC_COMBO8, m_CKF);
	DDX_Control(pDX, IDC_COMBO9, m_OSCFS);
	DDX_Control(pDX, IDC_COMBO10, m_FOSC);
	DDX_Control(pDX, IDC_COMBO11, m_RSTDBE);
	DDX_Control(pDX, IDC_COMBO12, m_RSTDBS);
	DDX_Control(pDX, IDC_COMBO13, m_DIV2);
}


BEGIN_MESSAGE_MAP(CDlg_N79E7X, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_N79E7X::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_N79E7X::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_N79E7X::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_N79E7X::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_N79E7X::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDlg_N79E7X::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDlg_N79E7X::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CDlg_N79E7X::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO9, &CDlg_N79E7X::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO10, &CDlg_N79E7X::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO11, &CDlg_N79E7X::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO12, &CDlg_N79E7X::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO13, &CDlg_N79E7X::OnCbnSelchangeCombo)
	ON_EN_CHANGE(IDC_EDIT6, &CDlg_N79E7X::OnEnChangeAddr)
END_MESSAGE_MAP()


// Dlg_N79E7X message handlers
BOOL CDlg_N79E7X::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	Val_Cfg0 = 0xFF;
	Val_Cfg1 = 0xFF;
	Val_Cfg2 = 0xFF;
	Val_Cfg3 = 0xFF;
	Val_Cfg4 = 0xFF;
	m_Cfg0.SetWindowTextW(L"FF");
	m_Cfg1.SetWindowTextW(L"FF");
	m_Cfg2.SetWindowTextW(L"FF");
	m_Cfg3.SetWindowTextW(L"FF");
	m_Cfg4.SetWindowTextW(L"FF");
	m_addr.SetWindowTextW(L"FF");
	m_CBS.InsertString(m_CBS.GetCount(), L"0 = MCU Boot from LDROM");
	m_CBS.InsertString(m_CBS.GetCount(), L"1 = MCU Boot from APROM");
	m_LOCK.InsertString(m_LOCK.GetCount(), L"0 = Chip is Locked");
	m_LOCK.InsertString(m_LOCK.GetCount(), L"1 = Chip is unLocked");
	m_DFEN.InsertString(m_DFEN.GetCount(), L"0 = DataFlash exists");
	m_DFEN.InsertString(m_DFEN.GetCount(), L"1 = There's no DataFlash");
	m_CBODEN.InsertString(m_CBODEN.GetCount(), L"0 = Enable BOD detection");
	m_CBODEN.InsertString(m_CBODEN.GetCount(), L"1 = Disable BOD detection");
	m_CBOV.InsertString(m_CBOV.GetCount(), L"0 = 3.8V");
	m_CBOV.InsertString(m_CBOV.GetCount(), L"1 = 2.7V");
	m_CBORST.InsertString(m_CBORST.GetCount(), L"0 = Disable BOD Reset when VDD drops below VBOD");
	m_CBORST.InsertString(m_CBORST.GetCount(), L"1 = Enable BOD Reset when VDD drops below VBOD");
	m_CWDTEN.InsertString(m_CWDTEN.GetCount(), L"0 = Enable Watchdog Timer after all resets");
	m_CWDTEN.InsertString(m_CWDTEN.GetCount(), L"1 = Disable Watchdog Timer after all resets");
	m_CKF.InsertString(m_CKF.GetCount(), L"0 = Disable Clock Filter");
	m_CKF.InsertString(m_CKF.GetCount(), L"1 = Enable Clock Filter");
	m_OSCFS.InsertString(m_OSCFS.GetCount(), L"0 = 11.0592MHz");
	m_OSCFS.InsertString(m_OSCFS.GetCount(), L"1 = 22.1184MHz");
	m_FOSC.InsertString(m_FOSC.GetCount(), L"00 = External Crystal,4MHz~24MHz");
	m_FOSC.InsertString(m_FOSC.GetCount(), L"10 = Reserved");
	m_FOSC.InsertString(m_FOSC.GetCount(), L"11 = Internal RC oscillator");
	m_RSTDBE.InsertString(m_RSTDBE.GetCount(), L"0 = de-bounce time depends on RSTDBS");
	m_RSTDBE.InsertString(m_RSTDBE.GetCount(), L"1 = de-bounce time is default value(8 Fsys clock)");
	m_RSTDBS.InsertString(m_RSTDBS.GetCount(), L"0 = de-bounce time is 32 Fsys clock");
	m_RSTDBS.InsertString(m_RSTDBS.GetCount(), L"1 = de-bounce time is 16 Fsys clock");
	m_DIV2.InsertString(m_DIV2.GetCount(), L"0 = Fsys is equal to Fosc/2");
	m_DIV2.InsertString(m_DIV2.GetCount(), L"1 = Fsys is equal to Fosc");
	m_addr.EnableWindow(FALSE);
	m_CBS.SetCurSel(1);
	m_LOCK.SetCurSel(1);
	m_DFEN.SetCurSel(1);
	m_CBODEN.SetCurSel(1);
	m_CBOV.SetCurSel(1);
	m_CBORST.SetCurSel(1);
	m_CWDTEN.SetCurSel(1);
	m_CKF.SetCurSel(1);
	m_OSCFS.SetCurSel(1);
	m_FOSC.SetCurSel(2);
	m_RSTDBE.SetCurSel(1);
	m_RSTDBS.SetCurSel(1);
	m_DIV2.SetCurSel(1);
	return TRUE;

}

BOOL CDlg_N79E7X::OnSetActive()
{
	CString Cfg0;
	CString Cfg1;
	CString Cfg2;
	CString Cfg3;
	CString Cfg4;
	if (*m_pDataLen != NULL)
	{
		memcpy((void*)&Val_Cfg0, m_RegBuff, sizeof(Val_Cfg0));
		memcpy((void*)&Val_Cfg1, m_RegBuff + sizeof(Val_Cfg0), sizeof(Val_Cfg1));
		memcpy((void*)&Val_Cfg2, m_RegBuff + sizeof(Val_Cfg0)+sizeof(Val_Cfg1), sizeof(Val_Cfg2));
		memcpy((void*)&Val_Cfg3, m_RegBuff + sizeof(Val_Cfg0)+sizeof(Val_Cfg1)+sizeof(Val_Cfg2), sizeof(Val_Cfg3));
		memcpy((void*)&Val_Cfg4, m_RegBuff + sizeof(Val_Cfg0)+sizeof(Val_Cfg1)+sizeof(Val_Cfg2)+sizeof(Val_Cfg3), sizeof(Val_Cfg4));
		//CFG0
		Cfg0.Format(_T("%02X"), Val_Cfg0);
		m_Cfg0.SetWindowTextW(Cfg0);
		//CFG1
		Cfg1.Format(_T("%02X"), Val_Cfg1);
		m_Cfg1.SetWindowTextW(Cfg1);
		m_addr.SetWindowTextW(Cfg1);
		//CFG2
		Cfg2.Format(_T("%02X"), Val_Cfg2);
		m_Cfg2.SetWindowTextW(Cfg2);
		//CFG3
		Cfg3.Format(_T("%02X"), Val_Cfg3);
		m_Cfg3.SetWindowTextW(Cfg3);
		//CFG4
		Cfg4.Format(_T("%02X"), Val_Cfg4);
		m_Cfg4.SetWindowTextW(Cfg4);

		//Boot Select
		if ((Val_Cfg0 & 0x80) == 0x80)
			m_CBS.SetCurSel(1);
		else if ((Val_Cfg0 & 0x80) == 0x00)
			m_CBS.SetCurSel(0);
		//Lock
		if ((Val_Cfg0 & 0x02) == 0x02)
			m_LOCK.SetCurSel(1);
		else if ((Val_Cfg0 & 0x02) == 0x00)
			m_LOCK.SetCurSel(0);
		//DFEN
		if ((Val_Cfg0 & 0x01) == 0x01)
		{
			m_DFEN.SetCurSel(1);
			m_addr.EnableWindow(FALSE);
		}
		else if ((Val_Cfg0 & 0x01) == 0x00)
		{
			m_DFEN.SetCurSel(0);
			m_addr.EnableWindow(TRUE);
		}
		//BOD Detect
		if ((Val_Cfg2 & 0x80) == 0x80)
			m_CBODEN.SetCurSel(1);
		else if ((Val_Cfg2 & 0x80) == 0x00)
			m_CBODEN.SetCurSel(0);
		//BOD Voltage
		if ((Val_Cfg2 & 0x40) == 0x40)
			m_CBOV.SetCurSel(1);
		else if ((Val_Cfg2 & 0x40) == 0x00)
			m_CBOV.SetCurSel(0);
		//BOD Reset
		if ((Val_Cfg2 & 0x10) == 0x10)
			m_CBORST.SetCurSel(1);
		else if ((Val_Cfg2 & 0x10) == 0x00)
			m_CBORST.SetCurSel(0);
		//WatchDog
		if ((Val_Cfg3 & 0x80) == 0x80)
			m_CWDTEN.SetCurSel(1);
		else if ((Val_Cfg3 & 0x80) == 0x00)
			m_CWDTEN.SetCurSel(0);
		//CLK Filter
		if ((Val_Cfg3 & 0x10) == 0x10)
			m_CKF.SetCurSel(1);
		else if ((Val_Cfg3 & 0x10) == 0x00)
			m_CKF.SetCurSel(0);
		//Inernal RC
		if ((Val_Cfg3 & 0x08) == 0x08)
			m_OSCFS.SetCurSel(1);
		else if ((Val_Cfg3 & 0x08) == 0x00)
			m_OSCFS.SetCurSel(0);
		//CLK Source
		if ((Val_Cfg3 & 0x03) == 0x03)
			m_FOSC.SetCurSel(2);
		else if ((Val_Cfg3 & 0x03) == 0x02)
			m_FOSC.SetCurSel(1);
		else if ((Val_Cfg3 & 0x03) == 0x00)
			m_FOSC.SetCurSel(0);
		//RSTDBE
		if ((Val_Cfg4 & 0x80) == 0x80)
			m_RSTDBE.SetCurSel(1);
		else if ((Val_Cfg4 & 0x80) == 0x00)
			m_RSTDBE.SetCurSel(0);
		//RSTDBS
		if ((Val_Cfg4 & 0x40) == 0x40)
			m_RSTDBS.SetCurSel(1);
		else if ((Val_Cfg4 & 0x40) == 0x00)
			m_RSTDBS.SetCurSel(0);
		//DIV2
		if ((Val_Cfg4 & 0x01) == 0x01)
			m_DIV2.SetCurSel(1);
		else if ((Val_Cfg4 & 0x01) == 0x00)
			m_DIV2.SetCurSel(0);

	}
	return CMFCPropertyPage::OnSetActive();
}

void CDlg_N79E7X::OnCbnSelchangeCombo()
{
	CString cfg0;
	CString cfg1;
	CString cfg2;
	CString cfg3;
	CString cfg4;
	Val_Cfg0 = 0xFF;
	Val_Cfg1 = 0xFF;
	Val_Cfg2 = 0xFF;
	Val_Cfg3 = 0xFF;
	Val_Cfg4 = 0xFF;
	//Boot Select
	Val_Cfg0 &= 0x7F;
	if (m_CBS.GetCurSel() == 1)
		Val_Cfg0 |= 0x80;
	else if (m_CBS.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	cfg0.Format(_T("%02X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(cfg0);
	//Code Protect
	Val_Cfg0 &= 0xFD;
	if (m_LOCK.GetCurSel() == 1)
		Val_Cfg0 |= 0x02;
	else if (m_LOCK.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	cfg0.Format(_T("%02X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(cfg0);
	//Data Flash enable
	Val_Cfg0 &= 0xFE;
	if (m_DFEN.GetCurSel() == 1)
	{
		Val_Cfg0 |= 0x01;
		Val_Cfg1 = 0xFF;
		cfg1.Format(_T("%02X"), Val_Cfg1);
		m_addr.EnableWindow(FALSE);
		m_addr.SetWindowTextW(cfg1);
		m_Cfg1.SetWindowTextW(cfg1);
	}
	else if (m_DFEN.GetCurSel() == 0)
	{
		Val_Cfg0 |= 0x00;
		m_addr.EnableWindow(TRUE);
	}
	cfg0.Format(_T("%02X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(cfg0);
	//BOD Detect
	Val_Cfg2 &= 0x7F;
	if (m_CBODEN.GetCurSel() == 1)
		Val_Cfg2 |= 0x80;
	else if (m_CBODEN.GetCurSel() == 0)
		Val_Cfg2 |= 0x00;
	cfg2.Format(_T("%02X"), Val_Cfg2);
	m_Cfg2.SetWindowTextW(cfg2);
	//BOD Voltage
	Val_Cfg2 &= 0xBF;
	if (m_CBOV.GetCurSel() == 1)
		Val_Cfg2 |= 0x40;
	else if (m_CBOV.GetCurSel() == 0)
		Val_Cfg2 |= 0x00;
	cfg2.Format(_T("%02X"), Val_Cfg2);
	m_Cfg2.SetWindowTextW(cfg2);
	//BOD Reset
	Val_Cfg2 &= 0xEF;
	if (m_CBORST.GetCurSel() == 1)
		Val_Cfg2 |= 0x10;
	else if (m_CBORST.GetCurSel() == 0)
		Val_Cfg2 |= 0x00;
	cfg2.Format(_T("%02X"), Val_Cfg2);
	m_Cfg2.SetWindowTextW(cfg2);
	//WatchDog
	Val_Cfg3 &= 0x7F;
	if (m_CWDTEN.GetCurSel() == 1)
		Val_Cfg3 |= 0x80;
	else if (m_CWDTEN.GetCurSel() == 0)
		Val_Cfg3 |= 0x00;
	cfg3.Format(_T("%02X"), Val_Cfg3);
	m_Cfg3.SetWindowTextW(cfg3);
	//CLK Filter
	Val_Cfg3 &= 0xEF;
	if (m_CKF.GetCurSel() == 1)
		Val_Cfg3 |= 0x10;
	else if (m_CKF.GetCurSel() == 0)
		Val_Cfg3 |= 0x00;
	cfg3.Format(_T("%02X"), Val_Cfg3);
	m_Cfg3.SetWindowTextW(cfg3);
	//Internal RC
	Val_Cfg3 &= 0xF7;
	if (m_OSCFS.GetCurSel() == 1)
		Val_Cfg3 |= 0x08;
	else if (m_OSCFS.GetCurSel() == 0)
		Val_Cfg3 |= 0x00;
	cfg3.Format(_T("%02X"), Val_Cfg3);
	m_Cfg3.SetWindowTextW(cfg3);
	//CLK Source
	Val_Cfg3 &= 0xFC;
	if (m_FOSC.GetCurSel() == 2)
		Val_Cfg3 |= 0x03;
	else if (m_FOSC.GetCurSel() == 1)
		Val_Cfg3 |= 0x02;
	else if (m_FOSC.GetCurSel() == 0)
		Val_Cfg3 |= 0x00;
	cfg3.Format(_T("%02X"), Val_Cfg3);
	m_Cfg3.SetWindowTextW(cfg3);
	//RSTDBE
	Val_Cfg4 &= 0x7F;
	if (m_RSTDBE.GetCurSel() == 1)
		Val_Cfg4 |= 0x80;
	else if (m_RSTDBE.GetCurSel() == 0)
		Val_Cfg4 |= 0x00;
	cfg4.Format(_T("%02X"), Val_Cfg4);
	m_Cfg4.SetWindowTextW(cfg4);
	//RSTDBS
	Val_Cfg4 &= 0xBF;
	if (m_RSTDBS.GetCurSel() == 1)
		Val_Cfg4 |= 0x40;
	else if (m_RSTDBS.GetCurSel() == 0)
		Val_Cfg4 |= 0x00;
	cfg4.Format(_T("%02X"), Val_Cfg4);
	m_Cfg4.SetWindowTextW(cfg4);
	//DIV2
	Val_Cfg4 &= 0xFE;
	if (m_DIV2.GetCurSel() == 1)
		Val_Cfg4 |= 0x01;
	else if (m_DIV2.GetCurSel() == 0)
		Val_Cfg4 |= 0x00;
	cfg4.Format(_T("%02X"), Val_Cfg4);
	m_Cfg4.SetWindowTextW(cfg4);

}

void CDlg_N79E7X::OnOK()
{
	CString temp;
	char fixint[3] = { 0x00, 0x00, 0x00 };

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

	m_Cfg4.GetWindowTextW(temp);
	Val_Cfg4 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff + sizeof(Val_Cfg0)+sizeof(Val_Cfg1)+sizeof(Val_Cfg2)+sizeof(Val_Cfg3), &Val_Cfg4, sizeof(Val_Cfg4));

	memcpy(m_RegBuff + sizeof(Val_Cfg0)+sizeof(Val_Cfg1)+sizeof(Val_Cfg2)+sizeof(Val_Cfg3)+sizeof(Val_Cfg4), &fixint, sizeof(fixint));


	*m_pDataLen = sizeof(Val_Cfg0)+sizeof(Val_Cfg1)+sizeof(Val_Cfg2)+sizeof(Val_Cfg3)+sizeof(Val_Cfg4)+sizeof(fixint);
	CMFCPropertyPage::OnOK();


}

LRESULT CDlg_N79E7X::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();
}


LRESULT CDlg_N79E7X::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}


void CDlg_N79E7X::OnEnChangeAddr()
{
	CString Addr;
	if (m_DFEN.GetCurSel() == 0)
	{
		m_addr.GetWindowTextW(Addr);
		m_Cfg1.SetWindowTextW(Addr);
		Val_Cfg1 = numeric_conversion::hexstring_to_size_t(Addr.GetString());
	}
}
