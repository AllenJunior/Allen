// Dlg_ATmega16.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_ATmega16.h"
#include "afxdialogex.h"
#include "numeric_conversion.h"
#include "../common/prog_info.h"

// Dlg_ATmega16 dialog

IMPLEMENT_DYNAMIC(CDlg_ATmega16, CMFCPropertyPage)

CDlg_ATmega16::CDlg_ATmega16(struct chip_info_c *ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_ATmega16::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_ATmega16::~CDlg_ATmega16()
{
}

void CDlg_ATmega16::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_OCDEN);
	DDX_Control(pDX, IDC_COMBO2, m_JTAGEN);
	DDX_Control(pDX, IDC_COMBO3, m_SPIEN);
	DDX_Control(pDX, IDC_COMBO4, m_CKOPT);
	DDX_Control(pDX, IDC_COMBO5, m_EESAVE);
	DDX_Control(pDX, IDC_COMBO6, m_BOOTSZ);
	DDX_Control(pDX, IDC_COMBO7, m_BOOTRST);
	DDX_Control(pDX, IDC_COMBO8, m_BODLevel);
	DDX_Control(pDX, IDC_COMBO9, m_BODEN);
	DDX_Control(pDX, IDC_COMBO10, m_CKSEL);
	DDX_Control(pDX, IDC_COMBO11, m_BLBB);
	DDX_Control(pDX, IDC_COMBO12, m_BLBA);
	DDX_Control(pDX, IDC_COMBO13, m_LOCK);
	DDX_Control(pDX, IDC_EDIT1, m_HFuse);
	DDX_Control(pDX, IDC_EDIT2, m_LFuse);
	DDX_Control(pDX, IDC_EDIT3, m_Lockbits);
	DDX_Control(pDX, IDC_CHECK1, Option_enable);
}


BEGIN_MESSAGE_MAP(CDlg_ATmega16, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_ATmega16::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_ATmega16::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_ATmega16::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_ATmega16::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_ATmega16::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDlg_ATmega16::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDlg_ATmega16::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CDlg_ATmega16::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO9, &CDlg_ATmega16::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO10, &CDlg_ATmega16::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO11, &CDlg_ATmega16::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO12, &CDlg_ATmega16::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO13, &CDlg_ATmega16::OnCbnSelchangeCombo)
	ON_BN_CLICKED(IDC_CHECK1, &CDlg_ATmega16::OnBnClickedEnable)
END_MESSAGE_MAP()


