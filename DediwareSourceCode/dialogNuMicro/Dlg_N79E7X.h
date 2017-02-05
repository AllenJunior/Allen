#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_N79E7X dialog

class CDlg_N79E7X : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_N79E7X)

public:
	CDlg_N79E7X(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);  // standard constructor
	virtual ~CDlg_N79E7X();

// Dialog Data
	enum { IDD = IDD_Dlg_N79EX };
	struct chip_info_c * m_ChipInfo;
	unsigned char  *m_RegBuff;
	unsigned long   m_BuffLen;
	unsigned long  *m_pDataLen;
	unsigned int m_Len;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	afx_msg void OnCbnSelchangeCombo();
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	virtual LRESULT OnWizardNext();
	virtual LRESULT OnWizardBack();
	unsigned char Val_Cfg0;
	unsigned char Val_Cfg1;
	unsigned char Val_Cfg2;
	unsigned char Val_Cfg3;
	unsigned char Val_Cfg4;


public:
	CEdit m_Cfg0;
	CEdit m_Cfg1;
	CEdit m_Cfg2;
	CEdit m_Cfg3;
	CEdit m_Cfg4;
	CComboBox m_CBS;
	CComboBox m_LOCK;
	CComboBox m_DFEN;
	CEdit m_addr;
	CComboBox m_CBODEN;
	CComboBox m_CBOV;
	CComboBox m_CBORST;
	CComboBox m_CWDTEN;
	CComboBox m_CKF;
	CComboBox m_OSCFS;
	CComboBox m_FOSC;
	CComboBox m_RSTDBE;
	CComboBox m_RSTDBS;
	CComboBox m_DIV2;
	afx_msg void OnEnChangeAddr();
};
