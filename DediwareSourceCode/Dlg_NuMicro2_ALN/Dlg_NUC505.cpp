// Dlg_NUC505.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_NUC505.h"
#include "afxdialogex.h"
#include "numeric_conversion.h"
#include "../common/prog_info.h"

// Dlg_NUC505 dialog

IMPLEMENT_DYNAMIC(CDlg_NUC505, CMFCPropertyPage)

CDlg_NUC505::CDlg_NUC505(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_NUC505::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_NUC505::~CDlg_NUC505()
{
}

void CDlg_NUC505::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, MTP_Enable);
	DDX_Control(pDX, IDC_EDIT1, m_MTPKey0);
	DDX_Control(pDX, IDC_EDIT2, m_MTPKey1);
	DDX_Control(pDX, IDC_EDIT3, m_MTPSignature);
	DDX_Control(pDX, IDC_EDIT4, m_MTPOffsetAddr);
	DDX_Control(pDX, IDC_EDIT5, m_MTPOption);
	DDX_Control(pDX, IDC_CHECK2, m_MTPLock);
}


BEGIN_MESSAGE_MAP(CDlg_NUC505, CMFCPropertyPage)
	ON_EN_CHANGE(IDC_EDIT1, &CDlg_NUC505::OnEnChangeMTPKey0)
	ON_EN_CHANGE(IDC_EDIT2, &CDlg_NUC505::OnEnChangeMTPKey1)
	ON_EN_CHANGE(IDC_EDIT3, &CDlg_NUC505::OnEnChangeMTPSignature)
	ON_EN_CHANGE(IDC_EDIT4, &CDlg_NUC505::OnEnChangeMTPoffsetAddr)
	ON_EN_CHANGE(IDC_EDIT5, &CDlg_NUC505::OnEnChangeMTPOption)
	ON_BN_CLICKED(IDC_CHECK1, &CDlg_NUC505::OnBnClickedMTPEnable)
	ON_BN_CLICKED(IDC_CHECK2, &CDlg_NUC505::OnBnClickedMTPLockEnable)
END_MESSAGE_MAP()


