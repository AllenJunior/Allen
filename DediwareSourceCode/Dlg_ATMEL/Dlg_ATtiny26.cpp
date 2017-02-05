// Dlg_ATtiny26.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_ATtiny26.h"
#include "afxdialogex.h"
#include "numeric_conversion.h"
#include "../common/prog_info.h"

// Dlg_ATtiny26 dialog

IMPLEMENT_DYNAMIC(CDlg_ATtiny26, CMFCPropertyPage)

CDlg_ATtiny26::CDlg_ATtiny26(struct chip_info_c *ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_ATtiny26::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_ATtiny26::~CDlg_ATtiny26()
{
}

void CDlg_ATtiny26::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, option_enable);
	DDX_Control(pDX, IDC_COMBO1, m_RSTDISBL);
	DDX_Control(pDX, IDC_COMBO2, m_SPIEN);
	DDX_Control(pDX, IDC_COMBO3, m_EESAVE);
	DDX_Control(pDX, IDC_COMBO4, m_BODLevel);
	DDX_Control(pDX, IDC_COMBO5, m_BODEN);
	DDX_Control(pDX, IDC_COMBO6, m_CKOPT);
	DDX_Control(pDX, IDC_COMBO7, m_CKSEL);
	DDX_Control(pDX, IDC_COMBO8, m_Lock);
	DDX_Control(pDX, IDC_EDIT1, m_HFuse);
	DDX_Control(pDX, IDC_EDIT2, m_LFuse);
	DDX_Control(pDX, IDC_EDIT3, m_Lockbits);
}


BEGIN_MESSAGE_MAP(CDlg_ATtiny26, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_ATtiny26::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_ATtiny26::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_ATtiny26::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_ATtiny26::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_ATtiny26::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDlg_ATtiny26::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDlg_ATtiny26::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CDlg_ATtiny26::OnCbnSelchangeCombo)
	ON_BN_CLICKED(IDC_CHECK1, &CDlg_ATtiny26::OnBnClickedEnable)
END_MESSAGE_MAP()


