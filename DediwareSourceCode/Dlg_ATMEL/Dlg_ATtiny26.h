#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_ATtiny26 dialog

class CDlg_ATtiny26 : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_ATtiny26)

public:
public:
	CDlg_ATtiny26(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_ATtiny26();

// Dialog Data
	enum { IDD = IDD_ATtiny26 };
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
	unsigned char HighFuse;
	unsigned char LowFuse;
	unsigned char Lockbits;
	unsigned char fixint;


	CButton option_enable;
	CComboBox m_RSTDISBL;
	CComboBox m_SPIEN;
	CComboBox m_EESAVE;
	CComboBox m_BODLevel;
	CComboBox m_BODEN;
	CComboBox m_CKOPT;
	CComboBox m_CKSEL;
	CComboBox m_Lock;
	CEdit m_HFuse;
	CEdit m_LFuse;
	CEdit m_Lockbits;
};
