// Dlg_ATmegaX5X9.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_ATmegaX5X9.h"
#include "afxdialogex.h"
#include "numeric_conversion.h"
#include "../common/prog_info.h"

// Dlg_ATmegaX5X9 dialog

IMPLEMENT_DYNAMIC(CDlg_ATmegaX5X9, CMFCPropertyPage)

CDlg_ATmegaX5X9::CDlg_ATmegaX5X9(struct chip_info_c *ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_ATmegaX5X9::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_ATmegaX5X9::~CDlg_ATmegaX5X9()
{
}

void CDlg_ATmegaX5X9::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, Option_enable);
	DDX_Control(pDX, IDC_COMBO1, m_BODLevel);
	DDX_Control(pDX, IDC_COMBO2, m_RSTDISBL);
	DDX_Control(pDX, IDC_COMBO3, m_OCDEN);
	DDX_Control(pDX, IDC_COMBO4, m_JTAGEN);
	DDX_Control(pDX, IDC_COMBO5, m_SPIEN);
	DDX_Control(pDX, IDC_COMBO6, m_WDTON);
	DDX_Control(pDX, IDC_COMBO7, m_EESAVE);
	DDX_Control(pDX, IDC_COMBO8, m_BOOTSZ);
	DDX_Control(pDX, IDC_COMBO9, m_BOOTRST);
	DDX_Control(pDX, IDC_COMBO10, m_CKDIV8);
	DDX_Control(pDX, IDC_COMBO11, m_CKOUT);
	DDX_Control(pDX, IDC_COMBO12, m_CKSEL);
	DDX_Control(pDX, IDC_COMBO13, m_BLBB);
	DDX_Control(pDX, IDC_COMBO14, m_BLBA);
	DDX_Control(pDX, IDC_COMBO15, m_LOCK);
	DDX_Control(pDX, IDC_EDIT1, m_ExFuse);
	DDX_Control(pDX, IDC_EDIT2, m_HFuse);
	DDX_Control(pDX, IDC_EDIT3, m_LFuse);
	DDX_Control(pDX, IDC_EDIT4, m_Lockbits);
}


BEGIN_MESSAGE_MAP(CDlg_ATmegaX5X9, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_ATmegaX5X9::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_ATmegaX5X9::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_ATmegaX5X9::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_ATmegaX5X9::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_ATmegaX5X9::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDlg_ATmegaX5X9::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDlg_ATmegaX5X9::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CDlg_ATmegaX5X9::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO9, &CDlg_ATmegaX5X9::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO10, &CDlg_ATmegaX5X9::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO11, &CDlg_ATmegaX5X9::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO12, &CDlg_ATmegaX5X9::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO13, &CDlg_ATmegaX5X9::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO14, &CDlg_ATmegaX5X9::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO15, &CDlg_ATmegaX5X9::OnCbnSelchangeCombo)
	ON_BN_CLICKED(IDC_CHECK1, &CDlg_ATmegaX5X9::OnBnClickedEnable)
END_MESSAGE_MAP()


