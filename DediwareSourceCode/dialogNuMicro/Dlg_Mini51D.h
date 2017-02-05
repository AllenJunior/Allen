#pragma once

#include "Resource.h"
#include "afxwin.h"
// Dlg_Mini51D dialog

class CDlg_Mini51D : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_Mini51D)

public:
	CDlg_Mini51D(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);
	virtual ~CDlg_Mini51D();

// Dialog Data
	enum { IDD = IDD_Dlg_Mini51D };
	struct chip_info_c * m_ChipInfo;
	unsigned char  *m_RegBuff;
	unsigned long   m_BuffLen;
	unsigned long  *m_pDataLen;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual void OnOK();
	virtual LRESULT OnWizardNext();
	virtual LRESULT OnWizardBack();
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo();
	unsigned int Val_Cfg0;
	unsigned int Val_Cfg1;
	CEdit m_Cfg0;
	CEdit m_Cfg1;
	CComboBox m_CBOVEXT;
	CComboBox m_CBOV;
	CComboBox m_CBORST;
	CComboBox m_CIOINI;
	CComboBox m_CBS;
	CComboBox m_Lock;
	CComboBox m_DFEN;
	CEdit m_Addr;
	afx_msg void OnEnChangeEdit3();
};
