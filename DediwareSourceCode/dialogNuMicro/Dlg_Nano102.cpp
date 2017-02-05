// Dlg_Nano102.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_Nano102.h"
#include "afxdialogex.h"
#include "../common/prog_info.h"
#include "numeric_conversion.h"

// Dlg_Nano102 dialog

IMPLEMENT_DYNAMIC(CDlg_Nano102, CMFCPropertyPage)

CDlg_Nano102::CDlg_Nano102(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_Nano102::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_Nano102::~CDlg_Nano102()
{
}

void CDlg_Nano102::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_cfg0);
	DDX_Control(pDX, IDC_EDIT2, m_cfg1);
	DDX_Control(pDX, IDC_COMBO1, m_CFOSC);
	DDX_Control(pDX, IDC_COMBO2, m_CBORST);
	DDX_Control(pDX, IDC_COMBO3, m_CHXTGAIN);
	DDX_Control(pDX, IDC_COMBO4, m_CBS);
	DDX_Control(pDX, IDC_COMBO5, m_LOCK);
	DDX_Control(pDX, IDC_COMBO6, m_DFEN);
	DDX_Control(pDX, IDC_COMBO7, m_WDTEN);
	DDX_Control(pDX, IDC_EDIT3, m_Addr);
}


BEGIN_MESSAGE_MAP(CDlg_Nano102, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_Nano102::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_Nano102::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_Nano102::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_Nano102::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_Nano102::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDlg_Nano102::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDlg_Nano102::OnCbnSelchangeCombo)
	ON_EN_CHANGE(IDC_EDIT3, &CDlg_Nano102::OnEnChangeAddr)
END_MESSAGE_MAP()


