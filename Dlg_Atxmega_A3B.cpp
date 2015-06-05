// Dlg_Atxmega_A3B.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_Atxmega_A3B.h"
#include "numeric_conversion.h"
#include "../common/prog_info.h"

// Dlg_Atxmega_A3B dialog

IMPLEMENT_DYNAMIC(CDlg_Atxmega_A3B, CMFCPropertyPage)

CDlg_Atxmega_A3B::CDlg_Atxmega_A3B(struct chip_info_c *ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_Atxmega_A3B::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_Atxmega_A3B::~CDlg_Atxmega_A3B()
{
}

void CDlg_Atxmega_A3B::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_JtagUID);
	DDX_Control(pDX, IDC_COMBO1, m_WDWPER);
	DDX_Control(pDX, IDC_COMBO2, m_WDPER);
	DDX_Control(pDX, IDC_COMBO3, m_BootRst);
	DDX_Control(pDX, IDC_COMBO4, m_TOSCSel);
	DDX_Control(pDX, IDC_COMBO5, m_BODPD);
	DDX_Control(pDX, IDC_COMBO6, m_RSTDISBL);
	DDX_Control(pDX, IDC_COMBO7, m_StartupTime);
	DDX_Control(pDX, IDC_COMBO8, m_WDLOCK);
	DDX_Control(pDX, IDC_COMBO9, m_JTAGEN);
	DDX_Control(pDX, IDC_COMBO10, m_BODACT);
	DDX_Control(pDX, IDC_COMBO11, m_EESAVE);
	DDX_Control(pDX, IDC_COMBO12, m_BODLevel);
	DDX_Control(pDX, IDC_COMBO13, m_BLBB);
	DDX_Control(pDX, IDC_COMBO14, m_BLBA);
	DDX_Control(pDX, IDC_COMBO15, m_BLBAT);
	DDX_Control(pDX, IDC_COMBO16, m_LB);
	DDX_Control(pDX, IDC_EDIT2, m_Cfg0);
	DDX_Control(pDX, IDC_EDIT3, m_Cfg1);
	DDX_Control(pDX, IDC_EDIT4, m_Cfg2);
	DDX_Control(pDX, IDC_EDIT5, m_Cfg4);
	DDX_Control(pDX, IDC_EDIT6, m_Cfg5);
	DDX_Control(pDX, IDC_EDIT7, m_Cfg6);
	DDX_Control(pDX, IDC_CHECK1, Option_Enable);
}


BEGIN_MESSAGE_MAP(CDlg_Atxmega_A3B, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_Atxmega_A3B::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_Atxmega_A3B::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_Atxmega_A3B::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_Atxmega_A3B::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_Atxmega_A3B::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDlg_Atxmega_A3B::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDlg_Atxmega_A3B::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CDlg_Atxmega_A3B::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO9, &CDlg_Atxmega_A3B::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO10, &CDlg_Atxmega_A3B::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO11, &CDlg_Atxmega_A3B::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO12, &CDlg_Atxmega_A3B::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO13, &CDlg_Atxmega_A3B::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO14, &CDlg_Atxmega_A3B::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO15, &CDlg_Atxmega_A3B::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO16, &CDlg_Atxmega_A3B::OnCbnSelchangeCombo)
	ON_EN_CHANGE(IDC_EDIT1, &CDlg_Atxmega_A3B::OnEnChangeJTAGUID)
	ON_BN_CLICKED(IDC_CHECK1, &CDlg_Atxmega_A3B::OnBnClickedEnable)
END_MESSAGE_MAP()


