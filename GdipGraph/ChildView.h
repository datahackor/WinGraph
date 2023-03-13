
// ChildView.h: CChildView 类的接口
//

#include <string>


namespace ATL { using ::CString; };
#define _WTL_NO_AUTOMATIC_NAMESPACE

#include "atlapp.h"
#include <atlmisc.h>
#include <atlgdi.h>
//#include "gdip.h"
#pragma once


// CChildView 窗口

class CChildView : public CScrollView
{
// 构造
public:
	CChildView();

// 特性
public:
	virtual void OnDraw(CDC* pDC);
// 操作
public:

// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CChildView();

	// 生成的消息映射函数
protected:

	DECLARE_MESSAGE_MAP()


private:
	//Gdiplus::Font* m_pFont;

public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFilePrint();
	afx_msg void OnFileSaveas();
	afx_msg void OnViewNomal();
	afx_msg void OnViewPolorfish();
	afx_msg void OnViewFixedpolorfish();
	afx_msg void OnViewCartesianfish();
	afx_msg void On3viewFixedradiuscart();
	afx_msg void OnViewEdgelabel();
	afx_msg void OnZoomZoomin();
	afx_msg void OnZoomZoomout();
	afx_msg void OnZoomZoomnormal();
	afx_msg void OnZoomFitallgraph();
	afx_msg void OnMovePositiononorigin();
	afx_msg void OnMoveEasygraphpanning();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	virtual void PostNcDestroy();
};