// Dlg_ATmegaX5X9 message handlers
BOOL CDlg_ATmegaX5X9::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	ExtendedFuse = 0xFF;
	HighFuse = 0x99;
	LowFuse = 0x62;
	Lockbits = 0xFF;
	m_ExFuse.SetWindowTextW(L"FF");
	m_HFuse.SetWindowTextW(L"99");
	m_LFuse.SetWindowTextW(L"62");
	m_Lockbits.SetWindowTextW(L"FF");
	//Extended Fuse
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"00 = Trigger Level at 4.3V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"01 = Trigger Level at 2.7V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"10 = Trigger Level at 1.8V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"11 = BOD Disable");//initial value
	m_RSTDISBL.InsertString(m_RSTDISBL.GetCount(), L"0 = Enabled external reset");
	m_RSTDISBL.InsertString(m_RSTDISBL.GetCount(), L"1 = Disabled external reset");//initial value
	//High Fuse
	m_OCDEN.InsertString(m_OCDEN.GetCount(), L"0 = OCD Enable");
	m_OCDEN.InsertString(m_OCDEN.GetCount(), L"1 = OCD Disable");//initial value
	m_JTAGEN.InsertString(m_JTAGEN.GetCount(), L"0 = JTAG Enabled");//initial value
	m_JTAGEN.InsertString(m_JTAGEN.GetCount(), L"1 = JTAG Disabled");
	m_SPIEN.InsertString(m_SPIEN.GetCount(), L"0 = Enabled ISP Programming");//initial value
	m_SPIEN.InsertString(m_SPIEN.GetCount(), L"1 = Disabled ISP Programming");
	m_WDTON.InsertString(m_WDTON.GetCount(), L"0 = Enable Watchdog Timer always on");
	m_WDTON.InsertString(m_WDTON.GetCount(), L"1 = Disable Watchdog Timer always on");//initial value
	m_EESAVE.InsertString(m_EESAVE.GetCount(), L"0 = Preserve EEPROM during Chip Erase");
	m_EESAVE.InsertString(m_EESAVE.GetCount(), L"1 = Unreserves EEPROM during Chip Erase");//initial value
	m_BOOTSZ.InsertString(m_BOOTSZ.GetCount(), L"00 = Boot Size refer to datasheet");//initial value
	m_BOOTSZ.InsertString(m_BOOTSZ.GetCount(), L"01 = Boot Size refer to datasheet");
	m_BOOTSZ.InsertString(m_BOOTSZ.GetCount(), L"10 = Boot Size refer to datasheet");
	m_BOOTSZ.InsertString(m_BOOTSZ.GetCount(), L"11 = Boot Size refer to datasheet");
	m_BOOTRST.InsertString(m_BOOTRST.GetCount(), L"0 = Enabled boot reset vector");
	m_BOOTRST.InsertString(m_BOOTRST.GetCount(), L"1 = Disabled boot reset vector");//initial value
	//Low Fuse
	m_CKDIV8.InsertString(m_CKDIV8.GetCount(), L"0 = Divide CLK by 8 Enable");
	m_CKDIV8.InsertString(m_CKDIV8.GetCount(), L"1 = Divide CLK by 8 Disable");
	m_CKOUT.InsertString(m_CKOUT.GetCount(), L"0 = Output system CLK on Port pin");
	m_CKOUT.InsertString(m_CKOUT.GetCount(), L"1 = Disabled Output system CLK");
	//CLK Setting
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0000 = Ext.CLK,StartupTime:6CK");			// 0 = 0x00
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0000 = Ext.CLK,StartupTime:6CK+4.1ms");  	// 1 = 0x10
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0000 = Ext.CLK,StartupTime:6CK+65ms");  	// 2 = 0x20
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0010 = Int.RC OSC:8MHz,StartupTime:6CK"); 		// 3 = 0x02
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0010 = Int.RC OSC:8MHz,StartupTime:6CK+4.1ms"); 	// 4 = 0x12
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0010 = Int.RC OSC:8MHz,StartupTime:6CK+64ms");  	// 5 = 0xx22
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0110 = Ext.full-swing crystal,StartupTime:1KCK");  		// 6 = 0x06
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0110 = Ext.full-swing crystal,StartupTime:1KCK+4.1ms"); // 7 = 0x16
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0110 = Ext.full-swing crystal,StartupTime:1KCK+65ms"); 	// 8 = 0x26
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0111 = Ext.full-swing crystal,StartupTime:32KCK"); 	// 9 = 0x07
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0111 = Ext.full-swing crystal,StartupTime:32KCK+4.1ms"); 		// 10 = 0x17
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0111 = Ext.full-swing crystal,StartupTime:32KCK+65ms"); // 11 = 0x27
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1000 = Ext.Crystal OSC:0.4-0.9MHz,StartupTime:258CK+4.1ms"); // 12 = 0x08
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1000 = Ext.Crystal OSC:0.4-0.9MHz,StartupTime:258CK+65ms"); 	// 13 = 0x18
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1000 = Ext.Crystal OSC:0.4-0.9MHz,StartupTime:1KCK"); 		// 14 = 0x28
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1000 = Ext.Crystal OSC:0.4-0.9MHz,StartupTime:1KCK+4.1ms");  // 15 = 0x38
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1001 = Ext.Crystal OSC:0.4-0.9MHz,StartupTime:1KCK+65ms"); 	// 16 = 0x09
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1001 = Ext.Crystal OSC:0.4-0.9MHz,StartupTime:16KCK"); 		// 17 = 0x19
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1001 = Ext.Crystal OSC:0.4-0.9MHz,StartupTime:16KCK+4.1ms"); // 18 = 0x29
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1001 = Ext.Crystal OSC:0.4-0.9MHz,StartupTime:16KCK+65ms");  // 19 = 0x39
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1010 = Ext.Crystal OSC:0.9-3.0MHz,StartupTime:258CK+4.1ms"); // 20 = 0x0A
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1010 = Ext.Crystal OSC:0.9-3.0MHz,StartupTime:258CK+65ms"); 	 // 21 = 0x1A
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1010 = Ext.Crystal OSC:0.9-3.0MHz,StartupTime:1KCK");			// 22 = 0x2A
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1010 = Ext.Crystal OSC:0.9-3.0MHz,StartupTime:1KCK+4.1ms");   // 23 = 0x3A
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1011 = Ext.Crystal OSC:0.9-3.0MHz,StartupTime:1KCK+65ms");	 // 24 = 0x0B
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1011 = Ext.Crystal OSC:0.9-3.0MHz,StartupTime:16KCK"); 	 	 // 25 = 0x1B
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1011 = Ext.Crystal OSC:0.9-3.0MHz,StartupTime:16KCK+4.1ms");	 // 26 = 0x2B
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1011 = Ext.Crystal OSC:0.9-3.0MHz,StartupTime:16KCK+65ms");   // 27 = 0x3B
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1100 = Ext.Crystal OSC:3-8MHz,StartupTime:258C+4.1ms");	 // 28 = 0x0C
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1100 = Ext.Crystal OSC:3-8MHz,StartupTime:258CK+65ms"); 	 	 // 29 = 0x1C
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1100 = Ext.Crystal OSC:3-8MHz,StartupTime:1KCK");	 		 // 30 = 0x2C
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1100 = Ext.Crystal OSC:3-8MHz,StartupTime:1KCK+4.1ms");   	 // 31 = 0x3C
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1101 = Ext.Crystal OSC:3-8MHz,StartupTime:1KCK+65ms");	 	// 32 = 0x0D
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1101 = Ext.Crystal OSC:3-8MHz,StartupTime:16KCK"); 	 	 	// 33 = 0x1D
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1101 = Ext.Crystal OSC:3-8MHz,StartupTime:16KCK+4.1ms");	 	// 34 = 0x2D
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1101 = Ext.Crystal OSC:3-8MHz,StartupTime:16KCK+65ms");   	// 35 = 0x3D
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1110 = Ext.Crystal OSC:>8MHz,StartupTime:258CK+4.1ms");		// 36 = 0x0E
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1110 = Ext.Crystal OSC:>8MHz,StartupTime:258CK+65ms"); 	 	// 37 = 0x1E
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1110 = Ext.Crystal OSC:>8MHz,StartupTime:1KCK");	 			// 38 = 0x2E
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1110 = Ext.Crystal OSC:>8MHz,StartupTime:1KCK+4.1ms");   	// 39 = 0x3E
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1111 = Ext.Crystal OSC:>8MHz,StartupTime:1KCK+65ms");		// 40 = 0x0F
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1111 = Ext.Crystal OSC:>8MHz,StartupTime:16KCK"); 	 			// 41 = 0x1F
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1111 = Ext.Crystal OSC:>8MHz,StartupTime:16KCK+4.1ms");	 	// 42 = 0x2F
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1111 = Ext.Crystal OSC:>8MHz,StartupTime:16KCK+65ms");   	// 43 = 0x3F
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
	m_WDTON.SetDroppedWidth(300);
	m_CKSEL.SetDroppedWidth(500);
	m_BLBB.SetDroppedWidth(300);
	m_BLBA.SetDroppedWidth(300);
	m_LOCK.SetDroppedWidth(300);
	//ExtenedFuse
	m_BODLevel.SetCurSel(3);
	m_RSTDISBL.SetCurSel(1);
	//HighFuse
	m_OCDEN.SetCurSel(1);
	m_JTAGEN.SetCurSel(0);
	m_SPIEN.SetCurSel(0);
	m_WDTON.SetCurSel(1);
	m_EESAVE.SetCurSel(1);
	m_BOOTSZ.SetCurSel(0);
	m_BOOTRST.SetCurSel(1);
	//LowFuse
	m_CKDIV8.SetCurSel(0);
	m_CKOUT.SetCurSel(1);
	m_CKSEL.SetCurSel(5);
	//Lockbits
	m_BLBB.SetCurSel(3);
	m_BLBA.SetCurSel(3);
	m_LOCK.SetCurSel(2);
	//Disable Window
	m_BODLevel.EnableWindow(FALSE);
	m_RSTDISBL.EnableWindow(FALSE);
	m_OCDEN.EnableWindow(FALSE);
	m_JTAGEN.EnableWindow(FALSE);
	m_SPIEN.EnableWindow(FALSE);
	m_WDTON.EnableWindow(FALSE);
	m_EESAVE.EnableWindow(FALSE);
	m_BOOTSZ.EnableWindow(FALSE);
	m_BOOTRST.EnableWindow(FALSE);
	m_CKDIV8.EnableWindow(FALSE);
	m_CKOUT.EnableWindow(FALSE);
	m_CKSEL.EnableWindow(FALSE);
	m_BLBB.EnableWindow(FALSE);
	m_BLBA.EnableWindow(FALSE);
	m_LOCK.EnableWindow(FALSE);
	return TRUE;
}

