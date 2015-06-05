#pragma once

#include "Resource.h"
#include "afxwin.h"
// Dlg_Atxmega_A3B dialog

class CDlg_Atxmega_A3B : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_Atxmega_A3B)

public:
	CDlg_Atxmega_A3B(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_Atxmega_A3B();

// Dialog Data
	enum { IDD = IDD_ATxmega };
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
	afx_msg void OnCbnSelchangeCombo();
	unsigned char Fuse0;
	unsigned char Fuse1;
	unsigned char Fuse2;
	unsigned char Fuse4;
	unsigned char Fuse5;
	unsigned char Lockbits;

	CEdit m_JtagUID;
	CComboBox m_WDWPER;
	CComboBox m_WDPER;
	CComboBox m_BootRst;
	CComboBox m_TOSCSel;
	CComboBox m_BODPD;
	CComboBox m_RSTDISBL;
	CComboBox m_StartupTime;
	CComboBox m_WDLOCK;
	CComboBox m_JTAGEN;
	CComboBox m_BODACT;
	CComboBox m_EESAVE;
	CComboBox m_BODLevel;
	CComboBox m_BLBB;
	CComboBox m_BLBA;
	CComboBox m_BLBAT;
	CComboBox m_LB;
	CEdit m_Cfg0;
	CEdit m_Cfg1;
	CEdit m_Cfg2;
	CEdit m_Cfg4;
	CEdit m_Cfg5;
	CEdit m_Cfg6;
	afx_msg void OnEnChangeJTAGUID();
	CButton Option_Enable;
	afx_msg void OnBnClickedEnable();
};
