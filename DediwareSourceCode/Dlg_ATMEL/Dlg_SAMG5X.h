#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_SAMG5X dialog

class CDlg_SAMG5X : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_SAMG5X)

public:
	CDlg_SAMG5X(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_SAMG5X();

// Dialog Data
	enum { IDD = IDD_SAMG5X };
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

	unsigned int GPNVM;
	unsigned int LockWord_0;
	unsigned int LockWord_1;

public:
	CButton option_enable;
	CEdit m_GPNVM;
	CComboBox m_GPNVMb1;
	CComboBox m_GPNVMb0;
	CEdit m_LW0;
	CEdit m_LW1;
	afx_msg void OnBnClickedEnable();
	afx_msg void OnEnChangeLockword_0();
	afx_msg void OnEnChangeLockword_1();

};