// Dlg_Nano102 message handlers
BOOL CDlg_Nano102::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	Val_Cfg0 = 0xFFFFFFFF;
	Val_Cfg1 = 0xFFFFFFFF;
	m_cfg0.SetWindowTextW(L"FFFFFFFF");
	m_cfg1.SetWindowTextW(L"FFFFFFFF");
	m_Addr.SetWindowTextW(L"FFFFFFFF");

	if (wcscmp(_T("NVT8899"), m_ChipInfo->description) == 0){
		m_WDTEN.InsertString(m_WDTEN.GetCount(), L"0 = Watchdog Timer Enable");
		m_WDTEN.InsertString(m_WDTEN.GetCount(), L"1 = Watchdog Timer Disable");
		m_WDTEN.SetCurSel(1);
	}
	else if (wcscmp(_T("Nano102"), m_ChipInfo->description) == 0){
		m_WDTEN.InsertString(m_WDTEN.GetCount(), L"Reserved");
		m_WDTEN.SetCurSel(0);
		m_WDTEN.EnableWindow(FALSE);
	}

	m_CFOSC.InsertString(m_CFOSC.GetCount(), L"0 = External 12MHz crystal clock");
	m_CFOSC.InsertString(m_CFOSC.GetCount(), L"1 = Internal RC 12MHz oscillator clock");

	m_CBORST.InsertString(m_CBORST.GetCount(), L"00 = 1.7V");
	m_CBORST.InsertString(m_CBORST.GetCount(), L"01 = 2.0V");
	m_CBORST.InsertString(m_CBORST.GetCount(), L"10 = 2.5V");
	m_CBORST.InsertString(m_CBORST.GetCount(), L"11 = Disable");

	m_CHXTGAIN.InsertString(m_CHXTGAIN.GetCount(), L"001 = Higher than 16MHz");
	m_CHXTGAIN.InsertString(m_CHXTGAIN.GetCount(), L"011 = 8MHz to 12MHz");
	m_CHXTGAIN.InsertString(m_CHXTGAIN.GetCount(), L"100 = Lower than 8MHz");
	m_CHXTGAIN.InsertString(m_CHXTGAIN.GetCount(), L"111 = 12MHz to 16MHz");

	m_CBS.InsertString(m_CBS.GetCount(), L"00 = Boot from LDROM with IAP");
	m_CBS.InsertString(m_CBS.GetCount(), L"01 = Boot from LDROM");
	m_CBS.InsertString(m_CBS.GetCount(), L"10 = Boot from APROM with IAP");
	m_CBS.InsertString(m_CBS.GetCount(), L"11 = Boot from APROM");

	m_LOCK.InsertString(m_LOCK.GetCount(), L"0 = Flash is Locked");
	m_LOCK.InsertString(m_LOCK.GetCount(), L"1 = Flash is not Locked");

	m_DFEN.InsertString(m_DFEN.GetCount(), L"0 = DataFlash Enabled");
	m_DFEN.InsertString(m_DFEN.GetCount(), L"1 = DataFlash Disabled");
	m_CFOSC.SetCurSel(1);
	m_CBORST.SetCurSel(3);
	m_CHXTGAIN.SetCurSel(3);
	m_CBS.SetCurSel(3);
	m_LOCK.SetCurSel(1);
	m_DFEN.SetCurSel(1);
	m_Addr.EnableWindow(FALSE);

	return TRUE;
}
BOOL CDlg_Nano102::OnSetActive()
{
	CString CFG0;
	CString CFG1;
	if (*m_pDataLen != NULL)
	{
		memcpy((void*)&Val_Cfg0, m_RegBuff, sizeof(Val_Cfg0));
		memcpy((void*)&Val_Cfg1, m_RegBuff + sizeof(Val_Cfg0), sizeof(Val_Cfg1));
		//Config0
		CFG0.Format(_T("%08X"), Val_Cfg0);
		m_cfg0.SetWindowTextW(CFG0);
		//Config1
		CFG1.Format(_T("%08X"), Val_Cfg1);
		m_cfg1.SetWindowTextW(CFG1);
		m_Addr.SetWindowTextW(CFG1);
		//WDTON
		if (wcscmp(_T("NVT8899"), m_ChipInfo->description) == 0){
			if ((Val_Cfg0 & 0x80000000) == 0x80000000)
				m_WDTEN.SetCurSel(1);
			else if ((Val_Cfg0 & 0x80000000) == 0x00)
				m_WDTEN.SetCurSel(0);
		}
		//Clock Source
		if ((Val_Cfg0 & 0x04000000) == 0x04000000)
			m_CFOSC.SetCurSel(1);
		else if ((Val_Cfg0 & 0x04000000) == 0x00)
			m_CFOSC.SetCurSel(0);
		//Brown-out Reset
		if ((Val_Cfg0 & 0x180000) == 0x180000)
			m_CBORST.SetCurSel(3);
		else if ((Val_Cfg0 & 0x180000) == 0x100000)
			m_CBORST.SetCurSel(2);
		else if ((Val_Cfg0 & 0x180000) == 0x080000)
			m_CBORST.SetCurSel(1);
		else if ((Val_Cfg0 & 0x180000) == 0x00)
			m_CBORST.SetCurSel(0);
		//CONFIG HXT
		if ((Val_Cfg0 & 0xE000) == 0xE000)
			m_CHXTGAIN.SetCurSel(3);
		else if ((Val_Cfg0 & 0xE000) == 0x8000)
			m_CHXTGAIN.SetCurSel(2);
		else if ((Val_Cfg0 & 0xE000) == 0x6000)
			m_CHXTGAIN.SetCurSel(1);
		else if ((Val_Cfg0 & 0xE000) == 0x2000)
			m_CHXTGAIN.SetCurSel(0);
		//Boot Select
		if ((Val_Cfg0 & 0xC0) == 0xC0)
			m_CBS.SetCurSel(3);
		else if ((Val_Cfg0 & 0xC0) == 0x80)
			m_CBS.SetCurSel(2);
		else if ((Val_Cfg0 & 0xC0) == 0x40)
			m_CBS.SetCurSel(1);
		else if ((Val_Cfg0 & 0xC0) == 0x00)
			m_CBS.SetCurSel(0);
		//Security Lock
		if ((Val_Cfg0 & 0x02) == 0x02)
			m_LOCK.SetCurSel(1);
		else if ((Val_Cfg0 & 0x02) == 0x00)
			m_LOCK.SetCurSel(0);
		//DataFlash
		if ((Val_Cfg0 & 0x01) == 0x01)
		{
			m_DFEN.SetCurSel(1);
			m_Addr.EnableWindow(FALSE);
		}
		else if ((Val_Cfg0 & 0x01) == 0x00)
		{
			m_DFEN.SetCurSel(0);
			m_Addr.EnableWindow(TRUE);
		}

	}
	return CMFCPropertyPage::OnSetActive();
}

