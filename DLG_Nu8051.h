#pragma once
#include "Resource.h"
#include "afxwin.h"

// DLG_Nu8051 dialog

class CDLG_Nu8051 : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDLG_Nu8051)

public:
	CDLG_Nu8051(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);  // standard constructor
	virtual ~CDLG_Nu8051();

// Dialog Data
	enum { IDD = IDD_DLG_Nu8051 };
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
	CEdit m_Cfg2;
	CEdit m_Cfg3;
	CComboBox m_Lock;
	CComboBox m_BootSelect;
	CComboBox m_BroVol;
	CComboBox m_BroReset;
	CComboBox m_BroDect;
	CComboBox m_CLKFilter;
	CComboBox m_CLKSource;
	CComboBox m_watchdog;
	CComboBox m_InternalRC;
	CComboBox m_DataFlash;
	virtual void OnOK();
	afx_msg void OnCbnSelchangeCombo();
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	virtual LRESULT OnWizardNext();
	virtual LRESULT OnWizardBack();
	unsigned int Val_Cfg0;
	unsigned int Val_Cfg1;
	unsigned int Val_Cfg2;
	unsigned int Val_Cfg3;
	afx_msg void OnEnChangeEdit5();
};
