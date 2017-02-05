// DLG_M05X.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_M05X.h"
#include "afxdialogex.h"
#include "../common/prog_info.h"
#include "numeric_conversion.h"

// DLG_M05X dialog

IMPLEMENT_DYNAMIC(CDLG_M05X, CMFCPropertyPage)

CDLG_M05X::CDLG_M05X(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDLG_M05X::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDLG_M05X::~CDLG_M05X()
{
}

void CDLG_M05X::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_CLKFilter);
	DDX_Control(pDX, IDC_COMBO2, m_CLKSource);
	DDX_Control(pDX, IDC_COMBO3, m_BroDect);
	DDX_Control(pDX, IDC_COMBO4, m_BroVol);
	DDX_Control(pDX, IDC_COMBO5, m_BroReset);
	DDX_Control(pDX, IDC_COMBO6, m_BootSelect);
	DDX_Control(pDX, IDC_COMBO7, m_Lock);
	DDX_Control(pDX, IDC_EDIT1, m_Cfg0);
}


BEGIN_MESSAGE_MAP(CDLG_M05X, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDLG_M05X::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDLG_M05X::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDLG_M05X::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDLG_M05X::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDLG_M05X::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDLG_M05X::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDLG_M05X::OnCbnSelchangeCombo)
	ON_BN_CLICKED(IDC_BUTTON1, &CDLG_M05X::OnBnClickedButton1)
END_MESSAGE_MAP()


// DLG_M05X message handlers

BOOL CDLG_M05X::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	Val_Cfg0 = 0xFFFFFFFF;
	m_Cfg0.SetWindowTextW(L"FFFFFFFF");
	m_Cfg0.EnableWindow(false);
	m_Lock.InsertString(m_Lock.GetCount(), L"0 = Flash data is locked");
	m_Lock.InsertString(m_Lock.GetCount(), L"1 = Flash data is NOT locked");
	m_BootSelect.InsertString(m_BootSelect.GetCount(), L"0 = Chip boot from LDROM");
	m_BootSelect.InsertString(m_BootSelect.GetCount(), L"1 = Chip boot from APROM");
	m_CLKFilter.InsertString(m_CLKFilter.GetCount(), L"0 = Disable clock filter");
	m_CLKFilter.InsertString(m_CLKFilter.GetCount(), L"1 = Enable clock filter");
	m_BroReset.InsertString(m_BroReset.GetCount(), L"0 = Enable Brown Out Reset after Power on");
	m_BroReset.InsertString(m_BroReset.GetCount(), L"1 = Disable Brown Out Reset after Power on");
	m_BroVol.InsertString(m_BroVol.GetCount(), L"00 = 2.2V");
	m_BroVol.InsertString(m_BroVol.GetCount(), L"01 = 2.7V");
	m_BroVol.InsertString(m_BroVol.GetCount(), L"10 = 3.8V");
	m_BroVol.InsertString(m_BroVol.GetCount(), L"11 = 4.5V");
	m_BroDect.InsertString(m_BroDect.GetCount(), L"0 = Enabled Brown-Out detect after powered on");
	m_BroDect.InsertString(m_BroDect.GetCount(), L"1 = Disabled Brown-Out detect after powered on");
	m_CLKSource.InsertString(m_CLKSource.GetCount(), L"000 = External 4~24 MHz crystal OSC Clock");
	m_CLKSource.InsertString(m_CLKSource.GetCount(), L"111 = Internal 22.1184 MHz OSC Clock");
	m_Lock.SetCurSel(1);
	m_BootSelect.SetCurSel(1);
	m_CLKFilter.SetCurSel(1);
	m_BroReset.SetCurSel(1);
	m_BroVol.SetCurSel(3);
	m_BroDect.SetCurSel(1);
	m_CLKSource.SetCurSel(1);

	return TRUE;

}