// Dlg_ATmega16 message handlers
BOOL CDlg_ATmega16::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	HighFuse = 0x99;
	LowFuse = 0xE1;
	Lockbits = 0xFF;
	m_HFuse.SetWindowTextW(L"99");
	m_LFuse.SetWindowTextW(L"E1");
	m_Lockbits.SetWindowTextW(L"FF");
	//Fuse High Byte
	m_OCDEN.InsertString(m_OCDEN.GetCount(), L"0 = OCD Enabled");
	m_OCDEN.InsertString(m_OCDEN.GetCount(), L"1 = OCD Disabled");
	m_JTAGEN.InsertString(m_JTAGEN.GetCount(), L"0 = JTAG Enabled");
	m_JTAGEN.InsertString(m_JTAGEN.GetCount(), L"1 = JTAG Disabled");
	m_SPIEN.InsertString(m_SPIEN.GetCount(), L"0 = Enabled ISP Programming");
	m_SPIEN.InsertString(m_SPIEN.GetCount(), L"1 = Disabled ISP Programming");
	m_CKOPT.InsertString(m_CKOPT.GetCount(), L"0 = Oscillator options enable");
	m_CKOPT.InsertString(m_CKOPT.GetCount(), L"1 = Oscillator options disable");
	m_EESAVE.InsertString(m_EESAVE.GetCount(), L"0 = Preserve EEPROM during Chip Erase");
	m_EESAVE.InsertString(m_EESAVE.GetCount(), L"1 = Unreserves EEPROM during Chip Erase");
	
	if (wcscmp(_T("ATmega16"), m_ChipInfo->description) == 0)
	{
		m_BOOTSZ.InsertString(m_BOOTSZ.GetCount(), L"00 = Boot Size 1024 words");
		m_BOOTSZ.InsertString(m_BOOTSZ.GetCount(), L"01 = Boot Size 512 words");
		m_BOOTSZ.InsertString(m_BOOTSZ.GetCount(), L"10 = Boot Size 256 words");
		m_BOOTSZ.InsertString(m_BOOTSZ.GetCount(), L"11 = Boot Size 128 words");
	}
	if (wcscmp(_T("ATmega32"), m_ChipInfo->description) == 0)
	{
		m_BOOTSZ.InsertString(m_BOOTSZ.GetCount(), L"00 = Boot Size 2048 words");
		m_BOOTSZ.InsertString(m_BOOTSZ.GetCount(), L"01 = Boot Size 1024 words");
		m_BOOTSZ.InsertString(m_BOOTSZ.GetCount(), L"10 = Boot Size 512 words");
		m_BOOTSZ.InsertString(m_BOOTSZ.GetCount(), L"11 = Boot Size 256 words");
	}
	m_BOOTRST.InsertString(m_BOOTRST.GetCount(), L"0 = Enabled boot reset vector");
	m_BOOTRST.InsertString(m_BOOTRST.GetCount(), L"1 = Disabled boot reset vector");//initial value
	//Fuse Low Byte
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"0 = Trigger Level at 4.3V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"1 = Trigger Level at 2.7V");
	m_BODEN.InsertString(m_BODEN.GetCount(), L"0 = Brownout detector enable");
	m_BODEN.InsertString(m_BODEN.GetCount(), L"1 = Brownout detector disable");
	//CLK Setting
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0000 = Ext.CLK, StartupTime : 6CK");			// 0 = 0x00
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0000 = Ext.CLK, StartupTime : 6CK+4ms");		// 1 = 0x10
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0000 = Ext.CLK, StartupTime : 6CK+64ms");		// 2 = 0x20
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0001 = Int.RC OSC : 1MHz, StartupTime : 6CK");// 3 = 0x01
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0001 = Int.RC OSC : 1MHz, StartupTime : 6CK+4ms"); // 4 = 0x11
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0001 = Int.RC OSC : 1MHz, StartupTime : 6CK+64ms"); // 5 = 0x21
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0010 = Int.RC OSC : 2MHz, StartupTime : 6CK"); // 6 = 0x02
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0010 = Int.RC OSC : 2MHz, StartupTime : 6CK+4ms"); // 7 = 0x12
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0010 = Int.RC OSC : 2MHz, StartupTime : 6CK+64ms"); // 8 = 0x22
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0011 = Int.RC OSC : 4MHz, StartupTime : 6CK"); // 9 = 0x03
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0011 = Int.RC OSC : 4MHz, StartupTime : 6CK+4ms"); // 10 = 0x13
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0011 = Int.RC OSC : 4MHz, StartupTime : 6CK+64ms");// 11 = 0x23
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0100 = Int.RC OSC : 8MHz, StartupTime : 6CK");     // 12 = 0x04
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0100 = Int.RC OSC : 8MHz, StartupTime : 6CK+4ms"); // 13 = 0x14
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0100 = Int.RC OSC : 8MHz, StartupTime : 6CK+64ms");// 14 = 0x24
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0101 = Ext.RC OSC : -0.9MHz, StartupTime : 18CK"); // 15 = 0x05
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0101 = Ext.RC OSC : -0.9MHz, StartupTime : 18CK+4ms"); // 16 = 0x15
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0101 = Ext.RC OSC : -0.9MHz, StartupTime : 18CK+64ms");// 17 = 0x25
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,0101 = Ext.RC OSC : -0.9MHz, StartupTime : 6CK+4ms");  // 18 = 0x35
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0110 = Ext.RC OSC : 0.9-3.0MHz, StartupTime : 18CK");  // 19 = 0x06
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0110 = Ext.RC OSC : 0.9-3.0MHz, StartupTime : 18CK+4ms"); // 20 = 0x16
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0110 = Ext.RC OSC : 0.9-3.0MHz, StartupTime : 18CK+64ms"); // 21 = 0x26
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,0110 = Ext.RC OSC : 0.9-3.0MHz, StartupTime : 6CK+4ms"); // 22 = 0x36
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0111 = Ext.RC OSC : 3.0-8.0MHz, StartupTime : 18CK"); // 23 = 0x07
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0111 = Ext.RC OSC : 3.0-8.0MHz, StartupTime : 18CK+4ms"); // 24 = 0x17
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0111 = Ext.RC OSC : 3.0-8.0MHz, StartupTime : 18CK+64ms"); // 25 = 0x27
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,0111 = Ext.RC OSC : 3.0-8.0MHz, StartupTime : 6CK+4ms");  // 26 = 0x37
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1000 = Ext.RC OSC : 8.0-12.0MHz, StartupTime : 18CK"); // 27 = 0x08
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1000 = Ext.RC OSC : 8.0-12.0MHz, StartupTime : 18CK+4ms"); // 28 = 0x18
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1000 = Ext.RC OSC : 8.0-12.0MHz, StartupTime : 18CK+64ms"); // 29 = 0x28
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1000 = Ext.RC OSC : 8.0-12.0MHz, StartupTime : 6CK+4ms"); // 30 = 0x38
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1001 = Ext.low.freq OSC, StartupTime : 1CK+4ms");  // 31 = 0x09
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1001 = Ext.low.freq OSC, StartupTime : 1CK+64ms"); // 32 = 0x19
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1001 = Ext.low.freq OSC, StartupTime : 32CK+64ms");// 33 = 0x29
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1010 = Ext.Crystal : 0.4-0.9MHz, StartupTime : 258CK+4ms"); // 34 = 0x0A
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1010 = Ext.Crystal : 0.4-0.9MHz, StartupTime : 258CK+64ms");// 35 = 0x1A
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1010 = Ext.Crystal : 0.4-0.9MHz, StartupTime : 1KCK"); // 36 = 0x2A
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1010 = Ext.Crystal : 0.4-0.9MHz, StartupTime : 1KCK+4ms"); // 37 = 0x3A
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1011 = Ext.Crystal : 0.4-0.9MHz, StartupTime : 1KCK+64ms"); // 38 = 0x0B
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1011 = Ext.Crystal : 0.4-0.9MHz, StartupTime : 16CK"); // 39 = 0x1B
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1011 = Ext.Crystal : 0.4-0.9MHz, StartupTime : 16CK+4ms");// 40 = 0x2B
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1011 = Ext.Crystal : 0.4-0.9MHz, StartupTime : 16CK+64ms"); // 41 = 0x3B
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1100 = Ext.Crystal : 0.9-3.0MHz, StartupTime : 258CK+4ms"); // 42 = 0x0C
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1100 = Ext.Crystal : 0.9-3.0MHz, StartupTime : 258CK+64ms"); // 43 = 0x1C
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1100 = Ext.Crystal : 0.9-3.0MHz, StartupTime : 1KCK"); // 44 = 0x2C
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1100 = Ext.Crystal : 0.9-3.0MHz, StartupTime : 1KCK+4ms"); // 45 = 0x3C
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1101 = Ext.Crystal : 0.9-3.0MHz, StartupTime : 1KCK+64ms"); // 46 = 0x0D
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1101 = Ext.Crystal : 0.9-3.0MHz, StartupTime : 16CK"); // 47 = 0x1D
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1101 = Ext.Crystal : 0.9-3.0MHz, StartupTime : 16CK+4ms"); // 48 = 0x2D
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1101 = Ext.Crystal : 0.9-3.0MHz, StartupTime : 16CK+64ms"); // 49 = 0x3D
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1110 = Ext.Crystal : 3.0-8.0MHz, StartupTime : 258CK+4ms"); // 50 = 0x0E
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1110 = Ext.Crystal : 3.0-8.0MHz, StartupTime : 258CK+64ms"); // 51 = 0x1E
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1110 = Ext.Crystal : 3.0-8.0MHz, StartupTime : 1KCK"); // 52 = 0x2E
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1110 = Ext.Crystal : 3.0-8.0MHz, StartupTime : 1KCK+4ms"); // 53 = 0x3E
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1111 = Ext.Crystal : 3.0-8.0MHz, StartupTime : 1KCK+64ms"); // 54 = 0x0F
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1111 = Ext.Crystal : 3.0-8.0MHz, StartupTime : 16CK"); // 55 = 0x1F
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1111 = Ext.Crystal : 3.0-8.0MHz, StartupTime : 16CK+4ms"); // 56 = 0x2F
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1111 = Ext.Crystal : 3.0-8.0MHz, StartupTime : 16CK+64ms"); // 57 = 0x3F
	//LOCK
	m_BLBB.InsertString(m_BLBB.GetCount(), L"00 = RWLock, SPM & LPM are prohibited");
	m_BLBB.InsertString(m_BLBB.GetCount(), L"01 = RLock, LPM is prohibited");
	m_BLBB.InsertString(m_BLBB.GetCount(), L"10 = WLock, SPM is prohibited");
	m_BLBB.InsertString(m_BLBB.GetCount(), L"11 = NoLock, Lock Disabled");//initial value
	m_BLBA.InsertString(m_BLBA.GetCount(), L"00 = RWLock, SPM & LPM are prohibited");
	m_BLBA.InsertString(m_BLBA.GetCount(), L"01 = RLock, LPM is prohibited");
	m_BLBA.InsertString(m_BLBA.GetCount(), L"10 = WLock, SPM is prohibited");
	m_BLBA.InsertString(m_BLBA.GetCount(), L"11 = NoLock, Lock Disabled");//initial value
	m_LOCK.InsertString(m_LOCK.GetCount(), L"00 = Further Programming and verification Disabled");
	m_LOCK.InsertString(m_LOCK.GetCount(), L"10 = Further Programming Disabled");
	m_LOCK.InsertString(m_LOCK.GetCount(), L"11 = No memory Lock features enabled");//initial value

	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	Encheck->SetCheck(0);
	m_EESAVE.SetDroppedWidth(300);
	m_CKSEL.SetDroppedWidth(400);
	m_BLBB.SetDroppedWidth(300);
	m_BLBA.SetDroppedWidth(300);
	m_LOCK.SetDroppedWidth(300);
	//
	m_OCDEN.SetCurSel(1);
	m_JTAGEN.SetCurSel(0);
	m_SPIEN.SetCurSel(0);
	m_CKOPT.SetCurSel(1);
	m_EESAVE.SetCurSel(1);
	m_BOOTSZ.SetCurSel(0);
	m_BOOTRST.SetCurSel(1);
	m_BODLevel.SetCurSel(1);
	m_BODEN.SetCurSel(1);
	m_CKSEL.SetCurSel(5);
	m_BLBB.SetCurSel(3);
	m_BLBA.SetCurSel(3);
	m_LOCK.SetCurSel(2);
	//Disable Screen
	m_OCDEN.EnableWindow(FALSE);
	m_JTAGEN.EnableWindow(FALSE);
	m_SPIEN.EnableWindow(FALSE);
	m_CKOPT.EnableWindow(FALSE);
	m_EESAVE.EnableWindow(FALSE);
	m_BOOTSZ.EnableWindow(FALSE);
	m_BOOTRST.EnableWindow(FALSE);
	m_BODLevel.EnableWindow(FALSE);
	m_BODEN.EnableWindow(FALSE);
	m_CKSEL.EnableWindow(FALSE);
	m_BLBB.EnableWindow(FALSE);
	m_BLBA.EnableWindow(FALSE);
	m_LOCK.EnableWindow(FALSE);

	return TRUE;
}

