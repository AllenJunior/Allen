#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_ATmega8 dialog

class CDlg_ATmega8 : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_ATmega8)

public:
	CDlg_ATmega8(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_ATmega8();

// Dialog Data
	enum { IDD = IDD_ATmega8 };
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


	CButton Option_enable;
	CComboBox m_RSTDISBL;
	CComboBox m_WDTON;
	CComboBox m_SPIEN;
	CComboBox m_CKOPT;
	CComboBox m_EESAVE;
	CComboBox m_BOOTSZ;
	CComboBox m_BOOTRST;
	CComboBox m_BODLevel;
	CComboBox m_BODEN;
	CComboBox m_CKSEL;
	CComboBox m_BLBB;
	CComboBox m_BLBA;
	CComboBox m_LOCK;
	CEdit m_HFuse;
	CEdit m_LFuse;
	CEdit m_Lockbits;
	
};