BOOL CDLG_M05X::OnSetActive()
{
	CString CFG0;
	if (*m_pDataLen != NULL){
		memcpy((void*)&Val_Cfg0, m_RegBuff, 4);

		//CFG0
		CFG0.Format(_T("%08X"), Val_Cfg0);
		GetDlgItem(IDC_EDIT1)->SetWindowTextW(CFG0);
		//Code Protect
		if ((Val_Cfg0 & 0x00000002) == 0x00000002)
			m_Lock.SetCurSel(1);
		else if ((Val_Cfg0 & 0x00000002) == 0x00000000)
			m_Lock.SetCurSel(0);
		//Boot Select
		if ((Val_Cfg0 & 0x00000080) == 0x00000080)
			m_BootSelect.SetCurSel(1);
		else if ((Val_Cfg0 & 0x00000080) == 0x00000000)
			m_BootSelect.SetCurSel(0);
		//Brown Out Reset Enable
		if ((Val_Cfg0 & 0x00100000) == 0x00100000)
			m_BroReset.SetCurSel(1);
		else if ((Val_Cfg0 & 0x00100000) == 0x00000000)
			m_BroReset.SetCurSel(0);
		//Brown Out Voltage
		if ((Val_Cfg0 & 0x00600000) == 0x00600000)
			m_BroVol.SetCurSel(3);
		else if ((Val_Cfg0 & 0x00600000) == 0x00400000)
			m_BroVol.SetCurSel(2);
		else if ((Val_Cfg0 & 0x00600000) == 0x00200000)
			m_BroVol.SetCurSel(1);
		else if ((Val_Cfg0 & 0x00600000) == 0x00000000)
			m_BroVol.SetCurSel(0);
		//CLK Source
		if ((Val_Cfg0 & 0x07000000) == 0x07000000)
			m_CLKSource.SetCurSel(1);
		else if ((Val_Cfg0 & 0x07000000) == 0x00000000)
			m_CLKSource.SetCurSel(0);
		//CLK Filter
		if ((Val_Cfg0 & 0x10000000) == 0x10000000)
			m_CLKFilter.SetCurSel(1);
		else if ((Val_Cfg0 & 0x10000000) == 0x00000000)
			m_CLKFilter.SetCurSel(0);
		//Brown Out Detection
		if ((Val_Cfg0 & 0x00800000) == 0x00800000)
			m_BroDect.SetCurSel(1);
		else if ((Val_Cfg0 & 0x00800000) == 0x00000000)
			m_BroDect.SetCurSel(0);
	}

	return CMFCPropertyPage::OnSetActive();
}

void CDLG_M05X::OnCbnSelchangeCombo()
{
	CString strTemp;
	Val_Cfg0 = 0xFFFFFFFF;

	//Code Protect
	Val_Cfg0 &= 0xFFFFFFFD;
	if (m_Lock.GetCurSel() == 1)
		Val_Cfg0 |= 0x02;
	else if (m_Lock.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);

	//BootSelect
	Val_Cfg0 &= 0xFFFFFF7F;
	if (m_BootSelect.GetCurSel() == 1)
		Val_Cfg0 |= 0x80;
	else if (m_BootSelect.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);

	//Brown Out Reset Enable
	Val_Cfg0 &= 0xFFEFFFFF;
	if (m_BroReset.GetCurSel() == 1)
		Val_Cfg0 |= 0x100000;
	else if (m_BroReset.GetCurSel() == 0)
		Val_Cfg0 |= 0x000000;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);

	//Brown Detection
	Val_Cfg0 &= 0xFF7FFFFF;
	if (m_BroDect.GetCurSel() == 1)
	Val_Cfg0 |= 0x800000;
	else if (m_BroDect.GetCurSel() == 0)
	Val_Cfg0 |= 0x000000;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);

	//Brown Out Voltage
	Val_Cfg0 &= 0xFF9FFFFF;
	switch (m_BroVol.GetCurSel())
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

	//Clock Source
	Val_Cfg0 &= 0xF8FFFFFF;
	if (m_CLKSource.GetCurSel() == 1)
		Val_Cfg0 |= 0x07000000;
	else if (m_CLKSource.GetCurSel() == 0)
		Val_Cfg0 |= 0x00000000;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);

	//CLK Filter
	Val_Cfg0 &= 0xEFFFFFFF;
	if (m_CLKFilter.GetCurSel() == 1)
		Val_Cfg0 |= 0x10000000;
	else if (m_CLKFilter.GetCurSel() == 0)
		Val_Cfg0 |= 0x00000000;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_Cfg0.SetWindowTextW(strTemp);

}

void CDLG_M05X::OnOK()
{
	CString temp;
	m_Cfg0.GetWindowTextW(temp);
	Val_Cfg0 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	*m_pDataLen = sizeof(Val_Cfg0);
	memcpy(m_RegBuff, &Val_Cfg0, sizeof(Val_Cfg0));
	CMFCPropertyPage::OnOK();

}

LRESULT CDLG_M05X::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();
}


LRESULT CDLG_M05X::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}

void CDLG_M05X::OnBnClickedButton1()
{
	m_Cfg0.EnableWindow(true);
}