// Dlg_Atxmega_A3B message handlers
BOOL CDlg_Atxmega_A3B::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	Fuse0 = 0xFF;
	Fuse1 = 0xFF;
	Fuse2 = 0xFF;
	Fuse4 = 0xFF;
	Fuse5 = 0xFF;
	Lockbits = 0xFF;
		
	m_WDWPER.InsertString(m_WDWPER.GetCount(), L"0000 = 8CLK");
	m_WDWPER.InsertString(m_WDWPER.GetCount(), L"0001 = 16CLK");
	m_WDWPER.InsertString(m_WDWPER.GetCount(), L"0010 = 32CLK");
	m_WDWPER.InsertString(m_WDWPER.GetCount(), L"0011 = 64CLK");
	m_WDWPER.InsertString(m_WDWPER.GetCount(), L"0100 = 128CLK");
	m_WDWPER.InsertString(m_WDWPER.GetCount(), L"0101 = 256CLK");
	m_WDWPER.InsertString(m_WDWPER.GetCount(), L"0110 = 512CLK");
	m_WDWPER.InsertString(m_WDWPER.GetCount(), L"0111 = 1KCLK");
	m_WDWPER.InsertString(m_WDWPER.GetCount(), L"1000 = 2KCLK");
	m_WDWPER.InsertString(m_WDWPER.GetCount(), L"1001 = 4KCLK");
	m_WDWPER.InsertString(m_WDWPER.GetCount(), L"1010 = 8KCLK");
	m_WDWPER.InsertString(m_WDWPER.GetCount(), L"1111 = Disabled");
	
	m_WDPER.InsertString(m_WDPER.GetCount(), L"0000 = 8CLK");
	m_WDPER.InsertString(m_WDPER.GetCount(), L"0001 = 16CLK");
	m_WDPER.InsertString(m_WDPER.GetCount(), L"0010 = 32CLK");
	m_WDPER.InsertString(m_WDPER.GetCount(), L"0011 = 64CLK");
	m_WDPER.InsertString(m_WDPER.GetCount(), L"0100 = 128CLK");
	m_WDPER.InsertString(m_WDPER.GetCount(), L"0101 = 256CLK");
	m_WDPER.InsertString(m_WDPER.GetCount(), L"0110 = 512CLK");
	m_WDPER.InsertString(m_WDPER.GetCount(), L"0111 = 1KCLK");
	m_WDPER.InsertString(m_WDPER.GetCount(), L"1000 = 2KCLK");
	m_WDPER.InsertString(m_WDPER.GetCount(), L"1001 = 4KCLK");
	m_WDPER.InsertString(m_WDPER.GetCount(), L"1010 = 8KCLK");
	m_WDPER.InsertString(m_WDPER.GetCount(), L"1111 = Disabled");

	m_BootRst.InsertString(m_BootRst.GetCount(), L"0 = Boot Loader Reset");
	m_BootRst.InsertString(m_BootRst.GetCount(), L"1 = Application Reset");

	m_TOSCSel.InsertString(m_TOSCSel.GetCount(), L"0 = ALTERNATE");
	m_TOSCSel.InsertString(m_TOSCSel.GetCount(), L"1 = XTAL");

	m_BODPD.InsertString(m_BODPD.GetCount(), L"01 = Sampled Mode");
	m_BODPD.InsertString(m_BODPD.GetCount(), L"10 = Continuously");
	m_BODPD.InsertString(m_BODPD.GetCount(), L"11 = Disabled");

	m_RSTDISBL.InsertString(m_RSTDISBL.GetCount(), L"0 = Enabled");
	m_RSTDISBL.InsertString(m_RSTDISBL.GetCount(), L"1 = Disabled");

	m_StartupTime.InsertString(m_StartupTime.GetCount(), L"00 = 64MS");
	m_StartupTime.InsertString(m_StartupTime.GetCount(), L"01 = 4MS");
	m_StartupTime.InsertString(m_StartupTime.GetCount(), L"11 = 0MS");

	m_WDLOCK.InsertString(m_WDLOCK.GetCount(), L"0 = Watchdog Timer locked for modfications");
	m_WDLOCK.InsertString(m_WDLOCK.GetCount(), L"1 = Watchdog Timer not locked");

	m_JTAGEN.InsertString(m_JTAGEN.GetCount(), L"0 = JTAG Enabled");
	m_JTAGEN.InsertString(m_JTAGEN.GetCount(), L"1 = JTAG Disabled");

	m_BODACT.InsertString(m_BODACT.GetCount(), L"01 = Sampled Mode");
	m_BODACT.InsertString(m_BODACT.GetCount(), L"10 = Continuously");
	m_BODACT.InsertString(m_BODACT.GetCount(), L"11 = Disabled");

	m_EESAVE.InsertString(m_EESAVE.GetCount(), L"0 = EEPROM is preserved during chip erase");
	m_EESAVE.InsertString(m_EESAVE.GetCount(), L"1 = EEPROM is erased during chip erase");

	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"000 = 3.0V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"001 = 2.8V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"010 = 2.6V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"011 = 2.4V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"100 = 2.2V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"101 = 2.0V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"110 = 1.8V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"111 = 1.6V");

	m_BLBB.InsertString(m_BLBB.GetCount(), L"00 = R/W Lock");
	m_BLBB.InsertString(m_BLBB.GetCount(), L"01 = R Lock");
	m_BLBB.InsertString(m_BLBB.GetCount(), L"10 = W Lock");
	m_BLBB.InsertString(m_BLBB.GetCount(), L"11 = No Lock");

	m_BLBA.InsertString(m_BLBA.GetCount(), L"00 = R/W Lock");
	m_BLBA.InsertString(m_BLBA.GetCount(), L"01 = R Lock");
	m_BLBA.InsertString(m_BLBA.GetCount(), L"10 = W Lock");
	m_BLBA.InsertString(m_BLBA.GetCount(), L"11 = No Lock");

	m_BLBAT.InsertString(m_BLBAT.GetCount(), L"00 = R/W Lock");
	m_BLBAT.InsertString(m_BLBAT.GetCount(), L"01 = R Lock");
	m_BLBAT.InsertString(m_BLBAT.GetCount(), L"10 = W Lock");
	m_BLBAT.InsertString(m_BLBAT.GetCount(), L"11 = No Lock");

	m_LB.InsertString(m_LB.GetCount(), L"00 = R/W Lock");
	m_LB.InsertString(m_LB.GetCount(), L"10 = W Lock");
	m_LB.InsertString(m_LB.GetCount(), L"11 = No Lock");

	m_WDWPER.SetDroppedWidth(150);
	m_WDPER.SetDroppedWidth(150);
	m_BootRst.SetDroppedWidth(150);
	m_TOSCSel.SetDroppedWidth(150);
	m_BODPD.SetDroppedWidth(150);
	m_RSTDISBL.SetDroppedWidth(150);
	m_StartupTime.SetDroppedWidth(150);
	m_WDLOCK.SetDroppedWidth(300);
	m_JTAGEN.SetDroppedWidth(150);
	m_BODACT.SetDroppedWidth(150);
	m_EESAVE.SetDroppedWidth(300);
	m_BODLevel.SetDroppedWidth(150);
	m_BLBB.SetDroppedWidth(150);
	m_BLBA.SetDroppedWidth(150);
	m_BLBAT.SetDroppedWidth(150);
	m_LB.SetDroppedWidth(150);

	m_JtagUID.SetWindowTextW(L"FF");	//Fuse0
	m_Cfg0.SetWindowTextW(L"FF");		//Fuse0
	m_Cfg0.EnableWindow(false);
	m_Cfg1.SetWindowTextW(L"FF");		//Fuse1
	m_Cfg1.EnableWindow(false);
	m_Cfg2.SetWindowTextW(L"FF");		//Fuse2
	m_Cfg2.EnableWindow(false);
	m_Cfg4.SetWindowTextW(L"FF");		//Fuse4
	m_Cfg4.EnableWindow(false);
	m_Cfg5.SetWindowTextW(L"FF");		//Fuse5
	m_Cfg5.EnableWindow(false);
	m_Cfg6.SetWindowTextW(L"FF");		//LockBits
	m_Cfg6.EnableWindow(false);

	m_WDWPER.SetCurSel(11);
	m_WDPER.SetCurSel(11);
	m_BootRst.SetCurSel(1);
	m_TOSCSel.SetCurSel(1);
	m_BODPD.SetCurSel(2);
	m_RSTDISBL.SetCurSel(1);
	m_StartupTime.SetCurSel(2);
	m_WDLOCK.SetCurSel(1);
	m_JTAGEN.SetCurSel(1);
	m_BODACT.SetCurSel(2);
	m_EESAVE.SetCurSel(1);
	m_BODLevel.SetCurSel(7);
	m_BLBB.SetCurSel(3);
	m_BLBA.SetCurSel(3);
	m_BLBAT.SetCurSel(3);
	m_LB.SetCurSel(2);

	m_JtagUID.EnableWindow(false);
	m_WDWPER.EnableWindow(false);
	m_WDPER.EnableWindow(false);
	m_BootRst.EnableWindow(false);
	m_TOSCSel.EnableWindow(false);
	m_BODPD.EnableWindow(false);
	m_RSTDISBL.EnableWindow(false);
	m_StartupTime.EnableWindow(false);
	m_WDLOCK.EnableWindow(false);
	m_JTAGEN.EnableWindow(false);
	m_BODACT.EnableWindow(false);
	m_EESAVE.EnableWindow(false);
	m_BODLevel.EnableWindow(false);
	m_BLBB.EnableWindow(false);
	m_BLBA.EnableWindow(false);
	m_BLBAT.EnableWindow(false);
	m_LB.EnableWindow(false);

	return TRUE;
}
BOOL CDlg_Atxmega_A3B::OnSetActive()
{
	if (*m_pDataLen != NULL)
	{
		m_JtagUID.EnableWindow(true);
		m_WDWPER.EnableWindow(true);
		m_WDPER.EnableWindow(true);
		m_BootRst.EnableWindow(true);
		m_TOSCSel.EnableWindow(true);
		m_BODPD.EnableWindow(true);
		m_RSTDISBL.EnableWindow(true);
		m_StartupTime.EnableWindow(true);
		m_WDLOCK.EnableWindow(true);
		m_JTAGEN.EnableWindow(true);
		m_BODACT.EnableWindow(true);
		m_EESAVE.EnableWindow(true);
		m_BODLevel.EnableWindow(true);
		m_BLBB.EnableWindow(true);
		m_BLBA.EnableWindow(true);
		m_BLBAT.EnableWindow(true);
		m_LB.EnableWindow(true);

		CString Cfg0;
		CString Cfg1;
		CString Cfg2;
		CString Cfg4;
		CString Cfg5;
		CString Cfg6;

		CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
		Encheck->SetCheck(1);
		
		memcpy((void*)&Fuse0, m_RegBuff, sizeof(Fuse0));
		Cfg0.Format(_T("%02X"), Fuse0);
		m_JtagUID.SetWindowTextW(Cfg0);
		m_Cfg0.SetWindowTextW(Cfg0);
		
		memcpy((void*)&Fuse1, m_RegBuff + 1, sizeof(Fuse1));
		Cfg1.Format(_T("%02X"), Fuse1);
		m_Cfg1.SetWindowTextW(Cfg1);
		
		memcpy((void*)&Fuse2, m_RegBuff + 2, sizeof(Fuse2));
		Cfg2.Format(_T("%02X"), Fuse2);
		m_Cfg2.SetWindowTextW(Cfg2);
		
		memcpy((void*)&Fuse4, m_RegBuff + 3, sizeof(Fuse4));
		Cfg4.Format(_T("%02X"), Fuse4);
		m_Cfg4.SetWindowTextW(Cfg4);
		
		memcpy((void*)&Fuse5, m_RegBuff + 4, sizeof(Fuse5));
		Cfg5.Format(_T("%02X"), Fuse5);
		m_Cfg5.SetWindowTextW(Cfg5);

		memcpy((void*)&Lockbits, m_RegBuff + 5, sizeof(Lockbits));
		Cfg6.Format(_T("%02X"), Lockbits);
		m_Cfg6.SetWindowTextW(Cfg6);

		//Watchdog Window Timeout Period
		if ((Fuse1 & 0xF0) == 0xF0)
			m_WDWPER.SetCurSel(11);
		else if ((Fuse1 & 0xF0) == 0xA0)
			m_WDWPER.SetCurSel(10);
		else if ((Fuse1 & 0xF0) == 0x90)
			m_WDWPER.SetCurSel(9);
		else if ((Fuse1 & 0xF0) == 0x80)
			m_WDWPER.SetCurSel(8);
		else if ((Fuse1 & 0xF0) == 0x70)
			m_WDWPER.SetCurSel(7);
		else if ((Fuse1 & 0xF0) == 0x60)
			m_WDWPER.SetCurSel(6);
		else if ((Fuse1 & 0xF0) == 0x50)
			m_WDWPER.SetCurSel(5);
		else if ((Fuse1 & 0xF0) == 0x40)
			m_WDWPER.SetCurSel(4);
		else if ((Fuse1 & 0xF0) == 0x30)
			m_WDWPER.SetCurSel(3);
		else if ((Fuse1 & 0xF0) == 0x20)
			m_WDWPER.SetCurSel(2);
		else if ((Fuse1 & 0xF0) == 0x10)
			m_WDWPER.SetCurSel(1);
		else if ((Fuse1 & 0xF0) == 0x00)
			m_WDWPER.SetCurSel(0);

		//Watchdog Timeout Period
		if ((Fuse1 & 0x0F) == 0x0F)
			m_WDPER.SetCurSel(11);
		else if ((Fuse1 & 0x0F) == 0x0A)
			m_WDPER.SetCurSel(10);
		else if ((Fuse1 & 0x0F) == 0x09)
			m_WDPER.SetCurSel(9);
		else if ((Fuse1 & 0x0F) == 0x08)
			m_WDPER.SetCurSel(8);
		else if ((Fuse1 & 0x0F) == 0x07)
			m_WDPER.SetCurSel(7);
		else if ((Fuse1 & 0x0F) == 0x06)
			m_WDPER.SetCurSel(6);
		else if ((Fuse1 & 0x0F) == 0x05)
			m_WDPER.SetCurSel(5);
		else if ((Fuse1 & 0x0F) == 0x04)
			m_WDPER.SetCurSel(4);
		else if ((Fuse1 & 0x0F) == 0x03)
			m_WDPER.SetCurSel(3);
		else if ((Fuse1 & 0x0F) == 0x02)
			m_WDPER.SetCurSel(2);
		else if ((Fuse1 & 0x0F) == 0x01)
			m_WDPER.SetCurSel(1);
		else if ((Fuse1 & 0x0F) == 0x00)
			m_WDPER.SetCurSel(0);

		//Boot Loader Section Reset Vector
		if ((Fuse2 & 0x40) == 0x40)
			m_BootRst.SetCurSel(1);
		else if ((Fuse2 & 0x40) == 0x00)
			m_BootRst.SetCurSel(0);

		//32.768KHz Timer OSC Pin Selection
		if ((Fuse2 & 0x20) == 0x20)
			m_TOSCSel.SetCurSel(1);
		else if ((Fuse2 & 0x20) == 0x00)
			m_TOSCSel.SetCurSel(0);

		//BOD Operation in Power-down Mode
		if ((Fuse2 & 0x03) == 0x03)
			m_BODPD.SetCurSel(2);
		else if ((Fuse2 & 0x03) == 0x02)
			m_BODPD.SetCurSel(1);
		else if ((Fuse2 & 0x03) == 0x01)
			m_BODPD.SetCurSel(0);

		//External Reset Disable
		if ((Fuse4 & 0x10) == 0x10)
			m_RSTDISBL.SetCurSel(1);
		else if ((Fuse4 & 0x10) == 0x00)
			m_RSTDISBL.SetCurSel(0);

		//Start-up Time
		if ((Fuse4 & 0x0C) == 0x0C)
			m_StartupTime.SetCurSel(2);
		else if ((Fuse4 & 0x0C) == 0x04)
			m_StartupTime.SetCurSel(1);
		else if ((Fuse4 & 0x0C) == 0x00)
			m_StartupTime.SetCurSel(0);

		//Watchdog Timer Lock
		if ((Fuse4 & 0x02) == 0x02)
			m_WDLOCK.SetCurSel(1);
		else if ((Fuse4 & 0x02) == 0x00)
			m_WDLOCK.SetCurSel(0);

		//JTAGEN
		if ((Fuse4 & 0x01) == 0x01)
			m_JTAGEN.SetCurSel(1);
		else if ((Fuse4 & 0x01) == 0x00)
			m_JTAGEN.SetCurSel(0);

		//BOD Operation in Active Mode
		if ((Fuse5 & 0x30) == 0x30)
			m_BODACT.SetCurSel(2);
		else if ((Fuse5 & 0x30) == 0x20)
			m_BODACT.SetCurSel(1);
		else if ((Fuse5 & 0x30) == 0x10)
			m_BODACT.SetCurSel(0);

		//EEPROM Preserved through chip erase
		if ((Fuse5 & 0x08) == 0x08)
			m_EESAVE.SetCurSel(1);
		else if ((Fuse5 & 0x08) == 0x00)
			m_EESAVE.SetCurSel(0);

		//BOD Level Setting
		if ((Fuse5 & 0x07) == 0x07)
			m_BODLevel.SetCurSel(7);
		else if ((Fuse5 & 0x07) == 0x06)
			m_BODLevel.SetCurSel(6);
		else if ((Fuse5 & 0x07) == 0x05)
			m_BODLevel.SetCurSel(5);
		else if ((Fuse5 & 0x07) == 0x04)
			m_BODLevel.SetCurSel(4);
		else if ((Fuse5 & 0x07) == 0x03)
			m_BODLevel.SetCurSel(3);
		else if ((Fuse5 & 0x07) == 0x02)
			m_BODLevel.SetCurSel(2);
		else if ((Fuse5 & 0x07) == 0x01)
			m_BODLevel.SetCurSel(1);
		else if ((Fuse5 & 0x07) == 0x00)
			m_BODLevel.SetCurSel(0);

		//Boot Lock Bit for Boot Loader Section
		if ((Lockbits & 0xC0) == 0xC0)
			m_BLBB.SetCurSel(3);
		else if ((Lockbits & 0xC0) == 0x80)
			m_BLBB.SetCurSel(2);
		else if ((Lockbits & 0xC0) == 0x40)
			m_BLBB.SetCurSel(1);
		else if ((Lockbits & 0xC0) == 0x00)
			m_BLBB.SetCurSel(0);

		//Boot Lock Bit for Application Section
		if ((Lockbits & 0x30) == 0x30)
			m_BLBA.SetCurSel(3);
		else if((Lockbits & 0x30) == 0x20)
			m_BLBA.SetCurSel(2);
		else if ((Lockbits & 0x30) == 0x10)
			m_BLBA.SetCurSel(1);
		else if ((Lockbits & 0x30) == 0x00)
			m_BLBA.SetCurSel(0);

		//Boot Lock Bit for the Application table Section
		if ((Lockbits & 0x0C) == 0x0C)
			m_BLBAT.SetCurSel(3);
		else if ((Lockbits & 0x0C) == 0x08)
			m_BLBAT.SetCurSel(2);
		else if ((Lockbits & 0x0C) == 0x04)
			m_BLBAT.SetCurSel(1);
		else if ((Lockbits & 0x0C) == 0x00)
			m_BLBAT.SetCurSel(0);
		
		//Lock bit Protection Mode
		if ((Lockbits & 0x03) == 0x03)
			m_LB.SetCurSel(2);
		else if ((Lockbits & 0x03) == 0x02)
			m_LB.SetCurSel(1);
		else if ((Lockbits & 0x03) == 0x00)
			m_LB.SetCurSel(0);

	}
	else if (*m_pDataLen == NULL)
	{
		CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
		Encheck->SetCheck(0);
		m_JtagUID.EnableWindow(false);
		m_WDWPER.EnableWindow(false);
		m_WDPER.EnableWindow(false);
		m_BootRst.EnableWindow(false);
		m_TOSCSel.EnableWindow(false);
		m_BODPD.EnableWindow(false);
		m_RSTDISBL.EnableWindow(false);
		m_StartupTime.EnableWindow(false);
		m_WDLOCK.EnableWindow(false);
		m_JTAGEN.EnableWindow(false);
		m_BODACT.EnableWindow(false);
		m_EESAVE.EnableWindow(false);
		m_BODLevel.EnableWindow(false);
		m_BLBB.EnableWindow(false);
		m_BLBA.EnableWindow(false);
		m_BLBAT.EnableWindow(false);
		m_LB.EnableWindow(false);
	}
	return CMFCPropertyPage::OnSetActive();
}

