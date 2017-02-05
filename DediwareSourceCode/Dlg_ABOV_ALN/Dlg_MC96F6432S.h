#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_MC96F6432S dialog

class CDlg_MC96F6432S : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_MC96F6432S)

public:
	CDlg_MC96F6432S(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_MC96F6432S();

// Dialog Data
	enum { IDD = IDD_MC96F6432S };
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

	unsigned char Cfg1;	// addr : 0x3E
	unsigned char Cfg2;	// addr : 0x3F
	unsigned int checksum; // addr : 0x16 ~ 0x17
	unsigned int crc_check;
	unsigned int id_check;

	CComboBox m_PAEN;
	CComboBox m_PASS;
	CEdit m_CFG1;
	CComboBox m_RP;
	CComboBox m_HL;
	CComboBox m_ENRST;
	CEdit m_CFG2;
	CEdit m_checksum;
	CButton ID_enable;
	CButton crc_enable;
	CEdit m_userid;
};
