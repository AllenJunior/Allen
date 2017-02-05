#pragma once

#include "Resource.h"
#include "afxwin.h"
// DLG_M05X dialog

class CDLG_M05X : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDLG_M05X)

public:
	CDLG_M05X(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLG_M05X();

// Dialog Data
	enum { IDD = IDD_DLG_M05X };
	struct chip_info_c * m_ChipInfo;
	unsigned char  *m_RegBuff;
	unsigned long   m_BuffLen;
	unsigned long  *m_pDataLen;
	unsigned int m_Len;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	
	CComboBox m_CLKFilter;
	CComboBox m_CLKSource;
	CComboBox m_BroDect;
	CComboBox m_BroVol;
	CComboBox m_BroReset;
	CComboBox m_BootSelect;
	CComboBox m_Lock;
	CEdit m_Cfg0;
	virtual void OnOK();
	afx_msg void OnCbnSelchangeCombo();
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	virtual LRESULT OnWizardNext();
	virtual LRESULT OnWizardBack();
	unsigned int Val_Cfg0;
	afx_msg void OnBnClickedButton1();
};
