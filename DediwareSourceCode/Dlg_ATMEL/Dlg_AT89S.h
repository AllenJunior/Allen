#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_AT89S dialog

class CDlg_AT89S : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_AT89S)

public:
	CDlg_AT89S(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_AT89S();

// Dialog Data
	enum { IDD = IDD_AT89S };
	struct chip_info_c * m_ChipInfo;
	unsigned char  *m_RegBuff;
	unsigned long   m_BuffLen;
	unsigned long  *m_pDataLen;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual void OnOK();
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo();
	virtual LRESULT OnWizardNext();
	virtual LRESULT OnWizardBack();
	unsigned int lockbits;

public:
	CButton m_En;
	CComboBox m_LOCK;
	afx_msg void OnBnClickedEnable();
};
