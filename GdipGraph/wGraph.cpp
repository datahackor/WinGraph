
// jrGraph.cpp: 定义应用程序的类行为。
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "wGraph.h"
#include "MainFrm.h"
#include "LibGraph.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ULONG_PTR m_gdiplusToken;
// CWinGraphApp

BEGIN_MESSAGE_MAP(CWinGraphApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CWinGraphApp::OnAppAbout)
END_MESSAGE_MAP()


// CWinGraphApp 构造

CWinGraphApp::CWinGraphApp() noexcept
{

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则: 
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("WinGraph.AppID.NoVersion"));

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的 CWinGraphApp 对象

CWinGraphApp theApp;


// CWinGraphApp 初始化

BOOL CWinGraphApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要 AfxInitRichEdit2()
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("JR.WinGraph"));


	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	// 若要创建主窗口，此代码将创建新的框架窗口
	// 对象，然后将其设置为应用程序的主窗口对象
	CFrameWndEx* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// 创建并加载框架及其资源
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, nullptr,
		nullptr);

	
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	//char* p_argv[20];
	//int nargc = __argc;
	//p_argv[0] = "fGraph";
	//p_argv[1] = "-timelimit";
	//p_argv[2] = "10";
	//p_argv[3] = "Test.gdl";
	//p_argv[4] = 0;
	//int i = 0;
	//
	//for (; i < __argc; i++)
	//{
	//	p_argv[i] = __argv[i];
	//}
	//p_argv[i] = 0;

	CRect DispArea;
	GetClientRect(m_pMainWnd->m_hWnd, &DispArea);


	int width = DispArea.right - DispArea.left;
	int height = DispArea.bottom - DispArea.top;

	updateOriginStatus();
	setDrawingRectangle(width - 4, height - 4);
	//  __wargv for unicode
	vcgMain(__argc, (char**)__argv);

	// displayCompleteGraph();
	fishEyeMode(4);
	updateZoomStatus();

	doStatistics();

	char buf[1024];
	_snprintf(buf, sizeof(buf), "%d nodes, %d edge segments, %d crossings",
		st_nr_vis_nodes,
		st_nr_vis_edges,
		nr_crossings);

	setNodesValue(buf);

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

int CWinGraphApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	
	Gdiplus::GdiplusShutdown(m_gdiplusToken);
	return CWinApp::ExitInstance();
}

// CWinGraphApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CWinGraphApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CWinGraphApp 消息处理程序



