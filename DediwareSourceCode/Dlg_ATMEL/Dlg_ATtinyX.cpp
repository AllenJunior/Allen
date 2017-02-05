// Dlg_ATtinyX.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_ATtinyX.h"
#include "afxdialogex.h"
#include "numeric_conversion.h"
#include "../common/prog_info.h"

// Dlg_ATtinyX dialog

IMPLEMENT_DYNAMIC(CDlg_ATtinyX, CMFCPropertyPage)

CDlg_ATtinyX::CDlg_ATtinyX(struct chip_info_c *ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_ATtinyX::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_ATtinyX::~CDlg_ATtinyX()
{
}

void CDlg_ATtinyX::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_Lockbits);
	DDX_Control(pDX, IDC_COMBO2, m_CKOUT);
	DDX_Control(pDX, IDC_COMBO3, m_WDTON);
	DDX_Control(pDX, IDC_COMBO4, m_RSTDISBL);
	DDX_Control(pDX, IDC_COMBO5, m_SELFPROGEN);
	DDX_Control(pDX, IDC_EDIT1, m_Cfg0);
	DDX_Control(pDX, IDC_EDIT2, m_Cfg1);
	DDX_Control(pDX, IDC_CHECK1, Option_Enable);
	
}


BEGIN_MESSAGE_MAP(CDlg_ATtinyX, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_ATtinyX::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_ATtinyX::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_ATtinyX::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_ATtinyX::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_ATtinyX::OnCbnSelchangeCombo)
	ON_BN_CLICKED(IDC_CHECK1, &CDlg_ATtinyX::OnBnClickedEnable)
END_MESSAGE_MAP()


// Dlg_ATtinyX message handlers
BOOL CDlg_ATtinyX::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	Fuse = 0xFF;
	Lockbits = 0xFF;
	m_Cfg0.SetWindowTextW(L"FF");
	m_Cfg1.SetWindowTextW(L"FF");
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	Encheck->SetCheck(0);
	m_Lockbits.InsertString(m_Lockbits.GetCount(), L"Further Programming and Verification Disabled(LB2=0,LB1=0)");
	m_Lockbits.InsertString(m_Lockbits.GetCount(), L"Further Programming Disabled(LB2=1,LB1=0)");
	m_Lockbits.InsertString(m_Lockbits.GetCount(), L"No Memory Lock features enabled(LB2=1,LB1=1)");
	m_CKOUT.InsertString(m_CKOUT.GetCount(), L"0 = Enabled");
	m_CKOUT.InsertString(m_CKOUT.GetCount(), L"1 = Disabled");
	m_WDTON.InsertString(m_WDTON.GetCount(), L"0 = Enabled");
	m_WDTON.InsertString(m_WDTON.GetCount(), L"1 = Disabled");
	m_RSTDISBL.InsertString(m_RSTDISBL.GetCount(), L"0 = Enabled");
	m_RSTDISBL.InsertString(m_RSTDISBL.GetCount(), L"1 = Disabled");

	//ATtiny1X Only
	if (wcscmp(_T("ATtiny1X"), m_ChipInfo->description) == 0)
	{
		m_SELFPROGEN.InsertString(m_SELFPROGEN.GetCount(), L"0 = Enabled");
		m_SELFPROGEN.InsertString(m_SELFPROGEN.GetCount(), L"1 = Disabled");
		m_SELFPROGEN.SetCurSel(1);
	}
	else if(wcscmp(_T("ATtinyX"), m_ChipInfo->description) == 0)
	{
		m_SELFPROGEN.InsertString(m_SELFPROGEN.GetCount(), L"Not Supported");
		m_SELFPROGEN.SetCurSel(0);
	}	

	m_Lockbits.SetCurSel(2);
	m_CKOUT.SetCurSel(1);
	m_WDTON.SetCurSel(1);
	m_RSTDISBL.SetCurSel(1);
	
	m_Lockbits.SetDroppedWidth(400);
	m_Lockbits.EnableWindow(FALSE);
	m_CKOUT.EnableWindow(FALSE);
	m_WDTON.EnableWindow(FALSE);
	m_RSTDISBL.EnableWindow(FALSE);
	m_SELFPROGEN.EnableWindow(FALSE);

	
	
	return TRUE;
}

