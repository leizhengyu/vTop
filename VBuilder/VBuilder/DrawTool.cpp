#include "StdAfx.h"
#include "DrawTool.h"
#include "VBuilderView.h"


CDrawTool::CDrawTool(void)
{
	selectMode = none;
	c_drawShape = selection;
}

CDrawTool::~CDrawTool(void)
{
	POSITION pos = m_objects.GetHeadPosition();
	while (pos != NULL)
		delete m_objects.GetNext(pos);
}
void CDrawTool::OnLButtonDown(CVBuilderView* pView, UINT nFlags, const CPoint& point)
{
	CPoint local = point;
	
	CDrawRect *pObj;

	if (m_selection.GetCount() == 1) {
		pObj = m_selection.GetHead();
		nDragHandle = pObj->HitTest(point, pView, TRUE);
		if (nDragHandle != 0)
			selectMode = size;
	}

	// See if the click was on an object, select and start move if so
	if (selectMode == none)	{
		pObj = ObjectAt(point);
		if (pObj != NULL) {
			selectMode = move;
			if (!IsSelected(pObj)) {
				if((nFlags & MK_SHIFT) == 0) {
					m_selection.RemoveAll();	
				}				
				m_selection.AddTail(pObj);	
				pView->Invalidate(TRUE);
			}
		}
		else {
			if ((nFlags & MK_SHIFT) == 0){
				m_selection.RemoveAll();
				pView->Invalidate(TRUE);
			}

			selectMode = netSelect;

			CClientDC dc(pView->m_hWnd);
			CRect rect(point.x, point.y, point.x, point.y);
			rect.NormalizeRect();
			dc.DrawFocusRect(rect);	
		}

		
	}

	pView->SetCapture();

	lastPoint = local;
	c_nDownFlags = nFlags;
	c_down = point;
	c_last = point;
	
}


BOOL CDrawTool::IsSelected(CDrawRect* pDocItem) 
{
	return m_selection.Find(pDocItem) != NULL;
}


void CDrawTool::OnLButtonDblClk(CVBuilderView* pView, UINT nFlags, const CPoint& point)
{
	if (m_selection.GetCount() == 1)
		m_selection.GetHead()->OnOpen(pView);
}
void CDrawTool::OnLButtonUp(CVBuilderView* pView, UINT nFlags, const CPoint& point)
{

	if (::GetCapture() == pView->m_hWnd)
	{
		if (selectMode == netSelect)
		{
			CClientDC dc(pView->m_hWnd);
			CRect rect(c_down.x, c_down.y, c_last.x, c_last.y);
			CRect lprect(c_down.x, c_down.y, c_last.x, c_last.y);
			pView->LPToDP(rect);
			rect.NormalizeRect();
			dc.DrawFocusRect(rect);

			POSITION posObj = m_objects.GetHeadPosition();
			while (posObj != NULL)
			{
				CDrawRect* pObj = m_objects.GetNext(posObj);
			//	pView->DPToLP(rect);
				if (pObj->Intersects(lprect))
					m_selection.AddTail(pObj);
			}

			pView->Invalidate();



			//pView->SelectWithinRect(rect, TRUE);
		}
		else if (selectMode != none)
		{
			pView->Invalidate();
		}
	}

	::ReleaseCapture();
	selectMode = none;
}
void CDrawTool::OnMouseMove(CVBuilderView* pView, UINT nFlags, const CPoint& point)
{
	if (::GetCapture() != pView->m_hWnd)
	{
		if (c_drawShape == selection && m_selection.GetCount() == 1)
		{
			CDrawRect* pObj = m_selection.GetHead();
			CPoint local = point;
		//	pView->ClientToDoc(local);
			int nHandle = pObj->HitTest(local, pView, TRUE);
			if (nHandle != 0)
			{
				SetCursor(pObj->GetHandleCursor(nHandle));
				return; // bypass CDrawTool
			}
		}
		if (c_drawShape == selection) {
			c_last = point;
			AtlLoadSysCursor(IDC_ARROW);
		}
			//CDrawTool::OnMouseMove(pView, nFlags, point);
		return;
	}


	if (selectMode == netSelect)
	{
		CClientDC dc(pView->m_hWnd);
		CRect rect(c_down.x, c_down.y, c_last.x, c_last.y);
//		pView->LPToDP(rect);

		rect.NormalizeRect();
		dc.DrawFocusRect(rect);
		rect.SetRect(c_down.x, c_down.y, point.x, point.y);
//		pView->LPToDP(rect);

		rect.NormalizeRect();
		dc.DrawFocusRect(rect);
		c_last = point;
		AtlLoadSysCursor(IDC_ARROW);
		return;
	}

	CPoint local = point;
//	pView->ClientToDoc(local);
	CPoint delta = (CPoint)(local - lastPoint);

	POSITION pos = m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		CDrawRect* pObj = m_selection.GetNext(pos);
		CRect position = pObj->m_position;

		if (selectMode == move)
		{
			position += delta;
			pObj->MoveTo(position, pView);
		}
		else if (nDragHandle != 0)
		{
			pObj->MoveHandleTo(nDragHandle, local, pView);
		}
	}

	lastPoint = local;

	if (selectMode == size && c_drawShape == selection)
	{
		c_last = point;
		SetCursor(m_selection.GetHead()->GetHandleCursor(nDragHandle));
		return; // bypass CDrawTool
	}

	c_last = point;

	if (c_drawShape == selection) 
		AtlLoadSysCursor(IDC_ARROW);
	
}

void CDrawTool::Add(CDrawRect * pObj)
{
	m_objects.AddTail(pObj);
}

void CDrawTool::Draw(CDCHandle* pDC)
{
	POSITION pos = m_objects.GetHeadPosition();
	while (pos != NULL)
	{
		CDrawRect* pObj = m_objects.GetNext(pos);
		pObj->Draw(pDC);
		if (IsSelected(pObj))
			pObj->DrawTracker(pDC);
	}
}


// point is in logical coordinates
CDrawRect* CDrawTool::ObjectAt(const CPoint& point)
{
	CRect rect(point, CSize(1, 1));
	POSITION pos = m_objects.GetTailPosition();
	while (pos != NULL)
	{
		CDrawRect* pObj = m_objects.GetPrev(pos);
		if (pObj->Intersects(rect))
			return pObj;
	}

	return NULL;
}
