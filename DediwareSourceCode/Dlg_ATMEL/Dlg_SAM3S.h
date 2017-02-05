#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_SAM3S dialog

class CDlg_SAM3S : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_SAM3S)

public:
	CDlg_SAM3S(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_SAM3S();

	// Dialog Data
	enum { IDD = IDD_SAM3S };
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
	
	unsigned int GPNVMbits;
	unsigned int Lockbits;

public :
	CButton Option_enable;
	CComboBox m_GPNVMb1;
	CComboBox m_GPNVMb0;
	CEdit m_Lockbits;
	CEdit m_GPNVMbits;
	afx_msg void OnEnChangeLockbits();
	afx_msg void OnBnClickedEnable();
	
};
