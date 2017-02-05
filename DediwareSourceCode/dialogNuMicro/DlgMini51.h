#pragma once

#include "Resource.h"
#include "afxwin.h"
// CDlgMini51 dialog

class CDlgMini51 : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlgMini51)

public:
	CDlgMini51(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);
	virtual ~CDlgMini51();

// Dialog Data
	enum { IDD = IDD_DLG_MINI51 };

	struct chip_info_c * m_ChipInfo;
	unsigned char  *m_RegBuff;
	unsigned long   m_BuffLen;
	unsigned long  *m_pDataLen;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_Lock;
	CComboBox m_Boot;
	CComboBox m_Clock;
	CComboBox m_Borv;
	CComboBox m_BorReset;
	CComboBox m_DFlash;
	CEdit m_Cfg0;
	CEdit m_Cfg1;
	afx_msg void OnCloseupCombo();
	virtual void OnOK();
	virtual LRESULT OnWizardNext();
	virtual LRESULT OnWizardBack();
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	afx_msg void OnDoubleclicked();
	unsigned int Val_Cfg0;
	unsigned int Val_Cfg1;
	unsigned int m_Len;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnSelchangeCombo();
	afx_msg void OnEnChangeEdit1Addr();
	
};
