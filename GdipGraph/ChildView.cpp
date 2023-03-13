
// ChildView.cpp: CChildView 类的实现
//

#include "pch.h"
#include "framework.h"
#include "wGraph.h"
#include "ChildView.h"
#include "LibGraph.h"
#include "MainFrm.h"

#pragma warning(disable:4244)
#pragma warning(disable:4838)

#define IsCTRLpressed() ( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 )

using namespace std;
using namespace Gdiplus;

const int STEP = 20;
const int PAGE = 100;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



static Gdiplus::Font* GetGDIFont(HDC hdc, int em, int charset = DEFAULT_CHARSET,
	bool bold = false, const char* fontName = "Arial")
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT)); // zero out structure 
	lf.lfHeight = em; // request a 8-pixel-height font
	lf.lfCharSet = charset;
	lstrcpy(lf.lfFaceName, fontName); // request a face name "Arial"
	if (bold)
		lf.lfWeight = FW_BOLD;
	else
		lf.lfWeight = FW_NORMAL;
	return ::new Gdiplus::Font(hdc, &lf);
}


// CChildView

CChildView::CChildView()
{

}

CChildView::~CChildView()
{
	
}


BEGIN_MESSAGE_MAP(CChildView, CScrollView)
	//ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_PRINT, &CChildView::OnFilePrint)
	ON_COMMAND(ID_FILE_SAVEAS, &CChildView::OnFileSaveas)
	ON_COMMAND(ID_VIEW_NOMAL, &CChildView::OnViewNomal)
	ON_COMMAND(ID_VIEW_POLORFISH, &CChildView::OnViewPolorfish)
	ON_COMMAND(ID_VIEW_FIXEDPOLORFISH, &CChildView::OnViewFixedpolorfish)
	ON_COMMAND(ID_VIEW_CARTESIANFISH, &CChildView::OnViewCartesianfish)
	ON_COMMAND(ID_3VIEW_FIXEDRADIUSCART, &CChildView::On3viewFixedradiuscart)
	ON_COMMAND(ID_VIEW_EDGELABEL, &CChildView::OnViewEdgelabel)
	ON_COMMAND(ID_ZOOM_ZOOMIN, &CChildView::OnZoomZoomin)
	ON_COMMAND(ID_ZOOM_ZOOMOUT, &CChildView::OnZoomZoomout)
	ON_COMMAND(ID_ZOOM_ZOOMNORMAL, &CChildView::OnZoomZoomnormal)
	ON_COMMAND(ID_ZOOM_FITALLGRAPH, &CChildView::OnZoomFitallgraph)
	ON_COMMAND(ID_MOVE_POSITIONONORIGIN, &CChildView::OnMovePositiononorigin)
	ON_COMMAND(ID_MOVE_EASYGRAPHPANNING, &CChildView::OnMoveEasygraphpanning)
	ON_WM_KEYDOWN()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CScrollView::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.style |= WS_VSCROLL | WS_HSCROLL;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}



BOOL CChildView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CScrollView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void CChildView::OnDestroy()
{
	CScrollView::OnDestroy();

}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;


	return 0;
}


void CChildView::OnFilePrint()
{
	// TODO: 在此添加命令处理程序代码


	int NOT_IMPLEMENTATION = 1;
}
extern char* contents;
extern long fsize;

void CChildView::OnFileSaveas()
{
	// TODO: 在此添加命令处理程序代码
	static TCHAR BASED_CODE	szFilters[] = _T(
		"Files type (*.*)|*.*|All Files (*.*)|*.*||");
	UpdateData(TRUE);
	CFileDialog fileDlg(false, "*.*", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, szFilters, this);
	int res = fileDlg.DoModal();
	if (res == IDOK)
	{
		char filePath[512];
		CString pathName = fileDlg.GetPathName();
		strcpy(filePath, pathName.GetBuffer());
		//wcstombs(filePath, pathName.GetBuffer(),256);
		FILE* fp = fopen(filePath, "w");
		if (fp == NULL) 
		{
			MessageBoxA("Application","Cannot open file %1.\n",MB_OK);
			return ;
		}

		if (fwrite(contents, 1, fsize, fp) != fsize) {
			MessageBoxA("Application", "Cannot write to file %1.\n", MB_OK);
		}
		fclose(fp);
		CMainFrame* frm = (CMainFrame*)AfxGetMainWnd();
		frm->m_wndStatusBar.SetPaneText(1, "File saved");

	}
}


void CChildView::OnViewNomal()
{
	fishEyeMode(4);
	updateZoomStatus();
	Invalidate();
}


void CChildView::OnViewPolorfish()
{
	fishEyeMode(0);
	updateZoomStatus();
	Invalidate();
}


void CChildView::OnViewFixedpolorfish()
{
	fishEyeMode(1);
	updateZoomStatus();
	Invalidate();
}


void CChildView::OnViewCartesianfish()
{
	fishEyeMode(2);
	updateZoomStatus();
	Invalidate();
}


void CChildView::On3viewFixedradiuscart()
{
	fishEyeMode(3);
	updateZoomStatus();
	Invalidate();
}


void CChildView::OnViewEdgelabel()
{
	relayOut();
	Invalidate();
}


void CChildView::OnZoomZoomin()
{
	changeFeScaling(3, 2);
	updateZoomStatus();
	Invalidate();
}


void CChildView::OnZoomZoomout()
{
	changeFeScaling(2, 3);
	updateZoomStatus();
	Invalidate();
}


void CChildView::OnZoomZoomnormal()
{
	setFeScaling(1, 1);
	updateZoomStatus();
	Invalidate();
}


