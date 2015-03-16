// DlgMini51.cpp : implementation file
//

#include "stdafx.h"
#include "DlgMini51.h"
#include "afxdialogex.h"
#include "../common/prog_info.h"
#include "numeric_conversion.h"

// CDlgMini51 dialog

IMPLEMENT_DYNAMIC(CDlgMini51, CMFCPropertyPage)

CDlgMini51::CDlgMini51(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
	: CMFCPropertyPage(CDlgMini51::IDD),
	m_ChipInfo(ChipInfo),
	m_RegBuff(RegisterBuff),
	m_BuffLen(BuffLen),
	m_pDataLen(pDataLen),
	m_Len(0)
{

}

CDlgMini51::~CDlgMini51()
{
}

void CDlgMini51::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_Lock);
	DDX_Control(pDX, IDC_COMBO2, m_Boot);
	DDX_Control(pDX, IDC_COMBO3, m_Clock);
	DDX_Control(pDX, IDC_COMBO5, m_Borv);
	DDX_Control(pDX, IDC_COMBO6, m_BorReset);
	DDX_Control(pDX, IDC_COMBO7, m_DFlash);
	DDX_Control(pDX, IDC_EDIT_CFG0, m_Cfg0);
	DDX_Control(pDX, IDC_EDIT_CFG1, m_Cfg1);
}


BEGIN_MESSAGE_MAP(CDlgMini51, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgMini51::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlgMini51::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlgMini51::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlgMini51::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDlgMini51::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDlgMini51::OnCbnSelchangeCombo)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgMini51::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1_Addr, &CDlgMini51::OnEnChangeEdit1Addr)
END_MESSAGE_MAP()


// CDlgMini51 message handlers
BOOL CDlgMini51::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	Val_Cfg0 = 0xFFFFFFFF;
	Val_Cfg1 = 0xFFFFFFFF;
	m_Cfg0.SetWindowTextW(L"FFFFFFFF");
	m_Cfg1.SetWindowTextW(L"FFFFFFFF");
	m_Cfg0.EnableWindow(false);
	m_Cfg1.EnableWindow(false);
	m_Lock.InsertString(m_Lock.GetCount(), L"0 = Flash data is locked");
	m_Lock.InsertString(m_Lock.GetCount(), L"1 = Flash data is NOT locked");
	m_Boot.InsertString(m_Boot.GetCount(), L"0 = Chip boot from LDROM");
	m_Boot.InsertString(m_Boot.GetCount(), L"1 = Chip boot from APROM");
	m_Clock.InsertString(m_Clock.GetCount(), L"0 = Disable clock filter");
	m_Clock.InsertString(m_Clock.GetCount(), L"1 = Enable clock filter");
	m_BorReset.InsertString(m_BorReset.GetCount(), L"0 = Enable Brown Out Reset after Power on");
	m_BorReset.InsertString(m_BorReset.GetCount(), L"1 = Disable Brown Out Reset after Power on");
	m_Borv.InsertString(m_Borv.GetCount(), L"00 = 2.7V");
	m_Borv.InsertString(m_Borv.GetCount(), L"01 = 2.7V");
	m_Borv.InsertString(m_Borv.GetCount(), L"10 = 3.8V");
	m_Borv.InsertString(m_Borv.GetCount(), L"11 = Disable");
	m_DFlash.InsertString(m_DFlash.GetCount(), L"0 = Enable Data Flash");
	m_DFlash.InsertString(m_DFlash.GetCount(), L"1 = Disable Data Flash");
	m_Lock.SetCurSel(1);
	m_Boot.SetCurSel(1);
	m_Clock.SetCurSel(1);
	m_BorReset.SetCurSel(1);
	m_Borv.SetCurSel(3);
	m_DFlash.SetCurSel(1);
	GetDlgItem(IDC_EDIT1_Addr)->EnableWindow(false);
	return TRUE;
	
}

