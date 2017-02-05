// Dlg_ATtinyISP_2.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_ATtinyISP_2.h"
#include "afxdialogex.h"
#include "numeric_conversion.h"
#include "../common/prog_info.h"

// Dlg_ATtinyISP_2 dialog

IMPLEMENT_DYNAMIC(CDlg_ATtinyISP_2, CMFCPropertyPage)

CDlg_ATtinyISP_2::CDlg_ATtinyISP_2(struct chip_info_c *ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_ATtinyISP_2::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}
CDlg_ATtinyISP_2::~CDlg_ATtinyISP_2()
{
}

void CDlg_ATtinyISP_2::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_SELFPRGEN);
	DDX_Control(pDX, IDC_COMBO2, m_RSTDISBL);
	DDX_Control(pDX, IDC_COMBO3, m_DWEN);
	DDX_Control(pDX, IDC_COMBO4, m_SPIEN);
	DDX_Control(pDX, IDC_COMBO5, m_WDTON);
	DDX_Control(pDX, IDC_COMBO6, m_EESAVE);
	DDX_Control(pDX, IDC_COMBO7, m_BODLevel);
	DDX_Control(pDX, IDC_COMBO8, m_CKDIV8);
	DDX_Control(pDX, IDC_COMBO9, m_CKOUT);
	DDX_Control(pDX, IDC_COMBO10, m_CKSEL);
	DDX_Control(pDX, IDC_COMBO11, m_Lock);
	DDX_Control(pDX, IDC_EDIT1, m_ExFuse);
	DDX_Control(pDX, IDC_EDIT2, m_HFuse);
	DDX_Control(pDX, IDC_EDIT3, m_LFuse);
	DDX_Control(pDX, IDC_EDIT4, m_Lockbits);
	DDX_Control(pDX, IDC_CHECK1, Option_enable);
}


BEGIN_MESSAGE_MAP(CDlg_ATtinyISP_2, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_ATtinyISP_2::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_ATtinyISP_2::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_ATtinyISP_2::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_ATtinyISP_2::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_ATtinyISP_2::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDlg_ATtinyISP_2::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDlg_ATtinyISP_2::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CDlg_ATtinyISP_2::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO9, &CDlg_ATtinyISP_2::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO10, &CDlg_ATtinyISP_2::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO11, &CDlg_ATtinyISP_2::OnCbnSelchangeCombo)
	ON_BN_CLICKED(IDC_CHECK1, &CDlg_ATtinyISP_2::OnBnClickedEnable)
END_MESSAGE_MAP()


