#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_Nano1X0AN dialog

class CDlg_Nano1X0AN : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_Nano1X0AN)

public:
	CDlg_Nano1X0AN(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);
	virtual ~CDlg_Nano1X0AN();
	struct chip_info_c * m_ChipInfo;
	unsigned char  *m_RegBuff;
	unsigned long   m_BuffLen;
	unsigned long  *m_pDataLen;
	unsigned int m_Len;
// Dialog Data
	enum { IDD = IDD_NANO1X0AN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	unsigned int Val_Cfg0;
	unsigned int Val_Cfg1;
	virtual void OnOK();
	virtual LRESULT OnWizardNext();
	virtual LRESULT OnWizardBack();
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo();
	CEdit m_Cfg0;
	CEdit m_Cfg1;
	CComboBox m_CBS;
	CComboBox m_CFOSC;
	CComboBox m_CBORST;
	CComboBox m_Lock;
	CComboBox m_DFEN;


	CEdit m_addr;
	afx_msg void OnEnChangeEdit3();
};
