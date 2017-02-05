// Dlg_ATtiny13.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_ATtiny13.h"
#include "afxdialogex.h"
#include "numeric_conversion.h"
#include "../common/prog_info.h"

// Dlg_ATtiny13 dialog

IMPLEMENT_DYNAMIC(CDlg_ATtiny13, CMFCPropertyPage)

CDlg_ATtiny13::CDlg_ATtiny13(struct chip_info_c *ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_ATtiny13::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_ATtiny13::~CDlg_ATtiny13()
{
}

void CDlg_ATtiny13::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_SELFPRGEN);
	DDX_Control(pDX, IDC_COMBO2, m_DWEN);
	DDX_Control(pDX, IDC_COMBO3, m_BODLevel);
	DDX_Control(pDX, IDC_COMBO4, m_RSTDISBL);
	DDX_Control(pDX, IDC_COMBO5, m_SPIEN);
	DDX_Control(pDX, IDC_COMBO6, m_EESAVE);
	DDX_Control(pDX, IDC_COMBO7, m_WDTON);
	DDX_Control(pDX, IDC_COMBO8, m_CKDIV8);
	DDX_Control(pDX, IDC_COMBO9, m_CKSEL);
	DDX_Control(pDX, IDC_COMBO10, m_Lock);
	DDX_Control(pDX, IDC_EDIT1, m_HFuse);
	DDX_Control(pDX, IDC_EDIT2, m_LFuse);
	DDX_Control(pDX, IDC_EDIT3, m_Lockbits);
}


BEGIN_MESSAGE_MAP(CDlg_ATtiny13, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_ATtiny13::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_ATtiny13::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_ATtiny13::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_ATtiny13::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_ATtiny13::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDlg_ATtiny13::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDlg_ATtiny13::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CDlg_ATtiny13::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO9, &CDlg_ATtiny13::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO10, &CDlg_ATtiny13::OnCbnSelchangeCombo)
	ON_BN_CLICKED(IDC_CHECK1, &CDlg_ATtiny13::OnBnClickedEnable)
END_MESSAGE_MAP()


// Dlg_ATtiny13 message handlers
BOOL CDlg_ATtiny13::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	HighFuse = 0xFF;
	LowFuse = 0x6A;
	Lockbits = 0xFF;
	m_HFuse.SetWindowTextW(L"FF");
	m_LFuse.SetWindowTextW(L"6A");
	m_Lockbits.SetWindowTextW(L"FF");

	m_SELFPRGEN.InsertString(m_SELFPRGEN.GetCount(), L"0 = Enable SPM Instruction");
	m_SELFPRGEN.InsertString(m_SELFPRGEN.GetCount(), L"1 = Disable SPM Instruction");
	m_DWEN.InsertString(m_DWEN.GetCount(), L"0 = Enable debugWIRE");
	m_DWEN.InsertString(m_DWEN.GetCount(), L"1 = Disable debugWIRE");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"00 = Trigger Level at 4.3V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"01 = Trigger Level at 2.7V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"10 = Trigger Level at 1.8V");
	m_BODLevel.InsertString(m_BODLevel.GetCount(), L"11 = BODLevel Disabled");
	m_RSTDISBL.InsertString(m_RSTDISBL.GetCount(), L"0 = Enabled external reset");
	m_RSTDISBL.InsertString(m_RSTDISBL.GetCount(), L"1 = Disabled external reset");
	m_SPIEN.InsertString(m_SPIEN.GetCount(), L"0 = Enabled ISP Programming");
	m_SPIEN.InsertString(m_SPIEN.GetCount(), L"1 = Disabled ISP Programming");
	m_EESAVE.InsertString(m_EESAVE.GetCount(), L"0 = Preserve EEPROM during Chip Erase");
	m_EESAVE.InsertString(m_EESAVE.GetCount(), L"1 = Unreserves EEPROM during Chip Erase");
	m_WDTON.InsertString(m_WDTON.GetCount(), L"0 =Set Watchdog Timer permanently on");
	m_WDTON.InsertString(m_WDTON.GetCount(), L"1 =UnSet Watchdog Timer permanently on");
	m_CKDIV8.InsertString(m_CKDIV8.GetCount(), L"0 = Divided CLK by 8");
	m_CKDIV8.InsertString(m_CKDIV8.GetCount(), L"1 = Disabled Divided");
	//Clock Setting
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0000 = Ext.CLK, StartupTime : 14CK");	// 0 = 0x00
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0100 = Ext.CLK, StartupTime : 14CK+4ms");	// 1 = 0x04
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"1000 = Ext.CLK, StartupTime : 14CK+64ms"); //2 = 0x08
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0001 = Int.RC OSC : 4.8MHz, StartupTime : 14CK"); //3 = 0x01
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0101 = Int.RC OSC : 4.8MHz, StartupTime : 14CK+4ms"); //4 = 0x05
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"1001 = Int.RC OSC : 4.8MHz, StartupTime : 14CK+64ms"); //5 = 0x09
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0010 = Int.RC OSC : 9.6MHz, StartupTime : 14CK"); //6 = 0x02
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0110 = Int.RC OSC : 9.6MHz, StartupTime : 14CK+4ms"); //7 = 0x06
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"1010 = Int.RC OSC : 9.6MHz, StartupTime : 14CK+64ms"); //8 = 0x0A
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0011 = Int.RC OSC : 128KHz, StartupTime : 14CK"); //9 = 0x02
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"0111 = Int.RC OSC : 128KHz, StartupTime : 14CK+4ms"); //10 = 0x06
	m_CKSEL.InsertString(m_CKSEL.GetCount(), L"1011 = Int.RC OSC : 128KHz, StartupTime : 14CK+64ms"); //11 = 0x0A

	m_Lock.InsertString(m_Lock.GetCount(), L"00 = Further Programming and verification Disabled");
	m_Lock.InsertString(m_Lock.GetCount(), L"10 = Further Programming Disabled");
	m_Lock.InsertString(m_Lock.GetCount(), L"11 = No memory Lock features enabled");

	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	Encheck->SetCheck(0);
	m_CKSEL.SetDroppedWidth(500);
	m_SELFPRGEN.SetCurSel(1);
	m_DWEN.SetCurSel(1);
	m_BODLevel.SetCurSel(3);
	m_RSTDISBL.SetCurSel(1);
	m_SPIEN.SetCurSel(0);
	m_EESAVE.SetCurSel(1);
	m_WDTON.SetCurSel(1);
	m_CKDIV8.SetCurSel(0);
	m_CKSEL.SetCurSel(8);
	m_Lock.SetCurSel(2);

	m_SELFPRGEN.EnableWindow(FALSE);
	m_DWEN.EnableWindow(FALSE);
	m_BODLevel.EnableWindow(FALSE);
	m_RSTDISBL.EnableWindow(FALSE);
	m_SPIEN.EnableWindow(FALSE);
	m_EESAVE.EnableWindow(FALSE);
	m_WDTON.EnableWindow(FALSE);
	m_CKDIV8.EnableWindow(FALSE);
	m_CKSEL.EnableWindow(FALSE);
	m_Lock.EnableWindow(FALSE);


	return TRUE;
}

