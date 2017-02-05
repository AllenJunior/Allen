// Dlg_ATmegaHVA.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_ATmegaHVA.h"
#include "afxdialogex.h"
#include "numeric_conversion.h"
#include "../common/prog_info.h"


// Dlg_ATmegaHVA dialog

IMPLEMENT_DYNAMIC(CDlg_ATmegaHVA, CMFCPropertyPage)

CDlg_ATmegaHVA::CDlg_ATmegaHVA(struct chip_info_c *ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_ATmegaHVA::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_ATmegaHVA::~CDlg_ATmegaHVA()
{
}

void CDlg_ATmegaHVA::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_option_enable);
	DDX_Control(pDX, IDC_COMBO1, m_CKSEL);
	DDX_Control(pDX, IDC_COMBO2, m_WDTON);
	DDX_Control(pDX, IDC_COMBO3, m_EESAVE);
	DDX_Control(pDX, IDC_COMBO4, m_SPIEN);
	DDX_Control(pDX, IDC_COMBO5, m_DWEN);
	DDX_Control(pDX, IDC_COMBO6, m_SELFPRGEN);
	DDX_Control(pDX, IDC_COMBO7, m_SUT);
	DDX_Control(pDX, IDC_COMBO8, m_LB);
	DDX_Control(pDX, IDC_EDIT1, m_HFuse);
	DDX_Control(pDX, IDC_EDIT2, m_LFuse);
	DDX_Control(pDX, IDC_EDIT3, m_Lockbits);
}


BEGIN_MESSAGE_MAP(CDlg_ATmegaHVA, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_ATmegaHVA::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_ATmegaHVA::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_ATmegaHVA::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_ATmegaHVA::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_ATmegaHVA::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDlg_ATmegaHVA::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDlg_ATmegaHVA::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CDlg_ATmegaHVA::OnCbnSelchangeCombo)
	ON_BN_CLICKED(IDC_CHECK1, &CDlg_ATmegaHVA::OnBnClickedEnable)
END_MESSAGE_MAP()


