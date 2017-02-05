// Dlg_SAMG5X.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_SAMG5X.h"
#include "afxdialogex.h"
#include "numeric_conversion.h"
#include "../common/prog_info.h"

// Dlg_SAMG5X dialog

IMPLEMENT_DYNAMIC(CDlg_SAMG5X, CMFCPropertyPage)

CDlg_SAMG5X::CDlg_SAMG5X(struct chip_info_c *ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_SAMG5X::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_SAMG5X::~CDlg_SAMG5X()
{
}

void CDlg_SAMG5X::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_GPNVM);
	DDX_Control(pDX, IDC_COMBO1, m_GPNVMb1);
	DDX_Control(pDX, IDC_COMBO2, m_GPNVMb0);
	DDX_Control(pDX, IDC_EDIT2, m_LW0);
	DDX_Control(pDX, IDC_EDIT3, m_LW1);
	DDX_Control(pDX, IDC_CHECK1, option_enable);
}


BEGIN_MESSAGE_MAP(CDlg_SAMG5X, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_SAMG5X::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_SAMG5X::OnCbnSelchangeCombo)
	ON_BN_CLICKED(IDC_CHECK1, &CDlg_SAMG5X::OnBnClickedEnable)
	ON_EN_CHANGE(IDC_EDIT2, &CDlg_SAMG5X::OnEnChangeLockword_0)
	ON_EN_CHANGE(IDC_EDIT3, &CDlg_SAMG5X::OnEnChangeLockword_1)
END_MESSAGE_MAP()


// Dlg_SAMG5X message handlers
BOOL CDlg_SAMG5X::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	GPNVM = 0x00;
	LockWord_0 = 0x00;
	LockWord_1 = 0x00;
	m_GPNVM.SetWindowTextW(L"00");
	m_LW0.SetWindowTextW(L"00000000");
	m_LW1.SetWindowTextW(L"00000000");
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	Encheck->SetCheck(0);

	m_GPNVMb1.InsertString(m_GPNVMb1.GetCount(), L"0 = Boot from the ROM");
	m_GPNVMb1.InsertString(m_GPNVMb1.GetCount(), L"1 = Boot from the Flash");
	m_GPNVMb0.InsertString(m_GPNVMb0.GetCount(), L"0 = Security Disabled");
	m_GPNVMb0.InsertString(m_GPNVMb0.GetCount(), L"1 = Security Enabled");

	m_GPNVMb1.SetCurSel(0);
	m_GPNVMb0.SetCurSel(0);

	m_GPNVMb1.EnableWindow(FALSE);
	m_GPNVMb0.EnableWindow(FALSE);
	m_GPNVM.EnableWindow(FALSE);
	m_LW0.EnableWindow(FALSE);
	m_LW1.EnableWindow(FALSE);

	return TRUE;
}

