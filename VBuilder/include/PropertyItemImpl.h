#ifndef __PROPERTYITEMIMPL__H
#define __PROPERTYITEMIMPL__H

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CPropertyItemImpl - Property implementations for the Property controls
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2001-2002 Bjarke Viksoe.
//
// This code may be used in compiled form in any way you desire. This
// source file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//

#ifndef __PROPERTYITEM__H
  #error PropertyItemImpl.h requires PropertyItem.h to be included first
#endif

#ifndef __PROPERTYITEMEDITORS__H
  #error PropertyItemImpl.h requires PropertyItemEditors.h to be included first
#endif

#ifndef __ATLBASE_H__
  #error PropertyItem.h requires atlbase.h to be included first
#endif



/////////////////////////////////////////////////////////////////////////////
// Base CProperty class

class CPropertyItem : public IProperty
{
protected:
   HWND   m_hWndOwner;
   CString m_sName;
   CSimpleArray<CString> m_Vals;

   //LPTSTR m_pszName;

   //LPTSTR m_pszType;
   //LPTSTR m_pszInst;
   //LPTSTR m_pszComment;

   bool   m_fEnabled;
   LPARAM m_lParam;

public:
	CPropertyItem(CString sName,LPARAM lParam) 
		: m_fEnabled(true), m_lParam(lParam), m_hWndOwner(NULL)
	{
		m_sName = sName;	
	}

   CPropertyItem(CString sName,CSimpleArray<CString> sVals,LPARAM lParam) 
	   : m_fEnabled(true), m_lParam(lParam), m_hWndOwner(NULL)
   {

	   m_sName = sName;
	   m_Vals  = sVals;

   //   ATLASSERT(!::IsBadStringPtr(pstrName,-1));
   //   ATLTRY( m_pszName = new TCHAR[ (::lstrlen(pstrName) * sizeof(TCHAR)) + 1 ] );
   //   ATLASSERT(m_pszName);
   //   ::lstrcpy(m_pszName, pstrName);

	  //ATLTRY( m_pszComment = new TCHAR[ (::lstrlen(pstrComment) * sizeof(TCHAR)) + 1 ] );
	  //ATLASSERT(m_pszComment);
	  //::lstrcpy(m_pszComment, pstrComment);

	  //ATLTRY( m_pszType = new TCHAR[ (::lstrlen(pstrType) * sizeof(TCHAR)) + 1 ] );
	  //ATLASSERT(m_pszType);
	  //::lstrcpy(m_pszType, pstrType);

	  //ATLTRY( m_pszInst = new TCHAR[ (::lstrlen(pstrInst) * sizeof(TCHAR)) + 1 ] );
	  //ATLASSERT(m_pszInst);
	  //::lstrcpy(m_pszInst, pstrInst);
   }
   //CProperty(LPCTSTR pstrName, LPARAM lParam) 
	  // : m_fEnabled(true), m_lParam(lParam), m_hWndOwner(NULL)
   //{
	  // ATLASSERT(!::IsBadStringPtr(pstrName,-1));
	  // ATLTRY( m_pszName = new TCHAR[ (::lstrlen(pstrName) * sizeof(TCHAR)) + 1 ] );
	  // ATLASSERT(m_pszName);
	  // ::lstrcpy(m_pszName, pstrName);
	  // m_pszComment=NULL;
	  // m_pszType = NULL;
   //}
   virtual ~CPropertyItem()
   {
	   //delete [] m_pszName;
	   //if (m_pszComment!=NULL)
		  // delete [] m_pszComment;
	   //if (m_pszType!=NULL)
		  // delete [] m_pszType;
   }

   virtual void SetOwner(HWND hWnd, LPVOID /*pData*/)
   {
      ATLASSERT(::IsWindow(hWnd));
      ATLASSERT(m_hWndOwner==NULL); // Cannot set it twice
      m_hWndOwner = hWnd;
   }

   virtual CString GetName() const
   {
      return m_sName; // Dangerous!
   }
   virtual CString GetColumText(UINT col) const
   {
	   return m_Vals[col]; // Dangerous!
   }
   //virtual LPCTSTR GetCInstName() const
   //{
	  // return m_pszInst; // Dangerous!
   //}
   virtual void SetEnabled(BOOL bEnable)
   {
      m_fEnabled = (bEnable == TRUE);
   }

   virtual BOOL IsEnabled() const
   {
      return m_fEnabled;
   }

   virtual void SetItemData(LPARAM lParam)
   {
      m_lParam = lParam;
   }

   virtual LPARAM GetItemData() const
   {
      return m_lParam;
   }

