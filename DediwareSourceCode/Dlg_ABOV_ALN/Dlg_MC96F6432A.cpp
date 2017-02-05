// Dlg_MC96F6432A.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_MC96F6432A.h"
#include "afxdialogex.h"
#include "../common/prog_info.h"
#include "numeric_conversion.h"

// Dlg_MC96F6432A dialog

IMPLEMENT_DYNAMIC(CDlg_MC96F6432A, CMFCPropertyPage)

CDlg_MC96F6432A::CDlg_MC96F6432A(struct chip_info_c *ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_MC96F6432A::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_MC96F6432A::~CDlg_MC96F6432A()
{
}

void CDlg_MC96F6432A::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_PAEN);
	DDX_Control(pDX, IDC_COMBO2, m_PASS);
	DDX_Control(pDX, IDC_COMBO3, m_RP);
	DDX_Control(pDX, IDC_COMBO4, m_HL);
	DDX_Control(pDX, IDC_COMBO5, m_VAPEN);
	DDX_Control(pDX, IDC_COMBO6, m_RSTS);
	DDX_Control(pDX, IDC_EDIT1, m_CFG1);
	DDX_Control(pDX, IDC_EDIT2, m_CFG2);
}


BEGIN_MESSAGE_MAP(CDlg_MC96F6432A, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_MC96F6432A::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_MC96F6432A::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_MC96F6432A::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_MC96F6432A::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_MC96F6432A::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDlg_MC96F6432A::OnCbnSelchangeCombo)
END_MESSAGE_MAP()


// Dlg_MC96F6432A message handlers
BOOL CDlg_MC96F6432A::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	Cfg1 = 0x00;
	Cfg2 = 0x00;
	
	m_PAEN.InsertString(m_PAEN.GetCount(), L"0 = Disable Protection");
	m_PAEN.InsertString(m_PAEN.GetCount(), L"1 = Enable Protection");

	m_PASS.InsertString(m_PASS.GetCount(), L"000 = 0.7KBytes(0100H-02FFH)");
	m_PASS.InsertString(m_PASS.GetCount(), L"001 = 1.7KBytes(0100H-07FFH)");
	m_PASS.InsertString(m_PASS.GetCount(), L"010 = 2.7KBytes(0100H-0BFFH)");
	m_PASS.InsertString(m_PASS.GetCount(), L"011 = 3.7KBytes(0100H-0FFFH)");
	m_PASS.InsertString(m_PASS.GetCount(), L"100 = 29.7KBytes(0100H-77FFH)");
	m_PASS.InsertString(m_PASS.GetCount(), L"101 = 30.7KBytes(0100H-7BFFH)");
	m_PASS.InsertString(m_PASS.GetCount(), L"110 = 31.2KBytes(0100H-7DFFH)");
	m_PASS.InsertString(m_PASS.GetCount(), L"111 = 31.5KBytes(0100H-7EFFH)");

	m_RP.InsertString(m_RP.GetCount(), L"0 = Disable Read Protection");
	m_RP.InsertString(m_RP.GetCount(), L"1 = Enable Read Protection");

	m_HL.InsertString(m_HL.GetCount(), L"0 = Disable Hard-Lock");
	m_HL.InsertString(m_HL.GetCount(), L"1 = Enable Hard-Lock");

	m_VAPEN.InsertString(m_VAPEN.GetCount(), L"0 = Disable Protection");
	m_VAPEN.InsertString(m_VAPEN.GetCount(), L"1 = Enable Protection");

	m_RSTS.InsertString(m_RSTS.GetCount(), L"0 = Disable RST input");
	m_RSTS.InsertString(m_RSTS.GetCount(), L"1 =Enable RST input");

	m_CFG1.SetWindowTextW(L"00");
	m_CFG2.SetWindowTextW(L"00");
	m_PAEN.SetCurSel(0);
	m_PASS.SetCurSel(0);
	m_RP.SetCurSel(0);
	m_HL.SetCurSel(0);
	m_VAPEN.SetCurSel(0);
	m_RSTS.SetCurSel(0);


	return TRUE;
}

