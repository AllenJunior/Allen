// Dlg_ATSAMD.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_ATSAMD.h"
#include "afxdialogex.h"
#include "numeric_conversion.h"
#include "../common/prog_info.h"

// Dlg_ATSAMD dialog

IMPLEMENT_DYNAMIC(CDlg_ATSAMD, CMFCPropertyPage)

CDlg_ATSAMD::CDlg_ATSAMD(struct chip_info_c *ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_ATSAMD::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_ATSAMD::~CDlg_ATSAMD()
{
}

void CDlg_ATSAMD::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, Option_enable);
	DDX_Control(pDX, IDC_CHECK2, Security_enable);
	DDX_Control(pDX, IDC_EDIT1, m_UW0);
	DDX_Control(pDX, IDC_EDIT2, m_UW1);
	DDX_Control(pDX, IDC_EDIT3, m_LOCKbits);
	DDX_Control(pDX, IDC_COMBO1, m_BOOTPROT);
	DDX_Control(pDX, IDC_COMBO2, m_EEPROM);
	DDX_Control(pDX, IDC_COMBO3, m_BODLEVEL);
	DDX_Control(pDX, IDC_COMBO4, m_BOD33Enable);
	DDX_Control(pDX, IDC_COMBO5, m_BODAction);
	DDX_Control(pDX, IDC_COMBO6, m_BODhyst);
	DDX_Control(pDX, IDC_COMBO7, m_WDTEnable);
	DDX_Control(pDX, IDC_COMBO8, m_WDTOn);
	DDX_Control(pDX, IDC_COMBO9, m_WDTPeriod);
	DDX_Control(pDX, IDC_COMBO10, m_WDTWindow);
	DDX_Control(pDX, IDC_COMBO11, m_WDTEWOFFSET);
	DDX_Control(pDX, IDC_COMBO12, m_WDTWEN);
}


BEGIN_MESSAGE_MAP(CDlg_ATSAMD, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_ATSAMD::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_ATSAMD::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_ATSAMD::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_ATSAMD::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_ATSAMD::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDlg_ATSAMD::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDlg_ATSAMD::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CDlg_ATSAMD::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO9, &CDlg_ATSAMD::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO10, &CDlg_ATSAMD::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO11, &CDlg_ATSAMD::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO12, &CDlg_ATSAMD::OnCbnSelchangeCombo)
	ON_EN_CHANGE(IDC_EDIT3, &CDlg_ATSAMD::OnEnChangeLockbits)
	ON_BN_CLICKED(IDC_CHECK1, &CDlg_ATSAMD::OnBnClickedEnable)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlg_ATSAMD::OnBnClickedKeyIn)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlg_ATSAMD::OnBnClickedSet)
END_MESSAGE_MAP()


