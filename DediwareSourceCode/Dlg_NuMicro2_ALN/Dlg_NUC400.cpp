// Dlg_NUC400.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_NUC400.h"
#include "afxdialogex.h"
#include "numeric_conversion.h"
#include "../common/prog_info.h"

unsigned char CRC8(unsigned char *data, unsigned char datalen)
{
	unsigned char i;
	unsigned char crc = 0xFF;   //init value, which means the seed;
	for (unsigned char j = 0; j < datalen; j++)
	{
		i = 0x80;
		for (int k = 0; k < 8; k++)
		{
			if ((crc & 0x80) != 0)    /* 余式CRC 乘以2 再求CRC */
			{
				crc *= 0x2;
				crc ^= 0x7;
			}
			else crc *= 0x2;

			if ((*data&i) != 0)
			{
				crc ^= 0x7;                    /* 再加上本位的CRC */
				i /= 2;
			}
			else
				i /= 2;
		}

		data++;
	}
	return(crc);
}

unsigned int CFGCHECKSUM(unsigned int cfg0_temp, unsigned int cfg1_temp, unsigned int cfg2_temp)
{
	if ((cfg0_temp != 0xFFFFFFFF) || (cfg1_temp != 0xFFFFFFFF) || (cfg2_temp != 0xFFFFFFFF))
	{
		CString temp_str;
		unsigned char CRC_Config0[4] = { 0x00 };
		unsigned char CRC_Config1[4] = { 0x00 };
		unsigned char CRC_Config2[4] = { 0x00 };
		unsigned char CRC_Config3[4] = { 0x00 };
		unsigned int cfg3_temp = 0;

		memcpy(CRC_Config0, &cfg0_temp, sizeof(cfg0_temp));
		memcpy(CRC_Config1, &cfg1_temp, sizeof(cfg1_temp));
		memcpy(CRC_Config2, &cfg2_temp, sizeof(cfg2_temp));
		for (unsigned int i = 0; i < 4; i++)
		{
			unsigned char CRC_temp[3] =
			{
				CRC_Config0[i], CRC_Config1[i], CRC_Config2[i],
			};
			CRC_Config3[i] = CRC8(CRC_temp, sizeof(CRC_temp));
		}
		memcpy(&cfg3_temp, CRC_Config3, sizeof(CRC_Config3));
		return cfg3_temp;
	}
	else
		return 0xFFFFFFFF;
}

// Dlg_NUC400 dialog

IMPLEMENT_DYNAMIC(CDlg_NUC400, CMFCPropertyPage)

CDlg_NUC400::CDlg_NUC400(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(CDlg_NUC400::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

CDlg_NUC400::~CDlg_NUC400()
{
}

void CDlg_NUC400::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_cfg0);
	DDX_Control(pDX, IDC_EDIT2, m_cfg1);
	DDX_Control(pDX, IDC_EDIT3, m_cfg2);
	DDX_Control(pDX, IDC_EDIT4, m_cfg3);
	DDX_Control(pDX, IDC_EDIT5, m_dataAddr);
	DDX_Control(pDX, IDC_EDIT6, m_BWP);
	DDX_Control(pDX, IDC_COMBO1, m_CWDTEN);
	DDX_Control(pDX, IDC_COMBO2, m_CWDTPDEN);
	DDX_Control(pDX, IDC_COMBO3, m_CKF);
	DDX_Control(pDX, IDC_COMBO4, m_CFGXT1);
	DDX_Control(pDX, IDC_COMBO5, m_CFOSC);
	DDX_Control(pDX, IDC_COMBO6, m_CBODEN);
	DDX_Control(pDX, IDC_COMBO7, m_CBOV);
	DDX_Control(pDX, IDC_COMBO8, m_CBORST);
	DDX_Control(pDX, IDC_COMBO9, m_RMII);
	DDX_Control(pDX, IDC_COMBO10, m_CFG32K);
	DDX_Control(pDX, IDC_COMBO11, m_LDWPEN);
	DDX_Control(pDX, IDC_COMBO12, m_CIOINI);
	DDX_Control(pDX, IDC_COMBO13, m_CBS);
	DDX_Control(pDX, IDC_COMBO14, m_LOCK);
	DDX_Control(pDX, IDC_CHECK1, m_DFEN);
}


