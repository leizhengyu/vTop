#pragma once



class CInst;
class CVBuilderView;

class CDrawRect
{
public:
	CDrawRect(void);
	CDrawRect(const CRect &re);
	~CDrawRect(void);
public:
	void Draw(CDCHandle* pDC);
	void DrawTracker(CDCHandle* pDC);
	void MoveHandleTo(int nHandle, CPoint point, CVBuilderView* pView);
	BOOL Intersects(const CRect& rect);
	int HitTest(CPoint point, CVBuilderView* pView, BOOL bSelected);
	CPoint GetHandle(int nHandle);
	CRect GetHandleRect(int nHandleID, CVBuilderView* pView);
	void MoveTo(const CRect& position, CVBuilderView* pView);
	HCURSOR GetHandleCursor(int nHandle);
	void OnOpen(CVBuilderView* pView);
	void Remove();
public:
	CRect m_position;
	CStringA  sModule;
	CStringA  sInst;
	CInst*   m_pInst;

};