// Dlg_NUC505 message handlers
BOOL CDlg_NUC505::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	MTPKey0 = 0x00;
	MTPKey1 = 0x00;
	MTPSignature = 0x00;
	MTPOffsetAddr = 0x00;
	MTPOption = 0x00;
	MTPLock = 0x00;

	MTP_Enable.SetCheck(0);
	m_MTPKey0.SetWindowTextW(L"00000000");
	m_MTPKey1.SetWindowTextW(L"00000000");
	m_MTPSignature.SetWindowTextW(L"00000000");
	m_MTPOffsetAddr.SetWindowTextW(L"0000");
	m_MTPOption.SetWindowTextW(L"00");
	m_MTPLock.SetCheck(0);

	m_MTPKey0.EnableWindow(FALSE);
	m_MTPKey1.EnableWindow(FALSE);
	m_MTPSignature.EnableWindow(FALSE);
	m_MTPOffsetAddr.EnableWindow(FALSE);
	m_MTPOption.EnableWindow(FALSE);
	m_MTPLock.EnableWindow(FALSE);

	return TRUE;
}
BOOL CDlg_NUC505::OnSetActive()
{
	CString MTPKey0_str, MTPKey1_str, MTPSignature_str, MTPOffset_str, MTPOption_str, MTPLock_str;
	if (*m_pDataLen != NULL)
	{
		MTP_Enable.SetCheck(TRUE);
		m_MTPKey0.EnableWindow(TRUE);
		m_MTPKey1.EnableWindow(TRUE);
		m_MTPSignature.EnableWindow(TRUE);
		m_MTPOffsetAddr.EnableWindow(TRUE);
		m_MTPOption.EnableWindow(TRUE);
		m_MTPLock.EnableWindow(TRUE);
		//MTP Key0
		memcpy((void*)&MTPKey0, m_RegBuff, sizeof(MTPKey0));
		MTPKey0_str.Format(_T("%08X"), MTPKey0);
		m_MTPKey0.SetWindowTextW(MTPKey0_str);
		//MTP Key1
		memcpy((void*)&MTPKey1, m_RegBuff + sizeof(MTPKey0), sizeof(MTPKey1));
		MTPKey1_str.Format(_T("%08X"), MTPKey1);
		m_MTPKey1.SetWindowTextW(MTPKey1_str);
		//MTP Signature
		memcpy((void*)&MTPSignature, m_RegBuff + sizeof(MTPKey0)+sizeof(MTPKey1), sizeof(MTPSignature));
		MTPSignature_str.Format(_T("%08X"), MTPSignature);
		m_MTPSignature.SetWindowTextW(MTPSignature_str);
		//MTP Offset Address
		memcpy((void*)&MTPOffsetAddr, m_RegBuff + sizeof(MTPKey0)+sizeof(MTPKey1)+sizeof(MTPSignature), sizeof(MTPOffsetAddr));
		MTPOffset_str.Format(_T("%04X"), MTPOffsetAddr);
		m_MTPOffsetAddr.SetWindowTextW(MTPOffset_str);
		//MTP Option
		memcpy((void*)&MTPOption, m_RegBuff + sizeof(MTPKey0)+sizeof(MTPKey1)+sizeof(MTPSignature)+sizeof(MTPOffsetAddr), sizeof(MTPOption));
		MTPOption_str.Format(_T("%02X"), MTPOption);
		m_MTPOption.SetWindowTextW(MTPOption_str);
		//MTP Lock
		memcpy((void*)&MTPLock, m_RegBuff + sizeof(MTPKey0)+sizeof(MTPKey1)+sizeof(MTPSignature)+sizeof(MTPOffsetAddr)+sizeof(MTPOption), sizeof(MTPLock));
		m_MTPLock.SetCheck(MTPLock);
	}
	else if (*m_pDataLen == NULL)
	{
		MTP_Enable.SetCheck(FALSE);
		m_MTPKey0.EnableWindow(FALSE);
		m_MTPKey1.EnableWindow(FALSE);
		m_MTPSignature.EnableWindow(FALSE);
		m_MTPOffsetAddr.EnableWindow(FALSE);
		m_MTPOption.EnableWindow(FALSE);
		m_MTPLock.EnableWindow(FALSE);
	}
	return CMFCPropertyPage::OnSetActive();
}
void CDlg_NUC505::OnOK()
{
	if (MTP_Enable.GetCheck() == TRUE)
	{
		memcpy(m_RegBuff, &MTPKey0, sizeof(MTPKey0));
		memcpy(m_RegBuff + sizeof(MTPKey0), &MTPKey1, sizeof(MTPKey1));
		memcpy(m_RegBuff + sizeof(MTPKey0)+sizeof(MTPKey1), &MTPSignature, sizeof(MTPSignature));
		memcpy(m_RegBuff + sizeof(MTPKey0)+sizeof(MTPKey1)+sizeof(MTPSignature), &MTPOffsetAddr, sizeof(MTPOffsetAddr));
		memcpy(m_RegBuff + sizeof(MTPKey0)+sizeof(MTPKey1)+sizeof(MTPSignature)+sizeof(MTPOffsetAddr), &MTPOption, sizeof(MTPOption));
		memcpy(m_RegBuff + sizeof(MTPKey0)+sizeof(MTPKey1)+sizeof(MTPSignature)+sizeof(MTPOffsetAddr)+sizeof(MTPOption), &MTPLock, sizeof(MTPLock));
		*m_pDataLen = sizeof(MTPKey0)+sizeof(MTPKey1)+sizeof(MTPSignature)+sizeof(MTPOffsetAddr)+sizeof(MTPOption)+sizeof(MTPLock);
	}
	else if (MTP_Enable.GetCheck() == FALSE)
	{
		*m_pDataLen = 0x00;
	}
	CMFCPropertyPage::OnOK();
}

LRESULT CDlg_NUC505::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();
}
LRESULT CDlg_NUC505::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}

void CDlg_NUC505::OnEnChangeMTPKey0()
{
	CString Key0_str;
	m_MTPKey0.GetWindowTextW(Key0_str);
	MTPKey0 = numeric_conversion::hexstring_to_size_t(Key0_str.GetString());
}


void CDlg_NUC505::OnEnChangeMTPKey1()
{
	CString Key1_str;
	m_MTPKey1.GetWindowTextW(Key1_str);
	MTPKey1 = numeric_conversion::hexstring_to_size_t(Key1_str.GetString());
}


void CDlg_NUC505::OnEnChangeMTPSignature()
{
	CString signature_str;
	m_MTPSignature.GetWindowTextW(signature_str);
	MTPSignature = numeric_conversion::hexstring_to_size_t(signature_str.GetString());
}


void CDlg_NUC505::OnEnChangeMTPoffsetAddr()
{
	CString OffsetAddr_str;
	m_MTPOffsetAddr.GetWindowTextW(OffsetAddr_str);
	MTPOffsetAddr = numeric_conversion::hexstring_to_size_t(OffsetAddr_str.GetString());
}


void CDlg_NUC505::OnEnChangeMTPOption()
{
	CString Option_str;
	m_MTPOption.GetWindowTextW(Option_str);
	MTPOption = numeric_conversion::hexstring_to_size_t(Option_str.GetString());
}


void CDlg_NUC505::OnBnClickedMTPEnable()
{
	unsigned check = MTP_Enable.GetCheck();
	m_MTPKey0.EnableWindow(check);
	m_MTPKey1.EnableWindow(check);
	m_MTPSignature.EnableWindow(check);
	m_MTPOffsetAddr.EnableWindow(check);
	m_MTPOption.EnableWindow(check);
	m_MTPLock.EnableWindow(check);
}


void CDlg_NUC505::OnBnClickedMTPLockEnable()
{
	MTPLock = m_MTPLock.GetCheck();
}
