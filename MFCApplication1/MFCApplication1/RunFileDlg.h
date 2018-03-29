#pragma once
#include "afxeditbrowsectrl.h"


// RunFileDlg dialog

class RunFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RunFileDlg)

public:
	RunFileDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~RunFileDlg();

	virtual void OnFinalRelease();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RUN_FILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

protected:

public:
	afx_msg void OnBnClickedOk();
protected:
	CString m_input_path;
	CString m_outfile_name;
public:
	afx_msg void OnEnChangeMfceditbrowse1();
	CMFCEditBrowseCtrl m_edit_browse;
	virtual BOOL OnInitDialog();
};