   virtual void DrawName(PROPERTYDRAWINFO& di)
   {
      CDCHandle dc(di.hDC);
      COLORREF clrBack, clrFront;
      if( (di.state & ODS_DISABLED) != 0 ) {
         clrFront = di.clrDisabled;
         clrBack = di.clrBack;
      }
      else if( (di.state & ODS_SELECTED) != 0 ) {
         clrFront = di.clrSelText;
         clrBack = di.clrSelBack;
      }
      else {
         clrFront = di.clrText;
         clrBack = di.clrBack;
      }
      RECT rcItem = di.rcItem;
      dc.FillSolidRect(&rcItem, clrBack);
      rcItem.left += 2; // Indent text
      dc.SetBkMode(TRANSPARENT);
      dc.SetBkColor(clrBack);
      dc.SetTextColor(clrFront);
      dc.DrawText(m_sName, -1, &rcItem, DT_LEFT | DT_SINGLELINE | DT_EDITCONTROL | DT_NOPREFIX | DT_VCENTER);
   }

   virtual void DrawValue(PROPERTYDRAWINFO& di,UINT iCol)
   {
	   if(m_Vals[iCol] == _T("")) return;
	   CDCHandle dc(di.hDC);
	   dc.SetBkMode(TRANSPARENT);
	   dc.SetTextColor((di.state & ODS_DISABLED) != 0 ? di.clrDisabled : di.clrText);
	   dc.SetBkColor(di.clrBack);
	   RECT rcText = di.rcItem;
	   rcText.left += PROP_TEXT_INDENT;	   
	   dc.DrawText(m_Vals[iCol], -1, &rcText, DT_LEFT | DT_SINGLELINE | DT_EDITCONTROL | DT_NOPREFIX | DT_END_ELLIPSIS | DT_VCENTER);
   }
   virtual HWND CreateInplaceControl(HWND /*hWnd*/, const RECT& /*rc*/) 
   { 
      return NULL; 
   }

   virtual BOOL Activate(UINT /*action*/, LPARAM /*lParam*/) 
   { 
      return TRUE; 
   }

   virtual CString GetDisplayValue(UINT iCol) const 
   { 
      return m_Vals[iCol]; 
   }

   virtual UINT GetDisplayValueLength() const 
   { 
      return m_Vals[0].GetLength(); 
   }

   virtual CString GetValue(UINT iCol) const 
   { 

      return m_Vals[iCol]; 
   }

   virtual BOOL SetValue(CString sVal,UINT iCol) 
   { 
      ATLASSERT(false);
      return FALSE; 
   }

   virtual BOOL SetValue(HWND /*hWnd*/) 
   { 
      ATLASSERT(false);
      return FALSE; 
   }

};
 
/////////////////////////////////////////////////////////////////////////////
// ReadOnly property (with enhanced display features)

class CPropertyReadOnlyItem : public CPropertyItem
{
protected:
   UINT m_uStyle;
   HICON m_hIcon;
   COLORREF m_clrBack;
   COLORREF m_clrText;

public:
   CPropertyReadOnlyItem(CString sName,CSimpleArray<CString> sVals,LPARAM lParam) : 
      CPropertyItem(sName,sVals, lParam), 
      m_uStyle( DT_LEFT | DT_SINGLELINE | DT_EDITCONTROL | DT_NOPREFIX | DT_END_ELLIPSIS | DT_VCENTER ),
      m_clrBack(CLR_INVALID),
      m_clrText(CLR_INVALID),
      m_hIcon(NULL)
   {
   }