BOOL CDlg_ATmegaX5X9::OnSetActive()
{
	if (*m_pDataLen != NULL)
	{
		CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
		Encheck->SetCheck(1);
		m_BODLevel.EnableWindow(TRUE);
		m_RSTDISBL.EnableWindow(TRUE);
		m_OCDEN.EnableWindow(TRUE);
		m_JTAGEN.EnableWindow(TRUE);
		m_SPIEN.EnableWindow(FALSE);
		m_WDTON.EnableWindow(TRUE);
		m_EESAVE.EnableWindow(TRUE);
		m_BOOTSZ.EnableWindow(TRUE);
		m_BOOTRST.EnableWindow(TRUE);
		m_CKDIV8.EnableWindow(TRUE);
		m_CKOUT.EnableWindow(TRUE);
		m_CKSEL.EnableWindow(TRUE);
		m_BLBB.EnableWindow(TRUE);
		m_BLBA.EnableWindow(TRUE);
		m_LOCK.EnableWindow(TRUE);
		CString ExF_str, HF_str, LF_str, L_str;
		//Extended Fuse
		memcpy((void*)&ExtendedFuse, m_RegBuff, sizeof(ExtendedFuse));
		ExF_str.Format(_T("%02X"), ExtendedFuse);
		m_ExFuse.SetWindowTextW(ExF_str);
		//HighFuse
		memcpy((void*)&HighFuse, m_RegBuff + sizeof(ExtendedFuse), sizeof(HighFuse));
		HF_str.Format(_T("%02X"), HighFuse);
		m_HFuse.SetWindowTextW(HF_str);
		//LowFuse
		memcpy((void*)&LowFuse, m_RegBuff + sizeof(ExtendedFuse)+sizeof(HighFuse), sizeof(LowFuse));
		LF_str.Format(_T("%02X"), LowFuse);
		m_LFuse.SetWindowTextW(LF_str);
		//Lockbits
		memcpy((void*)&Lockbits, m_RegBuff + sizeof(ExtendedFuse)+sizeof(HighFuse)+sizeof(LowFuse), sizeof(Lockbits));
		L_str.Format(_T("%02X"), Lockbits);
		m_Lockbits.SetWindowTextW(L_str);
		//BODLevel
		if ((ExtendedFuse & 0x06) == 0x06)
			m_BODLevel.SetCurSel(3);
		else if ((ExtendedFuse & 0x06) == 0x04)
			m_BODLevel.SetCurSel(2);
		else if ((ExtendedFuse & 0x06) == 0x02)
			m_BODLevel.SetCurSel(1);
		else if ((ExtendedFuse & 0x06) == 0x00)
			m_BODLevel.SetCurSel(0);
		//RSTDISBL
		if ((ExtendedFuse & 0x01) == 0x01)
			m_RSTDISBL.SetCurSel(1);
		else if ((ExtendedFuse & 0x01) == 0x00)
			m_RSTDISBL.SetCurSel(0);
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
		//WDTON
		if ((HighFuse & 0x10) == 0x10)
			m_WDTON.SetCurSel(1);
		else if ((HighFuse & 0x10) == 0x00)
			m_WDTON.SetCurSel(0);
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
		//CKDIV8
		if ((LowFuse & 0x80) == 0x80)
			m_CKDIV8.SetCurSel(1);
		else if ((LowFuse & 0x80) == 0x00)
			m_CKDIV8.SetCurSel(0);
		//CKOUT
		if ((LowFuse & 0x40) == 0x40)
			m_CKOUT.SetCurSel(1);
		else if ((LowFuse & 0x40) == 0x00)
			m_CKOUT.SetCurSel(0);
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
		case 0x02:
			m_CKSEL.SetCurSel(3);
			break;
		case 0x12:
			m_CKSEL.SetCurSel(4);
			break;
		case 0x22:
			m_CKSEL.SetCurSel(5);
			break;
		case 0x06:
			m_CKSEL.SetCurSel(6);
			break;
		case 0x16:
			m_CKSEL.SetCurSel(7);
			break;
		case 0x26:
			m_CKSEL.SetCurSel(8);
			break;
		case 0x07:
			m_CKSEL.SetCurSel(9);
			break;
		case 0x17:
			m_CKSEL.SetCurSel(10);
			break;
		case 0x27:
			m_CKSEL.SetCurSel(11);
			break;
		case 0x08:
			m_CKSEL.SetCurSel(12);
			break;
		case 0x18:
			m_CKSEL.SetCurSel(13);
			break;
		case 0x28:
			m_CKSEL.SetCurSel(14);
			break;
		case 0x38:
			m_CKSEL.SetCurSel(15);
			break;
		case 0x09:
			m_CKSEL.SetCurSel(16);
			break;
		case 0x19:
			m_CKSEL.SetCurSel(17);
			break;
		case 0x29:
			m_CKSEL.SetCurSel(18);
			break;
		case 0x39:
			m_CKSEL.SetCurSel(19);
			break;
		case 0x0A:
			m_CKSEL.SetCurSel(20);
			break;
		case 0x1A:
			m_CKSEL.SetCurSel(21);
			break;
		case 0x2A:
			m_CKSEL.SetCurSel(22);
			break;
		case 0x3A:
			m_CKSEL.SetCurSel(23);
			break;
		case 0x0B:
			m_CKSEL.SetCurSel(24);
			break;
		case 0x1B:
			m_CKSEL.SetCurSel(25);
			break;
		case 0x2B:
			m_CKSEL.SetCurSel(26);
			break;
		case 0x3B:
			m_CKSEL.SetCurSel(27);
			break;
		case 0x0C:
			m_CKSEL.SetCurSel(28);
			break;
		case 0x1C:
			m_CKSEL.SetCurSel(29);
			break;
		case 0x2C:
			m_CKSEL.SetCurSel(30);
			break;
		case 0x3C:
			m_CKSEL.SetCurSel(31);
			break;
		case 0x0D:
			m_CKSEL.SetCurSel(32);
			break;
		case 0x1D:
			m_CKSEL.SetCurSel(33);
			break;
		case 0x2D:
			m_CKSEL.SetCurSel(34);
			break;
		case 0x3D:
			m_CKSEL.SetCurSel(35);
			break;
		case 0x0E:
			m_CKSEL.SetCurSel(36);
			break;
		case 0x1E:
			m_CKSEL.SetCurSel(37);
			break;
		case 0x2E:
			m_CKSEL.SetCurSel(38);
			break;
		case 0x3E:
			m_CKSEL.SetCurSel(39);
			break;
		case 0x0F:
			m_CKSEL.SetCurSel(40);
			break;
		case 0x1F:
			m_CKSEL.SetCurSel(41);
			break;
		case 0x2F:
			m_CKSEL.SetCurSel(42);
			break;
		case 0x3F:
			m_CKSEL.SetCurSel(43);
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
		m_BODLevel.EnableWindow(FALSE);
		m_RSTDISBL.EnableWindow(FALSE);
		m_OCDEN.EnableWindow(FALSE);
		m_JTAGEN.EnableWindow(FALSE);
		m_SPIEN.EnableWindow(FALSE);
		m_WDTON.EnableWindow(FALSE);
		m_EESAVE.EnableWindow(FALSE);
		m_BOOTSZ.EnableWindow(FALSE);
		m_BOOTRST.EnableWindow(FALSE);
		m_CKDIV8.EnableWindow(FALSE);
		m_CKOUT.EnableWindow(FALSE);
		m_CKSEL.EnableWindow(FALSE);
		m_BLBB.EnableWindow(FALSE);
		m_BLBA.EnableWindow(FALSE);
		m_LOCK.EnableWindow(FALSE);
	}
	return CMFCPropertyPage::OnSetActive();
}