// Dlg_ATtinyISP_2 message handlers
BOOL CDlg_ATtinyISP_2::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	if (wcscmp(_T("ATtinyISP2_1"), m_ChipInfo->description) == 0			//ATtiny24/44/84
		|| wcscmp(_T("ATtinyISP2_2"), m_ChipInfo->description) == 0      //ATtiny25/45/85
		|| wcscmp(_T("ATtinyISP2_3"), m_ChipInfo->description) == 0      //ATtiny43
		|| wcscmp(_T("ATtinyISP2_4"), m_ChipInfo->description) == 0      //ATtiny261/461/861
		|| wcscmp(_T("ATtinyISP2_5"), m_ChipInfo->description) == 0      //ATtiny87/167
		){
		ExtendedFuse = 0xFF;
		HighFuse = 0xDF;
		LowFuse = 0x62;
		Lockbits = 0xFF;
		m_ExFuse.SetWindowTextW(L"FF");
		m_HFuse.SetWindowTextW(L"DF");
		m_LFuse.SetWindowTextW(L"62");
		m_Lockbits.SetWindowTextW(L"FF");
	}
	if (wcscmp(_T("ATtinyISP2_6"), m_ChipInfo->description) == 0)    //ATtiny48/88
	{
		ExtendedFuse = 0xFF;
		HighFuse = 0xDF;
		LowFuse = 0x6E;
		Lockbits = 0xFF;
		m_ExFuse.SetWindowTextW(L"FF");
		m_HFuse.SetWindowTextW(L"DF");
		m_LFuse.SetWindowTextW(L"6E");
		m_Lockbits.SetWindowTextW(L"FF");
	}
	//ComboBox Setting
	m_SELFPRGEN.InsertString(m_SELFPRGEN.GetCount(), L"0 = Enable SPM Instruction");
	m_SELFPRGEN.InsertString(m_SELFPRGEN.GetCount(), L"1 = Disable SPM Instruction");
	m_RSTDISBL.InsertString(m_RSTDISBL.GetCount(), L"0 = Enabled external reset");
	m_RSTDISBL.InsertString(m_RSTDISBL.GetCount(), L"1 = Disabled external reset");
	m_DWEN.InsertString(m_DWEN.GetCount(), L"0 = Enable debugWIRE");
	m_DWEN.InsertString(m_DWEN.GetCount(), L"1 = Disable debugWIRE");
	m_SPIEN.InsertString(m_SPIEN.GetCount(), L"0 = Enabled ISP Programming");
	m_SPIEN.InsertString(m_SPIEN.GetCount(), L"1 = Disabled ISP Programming");
	m_WDTON.InsertString(m_WDTON.GetCount(), L"0 =Set Watchdog Timer permanently on");
	m_WDTON.InsertString(m_WDTON.GetCount(), L"1 =UnSet Watchdog Timer permanently on");
	m_EESAVE.InsertString(m_EESAVE.GetCount(), L"0 = Preserve EEPROM during Chip Erase");
	m_EESAVE.InsertString(m_EESAVE.GetCount(), L"1 = Unreserves EEPROM during Chip Erase");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"000 = Trigger Level at 2.0V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"001 = Trigger Level at 1.9V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"010 = Trigger Level at 2.2V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"011 = Trigger Level at 2.3V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"100 = Trigger Level at 4.3V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"101 = Trigger Level at 2.7V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"110 = Trigger Level at 1.8V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"111 = BODLevel Disabled");
	m_CKDIV8.InsertString(m_CKDIV8.GetCount(), L"0 = Divided CLK by 8");
	m_CKDIV8.InsertString(m_CKDIV8.GetCount(), L"1 = Disabled Divided");
	m_CKOUT.InsertString(m_CKOUT.GetCount(), L"0 = Output system CLK on Port pin");
	m_CKOUT.InsertString(m_CKOUT.GetCount(), L"1 = Disabled Output system CLK");
	m_Lock.InsertString(m_Lock.GetCount(), L"00 = Further Programming and verification Disabled");
	m_Lock.InsertString(m_Lock.GetCount(), L"10 = Further Programming Disabled");
	m_Lock.InsertString(m_Lock.GetCount(), L"11 = No memory Lock features enabled");
	//CLKSetting
	if (wcscmp(_T("ATtinyISP2_1"), m_ChipInfo->description) == 0) //ATtiny24/44/84
	{
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0000 = Ext.CLK, StartupTime : 6CK");				// 0 = 0x00
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0000 = Ext.CLK, StartupTime : 6CK + 4ms"); // 1 = 0x10
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0000 = Ext.CLK, StartupTime : 6CK + 64ms"); //2 = 0x20
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0010 = Int.RC OSC : 8MHz, StartupTime : 6CK"); // 3 = 0x02
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0010 = Int.RC OSC : 8MHz, StartupTime : 6CK + 4ms"); // 4 = 0x12
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0010 = Int.RC OSC : 8MHz, StartupTime : 6CK + 64ms"); // 5 = 0x22 ; Initial Value
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0100 = WDT OSC : 128KHz, StartupTime : 6CK");  // 6 = 0x04
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0100 = WDT OSC : 128KHz, StartupTime : 6CK + 4ms"); // 7 = 0x14
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0100 = WDT OSC : 128KHz, StartupTime : 6CK + 64ms"); // 8 = 0x24
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0110 = Ext.Low.freq OSC : 128KHz, StartupTime : 1KCK + 4ms"); // 9 = 0x06
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0110 = Ext.Low.freq OSC : 128KHz, StartupTime : 1KCK + 64ms"); // 10 =0x16 
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0110 = Ext.Low.freq OSC : 128KHz, StartupTime : 32KCK + 64ms"); // 11 = 0x26
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1000 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 258CK + 4.1ms"); // 12 = 0x08
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1000 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 258CK + 65ms"); // 13 = 0x18
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1000 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 1KCK"); // 14 = 0x28
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1000 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 1KCK + 4.1ms"); //15 = 0x38
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1001 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 1KCK + 65ms"); // 16 = 0x09
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1001 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 16KCK"); // 17 = 0x19
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1001 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 16KCK + 4.1ms"); // 18 = 0x29
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1001 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 16KCK + 65ms"); // 19 = 0x39
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1010 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 258CK + 4.1ms"); // 20 = 0x0A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1010 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 258CK + 65ms"); // 21 = 0x1A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1010 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 1KCK"); // 22 = 0x2A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1010 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 1KCK + 4.1ms"); // 23 = 0x3A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1011 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 1KCK + 65ms"); // 24 = 0x0B
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1011 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 16KCK"); // 25 = 0x1B
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1011 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 16KCK + 4.1ms"); // 26 = 0x2B
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1011 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 16KCK + 65ms"); // 27 = 0x3B
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1100 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 258CK + 4.1ms"); // 28 = 0x0C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1100 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 258CK + 65ms"); // 29 = 0x1C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1100 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 1KCK");  //30 = 0x2C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1100 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 1KCK + 4.1ms"); //31 = 0x3C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1101 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 1KCK + 65ms"); //32 = 0x0D
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1101 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 16KCK"); //33 = 0x1D
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1101 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 16KCK + 4.1ms"); //34 = 0x2D
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1101 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 16KCK + 65ms"); //35 = 0x3D
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1110 = Ext.Crystal OSC : >8MHz, StartupTime : 258CK + 4.1ms"); //36 = 0x0E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1110 = Ext.Crystal OSC : >8MHz, StartupTime : 258CK + 65ms"); // 37 = 0x1E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1110 = Ext.Crystal OSC : >8MHz, StartupTime : 1KCK"); // 38 = 0x2E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1110 = Ext.Crystal OSC : >8MHz, StartupTime : 1KCK + 4.1ms"); // 39 =0x3E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1111 = Ext.Crystal OSC : >8MHz, StartupTime : 1KCK + 65ms"); // 40 = 0x0F
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1111 = Ext.Crystal OSC : >8MHz, StartupTime : 16KCK"); // 41 = 0x1F 
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1111 = Ext.Crystal OSC : >8MHz, StartupTime : 16KCK + 4.1ms"); //42 = 0x2F
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1111 = Ext.Crystal OSC : >8MHz, StartupTime : 16KCK + 65ms"); // 43 = 0x3F
	}
	if (wcscmp(_T("ATtinyISP2_2"), m_ChipInfo->description) == 0) //ATtiny25/45/85
	{
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0000 = Ext.CLK, StartupTime : 6CK/14CK(PWRDWN/RST)");				// 0 = 0x00
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0000 = Ext.CLK, StartupTime : 6CK/14CK+4.1ms(PWRDWN/RST)"); // 1 = 0x10
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0000 = Ext.CLK, StartupTime : 6CK/14CK+65ms(PWRDWN/RST)"); // 2 = 0x20
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0001 = PLLclk, StartupTime : 1KCK+4ms/14CK+4ms(PWRDWN/RST)"); // 3 = 0x01
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0001 = PLLclk, StartupTime : 16KCK+4ms/14CK+4ms(PWRDWN/RST)"); // 4 = 0x11
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0001 = PLLclk, StartupTime : 1KCK+64ms/14CK+64ms(PWRDWN/RST)"); // 5 = 0x21
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,0001 = PLLclk, StartupTime : 16KCK+64ms/14CK+64ms(PWRDWN/RST)"); // 6 = 0x31
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0010 = Int.RC OSC : 8MHz, StartupTime : 6CK/14CK(PWRDWN/RST)");  // 7 = 0x02
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0010 = Int.RC OSC : 8MHz, StartupTime : 6CK/14CK+4ms(PWRDWN/RST)"); // 8 = 0x12
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0010 = Int.RC OSC : 8MHz, StartupTime : 6CK/14CK+64ms(PWRDWN/RST)"); //9 = 0x22   // initial value
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0011 = Int.RC OSC : 6.4MHz, StartupTime : 6CK/14CK+64ms(PWRDWN/RST)"); // 10 = 0x03
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0011 = Int.RC OSC : 6.4MHz, StartupTime : 6CK/14CK+64ms(PWRDWN/RST)");// 11 = 0x13
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0011 = Int.RC OSC : 6.4MHz, StartupTime : 6CK/14CK+4ms(PWRDWN/RST)"); // 12 = 0x23
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,0011 = Int.RC OSC : 6.4MHz, StartupTime : 1CK/14CK(PWRDWN/RST)"); // 13 = 0x33
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0100 = WDT OSC : 128KHz, StartupTime : 6CK/14CK(PWRDWN/RST)"); // 14 = 0x04
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0100 = WDT OSC : 128KHz, StartupTime : 6CK/14CK+4ms(PWRDWN/RST)"); // 15 = 0x14
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0100 = WDT OSC : 128KHz, StartupTime : 6CK/14CK+64ms(PWRDWN/RST)"); // 16 = 24
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0110 = Ext.Low.freq OSC, StartupTime : 1KCK/14CK(PWRDWN/RST)"); // 17 = 0x06
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0110 = Ext.Low.freq OSC, StartupTime : 1KCK/14CK+4ms(PWRDWN/RST)"); // 18 = 0x16
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0110 = Ext.Low.freq OSC, StartupTime : 32KCK/14CK+64ms(PWRDWN/RST)"); // 19 = 0x26
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1000 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 258CK/14CK+4.1ms"); // 20 = 0x08
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1000 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 258CK/14CK+65ms"); // 21 = 0x18
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1000 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 1KCK/14CK"); // 22 = 0x28
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1000 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 1KCK/14CK+4.1ms"); // 23 = 0x38
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1001 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 1KCK/14CK+65ms"); // 24 = 0x09
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1001 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 16KCK/14CK"); // 25 = 0x19
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1001 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 16KCK/14CK+4.1ms"); // 26 = 0x29
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1001 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 16KCK/14CK+65ms"); // 27 = 0x39
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1010 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 258CK/14CK+4.1ms"); // 28 = 0x0A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1010 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 258CK/14CK+65ms"); // 29 = 0x1A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1010 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 1KCK/14CK");// 30 = 0x2A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1010 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 1KCK/14CK+4.1ms"); //31 = 0x3A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1011 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 1KCK/14CK+65ms"); // 32 = 0x0B
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1011 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 16KCK/14CK"); // 33 = 0x1B
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1011 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 16KCK/14CK+4.1ms"); // 34 = 0x2B
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1011 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 16KCK/14CK+65ms"); // 35 = 0x3B
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1100 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 258CK/14CK+4.1ms"); //36 = 0x0C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1100 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 258CK/14CK+65ms");//37 = 0x1C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1100 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 1KCK/14CK");// 38 = 0x2C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1100 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 1KCK/14CK+4.1ms"); //39 = 0x3C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1101 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 1KCK/14CK+65ms"); // 40 = 0x0D
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1101 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 16KCK/14CK"); // 41 = 0x1D
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1101 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 16KCK/14CK+4.1ms"); //42 = 0x2D
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1101 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 16KCK/14CK+65ms"); // 43 = 0x3D
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1110 = Ext.Crystal OSC : >8MHz, StartupTime : 258CK/14CK+4.1ms"); // 44 = 0x0E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1110 = Ext.Crystal OSC : >8MHz, StartupTime : 258CK/14CK+65ms"); // 45 = 0x1E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1110 = Ext.Crystal OSC : >8MHz, StartupTime : 1KCK/14CK"); // 46 = 0x2E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1110 = Ext.Crystal OSC : >8MHz, StartupTime : 1KCK/14CK+4.1ms"); //47 = 0x3E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1111 = Ext.Crystal OSC : >8MHz, StartupTime : 1KCK/14CK+65ms"); // 48 = 0x0F
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1111 = Ext.Crystal OSC : >8MHz, StartupTime : 16KCK/14CK");//49 = 0x1F
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1111 = Ext.Crystal OSC : >8MHz, StartupTime : 16KCK/14CK+4.1ms");//50 = 0x2F
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1111 = Ext.Crystal OSC : >8MHz, StartupTime : 16KCK/14CK+65ms");//51 = 0x3F
	}
	if (wcscmp(_T("ATtinyISP2_3"), m_ChipInfo->description) == 0) //ATtiny43
	{
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0000 = Ext.CLK, StartupTime : 6CK/14CK(PWRDWN/RST)");				// 0 = 0x00
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0000 = Ext.CLK, StartupTime : 6CK/14CK+4ms(PWRDWN/RST)"); // 1 = 0x10
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0000 = Ext.CLK, StartupTime : 6CK/14CK+64ms(PWRDWN/RST)"); // 2 = 0x20
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0001 = PLLclk, StartupTime : 1KCK/14CK+4.1ms(PWRDWN/RST)"); // 3 = 0x01
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0001 = PLLclk, StartupTime : 16KCK/14CK+64ms(PWRDWN/RST)"); // 4 = 0x11
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0001 = PLLclk, StartupTime : 1KCK/14CK+4.1ms(PWRDWN/RST)"); // 5 = 0x21
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,0001 = PLLclk, StartupTime : 16KCK/14CK+64ms(PWRDWN/RST)"); // 6 = 0x31
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0010 = Int.RC OSC : 8MHz, StartupTime : 6CK/14CK(PWRDWN/RST)"); // 7= 0x02
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0010 = Int.RC OSC : 8MHz, StartupTime : 6CK/14CK+4.1ms(PWRDWN/RST)"); // 8 = 0x12
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0010 = Int.RC OSC : 8MHz, StartupTime : 6CK/14CK+64ms(PWRDWN/RST)");  // 9 = 0xx22 initial value
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0011 = WDT OSC : 128KHz, StartupTime : 6CK/14CK(PWRDWN/RST)"); // 10 = 0x03
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0011 = WDT OSC : 128KHz, StartupTime : 6CK/14CK+4.1ms(PWRDWN/RST)"); // 11 = 0x13
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0011 = WDT OSC : 128KHz, StartupTime : 6CK/14CK+64ms(PWRDWN/RST)"); // 12 = 0x23
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0100 = Ext.Low.freq OSC, StartupTime : 6CK/14CK(PWRDWN/RST)"); // 13 = 0x04
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0100 = Ext.Low.freq OSC, StartupTime : 6CK/14CK+4.1ms(PWRDWN/RST)");  // 14 = 0x 14
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0100 = Ext.Low.freq OSC, StartupTime : 6CK/14CK+64ms(PWRDWN/RST)"); // 15 = 0x24
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1000 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 258CK/14CK+4.1ms(PWRDWN/RST)"); // 16 = 0x08
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1000 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 258CK/14CK+65ms(PWRDWN/RST)"); // 17 = 0x18
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1000 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 1KCK/14CK(PWRDWN/RST)"); //18 = 0x28
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1000 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 1KCK/14CK+4.1ms(PWRDWN/RST)"); // 19 = 0x38
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1001 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 1KCK/14CK+65ms(PWRDWN/RST)"); // 20 =0x09
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1001 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 16KCK/14CK(PWRDWN/RST)"); // 21 = 0x19
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1001 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 16KCK/14CK+4.1ms(PWRDWN/RST)"); // 22 = 0x29
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1001 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 16KCK/14CK+65ms(PWRDWN/RST)"); // 23 = 0x39
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1010 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 258CK/14CK+4.1ms(PWRDWN/RST)");// 24 = 0x0A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1010 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 258CK/14CK+65ms(PWRDWN/RST)"); // 25 = 0x1A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1010 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 1KCK/14CK(PWRDWN/RST)"); // 26 = 0x2A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1010 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 1KCK/14CK+4.1ms(PWRDWN/RST)"); // 27 = 0x3A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1011 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 1KCK/14CK+65ms(PWRDWN/RST)"); // 28 = 0x0B
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1011 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 16KCK/14CK(PWRDWN/RST)"); //29 = 0x1B
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1011 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 16KCK/14CK+4.1ms(PWRDWN/RST)"); // 30 = 0x2B
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1011 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 16KCK/14CK+65ms(PWRDWN/RST)"); //31 = 0x3B
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1100 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 258CK/14CK+4.1ms(PWRDWN/RST)"); //32 = 0x0C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1100 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 258CK/14CK+65ms(PWRDWN/RST)"); //33 = 0x1C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1100 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 1KCK/14CK(PWRDWN/RST)"); // 34 = 0x2C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1100 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 1KCK/14CK+4.1ms(PWRDWN/RST)"); //35 = 0x3C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1101 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 1KCK/14CK+65ms(PWRDWN/RST)"); //36 = 0x0D
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1101 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 16KCK/14CK(PWRDWN/RST)"); // 37 = 0x1D
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1101 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 16KCK/14CK+4.1ms(PWRDWN/RST)"); //38 = 0x2D
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1101 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 16KCK/14CK+65ms(PWRDWN/RST)"); // 39 = 0x3D
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1110 = Ext.Crystal OSC : >8MHz, StartupTime : 258CK/14CK+4.1ms(PWRDWN/RST)"); // 40 = 0x0E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1110 = Ext.Crystal OSC : >8MHz, StartupTime : 258CK/14CK+65ms(PWRDWN/RST)"); //41 = 0x1E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1110 = Ext.Crystal OSC : >8MHz, StartupTime : 1KCK/14CK(PWRDWN/RST)"); // 42 = 0x2E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1110 = Ext.Crystal OSC : >8MHz, StartupTime : 1KCK/14CK+4.1ms(PWRDWN/RST)"); // 43 = 0x 3E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1111 = Ext.Crystal OSC : >8MHz, StartupTime : 1KCK/14CK+65ms(PWRDWN/RST)"); // 44 = 0x0F
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1111 = Ext.Crystal OSC : >8MHz, StartupTime : 16KCK/14CK(PWRDWN/RST)"); // 45 = 0x1F
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1111 = Ext.Crystal OSC : >8MHz, StartupTime : 16KCK/14CK+4.1ms(PWRDWN/RST)"); // 46 = 0x2F
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1111 = Ext.Crystal OSC : >8MHz, StartupTime : 16KCK/14CK+65ms(PWRDWN/RST)"); // 47 = 0x3F
	}
	if (wcscmp(_T("ATtinyISP2_4"), m_ChipInfo->description) == 0) //ATtiny261/461/861
	{
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0000 = Ext.CLK, StartupTime : 6CK");				// 0 = 0x00
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0000 = Ext.CLK, StartupTime : 6CK+4ms"); // 1 = 0x10
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0000 = Ext.CLK, StartupTime : 6CK+64ms"); //2 = 0x20
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0001 = PLLclk, StartupTime : 1KCK+4ms");  // 3 = 0x01
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0001 = PLLclk, StartupTime : 16KCK+4ms"); // 4 = 0x11
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0001 = PLLclk, StartupTime : 1KCK+64ms"); // 5 = 0x21
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,0001 = PLLclk, StartupTime : 16KCK+64ms"); // 6 = 0x31
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0010 = Int.RC OSC : 8MHz, StartupTime : 6CK"); // 7 = 0x02
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0010 = Int.RC OSC : 8MHz, StartupTime : 6CK+4ms"); // 8 = 0x12
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0010 = Int.RC OSC : 8MHz, StartupTime : 6CK+64ms"); // 9 = 0x22   initial value
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0011 = WDT OSC : 128KHz, StartupTime : 6CK");  // 10 = 0x03
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0011 = WDT OSC : 128KHz, StartupTime : 6CK+4ms"); //11 = 0x13
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0011 = WDT OSC : 128KHz, StartupTime : 6CK+64ms"); // 12 = 0x23
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0100 = Ext.Low.freq OSC, StartupTime : 1KCK+4ms"); // 13 = 0x04
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0100 = Ext.Low.freq OSC, StartupTime : 1KCK+64ms"); // 14 = 0x14
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0100 = Ext.Low.freq OSC, StartupTime : 32KCK+64ms"); // 15 = 0x24
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1000 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 258CK+4.1ms"); // 16 = 0x08
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1000 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 258CK+65ms"); // 17 = 0x18
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1000 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 1KCK"); // 18 = 0x28
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1000 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 1KCK+4.1ms"); // 19 = 0x38
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1001 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 1KCK+65ms"); //20 = 0x09
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1001 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 16KCK"); //21 = 0x19
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1001 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 16KCK+4.1ms");  //22 = 0x29
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1001 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 16KCK+65ms"); // 23 = 0x39
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1010 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 258CK+4.1ms"); // 24 = 0x0A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1010 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 258CK+65ms"); //25 = 0x1A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1010 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 1KCK"); // 26 = 0x2A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1010 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 1KCK+4.1ms"); //27 = 0x3A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1011 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 1KCK+65ms"); //28 = 0x0B
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1011 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 16KCK"); // 29 = 0x1B
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1011 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 16KCK+4.1ms"); //30 = 0x2B
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1011 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 16KCK+65ms"); // 31 = 0x3B
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1100 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 258CK+4.1ms"); // 32 = 0x0C
 		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1100 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 258CK+65ms"); // 33 = 0x1C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1100 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 1KCK"); //34 = 0x2C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1100 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 1KCK+4.1ms"); //35 = 0x3C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1101 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 1KCK+65ms"); // 36 = 0x0D
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1101 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 16KCK"); //37 = 0x1D
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1101 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 16KCK+4.1ms"); // 38 = 0x2D
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1101 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 16KCK+65ms"); // 39 = 0x3D
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1110 = Ext.Crystal OSC : >8MHz, StartupTime : 258CK+4.1ms"); // 40 = 0x0E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1110 = Ext.Crystal OSC : >8MHz, StartupTime : 258CK+65ms"); // 41 = 0x1E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1110 = Ext.Crystal OSC : >8MHz, StartupTime : 1KCK"); //42 = 0x2E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1110 = Ext.Crystal OSC : >8MHz, StartupTime : 1KCK+4.1ms"); //43 = 0x3E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1111 = Ext.Crystal OSC : >8MHz, StartupTime : 1KCK+65ms"); //44 = 0x0F
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1111 = Ext.Crystal OSC : >8MHz, StartupTime : 16KCK"); //45 = 0x1F
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1111 = Ext.Crystal OSC : >8MHz, StartupTime : 16KCK+4.1ms"); //46 = 0x2F
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1111 = Ext.Crystal OSC : >8MHz, StartupTime : 16KCK+65ms"); //47 = 0x3F
	}
	if (wcscmp(_T("ATtinyISP2_5"), m_ChipInfo->description) == 0) //ATtiny87/167
	{
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0000 = Ext.CLK, StartupTime : 6CK/14CK(PWRDWN/RST)");				// 0 = 0x00
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0000 = Ext.CLK, StartupTime : 6CK/14CK+4.1ms(PWRDWN/RST)"); // 1 = 0x10
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0000 = Ext.CLK, StartupTime : 6CK/14CK+65ms(PWRDWN/RST)"); // 2 = 0x20
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0010 = Int.RC OSC : 8MHz, StartupTime : 6CK/14CK(PWRDWN/RST)"); // 3= 0x02
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0010 = Int.RC OSC : 8MHz, StartupTime : 6CK/14CK+4.1ms(PWRDWN/RST)"); // 4 = 0x12
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0010 = Int.RC OSC : 8MHz, StartupTime : 6CK/14CK+65ms(PWRDWN/RST)");  // 5 = 0xx22 initial value
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0011 = WDT OSC : 128KHz, StartupTime : 6CK/14CK(PWRDWN/RST)");			//6 = 0x03
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0011 = WDT OSC : 128KHz, StartupTime : 6CK/14CK+4.1ms(PWRDWN/RST)"); // 7 = 0x13
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0011 = WDT OSC : 128KHz, StartupTime : 6CK/14CK+65ms(PWRDWN/RST)"); // 8 = 0x23
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0100 = Ext.Low.freq OSC, StartupTime : 1KCK/1KCK+4.1ms(PWRDWN/RST)"); //9 = 0x04
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0100 = Ext.Low.freq OSC, StartupTime : 1KCK/1KCK+65ms(PWRDWN/RST)"); //10 = 0x14
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0100 = Ext.Low.freq OSC, StartupTime : 32KCK/32KCK+65ms(PWRDWN/RST)"); //11 = 0x24
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0101 = Ext.Low.freq OSC, StartupTime : 1KCK/1KCK+4.1ms(PWRDWN/RST)"); //12 = 0x05
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0101 = Ext.Low.freq OSC, StartupTime : 1KCK/1KCK+65ms(PWRDWN/RST)"); // 13 = 0x15
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0101 = Ext.Low.freq OSC, StartupTime : 32KCK/32KCK+65ms(PWRDWN/RST)"); //14 = 0x25
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0110 = Ext.Low.freq OSC, StartupTime : 1KCK/1KCK+4.1ms(PWRDWN/RST)"); //15 = 0x06
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0110 = Ext.Low.freq OSC, StartupTime : 1KCK/1KCK+65ms(PWRDWN/RST)"); // 16 = 0x16
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0110 = Ext.Low.freq OSC, StartupTime : 32KCK/32KCK+65ms(PWRDWN/RST)"); //17 = 0x26
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,0111 = Ext.Low.freq OSC, StartupTime : 1KCK/1KCK+4.1ms(PWRDWN/RST)"); //18 = 0x07
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,0111 = Ext.Low.freq OSC, StartupTime : 1KCK/1KCK+65ms(PWRDWN/RST)"); //19 = 0x17
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,0111 = Ext.Low.freq OSC, StartupTime : 32KCK/32KCK+65ms(PWRDWN/RST)"); //20 = 0x27
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1000 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 258CK/14CK+4.1ms(PWRDWN/RST)"); // 21 = 0x08
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1000 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 258CK/14CK+65ms(PWRDWN/RST)"); // 22 = 0x18
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1000 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 1KCK/14CK(PWRDWN/RST)"); //23 = 0x28
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1000 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 1KCK/14CK+4.1ms(PWRDWN/RST)"); // 24 = 0x38
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1001 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 1KCK/14CK+65ms(PWRDWN/RST)"); // 25 =0x09
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1001 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 16KCK/14CK(PWRDWN/RST)"); // 26 = 0x19
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1001 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 16KCK/14CK+4.1ms(PWRDWN/RST)"); // 27 = 0x29
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1001 = Ext.Crystal OSC : 0.4-0.9MHz, StartupTime : 16KCK/14CK+65ms(PWRDWN/RST)"); // 28 = 0x39
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1010 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 258CK/14CK+4.1ms(PWRDWN/RST)");// 29 = 0x0A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1010 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 258CK/14CK+65ms(PWRDWN/RST)"); // 30 = 0x1A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1010 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 1KCK/14CK(PWRDWN/RST)"); // 31 = 0x2A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1010 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 1KCK/14CK+4.1ms(PWRDWN/RST)"); // 32 = 0x3A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1011 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 1KCK/14CK+65ms(PWRDWN/RST)"); // 33 = 0x0B
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1011 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 16KCK/14CK(PWRDWN/RST)"); //34 = 0x1B
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1011 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 16KCK/14CK+4.1ms(PWRDWN/RST)"); // 35 = 0x2B
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1011 = Ext.Crystal OSC : 0.9-3.0MHz, StartupTime : 16KCK/14CK+65ms(PWRDWN/RST)"); //36 = 0x3B
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1100 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 258CK/14CK+4.1ms(PWRDWN/RST)"); //37 = 0x0C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1100 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 258CK/14CK+65ms(PWRDWN/RST)"); //38 = 0x1C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1100 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 1KCK/14CK(PWRDWN/RST)"); // 39 = 0x2C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1100 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 1KCK/14CK+4.1ms(PWRDWN/RST)"); //40 = 0x3C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1101 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 1KCK/14CK+65ms(PWRDWN/RST)"); //41 = 0x0D
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1101 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 16KCK/14CK(PWRDWN/RST)"); // 42 = 0x1D
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1101 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 16KCK/14CK+4.1ms(PWRDWN/RST)"); //43 = 0x2D
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1101 = Ext.Crystal OSC : 3.0-8MHz, StartupTime : 16KCK/14CK+65ms(PWRDWN/RST)"); // 44 = 0x3D
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1110 = Ext.Crystal OSC : >8MHz, StartupTime : 258CK/14CK+4.1ms(PWRDWN/RST)"); // 45 = 0x0E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1110 = Ext.Crystal OSC : >8MHz, StartupTime : 258CK/14CK+65ms(PWRDWN/RST)"); //46 = 0x1E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1110 = Ext.Crystal OSC : >8MHz, StartupTime : 1KCK/14CK(PWRDWN/RST)"); // 47 = 0x2E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1110 = Ext.Crystal OSC : >8MHz, StartupTime : 1KCK/14CK+4.1ms(PWRDWN/RST)"); // 48 = 0x 3E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1111 = Ext.Crystal OSC : >8MHz, StartupTime : 1KCK/14CK+65ms(PWRDWN/RST)"); // 49 = 0x0F
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1111 = Ext.Crystal OSC : >8MHz, StartupTime : 16KCK/14CK(PWRDWN/RST)"); // 50 = 0x1F
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1111 = Ext.Crystal OSC : >8MHz, StartupTime : 16KCK/14CK+4.1ms(PWRDWN/RST)"); // 51 = 0x2F
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"11,1111 = Ext.Crystal OSC : >8MHz, StartupTime : 16KCK/14CK+65ms(PWRDWN/RST)"); // 52 = 0x3F
	}
	if (wcscmp(_T("ATtinyISP2_6"), m_ChipInfo->description) == 0)    //ATtiny48/88
	{
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1100 = Ext.CLK, StartupTime : 6CK");				// 0 = 0x0C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1100 = Ext.CLK, StartupTime : 6CK+4.1ms"); // 1 = 0x1C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1100 = Ext.CLK, StartupTime : 6CK+65ms"); // 2 = 0x2C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1110 = Int.RC OSC : 8MHz, StartupTime : 6CK"); // 3 = 0x0E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1110 = Int.RC OSC : 8MHz, StartupTime : 6CK+4.1ms"); // 4 = 0x1E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1110 = Int.RC OSC : 8MHz, StartupTime : 6CK+65ms"); // 5 = 0x2E initial value
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"00,1111 = Int.RC OSC : 128KHz, StartupTime : 6CK"); // 6 = 0x0F
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"01,1111 = Int.RC OSC : 128KHz, StartupTime : 6CK+4.1ms"); // 7 = 0x1F
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"10,1111 = Int.RC OSC : 128KHz, StartupTime : 6CK+65ms"); // 8 = 0x2F
	}
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	Encheck->SetCheck(0);
	m_CKSEL.SetDroppedWidth(450);

	m_SELFPRGEN.SetCurSel(1);
	m_RSTDISBL.SetCurSel(1);
	m_DWEN.SetCurSel(1);
	m_SPIEN.SetCurSel(0);
	m_WDTON.SetCurSel(1);
	m_EESAVE.SetCurSel(1);
	m_BODLevel.SetCurSel(7);
	m_CKDIV8.SetCurSel(0);
	m_CKOUT.SetCurSel(1);
	m_Lock.SetCurSel(2);

	if (wcscmp(_T("ATtinyISP2_1"), m_ChipInfo->description) == 0
		|| wcscmp(_T("ATtinyISP2_5"), m_ChipInfo->description) == 0
		|| wcscmp(_T("ATtinyISP2_6"), m_ChipInfo->description) == 0)
	{
		m_CKSEL.SetCurSel(5);
	}
	if (wcscmp(_T("ATtinyISP2_2"), m_ChipInfo->description) == 0
		|| wcscmp(_T("ATtinyISP2_3"), m_ChipInfo->description) == 0
		|| wcscmp(_T("ATtinyISP2_4"), m_ChipInfo->description) == 0){
		m_CKSEL.SetCurSel(9);
	}
	
	m_SELFPRGEN.EnableWindow(FALSE);
	m_DWEN.EnableWindow(FALSE);
	m_EESAVE.EnableWindow(FALSE);
	m_SPIEN.EnableWindow(FALSE);
	m_WDTON.EnableWindow(FALSE);
	m_BODLevel.EnableWindow(FALSE);
	m_RSTDISBL.EnableWindow(FALSE);
	m_CKDIV8.EnableWindow(FALSE);
	m_CKOUT.EnableWindow(FALSE);
	m_CKSEL.EnableWindow(FALSE);
	m_Lock.EnableWindow(FALSE);
	return TRUE;
}