// Dlg_ATtiny26 message handlers
BOOL CDlg_ATtiny26::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	HighFuse = 0xFF;
	LowFuse = 0x6A;
	Lockbits = 0xFF;
	m_HFuse.SetWindowTextW(L"F7");
	m_LFuse.SetWindowTextW(L"E1");
	m_Lockbits.SetWindowTextW(L"FF");

	m_RSTDISBL.InsertString(m_RSTDISBL.GetCount(), L"0 = Enabled external reset");
	m_RSTDISBL.InsertString(m_RSTDISBL.GetCount(), L"1 = Disabled external reset");
	m_SPIEN.InsertString(m_SPIEN.GetCount(), L"0 = Enabled ISP Programming");
	m_SPIEN.InsertString(m_SPIEN.GetCount(), L"1 = Disabled ISP Programming");
	m_EESAVE.InsertString(m_EESAVE.GetCount(), L"0 = Preserve EEPROM during Chip Erase");
	m_EESAVE.InsertString(m_EESAVE.GetCount(), L"1 = Unreserves EEPROM during Chip Erase");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"0 = Trigger Level at 4.3V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"1 = Trigger Level at 2.7V");
	m_BODEN.InsertString(m_BODEN.GetCount(), L"0 = Brownout detector enable");
	m_BODEN.InsertString(m_BODEN.GetCount(), L"1 = Brownout detector disable");
	m_CKOPT.InsertString(m_CKOPT.GetCount(), L"0 = Oscillator options enable");
	m_CKOPT.InsertString(m_CKOPT.GetCount(), L"1 = Oscillator options disable");
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x01 = PLLclk.High.freq, start-up time : 1KCK"); //0
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x11 = PLLclk.High.freq, start-up time : 1KCK+4.1ms"); //1
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x21 = PLLclk.High.freq, start-up time : 1KCK+64ms"); //2
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x31 = PLLclk.High.freq, start-up time : 16KCK"); //3
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x80 = Ext.CLK, start-up time : 6CK"); //4
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x90 = Ext.CLK, start-up time : 6CK+4.1ms"); //5
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xA0 = Ext.CLK, start-up time : 6CK+65ms"); //6
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x81 = Int.RC OSC : 1MHz, start-up time : 6CK"); //7
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x91 = Int.RC OSC : 1MHz, start-up time : 6CK+4.1ms"); //8
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xA1 = Int.RC OSC : 1MHz, start-up time : 6CK+65ms"); //9   Initial value
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x82 = Int.RC OSC : 2MHz, start-up time : 6CK"); //10
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x92 = Int.RC OSC : 2MHz, start-up time : 6CK+4.1ms"); //11
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xA2 = Int.RC OSC : 2MHz, start-up time : 6CK+65ms"); //12
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x83 = Int.RC OSC : 4MHz, start-up time : 6CK"); //13
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x93 = Int.RC OSC : 4MHz, start-up time : 6CK+4.1ms"); //14
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xA3 = Int.RC OSC : 4MHz, start-up time : 6CK+65ms"); //15
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x84 = Int.RC OSC : 8MHz, start-up time : 6CK"); //16
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x94 = Int.RC OSC : 8MHz, start-up time : 6CK+4.1ms"); //17
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xA4 = Int.RC OSC : 8MHz, start-up time : 6CK+65ms"); //18
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x85 = Ext.RC OSC : -0.9MHz, start-up time : 18CK"); //19
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x95 = Ext.RC OSC : -0.9MHz, start-up time : 18CK+4.1ms"); //20
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xA5 = Ext.RC OSC : -0.9MHz, start-up time : 18CK+65ms"); //21
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xB5 = Ext.RC OSC : -0.9MHz, start-up time : 6CK+4.1ms"); //22
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x86 = Ext.RC OSC : 0.9-3.0MHz, start-up time : 18CK");  //23
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x96 = Ext.RC OSC : 0.9-3.0MHz, start-up time : 18CK+4.1ms"); //24
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xA6 = Ext.RC OSC : 0.9-3.0MHz, start-up time : 18CK+65ms"); //25
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xB6 = Ext.RC OSC : 0.9-3.0MHz, start-up time : 6CK+4.1ms"); //26
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x87 = Ext.RC OSC : 3.0-8.0MHz, start-up time : 18CK"); //27
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x97 = Ext.RC OSC : 3.0-8.0MHz, start-up time : 18CK+4.1ms"); //28
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xA7 = Ext.RC OSC : 3.0-8.0MHz, start-up time : 18CK+65ms"); //29
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xB7 = Ext.RC OSC : 3.0-8.0MHz, start-up time : 6CK+4.1ms"); //30
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x88 = Ext.RC OSC : 8.0-12.0MHz, start-up time : 18CK"); //31
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x98 = Ext.RC OSC : 8.0-12.0MHz, start-up time : 18CK+4.1ms"); //32
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xA8 = Ext.RC OSC : 8.0-12.0MHz, start-up time : 18CK+65ms"); //33
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xB8 = Ext.RC OSC : 8.0-12.0MHz, start-up time : 6CK+4.1ms"); //34
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x89 = Ext.lowfreq-OSC, start-up time : 1KCK+4.1ms"); //35
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x99 = Ext.lowfreq-OSC, start-up time : 1KCK+65ms"); //36
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xA9 = Ext.lowfreq-OSC, start-up time : 32KCK+65ms"); //37
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x8A = Ext.Crystal OSC : 0.4-0.9MHz, start-up time : 258CK+4.1ms"); //38
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x9A = Ext.Crystal OSC : 0.4-0.9MHz, start-up time : 258CK+65ms"); //39
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xAA = Ext.Crystal OSC : 0.4-0.9MHz, start-up time : 1KCK"); //40
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xBA = Ext.Crystal OSC : 0.4-0.9MHz, start-up time : 1KCK+4.1ms"); //41
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x8B = Ext.Crystal OSC : 0.4-0.9MHz, start-up time : 1KCK+65ms"); //42
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x9B = Ext.Crystal OSC : 0.4-0.9MHz, start-up time : 16KCK"); //43
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xAB = Ext.Crystal OSC : 0.4-0.9MHz, start-up time : 16KCK+4.1ms"); //44
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xBB = Ext.Crystal OSC : 0.4-0.9MHz, start-up time : 16KCK+65ms");  //45
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x8C = Ext.Crystal OSC : 0.9-3.0MHz, start-up time : 258CK+4.1ms"); //46
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x9C = Ext.Crystal OSC : 0.9-3.0MHz, start-up time : 258CK+65ms"); //47
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xAC = Ext.Crystal OSC : 0.9-3.0MHz, start-up time : 1KCK");  //48
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xBC = Ext.Crystal OSC : 0.9-3.0MHz, start-up time : 1KCK+4.1ms"); //49
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x8D = Ext.Crystal OSC : 0.9-3.0MHz, start-up time : 1KCK+65ms");  //50
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x9D = Ext.Crystal OSC : 0.9-3.0MHz, start-up time : 16KCK"); //51
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xAD = Ext.Crystal OSC : 0.9-3.0MHz, start-up time : 16KCK+4.1ms"); //52
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xBD = Ext.Crystal OSC : 0.9-3.0MHz, start-up time : 16KCK+65ms");  //53
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x8E = Ext.Crystal OSC : 3.0-16MHz, start-up time : 258CK+4.1ms");  //54
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x9E = Ext.Crystal OSC : 3.0-16MHz, start-up time : 258CK+65ms");  //55
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xAE = Ext.Crystal OSC : 3.0-16MHz, start-up time : 1KCK");  //56
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xBE = Ext.Crystal OSC : 3.0-16MHz, start-up time : 1KCK+4.1ms");  //57
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x8F = Ext.Crystal OSC : 3.0-16MHz, start-up time : 1KCK+65ms");   //58
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0x9F = Ext.Crystal OSC : 3.0-16MHz, start-up time : 16KCK"); //59
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xAF = Ext.Crystal OSC : 3.0-16MHz, start-up time : 16KCK+4.1ms"); //60
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0xBF = Ext.Crystal OSC : 3.0-16MHz, start-up time : 16KCK+65ms");  // 61
	m_Lock.InsertString(m_Lock.GetCount(), L"00 = Further Programming and verification Disabled");
	m_Lock.InsertString(m_Lock.GetCount(), L"10 = Further Programming Disabled");
	m_Lock.InsertString(m_Lock.GetCount(), L"11 = No memory Lock features enabled");

	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	Encheck->SetCheck(0);
	m_CKSEL.SetDroppedWidth(500);

	m_RSTDISBL.SetCurSel(1);
	m_SPIEN.SetCurSel(0);
	m_EESAVE.SetCurSel(1);
	m_BODLevel.SetCurSel(1);
	m_BODEN.SetCurSel(1);
	m_CKOPT.SetCurSel(1);
	m_CKSEL.SetCurSel(9);
	m_Lock.SetCurSel(2);

	m_RSTDISBL.EnableWindow(FALSE);
	m_SPIEN.EnableWindow(FALSE);
	m_EESAVE.EnableWindow(FALSE);
	m_BODLevel.EnableWindow(FALSE);
	m_BODEN.EnableWindow(FALSE);
	m_CKOPT.EnableWindow(FALSE);
	m_CKSEL.EnableWindow(FALSE);
	m_Lock.EnableWindow(FALSE);

	return TRUE;
}