void CDlg_ATmegaX5X9::OnCbnSelchangeCombo()
{
	CString ExFuse_str, HFuse_str, LFuse_str, Lockbits_str;
	ExtendedFuse = 0xFF;
	HighFuse = 0x99;
	LowFuse = 0x62;
	Lockbits = 0xFF;
	//BODLevel
	ExtendedFuse &= 0xF9;
	if (m_BODLevel.GetCurSel() == 3)
		ExtendedFuse |= 0x06;
	else if (m_BODLevel.GetCurSel() == 2)
		ExtendedFuse |= 0x04;
	else if (m_BODLevel.GetCurSel() == 1)
		ExtendedFuse |= 0x02;
	else if (m_BODLevel.GetCurSel() == 0)
		ExtendedFuse |= 0x00;
	ExFuse_str.Format(_T("%02X"), ExtendedFuse);
	m_ExFuse.SetWindowTextW(ExFuse_str);
	//RSTDISBL
	ExtendedFuse &= 0xFE;
	if (m_RSTDISBL.GetCurSel() == 1)
		ExtendedFuse |= 0x01;
	else if (m_RSTDISBL.GetCurSel() == 0)
		ExtendedFuse |= 0x00;
	ExFuse_str.Format(_T("%02X"), ExtendedFuse);
	m_ExFuse.SetWindowTextW(ExFuse_str);
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
	//WDTON
	HighFuse &= 0xEF;
	if (m_WDTON.GetCurSel() == 1)
		HighFuse |= 0x10;
	else if (m_WDTON.GetCurSel() == 0)
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
	//CKDIV8
	LowFuse &= 0x7F;
	if (m_CKDIV8.GetCurSel() == 1)
		LowFuse |= 0x80;
	else if (m_CKDIV8.GetCurSel() == 0)
		LowFuse |= 0x00;
	LFuse_str.Format(_T("%02X"), LowFuse);
	m_LFuse.SetWindowTextW(LFuse_str);
	//CKOUT
	LowFuse &= 0xBF;
	if (m_CKOUT.GetCurSel() == 1)
		LowFuse |= 0x40;
	else if (m_CKOUT.GetCurSel() == 0)
		LowFuse |= 0x00;
	LFuse_str.Format(_T("%02X"), LowFuse);
	m_LFuse.SetWindowTextW(LFuse_str);
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
		LowFuse |= 0x02;
		break;
	case 4:
		LowFuse |= 0x12;
		break;
	case 5:
		LowFuse |= 0x22;
		break;
	case 6:
		LowFuse |= 0x06;
		break;
	case 7:
		LowFuse |= 0x16;
		break;
	case 8:
		LowFuse |= 0x26;
		break;
	case 9:
		LowFuse |= 0x07;
		break;
	case 10:
		LowFuse |= 0x17;
		break;
	case 11:
		LowFuse |= 0x27;
		break;
	case 12:
		LowFuse |= 0x08;
		break;
	case 13:
		LowFuse |= 0x18;
		break;
	case 14:
		LowFuse |= 0x28;
		break;
	case 15:
		LowFuse |= 0x38;
		break;
	case 16:
		LowFuse |= 0x09;
		break;
	case 17:
		LowFuse |= 0x19;
		break;
	case 18:
		LowFuse |= 0x29;
		break;
	case 19:
		LowFuse |= 0x39;
		break;
	case 20:
		LowFuse |= 0x0A;
		break;
	case 21:
		LowFuse |= 0x1A;
		break;
	case 22:
		LowFuse |= 0x2A;
		break;
	case 23:
		LowFuse |= 0x3A;
		break;
	case 24:
		LowFuse |= 0x0B;
		break;
	case 25:
		LowFuse |= 0x1B;
		break;
	case 26:
		LowFuse |= 0x2B;
		break;
	case 27:
		LowFuse |= 0x3B;
		break;
	case 28:
		LowFuse |= 0x0C;
		break;
	case 29:
		LowFuse |= 0x1C;
		break;
	case 30:
		LowFuse |= 0x2C;
		break;
	case 31:
		LowFuse |= 0x3C;
		break;
	case 32:
		LowFuse |= 0x0D;
		break;
	case 33:
		LowFuse |= 0x1D;
		break;
	case 34:
		LowFuse |= 0x2D;
		break;
	case 35:
		LowFuse |= 0x3D;
		break;
	case 36:
		LowFuse |= 0x0E;
		break;
	case 37:
		LowFuse |= 0x1E;
		break;
	case 38:
		LowFuse |= 0x2E;
		break;
	case 39:
		LowFuse |= 0x3E;
		break;
	case 40:
		LowFuse |= 0x0F;
		break;
	case 41:
		LowFuse |= 0x1F;
		break;
	case 42:
		LowFuse |= 0x2F;
		break;
	case 43:
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

void CDlg_ATmegaX5X9::OnOK()
{
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	int state = Encheck->GetCheck();
	if (state == 1)
	{
		CString temp;
		m_ExFuse.GetWindowTextW(temp);
		ExtendedFuse = numeric_conversion::hexstring_to_size_t(temp.GetString());
		memcpy(m_RegBuff, &ExtendedFuse, sizeof(ExtendedFuse));

		m_HFuse.GetWindowTextW(temp);
		HighFuse = numeric_conversion::hexstring_to_size_t(temp.GetString());
		memcpy(m_RegBuff + sizeof(ExtendedFuse), &HighFuse, sizeof(HighFuse));

		m_LFuse.GetWindowTextW(temp);
		LowFuse = numeric_conversion::hexstring_to_size_t(temp.GetString());
		memcpy(m_RegBuff + sizeof(ExtendedFuse)+sizeof(HighFuse), &LowFuse, sizeof(LowFuse));

		m_Lockbits.GetWindowTextW(temp);
		Lockbits = numeric_conversion::hexstring_to_size_t(temp.GetString());
		memcpy(m_RegBuff + sizeof(ExtendedFuse)+sizeof(HighFuse)+sizeof(LowFuse), &Lockbits, sizeof(Lockbits));
		*m_pDataLen = sizeof(ExtendedFuse)+sizeof(HighFuse)+sizeof(LowFuse)+sizeof(Lockbits);
	}
	else if (state == 0)
		*m_pDataLen = 0;
	CMFCPropertyPage::OnOK();
}
LRESULT CDlg_ATmegaX5X9::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();

}
LRESULT CDlg_ATmegaX5X9::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}

