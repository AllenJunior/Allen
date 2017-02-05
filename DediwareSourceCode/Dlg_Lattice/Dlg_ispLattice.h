#pragma once

#include "Resource.h"
#include "afxwin.h"
// Dlg_ispLattice dialog

class CDlg_ispLattice : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlg_ispLattice)

public:
	CDlg_ispLattice(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_ispLattice();

// Dialog Data
	enum { IDD = IDD_Lattice };
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
	CButton m_securityEnable;
	CEdit m_usercode;
	unsigned int Cfg0;
	unsigned int Cfg1;

};
