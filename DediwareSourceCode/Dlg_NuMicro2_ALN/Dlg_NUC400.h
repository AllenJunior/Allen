#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_NUC400 dialog

class CDlg_NUC400 : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_NUC400)

public:
	CDlg_NUC400(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_NUC400();

// Dialog Data
	enum { IDD = IDD_NUC400 };
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

	unsigned int Config0;
	unsigned int Config1;
	unsigned int Config2;
	unsigned int Config3;

	CEdit m_cfg0;
	CEdit m_cfg1;
	CEdit m_cfg2;
	CEdit m_cfg3;
	CComboBox m_CWDTEN;
	CComboBox m_CWDTPDEN;
	CComboBox m_CKF;
	CComboBox m_CFGXT1;
	CComboBox m_CFOSC;
	CComboBox m_CBODEN;
	CComboBox m_CBOV;
	CComboBox m_CBORST;
	CComboBox m_RMII;
	CComboBox m_CFG32K;
	CComboBox m_LDWPEN;
	CComboBox m_CIOINI;
	CComboBox m_CBS;
	CComboBox m_LOCK;
	CEdit m_dataAddr;
	CButton m_DFEN;
	CEdit m_BWP;
	afx_msg void OnCbnSelchangeCombo();
	afx_msg void OnEnChangedataAddress();
	afx_msg void OnBnClickedDFENCheck();
	afx_msg void OnEnChangeFlashBlockProtect();
};
