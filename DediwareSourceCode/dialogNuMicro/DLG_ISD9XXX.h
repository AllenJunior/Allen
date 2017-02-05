#pragma once
#include "Resource.h"
#include "afxwin.h"

// DLG_ISD9XXX dialog

class CDLG_ISD9XXX : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDLG_ISD9XXX)

public:
	CDLG_ISD9XXX(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);
	virtual ~CDLG_ISD9XXX();

// Dialog Data
	enum { IDD = IDD_ISD9XXX };
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
	CComboBox m_BroDet;
	CComboBox m_boot;
	CComboBox m_LDEn;
	CComboBox m_lock;
	CComboBox m_DataFlash;
	virtual void OnOK();
	virtual LRESULT OnWizardNext();
	virtual LRESULT OnWizardBack();
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	unsigned int Val_Cfg0;
	unsigned int Val_Cfg1;
	afx_msg void OnCbnSelchangeCombo();
	afx_msg void OnEnChangeEdit3();
};
