// Dlg_Nano1X0AN.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_Nano1X0AN.h"
#include "afxdialogex.h"
#include "../common/prog_info.h"
#include "numeric_conversion.h"

// Dlg_Nano1X0AN dialog

IMPLEMENT_DYNAMIC(CDlg_Nano1X0AN, CMFCPropertyPage)

CDlg_Nano1X0AN::CDlg_Nano1X0AN(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_Nano1X0AN::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}
CDlg_Nano1X0AN::~CDlg_Nano1X0AN()
{
}

void CDlg_Nano1X0AN::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Cfg0);
	DDX_Control(pDX, IDC_EDIT2, m_Cfg1);
	DDX_Control(pDX, IDC_COMBO1, m_CBS);
	DDX_Control(pDX, IDC_COMBO2, m_CFOSC);
	DDX_Control(pDX, IDC_COMBO3, m_CBORST);
	DDX_Control(pDX, IDC_COMBO4, m_Lock);
	DDX_Control(pDX, IDC_COMBO5, m_DFEN);
	DDX_Control(pDX, IDC_EDIT3, m_addr);
}


BEGIN_MESSAGE_MAP(CDlg_Nano1X0AN, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_Nano1X0AN::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_Nano1X0AN::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_Nano1X0AN::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_Nano1X0AN::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_Nano1X0AN::OnCbnSelchangeCombo)
	ON_EN_CHANGE(IDC_EDIT3, &CDlg_Nano1X0AN::OnEnChangeEdit3)
END_MESSAGE_MAP()


// Dlg_Nano1X0AN message handlers
BOOL CDlg_Nano1X0AN::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	Val_Cfg0 = 0xFFFFFFFF;
	Val_Cfg1 = 0xFFFFFFFF;
	m_Cfg0.SetWindowTextW(L"FFFFFFFF");
	m_Cfg1.SetWindowTextW(L"FFFFFFFF");
	m_Cfg0.EnableWindow(false);
	m_Cfg1.EnableWindow(false);
	m_CFOSC.InsertString(m_CFOSC.GetCount(), L"0 = External 12MHz crystal clock");
	m_CFOSC.InsertString(m_CFOSC.GetCount(), L"1 = Internal RC 12MHz oscillator clock");
	m_CBORST.InsertString(m_CBORST.GetCount(), L"00 = 1.7V");
	m_CBORST.InsertString(m_CBORST.GetCount(), L"01 = 2.0V");
	m_CBORST.InsertString(m_CBORST.GetCount(), L"10 = 2.5V");
	m_CBORST.InsertString(m_CBORST.GetCount(), L"11 = Disable");
	m_CBS.InsertString(m_CBS.GetCount(), L"0 = Boot from LDROM");
	m_CBS.InsertString(m_CBS.GetCount(), L"1 = Boot from APROM");
	m_Lock.InsertString(m_Lock.GetCount(), L"0 = Flash is Locked");
	m_Lock.InsertString(m_Lock.GetCount(), L"1 = Flash is not Locked");
	m_DFEN.InsertString(m_DFEN.GetCount(), L"0 = DataFlash Enabled");
	m_DFEN.InsertString(m_DFEN.GetCount(), L"1 = DataFlash Disabled");
	m_CFOSC.SetCurSel(1);
	m_CBORST.SetCurSel(3);
	m_CBS.SetCurSel(1);
	m_Lock.SetCurSel(1);
	m_DFEN.SetCurSel(1);
	GetDlgItem(IDC_EDIT3)->EnableWindow(false);
	return TRUE;
}

BOOL CDlg_Nano1X0AN::OnSetActive()
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
		//Clock Source
		if ((Val_Cfg0 & 0x04000000) == 0x04000000)
			m_CFOSC.SetCurSel(1);
		else if ((Val_Cfg0 & 0x04000000) == 0x00)
			m_CFOSC.SetCurSel(0);
		//Brown-out Reset
		if ((Val_Cfg0 & 0x180000) == 0x180000)
			m_CBORST.SetCurSel(3);
		else if ((Val_Cfg0 & 0x180000) == 0x100000)
			m_CBORST.SetCurSel(2);
		else if ((Val_Cfg0 & 0x180000) == 0x080000)
			m_CBORST.SetCurSel(1);
		else if ((Val_Cfg0 & 0x180000) == 0x00)
			m_CBORST.SetCurSel(0);
		//Boot Select
		if ((Val_Cfg0 & 0x80) == 0x80)
			m_CBS.SetCurSel(1);
		else if ((Val_Cfg0 & 0x80) == 0x00)
			m_CBS.SetCurSel(0);
		//Security Lock
		if ((Val_Cfg0 & 0x02) == 0x02)
			m_Lock.SetCurSel(1);
		else if ((Val_Cfg0 & 0x02) == 0x00)
			m_Lock.SetCurSel(0);
		//DataFlash
		if ((Val_Cfg0 & 0x01) == 0x01)
		{
			m_DFEN.SetCurSel(1);
			GetDlgItem(IDC_EDIT3)->EnableWindow(false);
		}
		else if ((Val_Cfg0 & 0x01) == 0x00)
		{
			m_DFEN.SetCurSel(0);
			GetDlgItem(IDC_EDIT3)->EnableWindow(true);
		}
	}
	return CMFCPropertyPage::OnSetActive();
}
void CDlg_Nano1X0AN::OnCbnSelchangeCombo()
{
	CString strTemp;
	CString strTemp2;
	Val_Cfg0 = 0xFFFFFFFF;
	Val_Cfg1 = 0xFFFFFFFF;

	//Clock Source
	Val_Cfg0 &= 0xFBFFFFFF;
	if (m_CFOSC.GetCurSel() == 1)
		Val_Cfg0 |= 0x04000000;
	else if (m_CFOSC.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
	//Brown out Reset
	Val_Cfg0 &= 0xFFE7FFFF;
	if (m_CBORST.GetCurSel() == 3)
		Val_Cfg0 |= 0x180000;
	else if (m_CBORST.GetCurSel() == 2)
		Val_Cfg0 |= 0x100000;
	else if (m_CBORST.GetCurSel() == 1)
		Val_Cfg0 |= 0x080000;
	else if (m_CBORST.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
	//Boot Select
	Val_Cfg0 &= 0xFFFFFF7F;
	if (m_CBS.GetCurSel() == 1)
		Val_Cfg0 |= 0x80;
	else if (m_CBS.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
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
		GetDlgItem(IDC_EDIT3)->SetWindowTextW(strTemp2);
		GetDlgItem(IDC_EDIT3)->EnableWindow(false);
	}
	else if (m_DFEN.GetCurSel() == 0)
	{
		Val_Cfg0 |= 0x00;
		GetDlgItem(IDC_EDIT3)->EnableWindow(true);
	}
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);

}

void CDlg_Nano1X0AN::OnOK()
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

LRESULT CDlg_Nano1X0AN::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();
}
LRESULT CDlg_Nano1X0AN::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}

void CDlg_Nano1X0AN::OnEnChangeEdit3()
{
	CString Addr;
	if (m_DFEN.GetCurSel() == 0)
	{
		GetDlgItem(IDC_EDIT3)->GetWindowText(Addr);
		m_Cfg1.SetWindowTextW(Addr);
		Val_Cfg1 = numeric_conversion::hexstring_to_size_t(Addr.GetString());
	}
	
}
