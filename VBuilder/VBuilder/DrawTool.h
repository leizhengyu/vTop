#pragma once

#include "DrawRect.h"


class CVBuilderView;

class CDrawTool
{
public:
	CDrawTool(void);
	~CDrawTool(void);

	void OnLButtonDown(CVBuilderView* pView, UINT nFlags, const CPoint& point);
	void OnLButtonDblClk(CVBuilderView* pView, UINT nFlags, const CPoint& point);
	void OnLButtonUp(CVBuilderView* pView, UINT nFlags, const CPoint& point);
	void OnMouseMove(CVBuilderView* pView, UINT nFlags, const CPoint& point);
//	void OnEditProperties(CVBuilderView* pView);
	CDrawRect* ObjectAt(const CPoint& point);

	void Add(CDrawRect* pObj);
//	void ReMove();
	void Draw(CDCHandle* pDC);


	BOOL IsSelected(CDrawRect* pDocItem);


	enum SelectMode
	{
		none,
		netSelect,
		move,
		size
	};
	enum DrawShape
	{
		selection,
		line,
		rect,
		roundRect,
		ellipse,
		poly
	};
public:
	CAtlList<CDrawRect*> m_objects;
	CAtlList<CDrawRect*> m_selection;
	CPoint lastPoint;
	int nDragHandle;
	SelectMode selectMode;
	DrawShape  c_drawShape;
CPoint c_down;
CPoint c_last;
int    c_nDownFlags;

};
