#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_Nano102 dialog

class CDlg_Nano102 : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_Nano102)

public:
	CDlg_Nano102(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);
	virtual ~CDlg_Nano102();
	struct chip_info_c * m_ChipInfo;
	unsigned char  *m_RegBuff;
	unsigned long   m_BuffLen;
	unsigned long  *m_pDataLen;
	unsigned int m_Len;

// Dialog Data
	enum { IDD = IDD_Dlg_Nano102 };

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


	CEdit m_cfg0;
	CEdit m_cfg1;
	CComboBox m_CFOSC;
	CComboBox m_CBORST;
	CComboBox m_CHXTGAIN;
	CComboBox m_CBS;
	CComboBox m_LOCK;
	CComboBox m_DFEN;
	CEdit m_Addr;
	afx_msg void OnEnChangeAddr();
	CComboBox m_WDTEN;
};
