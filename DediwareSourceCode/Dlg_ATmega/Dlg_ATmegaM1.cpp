// Dlg_ATmegaM1.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_ATmegaM1.h"
#include "afxdialogex.h"
#include "numeric_conversion.h"
#include "../common/prog_info.h"

// Dlg_ATmegaM1 dialog

IMPLEMENT_DYNAMIC(CDlg_ATmegaM1, CMFCPropertyPage)

CDlg_ATmegaM1::CDlg_ATmegaM1(struct chip_info_c *ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_ATmegaM1::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_ATmegaM1::~CDlg_ATmegaM1()
{
}

void CDlg_ATmegaM1::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, Option_enable);
	DDX_Control(pDX, IDC_COMBO1, m_PSCRB);
	DDX_Control(pDX, IDC_COMBO2, m_PSCRVA);
	DDX_Control(pDX, IDC_COMBO3, m_PSCRVB);
	DDX_Control(pDX, IDC_COMBO4, m_BODLevel);
	DDX_Control(pDX, IDC_COMBO5, m_RSTDISBL);
	DDX_Control(pDX, IDC_COMBO6, m_DWEN);
	DDX_Control(pDX, IDC_COMBO7, m_SPIEN);
	DDX_Control(pDX, IDC_COMBO8, m_WDTON);
	DDX_Control(pDX, IDC_COMBO9, m_EESAVE);
	DDX_Control(pDX, IDC_COMBO10, m_BOOTSZ);
	DDX_Control(pDX, IDC_COMBO11, m_BOOTRST);
	DDX_Control(pDX, IDC_COMBO12, m_CKDIV8);
	DDX_Control(pDX, IDC_COMBO13, m_CKOUT);
	DDX_Control(pDX, IDC_COMBO14, m_CKSEL);
	DDX_Control(pDX, IDC_COMBO15, m_BLBB);
	DDX_Control(pDX, IDC_COMBO16, m_BLBA);
	DDX_Control(pDX, IDC_COMBO17, m_LOCK);
	DDX_Control(pDX, IDC_EDIT1, m_ExFuse);
	DDX_Control(pDX, IDC_EDIT2, m_HFuse);
	DDX_Control(pDX, IDC_EDIT3, m_LFuse);
	DDX_Control(pDX, IDC_EDIT4, m_Lockbits);
}


BEGIN_MESSAGE_MAP(CDlg_ATmegaM1, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_ATmegaM1::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_ATmegaM1::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_ATmegaM1::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_ATmegaM1::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_ATmegaM1::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDlg_ATmegaM1::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDlg_ATmegaM1::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CDlg_ATmegaM1::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO9, &CDlg_ATmegaM1::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO10, &CDlg_ATmegaM1::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO11, &CDlg_ATmegaM1::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO12, &CDlg_ATmegaM1::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO13, &CDlg_ATmegaM1::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO14, &CDlg_ATmegaM1::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO15, &CDlg_ATmegaM1::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO16, &CDlg_ATmegaM1::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO17, &CDlg_ATmegaM1::OnCbnSelchangeCombo)
	ON_BN_CLICKED(IDC_CHECK1, &CDlg_ATmegaM1::OnBnClickedEnable)

END_MESSAGE_MAP()


