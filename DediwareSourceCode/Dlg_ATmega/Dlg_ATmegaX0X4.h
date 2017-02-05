#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_ATmegaX0X4 dialog

class CDlg_ATmegaX0X4 : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_ATmegaX0X4)

public:
	CDlg_ATmegaX0X4(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_ATmegaX0X4();

// Dialog Data
	enum { IDD = IDD_ATmegaX0X4 };
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

	unsigned char ExtendedFuse;
	unsigned char HighFuse;
	unsigned char LowFuse;
	unsigned char Lockbits;

	CButton Option_enable;
	CComboBox m_BODLevel;
	CComboBox m_OCDEN;
	CComboBox m_JTAGEN;
	CComboBox m_SPIEN;
	CComboBox m_WDTON;
	CComboBox m_EESAVE;
	CComboBox m_BOOTSZ;
	CComboBox m_BOOTRST;
	CComboBox m_CKDIV8;
	CComboBox m_CKOUT;
	CComboBox m_CKSEL;
	CComboBox m_BLBB;
	CComboBox m_BLBA;
	CComboBox m_LOCK;
	CEdit m_ExFuse;
	CEdit m_HFuse;
	CEdit m_LFuse;
	CEdit m_Lockbits;
};