void CChildView::OnZoomFitallgraph()
{
	displayCompleteGraph();
	updateZoomStatus();
	Invalidate();
}


void CChildView::OnMovePositiononorigin()
{
	normalFeFocus();
	Invalidate();
	updateOriginStatus();
}


void CChildView::OnMoveEasygraphpanning()
{

}


void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_LEFT)
	{
		if (IsCTRLpressed())
			moveFocus(-PAGE, 0);
		else
			moveFocus(-STEP, 0);
	}
	else if (nChar == VK_RIGHT)
	{
		if (IsCTRLpressed())
			moveFocus(PAGE, 0);
		else
			moveFocus(STEP, 0);
	}
	else if (nChar == VK_UP)
	{
		if (IsCTRLpressed())
			moveFocus(0, -PAGE);
		else
			moveFocus(0, -STEP);
	}
	else if (nChar == VK_DOWN)
	{
		if (IsCTRLpressed())
			moveFocus(0, PAGE);
		else
			moveFocus(0, STEP);
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CChildView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	int old = 50;
	int code = 0;
	bool reset_slider = true;

		// When there is no member variable and we need to access the control
		//     CSliderCtrl* pSlider = reinterpret_cast<CSliderCtrl*>(pScrollBar);
		// Handle event here
	switch (nSBCode)
	{
	case TB_LINEUP:
		code = -STEP;
		break;
	case TB_LINEDOWN:
		code = STEP;
		break;
	case TB_PAGEUP:
		code = -PAGE;
		break;
	case TB_PAGEDOWN:
		code = PAGE;
		break;
	case TB_THUMBPOSITION:
		code = -20 * (old - nPos);
		old = nPos;
		reset_slider = false;
		break;
	case TB_TOP:
	case TB_BOTTOM:
	case TB_THUMBTRACK:
	case TB_ENDTRACK:
		old = 50;
		nPos = 50;
	default:
		break;
	}

	if (code != 0)
	{
		//if (reset_slider)
		//	pSlider->SetScrollPos(SB_HORZ,nPos);
		//if (pSlider == (QAbstractSlider*)canvas->verticalScrollBar())
		//	move_focus(0, code);
		//else
		moveFocus(code, 0);
		Invalidate();
		return;
	}

	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CChildView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	int code = 0;
	if (code != 0)
	{
		moveFocus(0, code);
		Invalidate();
	}

	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}

//
//void CChildView::readSettings() {
//	QSettings settings("Hex-Rays", "WinGraph32");
//	G_displayel = settings.value("EdgeLabels", true).toBool();
//	showLabelsAct->setChecked(G_displayel);
//	setShowLabelsIcon();
//	sticky = settings.value("StickMouse", false).toBool();
//	easyPanAct->setChecked(sticky);
//	QPoint pos = settings.value("pos", QPoint(360, 135)).toPoint();
//	QSize size = settings.value("size", QSize(542, 485)).toSize();
//	resize(size);
//	move(pos);
//}
//
//void CChildView::writeSettings() {
//	QSettings settings("Hex-Rays", "WinGraph32");
//	settings.setValue("pos", pos());
//	settings.setValue("size", size());
//	settings.setValue("EdgeLabels", G_displayel);
//	settings.setValue("StickMouse", sticky);
//}

void CChildView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	SetScrollSizes(MM_TEXT, CSize(800, 600));
	ShowScrollBar(SB_BOTH/*, TRUE*/); 
	// TODO: Add your specialized code here and/or call the base class
}

void CChildView::OnDraw(CDC* pDC)
{
	HideCaret();
	CPoint  const pos = GetDeviceScrollPosition();
	CRect rect;
	RectF rectf_text;
	GetClientRect(&rect);
	OffsetRect(&rect, pos.x, pos.y);
	Graphics graphics(pDC->m_hDC);
	static std::wstring temp(L"port by datahacker 2023-3");


	//rectf_text.X = 10;
	//rectf_text.Y = 10;
	//FontFamily fontFamily1(L"Arial"); // 定义"楷体"字样
	//Gdiplus::Font font(&fontFamily1, 20, FontStyleRegular, UnitPoint);
	//graphics.MeasureString(temp.c_str(), temp.size(), &font, rectf_text, &rectf_text);
	//auto sf = StringFormat::GenericDefault();
	//Color color;
	//color.SetFromCOLORREF(RGB(0, 0, 0));
	//SolidBrush sb(color);
	//graphics.DrawString(temp.c_str(), temp.size(), &font, rectf_text, sf, &sb);

	Gdiplus::Pen pen(Color(0, 0, 0, 0), 2);
	Gdiplus::SolidBrush brush(Color(0, 255, 0, 0));
	setDrawingRectangle(rect.Width(), rect.Height());

	drawGraph(&graphics);

	//ShowCaret();
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	//SetScrollSizes(MM_TEXT,
	//	{ 2560, 40000 },
	//	{ (cx / 20) * 20, (cy / 54) * 54 },
	//	{ 20, 54 });

}


void CChildView::OnSetFocus(CWnd* pOldWnd)
{
	CScrollView::OnSetFocus(pOldWnd);

	//CreateSolidCaret(4, 48);
	//SetCaretPos({ 0, 0 });
	//ShowCaret();
}


void CChildView::OnKillFocus(CWnd* pNewWnd)
{
	CScrollView::OnKillFocus(pNewWnd);
	//HideCaret();

}


void CChildView::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	// 不要析构，否则会在MainFrm再次析构
	//CScrollView::PostNcDestroy();
}