BOOL CDlg_ATtinyISP_2::OnSetActive()
{
	if (*m_pDataLen != NULL)
	{
		CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
		Encheck->SetCheck(1);
		m_SELFPRGEN.EnableWindow(TRUE);
		m_DWEN.EnableWindow(TRUE);
		m_EESAVE.EnableWindow(TRUE);
		m_SPIEN.EnableWindow(FALSE);
		m_WDTON.EnableWindow(TRUE);
		m_BODLevel.EnableWindow(TRUE);
		m_RSTDISBL.EnableWindow(TRUE);
		m_CKDIV8.EnableWindow(TRUE);
		m_CKOUT.EnableWindow(TRUE);
		m_CKSEL.EnableWindow(TRUE);
		m_Lock.EnableWindow(TRUE);

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

		//SELFPRGEN
		if ((ExtendedFuse & 0x01) == 0x01)
			m_SELFPRGEN.SetCurSel(1);
		else if ((ExtendedFuse & 0x01) == 0x00)
			m_SELFPRGEN.SetCurSel(0);
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
		//BODlevel
		if ((HighFuse & 0x07) == 0x07)
			m_BODLevel.SetCurSel(7);
		else if ((HighFuse & 0x07) == 0x06)
			m_BODLevel.SetCurSel(6);
		else if ((HighFuse & 0x07) == 0x05)
			m_BODLevel.SetCurSel(5);
		else if ((HighFuse & 0x07) == 0x04)
			m_BODLevel.SetCurSel(4);
		else if ((HighFuse & 0x07) == 0x03)
			m_BODLevel.SetCurSel(3);
		else if ((HighFuse & 0x07) == 0x02)
			m_BODLevel.SetCurSel(2);
		else if ((HighFuse & 0x07) == 0x01)
			m_BODLevel.SetCurSel(1);
		else if ((HighFuse & 0x07) == 0x00)
			m_BODLevel.SetCurSel(0);
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
		//Lock
		if ((Lockbits & 0x03) == 0x03)
			m_Lock.SetCurSel(2);
		else if ((Lockbits & 0x03) == 0x02)
			m_Lock.SetCurSel(1);
		else if ((Lockbits & 0x03) == 0x00)
			m_Lock.SetCurSel(0);
		//CLK Setting
		if (wcscmp(_T("ATtinyISP2_1"), m_ChipInfo->description) == 0) //ATtiny24/44/84
		{
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
			case 0x04:
				m_CKSEL.SetCurSel(6);
				break;
			case 0x14:
				m_CKSEL.SetCurSel(7);
				break;
			case 0x24:
				m_CKSEL.SetCurSel(8);
				break;
			case 0x06:
				m_CKSEL.SetCurSel(9);
				break;
			case 0x16:
				m_CKSEL.SetCurSel(10);
				break;
			case 0x26:
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
		}
		if (wcscmp(_T("ATtinyISP2_2"), m_ChipInfo->description) == 0) //ATtiny25/45/85
		{
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
			case 0x31:
				m_CKSEL.SetCurSel(6);
				break;
			case 0x02:
				m_CKSEL.SetCurSel(7);
				break;
			case 0x12:
				m_CKSEL.SetCurSel(8);
				break;
			case 0x22:
				m_CKSEL.SetCurSel(9);
				break;
			case 0x03:
				m_CKSEL.SetCurSel(10);
				break;
			case 0x13:
				m_CKSEL.SetCurSel(11);
				break;
			case 0x23:
				m_CKSEL.SetCurSel(12);
				break;
			case 0x33:
				m_CKSEL.SetCurSel(13);
				break;
			case 0x04:
				m_CKSEL.SetCurSel(14);
				break;
			case 0x14:
				m_CKSEL.SetCurSel(15);
				break;
			case 0x24:
				m_CKSEL.SetCurSel(16);
				break;
			case 0x06:
				m_CKSEL.SetCurSel(17);
				break;
			case 0x16:
				m_CKSEL.SetCurSel(18);
				break;
			case 0x26:
				m_CKSEL.SetCurSel(19);
				break;
			case 0x08:
				m_CKSEL.SetCurSel(20);
				break;
			case 0x18:
				m_CKSEL.SetCurSel(21);
				break;
			case 0x28:
				m_CKSEL.SetCurSel(22);
				break;
			case 0x38:
				m_CKSEL.SetCurSel(23);
				break;
			case 0x09:
				m_CKSEL.SetCurSel(24);
				break;
			case 0x19:
				m_CKSEL.SetCurSel(25);
				break;
			case 0x29:
				m_CKSEL.SetCurSel(26);
				break;
			case 0x39:
				m_CKSEL.SetCurSel(27);
				break;
			case 0x0A:
				m_CKSEL.SetCurSel(28);
				break;
			case 0x1A:
				m_CKSEL.SetCurSel(29);
				break;
			case 0x2A:
				m_CKSEL.SetCurSel(30);
				break;
			case 0x3A:
				m_CKSEL.SetCurSel(31);
				break;
			case 0x0B:
				m_CKSEL.SetCurSel(32);
				break;
			case 0x1B:
				m_CKSEL.SetCurSel(33);
				break;
			case 0x2B:
				m_CKSEL.SetCurSel(34);
				break;
			case 0x3B:
				m_CKSEL.SetCurSel(35);
				break;
			case 0x0C:
				m_CKSEL.SetCurSel(36);
				break;
			case 0x1C:
				m_CKSEL.SetCurSel(37);
				break;
			case 0x2C:
				m_CKSEL.SetCurSel(38);
				break;
			case 0x3C:
				m_CKSEL.SetCurSel(39);
				break;
			case 0x0D:
				m_CKSEL.SetCurSel(40);
				break;
			case 0x1D:
				m_CKSEL.SetCurSel(41);
				break;
			case 0x2D:
				m_CKSEL.SetCurSel(42);
				break;
			case 0x3D:
				m_CKSEL.SetCurSel(43);
				break;
			case 0x0E:
				m_CKSEL.SetCurSel(44);
				break;
			case 0x1E:
				m_CKSEL.SetCurSel(45);
				break;
			case 0x2E:
				m_CKSEL.SetCurSel(46);
				break;
			case 0x3E:
				m_CKSEL.SetCurSel(47);
				break;
			case 0x0F:
				m_CKSEL.SetCurSel(48);
				break;
			case 0x1F:
				m_CKSEL.SetCurSel(49);
				break;
			case 0x2F:
				m_CKSEL.SetCurSel(50);
				break;
			case 0x3F:
				m_CKSEL.SetCurSel(51);
				break;
			}
		}
		if (wcscmp(_T("ATtinyISP2_3"), m_ChipInfo->description) == 0 || wcscmp(_T("ATtinyISP2_4"), m_ChipInfo->description) == 0)
			//ATtiny43 ATtiny261/461/861
		{
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
			case 0x31:
				m_CKSEL.SetCurSel(6);
				break;
			case 0x02:
				m_CKSEL.SetCurSel(7);
				break;
			case 0x12:
				m_CKSEL.SetCurSel(8);
				break;
			case 0x22:
				m_CKSEL.SetCurSel(9);
				break;
			case 0x03:
				m_CKSEL.SetCurSel(10);
				break;
			case 0x13:
				m_CKSEL.SetCurSel(11);
				break;
			case 0x23:
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
			case 0x08:
				m_CKSEL.SetCurSel(16);
				break;
			case 0x18:
				m_CKSEL.SetCurSel(17);
				break;
			case 0x28:
				m_CKSEL.SetCurSel(18);
				break;
			case 0x38:
				m_CKSEL.SetCurSel(19);
				break;
			case 0x09:
				m_CKSEL.SetCurSel(20);
				break;
			case 0x19:
				m_CKSEL.SetCurSel(21);
				break;
			case 0x29:
				m_CKSEL.SetCurSel(22);
				break;
			case 0x39:
				m_CKSEL.SetCurSel(23);
				break;
			case 0x0A:
				m_CKSEL.SetCurSel(24);
				break;
			case 0x1A:
				m_CKSEL.SetCurSel(25);
				break;
			case 0x2A:
				m_CKSEL.SetCurSel(26);
				break;
			case 0x3A:
				m_CKSEL.SetCurSel(27);
				break;
			case 0x0B:
				m_CKSEL.SetCurSel(28);
				break;
			case 0x1B:
				m_CKSEL.SetCurSel(29);
				break;
			case 0x2B:
				m_CKSEL.SetCurSel(30);
				break;
			case 0x3B:
				m_CKSEL.SetCurSel(31);
				break;
			case 0x0C:
				m_CKSEL.SetCurSel(32);
				break;
			case 0x1C:
				m_CKSEL.SetCurSel(33);
				break;
			case 0x2C:
				m_CKSEL.SetCurSel(34);
				break;
			case 0x3C:
				m_CKSEL.SetCurSel(35);
				break;
			case 0x0D:
				m_CKSEL.SetCurSel(36);
				break;
			case 0x1D:
				m_CKSEL.SetCurSel(37);
				break;
			case 0x2D:
				m_CKSEL.SetCurSel(38);
				break;
			case 0x3D:
				m_CKSEL.SetCurSel(39);
				break;
			case 0x0E:
				m_CKSEL.SetCurSel(40);
				break;
			case 0x1E:
				m_CKSEL.SetCurSel(41);
				break;
			case 0x2E:
				m_CKSEL.SetCurSel(42);
				break;
			case 0x3E:
				m_CKSEL.SetCurSel(43);
				break;
			case 0x0F:
				m_CKSEL.SetCurSel(44);
				break;
			case 0x1F:
				m_CKSEL.SetCurSel(45);
				break;
			case 0x2F:
				m_CKSEL.SetCurSel(46);
				break;
			case 0x3F:
				m_CKSEL.SetCurSel(47);
				break;
			}
		}
		if (wcscmp(_T("ATtinyISP2_5"), m_ChipInfo->description) == 0) //ATtiny87/167
		{
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
			case 0x03:
				m_CKSEL.SetCurSel(6);
				break;
			case 0x13:
				m_CKSEL.SetCurSel(7);
				break;
			case 0x23:
				m_CKSEL.SetCurSel(8);
				break;
			case 0x04:
				m_CKSEL.SetCurSel(9);
				break;
			case 0x14:
				m_CKSEL.SetCurSel(10);
				break;
			case 0x24:
				m_CKSEL.SetCurSel(11);
				break;
			case 0x05:
				m_CKSEL.SetCurSel(12);
				break;
			case 0x15:
				m_CKSEL.SetCurSel(13);
				break;
			case 0x25:
				m_CKSEL.SetCurSel(14);
				break;
			case 0x06:
				m_CKSEL.SetCurSel(15);
				break;
			case 0x16:
				m_CKSEL.SetCurSel(16);
				break;
			case 0x26:
				m_CKSEL.SetCurSel(17);
				break;
			case 0x07:
				m_CKSEL.SetCurSel(18);
				break;
			case 0x17:
				m_CKSEL.SetCurSel(19);
				break;
			case 0x27:
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
		}
		if (wcscmp(_T("ATtinyISP2_6"), m_ChipInfo->description) == 0) //ATtiny48/88
		{
			switch (LowFuse & 0x3F)
			{
			case 0x0C:
				m_CKSEL.SetCurSel(0);
				break;
			case 0x1C:
					m_CKSEL.SetCurSel(1);
					break;
			case 0x2C:
				m_CKSEL.SetCurSel(2);
				break;
			case 0x0E:
				m_CKSEL.SetCurSel(3);
				break;
			case 0x1E:
				m_CKSEL.SetCurSel(4);
				break;
			case 0x2E:
				m_CKSEL.SetCurSel(5);
				break;
			case 0x0F:
				m_CKSEL.SetCurSel(6);
				break;
			case 0x1F:
				m_CKSEL.SetCurSel(7);
				break;
			case 0x2F:
				m_CKSEL.SetCurSel(8);
				break;
			}
		}
	}
	else if (*m_pDataLen == NULL)
	{
		CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
		Encheck->SetCheck(0);
		m_SELFPRGEN.EnableWindow(FALSE);
		m_DWEN.EnableWindow(FALSE);
		m_EESAVE.EnableWindow(FALSE);
		m_SPIEN.EnableWindow(FALSE);
		m_WDTON.EnableWindow(FALSE);
		m_BODLevel.EnableWindow(FALSE);
		m_RSTDISBL.EnableWindow(FALSE);
		m_CKDIV8.EnableWindow(FALSE);
		m_CKOUT.EnableWindow(FALSE);
		m_CKSEL.EnableWindow(FALSE);
		m_Lock.EnableWindow(FALSE);
	}
	return CMFCPropertyPage::OnSetActive();
}
void CDlg_ATtinyISP_2::OnCbnSelchangeCombo()
{
	CString ExFuse_str, HFuse_str, LFuse_str, Lockbits_str;
	ExtendedFuse = 0xFF;
	HighFuse = 0xDF;
	LowFuse = 0x62;
	Lockbits = 0xFF;
	//SELFPRGEN
	ExtendedFuse &= 0xFE;
	if (m_SELFPRGEN.GetCurSel() == 1)
		ExtendedFuse |= 0x01;
	else if (m_SELFPRGEN.GetCurSel() == 0)
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
	//BODlevel
	HighFuse &= 0xF8;
	switch(m_BODLevel.GetCurSel())
	{
	case 0 :
		HighFuse |= 0x00;
		break;
	case 1:
		HighFuse |= 0x01;
		break;
	case 2:
		HighFuse |= 0x02;
		break;
	case 3:
		HighFuse |= 0x03;
		break;
	case 4:
		HighFuse |= 0x04;
		break;
	case 5:
		HighFuse |= 0x05;
		break;
	case 6:
		HighFuse |= 0x06;
		break;
	case 7:
		HighFuse |= 0x07;
		break;
	}
	HFuse_str.Format(_T("%02X"), HighFuse);
	m_HFuse.SetWindowTextW(HFuse_str);
	//Lock
	Lockbits &= 0xFC;
	if (m_Lock.GetCurSel() == 2)
		Lockbits |= 0x03;
	else if (m_Lock.GetCurSel() == 1)
		Lockbits |= 0x02;
	else if (m_Lock.GetCurSel() == 0)
		Lockbits |= 0x00;
	Lockbits_str.Format(_T("%02X"), Lockbits);
	m_Lockbits.SetWindowTextW(Lockbits_str);
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
	//ATtinyISP2_1
	if (wcscmp(_T("ATtinyISP2_1"), m_ChipInfo->description) == 0) //ATtiny24/44/84
	{
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
			LowFuse |= 0x02;
			break;
		case 4:
			LowFuse |= 0x12;
			break;
		case 5:
			LowFuse |= 0x22;
			break;
		case 6:
			LowFuse |= 0x04;
			break;
		case 7:
			LowFuse |= 0x14;
			break;
		case 8:
			LowFuse |= 0x24;
			break;
		case 9:
			LowFuse |= 0x06;
			break;
		case 10:
			LowFuse |= 0x16;
			break;
		case 11:
			LowFuse |= 0x26;
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
	}
	if (wcscmp(_T("ATtinyISP2_2"), m_ChipInfo->description) == 0) //ATtiny25/45/85
	{
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
			LowFuse |= 0x31;
			break;
		case 7:
			LowFuse |= 0x02;
			break;
		case 8:
			LowFuse |= 0x12;
			break;
		case 9:
			LowFuse |= 0x22;
			break;
		case 10:
			LowFuse |= 0x03;
			break;
		case 11:
			LowFuse |= 0x13;
			break;
		case 12:
			LowFuse |= 0x23;
			break;
		case 13:
			LowFuse |= 0x33;
			break;
		case 14:
			LowFuse |= 0x04;
			break;
		case 15:
			LowFuse |= 0x14;
			break;
		case 16:
			LowFuse |= 0x24;
			break;
		case 17:
			LowFuse |= 0x06;
			break;
		case 18:
			LowFuse |= 0x16;
			break;
		case 19:
			LowFuse |= 0x26;
			break;
		case 20:
			LowFuse |= 0x08;
			break;
		case 21:
			LowFuse |= 0x18;
			break;
		case 22:
			LowFuse |= 0x28;
			break;
		case 23:
			LowFuse |= 0x38;
			break;
		case 24:
			LowFuse |= 0x09;
			break;
		case 25:
			LowFuse |= 0x19;
			break;
		case 26:
			LowFuse |= 0x29;
			break;
		case 27:
			LowFuse |= 0x39;
			break;
		case 28:
			LowFuse |= 0x0A;
			break;
		case 29:
			LowFuse |= 0x1A;
			break;
		case 30:
			LowFuse |= 0x2A;
			break;
		case 31:
			LowFuse |= 0x3A;
			break;
		case 32:
			LowFuse |= 0x0B;
			break;
		case 33:
			LowFuse |= 0x1B;
			break;
		case 34:
			LowFuse |= 0x2B;
			break;
		case 35:
			LowFuse |= 0x3B;
			break;
		case 36:
			LowFuse |= 0x0C;
			break;
		case 37:
			LowFuse |= 0x1C;
			break;
		case 38:
			LowFuse |= 0x2C;
			break;
		case 39:
			LowFuse |= 0x3C;
			break;
		case 40:
			LowFuse |= 0x0D;
			break;
		case 41:
			LowFuse |= 0x1D;
			break;
		case 42:
			LowFuse |= 0x2D;
			break;
		case 43:
			LowFuse |= 0x3D;
			break;
		case 44:
			LowFuse |= 0x0E;
			break;
		case 45:
			LowFuse |= 0x1E;
			break;
		case 46:
			LowFuse |= 0x2E;
			break;
		case 47:
			LowFuse |= 0x3E;
			break;
		case 48:
			LowFuse |= 0x0F;
			break;
		case 49:
			LowFuse |= 0x1F;
			break;
		case 50:
			LowFuse |= 0x2F;
			break;
		case 51:
			LowFuse |= 0x3F;
			break;
		}
		LFuse_str.Format(_T("%02X"), LowFuse);
		m_LFuse.SetWindowTextW(LFuse_str);
	}
	if (wcscmp(_T("ATtinyISP2_3"), m_ChipInfo->description) == 0 || wcscmp(_T("ATtinyISP2_4"), m_ChipInfo->description) == 0)
		//ATtiny43U  ATtiny261/461/861
	{
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
			LowFuse |= 0x31;
			break;
		case 7:
			LowFuse |= 0x02;
			break;
		case 8:
			LowFuse |= 0x12;
			break;
		case 9:
			LowFuse |= 0x22;
			break;
		case 10:
			LowFuse |= 0x03;
			break;
		case 11:
			LowFuse |= 0x13;
			break;
		case 12:
			LowFuse |= 0x23;
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
			LowFuse |= 0x08;
			break;
		case 17:
			LowFuse |= 0x18;
			break;
		case 18:
			LowFuse |= 0x28;
			break;
		case 19:
			LowFuse |= 0x38;
			break;
		case 20:
			LowFuse |= 0x09;
			break;
		case 21:
			LowFuse |= 0x19;
			break;
		case 22:
			LowFuse |= 0x29;
			break;
		case 23:
			LowFuse |= 0x39;
			break;
		case 24:
			LowFuse |= 0x0A;
			break;
		case 25:
			LowFuse |= 0x1A;
			break;
		case 26:
			LowFuse |= 0x2A;
			break;
		case 27:
			LowFuse |= 0x3A;
			break;
		case 28:
			LowFuse |= 0x0B;
			break;
		case 29:
			LowFuse |= 0x1B;
			break;
		case 30:
			LowFuse |= 0x2B;
			break;
		case 31:
			LowFuse |= 0x3B;
			break;
		case 32:
			LowFuse |= 0x0C;
			break;
		case 33:
			LowFuse |= 0x1C;
			break;
		case 34:
			LowFuse |= 0x2C;
			break;
		case 35:
			LowFuse |= 0x3C;
			break;
		case 36:
			LowFuse |= 0x0D;
			break;
		case 37:
			LowFuse |= 0x1D;
			break;
		case 38:
			LowFuse |= 0x2D;
			break;
		case 39:
			LowFuse |= 0x3D;
			break;
		case 40:
			LowFuse |= 0x0E;
			break;
		case 41:
			LowFuse |= 0x1E;
			break;
		case 42:
			LowFuse |= 0x2E;
			break;
		case 43:
			LowFuse |= 0x3E;
			break;
		case 44:
			LowFuse |= 0x0F;
			break;
		case 45:
			LowFuse |= 0x1F;
			break;
		case 46:
			LowFuse |= 0x2F;
			break;
		case 47:
			LowFuse |= 0x3F;
			break;
		}
		LFuse_str.Format(_T("%02X"), LowFuse);
		m_LFuse.SetWindowTextW(LFuse_str);
	}
	if (wcscmp(_T("ATtinyISP2_5"), m_ChipInfo->description) == 0) //ATtiny261/461/861
	{
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
			LowFuse |= 0x02;
			break;
		case 4:
			LowFuse |= 0x12;
			break;
		case 5:
			LowFuse |= 0x22;
			break;
		case 6:
			LowFuse |= 0x03;
			break;
		case 7:
			LowFuse |= 0x13;
			break;
		case 8:
			LowFuse |= 0x23;
			break;
		case 9:
			LowFuse |= 0x04;
			break;
		case 10:
			LowFuse |= 0x14;
			break;
		case 11:
			LowFuse |= 0x24;
			break;
		case 12:
			LowFuse |= 0x05;
			break;
		case 13:
			LowFuse |= 0x15;
			break;
		case 14:
			LowFuse |= 0x25;
			break;
		case 15:
			LowFuse |= 0x06;
			break;
		case 16:
			LowFuse |= 0x16;
			break;
		case 17:
			LowFuse |= 0x26;
			break;
		case 18:
			LowFuse |= 0x07;
			break;
		case 19:
			LowFuse |= 0x17;
			break;
		case 20:
			LowFuse |= 0x27;
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
	}
	if (wcscmp(_T("ATtinyISP2_6"), m_ChipInfo->description) == 0) //ATtiny261/461/861
	{
		LowFuse &= 0xC0;
		switch (m_CKSEL.GetCurSel())
		{
		case 0 :
			LowFuse |= 0x0C;
			break;
		case 1:
			LowFuse |= 0x1C;
			break;
		case 2:
			LowFuse |= 0x2C;
			break;
		case 3:
			LowFuse |= 0x0E;
			break;
		case 4:
			LowFuse |= 0x1E;
			break;
		case 5:
			LowFuse |= 0x2E;
			break;
		case 6:
			LowFuse |= 0x0F;
			break;
		case 7:
			LowFuse |= 0x1F;
			break;
		case 8:
			LowFuse |= 0x2F;
			break;
		}
		LFuse_str.Format(_T("%02X"), LowFuse);
		m_LFuse.SetWindowTextW(LFuse_str);
	}

}

