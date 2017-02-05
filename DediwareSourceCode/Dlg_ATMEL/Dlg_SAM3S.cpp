// Dlg_SAM3S.cpp : implementation file
//
#include "stdafx.h"
#include "Dlg_SAM3S.h"
#include "afxdialogex.h"
#include "numeric_conversion.h"
#include "../common/prog_info.h"

// Dlg_SAM3S dialog

IMPLEMENT_DYNAMIC(CDlg_SAM3S, CMFCPropertyPage)

CDlg_SAM3S::CDlg_SAM3S(struct chip_info_c *ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_SAM3S::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_SAM3S::~CDlg_SAM3S()
{
}

void CDlg_SAM3S::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, Option_enable);
	DDX_Control(pDX, IDC_COMBO1, m_GPNVMb1);
	DDX_Control(pDX, IDC_COMBO2, m_GPNVMb0);
	DDX_Control(pDX, IDC_EDIT1, m_Lockbits);
	DDX_Control(pDX, IDC_EDIT2, m_GPNVMbits);
}


BEGIN_MESSAGE_MAP(CDlg_SAM3S, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_SAM3S::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_SAM3S::OnCbnSelchangeCombo)
	ON_EN_CHANGE(IDC_EDIT1, &CDlg_SAM3S::OnEnChangeLockbits)
	ON_BN_CLICKED(IDC_CHECK1, &CDlg_SAM3S::OnBnClickedEnable)
END_MESSAGE_MAP()


// Dlg_SAM3S message handlers
BOOL CDlg_SAM3S::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	GPNVMbits = 0x00;
	Lockbits = 0x00;
	m_GPNVMbits.SetWindowTextW(L"02");
	m_Lockbits.SetWindowTextW(L"00");
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	Encheck->SetCheck(0);

	m_GPNVMb1.InsertString(m_GPNVMb1.GetCount(), L"0 = Boot from the ROM");
	m_GPNVMb1.InsertString(m_GPNVMb1.GetCount(), L"1 = Boot from the Flash");
	m_GPNVMb0.InsertString(m_GPNVMb0.GetCount(), L"0 = Security Disabled");
	m_GPNVMb0.InsertString(m_GPNVMb0.GetCount(), L"1 = Security Enabled");

	m_GPNVMb1.SetCurSel(1);
	m_GPNVMb0.SetCurSel(0);
	m_GPNVMb1.EnableWindow(FALSE);
	m_GPNVMb0.EnableWindow(FALSE);
	m_GPNVMbits.EnableWindow(FALSE);
	m_Lockbits.EnableWindow(FALSE);

	return TRUE;
}

BOOL CDlg_SAM3S::OnSetActive()
{
	if (*m_pDataLen != NULL)
	{
		CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
		Encheck->SetCheck(1);
		CString GPNVMbits_str;
		CString Lock_str;
		m_GPNVMb0.EnableWindow(TRUE);
		m_GPNVMb1.EnableWindow(TRUE);
		m_Lockbits.EnableWindow(TRUE);
		m_GPNVMbits.EnableWindow(FALSE);
		memcpy((void*)&GPNVMbits, m_RegBuff, sizeof(GPNVMbits));
		GPNVMbits_str.Format(_T("%02X"), GPNVMbits);
		m_GPNVMbits.SetWindowTextW(GPNVMbits_str);
		memcpy((void*)&Lockbits, m_RegBuff + sizeof(GPNVMbits), sizeof(Lockbits));
		Lock_str.Format(_T("%02X"), Lockbits);
		m_Lockbits.SetWindowTextW(Lock_str);
		//Security bit
		if ((GPNVMbits & 0x01) == 0x01)
			m_GPNVMb0.SetCurSel(1);
		else if ((GPNVMbits & 0x01) == 0x00)
			m_GPNVMb0.SetCurSel(0);
		//Boot Selection
		if ((GPNVMbits & 0x02) == 0x02)
			m_GPNVMb1.SetCurSel(1);
		else if ((GPNVMbits & 0x02) == 0x00)
		m_GPNVMb1.SetCurSel(0);
		//Lockbits
		
	}
	else if (*m_pDataLen == NULL)
	{
		CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
		Encheck->SetCheck(0);
		m_GPNVMb0.EnableWindow(FALSE);
		m_GPNVMb1.EnableWindow(FALSE);
		m_Lockbits.EnableWindow(FALSE);
		m_GPNVMbits.EnableWindow(FALSE);
	}
	return CMFCPropertyPage::OnSetActive();
}

void CDlg_SAM3S::OnCbnSelchangeCombo()
{
	CString GPNVMbits_str;
	GPNVMbits = 0x00;
	Lockbits = 0x00;
	//Boot Selection bit
	GPNVMbits & 0xFFFFFFFD;
	if (m_GPNVMb1.GetCurSel() == 1)
		GPNVMbits |= 0x02;
	else if (m_GPNVMb1.GetCurSel() == 0)
		GPNVMbits |= 0x00;
	GPNVMbits_str.Format(_T("%02X"), GPNVMbits);
	m_GPNVMbits.SetWindowTextW(GPNVMbits_str);
	//Security bit
	GPNVMbits & 0xFFFFFFFE;
	if (m_GPNVMb0.GetCurSel() == 1)
		GPNVMbits |= 0x01;
	else if (m_GPNVMb0.GetCurSel() == 0)
		GPNVMbits |= 0x00;
	GPNVMbits_str.Format(_T("%02X"), GPNVMbits);
	m_GPNVMbits.SetWindowTextW(GPNVMbits_str);
}

void CDlg_SAM3S::OnOK()
{
	CString temp_str;
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	int state = Encheck->GetCheck();
	if (state == 0x1)
	{
		m_GPNVMbits.GetWindowTextW(temp_str);
		GPNVMbits = numeric_conversion::hexstring_to_size_t(temp_str.GetString());
		memcpy(m_RegBuff, &GPNVMbits, sizeof(GPNVMbits));
		
		m_Lockbits.GetWindowTextW(temp_str);
		Lockbits = numeric_conversion::hexstring_to_size_t(temp_str.GetString());
		memcpy(m_RegBuff + sizeof(GPNVMbits), &Lockbits, sizeof(Lockbits));
		
		*m_pDataLen = sizeof(GPNVMbits)+sizeof(Lockbits);
	}
	else if (state == 0x0)
		*m_pDataLen = 0;

	CMFCPropertyPage::OnOK();
}


LRESULT CDlg_SAM3S::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();

}
LRESULT CDlg_SAM3S::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}

void CDlg_SAM3S::OnEnChangeLockbits()
{
	CString Lock_str;
	m_Lockbits.GetWindowTextW(Lock_str);
	Lockbits = numeric_conversion::hexstring_to_size_t(Lock_str.GetString());

}
void CDlg_SAM3S::OnBnClickedEnable()
{
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	int state = Encheck->GetCheck();
	if (state == 0x01)
	{
		m_GPNVMb0.EnableWindow(TRUE);
		m_GPNVMb1.EnableWindow(TRUE);
		m_Lockbits.EnableWindow(TRUE);
		m_GPNVMbits.EnableWindow(FALSE);
	}
	else if (state == 0x0)
	{
		m_GPNVMb0.EnableWindow(FALSE);
		m_GPNVMb1.EnableWindow(FALSE);
		m_Lockbits.EnableWindow(FALSE);
		m_GPNVMbits.EnableWindow(FALSE);
	}
}