BOOL CDlg_ATmega16::OnSetActive()
{
	if (*m_pDataLen != NULL)
	{
		CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
		Encheck->SetCheck(1);
		m_OCDEN.EnableWindow(TRUE);
		m_JTAGEN.EnableWindow(TRUE);
		m_SPIEN.EnableWindow(FALSE);
		m_CKOPT.EnableWindow(TRUE);
		m_EESAVE.EnableWindow(TRUE);
		m_BOOTSZ.EnableWindow(TRUE);
		m_BOOTRST.EnableWindow(TRUE);
		m_BODLevel.EnableWindow(FALSE);
		m_BODEN.EnableWindow(TRUE);
		m_CKSEL.EnableWindow(TRUE);
		m_BLBB.EnableWindow(TRUE);
		m_BLBA.EnableWindow(TRUE);
		m_LOCK.EnableWindow(TRUE);
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
		//OCDEN
		if ((HighFuse & 0x80) == 0x80)
			m_OCDEN.SetCurSel(1);
		else if ((HighFuse & 0x80) == 0x00)
			m_OCDEN.SetCurSel(0);
		//JTAGEN
		if ((HighFuse & 0x40) == 0x40)
			m_JTAGEN.SetCurSel(1);
		else if ((HighFuse & 0x40) == 0x00)
			m_JTAGEN.SetCurSel(0);
		//SPIEN
		if ((HighFuse & 0x20) == 0x20)
			m_SPIEN.SetCurSel(1);
		else if ((HighFuse & 0x20) == 0x00)
			m_SPIEN.SetCurSel(0);
		//CKOPT
		if ((HighFuse & 0x10) == 0x10)
			m_CKOPT.SetCurSel(1);
		else if ((HighFuse & 0x10) == 0x00)
			m_CKOPT.SetCurSel(0);
		//EESAVE
		if ((HighFuse & 0x08) == 0x08)
			m_EESAVE.SetCurSel(1);
		else if ((HighFuse & 0x08) == 0x00)
			m_EESAVE.SetCurSel(0);
		//BOOTSZ
		if ((HighFuse & 0x06) == 0x06)
			m_BOOTSZ.SetCurSel(3);
		else if ((HighFuse & 0x06) == 0x04)
			m_BOOTSZ.SetCurSel(2);
		else if ((HighFuse & 0x06) == 0x02)
			m_BOOTSZ.SetCurSel(1);
		else if ((HighFuse & 0x06) == 0x00)
			m_BOOTSZ.SetCurSel(0);
		//BOOTRST
		if ((HighFuse & 0x01) == 0x01)
			m_BOOTRST.SetCurSel(1);
		else if ((HighFuse & 0x01) == 0x00)
			m_BOOTRST.SetCurSel(0);
		//BODLevel
		if ((LowFuse & 0x80) == 0x80)
			m_BODLevel.SetCurSel(1);
		else if ((LowFuse & 0x80) == 0x00)
			m_BODLevel.SetCurSel(0);
		//BODEN
		if ((LowFuse & 0x40) == 0x40)
			m_BODEN.SetCurSel(1);
		else if ((LowFuse & 0x40) == 0x00)
			m_BODEN.SetCurSel(0);
		//CLK Setting
		switch (LowFuse & 0x3F)
		{
		case 0x00:
			m_CKSEL.SetCurSel(0);
			break;
		case 0x10:
			m_CKSEL.SetCurSel(1);
			break;
		case 0x20:
			m_CKSEL.SetCurSel(2);
			break;
		case 0x01:
			m_CKSEL.SetCurSel(3);
			break;
		case 0x11:
			m_CKSEL.SetCurSel(4);
			break;
		case 0x21:
			m_CKSEL.SetCurSel(5);
			break;
		case 0x02:
			m_CKSEL.SetCurSel(6);
			break;
		case 0x12:
			m_CKSEL.SetCurSel(7);
			break;
		case 0x22:
			m_CKSEL.SetCurSel(8);
			break;
		case 0x03:
			m_CKSEL.SetCurSel(9);
			break;
		case 0x13:
			m_CKSEL.SetCurSel(10);
			break;
		case 0x23:
			m_CKSEL.SetCurSel(11);
			break;
		case 0x04:
			m_CKSEL.SetCurSel(12);
			break;
		case 0x14:
			m_CKSEL.SetCurSel(13);
			break;
		case 0x24:
			m_CKSEL.SetCurSel(14);
			break;
		case 0x05:
			m_CKSEL.SetCurSel(15);
			break;
		case 0x15:
			m_CKSEL.SetCurSel(16);
			break;
		case 0x25:
			m_CKSEL.SetCurSel(17);
			break;
		case 0x35:
			m_CKSEL.SetCurSel(18);
			break;
		case 0x06:
			m_CKSEL.SetCurSel(19);
			break;
		case 0x16:
			m_CKSEL.SetCurSel(20);
			break;
		case 0x26:
			m_CKSEL.SetCurSel(21);
			break;
		case 0x36:
			m_CKSEL.SetCurSel(22);
			break;
		case 0x07:
			m_CKSEL.SetCurSel(23);
			break;
		case 0x17:
			m_CKSEL.SetCurSel(24);
			break;
		case 0x27:
			m_CKSEL.SetCurSel(25);
			break;
		case 0x37:
			m_CKSEL.SetCurSel(26);
			break;
		case 0x08:
			m_CKSEL.SetCurSel(27);
			break;
		case 0x18:
			m_CKSEL.SetCurSel(28);
			break;
		case 0x28:
			m_CKSEL.SetCurSel(29);
			break;
		case 0x38:
			m_CKSEL.SetCurSel(30);
			break;
		case 0x09:
			m_CKSEL.SetCurSel(31);
			break;
		case 0x19:
			m_CKSEL.SetCurSel(32);
			break;
		case 0x29:
			m_CKSEL.SetCurSel(33);
			break;
		case 0x0A:
			m_CKSEL.SetCurSel(34);
			break;
		case 0x1A:
			m_CKSEL.SetCurSel(35);
			break;
		case 0x2A:
			m_CKSEL.SetCurSel(36);
			break;
		case 0x3A:
			m_CKSEL.SetCurSel(37);
			break;
		case 0x0B:
			m_CKSEL.SetCurSel(38);
			break;
		case 0x1B:
			m_CKSEL.SetCurSel(39);
			break;
		case 0x2B:
			m_CKSEL.SetCurSel(40);
			break;
		case 0x3B:
			m_CKSEL.SetCurSel(41);
			break;
		case 0x0C:
			m_CKSEL.SetCurSel(42);
			break;
		case 0x1C:
			m_CKSEL.SetCurSel(43);
			break;
		case 0x2C:
			m_CKSEL.SetCurSel(44);
			break;
		case 0x3C:
			m_CKSEL.SetCurSel(45);
			break;
		case 0x0D:
			m_CKSEL.SetCurSel(46);
			break;
		case 0x1D:
			m_CKSEL.SetCurSel(47);
			break;
		case 0x2D:
			m_CKSEL.SetCurSel(48);
			break;
		case 0x3D:
			m_CKSEL.SetCurSel(49);
			break;
		case 0x0E:
			m_CKSEL.SetCurSel(50);
			break;
		case 0x1E:
			m_CKSEL.SetCurSel(51);
			break;
		case 0x2E:
			m_CKSEL.SetCurSel(52);
			break;
		case 0x3E:
			m_CKSEL.SetCurSel(53);
			break;
		case 0x0F:
			m_CKSEL.SetCurSel(54);
			break;
		case 0x1F:
			m_CKSEL.SetCurSel(55);
			break;
		case 0x2F:
			m_CKSEL.SetCurSel(56);
			break;
		case 0x3F:
			m_CKSEL.SetCurSel(57);
			break;
		}
		//BLBB
		if ((Lockbits & 0x30) == 0x30)
			m_BLBB.SetCurSel(3);
		else if ((Lockbits & 0x30) == 0x20)
			m_BLBB.SetCurSel(2);
		else if ((Lockbits & 0x30) == 0x10)
			m_BLBB.SetCurSel(1);
		else if ((Lockbits & 0x30) == 0x00)
			m_BLBB.SetCurSel(0);
		//BLBA
		if ((Lockbits & 0x0C) == 0x0C)
			m_BLBA.SetCurSel(3);
		else if ((Lockbits & 0x0C) == 0x08)
			m_BLBA.SetCurSel(2);
		else if ((Lockbits & 0x0C) == 0x04)
			m_BLBA.SetCurSel(1);
		else if ((Lockbits & 0x0C) == 0x00)
			m_BLBA.SetCurSel(0);
		//Lock
		if ((Lockbits & 0x03) == 0x03)
			m_LOCK.SetCurSel(2);
		else if ((Lockbits & 0x03) == 0x02)
			m_LOCK.SetCurSel(1);
		else if ((Lockbits & 0x03) == 0x00)
			m_LOCK.SetCurSel(0);
	}
	else if (*m_pDataLen == NULL)
	{
		CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
		Encheck->SetCheck(0);
		m_OCDEN.EnableWindow(FALSE);
		m_JTAGEN.EnableWindow(FALSE);
		m_SPIEN.EnableWindow(FALSE);
		m_CKOPT.EnableWindow(FALSE);
		m_EESAVE.EnableWindow(FALSE);
		m_BOOTSZ.EnableWindow(FALSE);
		m_BOOTRST.EnableWindow(FALSE);
		m_BODLevel.EnableWindow(FALSE);
		m_BODEN.EnableWindow(FALSE);
		m_CKSEL.EnableWindow(FALSE);
		m_BLBB.EnableWindow(FALSE);
		m_BLBA.EnableWindow(FALSE);
		m_LOCK.EnableWindow(FALSE);
	}
	return CMFCPropertyPage::OnSetActive();
}

