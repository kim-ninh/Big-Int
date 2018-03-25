// RunFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "RunFileDlg.h"
#include "Test.h"
#include "afxdialogex.h"
#include <string>


// RunFileDlg dialog

IMPLEMENT_DYNAMIC(RunFileDlg, CDialogEx)

RunFileDlg::RunFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_RUN_FILE)
	, m_input_path(_T(""))
	, m_outfile_name(_T(""))
{

	EnableAutomation();

}

RunFileDlg::~RunFileDlg()
{
}

void RunFileDlg::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialogEx::OnFinalRelease();
}

void RunFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MFCEDITBROWSE1, m_input_path);
	DDX_Text(pDX, IDC_EDIT1, m_outfile_name);
}


BEGIN_MESSAGE_MAP(RunFileDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &RunFileDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(RunFileDlg, CDialogEx)
END_DISPATCH_MAP()

// Note: we add support for IID_IRunFileDlg to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {3ACEEC8D-A131-4DC6-A46D-472C19DF28F3}
static const IID IID_IRunFileDlg =
{ 0x3ACEEC8D, 0xA131, 0x4DC6, { 0xA4, 0x6D, 0x47, 0x2C, 0x19, 0xDF, 0x28, 0xF3 } };

BEGIN_INTERFACE_MAP(RunFileDlg, CDialogEx)
	INTERFACE_PART(RunFileDlg, IID_IRunFileDlg, Dispatch)
END_INTERFACE_MAP()


// RunFileDlg message handlers


void RunFileDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	std::wifstream infile{std::wstring(m_input_path)};
	std::wofstream outfile{std::wstring(m_outfile_name)};

	Test(infile, outfile);


	CDialogEx::OnOK();
}