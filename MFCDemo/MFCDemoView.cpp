
// MFCDemoView.cpp : CMFCDemoView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCDemo.h"
#endif

#include "MFCDemoDoc.h"
#include "MFCDemoView.h"
//3
#include "MapObj.h"

//4
#include "SetColor.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCDemoView

IMPLEMENT_DYNCREATE(CMFCDemoView, CView)

BEGIN_MESSAGE_MAP(CMFCDemoView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_LINE, &CMFCDemoView::OnLine)
	ON_COMMAND(ID_POLYGON, &CMFCDemoView::OnPolygon)
	ON_COMMAND(ID_SETCOLOR, &CMFCDemoView::OnSetcolor)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CMFCDemoView 构造/析构

CMFCDemoView::CMFCDemoView()
{
	// TODO:  在此处添加构造代码
	//1
	//设置鼠标左键初始没有按下
	m_LButtonDown = false;
	//2+
	//程序初始状态不绘制任何图元
	type = 0;

	//4+
	r = 0; g = 0; b = 0;

}

CMFCDemoView::~CMFCDemoView()
{
	//3
	for (int i = 0; i<objList.GetSize(); i++){
		MapObj* obj = (MapObj*)objList.GetAt(i);
		delete obj;
	}
}

BOOL CMFCDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCDemoView 绘制

void CMFCDemoView::OnDraw(CDC* /*pDC*/)
{
	CMFCDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CDC* pDC = this->GetDC();
	for (int i = 0; i<objList.GetSize(); i++){
		MapObj* obj = (MapObj*)objList.GetAt(i);
		if (obj->type == 1){
			CPoint p1 = obj->points.GetAt(0);
			CPoint p2 = obj->points.GetAt(1);
			DDALine(pDC, p1.x, p1.y, p2.x, p2.y, RGB(r, g, b));
		}
		else if (obj->type == 2){
			CPoint p1 = obj->points.GetAt(0);
			CPoint p2;
			for (int j = 1; j<obj->points.GetSize(); j++){
				p2 = obj->points.GetAt(j);
				DDALine(pDC, p1.x, p1.y, p2.x, p2.y, RGB(r, g, b));
				p1 = p2;
			}
			p2 = obj->points.GetAt(0);
			DDALine(pDC, p1.x, p1.y, p2.x, p2.y, RGB(r, g, b));
		}
	}
	ReleaseDC(pDC);
}


// CMFCDemoView 打印

BOOL CMFCDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFCDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加额外的打印前进行的初始化过程
}

void CMFCDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加打印后进行的清理过程
}


// CMFCDemoView 诊断

#ifdef _DEBUG
void CMFCDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCDemoDoc* CMFCDemoView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCDemoDoc)));
	return (CMFCDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCDemoView 消息处理程序


void CMFCDemoView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (type == 1 || type == 2){
		this->SetCapture();//捕捉鼠标
		//设置开始点和终止点，此时为同一点
		m_StartPoint = point;
		m_EndPoint = point;
		m_LButtonDown = true;//设置鼠标左键按下
		pointList.Add(point);
	}

	CView::OnLButtonDown(nFlags, point);
}


void CMFCDemoView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (type == 1){
		ReleaseCapture();//释放鼠标
		MapObj* obj = new MapObj();
		obj->type = 1;
		obj->points.Add(m_StartPoint);
		obj->points.Add(m_EndPoint);
		objList.Add(obj);
		pointList.RemoveAll();
		CDC* dc = this->GetDC();
		DDALine(dc, m_StartPoint.x, m_StartPoint.y, m_EndPoint.x, m_EndPoint.y, RGB(r, g, b));
	}
	m_LButtonDown = false;

	CView::OnLButtonUp(nFlags, point);
}


void CMFCDemoView::OnMouseMove(UINT nFlags, CPoint point)
{
	if ((m_LButtonDown && type == 1) || (type == 2 && pointList.GetSize() != 0)){
		CDC* pDC = this->GetDC();//构造设备环境对象
		pDC->SetROP2(R2_NOT);//设置绘图模式为R2_NOT
		//重新绘制前一个鼠标移动消息处理函数绘制的直线段
		//因为绘图模式的原因，结果就是擦除了该线段
		DDALine(pDC, m_StartPoint.x, m_StartPoint.y, m_EndPoint.x, m_EndPoint.y, RGB(0, 0, 0));
		//绘制新的直线段
		DDALine(pDC, m_StartPoint.x, m_StartPoint.y, point.x, point.y, RGB(0, 0, 0));
		//保存新的直线段终点
		m_EndPoint = point;
	}

	CView::OnMouseMove(nFlags, point);
}