BEGIN_MESSAGE_MAP(CDlg_NUC400, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_NUC400::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlg_NUC400::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlg_NUC400::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlg_NUC400::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlg_NUC400::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDlg_NUC400::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDlg_NUC400::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CDlg_NUC400::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO9, &CDlg_NUC400::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO10, &CDlg_NUC400::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO11, &CDlg_NUC400::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO12, &CDlg_NUC400::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO13, &CDlg_NUC400::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO14, &CDlg_NUC400::OnCbnSelchangeCombo)
	ON_EN_CHANGE(IDC_EDIT5, &CDlg_NUC400::OnEnChangedataAddress)
	ON_BN_CLICKED(IDC_CHECK1, &CDlg_NUC400::OnBnClickedDFENCheck)
	ON_EN_CHANGE(IDC_EDIT6, &CDlg_NUC400::OnEnChangeFlashBlockProtect)
END_MESSAGE_MAP()


// Dlg_NUC400 message handlers
BOOL CDlg_NUC400::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	Config0 = 0xFFFFFFFF;
	Config1 = 0xFFFFFFFF;
	Config2 = 0xFFFFFFFF;
	Config3 = 0xFFFFFFFF;

	m_cfg0.SetWindowTextW(L"FFFFFFFF");
	m_cfg1.SetWindowTextW(L"FFFFFFFF");
	m_cfg2.SetWindowTextW(L"FFFFFFFF");
	m_cfg3.SetWindowTextW(L"FFFFFFFF");
	m_dataAddr.EnableWindow(FALSE);
	m_dataAddr.SetWindowTextW(L"FFFFFFFF");
	m_BWP.SetWindowTextW(L"FFFFFFFF");
	m_CWDTEN.InsertString(m_CWDTEN.GetCount(), L"0 = Watchdog Timer Enabled");
	m_CWDTEN.InsertString(m_CWDTEN.GetCount(), L"1 = Watchdog Timer Disabled");
	m_CWDTPDEN.InsertString(m_CWDTPDEN.GetCount(), L"0 = Watchdog Timer Clock kept enabled");
	m_CWDTPDEN.InsertString(m_CWDTPDEN.GetCount(), L"1 = Watchdog Timer Clock is controlled by LIRCEN");
	m_CKF.InsertString(m_CKF.GetCount(), L"0 = XT1 Clock filter Disabled");
	m_CKF.InsertString(m_CKF.GetCount(), L"1 = XT1 Clock filter Enabled");
	m_CFGXT1.InsertString(m_CFGXT1.GetCount(), L"0 = CPG[13:12] work as GPIO");
	m_CFGXT1.InsertString(m_CFGXT1.GetCount(), L"1 = CPG[13:12] work as external 4~24MHz crystal pin");
	m_CFOSC.InsertString(m_CFOSC.GetCount(), L"000 = External 4~24MHz high-speed crystal clock");
	m_CFOSC.InsertString(m_CFOSC.GetCount(), L"111 = Internal RC 22.1184MHz high-speed oscillator clock");
	m_CBODEN.InsertString(m_CBODEN.GetCount(), L"0 = Brown-out detect Enabled");
	m_CBODEN.InsertString(m_CBODEN.GetCount(), L"1 = Brown-out detect Disabled");
	m_CBOV.InsertString(m_CBOV.GetCount(), L"00 = 2.2V");
	m_CBOV.InsertString(m_CBOV.GetCount(), L"01 = 2.7V");
	m_CBOV.InsertString(m_CBOV.GetCount(), L"10 = 3.7V");
	m_CBOV.InsertString(m_CBOV.GetCount(), L"11 = 4.5V");
	m_CBORST.InsertString(m_CBORST.GetCount(), L"0 = Brown-out Reset Enabled");
	m_CBORST.InsertString(m_CBORST.GetCount(), L"1 = Brown-out Reset Disabled");
	m_RMII.InsertString(m_RMII.GetCount(), L"0 = MII mode Enabled");
	m_RMII.InsertString(m_RMII.GetCount(), L"1 = RMII mode Enabled");
	m_CFG32K.InsertString(m_CFG32K.GetCount(), L"0 = CPG[15:14] work as GPIO");
	m_CFG32K.InsertString(m_CFG32K.GetCount(), L"1 = CPG[15:14] work as external 32KHz crystal pin");
	m_LDWPEN.InsertString(m_LDWPEN.GetCount(), L"0 = LDROM Write protect Enabled");
	m_LDWPEN.InsertString(m_LDWPEN.GetCount(), L"1 = LDROM Write protect Disabled");
	m_CIOINI.InsertString(m_CIOINI.GetCount(), L"0 = All GPIO Set as Quasi-bidirectional mode");
	m_CIOINI.InsertString(m_CIOINI.GetCount(), L"1 = All GPIO Set as input tri-state mode");
	m_CBS.InsertString(m_CBS.GetCount(), L"00 = Boot from LDROM with IAP function");
	m_CBS.InsertString(m_CBS.GetCount(), L"01 = Boot from LDROM");
	m_CBS.InsertString(m_CBS.GetCount(), L"10 = Boot from APROM with IAP function");
	m_CBS.InsertString(m_CBS.GetCount(), L"11 = Boot from APROM");
	m_LOCK.InsertString(m_LOCK.GetCount(), L"0 = Lock Enabled");
	m_LOCK.InsertString(m_LOCK.GetCount(), L"1 = Lock Disabled");

	m_CWDTEN.SetCurSel(1);
	m_CWDTPDEN.SetCurSel(1);
	m_CKF.SetCurSel(1);
	m_CFGXT1.SetCurSel(1);
	m_CFOSC.SetCurSel(1);
	m_CBODEN.SetCurSel(1);
	m_CBOV.SetCurSel(3);
	m_CBORST.SetCurSel(1);
	m_RMII.SetCurSel(1);
	m_CFG32K.SetCurSel(1);
	m_LDWPEN.SetCurSel(1);
	m_CIOINI.SetCurSel(1);
	m_CBS.SetCurSel(3);
	m_LOCK.SetCurSel(1);
	m_DFEN.SetCheck(0);
	
	return TRUE;
}

