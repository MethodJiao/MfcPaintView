
// MFCPaintView.h: CMFCPaintView 类的接口
//

#pragma once
#include <vector>
struct PaintLine
{
	CPoint start;
	CPoint end;
};
class CMFCPaintView : public CView
{
protected: // 仅从序列化创建
	CMFCPaintView() noexcept;
	DECLARE_DYNCREATE(CMFCPaintView)

// 特性
public:
	CMFCPaintDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CMFCPaintView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	std::vector<PaintLine> m_lines;
	CPoint m_firstPoint;
// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	
	void DynamicLine(CDC &MemDc, UINT nFlags, CPoint point);
	void HighLightLine(CDC& MemDC, UINT nFlags, CPoint point, PaintLine line);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MFCPaintView.cpp 中的调试版本
inline CMFCPaintDoc* CMFCPaintView::GetDocument() const
   { return reinterpret_cast<CMFCPaintDoc*>(m_pDocument); }
#endif

