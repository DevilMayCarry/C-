
// MainWndDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MainWnd.h"
#include "MainWndDlg.h"
#include "afxdialogex.h"

//Lib 引入头文件，并在工程设置中，设置额外的库路径 一般的如果是Lib 在 Debug中生成，
//则在额外的库路径中添加 debug路径

#if 0
#include "..//libUsb//libUsb.h"
#pragma comment(lib, "libUsb.lib")
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define USB_CLOSE	0
#define USB_OPEN	1

//DLL 定义函数指针
typedef void(*MyCallBack)(int);

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMainWndDlg 对话框



CMainWndDlg::CMainWndDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainWndDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainWndDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainWndDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN, &CMainWndDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CMainWndDlg::OnBnClickedBtnClose)
END_MESSAGE_MAP()


// CMainWndDlg 消息处理程序

BOOL CMainWndDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMainWndDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMainWndDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMainWndDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMainWndDlg::OnBnClickedBtnOpen()
{
	// TODO:  在此添加控件通知处理程序代码

	//使用DLL动态链接库，需要修改 项目DllUsb, MainWnd 为动态链接库
	
	HINSTANCE hInstance = LoadLibrary(_T("DllUsb.dll"));

	MyCallBack Mycallback = (MyCallBack)GetProcAddress(hInstance, "OnMyCallBack");
	if (Mycallback)
	{
		Mycallback(USB_OPEN);
	}

#if 0
	//使用LIB 静态链接库，需要修改  libUsb, MainWnd 为静态链接库 , 
	//并且将 c/c++ -> 代码生成 -> 运行库 改为多线程MT，确保每个 Lib 的运行库都一致。

	OnMyCallBack(USB_OPEN);	
#endif
}


void CMainWndDlg::OnBnClickedBtnClose()
{
	// TODO:  在此添加控件通知处理程序代码

	//使用DLL动态链接库，需要修改 项目DllUsb, MainWnd 为动态链接库

	HINSTANCE hInstance = LoadLibrary(_T("DllUsb.dll"));

	MyCallBack Mycallback = (MyCallBack)GetProcAddress(hInstance, "OnMyCallBack");
	if (Mycallback)
	{
		Mycallback(USB_CLOSE);
	}



#if 0
	//使用LIB 静态链接库，需要修改  libUsb, MainWnd 为静态链接库
	//并且将 c/c++ -> 代码生成 -> 运行库 改为多线程MT，确保每个 Lib 的运行库都一致。
	OnMyCallBack(USB_CLOSE);
#endif
}