BOOL CDlg_ATtiny13::OnSetActive()
{
	if (*m_pDataLen != NULL)
	{
		CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
		Encheck->SetCheck(1);
		m_SELFPRGEN.EnableWindow(TRUE);
		m_DWEN.EnableWindow(TRUE);
		m_BODLevel.EnableWindow(TRUE);
		m_RSTDISBL.EnableWindow(TRUE);
		m_SPIEN.EnableWindow(FALSE);
		m_EESAVE.EnableWindow(TRUE);
		m_WDTON.EnableWindow(TRUE);
		m_CKDIV8.EnableWindow(TRUE);
		m_CKSEL.EnableWindow(TRUE);
		m_Lock.EnableWindow(TRUE);

		CString ExF_str, HF_str, LF_str, L_str;
		//HighFuse
		memcpy((void*)&HighFuse, m_RegBuff, sizeof(HighFuse));
		HF_str.Format(_T("%02X"), HighFuse);
		m_HFuse.SetWindowTextW(HF_str);
		//LowFuse
		memcpy((void*)&LowFuse, m_RegBuff +sizeof(HighFuse), sizeof(LowFuse));
		LF_str.Format(_T("%02X"), LowFuse);
		m_LFuse.SetWindowTextW(LF_str);
		//Lockbits
		memcpy((void*)&Lockbits, m_RegBuff +sizeof(HighFuse)+sizeof(LowFuse), sizeof(Lockbits));
		L_str.Format(_T("%02X"), Lockbits);
		m_Lockbits.SetWindowTextW(L_str);

		//SELFPRGEN
		if ((HighFuse & 0x10) == 0x10)
			m_SELFPRGEN.SetCurSel(1);
		else if ((HighFuse & 0x10) == 0x00)
			m_SELFPRGEN.SetCurSel(0);
		//DWEN
		if ((HighFuse & 0x08) == 0x08)
			m_DWEN.SetCurSel(1);
		else if ((HighFuse & 0x08) == 0x00)
			m_DWEN.SetCurSel(0);
		//BODLevel
		if ((HighFuse & 0x06) == 0x06)
			m_BODLevel.SetCurSel(3);
		else if ((HighFuse & 0x06) == 0x04)
			m_BODLevel.SetCurSel(2);
		else if ((HighFuse & 0x06) == 0x02)
			m_BODLevel.SetCurSel(1);
		else if ((HighFuse & 0x06) == 0x00)
			m_BODLevel.SetCurSel(0);
		//RSTDISBL
		if ((HighFuse & 0x01) == 0x01)
			m_RSTDISBL.SetCurSel(1);
		else if ((HighFuse & 0x01) == 0x00)
			m_RSTDISBL.SetCurSel(0);
		//SPIEN
		if ((LowFuse & 0x80) == 0x80)
			m_SPIEN.SetCurSel(1);
		else if ((LowFuse & 0x80) == 0x00)
			m_SPIEN.SetCurSel(0);
		//EESAVE
		if ((LowFuse & 0x40) == 0x40)
			m_EESAVE.SetCurSel(1);
		else if ((LowFuse & 0x40) == 0x00)
			m_EESAVE.SetCurSel(0);
		//WDTON
		if ((LowFuse & 0x20) == 0x20)
			m_WDTON.SetCurSel(1);
		else if ((LowFuse & 0x20) == 0x00)
			m_WDTON.SetCurSel(0);
		//CKDIV8
		if ((LowFuse & 0x10) == 0x10)
			m_CKDIV8.SetCurSel(1);
		else if ((LowFuse & 0x10) == 0x00)
			m_CKDIV8.SetCurSel(0);
		//CLK Setting
		switch (LowFuse & 0x0F)
		{
		case 0x00:
			m_CKSEL.SetCurSel(0);
			break;
		case 0x04:
			m_CKSEL.SetCurSel(1);
			break;
		case 0x08:
			m_CKSEL.SetCurSel(2);
			break;
		case 0x01:
			m_CKSEL.SetCurSel(3);
			break;
		case 0x05:
			m_CKSEL.SetCurSel(4);
			break;
		case 0x09:
			m_CKSEL.SetCurSel(5);
			break;
		case 0x02:
			m_CKSEL.SetCurSel(6);
			break;
		case 0x06:
			m_CKSEL.SetCurSel(7);
			break;
		case 0x0A:
			m_CKSEL.SetCurSel(8);
			break;
		case 0x03:
			m_CKSEL.SetCurSel(9);
			break;
		case 0x07:
			m_CKSEL.SetCurSel(10);
			break;
		case 0x0B:
			m_CKSEL.SetCurSel(11);
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
		m_SELFPRGEN.EnableWindow(FALSE);
		m_DWEN.EnableWindow(FALSE);
		m_BODLevel.EnableWindow(FALSE);
		m_RSTDISBL.EnableWindow(FALSE);
		m_SPIEN.EnableWindow(FALSE);
		m_EESAVE.EnableWindow(FALSE);
		m_WDTON.EnableWindow(FALSE);
		m_CKDIV8.EnableWindow(FALSE);
		m_CKSEL.EnableWindow(FALSE);
		m_Lock.EnableWindow(FALSE);
	}
	return CMFCPropertyPage::OnSetActive();
}

void CDlg_ATtiny13::OnCbnSelchangeCombo()
{
	CString HFuse_str, LFuse_str, Lockbits_str;
	HighFuse = 0xFF;
	LowFuse = 0x6A;
	Lockbits = 0xFF;
	//SELFPRGEN
	HighFuse &= 0xEF;
	if (m_SELFPRGEN.GetCurSel() == 1)
		HighFuse |= 0x10;
	else if (m_SELFPRGEN.GetCurSel() == 0)
		HighFuse |= 0x00;
	HFuse_str.Format(_T("%02X"), HighFuse);
	m_HFuse.SetWindowTextW(HFuse_str);
	//DWEN
	HighFuse &= 0xF7;
	if (m_DWEN.GetCurSel() == 1)
		HighFuse |= 0x08;
	else if (m_DWEN.GetCurSel() == 0)
		HighFuse |= 0x00;
	HFuse_str.Format(_T("%02X"), HighFuse);
	m_HFuse.SetWindowTextW(HFuse_str);
	//BODLevel
	HighFuse &= 0xF9;
	if (m_BODLevel.GetCurSel() == 3)
		HighFuse |= 0x06;
	else if (m_BODLevel.GetCurSel() == 2)
		HighFuse |= 0x04;
	else if (m_BODLevel.GetCurSel() == 1)
		HighFuse |= 0x02;
	else if (m_BODLevel.GetCurSel() == 0)
		HighFuse |= 0x00;
	HFuse_str.Format(_T("%02X"), HighFuse);
	m_HFuse.SetWindowTextW(HFuse_str);
	//RSTDISBL
	HighFuse &= 0xFE;
	if (m_RSTDISBL.GetCurSel() == 1)
		HighFuse |= 0x01;
	else if (m_RSTDISBL.GetCurSel() == 0)
		HighFuse |= 0x00;
	HFuse_str.Format(_T("%02X"), HighFuse);
	m_HFuse.SetWindowTextW(HFuse_str);
	//SPIEN
	LowFuse &= 0x7F;
	if (m_SPIEN.GetCurSel() == 1)
		LowFuse |= 0x80;
	else if (m_SPIEN.GetCurSel() == 0)
		LowFuse |= 0x00;
	LFuse_str.Format(_T("%02X"), LowFuse);
	m_LFuse.SetWindowTextW(LFuse_str);
	//EESAVE
	LowFuse &= 0xBF;
	if (m_EESAVE.GetCurSel() == 1)
		LowFuse |= 0x40;
	else if(m_EESAVE.GetCurSel() == 0)
		LowFuse |= 0x00;
	LFuse_str.Format(_T("%02X"), LowFuse);
	m_LFuse.SetWindowTextW(LFuse_str);
	//WDTON
	LowFuse &= 0xDF;
	if (m_WDTON.GetCurSel() == 1)
		LowFuse |= 0x20;
	else if (m_WDTON.GetCurSel() == 0)
		LowFuse |= 0x00;
	LFuse_str.Format(_T("%02X"), LowFuse);
	m_LFuse.SetWindowTextW(LFuse_str);
	//CKDIV8
	LowFuse &= 0xEF;
	if (m_CKDIV8.GetCurSel() == 1)
		LowFuse |= 0x10;
	else if (m_CKDIV8.GetCurSel() == 0)
		LowFuse |= 0x00;
	LFuse_str.Format(_T("%02X"), LowFuse);
	m_LFuse.SetWindowTextW(LFuse_str);
	//CLK Setting
	LowFuse &= 0xF0;
	switch (m_CKSEL.GetCurSel())
	{
	case 0:
		LowFuse |= 0x00;
		break;
	case 1:
		LowFuse |= 0x04;
		break;
	case 2:
		LowFuse |= 0x08;
		break;
	case 3:
		LowFuse |= 0x01;
		break;
	case 4:
		LowFuse |= 0x05;
		break;
	case 5:
		LowFuse |= 0x09;
		break;
	case 6:
		LowFuse |= 0x02;
		break;
	case 7:
		LowFuse |= 0x06;
		break;
	case 8:
		LowFuse |= 0x0A;
		break;
	case 9:
		LowFuse |= 0x03;
		break;
	case 10:
		LowFuse |= 0x07;
		break;
	case 11:
		LowFuse |= 0x0B;
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

void CDlg_ATtiny13::OnOK()
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
		memcpy(m_RegBuff+sizeof(HighFuse), &LowFuse, sizeof(LowFuse));

		m_Lockbits.GetWindowTextW(temp);
		Lockbits = numeric_conversion::hexstring_to_size_t(temp.GetString());
		memcpy(m_RegBuff+sizeof(HighFuse)+sizeof(LowFuse), &Lockbits, sizeof(Lockbits));

		fixint = 0x00;
		memcpy(m_RegBuff + sizeof(HighFuse)+sizeof(LowFuse)+sizeof(Lockbits), &Lockbits, sizeof(fixint));

		*m_pDataLen = sizeof(HighFuse)+sizeof(LowFuse)+sizeof(Lockbits)+sizeof(fixint);
	}
	else if (state == 0)
		*m_pDataLen = 0;

	CMFCPropertyPage::OnOK();
}


LRESULT CDlg_ATtiny13::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();

}
LRESULT CDlg_ATtiny13::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}
void CDlg_ATtiny13::OnBnClickedEnable()
{
	
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	int state = Encheck->GetCheck();
	if (state == 1)
	{
		m_SELFPRGEN.EnableWindow(TRUE);
		m_DWEN.EnableWindow(TRUE);
		m_BODLevel.EnableWindow(TRUE);
		m_RSTDISBL.EnableWindow(TRUE);
		m_SPIEN.EnableWindow(FALSE);
		m_EESAVE.EnableWindow(TRUE);
		m_WDTON.EnableWindow(TRUE);
		m_CKDIV8.EnableWindow(TRUE);
		m_CKSEL.EnableWindow(TRUE);
		m_Lock.EnableWindow(TRUE);
	}
	else if (state == 0)
	{
		m_SELFPRGEN.EnableWindow(FALSE);
		m_DWEN.EnableWindow(FALSE);
		m_BODLevel.EnableWindow(FALSE);
		m_RSTDISBL.EnableWindow(FALSE);
		m_SPIEN.EnableWindow(FALSE);
		m_EESAVE.EnableWindow(FALSE);
		m_WDTON.EnableWindow(FALSE);
		m_CKDIV8.EnableWindow(FALSE);
		m_CKSEL.EnableWindow(FALSE);
		m_Lock.EnableWindow(FALSE);
	}
}