void CDlg_ATmega16::OnCbnSelchangeCombo()
{
	CString HFuse_str, LFuse_str, Lockbits_str;
	HighFuse = 0x99;
	LowFuse = 0xE1;
	Lockbits = 0xFF;
	//OCDEN
	HighFuse &= 0x7F;
	if (m_OCDEN.GetCurSel() == 1)
		HighFuse |= 0x80;
	else if (m_OCDEN.GetCurSel() == 0)
		HighFuse |= 0x00;
	HFuse_str.Format(_T("%02X"), HighFuse);
	m_HFuse.SetWindowTextW(HFuse_str);
	//JTAGEN
	HighFuse &= 0xBF;
	if (m_JTAGEN.GetCurSel() == 1)
		HighFuse |= 0x40;
	else if (m_JTAGEN.GetCurSel() == 0)
		HighFuse |= 0x00;
	HFuse_str.Format(_T("%02X"), HighFuse);
	m_HFuse.SetWindowTextW(HFuse_str);
	//SPIEN
	HighFuse &= 0xDF;
	if (m_SPIEN.GetCurSel() == 1)
		HighFuse |= 0x20;
	else if (m_SPIEN.GetCurSel() == 0)
		HighFuse |= 0x00;
	HFuse_str.Format(_T("%02X"), HighFuse);
	m_HFuse.SetWindowTextW(HFuse_str);
	//CKOPT
	HighFuse &= 0xEF;
	if (m_CKOPT.GetCurSel() == 1)
		HighFuse |= 0x10;
	else if (m_CKOPT.GetCurSel() == 0)
		HighFuse |= 0x00;
	HFuse_str.Format(_T("%02X"), HighFuse);
	m_HFuse.SetWindowTextW(HFuse_str);
	//EESAVE
	HighFuse &= 0xF7;
	if (m_EESAVE.GetCurSel() == 1)
		HighFuse |= 0x08;
	else if (m_EESAVE.GetCurSel() == 0)
		HighFuse |= 0x00;
	HFuse_str.Format(_T("%02X"), HighFuse);
	m_HFuse.SetWindowTextW(HFuse_str);
	//BOOTSZ
	HighFuse &= 0xF9;
	if (m_BOOTSZ.GetCurSel() == 3)
		HighFuse |= 0x06;
	else if (m_BOOTSZ.GetCurSel() == 2)
		HighFuse |= 0x04;
	else if (m_BOOTSZ.GetCurSel() == 1)
		HighFuse |= 0x02;
	else if (m_BOOTSZ.GetCurSel() == 1)
		HighFuse |= 0x00;
	HFuse_str.Format(_T("%02X"), HighFuse);
	m_HFuse.SetWindowTextW(HFuse_str);
	//BOOTRST
	HighFuse &= 0xFE;
	if (m_BOOTRST.GetCurSel() == 1)
		HighFuse |= 0x01;
	else if (m_BOOTRST.GetCurSel() == 0)
		HighFuse |= 0x00;
	HFuse_str.Format(_T("%02X"), HighFuse);
	m_HFuse.SetWindowTextW(HFuse_str);
	//BODLevel
	LowFuse &= 0x7F;
	if (m_BODLevel.GetCurSel() == 1)
		LowFuse |= 0x80;
	else if (m_BODLevel.GetCurSel() == 0)
		LowFuse |= 0x00;
	LFuse_str.Format(_T("%02X"), LowFuse);
	m_LFuse.SetWindowTextW(LFuse_str);
	//BODEN
	LowFuse &= 0xBF;
	if (m_BODEN.GetCurSel() == 1)
		LowFuse |= 0x40;
	else if (m_BODEN.GetCurSel() == 0)
		LowFuse |= 0x00;
	LFuse_str.Format(_T("%02X"), LowFuse);
	m_LFuse.SetWindowTextW(LFuse_str);
	//CLK Setting
	LowFuse &= 0xC0;
	switch (m_CKSEL.GetCurSel())
	{
	case 0:
		LowFuse |= 0x00;
		break;
	case 1:
		LowFuse |= 0x10;
		break;
	case 2:
		LowFuse |= 0x20;
		break;
	case 3:
		LowFuse |= 0x01;
		break;
	case 4:
		LowFuse |= 0x11;
		break;
	case 5:
		LowFuse |= 0x21;
		break;
	case 6:
		LowFuse |= 0x02;
		break;
	case 7:
		LowFuse |= 0x12;
		break;
	case 8:
		LowFuse |= 0x22;
		break;
	case 9:
		LowFuse |= 0x03;
		break;
	case 10:
		LowFuse |= 0x13;
		break;
	case 11:
		LowFuse |= 0x23;
		break;
	case 12:
		LowFuse |= 0x04;
		break;
	case 13:
		LowFuse |= 0x14;
		break;
	case 14:
		LowFuse |= 0x24;
		break;
	case 15:
		LowFuse |= 0x05;
		break;
	case 16:
		LowFuse |= 0x15;
		break;
	case 17:
		LowFuse |= 0x25;
		break;
	case 18:
		LowFuse |= 0x35;
		break;
	case 19:
		LowFuse |= 0x06;
		break;
	case 20:
		LowFuse |= 0x16;
		break;
	case 21:
		LowFuse |= 0x26;
		break;
	case 22:
		LowFuse |= 0x36;
		break;
	case 23:
		LowFuse |= 0x07;
		break;
	case 24:
		LowFuse |= 0x17;
		break;
	case 25:
		LowFuse |= 0x27;
		break;
	case 26:
		LowFuse |= 0x37;
		break;
	case 27:
		LowFuse |= 0x08;
		break;
	case 28:
		LowFuse |= 0x18;
		break;
	case 29:
		LowFuse |= 0x28;
		break;
	case 30:
		LowFuse |= 0x38;
		break;
	case 31:
		LowFuse |= 0x09;
		break;
	case 32:
		LowFuse |= 0x19;
		break;
	case 33:
		LowFuse |= 0x29;
		break;
	case 34:
		LowFuse |= 0x0A;
		break;
	case 35:
		LowFuse |= 0x1A;
		break;
	case 36:
		LowFuse |= 0x2A;
		break;
	case 37:
		LowFuse |= 0x3A;
		break;
	case 38:
		LowFuse |= 0x0B;
		break;
	case 39:
		LowFuse |= 0x1B;
		break;
	case 40:
		LowFuse |= 0x2B;
		break;
	case 41:
		LowFuse |= 0x3B;
		break;
	case 42:
		LowFuse |= 0x0C;
		break;
	case 43:
		LowFuse |= 0x1C;
		break;
	case 44:
		LowFuse |= 0x2C;
		break;
	case 45:
		LowFuse |= 0x3C;
		break;
	case 46:
		LowFuse |= 0x0D;
		break;
	case 47:
		LowFuse |= 0x1D;
		break;
	case 48:
		LowFuse |= 0x2D;
		break;
	case 49:
		LowFuse |= 0x3D;
		break;
	case 50:
		LowFuse |= 0x0E;
		break;
	case 51:
		LowFuse |= 0x1E;
		break;
	case 52:
		LowFuse |= 0x2E;
		break;
	case 53:
		LowFuse |= 0x3E;
		break;
	case 54:
		LowFuse |= 0x0F;
		break;
	case 55:
		LowFuse |= 0x1F;
		break;
	case 56:
		LowFuse |= 0x2F;
		break;
	case 57:
		LowFuse |= 0x3F;
		break;
	}
	LFuse_str.Format(_T("%02X"), LowFuse);
	m_LFuse.SetWindowTextW(LFuse_str);
	//BLBB
	Lockbits &= 0xCF;
	if (m_BLBB.GetCurSel() == 3)
		Lockbits |= 0x30;
	else if (m_BLBB.GetCurSel() == 2)
		Lockbits |= 0x20;
	else if (m_BLBB.GetCurSel() == 1)
		Lockbits |= 0x10;
	else if (m_BLBB.GetCurSel() == 0)
		Lockbits |= 0x00;
	Lockbits_str.Format(_T("%02X"), Lockbits);
	m_Lockbits.SetWindowTextW(Lockbits_str);
	//BLBA
	Lockbits &= 0xF3;
	if (m_BLBA.GetCurSel() == 3)
		Lockbits |= 0x0C;
	else if (m_BLBA.GetCurSel() == 2)
		Lockbits |= 0x08;
	else if (m_BLBA.GetCurSel() == 1)
		Lockbits |= 0x04;
	else if (m_BLBA.GetCurSel() == 0)
		Lockbits |= 0x00;
	Lockbits_str.Format(_T("%02X"), Lockbits);
	m_Lockbits.SetWindowTextW(Lockbits_str);
	//Lock
	Lockbits &= 0xFC;
	if (m_LOCK.GetCurSel() == 2)
		Lockbits |= 0x03;
	else if (m_LOCK.GetCurSel() == 1)
		Lockbits |= 0x02;
	else if (m_LOCK.GetCurSel() == 0)
		Lockbits |= 0x00;
	Lockbits_str.Format(_T("%02X"), Lockbits);
	m_Lockbits.SetWindowTextW(Lockbits_str);
}

