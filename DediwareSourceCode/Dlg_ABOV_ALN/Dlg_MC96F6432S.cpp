// Dlg_MC96F6432S.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_MC96F6432S.h"
#include "afxdialogex.h"
#include "../common/prog_info.h"
#include "numeric_conversion.h"

// Dlg_MC96F6432S dialog

IMPLEMENT_DYNAMIC(CDlg_MC96F6432S, CMFCPropertyPage)

CDlg_MC96F6432S::CDlg_MC96F6432S(struct chip_info_c *ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_MC96F6432S::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_MC96F6432S::~CDlg_MC96F6432S()
{
}

void CDlg_MC96F6432S::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_PAEN);
	DDX_Control(pDX, IDC_COMBO2, m_PASS);
	DDX_Control(pDX, IDC_COMBO3, m_RP);
	DDX_Control(pDX, IDC_COMBO4, m_HL);
	DDX_Control(pDX, IDC_COMBO5, m_ENRST);
	DDX_Control(pDX, IDC_EDIT1, m_CFG1);
	DDX_Control(pDX, IDC_EDIT2, m_CFG2);
	DDX_Control(pDX, IDC_EDIT3, m_checksum);
	DDX_Control(pDX, IDC_EDIT4, m_userid);
	DDX_Control(pDX, IDC_CHECK1, crc_enable);
	DDX_Control(pDX, IDC_CHECK2, ID_enable);	
}


BEGIN_MESSAGE_MAP(CDlg_MC96F6432S, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_MC96F6432S::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_MC96F6432S::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_MC96F6432S::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_MC96F6432S::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_MC96F6432S::OnCbnSelchangeCombo)
END_MESSAGE_MAP()


// Dlg_MC96F6432S message handlers
BOOL CDlg_MC96F6432S::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	Cfg1 = 0x00;
	Cfg2 = 0x00;

	m_PAEN.InsertString(m_PAEN.GetCount(), L"0 = Disable Protection");
	m_PAEN.InsertString(m_PAEN.GetCount(), L"1 = Enable Protection");
	m_PASS.InsertString(m_PASS.GetCount(), L"00 = 3.8KBytes(0100H-0FFFH)");
	m_PASS.InsertString(m_PASS.GetCount(), L"01 = 1.7KBytes(0100H-07FFH)");
	m_PASS.InsertString(m_PASS.GetCount(), L"10 = 768Bytes(0100H-03FFH)");
	m_PASS.InsertString(m_PASS.GetCount(), L"11 = 256Bytes(0100H-01FFH)");
	m_RP.InsertString(m_RP.GetCount(), L"0 = Disable Read Protection");
	m_RP.InsertString(m_RP.GetCount(), L"1 = Enable Read Protection");
	m_HL.InsertString(m_HL.GetCount(), L"0 = Disable Hard-Lock");
	m_HL.InsertString(m_HL.GetCount(), L"1 = Enable Hard-Lock");
	m_ENRST.InsertString(m_ENRST.GetCount(), L"0 = Disable RST input");
	m_ENRST.InsertString(m_ENRST.GetCount(), L"1 =Enable RST input");

	m_CFG1.SetWindowTextW(L"00");
	m_CFG2.SetWindowTextW(L"00");
	m_checksum.SetWindowTextW(L"0000");
	m_userid.SetWindowTextW(L"----------");

	if (wcscmp(_T("MC96F6432S"), m_ChipInfo->description) == 0)
	{
		ID_enable.EnableWindow(FALSE);
		m_userid.EnableWindow(FALSE);
	}
	m_PAEN.SetCurSel(0);
	m_PASS.SetCurSel(0);
	m_RP.SetCurSel(0);
	m_HL.SetCurSel(0);
	m_ENRST.SetCurSel(0);
	crc_enable.SetCheck(0);
	ID_enable.SetCheck(0);

	
	return TRUE;
}