// Dlg_ATmegaHVA message handlers
BOOL CDlg_ATmegaHVA::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	HighFuse = 0xFD;
	LowFuse = 0xDF;
	Lockbits = 0xFF;
	m_HFuse.SetWindowTextW(L"FD");
	m_LFuse.SetWindowTextW(L"DF");
	m_Lockbits.SetWindowTextW(L"FF");
	//Fuse High Byte
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00 = Reserved");
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01 = Int.RC OSC 8MHz");
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10 = Reserved");
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11 = Reserved");
	//Fuse Low byte
	m_WDTON.InsertString(m_WDTON.GetCount(), L"0 = Enable Watchdog Timer always on");
	m_WDTON.InsertString(m_WDTON.GetCount(), L"1 = Disable Watchdog Timer always on");
	m_EESAVE.InsertString(m_EESAVE.GetCount(), L"0 = Preserve EEPROM during Chip Erase");
	m_EESAVE.InsertString(m_EESAVE.GetCount(), L"1 = Unreserves EEPROM during Chip Erase");
	m_SPIEN.InsertString(m_SPIEN.GetCount(), L"0 = Enabled ISP Programming");
	m_SPIEN.InsertString(m_SPIEN.GetCount(), L"1 = Disabled ISP Programming");
	m_DWEN.InsertString(m_DWEN.GetCount(), L"0 = debugWIRE Enable");
	m_DWEN.InsertString(m_DWEN.GetCount(), L"1 = debugWIRE Disable");
	m_SELFPRGEN.InsertString(m_SELFPRGEN.GetCount(), L"0 = Self Programming Enable");
	m_SELFPRGEN.InsertString(m_SELFPRGEN.GetCount(), L"1 = Self Programming Disable");
	m_SUT.InsertString(m_SUT.GetCount(), L"000 = 6CK/14CK + 4ms");
	m_SUT.InsertString(m_SUT.GetCount(), L"001 = 6CK/14CK + 8ms");
	m_SUT.InsertString(m_SUT.GetCount(), L"010 = 6CK/14CK + 16ms");
	m_SUT.InsertString(m_SUT.GetCount(), L"011 = 6CK/14CK + 32ms");
	m_SUT.InsertString(m_SUT.GetCount(), L"100 = 6CK/14CK + 64ms");
	m_SUT.InsertString(m_SUT.GetCount(), L"101 = 6CK/14CK + 128ms");
	m_SUT.InsertString(m_SUT.GetCount(), L"110 = 6CK/14CK + 256ms");
	m_SUT.InsertString(m_SUT.GetCount(), L"111 = 6CK/14CK + 512ms");
	//Lockbits
	m_LB.InsertString(m_LB.GetCount(), L"00 = Further Programming and verification Disabled");
	m_LB.InsertString(m_LB.GetCount(), L"10 = Further Programming Disabled");
	m_LB.InsertString(m_LB.GetCount(), L"11 = No memory Lock features enabled");

	m_option_enable.SetCheck(0);
	m_CKSEL.SetCurSel(1);
	m_WDTON.SetCurSel(1);
	m_EESAVE.SetCurSel(1);
	m_SPIEN.SetCurSel(0);
	m_DWEN.SetCurSel(1);
	m_SELFPRGEN.SetCurSel(1);
	m_SUT.SetCurSel(7);
	m_LB.SetCurSel(2);

	m_CKSEL.EnableWindow(FALSE);
	m_WDTON.EnableWindow(FALSE);
	m_EESAVE.EnableWindow(FALSE);
	m_SPIEN.EnableWindow(FALSE);
	m_DWEN.EnableWindow(FALSE);
	m_SELFPRGEN.EnableWindow(FALSE);
	m_SUT.EnableWindow(FALSE);
	m_LB.EnableWindow(FALSE);

	return TRUE;
}
BOOL CDlg_ATmegaHVA::OnSetActive()
{
	if (*m_pDataLen != NULL)
	{
		m_option_enable.SetCheck(1);
		m_CKSEL.EnableWindow(TRUE);
		m_WDTON.EnableWindow(TRUE);
		m_EESAVE.EnableWindow(TRUE);
		m_SPIEN.EnableWindow(FALSE);
		m_DWEN.EnableWindow(TRUE);
		m_SELFPRGEN.EnableWindow(TRUE);
		m_SUT.EnableWindow(TRUE);
		m_LB.EnableWindow(TRUE);
		CString HF_str, LF_str, L_str;
		//HighFuse
		memcpy((void*)&HighFuse, m_RegBuff, sizeof(HighFuse));
		HF_str.Format(_T("%02X"), HighFuse);
		m_HFuse.SetWindowTextW(HF_str);
		//LowFuse
		memcpy((void*)&LowFuse, m_RegBuff + sizeof(HighFuse), sizeof(LowFuse));
		LF_str.Format(_T("%02X"), LowFuse);
		m_LFuse.SetWindowTextW(LF_str);
		//Lockbits
		memcpy((void*)&Lockbits, m_RegBuff + sizeof(HighFuse)+sizeof(LowFuse), sizeof(Lockbits));
		L_str.Format(_T("%02X"), Lockbits);
		m_Lockbits.SetWindowTextW(L_str);
		//CKSEL
		if ((HighFuse & 0x03) == 0x03)
			m_CKSEL.SetCurSel(3);
		else if ((HighFuse & 0x02) == 0x02)
			m_CKSEL.SetCurSel(2);
		else if ((HighFuse & 0x01) == 0x01)
			m_CKSEL.SetCurSel(1);
		else if ((HighFuse & 0x00) == 0x00)
			m_CKSEL.SetCurSel(0);
		//WDTON
		if ((LowFuse & 0x80) == 0x80)
			m_WDTON.SetCurSel(1);
		else if ((LowFuse & 0x80) == 0x00)
			m_WDTON.SetCurSel(0);
		//EESAVE
		if ((LowFuse & 0x40) == 0x40)
			m_EESAVE.SetCurSel(1);
		else if ((LowFuse & 0x40) == 0x00)
			m_EESAVE.SetCurSel(0);
		//SPIEN
		if ((LowFuse & 0x20) == 0x20)
			m_SPIEN.SetCurSel(1);
		else if ((LowFuse & 0x20) == 0x00)
			m_SPIEN.SetCurSel(0);
		//DWEN
		if ((LowFuse & 0x10) == 0x10)
			m_DWEN.SetCurSel(1);
		else if ((LowFuse & 0x10) == 0x00)
			m_DWEN.SetCurSel(0);
		//SELFPRGEN
		if ((LowFuse & 0x08) == 0x08)
			m_SELFPRGEN.SetCurSel(1);
		else if ((LowFuse & 0x08) == 0x00)
			m_SELFPRGEN.SetCurSel(0);
		//SUT[2:0]
		if ((LowFuse & 0x07) == 0x07)
			m_SUT.SetCurSel(7);
		else if ((LowFuse & 0x07) == 0x06)
			m_SUT.SetCurSel(6);
		else if ((LowFuse & 0x07) == 0x05)
			m_SUT.SetCurSel(5);
		else if ((LowFuse & 0x07) == 0x04)
			m_SUT.SetCurSel(4);
		else if ((LowFuse & 0x07) == 0x03)
			m_SUT.SetCurSel(3);
		else if ((LowFuse & 0x07) == 0x02)
			m_SUT.SetCurSel(2);
		else if ((LowFuse & 0x07) == 0x01)
			m_SUT.SetCurSel(1);
		else if ((LowFuse & 0x07) == 0x00)
			m_SUT.SetCurSel(0);
		//LB
		if ((Lockbits & 0x03) == 0x03)
			m_LB.SetCurSel(2);
		else if((Lockbits & 0x03) == 0x02)
			m_LB.SetCurSel(1);
		else if ((Lockbits & 0x03) == 0x00)
			m_LB.SetCurSel(0);
	}
	else if (*m_pDataLen == NULL)
	{
		m_option_enable.SetCheck(0);
		m_CKSEL.EnableWindow(FALSE);
		m_WDTON.EnableWindow(FALSE);
		m_EESAVE.EnableWindow(FALSE);
		m_SPIEN.EnableWindow(FALSE);
		m_DWEN.EnableWindow(FALSE);
		m_SELFPRGEN.EnableWindow(FALSE);
		m_SUT.EnableWindow(FALSE);
		m_LB.EnableWindow(FALSE);
	}
	return CMFCPropertyPage::OnSetActive();
}
void CDlg_ATmegaHVA::OnCbnSelchangeCombo()
{
	CString HFuse_str, LFuse_str, Lockbits_str;
	HighFuse = 0xFD;
	LowFuse = 0xDF;
	Lockbits = 0xFF;

	//CKSEL
	HighFuse &= 0xFC;
	if (m_CKSEL.GetCurSel() == 0)
		HighFuse |= 0x00;
	else if (m_CKSEL.GetCurSel() == 1)
		HighFuse |= 0x01;
	else if (m_CKSEL.GetCurSel() == 2)
		HighFuse |= 0x02;
	else if (m_CKSEL.GetCurSel() == 3)
		HighFuse |= 0x03;
	HFuse_str.Format(_T("%02X"), HighFuse);
	m_HFuse.SetWindowTextW(HFuse_str);
	//WDTON
	LowFuse &= 0x7F;
	if (m_WDTON.GetCurSel() == 0)
		LowFuse |= 0x00;
	else if (m_WDTON.GetCurSel() == 1)
		LowFuse |= 0x80;
	LFuse_str.Format(_T("%02X"), LowFuse);
	m_LFuse.SetWindowTextW(LFuse_str);
	//EESAVE
	LowFuse &= 0xBF;
	if (m_EESAVE.GetCurSel() == 0)
		LowFuse |= 0x00;
	else if (m_EESAVE.GetCurSel() == 1)
		LowFuse |= 0x40;
	LFuse_str.Format(_T("%02X"), LowFuse);
	m_LFuse.SetWindowTextW(LFuse_str);
	//SPIEN
	LowFuse &= 0xDF;
	if (m_SPIEN.GetCurSel() == 0)
		LowFuse |= 0x00;
	else if (m_SPIEN.GetCurSel() == 1)
		LowFuse |= 0x20;
	LFuse_str.Format(_T("%02X"), LowFuse);
	m_LFuse.SetWindowTextW(LFuse_str);
	//DWEN
	LowFuse &= 0xEF;
	if (m_DWEN.GetCurSel() == 0)
		LowFuse |= 0x00;
	else if (m_DWEN.GetCurSel() == 1)
		LowFuse |= 0x10;
	LFuse_str.Format(_T("%02X"), LowFuse);
	m_LFuse.SetWindowTextW(LFuse_str);
	//SLEFPRGEN
	LowFuse &= 0xF7;
	if (m_SELFPRGEN.GetCurSel() == 0)
		LowFuse |= 0x00;
	else if (m_SELFPRGEN.GetCurSel() == 1)
		LowFuse |= 0x08;
	LFuse_str.Format(_T("%02X"), LowFuse);
	m_LFuse.SetWindowTextW(LFuse_str);
	//SUT[2:0}
	LowFuse &= 0xF8;
	if (m_SUT.GetCurSel() == 0)
		LowFuse |= 0x00;
	else if (m_SUT.GetCurSel() == 1)
		LowFuse |= 0x01;
	else if (m_SUT.GetCurSel() == 2)
		LowFuse |= 0x02;
	else if (m_SUT.GetCurSel() == 3)
		LowFuse |= 0x03;
	else if (m_SUT.GetCurSel() == 4)
		LowFuse |= 0x04;
	else if (m_SUT.GetCurSel() == 5)
		LowFuse |= 0x05;
	else if (m_SUT.GetCurSel() == 6)
		LowFuse |= 0x06;
	else if (m_SUT.GetCurSel() == 7)
		LowFuse |= 0x07;
	LFuse_str.Format(_T("%02X"), LowFuse);
	m_LFuse.SetWindowTextW(LFuse_str);
	//LB
	Lockbits &= 0xFC;
	if (m_LB.GetCurSel() == 2)
		Lockbits |= 0x03;
	else if (m_LB.GetCurSel() == 1)
		Lockbits |= 0x02;
	else if (m_LB.GetCurSel() == 0)
		Lockbits |= 0x00;
	Lockbits_str.Format(_T("%02X"), Lockbits);
	m_Lockbits.SetWindowTextW(Lockbits_str);
}