BOOL CDlg_NUC400::OnSetActive()
{
	CString CFG0, CFG1, CFG2, CFG3;
	if (*m_pDataLen != NULL)
	{
		memcpy((void*)&Config0, m_RegBuff, sizeof(Config0));
		memcpy((void*)&Config1, m_RegBuff+sizeof(Config0), sizeof(Config1));
		memcpy((void*)&Config2, m_RegBuff + sizeof(Config0)+sizeof(Config1), sizeof(Config2));
		memcpy((void*)&Config3, m_RegBuff + sizeof(Config0)+sizeof(Config1)+sizeof(Config2), sizeof(Config3));
		CFG0.Format(_T("%08X"), Config0);
		m_cfg0.SetWindowTextW(CFG0);
		CFG1.Format(_T("%08X"), Config1);
		m_cfg1.SetWindowTextW(CFG1);		
		CFG2.Format(_T("%08X"), Config2);
		m_cfg2.SetWindowTextW(CFG2);		
		CFG3.Format(_T("%08X"), Config3);
		m_cfg3.SetWindowTextW(CFG3);
		//Watchdog hardware Enable
		if ((Config0 & 0x80000000) == 0x80000000)
			m_CWDTEN.SetCurSel(1);
		else if ((Config0 & 0x80000000) == 0x00)
			m_CWDTEN.SetCurSel(0);
		//Watchdog clock Power-down Enable
		if ((Config0 & 0x40000000) == 0x40000000)
			m_CWDTPDEN.SetCurSel(1);
		else if ((Config0 & 0x40000000) == 0x00)
			m_CWDTPDEN.SetCurSel(0);
		//XT1 clock filter Enable
		if ((Config0 & 0x10000000) == 0x10000000)
			m_CKF.SetCurSel(1);
		else if ((Config0 & 0x10000000) == 0x00)
			m_CKF.SetCurSel(0);
		//CPG[13:12] Multi-Function
		if ((Config0 & 0x08000000) == 0x08000000)
			m_CFGXT1.SetCurSel(1);
		else if ((Config0 & 0x08000000) == 0x00)
			m_CFGXT1.SetCurSel(0);
		//CPU Clock Source Selection
		if ((Config0 & 0x07000000) == 0x07000000)
			m_CFOSC.SetCurSel(1);
		else if ((Config0 & 0x07000000) == 0x00)
			m_CFOSC.SetCurSel(0);
		//Brown-out Detector
		if ((Config0 & 0x800000) == 0x800000)
			m_CBODEN.SetCurSel(1);
		else if ((Config0 & 0x800000) == 0x00)
			m_CBODEN.SetCurSel(0);
		//Brown-out Voltage
		if ((Config0 & 0x600000) == 0x600000)
			m_CBOV.SetCurSel(3);
		else if ((Config0 & 0x600000) == 0x400000)
			m_CBOV.SetCurSel(2);
		else if ((Config0 & 0x600000) == 0x200000)
			m_CBOV.SetCurSel(1);
		else if ((Config0 & 0x600000) == 0x00)
			m_CBOV.SetCurSel(0);
		//Brown-out Reset
		if ((Config0 & 0x100000) == 0x100000)
			m_CBORST.SetCurSel(1);
		else if ((Config0 & 0x100000) == 0x00)
			m_CBORST.SetCurSel(0);
		//EMAC Interface
		if ((Config0 & 0x8000) == 0x8000)
			m_RMII.SetCurSel(1);
		else if ((Config0 & 0x8000) == 0x00)
			m_RMII.SetCurSel(0);
		//CPG[15:14] Multi-Function
		if ((Config0 & 0x4000) == 0x4000)
			m_CFG32K.SetCurSel(1);
		else if ((Config0 & 0x4000) == 0x00)
			m_CFG32K.SetCurSel(0);
		//LDROM Write protect
		if ((Config0 & 0x0800) == 0x0800)
			m_LDWPEN.SetCurSel(1);
		else if ((Config0 & 0x0800) == 0x00)
			m_LDWPEN.SetCurSel(0);
		//I/O Initial state selection
		if ((Config0 & 0x0400) == 0x0400)
			m_CIOINI.SetCurSel(1);
		else if ((Config0 & 0x0400) == 0x00)
			m_CIOINI.SetCurSel(0);
		//Chip Boot Selection
		if ((Config0 & 0xC0) == 0xC0)
			m_CBS.SetCurSel(3);
		else if ((Config0 & 0xC0) == 0x80)
			m_CBS.SetCurSel(2);
		else if ((Config0 & 0xC0) == 0x40)
			m_CBS.SetCurSel(1);
		else if ((Config0 & 0xC0) == 0x00)
			m_CBS.SetCurSel(0);
		//Lock Mode
		if ((Config0 & 0x02) == 0x02)
			m_LOCK.SetCurSel(1);
		else if ((Config0 & 0x02) == 0x00)
			m_LOCK.SetCurSel(0);
		//Data Flash Enable
		if ((Config0 & 0x01) == 0x01)
		{
			m_DFEN.SetCheck(0);
			m_dataAddr.EnableWindow(FALSE);
		}			
		else if ((Config0 & 0x01) == 0x00)
		{
			m_DFEN.SetCheck(1);
			m_dataAddr.SetWindowTextW(CFG1);
			m_dataAddr.EnableWindow(TRUE);
		}
	}
	return CMFCPropertyPage::OnSetActive();
}

