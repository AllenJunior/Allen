// ispMachXO2.cpp : implementation file
//

#include "stdafx.h"
#include "ispMachXO2.h"
#include "afxdialogex.h"
#include "numeric_conversion.h"
#include "../common/prog_info.h"

// ispMachXO2 dialog

IMPLEMENT_DYNAMIC(Dlg_ispMachXO2, CMFCPropertyPage)

Dlg_ispMachXO2::Dlg_ispMachXO2(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent /*=NULL*/)
: CMFCPropertyPage(Dlg_ispMachXO2::IDD),
m_ChipInfo(ChipInfo),
m_RegBuff(RegisterBuff),
m_BuffLen(BuffLen),
m_pDataLen(pDataLen)
{

}

Dlg_ispMachXO2::~Dlg_ispMachXO2()
{
}

void Dlg_ispMachXO2::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_FeatureRow);
	DDX_Control(pDX, IDC_EDIT2, m_FeatureBit);
	DDX_Control(pDX, IDC_EDIT3, m_usercode);
	DDX_Control(pDX, IDC_CHECK1, m_securityfuse);
}


BEGIN_MESSAGE_MAP(Dlg_ispMachXO2, CMFCPropertyPage)
END_MESSAGE_MAP()


// ispMachXO2 message handlers
BOOL Dlg_ispMachXO2::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	m_usercode.SetWindowTextW(L"FFFFFFFF");//4Bytes
	m_FeatureRow.SetWindowTextW(L"FFFFFFFFFFFFFFFF");//8Bytes
	m_FeatureBit.SetWindowTextW(L"FFFF");//2Bytes
	return TRUE;
}


BOOL Dlg_ispMachXO2::OnSetActive()
{
	CString usercode,FeatureRow,FeatureBit;
	if (*m_pDataLen != NULL)
	{
		//Feature Row
		int temprow[2];
		CString row;
		memset(temprow, 0, sizeof(temprow));
		memcpy(temprow, m_RegBuff, 8);
		row.Format(_T("%08X"), temprow[1]);
		FeatureRow += row;
		row.Format(_T("%08X"), temprow[0]);
		FeatureRow += row;
		GetDlgItem(IDC_EDIT1)->SetWindowTextW(FeatureRow);
		//FeatureBit
		memcpy((void*)&F_Bit, m_RegBuff + 8, 2);
		FeatureBit.Format(_T("%04X"), F_Bit);
		GetDlgItem(IDC_EDIT2)->SetWindowTextW(FeatureBit);
		//Usercode
		memcpy((void*)&U_Code, m_RegBuff + 12 ,4 );
		usercode.Format(_T("%08X"), U_Code);
		GetDlgItem(IDC_EDIT3)->SetWindowTextW(usercode);
		//Security Enable
		memcpy((void*)&S_En, m_RegBuff + 16, 4);
		if ((S_En & 0x00000001) == 0x00)
		{
			CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
			Encheck->SetCheck(0);
		}
		else if ((S_En & 0x00000001) == 0x01)
		{
			CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
			Encheck->SetCheck(1);
		}
	}

	return CMFCPropertyPage::OnSetActive();
}


void Dlg_ispMachXO2::OnOK()
{
	CString row, bit, code; 
	int temprow1 = 0;
	int temprow2 = 0;
	//FeatureRow
	m_FeatureRow.GetWindowTextW(row);
	//F_Row = numeric_conversion::hexstring_to_size_t(row.GetString());
	temprow2 = numeric_conversion::hexstring_to_size_t(row.GetString());
	row.Delete(8, 8);
	temprow1 = numeric_conversion::hexstring_to_size_t(row.GetString());
	memcpy(m_RegBuff, &temprow2, 4);
	memcpy(m_RegBuff + 4, &temprow1, 4);
	//FeatureBit
	m_FeatureBit.GetWindowTextW(bit);
	memset(&F_Bit, 0, 4);
	F_Bit = numeric_conversion::hexstring_to_size_t(bit.GetString());
	memcpy(m_RegBuff + 8, &F_Bit, 4);
	//usercode
	m_usercode.GetWindowTextW(code);
	U_Code = numeric_conversion::hexstring_to_size_t(code.GetString());
	memcpy(m_RegBuff + 12, &U_Code, 4);
	//security enable
	CButton* Encheck = (CButton*)GetDlgItem(IDC_CHECK1);
	int state = Encheck->GetCheck();
	if (state == 0x0)
	{
		S_En = 0x0;
		memcpy(m_RegBuff + 16, &S_En, 4);
	}
	else if (state == 0x1)
	{
		S_En = 0x1;

		memcpy(m_RegBuff + 16, &S_En, 4);
	}
	*m_pDataLen = sizeof(code)+sizeof(S_En)+sizeof(F_Row)+sizeof(F_Bit);
	CMFCPropertyPage::OnOK();
}


LRESULT Dlg_ispMachXO2::OnWizardNext()
{
	OnOK();
	return CMFCPropertyPage::OnWizardNext();

}
LRESULT Dlg_ispMachXO2::OnWizardBack()
{
	OnOK();
	return CMFCPropertyPage::OnWizardBack();
}