BOOL CDlg_ATtinyX::OnSetActive()
{
	if (*m_pDataLen != NULL)
	{
		CString Cfg0;
		CString Cfg1;
		if (wcscmp(_T("ATtiny1X"), m_ChipInfo->description) == 0)
		{
			m_SELFPROGEN.EnableWindow(TRUE);
		}
		else
		{
			m_SELFPROGEN.EnableWindow(FALSE);
		}
		m_Lockbits.EnableWindow(TRUE);
		m_CKOUT.EnableWindow(TRUE);
		m_WDTON.EnableWindow(TRUE);
		m_RSTDISBL.EnableWindow(TRUE);
		
		CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
		Encheck->SetCheck(1);
		memcpy((void*)&Lockbits, m_RegBuff, sizeof(Lockbits));
		Cfg0.Format(_T("%02X"), Lockbits);
		m_Cfg0.SetWindowTextW(Cfg0);
		memcpy((void*)&Fuse, m_RegBuff + sizeof(Lockbits), sizeof(Fuse));
		Cfg1.Format(_T("%02X"), Fuse);
		m_Cfg1.SetWindowTextW(Cfg1);
		//Lock byte
		if ((Lockbits & 0x03) == 0x03)
			m_Lockbits.SetCurSel(2);
		else if ((Lockbits & 0x03) == 0x02)
			m_Lockbits.SetCurSel(1);
		else if ((Lockbits & 0x03) == 0x00)
			m_Lockbits.SetCurSel(0);
		//Self-Programming
		if ((Fuse & 0x08) == 0x08)
			m_SELFPROGEN.SetCurSel(1);
		else if ((Fuse & 0x08) == 0x00)
			m_SELFPROGEN.SetCurSel(0);
		//System Clock Output
		if ((Fuse & 0x04) == 0x04)
			m_CKOUT.SetCurSel(1);
		else if ((Fuse & 0x04) == 0x00)
			m_CKOUT.SetCurSel(0);
		//Watchdog Timer always on
		if ((Fuse & 0x02) == 0x02)
			m_WDTON.SetCurSel(1);
		else if ((Fuse & 0x02) == 0x00)
			m_WDTON.SetCurSel(0);
		//External Reset disable
		if ((Fuse & 0x01) == 0x01)
			m_RSTDISBL.SetCurSel(1);
		else if ((Fuse & 0x01) == 0x00)
			m_RSTDISBL.SetCurSel(0);
	}
	else if (*m_pDataLen == NULL)
	{
		CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
		Encheck->SetCheck(0);
		m_Lockbits.EnableWindow(FALSE);
		m_CKOUT.EnableWindow(FALSE);
		m_WDTON.EnableWindow(FALSE);
		m_RSTDISBL.EnableWindow(FALSE);
		m_SELFPROGEN.EnableWindow(FALSE);
	}
	
	return CMFCPropertyPage::OnSetActive();
}

void CDlg_ATtinyX::OnCbnSelchangeCombo()
{
	CString Cfg0;
	CString Cfg1;
	Lockbits = 0xFF;
	Fuse = 0xFF;
	//Lock byte
	Lockbits &= 0xFC;
	if (m_Lockbits.GetCurSel() == 2)
		Lockbits |= 0x03;
	else if (m_Lockbits.GetCurSel() == 1)
		Lockbits |= 0x02;
	else if (m_Lockbits.GetCurSel() == 0)
		Lockbits |= 0x00;
	Cfg0.Format(_T("%02X"), Lockbits);
	m_Cfg0.SetWindowTextW(Cfg0);
	//ATtiny10X Only
	if (wcscmp(_T("ATtiny1X"), m_ChipInfo->description) == 0)
	{
		//Self-Programming
		Fuse &= 0xF7;
		if (m_SELFPROGEN.GetCurSel() == 1)
			Fuse |= 0x08;
		else if (m_SELFPROGEN.GetCurSel() == 0)
			Fuse |= 0x00;
		Cfg1.Format(_T("%02X"), Fuse);
		m_Cfg1.SetWindowTextW(Cfg1);
	}
	//system Clock Output
	Fuse &= 0xFB;
	if (m_CKOUT.GetCurSel() == 1)
		Fuse |= 0x04;
	else if (m_CKOUT.GetCurSel() == 0)
		Fuse |= 0x00;
	Cfg1.Format(_T("%02X"), Fuse);
	m_Cfg1.SetWindowTextW(Cfg1);
	//Watchdog Timer always on
	Fuse &= 0xFD;
	if (m_WDTON.GetCurSel() == 1)
		Fuse |= 0x02;
	else if (m_WDTON.GetCurSel() == 0)
		Fuse |= 0x00;
	Cfg1.Format(_T("%02X"), Fuse);
	m_Cfg1.SetWindowTextW(Cfg1);
	// External Reset disable
	Fuse &= 0xFE;
	if (m_RSTDISBL.GetCurSel() == 1)
		Fuse |= 0x01;
	else if (m_RSTDISBL.GetCurSel() == 0)
		Fuse |= 0x00;
	Cfg1.Format(_T("%02X"), Fuse);
	m_Cfg1.SetWindowTextW(Cfg1);
}

void CDlg_ATtinyX::OnOK()
{
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	int state = Encheck->GetCheck();
	if (state == 0x01)
	{
		CString temp;
		m_Cfg0.GetWindowTextW(temp);
		Lockbits = numeric_conversion::hexstring_to_size_t(temp.GetString());
		memcpy(m_RegBuff, &Lockbits, sizeof(Lockbits));
		
		m_Cfg1.GetWindowTextW(temp);
		Fuse = numeric_conversion::hexstring_to_size_t(temp.GetString());
		memcpy(m_RegBuff + sizeof(Lockbits), &Fuse, sizeof(Fuse));

		short int toint = 0x0000;
		memcpy(m_RegBuff + sizeof(Lockbits)+sizeof(Fuse), &toint, sizeof(toint));

		*m_pDataLen = sizeof(Lockbits)+sizeof(Fuse)+sizeof(toint);

	}
	else if (state == 0x00)
		*m_pDataLen = 0;
	CMFCPropertyPage::OnOK();
}

LRESULT CDlg_ATtinyX::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();

}
LRESULT CDlg_ATtinyX::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}

void CDlg_ATtinyX::OnBnClickedEnable()
{
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	unsigned int state = Encheck->GetCheck();
	m_Lockbits.EnableWindow(state);
	m_CKOUT.EnableWindow(state);
	m_WDTON.EnableWindow(state);
	m_RSTDISBL.EnableWindow(state);
	//ATtiny1X Only
	if (wcscmp(_T("ATtiny1X"), m_ChipInfo->description) == 0)
	{
		m_SELFPROGEN.EnableWindow(state);
	}
	else if (wcscmp(_T("ATtinyX"), m_ChipInfo->description) == 0)
	{
		m_SELFPROGEN.EnableWindow(FALSE);
	}
}


