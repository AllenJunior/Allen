#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_MC96F6432A dialog

class CDlg_MC96F6432A : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_MC96F6432A)

public:
	CDlg_MC96F6432A(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_MC96F6432A();

// Dialog Data
	enum { IDD = IDD_MC96F6432A };
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
	unsigned char Cfg1;	// addr : 0x3E
	unsigned char Cfg2;	// addr : 0x3F

	CComboBox m_PAEN;
	CComboBox m_PASS;
	CComboBox m_RP;
	CComboBox m_HL;
	CComboBox m_VAPEN;
	CComboBox m_RSTS;
	CEdit m_CFG1;
	CEdit m_CFG2;
};