void CMFCDemoView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (type == 2){
		ReleaseCapture();//释放鼠标
		MapObj* obj = new MapObj();
		obj->type = 2;
		for (int i = 0; i<pointList.GetSize(); i++)
			obj->points.Add(pointList.GetAt(i));
		objList.Add(obj);
		CDC* pDC = this->GetDC();
		CPoint p1 = obj->points.GetAt(0);
		CPoint p2;
		for (int j = 1; j<obj->points.GetSize(); j++){
			p2 = obj->points.GetAt(j);
			DDALine(pDC, p1.x, p1.y, p2.x, p2.y, RGB(r, g, b));
			p1 = p2;
		}
		p2 = obj->points.GetAt(0);
		DDALine(pDC, p1.x, p1.y, p2.x, p2.y, RGB(r, g, b));
		EdgeMarkFill(pDC, RGB(r, g, b));
		pointList.RemoveAll();
	}

	CView::OnLButtonDblClk(nFlags, point);
}

#include "math.h"
void CMFCDemoView::DDALine(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF color)
{
	double dx, dy, e, x, y;
	dx = x2 - x1;
	dy = y2 - y1;
	e = (fabs(dx) > fabs(dy)) ? fabs(dx) : fabs(dy);
	dx /= e;
	dy /= e;
	x = x1;
	y = y1;
	for (int i = 1; i <= e; i++)
	{
		pDC->SetPixel((int)(x + 0.5), (int)(y + 0.5), color);
		x += dx;
		y += dy;
	}
}


void CMFCDemoView::OnLine()
{
	// TODO:  在此添加命令处理程序代码
	//2
	type = 1;
}


void CMFCDemoView::OnPolygon()
{
	// TODO:  在此添加命令处理程序代码
	//2
	type = 2;
}


void CMFCDemoView::OnSetcolor()
{
	// TODO:  在此添加命令处理程序代码
	//4
	SetColor s;
	s.m_r = r; s.m_g = g; s.m_b = b;
	if (s.DoModal() == IDOK){
		r = s.m_r; g = s.m_g; b = s.m_b;
		this->Invalidate(true);
	}
}


