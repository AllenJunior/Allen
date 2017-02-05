#pragma once
#include "Resource.h"
#include "afxwin.h"

// DLG_M0518 dialog

class CDLG_M0518 : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDLG_M0518)

public:
	CDLG_M0518(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);
	virtual ~CDLG_M0518();

// Dialog Data
	enum { IDD = IDD_DLG_M0518 };
	struct chip_info_c * m_ChipInfo;
	unsigned char  *m_RegBuff;
	unsigned long   m_BuffLen;
	unsigned long  *m_pDataLen;
	unsigned int m_Len;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombo();
	afx_msg void OnEnChangeEdit3();
	virtual void OnOK();
	virtual LRESULT OnWizardNext();
	virtual LRESULT OnWizardBack();
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	CComboBox m_WDTEN;
	CComboBox m_WDTPDEN;
	CComboBox m_GPFMFP;
	CComboBox m_OSC;
	CComboBox m_BroDet;
	CComboBox m_BroVol;
	CComboBox m_BroRst;
	CComboBox m_IOinit;
	CComboBox m_Boot;
	CComboBox m_lock;
	CComboBox m_DataFlashEN;
	CEdit m_cfg0;
	CEdit m_cfg1;
	unsigned int Val_Cfg0;
	unsigned int Val_Cfg1;
	afx_msg void OnBnClickedButton1();
};
