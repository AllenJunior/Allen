// Dlg_MC96FC664A.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_MC96FC664A.h"
#include "numeric_conversion.h"
#include "afxdialogex.h"
#include "../common/prog_info.h"

// Dlg_MC96FC664A dialog

IMPLEMENT_DYNAMIC(CDlg_MC96FC664A, CMFCPropertyPage)

CDlg_MC96FC664A::CDlg_MC96FC664A(struct chip_info_c *ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_MC96FC664A::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_MC96FC664A::~CDlg_MC96FC664A()
{
}

void CDlg_MC96FC664A::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_BSize);
	DDX_Control(pDX, IDC_COMBO2, m_SXIEN);
	DDX_Control(pDX, IDC_COMBO3, m_XIENA);
	DDX_Control(pDX, IDC_COMBO4, m_LockB);
	DDX_Control(pDX, IDC_COMBO5, m_LockP);
	DDX_Control(pDX, IDC_EDIT1, m_CFG0);
}


BEGIN_MESSAGE_MAP(CDlg_MC96FC664A, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_MC96FC664A::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_MC96FC664A::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_MC96FC664A::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_MC96FC664A::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_MC96FC664A::OnCbnSelchangeCombo)
END_MESSAGE_MAP()


// Dlg_MC96FC664A message handlers
BOOL CDlg_MC96FC664A::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	
	Option0 = 0x00;
	m_BSize.InsertString(m_BSize.GetCount(), L"00 = 1KB(0x0000-0x03FF)");
	m_BSize.InsertString(m_BSize.GetCount(), L"01 = 2KB(0x0000-0x07FF)");
	m_BSize.InsertString(m_BSize.GetCount(), L"10 = 4KB(0x0000-0x0FFF)");
	m_BSize.InsertString(m_BSize.GetCount(), L"11 = 8KB(0x0000-0x1FFF)");
	m_SXIEN.InsertString(m_SXIEN.GetCount(), L"0 = Disable");
	m_SXIEN.InsertString(m_SXIEN.GetCount(), L"1 = Enable");
	m_XIENA.InsertString(m_XIENA.GetCount(), L"0 = Disable");
	m_XIENA.InsertString(m_XIENA.GetCount(), L"1 = Enable");
	m_LockB.InsertString(m_LockB.GetCount(), L"0 = Disable");
	m_LockB.InsertString(m_LockB.GetCount(), L"1 = Enable");
	m_LockP.InsertString(m_LockP.GetCount(), L"0 = Disable");
	m_LockP.InsertString(m_LockP.GetCount(), L"1 = Enable");
	m_CFG0.SetWindowTextW(L"00");
	m_BSize.SetCurSel(0);
	m_SXIEN.SetCurSel(0);
	m_XIENA.SetCurSel(0);
	m_LockB.SetCurSel(0);
	m_LockP.SetCurSel(0);
	return TRUE;
}

BOOL CDlg_MC96FC664A::OnSetActive()
{
	if (*m_pDataLen != NULL)
	{
		CString Cfg0;
		memcpy((void*)&Option0, m_RegBuff, sizeof(Option0));
		Cfg0.Format(_T("%02X"), Option0);
		m_CFG0.SetWindowTextW(Cfg0);
		//Boot Area Size
		if ((Option0 & 0xC0) == 0xC0)
			m_BSize.SetCurSel(3);
		else if ((Option0 & 0xC0) == 0x80)
			m_BSize.SetCurSel(2);
		else if ((Option0 & 0xC0) == 0x40)
			m_BSize.SetCurSel(1);
		else if ((Option0 & 0xC0) == 0x00)
			m_BSize.SetCurSel(0);
		//Enable Sub-Oscillation
		if ((Option0 & 0x20) == 0x20)
			m_SXIEN.SetCurSel(1);
		else if ((Option0 & 0x20) == 0x00)
			m_SXIEN.SetCurSel(0);
		//Enable Main-Oscillation
		if ((Option0 & 0x10) == 0x10)
			m_XIENA.SetCurSel(1);
		else if ((Option0 & 0x10) == 0x10)
			m_XIENA.SetCurSel(0);
		//Write Protection(Boot Area)
		if ((Option0 & 0x02) == 0x02)
			m_LockB.SetCurSel(1);
		else if ((Option0 & 0x02) == 0x00)
			m_LockB.SetCurSel(0);
		//Read Protection(Code)
		if ((Option0 & 0x01) == 0x01)
			m_LockP.SetCurSel(1);
		else if ((Option0 & 0x01) == 0x00)
			m_LockP.SetCurSel(0);
	}
	return CMFCPropertyPage::OnSetActive();
}

void CDlg_MC96FC664A::OnCbnSelchangeCombo()
{
	CString Cfg0;
	Option0 = 0x00;
	//Boot Area Size
	if (m_BSize.GetCurSel() == 3)
		Option0 |= 0xC0;
	else if (m_BSize.GetCurSel() == 2)
		Option0 |= 0x80;
	else if (m_BSize.GetCurSel() == 1)
		Option0 |= 0x40;
	else if (m_BSize.GetCurSel() == 0)
		Option0 |= 0x00;
	Cfg0.Format(_T("%02X"), Option0);
	m_CFG0.SetWindowTextW(Cfg0);
	//Enable Sub-oscillation
	if (m_SXIEN.GetCurSel() == 1)
		Option0 |= 0x20;
	else if (m_SXIEN.GetCurSel() == 0)
		Option0 |= 0x00;
	Cfg0.Format(_T("%02X"), Option0);
	m_CFG0.SetWindowTextW(Cfg0);
	//Enable Main-oscilation
	if (m_XIENA.GetCurSel() == 1)
		Option0 |= 0x10;
	else if (m_XIENA.GetCurSel() == 0)
		Option0 |= 0x00;
	Cfg0.Format(_T("%02X"), Option0);
	m_CFG0.SetWindowTextW(Cfg0);
	//Write Protection (Boot¡@Area)
	if (m_LockB.GetCurSel() == 1)
		Option0 |= 0x02;
	else if (m_LockB.GetCurSel() == 0)
		Option0 |= 0x00;
	Cfg0.Format(_T("%02X"), Option0);
	m_CFG0.SetWindowTextW(Cfg0);
	//Read Protection (Code)
	if (m_LockP.GetCurSel() == 1)
		Option0 |= 0x01;
	else if (m_LockP.GetCurSel() == 0)
		Option0 |= 0x00;
	Cfg0.Format(_T("%02X"), Option0);
	m_CFG0.SetWindowTextW(Cfg0);
}

void CDlg_MC96FC664A::OnOK()
{
	CString temp;
	m_CFG0.GetWindowTextW(temp);
	Option0 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff, &Option0, sizeof(Option0));
	*m_pDataLen = sizeof(Option0);
	CMFCPropertyPage::OnOK();
}

LRESULT CDlg_MC96FC664A::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();

}
LRESULT CDlg_MC96FC664A::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}
