// DLG_ISD9XXX.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_ISD9XXX.h"
#include "afxdialogex.h"
#include "../common/prog_info.h"
#include "numeric_conversion.h"

// DLG_ISD9XXX dialog

IMPLEMENT_DYNAMIC(CDLG_ISD9XXX, CMFCPropertyPage)

CDLG_ISD9XXX::CDLG_ISD9XXX(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDLG_ISD9XXX::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen),
m_Len(0)
{

}

CDLG_ISD9XXX::~CDLG_ISD9XXX()
{
}

void CDLG_ISD9XXX::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Cfg0);
	DDX_Control(pDX, IDC_EDIT2, m_Cfg1);
	DDX_Control(pDX, IDC_COMBO1, m_BroDet);
	DDX_Control(pDX, IDC_COMBO2, m_boot);
	DDX_Control(pDX, IDC_COMBO3, m_LDEn);
	DDX_Control(pDX, IDC_COMBO4, m_lock);
	DDX_Control(pDX, IDC_COMBO5, m_DataFlash);
}


BEGIN_MESSAGE_MAP(CDLG_ISD9XXX, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDLG_ISD9XXX::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDLG_ISD9XXX::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDLG_ISD9XXX::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDLG_ISD9XXX::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDLG_ISD9XXX::OnCbnSelchangeCombo)
	ON_EN_CHANGE(IDC_EDIT3, &CDLG_ISD9XXX::OnEnChangeEdit3)
END_MESSAGE_MAP()


// DLG_ISD9XXX message handlers
BOOL CDLG_ISD9XXX::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	Val_Cfg0 = 0xFFFFFFFF;
	Val_Cfg1 = 0xFFFFFFFF;
	m_Cfg0.SetWindowTextW(L"FFFFFFFF");
	m_Cfg1.SetWindowTextW(L"FFFFFFFF");
	GetDlgItem(IDC_EDIT3)->SetWindowTextW(L"FFFFFFFF");
	GetDlgItem(IDC_EDIT3)->EnableWindow(false);
	m_Cfg0.EnableWindow(false);
	m_Cfg1.EnableWindow(false);
	m_BroDet.InsertString(m_BroDet.GetCount(), L"0 = Enable");
	m_BroDet.InsertString(m_BroDet.GetCount(), L"1 = Disable Brown out detect after power on");
	m_boot.InsertString(m_boot.GetCount(), L"0 = Chip boot from LDROM");
	m_boot.InsertString(m_boot.GetCount(), L"1 = Chip boot from APROM");
	m_LDEn.InsertString(m_LDEn.GetCount(), L"0 = Disable");
	m_LDEn.InsertString(m_LDEn.GetCount(), L"1 = Enable");
	m_lock.InsertString(m_lock.GetCount(), L"0 = Flash data is locked");
	m_lock.InsertString(m_lock.GetCount(), L"1 = Flash data is NOT locked");
	m_DataFlash.InsertString(m_DataFlash.GetCount(), L"0 = Enable DataFlash");
	m_DataFlash.InsertString(m_DataFlash.GetCount(), L"1 = Disable DataFlash");
	m_BroDet.SetCurSel(1);
	m_boot.SetCurSel(1);
	m_LDEn.SetCurSel(1);
	m_lock.SetCurSel(1);
	m_DataFlash.SetCurSel(1);
	return TRUE;
}

BOOL CDLG_ISD9XXX::OnSetActive()
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
		
		//Brown-Out Detect
		if ((Val_Cfg0 & 0x00800000) == 0x00800000)
			m_BroDet.SetCurSel(1);
		else if ((Val_Cfg0 & 0x00800000) == 0x00)
			m_BroDet.SetCurSel(0);
		//BootSelect
		if ((Val_Cfg0 & 0x80) == 0x80)
			m_boot.SetCurSel(1);
		else if ((Val_Cfg0 & 0x80) == 0x00)
			m_boot.SetCurSel(0);
		//LDROM Enable
		if ((Val_Cfg0 & 0x04) == 0x04)
			m_LDEn.SetCurSel(1);
		else if ((Val_Cfg0 & 0x04) == 0x00)
			m_LDEn.SetCurSel(0);
		//Code Protect
		if ((Val_Cfg0 & 0x02) == 0x02)
			m_lock.SetCurSel(1);
		else if ((Val_Cfg0 & 0x02) == 0x00)
			m_lock.SetCurSel(0);
		//DataFlash
		if ((Val_Cfg0 & 0x01) == 0x01)
		{
			m_DataFlash.SetCurSel(1);
			GetDlgItem(IDC_EDIT3)->EnableWindow(false);
		}
		else if((Val_Cfg0 & 0x01) == 0x00)
		{
			m_DataFlash.SetCurSel(0);
			GetDlgItem(IDC_EDIT3)->EnableWindow(true);
		}
	}
	return CMFCPropertyPage::OnSetActive();
}

void CDLG_ISD9XXX::OnCbnSelchangeCombo()
{
	CString strTemp;
	Val_Cfg0 = 0xFFFFFFFF;
	Val_Cfg1 = 0xFFFFFFFF;

	//Brown-Out Detect
	Val_Cfg0 &= 0xFF7FFFFF;
	if (m_BroDet.GetCurSel() == 1)
		Val_Cfg0 |= 0x800000;
	else if (m_BroDet.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);

	//Boot Select
	Val_Cfg0 &= 0xFFFFFF7F;
	if (m_boot.GetCurSel() == 1)
		Val_Cfg0 |= 0x80;
	else if (m_boot.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);

	//LDROM Enable
	Val_Cfg0 &= 0xFFFFFFFB;
	if (m_LDEn.GetCurSel() == 1)
		Val_Cfg0 |= 0x04;
	else if (m_LDEn.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);

	//Code Protect
	Val_Cfg0 &= 0xFFFFFFFD;
	if (m_lock.GetCurSel() == 1)
		Val_Cfg0 |= 0x02;
	else if (m_lock.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);

	//DataFlash Enable
	Val_Cfg0 &= 0xFFFFFFFE;
	if (m_DataFlash.GetCurSel() == 1)
	{
		Val_Cfg0 |= 0x01;
		GetDlgItem(IDC_EDIT3)->EnableWindow(false);
	}
	else if (m_DataFlash.GetCurSel() == 0)
	{
		Val_Cfg0 |= 0x00;
		GetDlgItem(IDC_EDIT3)->EnableWindow(true);
	}
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);



}

void CDLG_ISD9XXX::OnOK()
{
	CString temp;
	m_Len = 8;
	*m_pDataLen = m_Len;
	m_Cfg0.GetWindowTextW(temp);
	Val_Cfg0 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff, &Val_Cfg0, 4);
	m_Cfg1.GetWindowTextW(temp);
	Val_Cfg1 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff + 4, &Val_Cfg1, 4);
	CMFCPropertyPage::OnOK();
}

LRESULT CDLG_ISD9XXX::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();
}
LRESULT CDLG_ISD9XXX::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}

void CDLG_ISD9XXX::OnEnChangeEdit3()
{
	//DataFlash Address
	CString Addr;
	if (m_DataFlash.GetCurSel() == 0)
	{
		GetDlgItem(IDC_EDIT3)->GetWindowText(Addr);
		m_Cfg1.SetWindowTextW(Addr);
		Val_Cfg1 = numeric_conversion::hexstring_to_size_t(Addr.GetString());
	}
}
