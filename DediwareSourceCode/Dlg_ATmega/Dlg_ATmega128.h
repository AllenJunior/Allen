#pragma once
#include "Resource.h"
#include "afxwin.h"
// Dlg_ATmega128 dialog

class CDlg_ATmega128 : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_ATmega128)

public:
	CDlg_ATmega128(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_ATmega128();

// Dialog Data
	enum { IDD = IDD_ATmega128 };
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
	CComboBox m_M103C;
	CComboBox m_WDTON;
	CComboBox m_OCDEN;
	CComboBox m_JTAGEN;
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
	CEdit m_ExFuse;
	CEdit m_HFuse;
	CEdit m_LFuse;
	CEdit m_Lockbits;
};
