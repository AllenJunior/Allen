// Dlg_A8XXX.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_A8XXX.h"
#include "afxdialogex.h"
#include "numeric_conversion.h"
#include "../common/prog_info.h"

// Dlg_A8XXX dialog

IMPLEMENT_DYNAMIC(CDlg_A8XXX, CMFCPropertyPage)

CDlg_A8XXX::CDlg_A8XXX(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_A8XXX::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_A8XXX::~CDlg_A8XXX()
{
}

void CDlg_A8XXX::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, option_enable);
	DDX_Control(pDX, IDC_EDIT1, m_Bank);
	DDX_Control(pDX, IDC_EDIT2, m_Addr);
	DDX_Control(pDX, IDC_EDIT3, m_Checksum);
	DDX_Control(pDX, IDC_BUTTON1, m_Refresh);
}


BEGIN_MESSAGE_MAP(CDlg_A8XXX, CMFCPropertyPage)
	ON_EN_CHANGE(IDC_EDIT1, &CDlg_A8XXX::OnEnChangeBank)
	ON_EN_CHANGE(IDC_EDIT2, &CDlg_A8XXX::OnEnChangeAddr)
	ON_EN_CHANGE(IDC_EDIT3, &CDlg_A8XXX::OnEnChangeChecksum)
	ON_BN_CLICKED(IDC_CHECK1, &CDlg_A8XXX::OnBnClickedCheckEnable)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlg_A8XXX::OnBnClickedRefresh)
END_MESSAGE_MAP()


// Dlg_A8XXX message handlers
BOOL CDlg_A8XXX::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	checkbox = 0x00;
	Bank = 0x00;
	Addr = 0x00;
	Checksum = 0x0000;

	m_Bank.SetWindowTextW(L"0");
	m_Addr.SetWindowTextW(L"0000");
	m_Checksum.SetWindowTextW(L"0000");

	option_enable.SetCheck(FALSE);
	m_Bank.EnableWindow(FALSE);
	m_Addr.EnableWindow(FALSE);
	m_Checksum.EnableWindow(FALSE);
	m_Refresh.EnableWindow(FALSE);

	return TRUE;
}
BOOL CDlg_A8XXX::OnSetActive()
{
	CString bank_str, Addr_str, Checksum_str;
	if (*m_pDataLen != NULL)
	{
		//Checkbox
		memcpy((void*)&checkbox, m_RegBuff, sizeof(checkbox));
		option_enable.SetCheck(checkbox);
		//Bank
		memcpy((void*)&Bank, m_RegBuff + sizeof(checkbox), sizeof(Bank));
		bank_str.Format(_T("%01X"), Bank);
		m_Bank.SetWindowTextW(bank_str);
		//Addr
		memcpy((void*)&Addr, m_RegBuff + sizeof(checkbox)+sizeof(Bank), sizeof(Addr));
		Addr_str.Format(_T("%04X"), Addr);
		m_Addr.SetWindowTextW(Addr_str);
		//Checksum
		memcpy((void*)&Checksum, m_RegBuff + sizeof(checkbox)+sizeof(Bank)+sizeof(Addr), sizeof(Checksum));
		Checksum_str.Format(_T("%04X"), Checksum);
		m_Checksum.SetWindowTextW(Checksum_str);
		//Dialog Item Enable
		m_Bank.EnableWindow(TRUE);
		m_Addr.EnableWindow(TRUE);
		m_Checksum.EnableWindow(TRUE);
		m_Refresh.EnableWindow(TRUE);
	}
	else if (*m_pDataLen == NULL)
	{
		option_enable.SetCheck(FALSE);
		m_Bank.EnableWindow(FALSE);
		m_Addr.EnableWindow(FALSE);
		m_Checksum.EnableWindow(FALSE);
		m_Refresh.EnableWindow(FALSE);
	}
	return CMFCPropertyPage::OnSetActive();
}
void CDlg_A8XXX::OnOK()
{
	checkbox = option_enable.GetCheck();
	if (checkbox == 1)
	{
		memcpy(m_RegBuff, &checkbox, sizeof(checkbox));
		memcpy(m_RegBuff + sizeof(checkbox), &Bank, sizeof(Bank));
		memcpy(m_RegBuff + sizeof(checkbox)+sizeof(Bank), &Addr, sizeof(Addr));
		memcpy(m_RegBuff + sizeof(checkbox)+sizeof(Bank)+sizeof(Addr), &Checksum, sizeof(Checksum));
		short int fix = 0x00;
		memcpy(m_RegBuff + sizeof(checkbox)+sizeof(Bank)+sizeof(Addr)+sizeof(Checksum), &fix, sizeof(fix));
		*m_pDataLen = sizeof(checkbox)+sizeof(Bank)+sizeof(Addr)+sizeof(Checksum)+sizeof(fix);
	}
	else if (checkbox == 0)
	{
		*m_pDataLen = 0x00;
	}
	CMFCPropertyPage::OnOK();
}
LRESULT CDlg_A8XXX::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();
}
LRESULT CDlg_A8XXX::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}

void CDlg_A8XXX::OnEnChangeBank()
{
	CString bank_str;
	m_Bank.GetWindowTextW(bank_str);
	Bank = numeric_conversion::hexstring_to_size_t(bank_str.GetString());
	Bank &= 0x3;
}
void CDlg_A8XXX::OnEnChangeAddr()
{
	CString Addr_str;
	m_Addr.GetWindowTextW(Addr_str);
	Addr = numeric_conversion::hexstring_to_size_t(Addr_str.GetString());
	Addr &= 0xFFFF;
}

void CDlg_A8XXX::OnEnChangeChecksum()
{
	CString Checksum_str;
	m_Checksum.GetWindowTextW(Checksum_str);
	Checksum = numeric_conversion::hexstring_to_size_t(Checksum_str.GetString());
	Checksum &= 0xFFFF;
}

void CDlg_A8XXX::OnBnClickedCheckEnable()
{
	checkbox = option_enable.GetCheck();
	if (checkbox == 1)
	{
		m_Bank.EnableWindow(TRUE);
		m_Addr.EnableWindow(TRUE);
		m_Checksum.EnableWindow(TRUE);
		m_Refresh.EnableWindow(TRUE);
	}
	else if (checkbox == 0)
	{
		m_Bank.EnableWindow(FALSE);
		m_Addr.EnableWindow(FALSE);
		m_Checksum.EnableWindow(FALSE);
		m_Refresh.EnableWindow(FALSE);
	}
}

void CDlg_A8XXX::OnBnClickedRefresh()
{
	CString bank_str, Addr_str;
	bank_str.Format(_T("%01X"), Bank);
	m_Bank.SetWindowTextW(bank_str);
	if (Bank == 0)
	{
		Addr &= 0x7FFF;
		Addr_str.Format(_T("%04X"), Addr);
		m_Addr.SetWindowTextW(Addr_str);
	}
	else if ((Bank == 1) || (Bank == 2) || (Bank == 3))
	{
		Addr |= 0x8000;
		Addr_str.Format(_T("%04X"), Addr);
		m_Addr.SetWindowTextW(Addr_str);
	}
	
}
