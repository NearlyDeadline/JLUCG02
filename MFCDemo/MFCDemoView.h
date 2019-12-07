
// MFCDemoView.h : CMFCDemoView ��Ľӿ�
//

#pragma once

//ע��1���֣�ֻ����ֱ�߶�
//ע��2���֣�����ֱ�߶μ������
//ע��3���֣��ػ�
//ע��4���֣���ɫ
//ע��5���֣�ƽ��

//2+
#include <afxtempl.h>


class CMFCDemoView : public CView
{
protected: // �������л�����
	CMFCDemoView();
	DECLARE_DYNCREATE(CMFCDemoView)

// ����
public:
	CMFCDemoDoc* GetDocument() const;
	//1
	CPoint m_StartPoint, m_EndPoint;//��Ƥ�ߵ������˵�
	bool m_LButtonDown;//�ж��������Ƿ���
	//2+
	CArray<CPoint, CPoint> pointList;//�����б�
	int type;//ѡ����Ƶ�ͼԪ����

	//3+
	CObArray objList;//ͼԪ�б�

	//4+
	int r, g, b;
private:
	COLORREF pattern[61][113];

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CMFCDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // MFCDemoView.cpp �еĵ��԰汾
inline CMFCDemoDoc* CMFCDemoView::GetDocument() const
   { return reinterpret_cast<CMFCDemoDoc*>(m_pDocument); }
#endif