   void DrawValue(PROPERTYDRAWINFO& di,UINT iCol)
   {
      //// Get property text
      //UINT cchMax = GetDisplayValueLength() + 1;
      //LPTSTR pszText = (LPTSTR) _alloca(cchMax * sizeof(TCHAR));
      //ATLASSERT(pszText);
      //if( !GetDisplayValue(pszText, cchMax) ) return;
      //// Prepare paint
      RECT rcText = di.rcItem;
      CDCHandle dc(di.hDC);
      dc.SetBkMode(OPAQUE);
      // Set background color
      COLORREF clrBack = di.clrBack;
      if( m_clrBack != CLR_INVALID ) clrBack = m_clrBack;
      dc.SetBkColor(clrBack);
      // Set text color
      COLORREF clrText = di.clrText;
      if( m_clrText != CLR_INVALID ) clrText = m_clrText;
      if( (di.state & ODS_DISABLED) != 0 ) clrText = di.clrDisabled; 
      dc.SetTextColor(clrText);
      // Draw icon if available
      if( m_hIcon ) {
         POINT pt = { rcText.left + 2, rcText.top + 2 };
         SIZE sizeIcon = { ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON) };
         ::DrawIconEx(dc, pt.x, pt.y, m_hIcon, sizeIcon.cx, sizeIcon.cy, 0, NULL, DI_NORMAL);
         rcText.left += sizeIcon.cx + 4;
      }
      // Draw text with custom style
      rcText.left += PROP_TEXT_INDENT;
      dc.DrawText(m_Vals[iCol], -1, &rcText, m_uStyle);
   }

   // Operations

   // NOTE: To use these methods, you must cast the HPROPERTY 
   //       handle back to the CPropertyReadOnlyItem class.
   //       Nasty stuff, but so far I've settled with this approach.

   COLORREF SetBkColor(COLORREF clrBack)
   {
      COLORREF clrOld = m_clrBack;
      m_clrBack = clrBack;
      return clrOld;
   }

   COLORREF SetTextColor(COLORREF clrText)
   {
      COLORREF clrOld = m_clrText;
      m_clrText = clrText;
      return clrOld;
   }

   HICON SetIcon(HICON hIcon)
   {
      HICON hOldIcon = m_hIcon;
      m_hIcon = hIcon;
      return hOldIcon;
   }

   void ModifyDrawStyle(UINT uRemove, UINT uAdd)
   {
      m_uStyle = (m_uStyle & ~uRemove) | uAdd;
   }
};


/////////////////////////////////////////////////////////////////////////////
// Simple Value property

class CPropertyEditItem : public CPropertyItem
{
protected:
   HWND m_hwndEdit;

public:
   CPropertyEditItem(CString sName,CSimpleArray<CString> sVals,LPARAM lParam)  : 
      CPropertyItem(sName,sVals,lParam), 
      m_hwndEdit(NULL)
   {
   }

   BYTE GetKind() const 
   { 
      return PROPKIND_EDIT; 
   }

   HWND CreateInplaceControl(HWND hWnd, const RECT& rc) 
   {
      // Get default text
      //UINT cchMax = GetDisplayValueLength() + 1;
      //LPTSTR pszText = (LPTSTR) _alloca(cchMax * sizeof(TCHAR));
      //ATLASSERT(pszText);
      //if( !GetDisplayValue(pszText, cchMax) ) return NULL;
      // Create EDIT control
      CPropertyEditWindow* win = new CPropertyEditWindow();
      ATLASSERT(win);
      RECT rcWin = rc;
      m_hwndEdit = win->Create(hWnd, rcWin, m_Vals[0], WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL);
      ATLASSERT(::IsWindow(m_hwndEdit));
      // Simple hack to validate numbers
      //switch( m_val.vt ) {
      //case VT_UI1:
      //case VT_UI2:
      //case VT_UI4:
      //   win->ModifyStyle(0, ES_NUMBER);
      //   break;
      //}
      return m_hwndEdit;
   }

   BOOL SetValue(CString sVal,UINT iCol)
   {
	   m_Vals[iCol] = sVal;
      //if( m_val.vt == VT_EMPTY ) m_val = value;
      //return SUCCEEDED( m_val.ChangeType(m_val.vt, &value) );
	  return 0;
   }

   BOOL SetValue(HWND hWnd) 
   { 
      ATLASSERT(::IsWindow(hWnd));
      int len = ::GetWindowTextLength(hWnd) + 1;
      LPTSTR pstr = (LPTSTR) _alloca(len * sizeof(TCHAR));
      ATLASSERT(pstr);
      if( ::GetWindowText(hWnd, pstr, len) == 0 ) {
         // Bah, an empty string *and* an error causes the same return code!
         if( ::GetLastError() != ERROR_SUCCESS ) return FALSE;
      }
      return SetValue(pstr,0);
   }

   BOOL Activate(UINT action, LPARAM /*lParam*/)
   {
      switch( action ) {
      case PACT_TAB:
      case PACT_SPACE:
      case PACT_DBLCLICK:
         if( ::IsWindow(m_hwndEdit) ) {
            ::SetFocus(m_hwndEdit);
            ::SendMessage(m_hwndEdit, EM_SETSEL, 0, -1);
         }
         break;
      }
      return TRUE;
   }
};



/////////////////////////////////////////////////////////////////////////////
//
// CProperty creators
//

inline HPROPERTY PropCreateSimple(CString sName, CSimpleArray<CString> sVals,LPARAM lParam = 0)
{
   return new CPropertyEditItem(sName, sVals,lParam);
}

#endif // __PROPERTYITEMIMPL__H
