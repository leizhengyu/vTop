// VBuilderView.h : interface of the CVBuilderView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DrawTool.h"
#include "Builder.h"

class CVBuilderView : public CScrollWindowImpl<CVBuilderView>
{
public:
	DECLARE_WND_CLASS(NULL)

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CVBuilderView)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		
		//MSG_WM_HSCROLL(OnHScroll)
		//MSG_WM_VSCROLL(OnVScroll)

		MSG_WM_LBUTTONDOWN(OnLButtonDown)  


		MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)

		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_LBUTTONUP(OnLButtonUp)  



		CHAIN_MSG_MAP(CScrollWindowImpl<CVBuilderView>)

	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)




	//LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	void DoPaint(CDCHandle dc);
	
	CPoint DPToLP(CPoint& pt);
	CPoint LPToDP(CPoint& pt);
	CRect LPToDP(CRect& pt);

/*
	 void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar);
	 void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar);*/

	 void OnLButtonDblClk(UINT nFlags, CPoint point);

	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);

	void    DrawGrid(CDCHandle dc);
	void    AssignDrawTool(CDrawTool *dt);
	void AssignBuilder( CBuilder * bld );
public:
	CDrawTool *m_DrawTool;
	CBuilder  *m_Builder;

};
