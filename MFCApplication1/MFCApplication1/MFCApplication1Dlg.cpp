
// MFCApplication1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

CFont CalculatorDlg::m_font;		// lưu trạng thái font dùng cho edit box
bool CalculatorDlg::m_calculated;	
// ánh xạ từ ký tự sang các toán tử
std::map<char, BigInt(*)(const BigInt&, const BigInt&)> CalculatorDlg::m_char_to_opr =
		{ {'+', &operator+}, 
		  {'-', &operator-},
		  {'*', &operator*},
		  {'/', &operator/},
		  {'%', &operator%},
		  {'&', &operator&},
		  {'|', &operator|},
		  {'^', &operator^}};
std::stack<char> CalculatorDlg::m_opr;		// lưu các ký tự của các toán tử và các dấu ngoặc
std::queue<char> CalculatorDlg::m_posFix;			// lưu chuỗi biểu thức dạng hậu tố
std::vector<BigInt> CalculatorDlg::m_operands;			// lưu các toán hạng


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CalculatorDlg dialog


CalculatorDlg::CalculatorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
	, m_text(_T("0"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_value = 0;
	m_lhs = m_rhs = 0;
	m_base = DECIMAL;
	m_calculated = false;
	m_prev = NONE;

	m_font.CreateFontW(25, 0, 0, 0, 500, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS
		, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
}

void CalculatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_text);
	DDV_MaxChars(pDX, m_text, 128);
	DDX_Control(pDX, IDC_RADIO_DEC, m_decButton);
}

BEGIN_MESSAGE_MAP(CalculatorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT2, &CalculatorDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON_C, &CalculatorDlg::OnBnClickedButtonC)
	ON_BN_CLICKED(IDC_BUTTON_NUMBER_1, &CalculatorDlg::OnBnClickedButtonNumber1)
	ON_BN_CLICKED(IDC_BUTTON_NUMBER_2, &CalculatorDlg::OnBnClickedButtonNumber2)
	ON_BN_CLICKED(IDC_BUTTON_NUMBER_3, &CalculatorDlg::OnBnClickedButtonNumber3)
	ON_BN_CLICKED(IDC_BUTTON_NUMBER_4, &CalculatorDlg::OnBnClickedButtonNumber4)
	ON_BN_CLICKED(IDC_BUTTON_NUMBER_5, &CalculatorDlg::OnBnClickedButtonNumber5)
	ON_BN_CLICKED(IDC_BUTTON_NUMBER_6, &CalculatorDlg::OnBnClickedButtonNumber6)
	ON_BN_CLICKED(IDC_BUTTON_NUMBER_7, &CalculatorDlg::OnBnClickedButtonNumber7)
	ON_BN_CLICKED(IDC_BUTTON_NUMBER_8, &CalculatorDlg::OnBnClickedButtonNumber8)
	ON_BN_CLICKED(IDC_BUTTON_NUMBER_9, &CalculatorDlg::OnBnClickedButtonNumber9)
	ON_BN_CLICKED(IDC_BUTTON_NUMBER_0, &CalculatorDlg::OnBnClickedButtonNumber0)
	ON_BN_CLICKED(IDC_RADIO_BIN, &CalculatorDlg::OnBnClickedRadioBin)
	ON_BN_CLICKED(IDC_RADIO_HEX, &CalculatorDlg::OnBnClickedRadioHex)
	ON_BN_CLICKED(IDC_BUTTON_SUBTRACT, &CalculatorDlg::OnBnClickedButtonSubtract)
	ON_BN_CLICKED(IDC_BUTTON_EQUAL, &CalculatorDlg::OnBnClickedButtonEqual)
	ON_BN_CLICKED(IDC_BUTTON_MULTIPLY, &CalculatorDlg::OnBnClickedButtonMultiply)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CalculatorDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DIVIDE, &CalculatorDlg::OnBnClickedButtonDivide)
	ON_BN_CLICKED(IDC_BUTTON_CE, &CalculatorDlg::OnBnClickedButtonCE)
	ON_BN_CLICKED(IDC_BUTTON_MOD, &CalculatorDlg::OnBnClickedButtonModulo)
	ON_BN_CLICKED(IDC_BUTTON_AND, &CalculatorDlg::OnBnClickedButtonAnd)
	ON_BN_CLICKED(IDC_BUTTON_OR, &CalculatorDlg::OnBnClickedButtonOr)
	ON_BN_CLICKED(IDC_BUTTON_XOR, &CalculatorDlg::OnBnClickedButtonXor)
	ON_BN_CLICKED(IDC_BUTTON_NOT, &CalculatorDlg::OnBnClickedButtonNot)
	ON_BN_CLICKED(IDC_BUTTON_SHIFTRIGHT, &CalculatorDlg::OnBnClickedButtonShiftRight)
	ON_BN_CLICKED(IDC_BUTTON_SHIFTLEFT, &CalculatorDlg::OnBnClickedButtonShiftLeft)
	ON_BN_CLICKED(IDC_RADIO_DEC, &CalculatorDlg::OnBnClickedRadioDec)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_PARENTHESE, &CalculatorDlg::OnBnClickedButtonLeftParenthese)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_PARENTHESE, &CalculatorDlg::OnBnClickedButtonRightParenthese)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_A, &CalculatorDlg::OnBnClickedButtonCharA)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_B, &CalculatorDlg::OnBnClickedButtonCharB)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_C, &CalculatorDlg::OnBnClickedButtonCharC)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_D, &CalculatorDlg::OnBnClickedButtonCharD)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_E, &CalculatorDlg::OnBnClickedButtonCharE)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_F, &CalculatorDlg::OnBnClickedButtonCharF)
	ON_WM_ERASEBKGND()
	ON_WM_DRAWITEM()
	ON_COMMAND(ID_HELP_ABOUT, &CalculatorDlg::OnHelpAbout)