// Dlg_ATSAMD message handlers
BOOL CDlg_ATSAMD::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	user_word_0 = 0xD9FEC7FF;
	user_word_1 = 0xFFFFFE5A;
	security = 0x00;
	Lockbits = 0xFFFF;
	m_UW0.SetWindowTextW(L"D9FEC7FF");
	m_UW1.SetWindowTextW(L"FFFFFE5A");
	m_LOCKbits.SetWindowTextW(L"FFFF");
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	Encheck->SetCheck(0);
	CButton* Security = (CButton*)GetDlgItem(IDC_CHECK2);
	Security->SetCheck(0);

	m_BOOTPROT.InsertString(m_BOOTPROT.GetCount(), L"000 = Bootloader size in 32768 bytes");
	m_BOOTPROT.InsertString(m_BOOTPROT.GetCount(), L"001 = Bootloader size in 16384 bytes");
	m_BOOTPROT.InsertString(m_BOOTPROT.GetCount(), L"010 = Bootloader size in 8192 bytes");
	m_BOOTPROT.InsertString(m_BOOTPROT.GetCount(), L"011 = Bootloader size in 4096 bytes");
	m_BOOTPROT.InsertString(m_BOOTPROT.GetCount(), L"100 = Bootloader size in 2048 bytes");
	m_BOOTPROT.InsertString(m_BOOTPROT.GetCount(), L"101 = Bootloader size in 1024 bytes");
	m_BOOTPROT.InsertString(m_BOOTPROT.GetCount(), L"110 = Bootloader size in 512 bytes");
	m_BOOTPROT.InsertString(m_BOOTPROT.GetCount(), L"111 = Bootloader size in 0 bytes");
	
	m_EEPROM.InsertString(m_EEPROM.GetCount(), L"000 = EEPROM size in 16384 Bytes");
	m_EEPROM.InsertString(m_EEPROM.GetCount(), L"001 = EEPROM size in 8192 Bytes");
	m_EEPROM.InsertString(m_EEPROM.GetCount(), L"010 = EEPROM size in 4096 Bytes");
	m_EEPROM.InsertString(m_EEPROM.GetCount(), L"011 = EEPROM size in 2048 Bytes");
	m_EEPROM.InsertString(m_EEPROM.GetCount(), L"100 = EEPROM size in 1024 Bytes");
	m_EEPROM.InsertString(m_EEPROM.GetCount(), L"101 = EEPROM size in 512 Bytes");
	m_EEPROM.InsertString(m_EEPROM.GetCount(), L"110 = EEPROM size in 256 Bytes");
	m_EEPROM.InsertString(m_EEPROM.GetCount(), L"111 = EEPROM size in 0 Bytes");

	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"0 = Typical 1.505V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"1 = Typical 1.54V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"2 = Typical 1.575V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"3 = Typical 1.61V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"4 = Typical 1.645V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"5 = Typical 1.68V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"6 = Typical 1.715V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"7 = Typical 1.750V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"8 = Typical 1.785V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"9 = Typical 1.82V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"10 = Typical 1.855V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"11 = Typical 1.89V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"12 = Typical 1.925V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"13 = Typical 1.96V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"14 = Typical 1.995V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"15 = Typical 2.03V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"16 = Typical 2.065V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"17 = Typical 2.1V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"18 = Typical 2.135V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"19 = Typical 2.17V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"20 = Typical 2.205V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"21 = Typical 2.24V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"22 = Typical 2.275V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"23 = Typical 2.31V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"24 = Typical 2.345V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"25 = Typical 2.38V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"26 = Typical 2.415V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"27 = Typical 2.45V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"28 = Typical 2.485V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"29 = Typical 2.52V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"30 = Typical 2.555V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"31 = Typical 2.59V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"32 = Typical 2.625V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"33 = Typical 2.66V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"34 = Typical 2.695V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"35 = Typical 2.73V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"36 = Typical 2.765V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"37 = Typical 2.79V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"38 = Typical 2.815V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"39 = Typical 2.84V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"40 = Typical 2.865V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"41 = Typical 2.899V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"42 = Typical 2.93V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"43 = Typical 2.967V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"44 = Typical 3.0V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"45 = Typical 3.04V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"46 = Typical 3.07V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"47 = Typical 3.104V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"48 = Typical 3.2V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"49 = Typical 3.234V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"50 = Typical 3.255V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"51 = Typical 3.26V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"52 = Typical 3.273V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"53 = Typical 3.31V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"54 = Typical 3.341V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"55 = Typical 3.37V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"56 = Typical 3.40V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"57 = Typical 3.443V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"58 = Typical 3.477V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"59 = Typical 3.511V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"60 = Typical 3.545V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"61 = Typical 3.592V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"62 = Typical 3.613V");
	m_BODLEVEL.InsertString(m_BODLEVEL.GetCount(), L"63 = Typical 3.64V");

	m_BOD33Enable.InsertString(m_BOD33Enable.GetCount(), L"0 = Enable");
	m_BOD33Enable.InsertString(m_BOD33Enable.GetCount(), L"1 = Disable");

	m_BODAction.InsertString(m_BODAction.GetCount(), L"00 = No Action");
	m_BODAction.InsertString(m_BODAction.GetCount(), L"01 = RESET");
	m_BODAction.InsertString(m_BODAction.GetCount(), L"10 = INTERRUPT");
	
	m_BODhyst.InsertString(m_BODhyst.GetCount(), L"0 = No Hysteresis");
	m_BODhyst.InsertString(m_BODhyst.GetCount(), L"1 = Hysteresis Enable");

	m_WDTEnable.InsertString(m_WDTEnable.GetCount(), L"0 = Enable");
	m_WDTEnable.InsertString(m_WDTEnable.GetCount(), L"1 = Disable");
	
	m_WDTOn.InsertString(m_WDTOn.GetCount(), L"0 = Enable");
	m_WDTOn.InsertString(m_WDTOn.GetCount(), L"1 = Disable");
	
	m_WDTPeriod.InsertString(m_WDTPeriod.GetCount(), L"0x00 = 8 clock cycles");
	m_WDTPeriod.InsertString(m_WDTPeriod.GetCount(), L"0x01 = 16 clock cycles");
	m_WDTPeriod.InsertString(m_WDTPeriod.GetCount(), L"0x02 = 32 clock cycles");
	m_WDTPeriod.InsertString(m_WDTPeriod.GetCount(), L"0x03 = 64 clock cycles");
	m_WDTPeriod.InsertString(m_WDTPeriod.GetCount(), L"0x04 = 128 clock cycles");
	m_WDTPeriod.InsertString(m_WDTPeriod.GetCount(), L"0x05 = 256 clock cycles");
	m_WDTPeriod.InsertString(m_WDTPeriod.GetCount(), L"0x06 = 512 clock cycles");
	m_WDTPeriod.InsertString(m_WDTPeriod.GetCount(), L"0x07 = 1024 clock cycles");
	m_WDTPeriod.InsertString(m_WDTPeriod.GetCount(), L"0x08 = 2048 clock cycles");
	m_WDTPeriod.InsertString(m_WDTPeriod.GetCount(), L"0x09 = 4096 clock cycles");
	m_WDTPeriod.InsertString(m_WDTPeriod.GetCount(), L"0x0A = 8192 clock cycles");
	m_WDTPeriod.InsertString(m_WDTPeriod.GetCount(), L"0x0B = 16384 clock cycles");
	
	m_WDTWindow.InsertString(m_WDTWindow.GetCount(), L"0x00 = 8 clock cycles");
	m_WDTWindow.InsertString(m_WDTWindow.GetCount(), L"0x01 = 16 clock cycles");
	m_WDTWindow.InsertString(m_WDTWindow.GetCount(), L"0x02 = 32 clock cycles");
	m_WDTWindow.InsertString(m_WDTWindow.GetCount(), L"0x03 = 64 clock cycles");
	m_WDTWindow.InsertString(m_WDTWindow.GetCount(), L"0x04 = 128 clock cycles");
	m_WDTWindow.InsertString(m_WDTWindow.GetCount(), L"0x05 = 256 clock cycles");
	m_WDTWindow.InsertString(m_WDTWindow.GetCount(), L"0x06 = 512 clock cycles");
	m_WDTWindow.InsertString(m_WDTWindow.GetCount(), L"0x07 = 1024 clock cycles");
	m_WDTWindow.InsertString(m_WDTWindow.GetCount(), L"0x08 = 2048 clock cycles");
	m_WDTWindow.InsertString(m_WDTWindow.GetCount(), L"0x09 = 4096 clock cycles");
	m_WDTWindow.InsertString(m_WDTWindow.GetCount(), L"0x0A = 8192 clock cycles");
	m_WDTWindow.InsertString(m_WDTWindow.GetCount(), L"0x0B = 16384 clock cycles");
	
	m_WDTEWOFFSET.InsertString(m_WDTEWOFFSET.GetCount(), L"0x00 = 8 clock cycles");
	m_WDTEWOFFSET.InsertString(m_WDTEWOFFSET.GetCount(), L"0x01 = 16 clock cycles");
	m_WDTEWOFFSET.InsertString(m_WDTEWOFFSET.GetCount(), L"0x02 = 32 clock cycles");
	m_WDTEWOFFSET.InsertString(m_WDTEWOFFSET.GetCount(), L"0x03 = 64 clock cycles");
	m_WDTEWOFFSET.InsertString(m_WDTEWOFFSET.GetCount(), L"0x04 = 128 clock cycles");
	m_WDTEWOFFSET.InsertString(m_WDTEWOFFSET.GetCount(), L"0x05 = 256 clock cycles");
	m_WDTEWOFFSET.InsertString(m_WDTEWOFFSET.GetCount(), L"0x06 = 512 clock cycles");
	m_WDTEWOFFSET.InsertString(m_WDTEWOFFSET.GetCount(), L"0x07 = 1024 clock cycles");
	m_WDTEWOFFSET.InsertString(m_WDTEWOFFSET.GetCount(), L"0x08 = 2048 clock cycles");
	m_WDTEWOFFSET.InsertString(m_WDTEWOFFSET.GetCount(), L"0x09 = 4096 clock cycles");
	m_WDTEWOFFSET.InsertString(m_WDTEWOFFSET.GetCount(), L"0x0A = 8192 clock cycles");
	m_WDTEWOFFSET.InsertString(m_WDTEWOFFSET.GetCount(), L"0x0B = 16384 clock cycles");
	
	m_WDTWEN.InsertString(m_WDTWEN.GetCount(), L"0 = Enable");
	m_WDTWEN.InsertString(m_WDTWEN.GetCount(), L"1 = Disable");

	m_BOOTPROT.SetCurSel(7);
	m_EEPROM.SetCurSel(7);
	m_BODLEVEL.SetCurSel(7);
	m_BOD33Enable.SetCurSel(1);
	m_BODAction.SetCurSel(1);
	m_BODhyst.SetCurSel(0);
	m_WDTEnable.SetCurSel(0);
	m_WDTOn.SetCurSel(0);
	m_WDTPeriod.SetCurSel(11);
	m_WDTWindow.SetCurSel(5);
	m_WDTEWOFFSET.SetCurSel(11);
	m_WDTWEN.SetCurSel(0);

	m_BOOTPROT.EnableWindow(FALSE);
	m_EEPROM.EnableWindow(FALSE);
	m_BODLEVEL.EnableWindow(FALSE);
	m_BOD33Enable.EnableWindow(FALSE);
	m_BODAction.EnableWindow(FALSE);
	m_BODhyst.EnableWindow(FALSE);
	m_WDTEnable.EnableWindow(FALSE);
	m_WDTOn.EnableWindow(FALSE);
	m_WDTPeriod.EnableWindow(FALSE);
	m_WDTWindow.EnableWindow(FALSE);
	m_WDTEWOFFSET.EnableWindow(FALSE);
	m_WDTWEN.EnableWindow(FALSE);
	m_LOCKbits.EnableWindow(FALSE);
	m_UW0.EnableWindow(FALSE);
	m_UW1.EnableWindow(FALSE);
	Security_enable.EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);

	return TRUE;
}
BOOL CDlg_ATSAMD::OnSetActive()
{
	if (*m_pDataLen != NULL)
	{
		m_BOOTPROT.EnableWindow(TRUE);
		m_EEPROM.EnableWindow(TRUE);
		m_BODLEVEL.EnableWindow(TRUE);
		m_BOD33Enable.EnableWindow(TRUE);
		m_BODAction.EnableWindow(TRUE);
		m_BODhyst.EnableWindow(TRUE);
		m_WDTEnable.EnableWindow(TRUE);
		m_WDTOn.EnableWindow(TRUE);
		m_WDTPeriod.EnableWindow(TRUE);
		m_WDTWindow.EnableWindow(TRUE);
		m_WDTEWOFFSET.EnableWindow(TRUE);
		m_WDTWEN.EnableWindow(TRUE);
		m_LOCKbits.EnableWindow(TRUE);
		Security_enable.EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
		//m_UW0.EnableWindow(TRUE);
		//m_UW1.EnableWindow(TRUE);
		
		CString uw0_str;
		CString uw1_str;
		CString lockbit_str;
		//Option Enable
		CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
		Encheck->SetCheck(1);
		//user
		memcpy((void*)&user_word_0, m_RegBuff, sizeof(user_word_0));
		uw0_str.Format(_T("%08X"), user_word_0);
		m_UW0.SetWindowTextW(uw0_str);
		memcpy((void*)&user_word_1, m_RegBuff + sizeof(user_word_0), sizeof(user_word_1));
		uw1_str.Format(_T("%08X"), user_word_1);
		m_UW1.SetWindowTextW(uw1_str);
		memcpy((void*)&security, m_RegBuff + sizeof(user_word_0)+sizeof(user_word_1), sizeof(security));
		//Security
		CButton* Security = (CButton*)GetDlgItem(IDC_CHECK2);
		if (security != 0)
			Security->SetCheck(1);
		else if (security == 0)
			Security->SetCheck(0);
		//BOOTPROT
		if ((user_word_0 & 0x00000007) == 0x0)
			m_BOOTPROT.SetCurSel(0);
		else if ((user_word_0 & 0x00000007) == 0x1)
			m_BOOTPROT.SetCurSel(1);
		else if ((user_word_0 & 0x00000007) == 0x2)
			m_BOOTPROT.SetCurSel(2);
		else if ((user_word_0 & 0x00000007) == 0x3)
			m_BOOTPROT.SetCurSel(3);
		else if ((user_word_0 & 0x00000007) == 0x4)
			m_BOOTPROT.SetCurSel(4);
		else if ((user_word_0 & 0x00000007) == 0x5)
			m_BOOTPROT.SetCurSel(5);
		else if ((user_word_0 & 0x00000007) == 0x6)
			m_BOOTPROT.SetCurSel(6);
		else if ((user_word_0 & 0x00000007) == 0x7)
			m_BOOTPROT.SetCurSel(7);
		//EEPROM
		if ((user_word_0 & 0x00000070) == 0x0)
			m_EEPROM.SetCurSel(0);
		else if ((user_word_0 & 0x00000070) == 0x10)
			m_EEPROM.SetCurSel(1);
		else if ((user_word_0 & 0x00000070) == 0x20)
			m_EEPROM.SetCurSel(2);
		else if ((user_word_0 & 0x00000070) == 0x30)
			m_EEPROM.SetCurSel(3);
		else if ((user_word_0 & 0x00000070) == 0x40)
			m_EEPROM.SetCurSel(4);
		else if ((user_word_0 & 0x00000070) == 0x50)
			m_EEPROM.SetCurSel(5);
		else if ((user_word_0 & 0x00000070) == 0x60)
			m_EEPROM.SetCurSel(6);
		else if ((user_word_0 & 0x00000070) == 0x70)
			m_EEPROM.SetCurSel(7);
		//BOD33Level
		if ((user_word_0 & 0x00003F00) == 0x0000)
			m_BODLEVEL.SetCurSel(0);
		else if ((user_word_0 & 0x00003F00) == 0x0100)
			m_BODLEVEL.SetCurSel(1);
		else if ((user_word_0 & 0x00003F00) == 0x0200)
			m_BODLEVEL.SetCurSel(2);
		else if ((user_word_0 & 0x00003F00) == 0x0300)
			m_BODLEVEL.SetCurSel(3);
		else if ((user_word_0 & 0x00003F00) == 0x0400)
			m_BODLEVEL.SetCurSel(4);
		else if ((user_word_0 & 0x00003F00) == 0x0500)
			m_BODLEVEL.SetCurSel(5);
		else if ((user_word_0 & 0x00003F00) == 0x0600)
			m_BODLEVEL.SetCurSel(6);
		else if ((user_word_0 & 0x00003F00) == 0x0700)
			m_BODLEVEL.SetCurSel(7);
		else if ((user_word_0 & 0x00003F00) == 0x0800)
			m_BODLEVEL.SetCurSel(8);
		else if ((user_word_0 & 0x00003F00) == 0x0900)
			m_BODLEVEL.SetCurSel(9);
		else if ((user_word_0 & 0x00003F00) == 0x0A00)
			m_BODLEVEL.SetCurSel(10);
		else if ((user_word_0 & 0x00003F00) == 0x0B00)
			m_BODLEVEL.SetCurSel(11);
		else if ((user_word_0 & 0x00003F00) == 0x0C00)
			m_BODLEVEL.SetCurSel(12);
		else if ((user_word_0 & 0x00003F00) == 0x0D00)
			m_BODLEVEL.SetCurSel(13);
		else if ((user_word_0 & 0x00003F00) == 0x0E00)
			m_BODLEVEL.SetCurSel(14);
		else if ((user_word_0 & 0x00003F00) == 0x0F00)
			m_BODLEVEL.SetCurSel(15);
		else if ((user_word_0 & 0x00003F00) == 0x1000)
			m_BODLEVEL.SetCurSel(16);
		else if ((user_word_0 & 0x00003F00) == 0x1100)
			m_BODLEVEL.SetCurSel(17);
		else if ((user_word_0 & 0x00003F00) == 0x1200)
			m_BODLEVEL.SetCurSel(18);
		else if ((user_word_0 & 0x00003F00) == 0x1300)
			m_BODLEVEL.SetCurSel(19);
		else if ((user_word_0 & 0x00003F00) == 0x1400)
			m_BODLEVEL.SetCurSel(20);
		else if ((user_word_0 & 0x00003F00) == 0x1500)
			m_BODLEVEL.SetCurSel(21);
		else if ((user_word_0 & 0x00003F00) == 0x1600)
			m_BODLEVEL.SetCurSel(22);
		else if ((user_word_0 & 0x00003F00) == 0x1700)
			m_BODLEVEL.SetCurSel(23);
		else if ((user_word_0 & 0x00003F00) == 0x1800)
			m_BODLEVEL.SetCurSel(24);
		else if ((user_word_0 & 0x00003F00) == 0x1900)
			m_BODLEVEL.SetCurSel(25);
		else if ((user_word_0 & 0x00003F00) == 0x1A00)
			m_BODLEVEL.SetCurSel(26);
		else if ((user_word_0 & 0x00003F00) == 0x1B00)
			m_BODLEVEL.SetCurSel(27);
		else if ((user_word_0 & 0x00003F00) == 0x1C00)
			m_BODLEVEL.SetCurSel(28);
		else if ((user_word_0 & 0x00003F00) == 0x1D00)
			m_BODLEVEL.SetCurSel(29);
		else if ((user_word_0 & 0x00003F00) == 0x1E00)
			m_BODLEVEL.SetCurSel(30);
		else if ((user_word_0 & 0x00003F00) == 0x1F00)
			m_BODLEVEL.SetCurSel(31);
		else if ((user_word_0 & 0x00003F00) == 0x2000)
			m_BODLEVEL.SetCurSel(32);
		else if ((user_word_0 & 0x00003F00) == 0x2100)
			m_BODLEVEL.SetCurSel(33);
		else if ((user_word_0 & 0x00003F00) == 0x2200)
			m_BODLEVEL.SetCurSel(34);
		else if ((user_word_0 & 0x00003F00) == 0x2300)
			m_BODLEVEL.SetCurSel(35);
		else if ((user_word_0 & 0x00003F00) == 0x2400)
			m_BODLEVEL.SetCurSel(36);
		else if ((user_word_0 & 0x00003F00) == 0x2500)
			m_BODLEVEL.SetCurSel(37);
		else if ((user_word_0 & 0x00003F00) == 0x2600)
			m_BODLEVEL.SetCurSel(38);
		else if ((user_word_0 & 0x00003F00) == 0x2700)
			m_BODLEVEL.SetCurSel(39);
		else if ((user_word_0 & 0x00003F00) == 0x2800)
			m_BODLEVEL.SetCurSel(40);
		else if ((user_word_0 & 0x00003F00) == 0x2900)
			m_BODLEVEL.SetCurSel(41);
		else if ((user_word_0 & 0x00003F00) == 0x2A00)
			m_BODLEVEL.SetCurSel(42);
		else if ((user_word_0 & 0x00003F00) == 0x2B00)
			m_BODLEVEL.SetCurSel(43);
		else if ((user_word_0 & 0x00003F00) == 0x2C00)
			m_BODLEVEL.SetCurSel(44);
		else if ((user_word_0 & 0x00003F00) == 0x2D00)
			m_BODLEVEL.SetCurSel(45);
		else if ((user_word_0 & 0x00003F00) == 0x2E00)
			m_BODLEVEL.SetCurSel(46);
		else if ((user_word_0 & 0x00003F00) == 0x2F00)
			m_BODLEVEL.SetCurSel(47);
		else if ((user_word_0 & 0x00003F00) == 0x3000)
			m_BODLEVEL.SetCurSel(48);
		else if ((user_word_0 & 0x00003F00) == 0x3100)
			m_BODLEVEL.SetCurSel(49);
		else if ((user_word_0 & 0x00003F00) == 0x3200)
			m_BODLEVEL.SetCurSel(50);
		else if ((user_word_0 & 0x00003F00) == 0x3300)
			m_BODLEVEL.SetCurSel(51);
		else if ((user_word_0 & 0x00003F00) == 0x3400)
			m_BODLEVEL.SetCurSel(52);
		else if ((user_word_0 & 0x00003F00) == 0x3500)
			m_BODLEVEL.SetCurSel(53);
		else if ((user_word_0 & 0x00003F00) == 0x3600)
			m_BODLEVEL.SetCurSel(54);
		else if ((user_word_0 & 0x00003F00) == 0x3700)
			m_BODLEVEL.SetCurSel(55);
		else if ((user_word_0 & 0x00003F00) == 0x3800)
			m_BODLEVEL.SetCurSel(56);
		else if ((user_word_0 & 0x00003F00) == 0x3900)
			m_BODLEVEL.SetCurSel(57);
		else if ((user_word_0 & 0x00003F00) == 0x3A00)
			m_BODLEVEL.SetCurSel(58);
		else if ((user_word_0 & 0x00003F00) == 0x3B00)
			m_BODLEVEL.SetCurSel(59);
		else if ((user_word_0 & 0x00003F00) == 0x3C00)
			m_BODLEVEL.SetCurSel(60);
		else if ((user_word_0 & 0x00003F00) == 0x3D00)
			m_BODLEVEL.SetCurSel(61);
		else if ((user_word_0 & 0x00003F00) == 0x3E00)
			m_BODLEVEL.SetCurSel(62);
		else if ((user_word_0 & 0x00003F00) == 0x3F00)
			m_BODLEVEL.SetCurSel(63);
		//BOD33 Enable
		if ((user_word_0 & 0x00004000) == 0x0)
			m_BOD33Enable.SetCurSel(0);
		else if ((user_word_0 & 0x00004000) == 0x4000)
			m_BOD33Enable.SetCurSel(1);
		//BOD33 Action
		if ((user_word_0 & 0x00018000) == 0x0)
			m_BODAction.SetCurSel(0);
		else if ((user_word_0 & 0x00018000) == 0x8000)
			m_BODAction.SetCurSel(1);
		else if ((user_word_0 & 0x00018000) == 0x10000)
			m_BODAction.SetCurSel(2);
		//WDT Enable
		if ((user_word_0 & 0x02000000) == 0x0)
			m_WDTEnable.SetCurSel(0);
		else if ((user_word_0 & 0x02000000) == 0x02000000)
			m_WDTEnable.SetCurSel(1);
		//WDT Always ON
		if ((user_word_0 & 0x04000000) == 0x0)
			m_WDTOn.SetCurSel(0);
		else if ((user_word_0 & 0x04000000) == 0x04000000)
			m_WDTOn.SetCurSel(1);
		//WDT Period
		if ((user_word_0 & 0x78000000) == 0x0)
			m_WDTPeriod.SetCurSel(0);
		else if ((user_word_0 & 0x78000000) == 0x08000000)
			m_WDTPeriod.SetCurSel(1);
		else if ((user_word_0 & 0x78000000) == 0x10000000)
			m_WDTPeriod.SetCurSel(2);
		else if ((user_word_0 & 0x78000000) == 0x18000000)
			m_WDTPeriod.SetCurSel(3);
		else if ((user_word_0 & 0x78000000) == 0x20000000)
			m_WDTPeriod.SetCurSel(4);
		else if ((user_word_0 & 0x78000000) == 0x28000000)
			m_WDTPeriod.SetCurSel(5);
		else if ((user_word_0 & 0x78000000) == 0x30000000)
			m_WDTPeriod.SetCurSel(6);
		else if ((user_word_0 & 0x78000000) == 0x38000000)
			m_WDTPeriod.SetCurSel(7);
		else if ((user_word_0 & 0x78000000) == 0x40000000)
			m_WDTPeriod.SetCurSel(8);
		else if ((user_word_0 & 0x78000000) == 0x48000000)
			m_WDTPeriod.SetCurSel(9);
		else if ((user_word_0 & 0x78000000) == 0x50000000)
			m_WDTPeriod.SetCurSel(10);
		else if ((user_word_0 & 0x78000000) == 0x58000000)
			m_WDTPeriod.SetCurSel(11);
		//WDT Window
		if ((user_word_0 & 0x80000000) == 0x0)
		{
			if((user_word_1 & 0x00000007) == 0x0)
				m_WDTWindow.SetCurSel(0);
			else if ((user_word_1 & 0x00000007) == 0x1)
				m_WDTWindow.SetCurSel(2);
			else if ((user_word_1 & 0x00000007) == 0x2)
				m_WDTWindow.SetCurSel(4);
			else if ((user_word_1 & 0x00000007) == 0x3)
				m_WDTWindow.SetCurSel(6);
			else if ((user_word_1 & 0x00000007) == 0x4)
				m_WDTWindow.SetCurSel(8);
			else if((user_word_1 & 0x00000007) == 0x5)
				m_WDTWindow.SetCurSel(10);
		}
		else if ((user_word_0 & 0x80000000) == 0x80000000)
		{
			if ((user_word_1 & 0x00000007) == 0x0)
				m_WDTWindow.SetCurSel(1);
			else if ((user_word_1 & 0x00000007) == 0x1)
				m_WDTWindow.SetCurSel(3);
			else if ((user_word_1 & 0x00000007) == 0x2)
				m_WDTWindow.SetCurSel(5);
			else if ((user_word_1 & 0x00000007) == 0x3)
				m_WDTWindow.SetCurSel(7);
			else if ((user_word_1 & 0x00000007) == 0x4)
				m_WDTWindow.SetCurSel(9);
			else if ((user_word_1 & 0x00000007) == 0x5)
				m_WDTWindow.SetCurSel(11);
		}
		//WDT EWOFFSET
		if ((user_word_1 & 0x00000078) == 0x0)
			m_WDTEWOFFSET.SetCurSel(0);
		else if ((user_word_1 & 0x00000078) == 0x08)
			m_WDTEWOFFSET.SetCurSel(1);
		else if ((user_word_1 & 0x00000078) == 0x10)
			m_WDTEWOFFSET.SetCurSel(2);
		else if ((user_word_1 & 0x00000078) == 0x18)
			m_WDTEWOFFSET.SetCurSel(3);
		else if ((user_word_1 & 0x00000078) == 0x20)
			m_WDTEWOFFSET.SetCurSel(4);
		else if ((user_word_1 & 0x00000078) == 0x28)
			m_WDTEWOFFSET.SetCurSel(5);
		else if ((user_word_1 & 0x00000078) == 0x30)
			m_WDTEWOFFSET.SetCurSel(6);
		else if ((user_word_1 & 0x00000078) == 0x38)
			m_WDTEWOFFSET.SetCurSel(7);
		else if ((user_word_1 & 0x00000078) == 0x40)
			m_WDTEWOFFSET.SetCurSel(8);
		else if ((user_word_1 & 0x00000078) == 0x48)
			m_WDTEWOFFSET.SetCurSel(9);
		else if ((user_word_1 & 0x00000078) == 0x50)
			m_WDTEWOFFSET.SetCurSel(10);
		else if ((user_word_1 & 0x00000078) == 0x58)
			m_WDTEWOFFSET.SetCurSel(11);
		//WDT WEN
		if ((user_word_1 & 0x00000080) == 0x0)
			m_WDTWEN.SetCurSel(0);
		else if ((user_word_1 & 0x00000080) == 0x80)
			m_WDTWEN.SetCurSel(1);
		//BOD33 hystersis
		if ((user_word_1 & 0x00000100) == 0x00)
			m_BODhyst.SetCurSel(0);
		else if ((user_word_1 & 0x00000100) == 0x0100)
			m_BODhyst.SetCurSel(1);
		//Lockbits
		int locktemp = (user_word_1 & 0xFFFF0000);
		Lockbits = (locktemp >> 16);
		lockbit_str.Format(_T("%4X"), Lockbits);
		m_LOCKbits.SetWindowTextW(lockbit_str);

		}
	else if (*m_pDataLen == NULL)
	{
		CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
		Encheck->SetCheck(0);
		m_BOOTPROT.EnableWindow(FALSE);
		m_EEPROM.EnableWindow(FALSE);
		m_BODLEVEL.EnableWindow(FALSE);
		m_BOD33Enable.EnableWindow(FALSE);
		m_BODAction.EnableWindow(FALSE);
		m_BODhyst.EnableWindow(FALSE);
		m_WDTEnable.EnableWindow(FALSE);
		m_WDTOn.EnableWindow(FALSE);
		m_WDTPeriod.EnableWindow(FALSE);
		m_WDTWindow.EnableWindow(FALSE);
		m_WDTEWOFFSET.EnableWindow(FALSE);
		m_WDTWEN.EnableWindow(FALSE);
		m_LOCKbits.EnableWindow(FALSE);
		Security_enable.EnableWindow(FALSE);
		//m_UW0.EnableWindow(FALSE);
		//m_UW1.EnableWindow(FALSE);
	}
	return CMFCPropertyPage::OnSetActive();
}

