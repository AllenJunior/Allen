#pragma once
#include "Resource.h"
#include "afxwin.h"

// Dlg_ATxmegaBCD dialog

class CDlg_ATxmegaBCD : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_ATxmegaBCD)

public:
	CDlg_ATxmegaBCD(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_ATxmegaBCD();

// Dialog Data
	enum { IDD = IDD_ATxmegaBCD };
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

	unsigned char Fuse1;
	unsigned char Fuse2;
	unsigned char Fuse4;
	unsigned char Fuse5;
	unsigned char Lockbits;
	

	CComboBox m_WDWPER;
	CComboBox m_WDPER;
	CComboBox m_BootRst;
	CComboBox m_TOSCSEL;
	CComboBox m_BODPD;
	CComboBox m_RSTDISBL;
	CComboBox m_Startuptime;
	CComboBox m_WDLOCK;
	CComboBox m_BODACT;
	CComboBox m_EESAVE;
	CComboBox m_BODLEVEL;
	CComboBox m_BLBB;
	CComboBox m_BLBA;
	CComboBox m_BLBAT;
	CComboBox m_LB;
	CButton Option_Enable;
	CEdit m_Cfg1;
	CEdit m_Cfg2;
	CEdit m_Cfg4;
	CEdit m_Cfg5;
	CEdit m_Cfg6;
};
