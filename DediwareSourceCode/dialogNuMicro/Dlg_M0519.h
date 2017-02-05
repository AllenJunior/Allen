#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_M0519 dialog

class CDlg_M0519 : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_M0519)

public:
	CDlg_M0519(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);
	virtual ~CDlg_M0519();

// Dialog Data
	enum { IDD = IDD_Dlg_M0519 };
	struct chip_info_c * m_ChipInfo;
	unsigned char  *m_RegBuff;
	unsigned long   m_BuffLen;
	unsigned long  *m_pDataLen;
	unsigned int	m_Len;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual LRESULT OnWizardNext();
	virtual LRESULT OnWizardBack();
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCbnSelchangeCombo();
	afx_msg void OnEnChangeAddr();
	unsigned int Val_Cfg0;
	unsigned int Val_Cfg1;


	CEdit m_cfg0;
	CEdit m_cfg1;
	CComboBox m_CWDTEN;
	CComboBox m_CWDTPDEN;
	CComboBox m_CFOSC;
	CComboBox m_CBODEN;
	CComboBox m_CBOV;
	CComboBox m_CBORST;
	CComboBox m_CHZ_BPWM;
	CComboBox m_CHZ_Odd1;
	CComboBox m_CHZ_Even1;
	CComboBox m_CHZ_Odd0;
	CComboBox m_CHZ_Even0;
	CComboBox m_CBS;
	CComboBox m_LOCK;
	CComboBox m_DFEN;
	CEdit m_Addr;
	
};