void CDlg_ATSAMD::OnCbnSelchangeCombo()
{
	CString UW0_str;
	CString UW1_str;
	user_word_0 = 0xFFFFFFFF;
	user_word_1 = 0xFFFFFFFF;
	//BOOTPROT
	user_word_0 &= 0xFFFFFFF8;
	switch (m_BOOTPROT.GetCurSel())
	{
	case 0:
		user_word_0 |= 0x0;
		break;
	case 1:
		user_word_0 |= 0x1;
		break;
	case 2:
		user_word_0 |= 0x2;
		break;
	case 3:
		user_word_0 |= 0x3;
		break;
	case 4:
		user_word_0 |= 0x4;
		break;
	case 5:
		user_word_0 |= 0x5;
		break;
	case 6:
		user_word_0 |= 0x6;
		break;
	case 7:
		user_word_0 |= 0x7;
		break;
	}
	UW0_str.Format(_T("%8X"), user_word_0);
	m_UW0.SetWindowTextW(UW0_str);

	//EEPROM
	user_word_0 &= 0xFFFFFF8F;
	switch (m_EEPROM.GetCurSel())
	{
	case 0:
		user_word_0 |= 0x0;
		break;
	case 1:
		user_word_0 |= 0x10;
		break;
	case 2:
		user_word_0 |= 0x20;
		break;
	case 3:
		user_word_0 |= 0x30;
		break;
	case 4:
		user_word_0 |= 0x40;
		break;
	case 5:
		user_word_0 |= 0x50;
		break;
	case 6:
		user_word_0 |= 0x60;
		break;
	case 7:
		user_word_0 |= 0x70;
		break;
	}
	UW0_str.Format(_T("%8X"), user_word_0);
	m_UW0.SetWindowTextW(UW0_str);

	//BOD 33 Level
	user_word_0 &= 0xFFFFC0FF;
	switch (m_BODLEVEL.GetCurSel())
	{
	case 0 :
		user_word_0 |= 0x0000;
		break;
	case 1 :
		user_word_0 |= 0x0100;
		break;
	case 2:
		user_word_0 |= 0x0200;
		break;
	case 3:
		user_word_0 |= 0x0300;
		break;
	case 4:
		user_word_0 |= 0x0400;
		break;
	case 5:
		user_word_0 |= 0x0500;
		break;
	case 6:
		user_word_0 |= 0x0600;
		break;
	case 7:
		user_word_0 |= 0x0700;
		break;
	case 8:
		user_word_0 |= 0x0800;
		break;
	case 9:
		user_word_0 |= 0x0900;
		break;
	case 10:
		user_word_0 |= 0x0A00;
		break;
	case 11:
		user_word_0 |= 0x0B00;
		break;
	case 12:
		user_word_0 |= 0x0C00;
		break;
	case 13:
		user_word_0 |= 0x0D00;
		break;
	case 14:
		user_word_0 |= 0x0E00;
		break;
	case 15:
		user_word_0 |= 0x0F00;
		break;
	case 16:
		user_word_0 |= 0x1000;
		break;
	case 17:
		user_word_0 |= 0x1100;
		break;
	case 18:
		user_word_0 |= 0x1200;
		break;
	case 19:
		user_word_0 |= 0x1300;
		break;
	case 20:
		user_word_0 |= 0x1400;
		break;
	case 21:
		user_word_0 |= 0x1500;
		break;
	case 22:
		user_word_0 |= 0x1600;
		break;
	case 23:
		user_word_0 |= 0x1700;
		break;
	case 24:
		user_word_0 |= 0x1800;
		break;
	case 25:
		user_word_0 |= 0x1900;
		break;
	case 26:
		user_word_0 |= 0x1A00;
		break;
	case 27:
		user_word_0 |= 0x1B00;
		break;
	case 28:
		user_word_0 |= 0x1C00;
		break;
	case 29:
		user_word_0 |= 0x1D00;
		break;
	case 30:
		user_word_0 |= 0x1E00;
		break;
	case 31:
		user_word_0 |= 0x1F00;
		break;
	case 32:
		user_word_0 |= 0x2000;
		break;
	case 33:
		user_word_0 |= 0x2100;
		break;
	case 34:
		user_word_0 |= 0x2200;
		break;
	case 35:
		user_word_0 |= 0x2300;
		break;
	case 36:
		user_word_0 |= 0x2400;
		break;
	case 37:
		user_word_0 |= 0x2500;
		break;
	case 38:
		user_word_0 |= 0x2600;
		break;
	case 39:
		user_word_0 |= 0x2700;
		break;
	case 40:
		user_word_0 |= 0x2800;
		break;
	case 41:
		user_word_0 |= 0x2900;
		break;
	case 42:
		user_word_0 |= 0x2A00;
		break;
	case 43:
		user_word_0 |= 0x2B00;
		break;
	case 44:
		user_word_0 |= 0x2C00;
		break;
	case 45:
		user_word_0 |= 0x2D00;
		break;
	case 46:
		user_word_0 |= 0x2E00;
		break;
	case 47:
		user_word_0 |= 0x2F00;
		break;
	case 48:
		user_word_0 |= 0x3000;
		break;
	case 49:
		user_word_0 |= 0x3100;
		break;
	case 50:
		user_word_0 |= 0x3200;
		break;
	case 51:
		user_word_0 |= 0x3300;
		break;
	case 52:
		user_word_0 |= 0x3400;
		break;
	case 53:
		user_word_0 |= 0x3500;
		break;
	case 54:
		user_word_0 |= 0x3600;
		break;
	case 55:
		user_word_0 |= 0x3700;
		break;
	case 56:
		user_word_0 |= 0x3800;
		break;
	case 57:
		user_word_0 |= 0x3900;
		break;
	case 58:
		user_word_0 |= 0x3A00;
		break;
	case 59:
		user_word_0 |= 0x3B00;
		break;
	case 60:
		user_word_0 |= 0x3C00;
		break;
	case 61:
		user_word_0 |= 0x3D00;
		break;
	case 62:
		user_word_0 |= 0x3E00;
		break;
	case 63:
		user_word_0 |= 0x3F00;
		break;
	}
	UW0_str.Format(_T("%8X"), user_word_0);
	m_UW0.SetWindowTextW(UW0_str);

	//BOD33 Enable
	user_word_0 &= 0xFFFFBFFF;
	if (m_BOD33Enable.GetCurSel() == 0)
		user_word_0 |= 0x0;
	else if (m_BOD33Enable.GetCurSel() == 1)
		user_word_0 |= 0x4000;
	UW0_str.Format(_T("%8X"), user_word_0);
	m_UW0.SetWindowTextW(UW0_str);

	//BOD33 Action
	user_word_0 &= 0xFFFE7FFF;
	if (m_BODAction.GetCurSel() == 0)
		user_word_0 |= 0x0;
	else if (m_BODAction.GetCurSel() == 1)
		user_word_0 |= 0x8000;
	else if (m_BODAction.GetCurSel() == 2)
		user_word_0 |= 0x10000;
	UW0_str.Format(_T("%8X"), user_word_0);
	m_UW0.SetWindowTextW(UW0_str);

	//WDT Enable
	user_word_0 &= 0xFDFFFFFF;
	if (m_WDTEnable.GetCurSel() == 0)
		user_word_0 |= 0x0;
	else if (m_WDTEnable.GetCurSel() == 1)
		user_word_0 |= 0x02000000;
	UW0_str.Format(_T("%8X"), user_word_0);
	m_UW0.SetWindowTextW(UW0_str);

	//WDT Always On
	user_word_0 &= 0xFBFFFFFF;
	if (m_WDTOn.GetCurSel() == 0)
		user_word_0 |= 0x0;
	else if (m_WDTOn.GetCurSel() == 1)
		user_word_0 |= 0x04000000;
	UW0_str.Format(_T("%8X"), user_word_0);
	m_UW0.SetWindowTextW(UW0_str);

	//WDT Period
	user_word_0 &= 0x87FFFFFF;
	switch (m_WDTPeriod.GetCurSel())
	{
	case 0:
		user_word_0 |= 0x0;
		break;
	case 1:
		user_word_0 |= 0x08000000;
		break;
	case 2:
		user_word_0 |= 0x10000000;
		break;
	case 3:
		user_word_0 |= 0x18000000;
		break;
	case 4:
		user_word_0 |= 0x20000000;
		break;
	case 5:
		user_word_0 |= 0x28000000;
		break;
	case 6:
		user_word_0 |= 0x30000000;
		break;
	case 7:
		user_word_0 |= 0x38000000;
		break;
	case 8:
		user_word_0 |= 0x40000000;
		break;
	case 9:
		user_word_0 |= 0x48000000;
		break;
	case 10:
		user_word_0 |= 0x50000000;
		break;
	case 11:
		user_word_0 |= 0x58000000;
		break;
	}
	UW0_str.Format(_T("%8X"), user_word_0);
	m_UW0.SetWindowTextW(UW0_str);

	//WDT Window
	user_word_0 &= 0x7FFFFFFF;
	user_word_1 &= 0xFFFFFFF8;
	switch (m_WDTWindow.GetCurSel())
	{
	case 0:
		user_word_0 |= 0x0;
		user_word_1 |= 0x0;
		break;
	case 1:
		user_word_0 |= 0x80000000;
		user_word_1 |= 0x0;
		break;
	case 2:
		user_word_0 |= 0x0;
		user_word_1 |= 0x1;
		break;
	case 3:
		user_word_0 |= 0x80000000;
		user_word_1 |= 0x1;
		break;
	case 4:
		user_word_0 |= 0x0;
		user_word_1 |= 0x2;
		break;
	case 5:
		user_word_0 |= 0x80000000;
		user_word_1 |= 0x2;
		break;
	case 6:
		user_word_0 |= 0x0;
		user_word_1 |= 0x3;
		break;
	case 7:
		user_word_0 |= 0x80000000;
		user_word_1 |= 0x3;
		break;
	case 8:
		user_word_0 |= 0x0;
		user_word_1 |= 0x4;
		break;
	case 9:
		user_word_0 |= 0x80000000;
		user_word_1 |= 0x4;
		break;
	case 10:
		user_word_0 |= 0x0;
		user_word_1 |= 0x5;
		break;
	case 11:
		user_word_0 |= 0x80000000;
		user_word_1 |= 0x5;
		break;
	}
	UW0_str.Format(_T("%8X"), user_word_0);
	m_UW0.SetWindowTextW(UW0_str);
	UW1_str.Format(_T("%8X"), user_word_1);
	m_UW1.SetWindowTextW(UW1_str);

	//WDRT EWOFFSET
	user_word_1 &= 0xFFFFFF87;
	switch (m_WDTEWOFFSET.GetCurSel())
	{
	case 0:
		user_word_1 |= 0x0;
		break;
	case 1:
		user_word_1 |= 0x08;
		break;
	case 2:
		user_word_1 |= 0x10;
		break;
	case 3:
		user_word_1 |= 0x18;
		break;
	case 4:
		user_word_1 |= 0x20;
		break;
	case 5:
		user_word_1 |= 0x28;
		break;
	case 6:
		user_word_1 |= 0x30;
		break;
	case 7:
		user_word_1 |= 0x38;
		break;
	case 8:
		user_word_1 |= 0x40;
		break;
	case 9:
		user_word_1 |= 0x48;
		break;
	case 10:
		user_word_1 |= 0x50;
		break;
	case 11:
		user_word_1 |= 0x58;
		break;
	}
	UW1_str.Format(_T("%8X"), user_word_1);
	m_UW1.SetWindowTextW(UW1_str);

	//WDT WEN
	user_word_1 &= 0xFFFFFF7F;
	if (m_WDTWEN.GetCurSel() == 0)
		user_word_1 |= 0x0;
	else if (m_WDTWEN.GetCurSel() == 1)
		user_word_1 |= 0x80;
	UW1_str.Format(_T("%8X"), user_word_1);
	m_UW1.SetWindowTextW(UW1_str);

	//BOD33 hystersis
	user_word_1 &= 0xFFFFFEFF;
	if (m_BODhyst.GetCurSel() == 0)
		user_word_1 |= 0x0;
	else if (m_BODhyst.GetCurSel() == 1)
		user_word_1 |= 0x0100;
	UW1_str.Format(_T("%8X"), user_word_1);
	m_UW1.SetWindowTextW(UW1_str);

}
void CDlg_ATSAMD::OnOK()
{
	CString temp_str;
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	int state = Encheck->GetCheck();
	if (state == 0x1)
	{
		//user word 0
		m_UW0.GetWindowTextW(temp_str);
		user_word_0 = numeric_conversion::hexstring_to_size_t(temp_str.GetString());
		memcpy(m_RegBuff, &user_word_0, sizeof(user_word_0));
		//user word 1
		m_UW1.GetWindowTextW(temp_str);
		user_word_1 = numeric_conversion::hexstring_to_size_t(temp_str.GetString());
		memcpy(m_RegBuff + sizeof(user_word_0), &user_word_1, sizeof(user_word_1));
		//Security
		CButton* SecurityEn = (CButton*)GetDlgItem(IDC_CHECK2);
		int state2 = SecurityEn->GetCheck();
		if (state2 == 0x1)
			security = 0x1;
		else if (state2 == 0x0)
			security = 0x0;
		memcpy(m_RegBuff + sizeof(user_word_0)+sizeof(user_word_1), &security, sizeof(security));
		*m_pDataLen = sizeof(user_word_0)+sizeof(user_word_1)+sizeof(security);
	}
	else if (state == 0x0)
		*m_pDataLen = 0;
	CMFCPropertyPage::OnOK();
}

