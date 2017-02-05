#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_A8XXX dialog

class CDlg_A8XXX : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_A8XXX)

public:
	CDlg_A8XXX(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_A8XXX();

// Dialog Data
	enum { IDD = IDD_A8XXX };
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
	unsigned char checkbox;
	unsigned char Bank;
	unsigned short int Addr;
	unsigned short int Checksum;

	CButton option_enable;
	CEdit m_Bank;
	CEdit m_Addr;
	CEdit m_Checksum;
	CButton m_Refresh;

	afx_msg void OnBnClickedRefresh();
	afx_msg void OnEnChangeBank();
	afx_msg void OnEnChangeAddr();
	afx_msg void OnEnChangeChecksum();
	afx_msg void OnBnClickedCheckEnable();
	
};
