// Dlg_Mini51D.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_Mini51D.h"
#include "afxdialogex.h"
#include "../common/prog_info.h"
#include "numeric_conversion.h"

// Dlg_Mini51D dialog

IMPLEMENT_DYNAMIC(CDlg_Mini51D, CMFCPropertyPage)

CDlg_Mini51D::CDlg_Mini51D(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_Mini51D::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_Mini51D::~CDlg_Mini51D()
{
}

void CDlg_Mini51D::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Cfg0);
	DDX_Control(pDX, IDC_EDIT2, m_Cfg1);
	DDX_Control(pDX, IDC_COMBO1, m_CBOVEXT);
	DDX_Control(pDX, IDC_COMBO2, m_CBOV);
	DDX_Control(pDX, IDC_COMBO3, m_CBORST);
	DDX_Control(pDX, IDC_COMBO4, m_CIOINI);
	DDX_Control(pDX, IDC_COMBO5, m_CBS);
	DDX_Control(pDX, IDC_COMBO6, m_Lock);
	DDX_Control(pDX, IDC_COMBO7, m_DFEN);
	DDX_Control(pDX, IDC_EDIT3, m_Addr);
}


BEGIN_MESSAGE_MAP(CDlg_Mini51D, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_Mini51D::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_Mini51D::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_Mini51D::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_Mini51D::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_Mini51D::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDlg_Mini51D::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDlg_Mini51D::OnCbnSelchangeCombo)
	ON_EN_CHANGE(IDC_EDIT3, &CDlg_Mini51D::OnEnChangeEdit3)
END_MESSAGE_MAP()


// Dlg_Mini51D message handlers
BOOL CDlg_Mini51D::OnInitDialog()
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
	m_CBOVEXT.InsertString(m_CBOVEXT.GetCount(), L"0 = Brown-out Voltage includes Enabled");
	m_CBOVEXT.InsertString(m_CBOVEXT.GetCount(), L"1 = Brown-out Voltage includes Disabled");
	m_CBOV.InsertString(m_CBOV.GetCount(), L"00 = 2.2V");
	m_CBOV.InsertString(m_CBOV.GetCount(), L"01 = 2.7V");
	m_CBOV.InsertString(m_CBOV.GetCount(), L"10 = 3.7V");
	m_CBOV.InsertString(m_CBOV.GetCount(), L"11 = 4.4V(Disable BOD function)");
	m_CBORST.InsertString(m_CBORST.GetCount(), L"0 = Brown-out Reset Enabled");
	m_CBORST.InsertString(m_CBORST.GetCount(), L"1 = Brown-out Reset Disabled");
	m_CIOINI.InsertString(m_CIOINI.GetCount(), L"0 = Quasi bi-direction mode");
	m_CIOINI.InsertString(m_CIOINI.GetCount(), L"1 = Input Tri-state mode");
	m_CBS.InsertString(m_CBS.GetCount(), L"00 = Boot from LDROM with IAP function");
	m_CBS.InsertString(m_CBS.GetCount(), L"01 = Boot from LDROM");
	m_CBS.InsertString(m_CBS.GetCount(), L"10 = Boot from APROM with IAP function");
	m_CBS.InsertString(m_CBS.GetCount(), L"11 = Boot from APROM");
	m_Lock.InsertString(m_Lock.GetCount(), L"0 = Data Flash Locked");
	m_Lock.InsertString(m_Lock.GetCount(), L"1 = Data Flash unLocked");
	m_DFEN.InsertString(m_DFEN.GetCount(), L"0 = Data Flash Enabled");
	m_DFEN.InsertString(m_DFEN.GetCount(), L"1 = Data Flash Disabled");
	m_CBOVEXT.SetCurSel(1);
	m_CBOV.SetCurSel(3);
	m_CBORST.SetCurSel(1);
	m_CIOINI.SetCurSel(1);
	m_CBS.SetCurSel(3);
	m_Lock.SetCurSel(1);
	m_DFEN.SetCurSel(1);
	return TRUE;
}

BOOL CDlg_Mini51D::OnSetActive()
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
		//Brown-out Voltage Selection Extension
		if ((Val_Cfg0 & 0x800000) == 0x800000)
			m_CBOVEXT.SetCurSel(1);
		else if ((Val_Cfg0 & 0x800000) == 0x00)
			m_CBOVEXT.SetCurSel(0);
		//Brown-out Reset Enable
		if ((Val_Cfg0 & 0x600000) == 0x600000)
			m_CBOV.SetCurSel(3);
		else if ((Val_Cfg0 & 0x600000) == 0x400000)
			m_CBOV.SetCurSel(2);
		else if ((Val_Cfg0 & 0x600000) == 0x200000)
			m_CBOV.SetCurSel(1);
		else if ((Val_Cfg0 & 0x600000) == 0x00)
			m_CBOV.SetCurSel(0);
		//IO Initial State Selection
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
		//Sercurity Lock
		if ((Val_Cfg0 & 0x02) == 0x02)
			m_Lock.SetCurSel(1);
		else if ((Val_Cfg0 & 0x02) == 0x00)
			m_Lock.SetCurSel(0);
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
void CDlg_Mini51D::OnCbnSelchangeCombo()
{
	CString strTemp;
	CString strTemp2;
	Val_Cfg0 = 0xFFFFFFFF;
	Val_Cfg1 = 0xFFFFFFFF;
	//Brown-out Voltage Selection Enable
	Val_Cfg0 &= 0xFF7FFFFF;
	if (m_CBOVEXT.GetCurSel() == 1)
		Val_Cfg0 |= 0x00800000;
	else if (m_CBOVEXT.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
	//Brown-out Voltage Selection
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
	//Brown-out Reset Enable Control
	Val_Cfg0 &= 0xFFEFFFFF;
	if (m_CBORST.GetCurSel() == 1)
		Val_Cfg0 |= 0x100000;
	else if (m_CBORST.GetCurSel() == 0)
		Val_Cfg0 |= 0x000000;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
	//IO Initial State Selection
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
	//Security Lock
	Val_Cfg0 &= 0xFFFFFFFD;
	if (m_Lock.GetCurSel() == 1)
		Val_Cfg0 |= 0x02;
	else if (m_Lock.GetCurSel() == 0)
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

void CDlg_Mini51D::OnOK()
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

LRESULT CDlg_Mini51D::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();
}


LRESULT CDlg_Mini51D::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}


void CDlg_Mini51D::OnEnChangeEdit3()
{
	CString Addr;
	if (m_DFEN.GetCurSel() == 0)
	{
		m_Addr.GetWindowTextW(Addr);
		m_Cfg1.SetWindowTextW(Addr);
		Val_Cfg1 = numeric_conversion::hexstring_to_size_t(Addr.GetString());
	}
}
