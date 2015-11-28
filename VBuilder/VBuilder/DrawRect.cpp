#include "StdAfx.h"
#include "InstSettingsDlg.h"
#include "DrawRect.h"
#include "VBuilderView.h"


CDrawRect::CDrawRect(void)
{
	m_position.top = 0;
	m_position.left =0;
	m_position.right =100;
	m_position.bottom =100;
}

CDrawRect::CDrawRect(const CRect &re)
{
	m_position.top = re.top;
	m_position.left =re.left;
	m_position.right =re.right;
	m_position.bottom =re.bottom;
}

CDrawRect::~CDrawRect(void)
{
}
void CDrawRect::Draw(CDCHandle* pDC)
{
	CBrush   brush;
	CPen     pen;
	HBRUSH   pOldBrush;
	HPEN     pOldPen;

	LOGBRUSH logBrush;
	LOGPEN   logPen;

	logPen.lopnStyle	= PS_INSIDEFRAME;
	logPen.lopnWidth.x	= 1;
	logPen.lopnWidth.y	= 1;
	logPen.lopnColor	= RGB(0, 0, 0);

	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = RGB(192, 192, 192);
	logBrush.lbHatch = HS_HORIZONTAL;

	if (!brush.CreateBrushIndirect(&logBrush))
		return;
	if (!pen.CreatePenIndirect(&logPen))
		return;


	pOldBrush = pDC->SelectBrush(brush);
	pOldPen = pDC->SelectPen(pen);

	
	

	pDC->Rectangle(m_position);

	CString strInst = sInst + "\n(" + sModule + ")";
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(strInst, strInst.GetLength(), m_position,DT_CENTER|DT_VCENTER);

	pDC->SelectBrush(pOldBrush);
	pDC->SelectPen(pOldPen);

}

void CDrawRect::DrawTracker(CDCHandle* pDC)
{
	int nHandleCount = 8;
	for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
	{
		CPoint handle = GetHandle(nHandle);
		pDC->PatBlt(handle.x - 3, handle.y - 3, 7, 7, DSTINVERT);
	}

}

// Note: if bSelected, hit-codes start at one for the top-left
// and increment clockwise, 0 means no hit.
// If !bSelected, 0 = no hit, 1 = hit (anywhere)

// point is in logical coordinates
int CDrawRect::HitTest(CPoint point, CVBuilderView* pView, BOOL bSelected)
{

	if (bSelected)
	{
		int nHandleCount = 8;
		for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
		{
			// GetHandleRect returns in logical coords
			CRect rc = GetHandleRect(nHandle,pView);
			if (point.x >= rc.left && point.x < rc.right &&
				point.y >= rc.top && point.y < rc.bottom)
				return nHandle;
		}
	}
	else
	{
		if (point.x >= m_position.left && point.x < m_position.right &&
			point.y >= m_position.top && point.y < m_position.bottom)
			return 1;
	}
	return 0;
}

// return rectange of handle in logical coords
CRect CDrawRect::GetHandleRect(int nHandleID, CVBuilderView* pView)
{

	CRect rect;
	// get the center of the handle in logical coords
	CPoint point = GetHandle(nHandleID);
	// convert to client/device coords
//	pView->DocToClient(point);
	// return CRect of handle in device coords
	rect.SetRect(point.x-3, point.y-3, point.x+3, point.y+3);
//	pView->ClientToDoc(rect);

	return rect;
}

// returns logical coords of center of handle
CPoint CDrawRect::GetHandle(int nHandle)
{

	int x, y, xCenter, yCenter;

	// this gets the center regardless of left/right and top/bottom ordering
	xCenter = m_position.left + m_position.Width() / 2;
	yCenter = m_position.top + m_position.Height() / 2;

	switch (nHandle)
	{
	default:

	case 1:
		x = m_position.left;
		y = m_position.top;
		break;

	case 2:
		x = xCenter;
		y = m_position.top;
		break;

	case 3:
		x = m_position.right;
		y = m_position.top;
		break;

	case 4:
		x = m_position.right;
		y = yCenter;
		break;

	case 5:
		x = m_position.right;
		y = m_position.bottom;
		break;

	case 6:
		x = xCenter;
		y = m_position.bottom;
		break;

	case 7:
		x = m_position.left;
		y = m_position.bottom;
		break;

	case 8:
		x = m_position.left;
		y = yCenter;
		break;
	}

	return CPoint(x, y);
}

HCURSOR CDrawRect::GetHandleCursor(int nHandle)
{

	LPCTSTR id;
	switch (nHandle)
	{
	default:

	case 1:
	case 5:
		id = IDC_SIZENWSE;
		break;

	case 2:
	case 6:
		id = IDC_SIZENS;
		break;

	case 3:
	case 7:
		id = IDC_SIZENESW;
		break;

	case 4:
	case 8:
		id = IDC_SIZEWE;
		break;
	}

	return AtlLoadSysCursor(id);
}

// rect must be in logical coordinates
BOOL CDrawRect::Intersects(const CRect& rect)
{
	CRect fixed = m_position;
	fixed.NormalizeRect();
	CRect rectT = rect;
	rectT.NormalizeRect();
	return !(rectT & fixed).IsRectEmpty();
}

// position is in logical
void CDrawRect::MoveTo(const CRect& position, CVBuilderView* pView)
{
	if (position == m_position)
		return;

	CRect rect = m_position;
//	DocToClient(rect);
	m_position=position;

	rect.left -= 4;
	rect.top -= 5;
	rect.right += 5;
	rect.bottom += 4;

	rect.InflateRect(1, 1); // handles CDrawOleObj objects
	pView->LPToDP(rect);	
	pView->InvalidateRect(rect);

	m_position = position;
	rect = position;
	pView->LPToDP(rect);	
	pView->InvalidateRect(rect);
}


void CDrawRect::MoveHandleTo(int nHandle, CPoint point, CVBuilderView* pView)
{
	CRect position = m_position;
	switch (nHandle)
	{
	default:

	case 1:
		position.left = point.x;
		position.top = point.y;
		break;

	case 2:
		position.top = point.y;
		break;

	case 3:
		position.right = point.x;
		position.top = point.y;
		break;

	case 4:
		position.right = point.x;
		break;

	case 5:
		position.right = point.x;
		position.bottom = point.y;
		break;

	case 6:
		position.bottom = point.y;
		break;

	case 7:
		position.left = point.x;
		position.bottom = point.y;
		break;

	case 8:
		position.left = point.x;
		break;
	}

	MoveTo(position, pView);
}

void CDrawRect::OnOpen(CVBuilderView* pView)
{
	CInstSettingsDlg dlg;
	
	dlg.SetInst(m_pInst);
	
	if(dlg.DoModal() == IDOK) {
		if (sInst != m_pInst->InstName) {
			pView->m_Builder->mInsts.RemoveKey(sInst);
			pView->m_Builder->mInsts.SetAt(m_pInst->InstName,m_pInst);
			sInst = m_pInst->InstName;
		}
		
		pView->Invalidate();
	}
	
}

void CDrawRect::Remove()
{
	delete this;
}
