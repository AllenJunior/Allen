#pragma once

#include "Resource.h"
#include "afxwin.h"

// Dlg_iCE40 dialog

class CDlg_iCE40 : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_iCE40)

public:
	CDlg_iCE40(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_iCE40();

// Dialog Data
	enum { IDD = IDD_iCE40 };
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
	unsigned int Security;
	CButton m_SecurityEnable;
};
