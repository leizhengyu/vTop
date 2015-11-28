// OutputDockingWindow.h: interface for the COutputDockingWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPUTDOCKINGWINDOW_H__0D6B82E7_6A06_4FCC_B621_B21ED1C742E9__INCLUDED_)
#define AFX_OUTPUTDOCKINGWINDOW_H__0D6B82E7_6A06_4FCC_B621_B21ED1C742E9__INCLUDED_

#include <ExtDockingWindow.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


 class CMyRichEdit : public CWindowImpl<CMyRichEdit, CRichEditCtrl>,
                     public CRichEditCommands<CMyRichEdit>
 {
 public:
      BEGIN_MSG_MAP(CMyRichEdit)
			  MESSAGE_HANDLER(WM_RBUTTONUP, OnRButtonUp)
			  //COMMAND_ID_HANDLER(ID_CLEAR, OnClear)
              CHAIN_MSG_MAP_ALT(CRichEditCommands<CMyRichEdit>, 1)
      END_MSG_MAP()

	  LRESULT OnRButtonUp(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	  {
		  CMenu popmenu;
		  popmenu.CreatePopupMenu();		  
		  popmenu.AppendMenu(0,ID_EDIT_CLEAR_ALL,_T("Çå³ýÐÅÏ¢"));

		  POINT pos;
		  GetCursorPos(&pos);
		  popmenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, m_hWnd);
		  return 0;
	  }

	  //LRESULT OnClear(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	  //{
		 // this->SetWindowText(_T(""));
		 // return 0;
	  //}
      // other stuff...
};




























class COutputDockingWindow : 
	public dockwins::CTitleDockingWindowImpl< COutputDockingWindow,CWindow,dockwins::COutlookLikeTitleDockingWindowTraits>
{
	typedef COutputDockingWindow thisClass;
	typedef dockwins::CTitleDockingWindowImpl< COutputDockingWindow,CWindow,dockwins::COutlookLikeTitleDockingWindowTraits> baseClass;
public:
    DECLARE_WND_CLASS(_T("COutputDockingWindow"))
	BEGIN_MSG_MAP(thisClass)	
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_SIZE, OnSize)

		CHAIN_MSG_MAP(baseClass)		
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		m_edit.Create(m_hWnd,NULL, NULL, ES_MULTILINE | WS_CHILD | WS_VISIBLE );
		//m_edit.SetReadOnly();
		
		//m_edit.AppendText(_T("some text here...\r\n"));	
		//m_edit.AppendText(_T("some text here...\r\n"));	
		//m_edit.AppendText(_T("some text here...\r\n"));	
		//m_edit.SetEventMask(ENM_MOUSEEVENTS); 
		return 0;
	}	

	LRESULT OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if(wParam != SIZE_MINIMIZED )
		{
			RECT rc;
			GetClientRect(&rc);
			m_edit.SetWindowPos(NULL, &rc ,SWP_NOZORDER | SWP_NOACTIVATE );
		}		
		bHandled = FALSE;
		return 1;
	}


public:
	CMyRichEdit m_edit;
};

#endif // !defined(AFX_OUTPUTDOCKINGWINDOW_H__0D6B82E7_6A06_4FCC_B621_B21ED1C742E9__INCLUDED_)