void CDlg_ATtinyISP_2::OnOK()
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

LRESULT CDlg_ATtinyISP_2::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();

}
LRESULT CDlg_ATtinyISP_2::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}

void CDlg_ATtinyISP_2::OnBnClickedEnable()
{
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	int state = Encheck->GetCheck();
	if (state == 1)
	{
		m_SELFPRGEN.EnableWindow(TRUE);
		m_DWEN.EnableWindow(TRUE);
		m_EESAVE.EnableWindow(TRUE);
		m_SPIEN.EnableWindow(FALSE);
		m_WDTON.EnableWindow(TRUE);
		m_BODLevel.EnableWindow(TRUE);
		m_RSTDISBL.EnableWindow(TRUE);
		m_CKDIV8.EnableWindow(TRUE);
		m_CKOUT.EnableWindow(TRUE);
		m_CKSEL.EnableWindow(TRUE);
		m_Lock.EnableWindow(TRUE);
	}
	else if (state == 0)
	{
		m_SELFPRGEN.EnableWindow(FALSE);
		m_DWEN.EnableWindow(FALSE);
		m_EESAVE.EnableWindow(FALSE);
		m_SPIEN.EnableWindow(FALSE);
		m_WDTON.EnableWindow(FALSE);
		m_BODLevel.EnableWindow(FALSE);
		m_RSTDISBL.EnableWindow(FALSE);
		m_CKDIV8.EnableWindow(FALSE);
		m_CKOUT.EnableWindow(FALSE);
		m_CKSEL.EnableWindow(FALSE);
		m_Lock.EnableWindow(FALSE);
	}
}
