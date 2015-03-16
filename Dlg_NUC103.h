#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_NUC103 dialog

class CDlg_NUC103 : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_NUC103)

public:
	CDlg_NUC103(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);
	virtual ~CDlg_NUC103();

// Dialog Data
	enum { IDD = IDD_Dlg_NUC103 };
	struct chip_info_c * m_ChipInfo;
	unsigned char  *m_RegBuff;
	unsigned long   m_BuffLen;
	unsigned long  *m_pDataLen;
	unsigned int m_Len;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombo();
	virtual void OnOK();
	virtual LRESULT OnWizardNext();
	virtual LRESULT OnWizardBack();
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	unsigned int Val_Cfg0;
	unsigned int Val_Cfg1;
	CEdit m_cfg0;
	CEdit m_cfg1;
	CComboBox m_lock;
	CComboBox m_ClkSource;
	CComboBox m_WDTEn;
	CComboBox m_WDTPD;
	CComboBox m_BroDet;
	CComboBox m_BroVolt;
	CComboBox m_BroRst;
	CComboBox m_boot;
	CComboBox m_DataFlash;
	afx_msg void OnEnChangeEdit3();
};