BOOL CDlg_ATtiny26::OnSetActive()
{
	if (*m_pDataLen != NULL)
	{
		CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
		Encheck->SetCheck(1);
		m_RSTDISBL.EnableWindow(TRUE);
		m_SPIEN.EnableWindow(FALSE);
		m_EESAVE.EnableWindow(TRUE);
		m_BODLevel.EnableWindow(FALSE);
		m_BODEN.EnableWindow(TRUE);
		m_CKOPT.EnableWindow(TRUE);
		m_CKSEL.EnableWindow(TRUE);
		m_Lock.EnableWindow(TRUE);

		CString ExF_str, HF_str, LF_str, L_str;
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

		//RSTDISBL
		if ((HighFuse & 0x10) == 0x10)
			m_RSTDISBL.SetCurSel(1);
		else if ((HighFuse & 0x10) == 0x00)
			m_RSTDISBL.SetCurSel(0);
		//SPIEN
		if ((HighFuse & 0x08) == 0x08)
			m_SPIEN.SetCurSel(1);
		else if ((HighFuse & 0x08) == 0x00)
			m_SPIEN.SetCurSel(0);
		//EESAVE
		if ((HighFuse & 0x04) == 0x04)
			m_EESAVE.SetCurSel(1);
		else if ((HighFuse & 0x04) == 0x00)
			m_EESAVE.SetCurSel(0);
		//BODLevel
		if ((HighFuse & 0x02) == 0x02)
			m_BODLevel.SetCurSel(1);
		else if ((HighFuse & 0x02) == 0x00)
			m_BODLevel.SetCurSel(0);
		//BODEN
		if ((HighFuse & 0x01) == 0x01)
			m_BODEN.SetCurSel(1);
		else if ((HighFuse & 0x01) == 0x00)
			m_BODEN.SetCurSel(0);
		//CKOPT
		if ((LowFuse & 0x40) == 0x40)
			m_CKOPT.SetCurSel(1);
		else if ((LowFuse & 0x40) == 0x00)
			m_CKOPT.SetCurSel(0);
		//CLK Setting
		switch (LowFuse & 0xBF)
		{
		case 0x01:
			m_CKSEL.SetCurSel(0);
			break;
		case 0x11:
			m_CKSEL.SetCurSel(1);
			break;
		case 0x21:
			m_CKSEL.SetCurSel(2);
			break;
		case 0x31:
			m_CKSEL.SetCurSel(3);
			break;
		case 0x80:
			m_CKSEL.SetCurSel(4);
			break;
		case 0x90:
			m_CKSEL.SetCurSel(5);
			break;
		case 0xA0:
			m_CKSEL.SetCurSel(6);
			break;
		case 0x81:
			m_CKSEL.SetCurSel(7);
			break;
		case 0x91:
			m_CKSEL.SetCurSel(8);
			break;
		case 0xA1:
			m_CKSEL.SetCurSel(9);
			break;
		case 0x82:
			m_CKSEL.SetCurSel(10);
			break;
		case 0x92:
			m_CKSEL.SetCurSel(11);
			break;
		case 0xA2:
			m_CKSEL.SetCurSel(12);
			break;
		case 0x83:
			m_CKSEL.SetCurSel(13);
			break;
		case 0x93:
			m_CKSEL.SetCurSel(14);
			break;
		case 0xA3:
			m_CKSEL.SetCurSel(15);
			break;
		case 0x84:
			m_CKSEL.SetCurSel(16);
			break;
		case 0x94:
			m_CKSEL.SetCurSel(17);
			break;
		case 0xA4:
			m_CKSEL.SetCurSel(18);
			break;
		case 0x85:
			m_CKSEL.SetCurSel(19);
			break;
		case 0x95:
			m_CKSEL.SetCurSel(20);
			break;
		case 0xA5:
			m_CKSEL.SetCurSel(21);
			break;
		case 0xB5:
			m_CKSEL.SetCurSel(22);
			break;
		case 0x86:
			m_CKSEL.SetCurSel(23);
			break;
		case 0x96:
			m_CKSEL.SetCurSel(24);
			break;
		case 0xA6:
			m_CKSEL.SetCurSel(25);
			break;
		case 0xB6:
			m_CKSEL.SetCurSel(26);
			break;
		case 0x87:
			m_CKSEL.SetCurSel(27);
			break;
		case 0x97:
			m_CKSEL.SetCurSel(28);
			break;
		case 0xA7:
			m_CKSEL.SetCurSel(29);
			break;
		case 0xB7:
			m_CKSEL.SetCurSel(30);
			break;
		case 0x88:
			m_CKSEL.SetCurSel(31);
			break;
		case 0x98:
			m_CKSEL.SetCurSel(32);
			break;
		case 0xA8:
			m_CKSEL.SetCurSel(33);
			break;
		case 0xB8:
			m_CKSEL.SetCurSel(34);
			break;
		case 0x89:
			m_CKSEL.SetCurSel(35);
			break;
		case 0x99:
			m_CKSEL.SetCurSel(36);
			break;
		case 0xA9:
			m_CKSEL.SetCurSel(37);
			break;
		case 0x8A:
			m_CKSEL.SetCurSel(38);
			break;
		case 0x9A:
			m_CKSEL.SetCurSel(39);
			break;
		case 0xAA:
			m_CKSEL.SetCurSel(40);
			break;
		case 0xBA:
			m_CKSEL.SetCurSel(41);
			break;
		case 0x8B:
			m_CKSEL.SetCurSel(42);
			break;
		case 0x9B:
			m_CKSEL.SetCurSel(43);
			break;
		case 0xAB:
			m_CKSEL.SetCurSel(44);
			break;
		case 0xBB:
			m_CKSEL.SetCurSel(45);
			break;
		case 0x8C:
			m_CKSEL.SetCurSel(46);
			break;
		case 0x9C:
			m_CKSEL.SetCurSel(47);
			break;
		case 0xAC:
			m_CKSEL.SetCurSel(48);
			break;
		case 0xBC:
			m_CKSEL.SetCurSel(49);
			break;
		case 0x8D:
			m_CKSEL.SetCurSel(50);
			break;
		case 0x9D:
			m_CKSEL.SetCurSel(51);
			break;
		case 0xAD:
			m_CKSEL.SetCurSel(52);
			break;
		case 0xBD:
			m_CKSEL.SetCurSel(53);
			break;
		case 0x8E:
			m_CKSEL.SetCurSel(54);
			break;
		case 0x9E:
			m_CKSEL.SetCurSel(55);
			break;
		case 0xAE:
			m_CKSEL.SetCurSel(56);
			break;
		case 0xBE:
			m_CKSEL.SetCurSel(57);
			break;
		case 0x8F:
			m_CKSEL.SetCurSel(58);
			break;
		case 0x9F:
			m_CKSEL.SetCurSel(59);
			break;
		case 0xAF:
			m_CKSEL.SetCurSel(60);
			break;
		case 0xBF:
			m_CKSEL.SetCurSel(61);
			break;
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
		m_RSTDISBL.EnableWindow(FALSE);
		m_SPIEN.EnableWindow(FALSE);
		m_EESAVE.EnableWindow(FALSE);
		m_BODLevel.EnableWindow(FALSE);
		m_BODEN.EnableWindow(FALSE);
		m_CKOPT.EnableWindow(FALSE);
		m_CKSEL.EnableWindow(FALSE);
		m_Lock.EnableWindow(FALSE);
	}
	return CMFCPropertyPage::OnSetActive();
}

void CDlg_ATtiny26::OnCbnSelchangeCombo()
{
	CString HFuse_str, LFuse_str, Lockbits_str;
	HighFuse = 0xF7;
	LowFuse = 0xE1;
	Lockbits = 0xFF;
	//RSTDISBL
	HighFuse &= 0xEF;
	if (m_RSTDISBL.GetCurSel() == 1)
		HighFuse |= 0x10;
	else if (m_RSTDISBL.GetCurSel() == 0)
		HighFuse |= 0x00;
	HFuse_str.Format(_T("%02X"), HighFuse);
	m_HFuse.SetWindowTextW(HFuse_str);
	//SPIEN
	HighFuse &= 0xF7;
	if (m_SPIEN.GetCurSel() == 1)
		HighFuse |= 0x08;
	else if (m_SPIEN.GetCurSel() == 0)
		HighFuse |= 0x00;
	HFuse_str.Format(_T("%02X"), HighFuse);
	m_HFuse.SetWindowTextW(HFuse_str);
	//EESAVE
	HighFuse &= 0xFB;
	if (m_EESAVE.GetCurSel() == 1)
		HighFuse |= 0x04;
	else if (m_EESAVE.GetCurSel() == 0)
		HighFuse |= 0x00;
	HFuse_str.Format(_T("%02X"), HighFuse);
	m_HFuse.SetWindowTextW(HFuse_str);
	//BODLevel
	HighFuse &= 0xFD;
	if (m_BODLevel.GetCurSel() == 1)
		HighFuse |= 0x02;
	else if (m_BODLevel.GetCurSel() == 0)
		HighFuse |= 0x00;
	HFuse_str.Format(_T("%02X"), HighFuse);
	m_HFuse.SetWindowTextW(HFuse_str);
	//BODEN
	HighFuse &= 0xFE;
	if (m_BODEN.GetCurSel() == 1)
		HighFuse |= 0x01;
	else if (m_BODEN.GetCurSel() == 0)
		HighFuse |= 0x00;
	HFuse_str.Format(_T("%02X"), HighFuse);
	m_HFuse.SetWindowTextW(HFuse_str);
	//CKOPT
	LowFuse &= 0xBF;
	if (m_CKOPT.GetCurSel() == 1)
		LowFuse |= 0x40;
	else if (m_CKOPT.GetCurSel() == 0)
		LowFuse |= 0x00;
	LFuse_str.Format(_T("%02X"), LowFuse);
	m_LFuse.SetWindowTextW(LFuse_str);
	//CLK Setting
	LowFuse &= 0x40;
	switch (m_CKSEL.GetCurSel())
	{
	case 0:
		LowFuse |= 0x01;
		break;
	case 1:
		LowFuse |= 0x11;
		break;
	case 2:
		LowFuse |= 0x21;
		break;
	case 3:
		LowFuse |= 0x31;
		break;
	case 4:
		LowFuse |= 0x80;
		break;
	case 5:
		LowFuse |= 0x90;
		break;
	case 6:
		LowFuse |= 0xA0;
		break;
	case 7:
		LowFuse |= 0x81;
		break;
	case 8:
		LowFuse |= 0x91;
		break;
	case 9:
		LowFuse |= 0xA1;
		break;
	case 10:
		LowFuse |= 0x82;
		break;
	case 11:
		LowFuse |= 0x92;
		break;
	case 12:
		LowFuse |= 0xA2;
		break;
	case 13:
		LowFuse |= 0x83;
		break;
	case 14:
		LowFuse |= 0x93;
		break;
	case 15:
		LowFuse |= 0xA3;
		break;
	case 16:
		LowFuse |= 0x84;
		break;
	case 17:
		LowFuse |= 0x94;
		break;
	case 18:
		LowFuse |= 0xA4;
		break;
	case 19:
		LowFuse |= 0x85;
		break;
	case 20:
		LowFuse |= 0x95;
		break;
	case 21:
		LowFuse |= 0xA5;
		break;
	case 22:
		LowFuse |= 0xB5;
		break;
	case 23:
		LowFuse |= 0x86;
		break;
	case 24:
		LowFuse |= 0x96;
		break;
	case 25:
		LowFuse |= 0xA6;
		break;
	case 26:
		LowFuse |= 0xB6;
		break;
	case 27:
		LowFuse |= 0x87;
		break;
	case 28:
		LowFuse |= 0x97;
		break;
	case 29:
		LowFuse |= 0xA7;
		break;
	case 30:
		LowFuse |= 0xB7;
		break;
	case 31:
		LowFuse |= 0x88;
		break;
	case 32:
		LowFuse |= 0x98;
		break;
	case 33:
		LowFuse |= 0xA8;
		break;
	case 34:
		LowFuse |= 0xB8;
		break;
	case 35:
		LowFuse |= 0x89;
		break;
	case 36:
		LowFuse |= 0x99;
		break;
	case 37:
		LowFuse |= 0xA9;
		break;
	case 38:
		LowFuse |= 0x8A;
		break;
	case 39:
		LowFuse |= 0x9A;
		break;
	case 40:
		LowFuse |= 0xAA;
		break;
	case 41:
		LowFuse |= 0xBA;
		break;
	case 42:
		LowFuse |= 0x8B;
		break;
	case 43:
		LowFuse |= 0x9B;
		break;
	case 44:
		LowFuse |= 0xAB;
		break;
	case 45:
		LowFuse |= 0xBB;
		break;
	case 46:
		LowFuse |= 0x8C;
		break;
	case 47:
		LowFuse |= 0x9C;
		break;
	case 48:
		LowFuse |= 0xAC;
		break;
	case 49:
		LowFuse |= 0xBC;
		break;
	case 50:
		LowFuse |= 0x8D;
		break;
	case 51:
		LowFuse |= 0x9D;
		break;
	case 52:
		LowFuse |= 0xAD;
		break;
	case 53:
		LowFuse |= 0xBD;
		break;
	case 54:
		LowFuse |= 0x8E;
		break;
	case 55:
		LowFuse |= 0x9E;
		break;
	case 56:
		LowFuse |= 0xAE;
		break;
	case 57:
		LowFuse |= 0xBE;
		break;
	case 58:
		LowFuse |= 0x8F;
		break;
	case 59:
		LowFuse |= 0x9F;
		break;
	case 60:
		LowFuse |= 0xAF;
		break;
	case 61:
		LowFuse |= 0xBF;
		break;
	}
	LFuse_str.Format(_T("%02X"), LowFuse);
	m_LFuse.SetWindowTextW(LFuse_str);
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

void CDlg_ATtiny26::OnOK()
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

		fixint = 0x00;
		memcpy(m_RegBuff + sizeof(HighFuse)+sizeof(LowFuse)+sizeof(Lockbits), &Lockbits, sizeof(fixint));

		*m_pDataLen = sizeof(HighFuse)+sizeof(LowFuse)+sizeof(Lockbits)+sizeof(fixint);
	}
	else if (state == 0)
		*m_pDataLen = 0;

	CMFCPropertyPage::OnOK();
}

LRESULT CDlg_ATtiny26::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();

}
LRESULT CDlg_ATtiny26::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}
void CDlg_ATtiny26::OnBnClickedEnable()
{

	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	int state = Encheck->GetCheck();
	if (state == 1)
	{
		m_RSTDISBL.EnableWindow(TRUE);
		m_SPIEN.EnableWindow(FALSE);
		m_EESAVE.EnableWindow(TRUE);
		m_BODLevel.EnableWindow(FALSE);
		m_BODEN.EnableWindow(TRUE);
		m_CKOPT.EnableWindow(TRUE);
		m_CKSEL.EnableWindow(TRUE);
		m_Lock.EnableWindow(TRUE);
	}
	else if (state == 0)
	{
		m_RSTDISBL.EnableWindow(FALSE);
		m_SPIEN.EnableWindow(FALSE);
		m_EESAVE.EnableWindow(FALSE);
		m_BODLevel.EnableWindow(FALSE);
		m_BODEN.EnableWindow(FALSE);
		m_CKOPT.EnableWindow(FALSE);
		m_CKSEL.EnableWindow(FALSE);
		m_Lock.EnableWindow(FALSE);
	}
}