LRESULT CDlg_ATSAMD::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();

}
LRESULT CDlg_ATSAMD::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}


void CDlg_ATSAMD::OnEnChangeLockbits()
{
	CString Lock_str;
	CString uw1temp_str;
	int temp;
	user_word_1 &= 0x0000FFFF;
	m_LOCKbits.GetWindowTextW(Lock_str);
	temp = numeric_conversion::hexstring_to_size_t(Lock_str.GetString());
	temp <<= 16;
	user_word_1 |= temp;
	uw1temp_str.Format(_T("%08X"), user_word_1);
	m_UW1.SetWindowTextW(uw1temp_str);
}

void CDlg_ATSAMD::OnBnClickedEnable()
{
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	int state = Encheck->GetCheck();
	if (state == 0x01)
	{
		m_BOOTPROT.EnableWindow(TRUE);
		m_EEPROM.EnableWindow(TRUE);
		m_BODLEVEL.EnableWindow(TRUE);
		m_BOD33Enable.EnableWindow(TRUE);
		m_BODAction.EnableWindow(TRUE);
		m_BODhyst.EnableWindow(TRUE);
		m_WDTEnable.EnableWindow(TRUE);
		m_WDTOn.EnableWindow(TRUE);
		m_WDTPeriod.EnableWindow(TRUE);
		m_WDTWindow.EnableWindow(TRUE);
		m_WDTEWOFFSET.EnableWindow(TRUE);
		m_WDTWEN.EnableWindow(TRUE);
		m_LOCKbits.EnableWindow(TRUE);
		Security_enable.EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
		//m_UW0.EnableWindow(TRUE);
		//m_UW1.EnableWindow(TRUE);
	}
	else if (state == 0x0)
	{
		m_BOOTPROT.EnableWindow(FALSE);
		m_EEPROM.EnableWindow(FALSE);
		m_BODLEVEL.EnableWindow(FALSE);
		m_BOD33Enable.EnableWindow(FALSE);
		m_BODAction.EnableWindow(FALSE);
		m_BODhyst.EnableWindow(FALSE);
		m_WDTEnable.EnableWindow(FALSE);
		m_WDTOn.EnableWindow(FALSE);
		m_WDTPeriod.EnableWindow(FALSE);
		m_WDTWindow.EnableWindow(FALSE);
		m_WDTEWOFFSET.EnableWindow(FALSE);
		m_WDTWEN.EnableWindow(FALSE);
		m_LOCKbits.EnableWindow(FALSE);
		Security_enable.EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		m_UW0.EnableWindow(FALSE);
		m_UW1.EnableWindow(FALSE);
	}


}

void CDlg_ATSAMD::OnBnClickedKeyIn()
{
	m_UW0.EnableWindow(TRUE);
	m_UW1.EnableWindow(TRUE);
}


void CDlg_ATSAMD::OnBnClickedSet()
{
	OnOK();
	OnSetActive();
}
