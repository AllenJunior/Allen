// Dlg_ATtinyX41.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_ATtinyX41.h"
#include "afxdialogex.h"
#include "numeric_conversion.h"
#include "../common/prog_info.h"

// Dlg_ATtinyX41 dialog

IMPLEMENT_DYNAMIC(CDlg_ATtinyX41, CMFCPropertyPage)

CDlg_ATtinyX41::CDlg_ATtinyX41(struct chip_info_c *ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_ATtinyX41::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}
CDlg_ATtinyX41::~CDlg_ATtinyX41()
{
}

void CDlg_ATtinyX41::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, option_enable);
	DDX_Control(pDX, IDC_COMBO1, m_ULPOSCSEL);
	DDX_Control(pDX, IDC_COMBO2, m_BODPD);
	DDX_Control(pDX, IDC_COMBO3, m_BODACT);
	DDX_Control(pDX, IDC_COMBO4, m_SELFPRGEN);
	DDX_Control(pDX, IDC_COMBO5, m_RSTDISBL);
	DDX_Control(pDX, IDC_COMBO6, m_DWEN);
	DDX_Control(pDX, IDC_COMBO7, m_SPIEN);
	DDX_Control(pDX, IDC_COMBO8, m_WDTON);
	DDX_Control(pDX, IDC_COMBO9, m_EESAVE);
	DDX_Control(pDX, IDC_COMBO10, m_BODLevel);
	DDX_Control(pDX, IDC_COMBO11, m_CKDIV8);
	DDX_Control(pDX, IDC_COMBO12, m_CKOUT);
	DDX_Control(pDX, IDC_COMBO13, m_CKSEL);
	DDX_Control(pDX, IDC_COMBO14, m_Lock);
	DDX_Control(pDX, IDC_EDIT1, m_ExFuse);
	DDX_Control(pDX, IDC_EDIT2, m_HFuse);
	DDX_Control(pDX, IDC_EDIT3, m_LFuse);
	DDX_Control(pDX, IDC_EDIT4, m_Lockbits);
}


BEGIN_MESSAGE_MAP(CDlg_ATtinyX41, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_ATtinyX41::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_ATtinyX41::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_ATtinyX41::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_ATtinyX41::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_ATtinyX41::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDlg_ATtinyX41::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDlg_ATtinyX41::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CDlg_ATtinyX41::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO9, &CDlg_ATtinyX41::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO10, &CDlg_ATtinyX41::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO11, &CDlg_ATtinyX41::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO12, &CDlg_ATtinyX41::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO13, &CDlg_ATtinyX41::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO14, &CDlg_ATtinyX41::OnCbnSelchangeCombo)
	ON_BN_CLICKED(IDC_CHECK1, &CDlg_ATtinyX41::OnBnClickedEnable)
END_MESSAGE_MAP()


