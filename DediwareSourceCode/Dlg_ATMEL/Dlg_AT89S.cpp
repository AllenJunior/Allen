// Dlg_AT89S.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_AT89S.h"
#include "afxdialogex.h"


// Dlg_AT89S dialog

IMPLEMENT_DYNAMIC(CDlg_AT89S, CMFCPropertyPage)

CDlg_AT89S::CDlg_AT89S(struct chip_info_c *ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_AT89S::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}
CDlg_AT89S::~CDlg_AT89S()
{
}

void CDlg_AT89S::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_En);
	DDX_Control(pDX, IDC_COMBO1, m_LOCK);
}


BEGIN_MESSAGE_MAP(CDlg_AT89S, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_AT89S::OnCbnSelchangeCombo)
	ON_BN_CLICKED(IDC_CHECK1, &CDlg_AT89S::OnBnClickedEnable)
END_MESSAGE_MAP()


// Dlg_AT89S message handlers
BOOL CDlg_AT89S::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	lockbits = 0x00;
	m_LOCK.InsertString(m_LOCK.GetCount(), L"B1 = 0; B2 =0; <Lock Disable>");
	m_LOCK.InsertString(m_LOCK.GetCount(), L"B1 = 0; B2 =1; <LB1 active>");
	m_LOCK.InsertString(m_LOCK.GetCount(), L"B1 = 1; B2 =0; <LB2 active>");
	m_LOCK.InsertString(m_LOCK.GetCount(), L"B1 = 1; B2 =1; <LB3 active>");

	m_En.SetCheck(0);
	m_LOCK.SetCurSel(0);
	m_LOCK.EnableWindow(FALSE);

	return TRUE;
}
BOOL CDlg_AT89S::OnSetActive()
{
	if (*m_pDataLen != NULL)
	{
		m_En.SetCheck(TRUE);
		m_LOCK.EnableWindow(TRUE);
		memcpy((void*)&lockbits, m_RegBuff, sizeof(lockbits));

		if ((lockbits & 0x3) == 0x00)
			m_LOCK.SetCurSel(0);
		else if ((lockbits & 0x3) == 0x01)
			m_LOCK.SetCurSel(1);
		else if ((lockbits & 0x3) == 0x02)
			m_LOCK.SetCurSel(2);
		else if ((lockbits & 0x3) == 0x03)
			m_LOCK.SetCurSel(3);
	}
	else if (*m_pDataLen == NULL)
	{
		m_En.SetCheck(FALSE);
		m_LOCK.EnableWindow(FALSE);
	}
	return CMFCPropertyPage::OnSetActive();
}

void CDlg_AT89S::OnCbnSelchangeCombo()
{
	lockbits = 0x00;
	if (m_LOCK.GetCurSel() == 0)
		lockbits |= 0x00;
	else if (m_LOCK.GetCurSel() == 1)
		lockbits |= 0x01;
	else if (m_LOCK.GetCurSel() == 2)
		lockbits |= 0x02;
	else if (m_LOCK.GetCurSel() == 3)
		lockbits |= 0x03;
}

void CDlg_AT89S::OnOK()
{
	if (m_En.GetCheck() == 1)
	{
		memcpy(m_RegBuff, &lockbits, sizeof(lockbits));
		*m_pDataLen = sizeof(lockbits);
	}
	else if (m_En.GetCheck() == 0)
	{
		*m_pDataLen = 0;
	}
}
LRESULT CDlg_AT89S::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();

}
LRESULT CDlg_AT89S::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}


void CDlg_AT89S::OnBnClickedEnable()
{
		m_LOCK.EnableWindow(m_En.GetCheck());
}