void CDlg_ATmega16::OnOK()
{
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	int state = Encheck->GetCheck();
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

LRESULT CDlg_ATmega16::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();

}
LRESULT CDlg_ATmega16::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}

void CDlg_ATmega16::OnBnClickedEnable()
{
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	int state = Encheck->GetCheck();
	if (state == 1)
	{
		m_OCDEN.EnableWindow(TRUE);
		m_JTAGEN.EnableWindow(TRUE);
		m_SPIEN.EnableWindow(FALSE);
		m_CKOPT.EnableWindow(TRUE);
		m_EESAVE.EnableWindow(TRUE);
		m_BOOTSZ.EnableWindow(TRUE);
		m_BOOTRST.EnableWindow(TRUE);
		m_BODLevel.EnableWindow(FALSE);
		m_BODEN.EnableWindow(TRUE);
		m_CKSEL.EnableWindow(TRUE);
		m_BLBB.EnableWindow(TRUE);
		m_BLBA.EnableWindow(TRUE);
		m_LOCK.EnableWindow(TRUE);
	}
	else if (state == 0)
	{
		m_OCDEN.EnableWindow(FALSE);
		m_JTAGEN.EnableWindow(FALSE);
		m_SPIEN.EnableWindow(FALSE);
		m_CKOPT.EnableWindow(FALSE);
		m_EESAVE.EnableWindow(FALSE);
		m_BOOTSZ.EnableWindow(FALSE);
		m_BOOTRST.EnableWindow(FALSE);
		m_BODLevel.EnableWindow(FALSE);
		m_BODEN.EnableWindow(FALSE);
		m_CKSEL.EnableWindow(FALSE);
		m_BLBB.EnableWindow(FALSE);
		m_BLBA.EnableWindow(FALSE);
		m_LOCK.EnableWindow(FALSE);
	}
}