void CDlg_Atxmega_A3B::OnCbnSelchangeCombo()
{
	CString Cfg1;
	CString Cfg2;
	CString Cfg4;
	CString Cfg5;
	CString Cfg6;
	Fuse1 = 0xFF;
	Fuse2 = 0xFF;
	Fuse4 = 0xFF;
	Fuse5 = 0xFF;
	Lockbits = 0xFF;

	//Watchdog Window Timeout Period
	Fuse1 &= 0x0F;
	switch (m_WDWPER.GetCurSel())
	{
	case 0:
		Fuse1 |= 0x00;
		break;
	case 1:
		Fuse1 |= 0x10;
		break;
	case 2:
		Fuse1 |= 0x20;
		break;
	case 3:
		Fuse1 |= 0x30;
		break;
	case 4:
		Fuse1 |= 0x40;
		break;
	case 5:
		Fuse1 |= 0x50;
		break;
	case 6:
		Fuse1 |= 0x60;
		break;
	case 7:
		Fuse1 |= 0x70;
		break;
	case 8:
		Fuse1 |= 0x80;
		break;
	case 9:
		Fuse1 |= 0x90;
		break;
	case 10:
		Fuse1 |= 0xA0;
		break;
	case 11:
		Fuse1 |= 0xF0;
		break;
	}
	Cfg1.Format(_T("%02X"), Fuse1);
	m_Cfg1.SetWindowTextW(Cfg1);

	//Watchdog Timeout Period
	Fuse1 &= 0xF0;
	switch (m_WDPER.GetCurSel())
	{
	case 0:
		Fuse1 |= 0x00;
		break;
	case 1:
		Fuse1 |= 0x01;
		break;
	case 2:
		Fuse1 |= 0x02;
		break;
	case 3:
		Fuse1 |= 0x03;
		break;
	case 4:
		Fuse1 |= 0x04;
		break;
	case 5:
		Fuse1 |= 0x05;
		break;
	case 6:
		Fuse1 |= 0x06;
		break;
	case 7:
		Fuse1 |= 0x07;
		break;
	case 8:
		Fuse1 |= 0x08;
		break;
	case 9:
		Fuse1 |= 0x09;
		break;
	case 10:
		Fuse1 |= 0x0A;
		break;
	case 11:
		Fuse1 |= 0x0F;
		break;
	}
	Cfg1.Format(_T("%02X"), Fuse1);
	m_Cfg1.SetWindowTextW(Cfg1);

	//Boot Loader Section Reset Vector
	Fuse2 &= 0xBF;
	if (m_BootRst.GetCurSel() == 1)
		Fuse2 |= 0x40;
	else if (m_BootRst.GetCurSel() == 0)
		Fuse2 |= 0x00;
	Cfg2.Format(_T("%02X"), Fuse2);
	m_Cfg2.SetWindowTextW(Cfg2);

	//32.768KHz Timer OSC Pin Selection
	Fuse2 &= 0xDF;
	if (m_TOSCSel.GetCurSel() == 1)
		Fuse2 |= 0x20;
	else if (m_TOSCSel.GetCurSel() == 0)
		Fuse2 |= 0x00;
	Cfg2.Format(_T("%02X"), Fuse2);
	m_Cfg2.SetWindowTextW(Cfg2);

	//BOD Operation in Power-down Mode
	Fuse2 &= 0xFC;
	if (m_BODPD.GetCurSel() == 2)
		Fuse2 |= 0x03;
	else if (m_BODPD.GetCurSel() == 1)
		Fuse2 |= 0x02;
	else if (m_BODPD.GetCurSel() == 0)
		Fuse2 |= 0x01;
	Cfg2.Format(_T("%02X"), Fuse2);
	m_Cfg2.SetWindowTextW(Cfg2);

	//External Reset Disable
	Fuse4 &= 0xEF;
	if (m_RSTDISBL.GetCurSel() == 1)
		Fuse4 |= 0x10;
	else if (m_RSTDISBL.GetCurSel() == 0)
		Fuse4 |= 0x00;
	Cfg4.Format(_T("%02X"), Fuse4);
	m_Cfg4.SetWindowTextW(Cfg4);

	//Start Up Time
	Fuse4 &= 0xF3;
	if (m_StartupTime.GetCurSel() == 2)
		Fuse4 |= 0x0C;
	else if (m_StartupTime.GetCurSel() == 1)
		Fuse4 |= 0x04;
	else if (m_StartupTime.GetCurSel() == 0)
		Fuse4 |= 0x00;
	Cfg4.Format(_T("%02X"), Fuse4);
	m_Cfg4.SetWindowTextW(Cfg4);

	//Watchdog Timer lock
	Fuse4 &= 0xFD;
	if (m_WDLOCK.GetCurSel() == 1)
		Fuse4 |= 0x02;
	else if (m_WDLOCK.GetCurSel() == 0)
		Fuse4 |= 0x00;
	Cfg4.Format(_T("%02X"), Fuse4);
	m_Cfg4.SetWindowTextW(Cfg4);

	//JTAG Enable
	Fuse4 &= 0xFE;
	if (m_JTAGEN.GetCurSel() == 1)
		Fuse4 |= 0x01;
	else if (m_JTAGEN.GetCurSel() == 0)
		Fuse4 |= 0x00;
	Cfg4.Format(_T("%02X"), Fuse4);
	m_Cfg4.SetWindowTextW(Cfg4);

	//BOD operation in Active Mode
	Fuse5 &= 0xCF;
	if (m_BODACT.GetCurSel() == 2)
		Fuse5 |= 0x30;
	else if (m_BODACT.GetCurSel() == 1)
		Fuse5 |= 0x20;
	else if (m_BODACT.GetCurSel() == 0)
		Fuse5 |= 0x10;
	Cfg5.Format(_T("%02X"), Fuse5);
	m_Cfg5.SetWindowTextW(Cfg5);

	//EEPROM preserved through chip erase
	Fuse5 &= 0xF7;
	if (m_EESAVE.GetCurSel() == 1)
		Fuse5 |= 0x08;
	else if (m_EESAVE.GetCurSel() == 0)
		Fuse5 |= 0x00;
	Cfg5.Format(_T("%02X"), Fuse5);
	m_Cfg5.SetWindowTextW(Cfg5);

	//Brownout Detection voltage level
	Fuse5 &= 0xF8;
	switch (m_BODLevel.GetCurSel())
	{
	case 0:
		Fuse5 |= 0x00;
		break;
	case 1:
		Fuse5 |= 0x01;
		break;
	case 2:
		Fuse5 |= 0x02;
		break;
	case 3:
		Fuse5 |= 0x03;
		break;
	case 4:
		Fuse5 |= 0x04;
		break;
	case 5:
		Fuse5 |= 0x05;
		break;
	case 6:
		Fuse5 |= 0x06;
		break;
	case 7:
		Fuse5 |= 0x07;
		break;
	}
	Cfg5.Format(_T("%02X"), Fuse5);
	m_Cfg5.SetWindowTextW(Cfg5);
	
	//Boot Lock bit for Boot Loader
	Lockbits &= 0x3F;
	switch (m_BLBB.GetCurSel())
	{
	case 0:
		Lockbits |= 0x00;
		break;
	case 1:
		Lockbits |= 0x40;
		break;
	case 2:
		Lockbits |= 0x80;
		break;
	case 3:
		Lockbits |= 0xC0;
		break;
	}
	Cfg6.Format(_T("%02X"), Lockbits);
	m_Cfg6.SetWindowTextW(Cfg6);

	//Boot Lock bit for Application
	Lockbits &= 0xCF;
	switch (m_BLBA.GetCurSel())
	{
	case 0:
		Lockbits |= 0x00;
		break;
	case 1:
		Lockbits |= 0x10;
		break;
	case 2:
		Lockbits |= 0x20;
		break;
	case 3:
		Lockbits |= 0x30;
		break;
	}
	Cfg6.Format(_T("%02X"), Lockbits);
	m_Cfg6.SetWindowTextW(Cfg6);

	//Boot Lock bit for Application table Section
	Lockbits &= 0xF3;
	switch (m_BLBAT.GetCurSel())
	{
	case 0:
		Lockbits |= 0x00;
		break;
	case 1:
		Lockbits |= 0x04;
		break;
	case 2:
		Lockbits |= 0x08;
		break;
	case 3:
		Lockbits |= 0x0C;
		break;
	}
	Cfg6.Format(_T("%02X"), Lockbits);
	m_Cfg6.SetWindowTextW(Cfg6);

	//Lock Bits
	Lockbits &= 0xFC;
	if (m_LB.GetCurSel() == 2)
		Lockbits |= 0x03;
	else if (m_LB.GetCurSel() == 1)
		Lockbits |= 0x02;
	else if (m_LB.GetCurSel() == 0)
		Lockbits |= 0x00;
	Cfg6.Format(_T("%02X"), Lockbits);
	m_Cfg6.SetWindowTextW(Cfg6);

}

