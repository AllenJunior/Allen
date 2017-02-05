// DlgS08.cpp : implementation file
//

#include "stdafx.h"
#include "DlgS08.h"
#include "afxdialogex.h"
#include "../common/prog_info.h"

// DlgS08 dialog


IMPLEMENT_DYNAMIC(DlgS08, CMFCPropertyPage)

DlgS08::DlgS08(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
	: CMFCPropertyPage(DlgS08::IDD),
	m_ChipInfo(ChipInfo),
	m_RegBuff(RegisterBuff),
	m_BuffLen(BuffLen),
	m_pDataLen(pDataLen)
	//, m_Len(0xFFFF)
{

}

DlgS08::~DlgS08()
{
}

void DlgS08::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_Protect);
	DDX_Control(pDX, IDC_CHECK2, m_Trim);
}


BEGIN_MESSAGE_MAP(DlgS08, CMFCPropertyPage)
END_MESSAGE_MAP()


// DlgS08 message handlers

BOOL DlgS08::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	// TODO:  Add extra initialization here
	switch (m_ChipInfo->partition_info[0].init_parameter&0xFF)
	{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			m_Protect.SetWindowTextW(_T("Write Protect\r\n\
										0xFFBF = NVOPT"));
			m_Trim.SetWindowTextW(_T("Write Trim Value(Base on 3.3V,31.25kHz)\r\n\
									 0xFFAE = NVFTRIM\r\n\
									 0xFFAF = NVTRIM "));
			m_Len = 8;
			break;
		case 6:
		case 7:
		case 8:
		case 9:
			m_Protect.SetWindowTextW(_T("Write Protect\r\n\
										0xFFBF = NVOPT"));
			m_Trim.SetWindowTextW(_T("Write Trim Value(Base on 3.3V)\r\n\
									 0xFFBC = NVTRIM(250kHz)\r\n\
									 0xFFBE = NVTRIM(243kHz)"));
			m_Len = 12;
			break;
		default:
			MessageBoxW(_T("Unsupport"));
			break;

	}
	m_Trim.SetCheck(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL DlgS08::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	OPTION option;

	if (*m_pDataLen != NULL){
		memcpy((void*)&option, m_RegBuff, m_Len);
		m_Protect.SetCheck(option.EnOption & 0x1);
		m_Trim.SetCheck((option.EnOption & 0x2) >> 1);
	}

	return CMFCPropertyPage::OnSetActive();
}

void DlgS08::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	OPTION option;

	option.EnOption =(m_Trim.GetCheck()<<1) | m_Protect.GetCheck();
	option.Trim1_Value = 0;
	option.Trim2_Value = 0;

	switch (m_ChipInfo->partition_info[0].init_parameter&0xFF){
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			option.Trim1_Value = 0x640;
			m_Len = 8;
			break;

		case 6:
		case 7:
		case 8:
		case 9:
			option.Trim1_Value = 0x500;
			option.Trim2_Value = 0x4DC;
			m_Len = 12;
			break;
		default:
			MessageBoxW(_T("Unsupport"));
			m_Len = 8;
			break;
	}

	*m_pDataLen = m_Len;
	memcpy(m_RegBuff, (void*)&option, m_Len);

	CMFCPropertyPage::OnOK();
}


LRESULT DlgS08::OnWizardNext()
{
	// TODO: Add your specialized code here and/or call the base class
	OnOK();
	return CMFCPropertyPage::OnWizardNext();
}


LRESULT DlgS08::OnWizardBack()
{
	// TODO: Add your specialized code here and/or call the base class
	OnOK();

	return CMFCPropertyPage::OnWizardBack();
}



