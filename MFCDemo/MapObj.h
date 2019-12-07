#pragma once
#include "afx.h"

#include <afxtempl.h>

class MapObj :
	public CObject
{
public:
	CArray<CPoint, CPoint> points;
	int type;
	COLORREF color;

public:
	MapObj();
	virtual ~MapObj();
};

