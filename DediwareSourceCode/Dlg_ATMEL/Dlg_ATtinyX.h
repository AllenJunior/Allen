#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_ATtinyX dialog

class CDlg_ATtinyX : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_ATtinyX)

public:
	CDlg_ATtinyX(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_ATtinyX();

// Dialog Data
	enum { IDD = IDD_Dlg_ATtinyX };
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
	afx_msg void OnBnClickedEnable();
	unsigned char Fuse;
	unsigned char Lockbits;
	CComboBox m_Lockbits;
	CComboBox m_CKOUT;
	CComboBox m_WDTON;
	CComboBox m_RSTDISBL;
	CButton Option_Enable;
	CEdit m_Cfg0;
	CEdit m_Cfg1;
	
	CComboBox m_SELFPROGEN;
};