void CDlg_Atxmega_A3B::OnOK()
{
	
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	int state = Encheck->GetCheck();
	if (state == 0x01)
	{
		CString temp;
		m_Cfg0.GetWindowTextW(temp);
		Fuse0 = numeric_conversion::hexstring_to_size_t(temp.GetString());
		memcpy(m_RegBuff, &Fuse0, 1);

		m_Cfg1.GetWindowTextW(temp);
		Fuse1 = numeric_conversion::hexstring_to_size_t(temp.GetString());
		memcpy(m_RegBuff + sizeof(Fuse0), &Fuse1, sizeof(Fuse1));

		m_Cfg2.GetWindowTextW(temp);
		Fuse2 = numeric_conversion::hexstring_to_size_t(temp.GetString());
		memcpy(m_RegBuff + sizeof(Fuse0)+sizeof(Fuse1), &Fuse2, sizeof(Fuse2));

		m_Cfg4.GetWindowTextW(temp);
		Fuse4 = numeric_conversion::hexstring_to_size_t(temp.GetString());
		memcpy(m_RegBuff + sizeof(Fuse0)+sizeof(Fuse1)+sizeof(Fuse2), &Fuse4, sizeof(Fuse4));

		m_Cfg5.GetWindowTextW(temp);
		Fuse5 = numeric_conversion::hexstring_to_size_t(temp.GetString());
		memcpy(m_RegBuff + sizeof(Fuse0)+sizeof(Fuse1)+sizeof(Fuse2)+sizeof(Fuse4), &Fuse5, sizeof(Fuse5));

		m_Cfg6.GetWindowTextW(temp);
		Lockbits = numeric_conversion::hexstring_to_size_t(temp.GetString());
		memcpy(m_RegBuff + sizeof(Fuse0)+sizeof(Fuse1)+sizeof(Fuse2)+sizeof(Fuse4)+sizeof(Fuse5), &Lockbits, sizeof(Lockbits));
		unsigned short int restdata = 0xFFFF;
		memcpy(m_RegBuff + sizeof(Fuse0)+sizeof(Fuse1)+sizeof(Fuse2)+sizeof(Fuse4)+sizeof(Fuse5)+sizeof(Lockbits), &restdata, sizeof(restdata));
		*m_pDataLen = sizeof(Fuse0)+sizeof(Fuse1)+sizeof(Fuse2)+sizeof(Fuse4)+sizeof(Fuse5)+sizeof(Lockbits)+sizeof(restdata);
	}
	else if (state == 0x00)
		*m_pDataLen = 0;

	CMFCPropertyPage::OnOK();
}
LRESULT CDlg_Atxmega_A3B::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();

}
LRESULT CDlg_Atxmega_A3B::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}