void CDlg_ATmegaHVA::OnOK()
{
	unsigned int state = m_option_enable.GetCheck();
	if (state == 1)
	{
		CString temp;
		m_HFuse.GetWindowTextW(temp);
		HighFuse = numeric_conversion::hexstring_to_size_t(temp.GetString());
		memcpy(m_RegBuff, &HighFuse, sizeof(HighFuse));

		m_LFuse.GetWindowTextW(temp);
		LowFuse = numeric_conversion::hexstring_to_size_t(temp.GetString());
		memcpy(m_RegBuff + sizeof(HighFuse), &LowFuse, sizeof(LowFuse));

		m_Lockbits.GetWindowTextW(temp);
		Lockbits = numeric_conversion::hexstring_to_size_t(temp.GetString());
		memcpy(m_RegBuff + sizeof(HighFuse)+sizeof(LowFuse), &Lockbits, sizeof(Lockbits));

		unsigned char fixint = 0x00;
		memcpy(m_RegBuff + sizeof(HighFuse)+sizeof(LowFuse)+sizeof(Lockbits), &fixint, sizeof(fixint));

		*m_pDataLen = sizeof(HighFuse)+sizeof(LowFuse)+sizeof(Lockbits)+sizeof(fixint);
	}
	else if (state == 0)
		*m_pDataLen = 0;
	CMFCPropertyPage::OnOK();
}

LRESULT CDlg_ATmegaHVA::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();

}
LRESULT CDlg_ATmegaHVA::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}

void CDlg_ATmegaHVA::OnBnClickedEnable()
{
	unsigned int state = m_option_enable.GetCheck();
	m_CKSEL.EnableWindow(state);
	m_WDTON.EnableWindow(state);
	m_EESAVE.EnableWindow(state);
	m_SPIEN.EnableWindow(FALSE);
	m_DWEN.EnableWindow(state);
	m_SELFPRGEN.EnableWindow(state);
	m_SUT.EnableWindow(state);
	m_LB.EnableWindow(state);
}