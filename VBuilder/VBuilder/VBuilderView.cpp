// VBuilderView.cpp : implementation of the CVBuilderView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "VBuilderView.h"
#include "Builder.h"

BOOL CVBuilderView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}
void CVBuilderView::DoPaint(CDCHandle dc)
{
	DrawGrid(dc);

	CRect rect;
	GetClientRect(&rect);
	//GetWindowRect
	
	dc.DPtoLP(rect);	

	//CMemDC memDc(dc.m_hDC,&rect);
	//memDc.FillRect(&rect, brush);
	
	if(m_DrawTool !=NULL )
		m_DrawTool->Draw(&dc);

}
void CVBuilderView::AssignDrawTool(CDrawTool *dt)
{
	m_DrawTool = dt;
}
void CVBuilderView::DrawGrid(CDCHandle dc)
{
//	COLORREF oldBkColor = pDC->SetBkColor(pDoc->GetPaperColor());
	// Major unit lines
	CPen penDot;
	HPEN OldPen;

	CBrush brush;
	if (!brush.CreateSolidBrush(RGB(255,255,255)))
		return ;
	CSize size;

	GetScrollSize(size);
	CRect rect(0,0,size.cx ,size.cy);
	dc.FillRect(&rect, brush);
	
	penDot.CreatePen(PS_DOT, 1, RGB(230,230,200));
	OldPen = dc.SelectPen(penDot);

	for (int x = 0; x < size.cx; x += 30)
	{
		if (x != 0)
		{
			dc.MoveTo(x, 0);
			dc.LineTo(x, size.cy);
		}
	}

	for (int y = 0; y < size.cy; y += 30)
	{
		if (y != 0)
		{
			dc.MoveTo(0, y);
			dc.LineTo(size.cx, y);
		}
	}

	dc.SelectPen(OldPen);
//	pDC->SetBkColor(oldBkColor);


}
CPoint CVBuilderView::DPToLP(CPoint& pt)
{
	CSize size;
	CPoint ptoffset;
	CPoint local=pt;

	CDCHandle dc(this->GetDC());
	this->GetScrollSize(size);
	this->GetScrollOffset(ptoffset);
	dc.SetWindowOrg(ptoffset);
	dc.DPtoLP(&local);
	pt=local;
	return local;
}
CPoint CVBuilderView::LPToDP(CPoint& pt)
{
	CSize size;
	CPoint ptoffset;
	CPoint local=pt;

	CDCHandle dc(this->GetDC());
	this->GetScrollSize(size);
	this->GetScrollOffset(ptoffset);
	dc.SetViewportExt(dc.GetDeviceCaps(LOGPIXELSX),dc.GetDeviceCaps(LOGPIXELSY));
	dc.SetWindowOrg(ptoffset);
	dc.SetViewportOrg(0,0);
	dc.LPtoDP(&local);
	pt=local;
	return local;
}

CRect CVBuilderView::LPToDP(CRect& pt)
{
	CSize size;
	CPoint ptoffset;
	CRect local = pt;

	CDCHandle dc(this->GetDC());
	this->GetScrollSize(size);
	this->GetScrollOffset(ptoffset);

	dc.SetViewportExt(dc.GetDeviceCaps(LOGPIXELSX),dc.GetDeviceCaps(LOGPIXELSY));

	dc.SetWindowOrg(ptoffset);
	dc.SetViewportOrg(0,0);
	dc.LPtoDP(&local);
	pt = local;
	return local;
}





void CVBuilderView::OnLButtonDblClk(UINT nFlags, CPoint point)
{

	m_DrawTool->OnLButtonDblClk(this,nFlags,DPToLP(point));

}
void CVBuilderView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_DrawTool->OnLButtonDown(this,nFlags,DPToLP(point));

}
void CVBuilderView::OnMouseMove(UINT nFlags, CPoint point)
{
	m_DrawTool->OnMouseMove(this, nFlags, DPToLP(point));
}

void CVBuilderView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_DrawTool->OnLButtonUp(this, nFlags, DPToLP(point));
}

void CVBuilderView::AssignBuilder( CBuilder * bld )
{
	m_Builder = bld;
	
}

//void CVBuilderView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar)
//{
//	SCROLLINFO sif = { sizeof(SCROLLINFO),SIF_ALL };
//	GetScrollInfo(SB_VERT,&sif);  
//	 int oldPos = sif.nPos;  
//        switch(nSBCode)  
//        {  
//        case SB_TOP:    sif.nPos = sif.nMin;    break;  
//        case SB_BOTTOM: sif.nPos = sif.nMax;    break;  
//        case SB_LINEUP:     sif.nPos -= 12;   break;  
//        case SB_LINEDOWN:   sif.nPos += 12;   break;  
//        case SB_PAGEUP:     sif.nPos -= sif.nPage;  break;  
//        case SB_PAGEDOWN:   sif.nPos += sif.nPage;  break;  
//        case SB_THUMBTRACK: sif.nPos = sif.nTrackPos;   break;  
//        default:    break;  
//        }  
//        if(sif.nPos > sif.nMax)  
//            sif.nPos = sif.nMax;  
//        else if(sif.nPos < sif.nMin)  
//            sif.nPos = sif.nMin;  
//        sif.fMask = SIF_POS;  
//        SetScrollInfo(SB_VERT,&sif);  
//  
//        GetScrollInfo(SB_VERT,&sif);  
//        if(sif.nPos != oldPos)  
//            ScrollWindow(0,oldPos - sif.nPos);  //这里是重点  
//}
//
//void CVBuilderView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar)
//{
//	 SCROLLINFO sif = { sizeof(SCROLLINFO),SIF_ALL };  
//        GetScrollInfo(SB_HORZ,&sif);  
//  
//        int oldPos = sif.nPos;  
//        switch(nSBCode)  
//        {  
//        case SB_LEFT:   sif.nPos = sif.nMin;    break;  
//        case SB_RIGHT:  sif.nPos = sif.nMax;    break;  
//        case SB_LINELEFT:   sif.nPos -= 12;   break;  
//        case SB_LINERIGHT:  sif.nPos += 12;   break;  
//        case SB_PAGELEFT:   sif.nPos -= sif.nPage;  break;  
//        case SB_PAGERIGHT:  sif.nPos += sif.nPage;  break;  
//        case SB_THUMBTRACK: sif.nPos = sif.nTrackPos;   break;  
//        default:    break;  
//        }  
//        if(sif.nPos > sif.nMax)  
//            sif.nPos = sif.nMax;  
//        else if(sif.nPos < sif.nMin)  
//            sif.nPos = sif.nMin;  
//        sif.fMask = SIF_POS;  
//        SetScrollInfo(SB_HORZ,&sif);  
//  
//        GetScrollInfo(SB_HORZ,&sif);  
//        if(sif.nPos != oldPos)  
//            ScrollWindow(oldPos - sif.nPos,0);  
//}
//
//