void CDlg_NUC400::OnCbnSelchangeCombo()
{
	CString temp_str;
	Config0 = 0xFFFFFFFF;

	//Watchdog hardware Enable
	Config0 &= 0x7FFFFFFF;
	if (m_CWDTEN.GetCurSel() == 1)
		Config0 |= 0x80000000;
	else if (m_CWDTEN.GetCurSel() == 0)
		Config0 |= 0x00;
	temp_str.Format(_T("%08X"), Config0);
	m_cfg0.SetWindowTextW(temp_str);
	//Watch clock power-down
	Config0 &= 0xBFFFFFFF;
	if (m_CWDTPDEN.GetCurSel() == 1)
		Config0 |= 0x40000000;
	else if (m_CWDTPDEN.GetCurSel() == 0)
		Config0 |= 0x00;
	temp_str.Format(_T("%08X"), Config0);
	m_cfg0.SetWindowTextW(temp_str);
	//XT1 Clock filter Enable
	Config0 &= 0xEFFFFFFF;
	if (m_CKF.GetCurSel() == 1)
		Config0 |= 0x10000000;
	else if (m_CKF.GetCurSel() == 0)
		Config0 |= 0x00;
	temp_str.Format(_T("%08X"), Config0);
	m_cfg0.SetWindowTextW(temp_str);
	//CPG[13:12] Multi-function
	Config0 &= 0xF7FFFFFF;
	if (m_CFGXT1.GetCurSel() == 1)
		Config0 |= 0x08000000;
	else if (m_CFGXT1.GetCurSel() == 0)
		Config0 |= 0x00;
	temp_str.Format(_T("%08X"), Config0);
	m_cfg0.SetWindowTextW(temp_str);
	//CPU Clock Source
	Config0 &= 0xF8FFFFFF;
	if (m_CFOSC.GetCurSel() == 1)
		Config0 |= 0x07000000;
	else if (m_CFOSC.GetCurSel() == 0)
		Config0 |= 0x00;
	temp_str.Format(_T("%08X"), Config0);
	m_cfg0.SetWindowTextW(temp_str);
	// Brown-out Detector
	Config0 &= 0xFF7FFFFF;
	if (m_CBODEN.GetCurSel() == 1)
		Config0 |= 0x800000;
	else if (m_CBODEN.GetCurSel() == 0)
		Config0 |= 0x00;
	temp_str.Format(_T("%08X"), Config0);
	m_cfg0.SetWindowTextW(temp_str);
	//Brown-out Voltage
	Config0 &= 0xFF9FFFFF;
	if (m_CBOV.GetCurSel() == 3)
		Config0 |= 0x600000;
	else if (m_CBOV.GetCurSel() == 2)
		Config0 |= 0x400000;
	else if (m_CBOV.GetCurSel() == 1)
		Config0 |= 0x200000;
	else if (m_CBOV.GetCurSel() == 0)
		Config0 |= 0x00;
	temp_str.Format(_T("%08X"), Config0);
	m_cfg0.SetWindowTextW(temp_str);
	//Brown-out Reset
	Config0 &= 0xFFEFFFFF;
	if (m_CBORST.GetCurSel() == 1)
		Config0 |= 0x100000;
	else if (m_CBORST.GetCurSel() == 0)
		Config0 |= 0x00;
	temp_str.Format(_T("%08X"), Config0);
	m_cfg0.SetWindowTextW(temp_str);
	//EMAC Interface
	Config0 &= 0xFFFF7FFF;
	if (m_RMII.GetCurSel() == 1)
		Config0 |= 0x8000;
	else if (m_RMII.GetCurSel() == 0)
		Config0 |= 0x00;
	temp_str.Format(_T("%08X"), Config0);
	m_cfg0.SetWindowTextW(temp_str);
	//CPG[15:14]Multi-Function
	Config0 &= 0xFFFFBFFF;
	if (m_CFG32K.GetCurSel() == 1)
		Config0 |= 0x4000;
	else if (m_CFG32K.GetCurSel() == 0)
		Config0 |= 0x00;
	temp_str.Format(_T("%08X"), Config0);
	m_cfg0.SetWindowTextW(temp_str);
	//LDROM Write protect
	Config0 &= 0xFFFFF7FF;
	if (m_LDWPEN.GetCurSel() == 1)
		Config0 |= 0x800;
	else if (m_LDWPEN.GetCurSel() == 0)
		Config0 |= 0x00;
	temp_str.Format(_T("%08X"), Config0);
	m_cfg0.SetWindowTextW(temp_str);
	//I/O Initial state
	Config0 &= 0xFFFFFBFF;
	if (m_CIOINI.GetCurSel() == 1)
		Config0 |= 0x400;
	else if (m_CIOINI.GetCurSel() == 0)
		Config0 |= 0x00;
	temp_str.Format(_T("%08X"), Config0);
	m_cfg0.SetWindowTextW(temp_str);
	//Chip Boot Select
	Config0 &= 0xFFFFFF3F;
	if (m_CBS.GetCurSel() == 3)
		Config0 |= 0xC0;
	else if (m_CBS.GetCurSel() == 2)
		Config0 |= 0x80;
	else if (m_CBS.GetCurSel() == 1)
		Config0 |= 0x40;
	else if (m_CBS.GetCurSel() == 0)
		Config0 |= 0x00;
	temp_str.Format(_T("%08X"), Config0);
	m_cfg0.SetWindowTextW(temp_str);
	//LOCK Mode
	Config0 &= 0xFFFFFFFD;
	if (m_LOCK.GetCurSel() == 1)
		Config0 |= 0x02;
	else if (m_LOCK.GetCurSel() == 0)
		Config0 |= 0x00;
	temp_str.Format(_T("%08X"), Config0);
	m_cfg0.SetWindowTextW(temp_str);
	//Data Flash
	OnBnClickedDFENCheck();
	//CFGChecksum
	Config3 = CFGCHECKSUM(Config0, Config1, Config2);
	temp_str.Format(_T("%08X"), Config3);
	m_cfg3.SetWindowTextW(temp_str);
}

