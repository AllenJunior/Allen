#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_MC96FC664A dialog

class CDlg_MC96FC664A : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_MC96FC664A)

public:
	CDlg_MC96FC664A(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_MC96FC664A();

// Dialog Data
	enum { IDD = IDD_MC96FC664A };
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


	unsigned int Option0;

	CComboBox m_BSize;
	CComboBox m_SXIEN;
	CComboBox m_XIENA;
	CComboBox m_LockB;
	CComboBox m_LockP;
	CEdit m_CFG0;

};