void CMFCDemoView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//5
	if (nChar == VK_LEFT){
		for (int i = 0; i<objList.GetSize(); i++){
			MapObj* obj = (MapObj*)objList.GetAt(i);
			for (int j = 0; j<obj->points.GetSize(); j++){
				CPoint p = (CPoint)obj->points.GetAt(j);
				this->Move(&p, -1, 0);
				obj->points.SetAt(j, p);
			}
		}
		this->Invalidate(true);
	}
	if (nChar == VK_RIGHT){
		for (int i = 0; i<objList.GetSize(); i++){
			MapObj* obj = (MapObj*)objList.GetAt(i);
			for (int j = 0; j<obj->points.GetSize(); j++){
				CPoint p = (CPoint)obj->points.GetAt(j);
				this->Move(&p, 1, 0);
				obj->points.SetAt(j, p);
			}
		}
		this->Invalidate(true);
	}
	if (nChar == VK_UP){
		for (int i = 0; i<objList.GetSize(); i++){
			MapObj* obj = (MapObj*)objList.GetAt(i);
			for (int j = 0; j<obj->points.GetSize(); j++){
				CPoint p = (CPoint)obj->points.GetAt(j);
				this->Move(&p, 0, -1);
				obj->points.SetAt(j, p);
			}
		}
		this->Invalidate(true);
	}
	if (nChar == VK_DOWN){
		for (int i = 0; i<objList.GetSize(); i++){
			MapObj* obj = (MapObj*)objList.GetAt(i);
			for (int j = 0; j<obj->points.GetSize(); j++){
				CPoint p = (CPoint)obj->points.GetAt(j);
				this->Move(&p, 0, 1);
				obj->points.SetAt(j, p);
			}
		}
		this->Invalidate(true);
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CMFCDemoView::Move(CPoint* point, int x, int y)
{
	point->x = point->x + x;
	point->y = point->y + y;
}


void CMFCDemoView::EdgeMarkFill(CDC* pDC, COLORREF color)
{
	Pattern();
	if (pointList.GetSize() <= 2)
		return;
	int x1 = pointList.GetAt(0).x;
	int x2 = pointList.GetAt(0).x;
	int y1 = pointList.GetAt(0).y;
	int y2 = pointList.GetAt(0).y;
	for (int i = 1; i < pointList.GetSize(); i++) {
		if (pointList[i].x < x1)
			x1 = pointList[i].x;
		if (pointList[i].y < y1)
			y1 = pointList[i].y;
		if (pointList[i].x > x2)
			x2 = pointList[i].x;
		if (pointList[i].y > y2)
			y2 = pointList[i].y;
	}
	std::vector<std::vector<bool>> MASK;
	{
		bool i = false;
		std::vector<bool> j;
		for (int k = 0; k <= x2; k++)
			j.push_back(i);
		for (int m = 0; m <= y2; m++) {
			MASK.push_back(j);
		}
	}
	double dx, dy, e, x, y;
	for (int i = 0; i < pointList.GetSize(); i++) {
		CPoint p1 = pointList.GetAt(i);
		CPoint p2 = pointList.GetAt((i + 1) % pointList.GetSize());
		if (p1.y == p2.y)
			continue;
		if (p1.y > p2.y) {
			int c;
			c = p1.y; p1.y = p2.y; p2.y = c;
			c = p1.x; p1.x = p2.x; p2.x = c;
		}
		double dxs = (double)(p2.x - p1.x) / (p2.y - p1.y);
		double xs = p1.x;
		for (int ys = p1.y; ys != p2.y; ys++)
		{
			int Ixs = int(xs + 0.5);
			MASK.at((int)(ys)).at((int)(Ixs)) = !MASK.at((int)(ys)).at((int)(Ixs));
			xs = xs + dxs;
		}
	}
	/*double dx, dy, e, x, y;
	for (int i = 0; i < pointList.GetSize() - 1; i++) {
		dx = pointList[i + 1].x - pointList[i].x;
		dy = pointList[i + 1].y - pointList[i].y;
		x = pointList[i].x;
		y = pointList[i].y;
		if (dy <= 0) {
			dy = -dy;
			dx = -dx;
			x = pointList[i + 1].x;
			y = pointList[i + 1].y;
		}
		e = (fabs(dx) > fabs(dy)) ? fabs(dx) : fabs(dy);
		dx /= e;
		dy /= e;
		for (int j = 1; j <= e; j++)
		{
			MASK.at((int)(y + 0.5)).at((int)(x + 0.5)) = !MASK.at((int)(y + 0.5)).at((int)(x + 0.5));
			x += dx;
			y += dy;
		}
	}*/

	/*dx = pointList[0].x - pointList[pointList.GetSize() - 1].x;
	dy = pointList[0].y - pointList[pointList.GetSize() - 1].y;
	x = pointList[pointList.GetSize() - 1].x;
	y = pointList[pointList.GetSize() - 1].y;
	if (dy <= 0) {
		dy = -dy;
		dx = -dx;
		x = pointList[0].x;
		y = pointList[0].y;
	}
	e = (fabs(dx) > fabs(dy)) ? fabs(dx) : fabs(dy);
	dx /= e;
	dy /= e;
	for (int j = 1; j <= e; j++)
	{
		MASK.at((int)(y + 0.5)).at((int)(x + 0.5)) = !MASK.at((int)(y + 0.5)).at((int)(x + 0.5));
		x += dx;
		y += dy;
	}*/

	bool inside = false;
	for (int j = 0; j <= y2; j++) {
		inside = false;
		for (int i = 0; i <= x2; i++) {
			if (MASK.at(j).at(i))
				inside = !inside;
			if (inside) {
				pDC->SetPixel(i, j, pattern[j % 61][i % 113]);
			}
				
		}
	}
}


void CMFCDemoView::Pattern()
{
	int image[61][113];

	using namespace std;
	ifstream inFile;
	inFile.open("2501.txt", ios::in);//inFile来自fstream,ifstream为输入文件流(从文件读入)
	string lineStr;
	string str;
	vector<vector<string>> strArray;
	while (getline(inFile, lineStr))
	{
		stringstream ss(lineStr);
		vector<string> lineArray;
		while (getline(ss, str, ' '))
			lineArray.push_back(str);
		strArray.push_back(lineArray);
	}
	for (int i = 0; i < strArray.size(); i++)
	{
		for (int j = 0; j < strArray.at(i).size(); j++)
		{
			image[i][j] = stoi(strArray.at(i).at(j));
		}
	}

	for (int i = 0; i < 61; i++) {
		for (int j = 0; j < 113; j++) {
			if (image[i][j] == 1)
				pattern[i][j] = RGB(255, 0, 0);
			else
				pattern[i][j] = RGB(0, 255, 0);
		}
	}
}
