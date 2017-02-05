#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_ATSAMD dialog

class CDlg_ATSAMD : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_ATSAMD)

public:
	CDlg_ATSAMD(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_ATSAMD();

// Dialog Data
	enum { IDD = IDD_SAMDXX };
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
	unsigned int user_word_0;
	unsigned int user_word_1;
	unsigned int security;
	unsigned short int Lockbits;
public:
	CButton Option_enable;
	CButton Security_enable;
	CEdit m_UW0;
	CEdit m_UW1;
	CEdit m_LOCKbits;
	CComboBox m_BOOTPROT;
	CComboBox m_EEPROM;
	CComboBox m_BODLEVEL;
	CComboBox m_BOD33Enable;
	CComboBox m_BODAction;
	CComboBox m_BODhyst;
	CComboBox m_WDTEnable;
	CComboBox m_WDTOn;
	CComboBox m_WDTPeriod;
	CComboBox m_WDTWindow;
	CComboBox m_WDTEWOFFSET;
	CComboBox m_WDTWEN;
	afx_msg void OnEnChangeLockbits();
	afx_msg void OnBnClickedEnable();
	afx_msg void OnBnClickedKeyIn();
	afx_msg void OnBnClickedSet();
};