void CDlg_NUC400::OnOK()
{
	CString datatemp_str;
	m_cfg0.GetWindowTextW(datatemp_str);
	Config0 = numeric_conversion::hexstring_to_size_t(datatemp_str.GetString());
	memcpy(m_RegBuff, &Config0, sizeof(Config0));

	m_cfg1.GetWindowTextW(datatemp_str);
	Config1 = numeric_conversion::hexstring_to_size_t(datatemp_str.GetString());
	memcpy(m_RegBuff+sizeof(Config0), &Config1, sizeof(Config1));

	m_cfg2.GetWindowTextW(datatemp_str);
	Config2 = numeric_conversion::hexstring_to_size_t(datatemp_str.GetString());
	memcpy(m_RegBuff + sizeof(Config0)+sizeof(Config1), &Config2, sizeof(Config2));

	m_cfg3.GetWindowTextW(datatemp_str);
	Config3 = numeric_conversion::hexstring_to_size_t(datatemp_str.GetString());
	memcpy(m_RegBuff + sizeof(Config0)+sizeof(Config1)+sizeof(Config2), &Config3, sizeof(Config3));

	*m_pDataLen = sizeof(Config0)+sizeof(Config1)+sizeof(Config2)+sizeof(Config3);

	CMFCPropertyPage::OnOK();
}

