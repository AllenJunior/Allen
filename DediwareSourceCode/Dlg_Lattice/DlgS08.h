#pragma once
#include "Resource.h"
#include "afxwin.h"
// DlgS08 dialog

class DlgS08 : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(DlgS08)

public:
	DlgS08(struct chip_info_c * ChipInfo, unsigned char *RegisterBuff, unsigned long BuffLen, unsigned long *pDataLen, CWnd* pParent = NULL);
	virtual ~DlgS08();

// Dialog Data
	enum { IDD = IDD_DIALOG_S08 };

	struct chip_info_c * m_ChipInfo;
	unsigned char  *m_RegBuff;
	unsigned long   m_BuffLen;
	unsigned long  *m_pDataLen;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CButton m_Protect;
	CButton m_Trim;
	virtual void OnOK();

	typedef struct{
		unsigned int EnOption;
		unsigned int Trim1_Value;
		unsigned int Trim2_Value;
	}OPTION;

	virtual LRESULT OnWizardNext();
	virtual LRESULT OnWizardBack();
	virtual BOOL OnSetActive();
	unsigned int m_Len;
};
