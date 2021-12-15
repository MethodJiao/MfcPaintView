
// MFCPaintView.cpp: CMFCPaintView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCPaint.h"
#endif

#include "MFCPaintDoc.h"
#include "MFCPaintView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCPaintView

IMPLEMENT_DYNCREATE(CMFCPaintView, CView)

BEGIN_MESSAGE_MAP(CMFCPaintView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CMFCPaintView 构造/析构

CMFCPaintView::CMFCPaintView() noexcept
{
	// TODO: 在此处添加构造代码

}

CMFCPaintView::~CMFCPaintView()
{
}

BOOL CMFCPaintView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCPaintView 绘图

void CMFCPaintView::OnDraw(CDC* /*pDC*/)
{
	CMFCPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}

void CMFCPaintView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCPaintView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCPaintView 诊断

#ifdef _DEBUG
void CMFCPaintView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCPaintView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCPaintDoc* CMFCPaintView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCPaintDoc)));
	return (CMFCPaintDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCPaintView 消息处理程序


void CMFCPaintView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	CDC* pDC = GetDC();
	this->GetWindowRect(rect);

	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	//创建一个内存中的图像 双缓冲
	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	MemDC.SelectObject(&MemBitmap);
	MemDC.FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(255, 255, 255));//填充初始颜色
	CPen PenForDrawAxis(PS_SOLID, 2, RGB(0, 0, 0));
	MemDC.SelectObject(PenForDrawAxis);
	if ((nFlags & MK_LBUTTON) != 0)
	{
		MemDC.MoveTo(m_firstPoint);
		MemDC.LineTo(point);
	}
	for (size_t i = 0; i < m_lines.size(); i++)
	{
		MemDC.MoveTo(m_lines[i].start);
		MemDC.LineTo(m_lines[i].end);
		//这段不必要 start
		CRect re;
		if (m_lines[i].start.x < m_lines[i].end.x)
		{
			re = CRect(m_lines[i].start, m_lines[i].end);
		}
		else
		{
			re = CRect(m_lines[i].end, m_lines[i].start);

		}
		if (re.PtInRect(point))
		{
			CPen PenForDrawAxis1(PS_SOLID, 5, RGB(255, 0, 0));
			MemDC.SelectObject(PenForDrawAxis1);
			MemDC.MoveTo(m_lines[i].start);
			MemDC.LineTo(m_lines[i].end);
		}
		//end
	}
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);

	ReleaseDC(pDC);
	//InvalidateRect(nullptr);
	CView::OnMouseMove(nFlags, point);
}


void CMFCPaintView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_firstPoint = point;
	CView::OnLButtonDown(nFlags, point);
}


void CMFCPaintView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDC* PDC = GetDC();
	PDC->MoveTo(m_firstPoint);
	PDC->LineTo(point);
	PaintLine line;
	line.start = m_firstPoint;
	line.end = point;
	m_lines.push_back(line);
	ReleaseDC(PDC);
	CView::OnLButtonUp(nFlags, point);
}
