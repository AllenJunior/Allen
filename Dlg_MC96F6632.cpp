// Dlg_MC96F6632.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_MC96F6632.h"
#include "numeric_conversion.h"
//#include "afxdialogex.h"
#include "../common/prog_info.h"


// Dlg_MC96F6632 dialog

IMPLEMENT_DYNAMIC(CDlg_MC96F6632, CMFCPropertyPage)

CDlg_MC96F6632::CDlg_MC96F6632(struct chip_info_c *ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_MC96F6632::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_MC96F6632::~CDlg_MC96F6632()
{
}

void CDlg_MC96F6632::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Cfg0);
	DDX_Control(pDX, IDC_EDIT2, m_Cfg1);
	DDX_Control(pDX, IDC_COMBO1, m_PAEN);
	DDX_Control(pDX, IDC_COMBO2, m_PASS);
	DDX_Control(pDX, IDC_COMBO3, m_RP);
	DDX_Control(pDX, IDC_COMBO4, m_HL);
	DDX_Control(pDX, IDC_CHECK1, m_EnUIDandCRC);
	DDX_Control(pDX, IDC_EDIT3, m_UserID);
	DDX_Control(pDX, IDC_EDIT4, m_CRC);
}


BEGIN_MESSAGE_MAP(CDlg_MC96F6632, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_MC96F6632::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_MC96F6632::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_MC96F6632::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_MC96F6632::OnCbnSelchangeCombo)
END_MESSAGE_MAP()


// Dlg_MC96F6632 message handlers
BOOL CDlg_MC96F6632::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	Option1 = 0x00;
	Option2 = 0x00;
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

	m_Cfg0.SetWindowTextW(L"00");
	m_Cfg1.SetWindowTextW(L"00");
	m_UserID.SetWindowTextW(L"00000000");
	m_CRC.SetWindowTextW(L"0000");
	m_PAEN.SetCurSel(0);
	m_PASS.SetCurSel(0);
	m_RP.SetCurSel(0);
	m_HL.SetCurSel(0);


	return TRUE;
}

BOOL CDlg_MC96F6632::OnSetActive()
{
	if (*m_pDataLen != NULL)
	{
		CString Cfg0;
		CString Cfg1;
		CString CRCValue_str;
		CString UserID_str,UserID_temp;
		memcpy((void*)&Option1, m_RegBuff, sizeof(Option1));
		Cfg0.Format(_T("%02X"), Option1);
		m_Cfg0.SetWindowTextW(Cfg0);

		memcpy((void*)&Option2, m_RegBuff + sizeof(Option1), sizeof(Option2));
		Cfg1.Format(_T("%02X"), Option2);
		m_Cfg1.SetWindowTextW(Cfg1);
		//Protection Area Enable
		if ((Option1 & 0x04) == 0x04)
			m_PAEN.SetCurSel(1);
		else if ((Option1 & 0x04) == 0x00)
			m_PAEN.SetCurSel(0);
		//Protection Area Size
		if ((Option1 & 0x03) == 0x03)
			m_PASS.SetCurSel(3);
		else if ((Option1 & 0x03) == 0x02)
			m_PASS.SetCurSel(2);
		else if ((Option1 & 0x03) == 0x01)
			m_PASS.SetCurSel(1);
		else if ((Option1 & 0x03) == 0x00)
			m_PASS.SetCurSel(0);
		//Read Protection
		if ((Option2 & 0x80) == 0x80)
			m_RP.SetCurSel(1);
		else if ((Option2 & 0x80) == 0x00)
			m_RP.SetCurSel(0);
		//Hard-Lock
		if ((Option2 & 0x40) == 0x40)
			m_HL.SetCurSel(1);
		else if ((Option2 & 0x40) == 0x00)
			m_HL.SetCurSel(0);
		//Check Button
		
		memcpy((void*)&UC_En, m_RegBuff + sizeof(Option1)+sizeof(Option2)+2, sizeof(UC_En));
		if ((UC_En & 0x00000001) == 0x00)
		{
			CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
			Encheck->SetCheck(0);
		}
		else if ((UC_En & 0x00000001) == 0x01)
		{
			CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
			Encheck->SetCheck(1);
		}

		memcpy((void*)&CRC, m_RegBuff + sizeof(Option1)+sizeof(Option2)+2 + sizeof(UC_En)+4, sizeof(CRC));
		CRCValue_str.Format(_T("%04X"), CRC);
		m_CRC.SetWindowTextW(CRCValue_str);
		int UserID_temp_D[2];
		memcpy((void*)&UserID_temp_D, m_RegBuff + sizeof(Option1)+sizeof(Option2)+2 + sizeof(UC_En)+4 + sizeof(CRC), 8);
		UserID_temp.Format(_T("%08X"), UserID_temp_D[1]);
		UserID_str += UserID_temp;
		UserID_temp.Format(_T("%08X"), UserID_temp_D[0]);
		UserID_temp.Delete(4, 4);
		UserID_str += UserID_temp;
		m_UserID.SetWindowTextW(UserID_str);

	}
	return CMFCPropertyPage::OnSetActive();
}