// Dlg_ATtinyX41 message handlers
BOOL CDlg_ATtinyX41::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	ExtendedFuse = 0xFF;
	HighFuse = 0xDF;
	LowFuse = 0x62;
	Lockbits = 0xFF;
	
	m_ExFuse.SetWindowTextW(L"FF");
	m_HFuse.SetWindowTextW(L"DF");
	m_LFuse.SetWindowTextW(L"62");
	m_Lockbits.SetWindowTextW(L"FF");
	//ComboBox Setting
	if (wcscmp(_T("ATtinyX41"), m_ChipInfo->description) == 0)    //ATtinyX41
	{
		m_ULPOSCSEL.InsertString(m_ULPOSCSEL.GetCount(), L"011 = ULP-OSC frequency : 512KHz");
		m_ULPOSCSEL.InsertString(m_ULPOSCSEL.GetCount(), L"100 = ULP-OSC frequency : 256KHz");
		m_ULPOSCSEL.InsertString(m_ULPOSCSEL.GetCount(), L"101 = ULP-OSC frequency : 128KHz");
		m_ULPOSCSEL.InsertString(m_ULPOSCSEL.GetCount(), L"110 = ULP-OSC frequency : 64KHz");
		m_ULPOSCSEL.InsertString(m_ULPOSCSEL.GetCount(), L"111 = ULP-OSC frequency : 32KHz"); // initia value
	}
	if (wcscmp(_T("ATtiny1634"), m_ChipInfo->description) == 0)
	{
		m_ULPOSCSEL.InsertString(m_ULPOSCSEL.GetCount(), L"Reserved"); // initia value
	}
	m_BODPD.InsertString(m_BODPD.GetCount(), L"01 = BOD circuit in all sleep mode Sampled"); 
	m_BODPD.InsertString(m_BODPD.GetCount(), L"10 = BOD circuit in all sleep mode Enabled");
	m_BODPD.InsertString(m_BODPD.GetCount(), L"11 = BOD circuit in all sleep mode Disabled");//initial value
	m_BODACT.InsertString(m_BODACT.GetCount(), L"01 = BOD circuit in active mode Sampled");
	m_BODACT.InsertString(m_BODACT.GetCount(), L"10 = BOD circuit in active mode Enabled");
	m_BODACT.InsertString(m_BODACT.GetCount(), L"11 = BOD circuit in active mode Disabled");
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
	if (wcscmp(_T("ATtinyX41"), m_ChipInfo->description) == 0)    //ATtinyX41
	{
		//CLK Setting string
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0,0010 = Int.RC OSC : 8MHz, StartupTime : 6CK(14CK+16ms after PWRDWN/RST)");	// 0 = 0x02
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0,0100 = Int.ULP-RC OSC : 32KHz, StartupTime : 6CK(14CK+16ms after PWRDWN/RST)");	// 1 = 0x04
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0,0110 = Int.LF-RC OSC : 1KHz, StartupTime : 1KCK(14CK+16ms after PWRDWN/RST)");	// 2 = 0x06
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"1,0110 = Int.LF-RC OSC : 32KHz, StartupTime : 32KCK(14CK+16ms after PWRDWN/RST)");	// 3 = 0x16
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0,1000 = Ext. ceramic resinator : 0.4-0.9MHz, StartupTime : 258CK/14CK+16ms(PWRDWN/RST)");	// 4 = 0x08
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"1,1000 = Ext. ceramic resinator : 0.4-0.9MHz, StartupTime : 1KCK/14CK+16ms(PWRDWN/RST)");	// 5 = 0x18
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0,1001 = Ext. ceramic resinator : 0.4-0.9MHz, StartupTime : 16KCK/14CK+16ms(PWRDWN/RST)");	// 6 = 0x09
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0,1010 = Ext. ceramic resinator : 0.9-3.0MHz, StartupTime : 258CK/14CK+16ms(PWRDWN/RST)");	// 7 = 0x0A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"1,1010 = Ext. ceramic resinator : 0.9-3.0MHz, StartupTime : 1KCK/14CK+16ms(PWRDWN/RST)");	// 8 = 0x1A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0,1011 = Ext. ceramic resinator : 0.9-3.0MHz, StartupTime : 16KCK/14CK+16ms(PWRDWN/RST)");	// 9 = 0x0B
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0,1100 = Ext. ceramic resinator : 3.0-8.0MHz, StartupTime : 258CK/14CK+16ms(PWRDWN/RST)");	// 10 = 0x0C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"1,1100 = Ext. ceramic resinator : 3.0-8.0MHz, StartupTime : 1KCK/14CK+16ms(PWRDWN/RST)");	// 11 = 0x1C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0,1101 = Ext. ceramic resinator : 3.0-8.0MHz, StartupTime : 16KCK/14CK+16ms(PWRDWN/RST)");	// 12 = 0x0D
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0,1110 = Ext. ceramic resinator : >8.0MHz, StartupTime : 258CK/14CK+16ms(PWRDWN/RST)");	// 13 = 0x0E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"1,1110 = Ext. ceramic resinator : >8.0MHz, StartupTime : 1KCK/14CK+16ms(PWRDWN/RST)");	// 14 = 0x1E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0,1111 = Ext. ceramic resinator : >8.0MHz, StartupTime : 16KCK/14CK+16ms(PWRDWN/RST)");	// 15 = 0x0F
		
	}
	if (wcscmp(_T("ATtiny1634"), m_ChipInfo->description) == 0)
	{
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0,0000 = Ext.CLK, StartupTime : 6CK(22CK+16ms after PWRDWN/RST)");	// 0 = 0x00
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0,0010 = Int.RC OSC : 8MHz, StartupTime : 6CK(20CK+16ms after PWRDWN/RST)");	// 1 = 0x02
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0,0100 = Int.RC OSC : 32KHz, StartupTime : 6CK(22CK+16ms after PWRDWN/RST)");	// 2 = 0x04
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0,1000 = Ext. ceramic resinator : 0.4-0.9MHz, StartupTime : 258CK/274CK+16ms(PWRDWN/RST)");	// 3 = 0x08
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"1,1000 = Ext. ceramic resinator : 0.4-0.9MHz, StartupTime : 1KCK/1KCK+16ms(PWRDWN/RST)");	// 4 = 0x18
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0,1001 = Ext. ceramic resinator : 0.4-0.9MHz, StartupTime : 16KCK/16KCK+16ms(PWRDWN/RST)");	// 5 = 0x09
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0,1010 = Ext. ceramic resinator : 0.9-3.0MHz, StartupTime : 258CK/274CK+16ms(PWRDWN/RST)");	// 6 = 0x0A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"1,1010 = Ext. ceramic resinator : 0.9-3.0MHz, StartupTime : 1KCK/1KCK+16ms(PWRDWN/RST)");	// 7 = 0x1A
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0,1011 = Ext. ceramic resinator : 0.9-3.0MHz, StartupTime : 16KCK/16KCK+16ms(PWRDWN/RST)");	// 8 = 0x0B
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0,1100 = Ext. ceramic resinator : 3.0-8.0MHz, StartupTime : 258CK/274CK+16ms(PWRDWN/RST)");	// 9 = 0x0C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"1,1100 = Ext. ceramic resinator : 3.0-8.0MHz, StartupTime : 1KCK/1KCK+16ms(PWRDWN/RST)");	// 10 = 0x1C
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0,1101 = Ext. ceramic resinator : 3.0-8.0MHz, StartupTime : 16KCK/16KCK+16ms(PWRDWN/RST)");	// 11 = 0x0D
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0,1110 = Ext. ceramic resinator : >8.0MHz, StartupTime : 258CK/274CK+16ms(PWRDWN/RST)");	// 12 = 0x0E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"1,1110 = Ext. ceramic resinator : >8.0MHz, StartupTime : 1KCK/1KCK+16ms(PWRDWN/RST)");	// 13 = 0x1E
		m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0,1111 = Ext. ceramic resinator : >8.0MHz, StartupTime : 16KCK/16KCK+16ms(PWRDWN/RST)");	// 14 = 0x0F
		
	}
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	Encheck->SetCheck(0);
	//Scall
	m_ULPOSCSEL.SetDroppedWidth(300);
	m_BODPD.SetDroppedWidth(300);
	m_BODACT.SetDroppedWidth(300);
	m_WDTON.SetDroppedWidth(300);
	m_EESAVE.SetDroppedWidth(300);
	m_CKSEL.SetDroppedWidth(550);
	m_CKOUT.SetDroppedWidth(300);
	m_Lock.SetDroppedWidth(300);
	//Initial Selection
	if (wcscmp(_T("ATtinyX41"), m_ChipInfo->description) == 0)    //ATtinyX41
	{
		m_ULPOSCSEL.SetCurSel(4);
		m_CKSEL.SetCurSel(0);
	}
	if (wcscmp(_T("ATtiny1634"), m_ChipInfo->description) == 0)
	{
		m_ULPOSCSEL.SetCurSel(0);
		m_CKSEL.SetCurSel(1);
	}
	m_BODPD.SetCurSel(2);
	m_BODACT.SetCurSel(2);
	m_SELFPRGEN.SetCurSel(1);
	m_RSTDISBL.SetCurSel(1);
	m_DWEN.SetCurSel(1);
	m_SPIEN.SetCurSel(0);
	m_WDTON.SetCurSel(1);
	m_EESAVE.SetCurSel(1);
	m_BODLevel.SetCurSel(3);
	m_CKDIV8.SetCurSel(0);
	m_CKOUT.SetCurSel(1);
	m_Lock.SetCurSel(2);
	//Disable Screen
	m_ULPOSCSEL.EnableWindow(FALSE);
	m_BODPD.EnableWindow(FALSE);
	m_BODACT.EnableWindow(FALSE);
	m_SELFPRGEN.EnableWindow(FALSE);
	m_RSTDISBL.EnableWindow(FALSE);
	m_DWEN.EnableWindow(FALSE);
	m_SPIEN.EnableWindow(FALSE);
	m_WDTON.EnableWindow(FALSE);
	m_EESAVE.EnableWindow(FALSE);
	m_BODLevel.EnableWindow(FALSE);
	m_CKDIV8.EnableWindow(FALSE);
	m_CKOUT.EnableWindow(FALSE);
	m_CKSEL.EnableWindow(FALSE);
	m_Lock.EnableWindow(FALSE);

	return TRUE;
}

