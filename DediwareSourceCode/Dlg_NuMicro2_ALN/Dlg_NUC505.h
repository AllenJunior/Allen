#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_NUC505 dialog

class CDlg_NUC505 : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_NUC505)

public:
	CDlg_NUC505(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_NUC505();

// Dialog Data
	enum { IDD = IDD_NUC505 };
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
	
	unsigned int MTPKey0;
	unsigned int MTPKey1;
	unsigned int MTPSignature;
	unsigned int MTPOffsetAddr;
	unsigned int MTPOption;
	unsigned int MTPLock;

	CButton MTP_Enable;
	CEdit m_MTPKey0;
	CEdit m_MTPKey1;
	CEdit m_MTPSignature;
	CEdit m_MTPOffsetAddr;
	CEdit m_MTPOption;
	CButton m_MTPLock;

	afx_msg void OnEnChangeMTPKey0();
	afx_msg void OnEnChangeMTPKey1();
	afx_msg void OnEnChangeMTPSignature();
	afx_msg void OnEnChangeMTPoffsetAddr();
	afx_msg void OnEnChangeMTPOption();
	afx_msg void OnBnClickedMTPEnable();
	afx_msg void OnBnClickedMTPLockEnable();
};
