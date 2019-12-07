
// MFCDemoView.h : CMFCDemoView 类的接口
//

#pragma once

//注释1部分：只绘制直线段
//注释2部分：绘制直线段及多边形
//注释3部分：重画
//注释4部分：颜色
//注释5部分：平移

//2+
#include <afxtempl.h>


class CMFCDemoView : public CView
{
protected: // 仅从序列化创建
	CMFCDemoView();
	DECLARE_DYNCREATE(CMFCDemoView)

// 特性
public:
	CMFCDemoDoc* GetDocument() const;
	//1
	CPoint m_StartPoint, m_EndPoint;//橡皮线的两个端点
	bool m_LButtonDown;//判断鼠标左键是否按下
	//2+
	CArray<CPoint, CPoint> pointList;//顶点列表
	int type;//选择绘制的图元类型

	//3+
	CObArray objList;//图元列表

	//4+
	int r, g, b;
private:
	COLORREF pattern[61][113];

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMFCDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	void DDALine(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF color);
	afx_msg void OnLine();
	afx_msg void OnPolygon();
	afx_msg void OnSetcolor();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void Move(CPoint* point, int x, int y);
	void EdgeMarkFill(CDC* pDC, COLORREF color);
private:
	void Pattern();
};

#ifndef _DEBUG  // MFCDemoView.cpp 中的调试版本
inline CMFCDemoDoc* CMFCDemoView::GetDocument() const
   { return reinterpret_cast<CMFCDemoDoc*>(m_pDocument); }
#endif