BOOL CDlg_MC96F6432S::OnSetActive()
{
	if (*m_pDataLen != NULL)
	{
		CString Cfg1_str;
		CString Cfg2_str;
		CString checksum_str;

		memcpy((void*)&Cfg1, m_RegBuff, sizeof(Cfg1));
		Cfg1_str.Format(_T("%02X"), Cfg1);
		m_CFG1.SetWindowTextW(Cfg1_str);

		memcpy((void*)&Cfg2, m_RegBuff+sizeof(Cfg1), sizeof(Cfg2));
		Cfg2_str.Format(_T("%02X"), Cfg2);
		m_CFG2.SetWindowTextW(Cfg2_str);

		//Protection Area Enable
		if ((Cfg1 & 0x04) == 0x04)
			m_PAEN.SetCurSel(1);
		else if ((Cfg1 & 0x04) == 0x00)
			m_PAEN.SetCurSel(0);
		//Protection Area Size
		if ((Cfg1 & 0x03) == 0x03)
			m_PASS.SetCurSel(3);
		else if ((Cfg1 & 0x03) == 0x02)
			m_PASS.SetCurSel(2);
		else if ((Cfg1 & 0x03) == 0x01)
			m_PASS.SetCurSel(1);
		else if ((Cfg1 & 0x03) == 0x00)
			m_PASS.SetCurSel(0);
		//Read Protection
		if ((Cfg2 & 0x80) == 0x80)
			m_RP.SetCurSel(1);
		else if ((Cfg2 & 0x80) == 0x00)
			m_RP.SetCurSel(0);
		//Hard-Lock
		if ((Cfg2 & 0x40) == 0x40)
			m_HL.SetCurSel(1);
		else if ((Cfg2 & 0x40) == 0x00)
			m_HL.SetCurSel(0);
		//Enable Reset
		if ((Cfg2 & 0x01) == 0x01)
			m_ENRST.SetCurSel(1);
		else if ((Cfg2 & 0x01) == 0x00)
			m_ENRST.SetCurSel(0);

		//Enable crc clickbox
		memcpy((void*)&crc_check, m_RegBuff + sizeof(Cfg1)+sizeof(Cfg2)+2, sizeof(crc_check));
		crc_enable.SetCheck(crc_check);

		//Checksum Value
		memcpy((void*)&checksum, m_RegBuff + sizeof(Cfg1)+sizeof(Cfg2)+2 + sizeof(crc_check), sizeof(checksum));
		checksum_str.Format(_T("%04X"), checksum);
		m_checksum.SetWindowTextW(checksum_str);

		if (wcscmp(_T("MC96F6432"), m_ChipInfo->description) == 0)
		{
			//Enable user id clickbox
			memcpy((void*)&id_check, m_RegBuff + sizeof(Cfg1)+sizeof(Cfg2)+2 + sizeof(crc_check)+sizeof(checksum), sizeof(id_check));
			ID_enable.SetCheck(id_check);
			//Userid
			CString uid_str,uid_temp_str;
			unsigned int uid_temp[2] ;
			memcpy((void*)&uid_temp, m_RegBuff + sizeof(Cfg1)+sizeof(Cfg2)+2 + sizeof(crc_check)+sizeof(checksum)+sizeof(id_check)+4, sizeof(uid_temp));
			uid_temp_str.Format(_T("%08X"), uid_temp[1]);
			uid_temp_str.Delete(0, 4);
			uid_str += uid_temp_str;
			uid_temp_str.Format(_T("%08X"), uid_temp[0]);
			uid_str += uid_temp_str;
			m_userid.SetWindowTextW(uid_str);
		}
	}
	return CMFCPropertyPage::OnSetActive();
}

void CDlg_MC96F6432S::OnCbnSelchangeCombo()
{
	CString Cfg1_str;
	CString Cfg2_str;
	Cfg1 = 0x00;
	Cfg2 = 0x00;
	//Protection Area Enable
	if (m_PAEN.GetCurSel() == 1)
		Cfg1 |= 0x04;
	else if (m_PAEN.GetCurSel() == 0)
		Cfg1 |= 0x00;
	Cfg1_str.Format(_T("%02X"), Cfg1);
	m_CFG1.SetWindowTextW(Cfg1_str);

	//Protection Area Size
	if (m_PASS.GetCurSel() == 3)
		Cfg1 |= 0x03;
	else if (m_PASS.GetCurSel() == 2)
		Cfg1 |= 0x02;
	else if (m_PASS.GetCurSel() == 1)
		Cfg1 |= 0x01;
	else if (m_PASS.GetCurSel() == 0)
		Cfg1 |= 0x00;
	Cfg1_str.Format(_T("%02X"), Cfg1);
	m_CFG1.SetWindowTextW(Cfg1_str);

	//Read Protection
	if (m_RP.GetCurSel() == 1)
		Cfg2 |= 0x80;
	else if (m_RP.GetCurSel() == 0)
		Cfg2 |= 0x00;
	Cfg2_str.Format(_T("%02X"), Cfg2);
	m_CFG2.SetWindowTextW(Cfg2_str);

	//Hard Lock
	if (m_HL.GetCurSel() == 1)
		Cfg2 |= 0x40;
	else if (m_HL.GetCurSel() == 0)
		Cfg2 |= 0x00;
	Cfg2_str.Format(_T("%02X"), Cfg2);
	m_CFG2.SetWindowTextW(Cfg2_str);

	//Enable Reset
	if (m_ENRST.GetCurSel() == 1)
		Cfg2 |= 0x01;
	else if (m_ENRST.GetCurSel() == 0)
		Cfg2 |= 0x00;
	Cfg2_str.Format(_T("%02X"), Cfg2);
	m_CFG2.SetWindowTextW(Cfg2_str);

}
void CDlg_MC96F6432S::OnOK()
{

	CString temp;
	m_CFG1.GetWindowTextW(temp);
	Cfg1 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff, &Cfg1, sizeof(Cfg1));
	m_CFG2.GetWindowTextW(temp);
	Cfg2= numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff+sizeof(Cfg1), &Cfg2, sizeof(Cfg2));
	unsigned short int fixint = 0x0000;
	memcpy(m_RegBuff + sizeof(Cfg1)+sizeof(Cfg2), &fixint, sizeof(fixint));
	//crc checkbox
	crc_check = crc_enable.GetCheck();
	memcpy(m_RegBuff + sizeof(Cfg1)+sizeof(Cfg2)+sizeof(fixint), &crc_check, sizeof(crc_check));
	//Checksum
	m_checksum.GetWindowTextW(temp);
	checksum = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff + sizeof(Cfg1)+sizeof(Cfg2)+sizeof(fixint)+sizeof(crc_check), &checksum, sizeof(checksum));
	if (wcscmp(_T("MC96F6432"), m_ChipInfo->description) == 0)
	{
		//user id checkbox
		id_check = ID_enable.GetCheck();
		memcpy(m_RegBuff + sizeof(Cfg1)+sizeof(Cfg2)+sizeof(fixint)+sizeof(crc_check)+sizeof(checksum), &id_check, sizeof(id_check));
	}
	*m_pDataLen = m_ChipInfo->partition_info[1].size_in_bytes;
	CMFCPropertyPage::OnOK();
}

LRESULT CDlg_MC96F6432S::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();

}
LRESULT CDlg_MC96F6432S::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}