void CDlg_ATmegaX5X9::OnBnClickedEnable()
{
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	int state = Encheck->GetCheck();
	if (state == 1)
	{
		m_BODLevel.EnableWindow(TRUE);
		m_RSTDISBL.EnableWindow(TRUE);
		m_OCDEN.EnableWindow(TRUE);
		m_JTAGEN.EnableWindow(TRUE);
		m_SPIEN.EnableWindow(FALSE);
		m_WDTON.EnableWindow(TRUE);
		m_EESAVE.EnableWindow(TRUE);
		m_BOOTSZ.EnableWindow(TRUE);
		m_BOOTRST.EnableWindow(TRUE);
		m_CKDIV8.EnableWindow(TRUE);
		m_CKOUT.EnableWindow(TRUE);
		m_CKSEL.EnableWindow(TRUE);
		m_BLBB.EnableWindow(TRUE);
		m_BLBA.EnableWindow(TRUE);
		m_LOCK.EnableWindow(TRUE);
	}
	else if (state == 0)
	{
		m_BODLevel.EnableWindow(FALSE);
		m_RSTDISBL.EnableWindow(FALSE);
		m_OCDEN.EnableWindow(FALSE);
		m_JTAGEN.EnableWindow(FALSE);
		m_SPIEN.EnableWindow(FALSE);
		m_WDTON.EnableWindow(FALSE);
		m_EESAVE.EnableWindow(FALSE);
		m_BOOTSZ.EnableWindow(FALSE);
		m_BOOTRST.EnableWindow(FALSE);
		m_CKDIV8.EnableWindow(FALSE);
		m_CKOUT.EnableWindow(FALSE);
		m_CKSEL.EnableWindow(FALSE);
		m_BLBB.EnableWindow(FALSE);
		m_BLBA.EnableWindow(FALSE);
		m_LOCK.EnableWindow(FALSE);
	}
}