#pragma once
#include "Resource.h"
#include "afxwin.h"

// DLG_NUC1X0ACN dialog

class CDLG_NUC1X0ACN : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDLG_NUC1X0ACN)

public:
	CDLG_NUC1X0ACN(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLG_NUC1X0ACN();

// Dialog Data
	enum { IDD = IDD_DLG_NUC1X0ACN };
	struct chip_info_c * m_ChipInfo;
	unsigned char  *m_RegBuff;
	unsigned long   m_BuffLen;
	unsigned long  *m_pDataLen;
	unsigned int m_Len;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Cfg0;
	CEdit m_Cfg1;
	CComboBox m_CLKFilter;
	CComboBox m_CLKSource;
	CComboBox m_BroDect;
	CComboBox m_BroVol;
	CComboBox m_BroReset;
	CComboBox m_BootSelect;
	CComboBox m_Lock;
	virtual void OnOK();
	afx_msg void OnCbnSelchangeCombo();
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	virtual LRESULT OnWizardNext();
	virtual LRESULT OnWizardBack();
	unsigned int Val_Cfg0;
	unsigned int Val_Cfg1;
	afx_msg void OnBnClickedButton1();
//	afx_msg void OnEnChangeEdit1Addr();
	CComboBox m_DFlash;
	afx_msg void OnEnChangeEdit1Addr();
};