BOOL CDlgMini51::OnSetActive()
{
	CString CFG0;
	CString CFG1;
	if (*m_pDataLen != NULL){
		memcpy((void*)&Val_Cfg0, m_RegBuff, 4);
		memcpy((void*)&Val_Cfg1, m_RegBuff+4, 4);

		//CFG0
		CFG0.Format(_T("%08X"), Val_Cfg0);
		GetDlgItem(IDC_EDIT_CFG0)->SetWindowTextW(CFG0);
		//CFG1
		CFG1.Format(_T("%08X"), Val_Cfg1);
		GetDlgItem(IDC_EDIT_CFG1)->SetWindowTextW(CFG1);
		GetDlgItem(IDC_EDIT1_Addr)->SetWindowTextW(CFG1);

		//Data Flash Enable
		if ((Val_Cfg0 & 0x00000001) == 0x00000001)
		{
			m_DFlash.SetCurSel(1);
			GetDlgItem(IDC_EDIT1_Addr)->EnableWindow(false);
		}
		else if ((Val_Cfg0 & 0x00000001) == 0x00000000)
		{
			m_DFlash.SetCurSel(0);
			GetDlgItem(IDC_EDIT1_Addr)->EnableWindow(true);
		}
		//Code Protect
		if ((Val_Cfg0 & 0x00000002) == 0x00000002)
			m_Lock.SetCurSel(1);
		else if ((Val_Cfg0 & 0x00000002) == 0x00000000)
			m_Lock.SetCurSel(0);
		//Boot Select
		if ((Val_Cfg0 & 0x00000080) == 0x00000002)
			m_Boot.SetCurSel(1);
		else if ((Val_Cfg0 & 0x00000080) == 0x00000000)
			m_Boot.SetCurSel(0);
		//Brown Out Reset Enable
		if ((Val_Cfg0 & 0x00100000) == 0x00100000)
			m_BorReset.SetCurSel(1);
		else if ((Val_Cfg0 & 0x00100000) == 0x00000000)
			m_BorReset.SetCurSel(0);
		//Brown Out Voltage
		if ((Val_Cfg0 & 0x00600000) == 0x00600000)
			m_Borv.SetCurSel(3);
		else if ((Val_Cfg0 & 0x00600000) == 0x00400000)
			m_Borv.SetCurSel(2);
		else if ((Val_Cfg0 & 0x00600000) == 0x00200000)
			m_Borv.SetCurSel(1);
		else if ((Val_Cfg0 & 0x00600000) == 0x00000000)
			m_Borv.SetCurSel(0);
		//CLK Filter
		if ((Val_Cfg0 & 0x10000000) == 0x10000000)
			m_Clock.SetCurSel(1);
		else if ((Val_Cfg0 & 0x10000000) == 0x00000000)
			m_Clock.SetCurSel(0);
	}
	return CMFCPropertyPage::OnSetActive();
}

void CDlgMini51::OnCbnSelchangeCombo()
{
	CString strTemp;
	Val_Cfg0 = 0xFFFFFFFF;
	Val_Cfg1 = 0xFFFFFFFF;

	//Code Protect
	Val_Cfg0 &= 0xFFFFFFFD;		
	if (m_Lock.GetCurSel() == 1)
		Val_Cfg0 |= 0x02;
	else if(m_Lock.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);

	//DataFlash Enable
	Val_Cfg0 &= 0xFFFFFFFE;		
	if (m_DFlash.GetCurSel() == 1)
	{
		Val_Cfg0 |= 0x01;
		GetDlgItem(IDC_EDIT1_Addr)->EnableWindow(false);
	}
	else if (m_DFlash.GetCurSel() == 0)
	{
		Val_Cfg0 |= 0x00;
		GetDlgItem(IDC_EDIT1_Addr)->EnableWindow(true);
	}
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);
	
	//BootSelect
	Val_Cfg0 &= 0xFFFFFF7F;
	if (m_Boot.GetCurSel() == 1)
		Val_Cfg0 |= 0x80;
	else if (m_Boot.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);

	//CLK Filter
	Val_Cfg0 &= 0xEFFFFFFF;
	if (m_Clock.GetCurSel() == 1)
		Val_Cfg0 |= 0x10000000;
	else if (m_Clock.GetCurSel() == 0)
		Val_Cfg0 |= 0x00000000;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);

	//Brown Out Reset Enable
	Val_Cfg0 &= 0xFFEFFFFF;
	if (m_BorReset.GetCurSel() == 1)
		Val_Cfg0 |= 0x100000;
	else if (m_BorReset.GetCurSel() == 0)
		Val_Cfg0 |= 0x000000;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);

	//Brown Out Voltage
	Val_Cfg0 &= 0xFF9FFFFF;
	switch (m_Borv.GetCurSel())
	{
	case 0:
		Val_Cfg0 |= 0x000000;
		break;
	case 1:
		Val_Cfg0 |= 0x200000;
		break;
	case 2:
		Val_Cfg0 |= 0x400000;
		break;
	case 3:
		Val_Cfg0 |= 0x600000;
		break;
	}
		strTemp.Format(_T("%08X"), Val_Cfg0);
		m_Cfg0.SetWindowTextW(strTemp);
}


void CDlgMini51::OnOK()
{
	CString temp;
	m_Len = 8;
	*m_pDataLen = m_Len;
	m_Cfg0.GetWindowTextW(temp);
	Val_Cfg0 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff, &Val_Cfg0, 4);

	m_Cfg1.GetWindowTextW(temp);
	Val_Cfg1 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff+4, &Val_Cfg1, 4);
	CMFCPropertyPage::OnOK();
}


LRESULT CDlgMini51::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();
}


LRESULT CDlgMini51::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}



void CDlgMini51::OnDoubleclicked()
{
	m_Cfg0.EnableWindow(true);
	m_Cfg1.EnableWindow(true);
}


void CDlgMini51::OnBnClickedButton1()
{
	m_Cfg0.EnableWindow(true);
	m_Cfg1.EnableWindow(true);
}

void CDlgMini51::OnEnChangeEdit1Addr()
{
	//DataFlash Address
	CString Addr;
	if (m_DFlash.GetCurSel() == 0)
	{
		GetDlgItem(IDC_EDIT1_Addr)->GetWindowText(Addr);
		m_Cfg1.SetWindowTextW(Addr);
		Val_Cfg1 = numeric_conversion::hexstring_to_size_t(Addr.GetString());
	}
}
