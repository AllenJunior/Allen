// DLG_NUC1X0ACN.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_NUC1X0ACN.h"
#include "afxdialogex.h"
#include "../common/prog_info.h"
#include "numeric_conversion.h"


// DLG_NUC1X0ACN dialog

IMPLEMENT_DYNAMIC(CDLG_NUC1X0ACN, CMFCPropertyPage)

CDLG_NUC1X0ACN::CDLG_NUC1X0ACN(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDLG_NUC1X0ACN::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen),
m_Len(0)
{

}

CDLG_NUC1X0ACN::~CDLG_NUC1X0ACN()
{
}

void CDLG_NUC1X0ACN::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Cfg0);
	DDX_Control(pDX, IDC_EDIT2, m_Cfg1);
	DDX_Control(pDX, IDC_COMBO1, m_CLKFilter);
	DDX_Control(pDX, IDC_COMBO2, m_CLKSource);
	DDX_Control(pDX, IDC_COMBO3, m_BroDect);
	DDX_Control(pDX, IDC_COMBO4, m_BroVol);
	DDX_Control(pDX, IDC_COMBO5, m_BroReset);
	DDX_Control(pDX, IDC_COMBO6, m_BootSelect);
	DDX_Control(pDX, IDC_COMBO7, m_Lock);
	DDX_Control(pDX, IDC_COMBO9, m_DFlash);
}


BEGIN_MESSAGE_MAP(CDLG_NUC1X0ACN, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDLG_NUC1X0ACN::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDLG_NUC1X0ACN::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDLG_NUC1X0ACN::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDLG_NUC1X0ACN::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDLG_NUC1X0ACN::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDLG_NUC1X0ACN::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDLG_NUC1X0ACN::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO9, &CDLG_NUC1X0ACN::OnCbnSelchangeCombo)
	ON_BN_CLICKED(IDC_BUTTON1, &CDLG_NUC1X0ACN::OnBnClickedButton1)
ON_EN_CHANGE(IDC_EDIT1_Addr, &CDLG_NUC1X0ACN::OnEnChangeEdit1Addr)
END_MESSAGE_MAP()


// DLG_NUC1X0ACN message handlers
BOOL CDLG_NUC1X0ACN::OnInitDialog()
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
	m_BootSelect.InsertString(m_BootSelect.GetCount(), L"0 = Chip boot from LDROM");
	m_BootSelect.InsertString(m_BootSelect.GetCount(), L"1 = Chip boot from APROM");
	m_CLKFilter.InsertString(m_CLKFilter.GetCount(), L"0 = Disable clock filter");
	m_CLKFilter.InsertString(m_CLKFilter.GetCount(), L"1 = Enable clock filter");
	m_BroReset.InsertString(m_BroReset.GetCount(), L"0 = Enable Brown Out Reset after Power on");
	m_BroReset.InsertString(m_BroReset.GetCount(), L"1 = Disable Brown Out Reset after Power on");
	m_BroVol.InsertString(m_BroVol.GetCount(), L"00 = 2.2V");
	m_BroVol.InsertString(m_BroVol.GetCount(), L"01 = 2.6V");
	m_BroVol.InsertString(m_BroVol.GetCount(), L"10 = 3.8V");
	m_BroVol.InsertString(m_BroVol.GetCount(), L"11 = 4.5V");
	m_BroDect.InsertString(m_BroDect.GetCount(), L"0 = Enabled Brown-Out detect after powered on");
	m_BroDect.InsertString(m_BroDect.GetCount(), L"1 = Disabled Brown-Out detect after powered on");
	m_CLKSource.InsertString(m_CLKSource.GetCount(), L"000 = External 12 MHz crystal Clock");
	m_CLKSource.InsertString(m_CLKSource.GetCount(), L"111 = Internal RC 22 MHz OSC Clock");
	m_DFlash.InsertString(m_DFlash.GetCount(), L"0 = Enable Data Flash");
	m_DFlash.InsertString(m_DFlash.GetCount(), L"1 = Disable Data Flash");	
	m_Lock.SetCurSel(1);
	m_BootSelect.SetCurSel(1);
	m_CLKFilter.SetCurSel(1);
	m_BroReset.SetCurSel(1);
	m_BroVol.SetCurSel(3);
	m_BroDect.SetCurSel(1);
	m_CLKSource.SetCurSel(1);
	m_DFlash.SetCurSel(1);
	GetDlgItem(IDC_EDIT1_Addr)->EnableWindow(false);
	return TRUE;
}

BOOL CDLG_NUC1X0ACN::OnSetActive()
{

	CString CFG0;
	CString CFG1;
	if (*m_pDataLen != NULL){
		memcpy((void*)&Val_Cfg0, m_RegBuff, 4);
		memcpy((void*)&Val_Cfg1, m_RegBuff + 4, 4);
		//CFG0
		CFG0.Format(_T("%08X"), Val_Cfg0);
		GetDlgItem(IDC_EDIT1)->SetWindowTextW(CFG0);
		//CFG1
		CFG1.Format(_T("%08X"), Val_Cfg1);
		GetDlgItem(IDC_EDIT2)->SetWindowTextW(CFG1);
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


void CDLG_NUC1X0ACN::OnCbnSelchangeCombo()
{
	CString strTemp;
	Val_Cfg0 = 0xFFFFFFFF;
	Val_Cfg1 = 0xFFFFFFFF;

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

void CDLG_NUC1X0ACN::OnOK()
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


LRESULT CDLG_NUC1X0ACN::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();
}

LRESULT CDLG_NUC1X0ACN::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}

void CDLG_NUC1X0ACN::OnBnClickedButton1()
{
	m_Cfg0.EnableWindow(true);
	m_Cfg1.EnableWindow(true);
}



void CDLG_NUC1X0ACN::OnEnChangeEdit1Addr()
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