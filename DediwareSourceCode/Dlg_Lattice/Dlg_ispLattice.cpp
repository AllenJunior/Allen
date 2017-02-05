// Dlg_ispLattice.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_ispLattice.h"
#include "afxdialogex.h"
#include "numeric_conversion.h"
#include "../common/prog_info.h"
// Dlg_ispLattice dialog

IMPLEMENT_DYNAMIC(CDlg_ispLattice, CMFCPropertyPage)

CDlg_ispLattice::CDlg_ispLattice(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_ispLattice::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_ispLattice::~CDlg_ispLattice()
{
}

void CDlg_ispLattice::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_securityEnable);
	DDX_Control(pDX, IDC_EDIT1, m_usercode);
}


BEGIN_MESSAGE_MAP(CDlg_ispLattice, CMFCPropertyPage)
END_MESSAGE_MAP()


// Dlg_ispLattice message handlers
BOOL CDlg_ispLattice::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	Cfg0 = 0xFFFFFFFF;
	m_usercode.SetWindowTextW(L"FFFFFFFF");
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	Encheck->SetCheck(0);
	return TRUE;
}

BOOL CDlg_ispLattice::OnSetActive()
{
	CString usercode;
	if (*m_pDataLen != NULL)
	{
		memcpy((void*)&Cfg0, m_RegBuff, sizeof(Cfg0));
		usercode.Format(_T("%08X"), Cfg0);
		GetDlgItem(IDC_EDIT1)->SetWindowTextW(usercode);
		memcpy((void*)&Cfg1, m_RegBuff + 4, sizeof(Cfg1));
		if ((Cfg1 & 0x00000001) == 0x01)
		{
			CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
			Encheck->SetCheck(1);
		}
		else if ((Cfg1 & 0x00000001) == 0x00)
		{
			CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
			Encheck->SetCheck(0);
		}
		
	}
	return CMFCPropertyPage::OnSetActive();
}

void CDlg_ispLattice::OnOK()
{
	CString temp;
	m_usercode.GetWindowTextW(temp);
	Cfg0 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff, &Cfg0, sizeof(Cfg0));
	
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	int state = Encheck->GetCheck();
	if (state == 0x1)
	{
		Cfg1 = 0x01;
		memcpy(m_RegBuff + sizeof(Cfg0), &Cfg1, sizeof(Cfg1));
	}
	else if (state == 0x0)
	{
		Cfg1 = 0x00;
		memcpy(m_RegBuff + sizeof(Cfg0), &Cfg1, sizeof(Cfg1));
	}
	*m_pDataLen = sizeof(Cfg1)+sizeof(Cfg0); 

	CMFCPropertyPage::OnOK();
}

LRESULT CDlg_ispLattice::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();

}
LRESULT CDlg_ispLattice::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}
