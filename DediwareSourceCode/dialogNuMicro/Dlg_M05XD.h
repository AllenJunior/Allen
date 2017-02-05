#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_M05XD dialog

class CDlg_M05XD : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_M05XD)

public:
	CDlg_M05XD(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_M05XD();

// Dialog Data
	enum { IDD = IDD_DLG_M05XD };
	struct chip_info_c * m_ChipInfo;
	unsigned char  *m_RegBuff;
	unsigned long   m_BuffLen;
	unsigned long  *m_pDataLen;
	unsigned int m_Len;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	unsigned int Val_Cfg0;
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	afx_msg void OnCbnSelchangeCombo();
	virtual LRESULT OnWizardNext();
	virtual LRESULT OnWizardBack();
	virtual void OnOK();

	CEdit m_Cfg0;
	CComboBox m_CWDTEN;
	CComboBox m_CWDTPDEN;
	CComboBox m_CFOSC;
	CComboBox m_CBODEN;
	CComboBox m_CBOV;
	CComboBox m_CBORST;
	CComboBox m_CIOINI;
	CComboBox m_CBS;
	CComboBox m_Lock;
};