END_MESSAGE_MAP()


// CalculatorDlg message handlers

BOOL CalculatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	// check 'Dec' Radio Button
	CButton *pBt = (CButton*)this->GetDlgItem(IDC_RADIO_DEC);
	pBt->SetCheck(TRUE);

	// set color for button
	pBt = (CButton*)this->GetDlgItem(IDC_BUTTON_NUMBER_1);

	// hide cursor in Edit Box
	CEdit *pEdit = (CEdit*)this->GetDlgItem(IDC_EDIT2);
	pEdit->HideCaret();
	pEdit->SetFont(&m_font);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CalculatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CalculatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CalculatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CalculatorDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (nIDCtl == IDC_RADIO_DEC || nIDCtl == IDC_RADIO_BIN || nIDCtl == IDC_RADIO_HEX)
	{
		CDC dc;

		dc.Attach(lpDrawItemStruct->hDC);
		RECT rect;
		rect = lpDrawItemStruct->rcItem;

		dc.FillSolidRect(&rect, RGB(0, 255, 0));
		UINT state = lpDrawItemStruct->itemState;

		if ((state & ODS_SELECTED))
		{
			dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);

		}
		else
		{
			dc.DrawEdge(&rect, EDGE_RAISED, BF_RECT);
		}

		dc.SetBkColor(RGB(100, 100, 100));
		dc.SetTextColor(RGB(255, 100, 100));
		dc.SetBkMode(TRANSPARENT);

		TCHAR buffer[MAX_PATH];
		ZeroMemory(buffer, MAX_PATH);
		::GetWindowText(lpDrawItemStruct->hwndItem, buffer, MAX_PATH);
		dc.DrawText(buffer, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		dc.Detach();
	}

	CalculatorDlg::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

BOOL CalculatorDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect r;
	pDC->GetClipBox(&r);

	pDC->FillSolidRect(r, RGB(0xD9, 0xE4, 0xF1)); //ugly magenta background
	return TRUE;
}

void CalculatorDlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CalculatorDlg::insert_num(int num)
{
	if (m_calculated)
	{
		m_value = 0;
		m_calculated = false;
	}

	if (m_base == DECIMAL)
		m_value = m_value * 10 + num;
	else if (m_base == BINARY && (num == 0 || num == 1))
		m_value = (m_value << 1) + num;
	else if (m_base == HEXADECIMAL)
		m_value = m_value * 16 + num;
}

void CalculatorDlg::update_value()
{
	if (m_base == DECIMAL)
		m_text = to_wstring(m_value).c_str();
	else if (m_base == BINARY)
	{
		m_text = DecToBin(m_value).c_str();
		const int len = m_text.GetLength();
		for (int i = len - 1; i >= 0; --i)
		{
			if ((len - i) % 8 == 0)
				m_text.Insert(i, _T(" "));
		}
	}
	else if (m_base == HEXADECIMAL)
		m_text = DecToHex(m_value).c_str();

	CEdit *pEdit = static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT2));
	pEdit->SetFocus();
	pEdit->HideCaret();

	UpdateData(FALSE);
}