BOOL CDlg_SAMG5X::OnSetActive()
{
	if (*m_pDataLen != NULL)
	{
		CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
		Encheck->SetCheck(1);
		CString GPNVM_str, LW0_str, LW1_str;
		m_GPNVMb0.EnableWindow(TRUE);
		m_GPNVMb1.EnableWindow(TRUE);
		m_GPNVM.EnableWindow(TRUE);
		m_LW0.EnableWindow(TRUE);
		m_LW1.EnableWindow(TRUE);
		memcpy((void*)&GPNVM, m_RegBuff, sizeof(GPNVM));
		GPNVM_str.Format(_T("%02X"), GPNVM);
		m_GPNVM.SetWindowTextW(GPNVM_str);

		memcpy((void*)&LockWord_0, m_RegBuff + sizeof(GPNVM), sizeof(LockWord_0));
		LW0_str.Format(_T("%08X"), LockWord_0);
		m_LW0.SetWindowTextW(LW0_str);
		
		memcpy((void*)&LockWord_1, m_RegBuff + sizeof(GPNVM)+sizeof(LockWord_0), sizeof(LockWord_1));
		LW1_str.Format(_T("%08X"), LockWord_1);
		m_LW1.SetWindowTextW(LW1_str);

		//Security bit
		if ((GPNVM & 0x01) == 0x01)
			m_GPNVMb0.SetCurSel(1);
		else if ((GPNVM & 0x01) == 0x00)
			m_GPNVMb0.SetCurSel(0);
		//Boot Selection
		if ((GPNVM & 0x02) == 0x02)
			m_GPNVMb1.SetCurSel(1);
		else if ((GPNVM & 0x02) == 0x00)
			m_GPNVMb1.SetCurSel(0);

	}
	else if (*m_pDataLen == NULL)
	{
		CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
		Encheck->SetCheck(0);
		m_GPNVMb0.EnableWindow(FALSE);
		m_GPNVMb1.EnableWindow(FALSE);
		m_GPNVM.EnableWindow(FALSE);
		m_LW0.EnableWindow(FALSE);
		m_LW1.EnableWindow(FALSE);
	}
	return CMFCPropertyPage::OnSetActive();
}
void CDlg_SAMG5X::OnCbnSelchangeCombo()
{
	CString GPNVM_str;
	GPNVM = 0x00;
	//Boot Selection bit
	GPNVM & 0xFFFFFFFD;
	if (m_GPNVMb1.GetCurSel() == 1)
		GPNVM |= 0x02;
	else if (m_GPNVMb1.GetCurSel() == 0)
		GPNVM |= 0x00;
	GPNVM_str.Format(_T("%02X"), GPNVM);
	m_GPNVM.SetWindowTextW(GPNVM_str);
	//Security bit
	GPNVM & 0xFFFFFFFE;
	if (m_GPNVMb0.GetCurSel() == 1)
		GPNVM |= 0x01;
	else if (m_GPNVMb0.GetCurSel() == 0)
		GPNVM |= 0x00;
	GPNVM_str.Format(_T("%02X"), GPNVM);
	m_GPNVM.SetWindowTextW(GPNVM_str);
}
void CDlg_SAMG5X::OnOK()
{
	CString temp_str;
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	int state = Encheck->GetCheck();
	if (state == 0x1)
	{
		m_GPNVM.GetWindowTextW(temp_str);
		GPNVM = numeric_conversion::hexstring_to_size_t(temp_str.GetString());
		memcpy(m_RegBuff, &GPNVM, sizeof(GPNVM));

		m_LW0.GetWindowTextW(temp_str);
		LockWord_0 = numeric_conversion::hexstring_to_size_t(temp_str.GetString());
		memcpy(m_RegBuff + sizeof(GPNVM), &LockWord_0, sizeof(LockWord_0));

		m_LW1.GetWindowTextW(temp_str);
		LockWord_1 = numeric_conversion::hexstring_to_size_t(temp_str.GetString());
		memcpy(m_RegBuff + sizeof(GPNVM)+sizeof(LockWord_0), &LockWord_1, sizeof(LockWord_1));

		*m_pDataLen = sizeof(GPNVM)+sizeof(LockWord_0)+sizeof(LockWord_1);
	}
	else if (state == 0x0)
		*m_pDataLen = 0;
	CMFCPropertyPage::OnOK();
}

LRESULT CDlg_SAMG5X::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();

}
LRESULT CDlg_SAMG5X::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}

void CDlg_SAMG5X::OnEnChangeLockword_0()
{
	CString LW0;
	m_LW0.GetWindowTextW(LW0);
	LockWord_0 = numeric_conversion::hexstring_to_size_t(LW0.GetString());
}

void CDlg_SAMG5X::OnEnChangeLockword_1()
{
	CString LW1;
	m_LW1.GetWindowTextW(LW1);
	LockWord_1 = numeric_conversion::hexstring_to_size_t(LW1.GetString());
}

void CDlg_SAMG5X::OnBnClickedEnable()
{
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	int state = Encheck->GetCheck();
	if (state == 0x01)
	{
		m_GPNVMb0.EnableWindow(TRUE);
		m_GPNVMb1.EnableWindow(TRUE);
		m_GPNVM.EnableWindow(TRUE);
		m_LW0.EnableWindow(TRUE);
		m_LW1.EnableWindow(TRUE);
	}
	else if (state == 0x0)
	{
		m_GPNVMb0.EnableWindow(FALSE);
		m_GPNVMb1.EnableWindow(FALSE);
		m_GPNVM.EnableWindow(FALSE);
		m_LW0.EnableWindow(FALSE);
		m_LW1.EnableWindow(FALSE);
	}
}