LRESULT CDlg_NUC400::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();
}
LRESULT CDlg_NUC400::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}

void CDlg_NUC400::OnBnClickedDFENCheck()
{
	CString temp_str;
	//Data Flash
	Config0 &= 0xFFFFFFFE;

	if (m_DFEN.GetCheck() == 1)
	{
		Config0 |= 0x00;
		m_dataAddr.EnableWindow(TRUE);
	}
	else if (m_DFEN.GetCheck() == 0)
	{
		Config0 |= 0x01;
		m_cfg1.SetWindowTextW(L"FFFFFFFF");
		m_dataAddr.SetWindowTextW(L"FFFFFFFF");
		m_dataAddr.EnableWindow(FALSE);
	}
	temp_str.Format(_T("%08X"), Config0);
	m_cfg0.SetWindowTextW(temp_str);
	//CFGChecksum
	Config3 = CFGCHECKSUM(Config0, Config1, Config2);
	temp_str.Format(_T("%08X"), Config3);
	m_cfg3.SetWindowTextW(temp_str);
}

void CDlg_NUC400::OnEnChangedataAddress()
{
	CString addr_str,temp_str;
	m_dataAddr.GetWindowTextW(addr_str);
	m_cfg1.SetWindowTextW(addr_str);
	Config1 = numeric_conversion::hexstring_to_size_t(addr_str.GetString());
	//CFGChecksum
	Config3 = CFGCHECKSUM(Config0, Config1, Config2);
	temp_str.Format(_T("%08X"), Config3);
	m_cfg3.SetWindowTextW(temp_str);
}

void CDlg_NUC400::OnEnChangeFlashBlockProtect()
{
	CString protect_str,temp_str;
	m_BWP.GetWindowTextW(protect_str);
	m_cfg2.SetWindowTextW(protect_str);
	Config2 = numeric_conversion::hexstring_to_size_t(protect_str.GetString());
	//CFGChecksum
	Config3 = CFGCHECKSUM(Config0, Config1, Config2);
	temp_str.Format(_T("%08X"), Config3);
	m_cfg3.SetWindowTextW(temp_str);
}