void CalculatorDlg::insert_operator(char opr)
{
	// Thuật toán Ba Lan ngược

	switch(opr)
	{
	case '(':
		m_opr.push(opr);
		break;
	case ')':
		while (m_opr.top() != '(')
		{
			m_posFix.push(m_opr.top());
			m_opr.pop();
		}
		m_opr.pop();		// lấy ra dấu ngoặc trái '('
		break;
	default:		// là toán tử
		while (!m_opr.empty() && priority_less_equal_than(opr, m_opr.top()))
		{
			m_posFix.push(m_opr.top());
			m_opr.pop();
		}
		m_opr.push(opr);
	}
}

void CalculatorDlg::update_posfix_number()
{
	if (m_prev != PARENTHESES)
	{
		m_operands.push_back(m_value);
		m_posFix.push(m_operands.size() - 1);
		m_value = 0;
		m_prev = NUMBER;
	}
}

bool CalculatorDlg::priority_less_equal_than(char opr1, char opr2)
{
	static std::map<char, int> precedence =
	{ {'*', 5},
	  {'/', 5},
	  {'%', 5},
	  {'+', 4},
	  {'-', 4},
	  {'&', 3},
	  {'|', 2},
	  {'&', 1},
	  {'(', 0}};

	return precedence[opr1] <= precedence[opr2];
}

bool CalculatorDlg::is_operator(char ch)
{
	return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '&' || ch == '|' || ch == '^';
}

void CalculatorDlg::OnBnClickedButtonC()
{
	switch (m_base)
	{
		case CalculatorDlg::DECIMAL:
			m_value = m_value / 10;
			break;
		case CalculatorDlg::BINARY:
			m_value = m_value >> 1;
			break;
		case CalculatorDlg::HEXADECIMAL:
			m_value = m_value / 16;
			break;
		default:
			break;
	}

	update_value();
}


void CalculatorDlg::OnBnClickedButtonNumber1()
{
	// button number 1
	
	insert_num(1);
	update_value();
}


void CalculatorDlg::OnBnClickedButtonNumber2()
{
	// button number 2

	insert_num(2);
	update_value();
}


void CalculatorDlg::OnBnClickedButtonNumber3()
{
	// button number 3

	insert_num(3);
	update_value();
}


void CalculatorDlg::OnBnClickedButtonNumber4()
{
	// button number 4

	insert_num(4);
	update_value();
}


void CalculatorDlg::OnBnClickedButtonNumber5()
{
	// button number 5

	insert_num(5);
	update_value();
}


void CalculatorDlg::OnBnClickedButtonNumber6()
{
	// button number 6

	insert_num(6);
	update_value();
}


void CalculatorDlg::OnBnClickedButtonNumber7()
{
	// button number 7

	insert_num(7);
	update_value();
}


void CalculatorDlg::OnBnClickedButtonNumber8()
{
	// button number 8

	insert_num(8);
	update_value();
}


void CalculatorDlg::OnBnClickedButtonNumber9()
{
	// button number 9

	insert_num(9);
	update_value();
}


void CalculatorDlg::OnBnClickedButtonNumber0()
{
	// button number 0

	insert_num(0);
	update_value();
}


void CalculatorDlg::OnBnClickedRadioBin()
{
	// bin radio button
	m_base = BINARY;
	update_value();
	m_calculated = true;
}


void CalculatorDlg::OnBnClickedRadioHex()
{
	// hex radion button
	m_base = HEXADECIMAL;
	update_value();
	m_calculated = true;
}


void CalculatorDlg::OnBnClickedButtonSubtract()
{
	// subtract operation
	update_posfix_number();
	insert_operator('-');
	m_prev = OPERATOR;
}


void CalculatorDlg::OnBnClickedButtonEqual()
{
	UpdateData(TRUE);

	m_operands.push_back(m_value);
	m_posFix.push(m_operands.size() - 1);
	while (!m_opr.empty())
	{
		m_posFix.push(m_opr.top());
		m_opr.pop();
	}

	std::stack<BigInt> s;		// stack lưu kết quả cuối cùng
	while (!m_posFix.empty())
	{
		if (!is_operator(m_posFix.front()))			// là số
		{
			s.push(m_operands[m_posFix.front()]);
			//m_operands.erase(m_operands.begin() + m_posFix.front());
		}
		else			// là toán tử
		{
			BigInt rhs = s.top();
			s.pop();
			BigInt lhs = s.top();
			s.pop();

			// ánh xạ từ ký tự sang toán tử
			try
			{
				s.push(m_char_to_opr[m_posFix.front()](lhs, rhs));
			}
			catch(const std::exception &e)
			{
				m_text = e.what();
				m_value = 0;
				UpdateData(FALSE);
				m_operands.clear();
				std::queue<char>().swap(m_posFix);		// clear m_posFix queue
				m_calculated = true;
				return;
			}
		}

		m_posFix.pop();
	}

	m_value = s.top();
	update_value();
	m_operands.clear();
	m_calculated = true;
}