BOOL CDlg_MC96F6432A::OnSetActive()
{
	if (*m_pDataLen != NULL)
	{
		CString Cfg1_str;
		CString Cfg2_str;

		memcpy((void*)&Cfg1, m_RegBuff, sizeof(Cfg1));
		Cfg1_str.Format(_T("%02X"), Cfg1);
		m_CFG1.SetWindowTextW(Cfg1_str);

		memcpy((void*)&Cfg2, m_RegBuff + sizeof(Cfg1), sizeof(Cfg2));
		Cfg2_str.Format(_T("%02X"), Cfg2);
		m_CFG2.SetWindowTextW(Cfg2_str);

		//Protection Area Enable
		if ((Cfg1 & 0x08) == 0x08)
			m_PAEN.SetCurSel(1);
		else if ((Cfg1 & 0x08) == 0x00)
			m_PAEN.SetCurSel(0);
		//Protection Area Size
		if ((Cfg1 & 0x07) == 0x07)
			m_PASS.SetCurSel(7);
		else if ((Cfg1 & 0x07) == 0x06)
			m_PASS.SetCurSel(6);
		else if ((Cfg1 & 0x07) == 0x05)
			m_PASS.SetCurSel(5);
		else if ((Cfg1 & 0x07) == 0x04)
			m_PASS.SetCurSel(4);
		else if ((Cfg1 & 0x07) == 0x03)
			m_PASS.SetCurSel(3);
		else if ((Cfg1 & 0x07) == 0x02)
			m_PASS.SetCurSel(2);
		else if ((Cfg1 & 0x07) == 0x01)
			m_PASS.SetCurSel(1);
		else if ((Cfg1 & 0x07) == 0x00)
			m_PASS.SetCurSel(0);
		//Read Protection
		if ((Cfg2 & 0x80) == 0x80)
			m_RP.SetCurSel(1);
		else if ((Cfg2 & 0x80) == 0x00)
			m_RP.SetCurSel(0);
		//Hard-Lock
		if ((Cfg2 & 0x40) == 0x40)
			m_HL.SetCurSel(1);
		else if ((Cfg2 & 0x40) == 0x00)
			m_HL.SetCurSel(0);
		//Vector Area Protection
		if ((Cfg2 & 0x10) == 0x10)
			m_VAPEN.SetCurSel(1);
		else if ((Cfg2 & 0x10) == 0x00)
			m_VAPEN.SetCurSel(0);
		//Enable Reset
		if ((Cfg2 & 0x01) == 0x01)
			m_RSTS.SetCurSel(1);
		else if ((Cfg2 & 0x01) == 0x00)
			m_RSTS.SetCurSel(0);

	}
	return CMFCPropertyPage::OnSetActive();
}
void CDlg_MC96F6432A::OnCbnSelchangeCombo()
{
	CString Cfg1_str;
	CString Cfg2_str;
	Cfg1 = 0x00;
	Cfg2 = 0x00;

	//Protection Area Enable
	if (m_PAEN.GetCurSel() == 1)
		Cfg1 |= 0x08;
	else if (m_PAEN.GetCurSel() == 0)
		Cfg1 |= 0x00;
	Cfg1_str.Format(_T("%02X"), Cfg1);
	m_CFG1.SetWindowTextW(Cfg1_str);
	//Protection Area Size
	if (m_PASS.GetCurSel() == 7)
		Cfg1 |= 0x07;
	else if (m_PASS.GetCurSel() == 6)
		Cfg1 |= 0x06;
	else if (m_PASS.GetCurSel() == 5)
		Cfg1 |= 0x05;
	else if (m_PASS.GetCurSel() == 4)
		Cfg1 |= 0x04;
	else if (m_PASS.GetCurSel() == 3)
		Cfg1 |= 0x03;
	else if (m_PASS.GetCurSel() == 2)
		Cfg1 |= 0x02;
	else if (m_PASS.GetCurSel() == 1)
		Cfg1 |= 0x01;
	else if (m_PASS.GetCurSel() == 0)
		Cfg1 |= 0x00;
	Cfg1_str.Format(_T("%02X"), Cfg1);
	m_CFG1.SetWindowTextW(Cfg1_str);

	//Read Protection
	if (m_RP.GetCurSel() == 1)
		Cfg2 |= 0x80;
	else if (m_RP.GetCurSel() == 0)
		Cfg2 |= 0x00;
	Cfg2_str.Format(_T("%02X"), Cfg2);
	m_CFG2.SetWindowTextW(Cfg2_str);

	//Hard Lock
	if (m_HL.GetCurSel() == 1)
		Cfg2 |= 0x40;
	else if (m_HL.GetCurSel() == 0)
		Cfg2 |= 0x00;
	Cfg2_str.Format(_T("%02X"), Cfg2);
	m_CFG2.SetWindowTextW(Cfg2_str);
	
	//Vector Area Protection
	if (m_VAPEN.GetCurSel() == 1)
		Cfg2 |= 0x10;
	else if (m_VAPEN.GetCurSel() == 0)
		Cfg2 |= 0x00;
	Cfg2_str.Format(_T("%02X"), Cfg2);
	m_CFG2.SetWindowTextW(Cfg2_str);

	//Enable Reset
	if (m_RSTS.GetCurSel() == 1)
		Cfg2 |= 0x01;
	else if (m_RSTS.GetCurSel() == 0)
		Cfg2 |= 0x00;
	Cfg2_str.Format(_T("%02X"), Cfg2);
	m_CFG2.SetWindowTextW(Cfg2_str);
}
void CDlg_MC96F6432A::OnOK()
{
	CString temp;
	m_CFG1.GetWindowTextW(temp);
	Cfg1 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff, &Cfg1, sizeof(Cfg1));
	m_CFG2.GetWindowTextW(temp);
	Cfg2 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff + sizeof(Cfg1), &Cfg2, sizeof(Cfg2));
	unsigned short int fixint = 0x0000;
	memcpy(m_RegBuff + sizeof(Cfg1)+sizeof(Cfg2), &fixint, sizeof(fixint));

	*m_pDataLen = m_ChipInfo->partition_info[1].size_in_bytes;
	CMFCPropertyPage::OnOK();
}

LRESULT CDlg_MC96F6432A::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();

}
LRESULT CDlg_MC96F6432A::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}