// Dlg_ATmegaM1 message handlers
BOOL CDlg_ATmegaM1::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	ExtendedFuse = 0xFF;
	HighFuse = 0xD9;
	LowFuse = 0x62;
	Lockbits = 0xFF;
	m_ExFuse.SetWindowTextW(L"FF");
	m_HFuse.SetWindowTextW(L"D9");
	m_LFuse.SetWindowTextW(L"62");
	m_Lockbits.SetWindowTextW(L"FF");
	//Extended Fuse
	m_PSCRB.InsertString(m_PSCRB.GetCount(), L"0 = PSC reset enable");
	m_PSCRB.InsertString(m_PSCRB.GetCount(), L"1 = PSC reset Disable");//Initial
	m_PSCRVA.InsertString(m_PSCRVA.GetCount(), L"0 = PSCOUTnA reset value");
	m_PSCRVA.InsertString(m_PSCRVA.GetCount(), L"1 = PSCOUTnA reset value");//Initial
	m_PSCRVB.InsertString(m_PSCRVB.GetCount(), L"0 = PSCOUTnB reset value");
	m_PSCRVB.InsertString(m_PSCRVB.GetCount(), L"1 = PSCOUTnB reset value");//Initial
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"000 = Trigger Level at 2.6V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"001 = Trigger Level at 2.8V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"010 = Trigger Level at 4.2V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"011 = Trigger Level at 4.4V");//initial value
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"100 = Trigger Level at 4.3V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"101 = Trigger Level at 2.7V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"110 = Trigger Level at 4.5V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"111 = BOD Disable");//initial value
	//High Fuse
	m_RSTDISBL.InsertString(m_RSTDISBL.GetCount(), L"0 = Enabled external reset");
	m_RSTDISBL.InsertString(m_RSTDISBL.GetCount(), L"1 = Disabled external reset");//initial value
	m_DWEN.InsertString(m_DWEN.GetCount(), L"0 = debugWIRE Enable");
	m_DWEN.InsertString(m_DWEN.GetCount(), L"1 = debugWIRE Disable");//initial value
	m_SPIEN.InsertString(m_SPIEN.GetCount(), L"0 = Enabled ISP Programming");//initial value
	m_SPIEN.InsertString(m_SPIEN.GetCount(), L"1 = Disabled ISP Programming");
	m_WDTON.InsertString(m_WDTON.GetCount(), L"0 = Enable Watchdog Timer always on");
	m_WDTON.InsertString(m_WDTON.GetCount(), L"1 = Disable Watchdog Timer always on");//initial value
	m_EESAVE.InsertString(m_EESAVE.GetCount(), L"0 = Preserve EEPROM during Chip Erase");
	m_EESAVE.InsertString(m_EESAVE.GetCount(), L"1 = Unreserves EEPROM during Chip Erase");//initial value
	m_BOOTSZ.InsertString(m_BOOTSZ.GetCount(), L"00 = Boot Size 2048 words");//initial value
	m_BOOTSZ.InsertString(m_BOOTSZ.GetCount(), L"01 = Boot Size 1024 words");
	m_BOOTSZ.InsertString(m_BOOTSZ.GetCount(), L"10 = Boot Size 512 words");
	m_BOOTSZ.InsertString(m_BOOTSZ.GetCount(), L"11 = Boot Size 256 words");
	m_BOOTRST.InsertString(m_BOOTRST.GetCount(), L"0 = Enabled boot reset vector");
	m_BOOTRST.InsertString(m_BOOTRST.GetCount(), L"1 = Disabled boot reset vector");//initial value
	//Low Fuse
	m_CKDIV8.InsertString(m_CKDIV8.GetCount(), L"0 = Divide CLK by 8 Enable");
	m_CKDIV8.InsertString(m_CKDIV8.GetCount(), L"1 = Divide CLK by 8 Disable");
	m_CKOUT.InsertString(m_CKOUT.GetCount(), L"0 = Output system CLK on Port pin");
	m_CKOUT.InsertString(m_CKOUT.GetCount(), L"1 = Disabled Output system CLK");
	//CLK Setting
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0000 = Ext.CLK,StartupTime:6CK/14CK(PWRDWN/RST)");			// 0 = 0x00
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0000 = Ext.CLK,StartupTime:6CK/14CK+4.1ms(PWRDWN/RST)");  	// 1 = 0x10
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0000 = Ext.CLK,StartupTime:6CK/14CK+65ms(PWRDWN/RST)");  		// 2 = 0x20
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0001 = PLL.CLK/4,PLL.input Ext.CLK,StartupTime:6CK/14CK(PWRDWN/RST)");		// 3 = 0x01
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0001 = PLL.CLK/4,PLL.input Ext.CLK,StartupTime:6CK/14CK+4.1ms(PWRDWN/RST)");  // 4 = 0x11
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0001 = PLL.CLK/4,PLL.input Ext.CLK,StartupTime:6CK/14CK+65ms(PWRDWN/RST)");  	// 5 = 0x21
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0010 = Int.RC OSC:8MHz,StartupTime:6CK/14CK(PWRDWN/RST)"); 		// 6 = 0x02
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0010 = Int.RC OSC:8MHz,StartupTime:6CK/14CK+4.1ms(PWRDWN/RST)"); 	// 7 = 0x12
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0010 = Int.RC OSC:8MHz,StartupTime:6CK/14CK+65ms(PWRDWN/RST)");  	// 8 = 0xx22
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0011 = PLL.CLK:16MHz,StartupTime:1KCK/14CK(PWRDWN/RST)"); 		// 9 = 0x03
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0011 = PLL.CLK:16MHz,StartupTime:1KCK/14CK+4.1ms(PWRDWN/RST)"); 	// 10 = 0x13
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0011 = PLL.CLK:16MHz,StartupTime:1KCK/14CK+65ms(PWRDWN/RST)");  	// 11 = 0x23
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,0011 = PLL.CLK:16MHz,StartupTime:16KCK/14CK(PWRDWN/RST)");  		// 12 = 0x33
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0100 = Ext/PLL.input.crystal.Resonator,StartupTime:1KCK/14CK(PWRDWN/RST)"); 		// 13 = 0x04
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0100 = Ext/PLL.input.crystal.Resonator,StartupTime:1KCK/14CK+4ms(PWRDWN/RST)"); 	// 14 = 0x14
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0100 = Ext/PLL.input.crystal.Resonator,StartupTime:1KCK/14CK+65ms(PWRDWN/RST)");  // 15 = 0x24
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,0100 = Ext/PLL.input.crystal.Resonator,StartupTime:16KCK/14CK(PWRDWN/RST)");  	// 16 = 0x34
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0101 = PLL/4,input-Ext.Crystal.Resonator,StartupTime:1KCK/14CK(PWRDWN/RST)"); 	// 17 = 0x05
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0101 = PLL/4,input-Ext.Crystal.Resonator,StartupTime:1KCK/14CK+4(PWRDWN/RST)"); 	// 18 = 0x15
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0101 = PLL/4,input-Ext.Crystal.Resonator,StartupTime:1KCK/14CK+65ms(PWRDWN/RST)");// 19 = 0x25
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,0101 = PLL/4,input-Ext.Crystal.Resonator,StartupTime:16KCK/14CK(PWRDWN/RST)");  	// 20 = 0x35
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1000 = Ext.Crystal.OSC:0.4-0.9MHz,StartupTime:258CK/14CK+4.1ms(PWRDWN/RST)"); // 21 = 0x08
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1000 = Ext.Crystal.OSC:0.4-0.9MHz,StartupTime:258CK/14CK+65ms(PWRDWN/RST)");  // 22 = 0x18
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1000 = Ext.Crystal.OSC:0.4-0.9MHz,StartupTime:1KCK/14CK(PWRDWN/RST)"); 		// 23 = 0x28
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1000 = Ext.Crystal.OSC:0.4-0.9MHz,StartupTime:1KCK/14CK+4.1ms(PWRDWN/RST)");  // 24 = 0x38
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1001 = Ext.Crystal.OSC:0.4-0.9MHz,StartupTime:1KCK/14CK+65ms(PWRDWN/RST)"); 	// 25 = 0x09
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1001 = Ext.Crystal.OSC:0.4-0.9MHz,StartupTime:16KCK/14CK(PWRDWN/RST)"); 		// 26 = 0x19
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1001 = Ext.Crystal.OSC:0.4-0.9MHz,StartupTime:16KCK/14CK+4.1ms(PWRDWN/RST)"); // 27 = 0x29
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1001 = Ext.Crystal.OSC:0.4-0.9MHz,StartupTime:16KCK/14CK+65ms(PWRDWN/RST)");  // 28 = 0x39
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1010 = Ext.Crystal.OSC:0.9-3.0MHz,StartupTime:258CK/14CK+4.1ms(PWRDWN/RST)"); // 29 = 0x0A
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1010 = Ext.Crystal.OSC:0.9-3.0MHz,StartupTime:258CK/14CK+65ms(PWRDWN/RST)"); 	// 30 = 0x1A
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1010 = Ext.Crystal.OSC:0.9-3.0MHz,StartupTime:1KCK/14CK(PWRDWN/RST)");		// 31 = 0x2A
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1010 = Ext.Crystal.OSC:0.9-3.0MHz,StartupTime:1KCK/14CK+4.1ms(PWRDWN/RST)");  // 32 = 0x3A
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1011 = Ext.Crystal.OSC:0.9-3.0MHz,StartupTime:1KCK/14CK+65ms(PWRDWN/RST)");	// 33 = 0x0B
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1011 = Ext.Crystal.OSC:0.9-3.0MHz,StartupTime:16KCK/14CK(PWRDWN/RST)"); 	 	// 34 = 0x1B
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1011 = Ext.Crystal.OSC:0.9-3.0MHz,StartupTime:16KCK/14CK+4.1ms(PWRDWN/RST)");	// 35 = 0x2B
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1011 = Ext.Crystal.OSC:0.9-3.0MHz,StartupTime:16KCK/14CK+65ms(PWRDWN/RST)");  // 36 = 0x3B
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1100 = Ext.Crystal.OSC:3-8MHz,StartupTime:258CK/14CK+4.1ms((PWRDWN/RST)");	// 37 = 0x0C
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1100 = Ext.Crystal.OSC:3-8MHz,StartupTime:258CK/14CK+65ms(PWRDWN/RST)"); 	 	// 38 = 0x1C
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1100 = Ext.Crystal.OSC:3-8MHz,StartupTime:1KCK/14CK(PWRDWN/RST)");	 		// 39 = 0x2C
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1100 = Ext.Crystal.OSC:3-8MHz,StartupTime:1KCK/14CK+4.1ms(PWRDWN/RST)");   	// 40 = 0x3C
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1101 = Ext.Crystal.OSC:3-8MHz,StartupTime:1KCK/14CK+65ms(PWRDWN/RST)");	 	// 41 = 0x0D
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1101 = Ext.Crystal.OSC:3-8MHz,StartupTime:16KCK/14CK(PWRDWN/RST)"); 	 	 	// 42 = 0x1D
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1101 = Ext.Crystal.OSC:3-8MHz,StartupTime:16KCK/14CK+4.1ms(PWRDWN/RST)");	 	// 43 = 0x2D
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1101 = Ext.Crystal.OSC:3-8MHz,StartupTime:16KCK/14CK+65ms(PWRDWN/RST)");   	// 44 = 0x3D
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1110 = Ext.Crystal.OSC:>8MHz,StartupTime:258CK/14CK+4.1ms(PWRDWN/RST)");		// 45 = 0x0E
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1110 = Ext.Crystal.OSC:>8MHz,StartupTime:258CK/14CK+65ms(PWRDWN/RST)"); 	 	// 46 = 0x1E
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1110 = Ext.Crystal.OSC:>8MHz,StartupTime:1KCK/14CK(PWRDWN/RST)");	 			// 47 = 0x2E
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1110 = Ext.Crystal.OSC:>8MHz,StartupTime:1KCK/14CK+4.1ms(PWRDWN/RST)");   	// 48 = 0x3E
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1111 = Ext.Crystal.OSC:>8MHz,StartupTime:1KCK/14CK+65ms(PWRDWN/RST)");		// 49 = 0x0F
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1111 = Ext.Crystal.OSC:>8MHz,StartupTime:16KCK/14CK(PWRDWN/RST)"); 	 		// 50 = 0x1F
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1111 = Ext.Crystal.OSC:>8MHz,StartupTime:16KCK/14CK+4.1ms(PWRDWN/RST)");	 	// 51 = 0x2F
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1111 = Ext.Crystal.OSC:>8MHz,StartupTime:16KCK/14CK+65ms(PWRDWN/RST)");   	// 52 = 0x3F
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
	m_WDTON.SetDroppedWidth(300);
	m_EESAVE.SetDroppedWidth(300);
	m_CKSEL.SetDroppedWidth(600);
	m_BLBB.SetDroppedWidth(300);
	m_BLBA.SetDroppedWidth(300);
	m_LOCK.SetDroppedWidth(300);
	//Set Combobox initial selection
	//ExtenedFuse
	m_PSCRB.SetCurSel(1);
	m_PSCRVA.SetCurSel(1);
	m_PSCRVB.SetCurSel(1);
	m_BODLevel.SetCurSel(7);
	//HighFuse
	m_RSTDISBL.SetCurSel(1);
	m_DWEN.SetCurSel(1);
	m_SPIEN.SetCurSel(0);
	m_WDTON.SetCurSel(1);
	m_EESAVE.SetCurSel(1);
	m_BOOTSZ.SetCurSel(0);
	m_BOOTRST.SetCurSel(1);
	//LowFuse
	m_CKDIV8.SetCurSel(0);
	m_CKOUT.SetCurSel(1);
	m_CKSEL.SetCurSel(8);
	//Lockbits
	m_BLBB.SetCurSel(3);
	m_BLBA.SetCurSel(3);
	m_LOCK.SetCurSel(2);
	//Disable Screen
	m_PSCRB.EnableWindow(FALSE);
	m_PSCRVA.EnableWindow(FALSE);
	m_PSCRVB.EnableWindow(FALSE);
	m_BODLevel.EnableWindow(FALSE);
	m_RSTDISBL.EnableWindow(FALSE);
	m_DWEN.EnableWindow(FALSE);
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

BOOL CDlg_ATmegaM1::OnSetActive()
{
	if (*m_pDataLen != NULL)
	{
		CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
		Encheck->SetCheck(1);
		m_PSCRB.EnableWindow(TRUE);
		m_PSCRVA.EnableWindow(TRUE);
		m_PSCRVB.EnableWindow(TRUE);
		m_BODLevel.EnableWindow(TRUE);
		m_RSTDISBL.EnableWindow(TRUE);
		m_DWEN.EnableWindow(TRUE);
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
		//PSCRB
		if ((ExtendedFuse & 0x20) == 0x20)
			m_PSCRB.SetCurSel(1);
		else if ((ExtendedFuse & 0x20) == 0x00)
			m_PSCRB.SetCurSel(0);
		//PSCRVA
		if ((ExtendedFuse & 0x10) == 0x10)
			m_PSCRVA.SetCurSel(1);
		else if ((ExtendedFuse & 0x10) == 0x00)
			m_PSCRVA.SetCurSel(0);
		//PSCRVB
		if ((ExtendedFuse & 0x08) == 0x08)
			m_PSCRVB.SetCurSel(1);
		else if ((ExtendedFuse & 0x08) == 0x00)
			m_PSCRVB.SetCurSel(0);
		//BODLevel
		if ((ExtendedFuse & 0x07) == 0x07)
			m_BODLevel.SetCurSel(7);
		else if ((ExtendedFuse & 0x07) == 0x06)
			m_BODLevel.SetCurSel(6);
		else if ((ExtendedFuse & 0x07) == 0x05)
			m_BODLevel.SetCurSel(5);
		else if ((ExtendedFuse & 0x07) == 0x04)
			m_BODLevel.SetCurSel(4);
		else if ((ExtendedFuse & 0x07) == 0x03)
			m_BODLevel.SetCurSel(3);
		else if ((ExtendedFuse & 0x07) == 0x02)
			m_BODLevel.SetCurSel(2);
		else if ((ExtendedFuse & 0x07) == 0x01)
			m_BODLevel.SetCurSel(1);
		else if ((ExtendedFuse & 0x07) == 0x00)
			m_BODLevel.SetCurSel(0);
		//RSTDISBL
		if ((HighFuse & 0x80) == 0x80)
			m_RSTDISBL.SetCurSel(1);
		else if ((HighFuse & 0x80) == 0x00)
			m_RSTDISBL.SetCurSel(0);
		//DWEN
		if ((HighFuse & 0x40) == 0x40)
			m_DWEN.SetCurSel(1);
		else if ((HighFuse & 0x40) == 0x00)
			m_DWEN.SetCurSel(0);
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
		case 0x33:
			m_CKSEL.SetCurSel(12);
			break;
		case 0x04:
			m_CKSEL.SetCurSel(13);
			break;
		case 0x14:
			m_CKSEL.SetCurSel(14);
			break;
		case 0x24:
			m_CKSEL.SetCurSel(15);
			break;
		case 0x34:
			m_CKSEL.SetCurSel(16);
			break;
		case 0x05:
			m_CKSEL.SetCurSel(17);
			break;
		case 0x15:
			m_CKSEL.SetCurSel(18);
			break;
		case 0x25:
			m_CKSEL.SetCurSel(19);
			break;
		case 0x35:
			m_CKSEL.SetCurSel(20);
			break;
		case 0x08:
			m_CKSEL.SetCurSel(21);
			break;
		case 0x18:
			m_CKSEL.SetCurSel(22);
			break;
		case 0x28:
			m_CKSEL.SetCurSel(23);
			break;
		case 0x38:
			m_CKSEL.SetCurSel(24);
			break;
		case 0x09:
			m_CKSEL.SetCurSel(25);
			break;
		case 0x19:
			m_CKSEL.SetCurSel(26);
			break;
		case 0x29:
			m_CKSEL.SetCurSel(27);
			break;
		case 0x39:
			m_CKSEL.SetCurSel(28);
			break;
		case 0x0A:
			m_CKSEL.SetCurSel(29);
			break;
		case 0x1A:
			m_CKSEL.SetCurSel(30);
			break;
		case 0x2A:
			m_CKSEL.SetCurSel(31);
			break;
		case 0x3A:
			m_CKSEL.SetCurSel(32);
			break;
		case 0x0B:
			m_CKSEL.SetCurSel(33);
			break;
		case 0x1B:
			m_CKSEL.SetCurSel(34);
			break;
		case 0x2B:
			m_CKSEL.SetCurSel(35);
			break;
		case 0x3B:
			m_CKSEL.SetCurSel(36);
			break;
		case 0x0C:
			m_CKSEL.SetCurSel(37);
			break;
		case 0x1C:
			m_CKSEL.SetCurSel(38);
			break;
		case 0x2C:
			m_CKSEL.SetCurSel(39);
			break;
		case 0x3C:
			m_CKSEL.SetCurSel(40);
			break;
		case 0x0D:
			m_CKSEL.SetCurSel(41);
			break;
		case 0x1D:
			m_CKSEL.SetCurSel(42);
			break;
		case 0x2D:
			m_CKSEL.SetCurSel(43);
			break;
		case 0x3D:
			m_CKSEL.SetCurSel(44);
			break;
		case 0x0E:
			m_CKSEL.SetCurSel(45);
			break;
		case 0x1E:
			m_CKSEL.SetCurSel(46);
			break;
		case 0x2E:
			m_CKSEL.SetCurSel(47);
			break;
		case 0x3E:
			m_CKSEL.SetCurSel(48);
			break;
		case 0x0F:
			m_CKSEL.SetCurSel(49);
			break;
		case 0x1F:
			m_CKSEL.SetCurSel(50);
			break;
		case 0x2F:
			m_CKSEL.SetCurSel(51);
			break;
		case 0x3F:
			m_CKSEL.SetCurSel(52);
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
		m_PSCRB.EnableWindow(FALSE);
		m_PSCRVA.EnableWindow(FALSE);
		m_PSCRVB.EnableWindow(FALSE);
		m_BODLevel.EnableWindow(FALSE);
		m_RSTDISBL.EnableWindow(FALSE);
		m_DWEN.EnableWindow(FALSE);
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
void CDlg_ATmegaM1::OnCbnSelchangeCombo()
{
	CString ExFuse_str, HFuse_str, LFuse_str, Lockbits_str;
	ExtendedFuse = 0xFF;
	HighFuse = 0xD9;
	LowFuse = 0x62;
	Lockbits = 0xFF;
	//PSCRB
	ExtendedFuse &= 0xDF;
	if (m_PSCRB.GetCurSel() == 1)
		ExtendedFuse |= 0x20;
	else if (m_PSCRB.GetCurSel() == 0)
		ExtendedFuse |= 0x00;
	ExFuse_str.Format(_T("%02X"), ExtendedFuse);
	m_ExFuse.SetWindowTextW(ExFuse_str);
	//PSCRVA
	ExtendedFuse &= 0xEF;
	if (m_PSCRVA.GetCurSel() == 1)
		ExtendedFuse |= 0x10;
	else if (m_PSCRVA.GetCurSel() == 0)
		ExtendedFuse |= 0x00;
	ExFuse_str.Format(_T("%02X"), ExtendedFuse);
	m_ExFuse.SetWindowTextW(ExFuse_str);
	//PSCRVB
	ExtendedFuse &= 0xF7;
	if (m_PSCRVB.GetCurSel() == 1)
		ExtendedFuse |= 0x08;
	else if (m_PSCRVB.GetCurSel() == 0)
		ExtendedFuse |= 0x00;
	ExFuse_str.Format(_T("%02X"), ExtendedFuse);
	m_ExFuse.SetWindowTextW(ExFuse_str);
	//BODLevel
	ExtendedFuse &= 0xF8;
	if (m_BODLevel.GetCurSel() == 7)
		ExtendedFuse |= 0x07;
	else if (m_BODLevel.GetCurSel() == 6)
		ExtendedFuse |= 0x06;
	else if (m_BODLevel.GetCurSel() == 5)
		ExtendedFuse |= 0x05;
	else if (m_BODLevel.GetCurSel() == 4)
		ExtendedFuse |= 0x04;
	else if (m_BODLevel.GetCurSel() == 3)
		ExtendedFuse |= 0x03;
	else if (m_BODLevel.GetCurSel() == 2)
		ExtendedFuse |= 0x02;
	else if (m_BODLevel.GetCurSel() == 1)
		ExtendedFuse |= 0x01;
	else if (m_BODLevel.GetCurSel() == 0)
		ExtendedFuse |= 0x00;
	ExFuse_str.Format(_T("%02X"), ExtendedFuse);
	m_ExFuse.SetWindowTextW(ExFuse_str);
	//RSTDISBL
	HighFuse &= 0x7F;
	if (m_RSTDISBL.GetCurSel() == 1)
		HighFuse |= 0x80;
	else if (m_RSTDISBL.GetCurSel() == 0)
		HighFuse |= 0x00;
	HFuse_str.Format(_T("%02X"), HighFuse);
	m_HFuse.SetWindowTextW(HFuse_str);
	//DWEN
	HighFuse &= 0xBF;
	if (m_DWEN.GetCurSel() == 1)
		HighFuse |= 0x40;
	else if (m_DWEN.GetCurSel() == 0)
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
	//CKSEL
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
		LowFuse |= 0x33;
		break;
	case 13:
		LowFuse |= 0x04;
		break;
	case 14:
		LowFuse |= 0x14;
		break;
	case 15:
		LowFuse |= 0x24;
		break;
	case 16:
		LowFuse |= 0x34;
		break;
	case 17:
		LowFuse |= 0x05;
		break;
	case 18:
		LowFuse |= 0x15;
		break;
	case 19:
		LowFuse |= 0x25;
		break;
	case 20:
		LowFuse |= 0x35;
		break;
	case 21:
		LowFuse |= 0x08;
		break;
	case 22:
		LowFuse |= 0x18;
		break;
	case 23:
		LowFuse |= 0x28;
		break;
	case 24:
		LowFuse |= 0x38;
		break;
	case 25:
		LowFuse |= 0x09;
		break;
	case 26:
		LowFuse |= 0x19;
		break;
	case 27:
		LowFuse |= 0x29;
		break;
	case 28:
		LowFuse |= 0x39;
		break;
	case 29:
		LowFuse |= 0x0A;
		break;
	case 30:
		LowFuse |= 0x1A;
		break;
	case 31:
		LowFuse |= 0x2A;
		break;
	case 32:
		LowFuse |= 0x3A;
		break;
	case 33:
		LowFuse |= 0x0B;
		break;
	case 34:
		LowFuse |= 0x1B;
		break;
	case 35:
		LowFuse |= 0x2B;
		break;
	case 36:
		LowFuse |= 0x3B;
		break;
	case 37:
		LowFuse |= 0x0C;
		break;
	case 38:
		LowFuse |= 0x1C;
		break;
	case 39:
		LowFuse |= 0x2C;
		break;
	case 40:
		LowFuse |= 0x3C;
		break;
	case 41:
		LowFuse |= 0x0D;
		break;
	case 42:
		LowFuse |= 0x1D;
		break;
	case 43:
		LowFuse |= 0x2D;
		break;
	case 44:
		LowFuse |= 0x3D;
		break;
	case 45:
		LowFuse |= 0x0E;
		break;
	case 46:
		LowFuse |= 0x1E;
		break;
	case 47:
		LowFuse |= 0x2E;
		break;
	case 48:
		LowFuse |= 0x3E;
		break;
	case 49:
		LowFuse |= 0x0F;
		break;
	case 50:
		LowFuse |= 0x1F;
		break;
	case 51:
		LowFuse |= 0x2F;
		break;
	case 52:
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
void CDlg_ATmegaM1::OnOK()
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

LRESULT CDlg_ATmegaM1::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();

}
LRESULT CDlg_ATmegaM1::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}

void CDlg_ATmegaM1::OnBnClickedEnable()
{
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	int state = Encheck->GetCheck();
	if (state == 1)
	{
		m_PSCRB.EnableWindow(TRUE);
		m_PSCRVA.EnableWindow(TRUE);
		m_PSCRVB.EnableWindow(TRUE);
		m_BODLevel.EnableWindow(TRUE);
		m_RSTDISBL.EnableWindow(TRUE);
		m_DWEN.EnableWindow(TRUE);
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
		m_PSCRB.EnableWindow(FALSE);
		m_PSCRVA.EnableWindow(FALSE);
		m_PSCRVB.EnableWindow(FALSE);
		m_BODLevel.EnableWindow(FALSE);
		m_RSTDISBL.EnableWindow(FALSE);
		m_DWEN.EnableWindow(FALSE);
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