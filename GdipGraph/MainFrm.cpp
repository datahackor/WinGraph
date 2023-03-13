﻿
// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "wGraph.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CMFCStatusBar* m_pStatusBar = NULL;

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	//ID_SEPARATOR,           // 状态行指示器
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
	ID_INDICATOR_ZOOM,
	ID_INDICATOR_ORIGIN,
	ID_INDICATOR_NODES,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
	// TODO: 在此添加成员初始化代码
	
}

CMainFrame::~CMainFrame()
{

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 创建一个视图以占用框架的工作区
	if (!m_wndView.Create(nullptr, nullptr, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, nullptr))
	{
		TRACE0("未能创建视图窗口\n");
		return -1;
	}

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	int a, b,c;

	//获取状态栏索引号
	a = m_wndStatusBar.CommandToIndex(ID_INDICATOR_ZOOM);  
	b = m_wndStatusBar.CommandToIndex(ID_INDICATOR_ORIGIN);
	c = m_wndStatusBar.CommandToIndex(ID_INDICATOR_NODES); 

	//设置窗格宽度
	m_wndStatusBar.SetPaneWidth(a, 50);
	m_wndStatusBar.SetPaneWidth(b, 280);
	m_wndStatusBar.SetPaneWidth(c, 280);

	//设置背景颜色
	//m_wndStatusBar.SetPaneBackgroundColor(a, RGB(0,32,64));
	//m_wndStatusBar.SetPaneBackgroundColor(b, RGB(0,32,64));
	//m_wndStatusBar.SetPaneBackgroundColor(c, RGB(0,32,64));

	//设置文本颜色
	m_wndStatusBar.SetPaneTextColor(a, RGB(0, 0, 0));
	m_wndStatusBar.SetPaneTextColor(b, RGB(0, 0, 0));
	m_wndStatusBar.SetPaneTextColor(c, RGB(0, 0, 0));

	m_wndStatusBar.SetPaneText(a, "100.00%");
	m_wndStatusBar.SetPaneText(b, "0, 0");
	m_wndStatusBar.SetPaneText(c, "0");

	// TODO: 如果不需要可停靠工具栏，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndStatusBar.EnableDocking(CBRS_BORDER_BOTTOM);
	EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar); // Debug Assertion Failed!
	DockPane(&m_wndToolBar);


	m_pStatusBar = &m_wndStatusBar;

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 将焦点前移到视图窗口
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 让视图第一次尝试该命令
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 否则，执行默认处理
	return CFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



void CMainFrame::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CFrameWndEx::PostNcDestroy();
}
