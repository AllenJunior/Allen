// Dlg_iCE40.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_iCE40.h"
#include "afxdialogex.h"
#include "numeric_conversion.h"
#include "../common/prog_info.h"

// Dlg_iCE40 dialog

IMPLEMENT_DYNAMIC(CDlg_iCE40, CMFCPropertyPage)

CDlg_iCE40::CDlg_iCE40(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_iCE40::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_iCE40::~CDlg_iCE40()
{
}

void CDlg_iCE40::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_SecurityEnable);
}


BEGIN_MESSAGE_MAP(CDlg_iCE40, CMFCPropertyPage)
END_MESSAGE_MAP()


// Dlg_iCE40 message handlers
BOOL CDlg_iCE40::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	Security = 0x00;
	//CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	//Encheck->SetCheck(0);
	m_SecurityEnable.SetCheck(0);

	return TRUE;
}

BOOL CDlg_iCE40::OnSetActive()
{
	if (*m_pDataLen != NULL)
	{
		//CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
		memcpy((void*)&Security, m_RegBuff, sizeof(Security));
		m_SecurityEnable.SetCheck(Security);
	}
	else if (*m_pDataLen == NULL)
	{
		Security = 0x00;
		//CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
		m_SecurityEnable.SetCheck(0);
	}
	return CMFCPropertyPage::OnSetActive();
}
void CDlg_iCE40::OnOK()
{
	//CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	//Security = Encheck->GetCheck();
	
	Security = m_SecurityEnable.GetCheck();
	
	if (Security == 1)
	{
		memcpy(m_RegBuff, &Security, sizeof(Security));
		*m_pDataLen = sizeof(Security);
	}
	else if (Security == 0)
	{
		*m_pDataLen = 0x00;
	}
	
	CMFCPropertyPage::OnOK();
}

LRESULT CDlg_iCE40::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();

}
LRESULT CDlg_iCE40::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}