void CDlg_Nano102::OnCbnSelchangeCombo()
{
	CString strTemp;
	Val_Cfg0 = 0xFFFFFFFF;
	Val_Cfg1 = 0xFFFFFFFF;
	//WDTON
	if (wcscmp(_T("NVT8899"), m_ChipInfo->description) == 0){
		Val_Cfg0 &= 0x7FFFFFFF;
		if (m_WDTEN.GetCurSel() == 1)
			Val_Cfg0 |= 0x80000000;
		else if (m_WDTEN.GetCurSel() == 0)
			Val_Cfg0 |= 0x00;
		strTemp.Format(_T("%08X"), Val_Cfg0);
		m_cfg0.SetWindowTextW(strTemp);
	}
	//Clock Source
	Val_Cfg0 &= 0xFBFFFFFF;
	if (m_CFOSC.GetCurSel() == 1)
		Val_Cfg0 |= 0x04000000;
	else if (m_CFOSC.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(strTemp);
	//Brown out Reset
	Val_Cfg0 &= 0xFFE7FFFF;
	if (m_CBORST.GetCurSel() == 3)
		Val_Cfg0 |= 0x180000;
	else if (m_CBORST.GetCurSel() == 2)
		Val_Cfg0 |= 0x100000;
	else if (m_CBORST.GetCurSel() == 1)
		Val_Cfg0 |= 0x080000;
	else if (m_CBORST.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(strTemp);
	//CHXT_GAIN
	Val_Cfg0 &= 0xFFFF1FFF;
	if (m_CHXTGAIN.GetCurSel() == 3)
		Val_Cfg0 |= 0xE000;
	else if (m_CHXTGAIN.GetCurSel() == 2)
		Val_Cfg0 |= 0x8000;
	else if (m_CHXTGAIN.GetCurSel() == 1)
		Val_Cfg0 |= 0x6000;
	else if (m_CHXTGAIN.GetCurSel() == 0)
		Val_Cfg0 |= 0x2000;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(strTemp);
	//Boot Select
	Val_Cfg0 &= 0xFFFFFF3F;
	if (m_CBS.GetCurSel() == 3)
		Val_Cfg0 |= 0xC0;
	else if (m_CBS.GetCurSel() == 2)
		Val_Cfg0 |= 0x80;
	else if (m_CBS.GetCurSel() == 1)
		Val_Cfg0 |= 0x40;
	else if (m_CBS.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(strTemp);
	//Security Lock
	Val_Cfg0 &= 0xFFFFFFFD;
	if (m_LOCK.GetCurSel() == 1)
		Val_Cfg0 |= 0x02;
	else if (m_LOCK.GetCurSel() == 0)
		Val_Cfg0 |= 0x00;
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(strTemp);
	//DataFlash Enable
	Val_Cfg0 &= 0xFFFFFFFE;
	if (m_DFEN.GetCurSel() == 1)
	{
		Val_Cfg0 |= 0x01;
		Val_Cfg1 = 0xFFFFFFFF;
		m_cfg1.SetWindowTextW(L"FFFFFFFF");
		m_Addr.SetWindowTextW(L"FFFFFFFF");
		m_Addr.EnableWindow(FALSE);
	}
	else if (m_DFEN.GetCurSel() == 0)
	{
		Val_Cfg0 |= 0x00;
		m_Addr.EnableWindow(TRUE);
	}
	strTemp.Format(_T("%08X"), Val_Cfg0);
	m_cfg0.SetWindowTextW(strTemp);

}

void CDlg_Nano102::OnOK()
{
	CString temp;
	m_cfg0.GetWindowTextW(temp);
	Val_Cfg0 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff, &Val_Cfg0, sizeof(Val_Cfg0));
	m_cfg1.GetWindowTextW(temp);
	Val_Cfg1 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff + sizeof(Val_Cfg0), &Val_Cfg1, sizeof(Val_Cfg1));
	*m_pDataLen = sizeof(Val_Cfg0)+sizeof(Val_Cfg1);
	CMFCPropertyPage::OnOK();
}

LRESULT CDlg_Nano102::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();
}
LRESULT CDlg_Nano102::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}


void CDlg_Nano102::OnEnChangeAddr()
{
	CString Addr;
	if (m_DFEN.GetCurSel() == 0)
	{
		m_Addr.GetWindowTextW(Addr);
		m_cfg1.SetWindowTextW(Addr);
		Val_Cfg1 = numeric_conversion::hexstring_to_size_t(Addr.GetString());
	}
}