BOOL CDlg_ATtinyX41::OnSetActive()
{
	if (*m_pDataLen != NULL)
	{
		CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
		Encheck->SetCheck(1);
		if (wcscmp(_T("ATtinyX41"), m_ChipInfo->description) == 0)    //ATtinyX41
		{
			m_ULPOSCSEL.EnableWindow(TRUE);
		}
		if (wcscmp(_T("ATtiny1634"), m_ChipInfo->description) == 0)
		{
			m_ULPOSCSEL.EnableWindow(FALSE);
		}
		m_BODPD.EnableWindow(TRUE);
		m_BODACT.EnableWindow(TRUE);
		m_SELFPRGEN.EnableWindow(TRUE);
		m_RSTDISBL.EnableWindow(TRUE);
		m_DWEN.EnableWindow(TRUE);
		m_SPIEN.EnableWindow(FALSE);
		m_WDTON.EnableWindow(TRUE);
		m_EESAVE.EnableWindow(TRUE);
		m_BODLevel.EnableWindow(TRUE);
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
		if (wcscmp(_T("ATtinyX41"), m_ChipInfo->description) == 0)    //ATtinyX41
		{
			//ULPOSCSEL
			if ((ExtendedFuse & 0xE0) == 0xE0)
				m_ULPOSCSEL.SetCurSel(4);
			else if ((ExtendedFuse & 0xE0) == 0xC0)
				m_ULPOSCSEL.SetCurSel(3);
			else if ((ExtendedFuse & 0xE0) == 0xA0)
				m_ULPOSCSEL.SetCurSel(2);
			else if ((ExtendedFuse & 0xE0) == 0x80)
				m_ULPOSCSEL.SetCurSel(1);
			else if ((ExtendedFuse & 0xE0) == 0x60)
				m_ULPOSCSEL.SetCurSel(0);
		}
		else if (wcscmp(_T("ATtiny1634"), m_ChipInfo->description) == 0)
		{
			m_ULPOSCSEL.SetCurSel(0);
		}
		//BODPD
		if ((ExtendedFuse & 0x18) == 0x18)
			m_BODPD.SetCurSel(2);
		else if((ExtendedFuse & 0x18) == 0x10)
			m_BODPD.SetCurSel(1);
		else if ((ExtendedFuse & 0x18) == 0x08)
			m_BODPD.SetCurSel(0);
		//BODACT
		if ((ExtendedFuse & 0x06) == 0x06)
			m_BODACT.SetCurSel(2);
		else if ((ExtendedFuse & 0x06) == 0x04)
			m_BODACT.SetCurSel(1);
		else if ((ExtendedFuse & 0x06) == 0x02)
			m_BODACT.SetCurSel(0);
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
		//BODLevel
		if ((HighFuse & 0x07) == 0x07)
			m_BODLevel.SetCurSel(3);
		else if ((HighFuse & 0x07) == 0x06)
			m_BODLevel.SetCurSel(2);
		else if ((HighFuse & 0x07) == 0x05)
			m_BODLevel.SetCurSel(1);
		else if ((HighFuse & 0x07) == 0x04)
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
		//CLK Setting
		if (wcscmp(_T("ATtinyX41"), m_ChipInfo->description) == 0)    //ATtinyX41
		{
			switch (LowFuse & 0x1F)
			{
			case 0x02:
				m_CKSEL.SetCurSel(0);
				break;
			case 0x04:
				m_CKSEL.SetCurSel(1);
				break;
			case 0x06:
				m_CKSEL.SetCurSel(2);
				break;
			case 0x16:
				m_CKSEL.SetCurSel(3);
				break;
			case 0x08:
				m_CKSEL.SetCurSel(4);
				break;
			case 0x18:
				m_CKSEL.SetCurSel(5);
				break;
			case 0x09:
				m_CKSEL.SetCurSel(6);
				break;
			case 0x0A:
				m_CKSEL.SetCurSel(7);
				break;
			case 0x1A:
				m_CKSEL.SetCurSel(8);
				break;
			case 0x0B:
				m_CKSEL.SetCurSel(9);
				break;
			case 0x0C:
				m_CKSEL.SetCurSel(10);
				break;
			case 0x1C:
				m_CKSEL.SetCurSel(11);
				break;
			case 0x0D:
				m_CKSEL.SetCurSel(12);
				break;
			case 0x0E:
				m_CKSEL.SetCurSel(13);
				break;
			case 0x1E:
				m_CKSEL.SetCurSel(14);
				break;
			case 0x0F:
				m_CKSEL.SetCurSel(15);
				break;
			}
		}
		if (wcscmp(_T("ATtiny1634"), m_ChipInfo->description) == 0)
		{
			switch (LowFuse & 0x1F)
			{
			case 0x00:
				m_CKSEL.SetCurSel(0);
				break;
			case 0x02:
				m_CKSEL.SetCurSel(1);
				break;
			case 0x04:
				m_CKSEL.SetCurSel(2);
				break;
			case 0x08:
				m_CKSEL.SetCurSel(3);
				break;
			case 0x18:
				m_CKSEL.SetCurSel(4);
				break;
			case 0x09:
				m_CKSEL.SetCurSel(5);
				break;
			case 0x0A:
				m_CKSEL.SetCurSel(6);
				break;
			case 0x1A:
				m_CKSEL.SetCurSel(7);
				break;
			case 0x0B:
				m_CKSEL.SetCurSel(8);
				break;
			case 0x0C:
				m_CKSEL.SetCurSel(9);
				break;
			case 0x1C:
				m_CKSEL.SetCurSel(10);
				break;
			case 0x0D:
				m_CKSEL.SetCurSel(11);
				break;
			case 0x0E:
				m_CKSEL.SetCurSel(12);
				break;
			case 0x1E:
				m_CKSEL.SetCurSel(13);
				break;
			case 0x0F:
				m_CKSEL.SetCurSel(14);
				break;
			}
		}
		//Lock
		if ((Lockbits & 0x03) == 0x03)
			m_Lock.SetCurSel(2);
		else if ((Lockbits & 0x03) == 0x02)
			m_Lock.SetCurSel(1);
		else if ((Lockbits & 0x03) == 0x00)
			m_Lock.SetCurSel(0);
	}
	else if (*m_pDataLen == NULL)
	{
		CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
		Encheck->SetCheck(0);
		m_ULPOSCSEL.EnableWindow(FALSE);
		m_BODPD.EnableWindow(FALSE);
		m_BODACT.EnableWindow(FALSE);
		m_SELFPRGEN.EnableWindow(FALSE);
		m_RSTDISBL.EnableWindow(FALSE);
		m_DWEN.EnableWindow(FALSE);
		m_SPIEN.EnableWindow(FALSE);
		m_WDTON.EnableWindow(FALSE);
		m_EESAVE.EnableWindow(FALSE);
		m_BODLevel.EnableWindow(FALSE);
		m_CKDIV8.EnableWindow(FALSE);
		m_CKOUT.EnableWindow(FALSE);
		m_CKSEL.EnableWindow(FALSE);
		m_Lock.EnableWindow(FALSE);
	}
	return CMFCPropertyPage::OnSetActive();
}
void CDlg_ATtinyX41::OnCbnSelchangeCombo()
{
	CString ExFuse_str, HFuse_str, LFuse_str, Lockbits_str;
	ExtendedFuse = 0xFF;
	HighFuse = 0xDF;
	LowFuse = 0x62;
	Lockbits = 0xFF;
	if (wcscmp(_T("ATtinyX41"), m_ChipInfo->description) == 0)    //ATtinyX41
	{
		//ULPOSCSEL
		ExtendedFuse &= 0x1F;
		if (m_ULPOSCSEL.GetCurSel() == 4)
			ExtendedFuse |= 0xE0;
		else if (m_ULPOSCSEL.GetCurSel() == 3)
			ExtendedFuse |= 0xC0;
		else if (m_ULPOSCSEL.GetCurSel() == 2)
			ExtendedFuse |= 0xA0;
		else if (m_ULPOSCSEL.GetCurSel() == 1)
			ExtendedFuse |= 0x80;
		else if (m_ULPOSCSEL.GetCurSel() == 0)
			ExtendedFuse |= 0x60;
		ExFuse_str.Format(_T("%02X"), ExtendedFuse);
		m_ExFuse.SetWindowTextW(ExFuse_str);
	}
	//BODPD
	ExtendedFuse &= 0xE7;
	if (m_BODPD.GetCurSel() == 2)
		ExtendedFuse |= 0x18;
	else if (m_BODPD.GetCurSel() == 1)
		ExtendedFuse |= 0x10;
	else if (m_BODPD.GetCurSel() == 0)
		ExtendedFuse |= 0x08;
	ExFuse_str.Format(_T("%02X"), ExtendedFuse);
	m_ExFuse.SetWindowTextW(ExFuse_str);
	//BODACT
	ExtendedFuse &= 0xF9;
	if (m_BODACT.GetCurSel() == 2)
		ExtendedFuse |= 0x06;
	else if (m_BODACT.GetCurSel() == 1)
		ExtendedFuse |= 0x04;
	else if (m_BODACT.GetCurSel() == 0)
		ExtendedFuse |= 0x02;
	ExFuse_str.Format(_T("%02X"), ExtendedFuse);
	m_ExFuse.SetWindowTextW(ExFuse_str);
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
	if (m_BODLevel.GetCurSel() == 3)
		HighFuse |= 0x07;
	else if (m_BODLevel.GetCurSel() == 2)
		HighFuse |= 0x06;
	else if (m_BODLevel.GetCurSel() == 1)
		HighFuse |= 0x05;
	else if (m_BODLevel.GetCurSel() == 0)
		HighFuse |= 0x04;
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
	//CLK Setting
	if (wcscmp(_T("ATtinyX41"), m_ChipInfo->description) == 0)    //ATtinyX41
	{
		LowFuse &= 0xE0;
		switch (m_CKSEL.GetCurSel())
		{
		case 0:
			LowFuse |= 0x02;
			break;
		case 1:
			LowFuse |= 0x04;
			break;
		case 2:
			LowFuse |= 0x06;
			break;
		case 3:
			LowFuse |= 0x16;
			break;
		case 4:
			LowFuse |= 0x08;
			break;
		case 5:
			LowFuse |= 0x18;
			break;
		case 6:
			LowFuse |= 0x09;
			break;
		case 7:
			LowFuse |= 0x0A;
			break;
		case 8:
			LowFuse |= 0x1A;
			break;
		case 9:
			LowFuse |= 0x0B;
			break;
		case 10:
			LowFuse |= 0x0C;
			break;
		case 11:
			LowFuse |= 0x1C;
			break;
		case 12:
			LowFuse |= 0x0D;
			break;
		case 13:
			LowFuse |= 0x0E;
			break;
		case 14:
			LowFuse |= 0x1E;
			break;
		case 15:
			LowFuse |= 0x0F;
			break;
		}
		LFuse_str.Format(_T("%02X"), LowFuse);
		m_LFuse.SetWindowTextW(LFuse_str);
	}
	if (wcscmp(_T("ATtiny1634"), m_ChipInfo->description) == 0)    //ATtinyX41
	{
		LowFuse &= 0xE0;
		switch (m_CKSEL.GetCurSel())
		{
		case 0:
			LowFuse |= 0x00;
			break;
		case 1:
			LowFuse |= 0x02;
			break;
		case 2:
			LowFuse |= 0x04;
			break;
		case 3:
			LowFuse |= 0x08;
			break;
		case 4:
			LowFuse |= 0x18;
			break;
		case 5:
			LowFuse |= 0x09;
			break;
		case 6:
			LowFuse |= 0x0A;
			break;
		case 7:
			LowFuse |= 0x1A;
			break;
		case 8:
			LowFuse |= 0x0B;
			break;
		case 9:
			LowFuse |= 0x0C;
			break;
		case 10:
			LowFuse |= 0x1C;
			break;
		case 11:
			LowFuse |= 0x0D;
			break;
		case 12:
			LowFuse |= 0x0E;
			break;
		case 13:
			LowFuse |= 0x1E;
			break;
		case 14:
			LowFuse |= 0x0F;
			break;
		}
		LFuse_str.Format(_T("%02X"), LowFuse);
		m_LFuse.SetWindowTextW(LFuse_str);
	}
	
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

}

