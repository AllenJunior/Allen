#pragma once

#include "Resource.h"
#include "afxwin.h"
// ispMachXO2 dialog

class Dlg_ispMachXO2 : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(Dlg_ispMachXO2)

public:
	Dlg_ispMachXO2(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~Dlg_ispMachXO2();

// Dialog Data
	enum { IDD = IDD_LCMXO2 };
	struct chip_info_c * m_ChipInfo;
	unsigned char  *m_RegBuff;
	unsigned long   m_BuffLen;
	unsigned long  *m_pDataLen;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	virtual LRESULT OnWizardNext();
	virtual LRESULT OnWizardBack();
	CEdit m_FeatureRow;
	CEdit m_FeatureBit;
	CEdit m_usercode;
	CButton m_securityfuse;
	unsigned long long F_Row;
	unsigned short int F_Bit;
	unsigned int U_Code;
	unsigned int S_En;
};
