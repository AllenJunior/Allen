#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_M451AE dialog

class CDlg_M451AE : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_M451AE)

public:
	CDlg_M451AE(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);
	virtual ~CDlg_M451AE();

// Dialog Data
	enum { IDD = IDD_Dlg_M451AE };
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
	afx_msg void OnEnChangeAddr();
	unsigned int Val_Cfg0;
	unsigned int Val_Cfg1;
	CEdit m_Cfg0;
	CEdit m_Cfg1;
	CEdit m_Addr;
	CComboBox m_CWDTEN;
	CComboBox m_CWDTPDEN;
	CComboBox m_CFGXT1;
	CComboBox m_CFOSC;
	CComboBox m_CBODEN;
	CComboBox m_CBOV;
	CComboBox m_CBORST;
	CComboBox m_CIOINI;
	CComboBox m_CBS;
	CComboBox m_MBS;
	CComboBox m_LOCK;
	CComboBox m_DFEN;
	
};
