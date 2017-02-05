#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_Mini58 dialog

class CDlg_Mini58 : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_Mini58)

public:
	CDlg_Mini58(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);
	virtual ~CDlg_Mini58();

// Dialog Data
	enum { IDD = IDD_Dlg_MINI58 };
	struct chip_info_c * m_ChipInfo;
	unsigned char  *m_RegBuff;
	unsigned long   m_BuffLen;
	unsigned long  *m_pDataLen;
	unsigned int m_Len;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual LRESULT OnWizardNext();
	virtual LRESULT OnWizardBack();
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	unsigned int Val_Cfg0;
	unsigned int Val_Cfg1;
	afx_msg void OnCbnSelchangeCombo();


	CEdit m_Cfg0;
	CEdit m_Cfg1;
	CComboBox m_CBOVEXT;
	CComboBox m_CBOV;
	CComboBox m_CBORST;
	CComboBox m_CIOINI;
	CComboBox m_CBS;
	CComboBox m_Lock;
	CComboBox m_DFEN;
	CEdit m_addr;
	afx_msg void OnEnChangeaddr();
};