void CDlg_ATtinyX41::OnOK()
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

LRESULT CDlg_ATtinyX41::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();

}
LRESULT CDlg_ATtinyX41::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}

void CDlg_ATtinyX41::OnBnClickedEnable()
{
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	int state = Encheck->GetCheck();
	if (state == 1)
	{
		if (wcscmp(_T("ATtinyX41"), m_ChipInfo->description) == 0)    //ATtinyX41
		{
			m_ULPOSCSEL.EnableWindow(TRUE);
		}
		if (wcscmp(_T("ATtiny1634"), m_ChipInfo->description) == 0)
		{
			m_ULPOSCSEL.EnableWindow(FALSE);
		}
		m_BODPD.EnableWindow(TRUE);
		m_BODACT.EnableWindow(TRUE);
		m_SELFPRGEN.EnableWindow(TRUE);
		m_RSTDISBL.EnableWindow(TRUE);
		m_DWEN.EnableWindow(TRUE);
		m_SPIEN.EnableWindow(FALSE);
		m_WDTON.EnableWindow(TRUE);
		m_EESAVE.EnableWindow(TRUE);
		m_BODLevel.EnableWindow(TRUE);
		m_CKDIV8.EnableWindow(TRUE);
		m_CKOUT.EnableWindow(TRUE);
		m_CKSEL.EnableWindow(TRUE);
		m_Lock.EnableWindow(TRUE);
	}
	else if (state == 0)
	{
		m_ULPOSCSEL.EnableWindow(FALSE);
		m_BODPD.EnableWindow(FALSE);
		m_BODACT.EnableWindow(FALSE);
		m_SELFPRGEN.EnableWindow(FALSE);
		m_RSTDISBL.EnableWindow(FALSE);
		m_DWEN.EnableWindow(FALSE);
		m_SPIEN.EnableWindow(FALSE);
		m_WDTON.EnableWindow(FALSE);
		m_EESAVE.EnableWindow(FALSE);
		m_BODLevel.EnableWindow(FALSE);
		m_CKDIV8.EnableWindow(FALSE);
		m_CKOUT.EnableWindow(FALSE);
		m_CKSEL.EnableWindow(FALSE);
		m_Lock.EnableWindow(FALSE);
	}

}