void CDlg_Atxmega_A3B::OnEnChangeJTAGUID()
{
	CString Cfg0;
	//Fuse0 = 0xFF;
	GetDlgItem(IDC_EDIT1)->GetWindowTextW(Cfg0);
	m_Cfg0.SetWindowTextW(Cfg0);
	Fuse0 = numeric_conversion::hexstring_to_size_t(Cfg0.GetString());
	
}


void CDlg_Atxmega_A3B::OnBnClickedEnable()
{
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	int state = Encheck->GetCheck();
	if (state == 0x1)
	{
		m_JtagUID.EnableWindow(true);
		m_WDWPER.EnableWindow(true);
		m_WDPER.EnableWindow(true);
		m_BootRst.EnableWindow(true);
		m_TOSCSel.EnableWindow(true);
		m_BODPD.EnableWindow(true);
		m_RSTDISBL.EnableWindow(true);
		m_StartupTime.EnableWindow(true);
		m_WDLOCK.EnableWindow(true);
		m_JTAGEN.EnableWindow(true);
		m_BODACT.EnableWindow(true);
		m_EESAVE.EnableWindow(true);
		m_BODLevel.EnableWindow(true);
		m_BLBB.EnableWindow(true);
		m_BLBA.EnableWindow(true);
		m_BLBAT.EnableWindow(true);
		m_LB.EnableWindow(true);
	}
	else if (state == 0x0)
	{
		m_JtagUID.EnableWindow(false);
		m_WDWPER.EnableWindow(false);
		m_WDPER.EnableWindow(false);
		m_BootRst.EnableWindow(false);
		m_TOSCSel.EnableWindow(false);
		m_BODPD.EnableWindow(false);
		m_RSTDISBL.EnableWindow(false);
		m_StartupTime.EnableWindow(false);
		m_WDLOCK.EnableWindow(false);
		m_JTAGEN.EnableWindow(false);
		m_BODACT.EnableWindow(false);
		m_EESAVE.EnableWindow(false);
		m_BODLevel.EnableWindow(false);
		m_BLBB.EnableWindow(false);
		m_BLBA.EnableWindow(false);
		m_BLBAT.EnableWindow(false);
		m_LB.EnableWindow(false);
	}
}
