#pragma once
#include "Resource.h"
#include "afxwin.h"
// Dlg_MC96F6632 dialog

class CDlg_MC96F6632 : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_MC96F6632)

public:
	CDlg_MC96F6632(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_MC96F6632();

// Dialog Data
	enum { IDD = IDD_MC96F6632 };
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

	unsigned char Option1;
	unsigned char Option2;
	unsigned int UC_En;
	unsigned int CRC;
	unsigned int UserID[2];
	CEdit m_Cfg0;
	CEdit m_Cfg1;
	CComboBox m_PAEN;
	CComboBox m_PASS;
	CComboBox m_RP;
	CComboBox m_HL;
	CButton m_EnUIDandCRC;
	CEdit m_UserID;
	CEdit m_CRC;
};