void CalculatorDlg::OnBnClickedButtonMultiply()
{
	// multiply operation
	update_posfix_number();
	insert_operator('*');
	m_prev = OPERATOR;
}


void CalculatorDlg::OnBnClickedButtonAdd()
{
	// add operation
	update_posfix_number();
	insert_operator('+');
	m_prev = OPERATOR;
}


void CalculatorDlg::OnBnClickedButtonDivide()
{
	// divide operation
	update_posfix_number();
	insert_operator('/');
	m_prev = OPERATOR;
}


void CalculatorDlg::OnBnClickedButtonCE()
{
	// CE button
	std::stack<char>().swap(m_opr);
	std::queue<char>().swap(m_posFix);
	m_value = 0;
	m_lhs = m_rhs = 0;
	update_value();
}


void CalculatorDlg::OnBnClickedButtonModulo()
{
	// mod operation
	update_posfix_number();
	insert_operator('%');
	m_prev = OPERATOR;
}


void CalculatorDlg::OnBnClickedButtonAnd()
{
	// And operation
	update_posfix_number();
	insert_operator('&');
	m_prev = OPERATOR;
}


void CalculatorDlg::OnBnClickedButtonOr()
{
	// Or operation
	update_posfix_number();
	insert_operator('|');
	m_prev = OPERATOR;
}


void CalculatorDlg::OnBnClickedButtonXor()
{
	// Xor operation
	update_posfix_number();
	insert_operator('^');
	m_prev = OPERATOR;
}


void CalculatorDlg::OnBnClickedButtonNot()
{
	// Not operation
	//m_operation = CalculatorDlg::Operation::NOT;
	m_value = ~m_value;
	update_value();
}


void CalculatorDlg::OnBnClickedButtonShiftRight()
{
	// Shift right operation
	//m_operation = CalculatorDlg::Operation::SHIFT_RIGHT;
	m_value = m_value >> 1;
	update_value();
}


void CalculatorDlg::OnBnClickedButtonShiftLeft()
{
	// Shift left operation
	//m_operation = CalculatorDlg::Operation::SHIFT_LEFT;
	m_value = m_value << 1;
	update_value();
}


void CalculatorDlg::OnBnClickedRadioDec()
{
	m_base = DECIMAL;
	update_value();
	m_calculated = true;
}


void CalculatorDlg::OnBnClickedButtonLeftParenthese()
{
	// dấu ngoặc trái
	insert_operator('(');
}


void CalculatorDlg::OnBnClickedButtonRightParenthese()
{
	// dấu ngoặc phải
	update_posfix_number();
	insert_operator(')');
	m_prev = PARENTHESES;
}


void CalculatorDlg::OnBnClickedButtonCharA()
{
	// A button
	if (m_base == HEXADECIMAL)
	{
		m_value = m_value * 16 + 10;
		update_value();
	}
}


void CalculatorDlg::OnBnClickedButtonCharB()
{
	// Char B button
	if (m_base == HEXADECIMAL)
	{
		m_value = m_value * 16 + 11;
		update_value();
	}
}


void CalculatorDlg::OnBnClickedButtonCharC()
{
	// Char C button
	if (m_base == HEXADECIMAL)
	{
		m_value = m_value * 16 + 12;
		update_value();
	}
}


void CalculatorDlg::OnBnClickedButtonCharD()
{
	// Char D button
	if (m_base == HEXADECIMAL)
	{
		m_value = m_value * 16 + 13;
		update_value();
	}
}


void CalculatorDlg::OnBnClickedButtonCharE()
{
	// Char E button
	if (m_base == HEXADECIMAL)
	{
		m_value = m_value * 16 + 14;
		update_value();
	}
}


void CalculatorDlg::OnBnClickedButtonCharF()
{
	// Char F button
	if (m_base == HEXADECIMAL)
	{
		m_value = m_value * 16 + 15;
		update_value();
	}
}


void CalculatorDlg::OnHelpAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}
