#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_N78E517 dialog

class CDlg_N78E517 : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_N78E517)

public:
	CDlg_N78E517(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);  // standard constructor
	virtual ~CDlg_N78E517();

// Dialog Data
	enum { IDD = IDD_DLG_N78 };
	struct chip_info_c * m_ChipInfo;
	unsigned char  *m_RegBuff;
	unsigned long   m_BuffLen;
	unsigned long  *m_pDataLen;
	unsigned int m_Len;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	unsigned char Val_Cfg0;
	unsigned char Val_Cfg1;
	unsigned char Val_Cfg2;
	unsigned char Val_Cfg3;
	virtual void OnOK();
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	virtual LRESULT OnWizardNext();
	virtual LRESULT OnWizardBack();
	afx_msg void OnCbnSelchangeCombo();
	CEdit m_cfg0;
	CEdit m_cfg1;
	CEdit m_cfg2;
	CEdit m_cfg3;
	CComboBox m_boot;
	CComboBox m_lock;
	CComboBox m_DataFlash;
	CComboBox m_BroDet;
	CComboBox m_BroVolt;
	CComboBox m_BroRst;
	CComboBox m_WTEn;
	CComboBox m_6TMEn;
	CComboBox m_Extal;
	CComboBox m_ClkFilter;
	CComboBox m_ClkSource;
	CComboBox m_Osc;
	afx_msg void OnEnChangeEdit5();
	
};