void CDlg_MC96F6632::OnCbnSelchangeCombo()
{
	CString Cfg0;
	CString Cfg1;
	Option1 = 0x00;
	Option2 = 0x00;

	//Protection Area Enable
	if (m_PAEN.GetCurSel() == 1)
		Option1 |= 0x04;
	else if (m_PAEN.GetCurSel() == 0)
		Option1 |= 0x00;
	Cfg0.Format(_T("%02X"), Option1);
	m_Cfg0.SetWindowTextW(Cfg0);

	//Protection Area Size
	if (m_PASS.GetCurSel() == 3)
		Option1 |= 0x03;
	else if (m_PASS.GetCurSel() == 2)
		Option1 |= 0x02;
	else if (m_PASS.GetCurSel() == 1)
		Option1 |= 0x01;
	else if (m_PASS.GetCurSel() == 0)
		Option1 |= 0x00;
	Cfg0.Format(_T("%02X"), Option1);
	m_Cfg0.SetWindowTextW(Cfg0);

	//Read Protection
	if (m_RP.GetCurSel() == 1)
		Option2 |= 0x80;
	else if (m_RP.GetCurSel() == 0)
		Option2 |= 0x00;
	Cfg1.Format(_T("%02X"), Option2);
	m_Cfg1.SetWindowTextW(Cfg1);

	//Hard Lock
	if (m_HL.GetCurSel() == 1)
		Option2 |= 0x40;
	else if (m_HL.GetCurSel() == 0)
		Option2 |= 0x00;
	Cfg1.Format(_T("%02X"), Option2);
	m_Cfg1.SetWindowTextW(Cfg1);

}

void CDlg_MC96F6632::OnOK()
{
	CString temp;
	m_Cfg0.GetWindowTextW(temp);
	Option1 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff, &Option1, sizeof(Option1));

	m_Cfg1.GetWindowTextW(temp);
	Option2 = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff + sizeof(Option1), &Option2, sizeof(Option2));

	unsigned short int fixint = 0x0000;
	memcpy(m_RegBuff + sizeof(Option1)+sizeof(Option2), &fixint, sizeof(fixint));
	
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	int state = Encheck->GetCheck();
	if (state == 0x0)
	{
		UC_En = 0x0;
		memcpy(m_RegBuff + 4, &UC_En, 4);
	}
	else if (state == 0x1)
	{
		UC_En = 0x1;

		memcpy(m_RegBuff + 4, &UC_En, 4);
	}
	//CRC Value
	m_CRC.GetWindowTextW(temp);
	CRC = numeric_conversion::hexstring_to_size_t(temp.GetString());
	memcpy(m_RegBuff + 12, &CRC, 4);

	*m_pDataLen = sizeof(Option1)+sizeof(Option2)+sizeof(fixint)+sizeof(UC_En) + 8 + 8;
	CMFCPropertyPage::OnOK();
}

LRESULT CDlg_MC96F6632::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();

}
LRESULT CDlg_MC96F6632::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}
