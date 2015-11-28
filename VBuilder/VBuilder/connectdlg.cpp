// connectdlg.cpp : implementation of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "connectdlg.h"

LRESULT CConnectDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{

	m_LeftctrlList.SubclassWindow(GetDlgItem(IDC_LEFTLIST));
	m_LeftctrlList.SetExtendedListStyle(PLS_EX_CATEGORIZED | PLS_EX_XPLOOK|PLS_EX_SHOWSELALWAYS);
	m_RightctrlList.SubclassWindow(GetDlgItem(IDC_RIGHTLIST));
	m_RightctrlList.SetExtendedListStyle(PLS_EX_CATEGORIZED | PLS_EX_XPLOOK|PLS_EX_SHOWSELALWAYS);

	CenterWindow(GetParent());
	InitPropertyList();
	DlgResize_Init(false, true, WS_CLIPCHILDREN);

	CheckDlgButton(IDC_LEFTALL, true);
	CheckDlgButton(IDC_RIGHTALL, true);
	m_rType =0;
	m_lType =0;
//	SendMessage(WM_SIZE,0,0);
	return TRUE;
}



void CConnectDlg::UpdatePropertyList(int LeftRight,int nType ,CString sFilter)
{
	UINT iCnt;
	HPROPERTY prop;
	CPropertyListCtrl *list;
	
	if(LeftRight == 0) {
		list = &m_LeftctrlList; 
	}
	else {
		list = &m_RightctrlList; 
	}



	
	iCnt = list->GetCount();
	while (iCnt>0) {
		prop = list->GetProperty(iCnt-1);
		list->DeleteItem(prop);				
		iCnt = list->GetCount();
		}

	CString sTypeName,sComment;

	POSITION pos = m_builder->mInsts.GetStartPosition();
	while(pos!=0) {
		CInst* inst = m_builder->mInsts.GetNextValue(pos);		
		CString strFmt = inst->InstName + "(" + inst->ModuleName + ")";

		list->AddItem( PropCreateCategory(strFmt) );

		POSITION idx = inst->mLinks.GetStartPosition();
		while(idx!=0) {
			CAtlMap<CStringA,TLink>::CPair *xp = inst->mLinks.GetNext(idx);
			CAtlMap<CStringA,TWire>::CPair *xw = m_builder->mWires.Lookup(xp->m_value.wName);
			if (xw == NULL) {
				sTypeName = "";
				if (xp->m_value.wType=="c" || xp->m_value.wType=="n") 
					sComment = "";
				else 
					sComment = "Error:not connected";
			}
			else { 
				sTypeName = xw->m_value.GetTypeName();
				if(xp->m_value.iWidth != xw->m_value.iMsb - xw->m_value.iLsb +1){
					sComment = "bit width error";
				}
				else {
					sComment = "";
				}
			}

			CSimpleArray<CString> sVal;
			CString sName = xp->m_value.wName;
			CString sInst = inst->InstName;
			sVal.Add(sName);
			sVal.Add(sTypeName);
			sVal.Add(sComment);
			sVal.Add(sInst);
			
			CString sKey = xp->m_value.ioName;
			
			if ((xp->m_value.ioType == "i" && LeftRight ==0 ) || (xp->m_value.ioType == "o" && LeftRight ==1)) {
				
				CAtlRegExp<CAtlRECharTraitsA>  re;
				CAtlREMatchContext<CAtlRECharTraitsA> mc ;
				re.Parse(sFilter);

				if (re.Match(sKey,&mc) || re.Match(sName,&mc)) {
					if(nType==0) {
						list->AddItem( PropCreateSimple(sKey,sVal) );
					}
					else if (nType == 1) {
						if(xp->m_value.wName == "") {
							list->AddItem( PropCreateSimple(sKey,sVal) );
						}
					}
					else if (nType == 2) {
						if(xw != NULL) {
							if(xp->m_value.iWidth != xw->m_value.iMsb - xw->m_value.iLsb+1) {
								list->AddItem( PropCreateSimple(sKey,sVal) );
							}
						}
					}
				}
			}
		}
	}	
}


void CConnectDlg::InitPropertyList()
{
	CString sTypeName,sComment;

	POSITION pos = m_builder->mInsts.GetStartPosition();
	while(pos!=0) {
		CInst* inst = m_builder->mInsts.GetNextValue(pos);
		
		CString strFmt = inst->InstName + "(" + inst->ModuleName + ")";

		m_LeftctrlList.AddItem( PropCreateCategory(strFmt) );
		m_RightctrlList.AddItem( PropCreateCategory(strFmt) );

		POSITION idx = inst->mLinks.GetStartPosition();
		while(idx!=0) {
			CAtlMap<CStringA,TLink>::CPair *xp = inst->mLinks.GetNext(idx);
			CAtlMap<CStringA,TWire>::CPair *xw = m_builder->mWires.Lookup(xp->m_value.wName);
			if (xw == NULL) {
				sTypeName = "";
				if (xp->m_value.wType=="c" || xp->m_value.wType=="n") 
					sComment = "";
				else 
					sComment = "Error:not connected";
			}
			else { 
				sTypeName = xw->m_value.GetTypeName();
				if(xp->m_value.iWidth != xw->m_value.iMsb - xw->m_value.iLsb+1){
					sComment = "bit width error";
				}
				else {
					sComment = "";
				}
			}

			CSimpleArray<CString> sVal;
			CString sName = xp->m_value.wName;
			CString sInst = inst->InstName;
			sVal.Add(sName);
			sVal.Add(sTypeName);
			sVal.Add(sComment);
			sVal.Add(sInst);
			
			CString sKey = xp->m_value.ioName;
			
			if (xp->m_value.ioType == "i") {				
				m_LeftctrlList.AddItem( PropCreateSimple(sKey,sVal) );
			}
			else {
				m_RightctrlList.AddItem( PropCreateSimple(sKey,sVal) );
			}
		}
	}

}

void CConnectDlg::SetBuilder(CBuilder* builder)
{
	m_builder = builder;
}

LRESULT CConnectDlg::OnConnect(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CStringA sInst;
	UINT iMsb;
	CStringA sLink;
	CStringA strName;

	UINT i_idx,o_idx;
	HPROPERTY i_prop,o_prop;
	CStringA iPort,oPort;
	CStringA iLink,oLink;
	CInst   *inst;

	i_idx = m_LeftctrlList.GetCurSel();
	o_idx = m_RightctrlList.GetCurSel();

	if(i_idx != LB_ERR && o_idx != LB_ERR) {
		i_prop = m_LeftctrlList.GetProperty(i_idx);
		o_prop = m_RightctrlList.GetProperty(o_idx);
		if( i_prop->GetKind() != PROPKIND_CATEGORY && o_prop->GetKind() != PROPKIND_CATEGORY ) {


			iPort = i_prop->GetName();
			oPort = o_prop->GetName();
			
			iLink= i_prop->GetValue(0);
			oLink= o_prop->GetValue(0);


			if (iLink != "" && oLink!="")	{
				MessageBox(_T("already linked!"));
			}
			else if (iLink!="") {
				CString wLink = iLink;
				o_prop->SetValue(wLink,0);
				sInst = o_prop->GetValue(3);
				inst = m_builder->mInsts[sInst];
				inst->mLinks[oPort].wName = iLink;

				sLink = iLink;
				iMsb = inst->mLinks[oPort].iWidth - 1;
			}
			else if (oLink != "") {
				CString wLink = oLink;
				i_prop->SetValue(wLink,0);
				sInst = o_prop->GetValue(3);
				inst = m_builder->mInsts[sInst];
				inst->mLinks[iPort].wName = oLink;
				sLink = oLink;
				sInst = o_prop->GetValue(3);
				iMsb = m_builder->mInsts[sInst]->mLinks[iPort].iWidth - 1;
			}
			else {
				CString wPort = iPort;
				i_prop->SetValue(wPort,0);
				o_prop->SetValue(wPort,0);
				sInst = i_prop->GetValue(3);
				m_builder->mInsts[sInst]->mLinks[iPort].wName = iPort;
				sInst = o_prop->GetValue(3);
				m_builder->mInsts[sInst]->mLinks[oPort].wName = iPort;
				
				sLink = iPort;
				sInst = o_prop->GetValue(3);
				iMsb = m_builder->mInsts[sInst]->mLinks[oPort].iWidth - 1;

			}

			if (sLink != "") {
				CAtlMap<CStringA,TWire>::CPair *xp = m_builder->mWires.Lookup(sLink);

				if (xp == NULL) {  //Not Defined ,then
					TWire wire;
					wire.sName = iLink;
					wire.iMsb = iMsb;
					wire.iLsb = 0;
					wire.sType = "w";
					m_builder->mWires[sLink] = wire;
				}
				else {
					m_builder->mWires[sLink].sType = "w";
				}

			}

			m_LeftctrlList.InvalidateItem(i_idx);
			m_RightctrlList.InvalidateItem(o_idx);
		}
	}

	UpdateLinks();

	return 0;
}
LRESULT CConnectDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}
//LRESULT CConnectDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)

void CConnectDlg::OnSize(UINT nType, CSize size)
{
	BOOL bHandled;
//	CDialogResize<CConnectDlg>::OnSize(WM_SIZE, size.cx , size.cy, bHandled);
//	UINT nType = (UINT)wParam;
//	CSize size = _WTYPES_NS::CSize(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

	int iwidth = size.cx/2 - 100;  
	CRect rect;
	UINT  iTop=100;

	if (nType != SIZE_MINIMIZED ) {
		
		GetDlgItem(IDC_LEFTLIST).GetWindowRect(rect);

		GetDlgItem(IDC_LEFT_FRM).MoveWindow(10,10,iwidth,80);
		GetDlgItem(IDC_RIGHT_FRM).MoveWindow(size.cx/2+90,10,iwidth,80);

		GetDlgItem(IDC_LEFTALL).MoveWindow(50,25,60,30);
		GetDlgItem(IDC_RIGHTALL).MoveWindow(size.cx/2+140,25,60,30);

		GetDlgItem(IDC_LEFTNC).MoveWindow(110,25,60,30);
		GetDlgItem(IDC_RIGHTNC).MoveWindow(size.cx/2+200,25,60,30);

		GetDlgItem(IDC_LEFTERR).MoveWindow(170,25,60,30);
		GetDlgItem(IDC_RIGHTERR).MoveWindow(size.cx/2+260,25,60,30);

		GetDlgItem(IDC_STATICL).MoveWindow(15,60,60,30);
		GetDlgItem(IDC_STATICR).MoveWindow(size.cx/2+105,60,60,30);
		GetDlgItem(IDC_LEFTFILTER).MoveWindow(50,60,iwidth-50,20);
		GetDlgItem(IDC_RIGHTFILTER).MoveWindow(size.cx/2+140,60,iwidth-60,20);


		GetDlgItem(IDC_LEFTLIST).MoveWindow(10,iTop,iwidth,rect.Height());		
		GetDlgItem(IDC_RIGHTLIST).MoveWindow(size.cx/2+90,iTop,iwidth,rect.Height());

		GetDlgItem(IDC_LCLEAR).MoveWindow(size.cx/2-75,iTop+30,70,25);
		GetDlgItem(IDC_RCLEAR).MoveWindow(size.cx/2+5,iTop+30,70,25);

		GetDlgItem(IDC_LAUTOPORT).MoveWindow(size.cx/2-75,iTop+60,70,25);
		GetDlgItem(IDC_RAUTOPORT).MoveWindow(size.cx/2+5,iTop+60,70,25);

		GetDlgItem(IDC_LTIE0).MoveWindow(size.cx/2-75,iTop+90,70,25);
		GetDlgItem(IDC_RTIE0).MoveWindow(size.cx/2+5,iTop+90,70,25);

		GetDlgItem(IDC_LTIE1).MoveWindow(size.cx/2-75,iTop+120,70,25);
		GetDlgItem(IDC_RTIE1).MoveWindow(size.cx/2+5,iTop+120,70,25);

		GetDlgItem(IDC_LNOTCARE).MoveWindow(size.cx/2-75,iTop+150,70,25);
		GetDlgItem(IDC_RNOTCARE).MoveWindow(size.cx/2+5,iTop+150,70,25);

		GetDlgItem(IDC_SIGLIST).MoveWindow(size.cx/2-200,iTop+150,100,40);
		GetDlgItem(IDC_CLOSE).MoveWindow(size.cx/2+109,iTop+150,100,40);

	}

//	CDialogResize<CConnectDlg>::OnSize(uMsg, wParam, lParam,bHandled);
//	CDialogResize<CConnectDlg>::OnSize(WM_SIZE, size.cx , size.cy, bHandled);
	SetMsgHandled(FALSE);

//	return 0;
}


LRESULT CConnectDlg::OnLinkTo(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	HPROPERTY prop;
	int idx;
	CPropertyListCtrl *plist;
	CStringA sLink;
	CStringA sType;
	CStringA sName;
	CStringA sInst;

	switch(wID) {
		case IDC_LCLEAR : 
		case IDC_LTIE0 :
		case IDC_LTIE1 :
			plist = &m_LeftctrlList;
			break;
		case IDC_RCLEAR :
		case IDC_RTIE0 :
		case IDC_RTIE1 :
			plist = &m_RightctrlList;
			break;
		default: break;
	}

	idx = plist->GetCurSel();
	if(idx != LB_ERR) {
		prop = plist->GetProperty(idx);
		sName = prop->GetName();
		if( prop->GetKind() != PROPKIND_CATEGORY) {
			sInst = prop->GetValue(3);

			UINT iW = m_builder->mInsts[sInst]->mLinks[sName].iWidth;

			switch(wID) {
				case IDC_LCLEAR : 
				case IDC_RCLEAR :
					sLink = "";
					sType = "e";
					break;
				case IDC_LTIE0 :
				case IDC_RTIE0 :
					sLink.Format("%d'h0",iW);
					sType = "c";
					break;
				case IDC_LTIE1 :
				case IDC_RTIE1 :
					sLink.Format("%d'h0",iW);
					sType = "c";
					break;
				default: break;
			}
			m_builder->mInsts[sInst]->mLinks[sName].wName = sLink;
			m_builder->mInsts[sInst]->mLinks[sName].wType = sType;

			CString wLink = sLink;
			prop->SetValue(wLink,0);
			plist->InvalidateItem(idx);
		}
	}
	UpdateLinks();
	return 0;
}

LRESULT CConnectDlg::OnAutoPort(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CStringA sInst;
	HPROPERTY prop;
	int idx;
	CPropertyListCtrl *plist;
	CStringA sLink;
	CStringA sName;
	CStringA sType;
	CString sDirection;

	switch(wID)
	{
	case IDC_LAUTOPORT:
		plist = &m_LeftctrlList;
		sType = "i";	
		sDirection = "input";
		break;
	case IDC_RAUTOPORT:
		plist = &m_RightctrlList;
		sType = "o";
		sDirection = "output";
		break;
	default:
		break;
	}
	
	idx = plist->GetCurSel();
	if(idx != LB_ERR) {
		prop = plist->GetProperty(idx);
		if( prop->GetKind() != PROPKIND_CATEGORY) {
			sLink = prop->GetValue(0);
			if (sLink=="")  // Not Connected
			{
				sLink = prop->GetName();
				prop->SetValue(prop->GetName(),0);
				prop->SetValue(sDirection,1);
				prop->SetValue("",2);
			}

			
			sName = prop->GetName();
			sInst = prop->GetValue(3);
			CInst *inst = m_builder->mInsts[sInst];

			inst->mLinks[sName].wName = sLink;
			inst->mLinks[sName].wType = sType;

			CAtlMap<CStringA,TWire>::CPair *xp = m_builder->mWires.Lookup(sLink);
			if (xp==NULL) {   //Port define not exist
				TWire port;
				port.sName = sLink;
				port.sType = sType;
				port.iMsb  = inst->mLinks[sLink].iWidth - 1;
				port.iLsb  = 0;
				m_builder->mWires.SetAt(port.sName,port);			
			}	
			else {
				m_builder->mWires[sLink].sType = sType;
			}
			plist->InvalidateItem(idx);
		}
	}
	UpdateLinks();

	return 0;
}


LRESULT CConnectDlg::OnPropertyItemChanged(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/)
{
	CPropertyListCtrl *plist;
	HPROPERTY prop=((NMPROPERTYITEM*)pnmh)->prop;
	switch(idCtrl)
	{
	case IDC_LEFTLIST:
		plist = &m_LeftctrlList;
		break;
	case IDC_RIGHTLIST:
		plist = &m_RightctrlList;
		break;
	default:
		break;
	}
	
	CAtlRegExp<CAtlRECharTraitsA>  re;
	CAtlREMatchContext<CAtlRECharTraitsA> mc ;

	CStringA sVal = prop->GetValue(0);
	
	re.Parse("^(\\d+'[hdb])?[0-9a-fA-F]+");
		
	if (sVal!= "") {
		CStringA sInst = prop->GetValue(3);
		CStringA sName = prop->GetName();
		m_builder->mInsts[sInst]->mLinks[sName].wName = sVal;
		if (re.Match(sVal,&mc)) {
			m_builder->mInsts[sInst]->mLinks[sName].wType = "c";
		}
		else {
			CAtlMap<CStringA,TWire>::CPair *xp=m_builder->mWires.Lookup(sVal);
			if (xp==NULL) {
				TWire wire;
				wire.sName = sVal;
				wire.sType = "w";
				wire.iMsb  = m_builder->mInsts[sInst]->mLinks[sName].iWidth - 1;
				wire.iLsb  = 0;
				m_builder->mWires[sVal] = wire;

				m_builder->mInsts[sInst]->mLinks[sName].wType = "w";
			}
		}
	}
	UpdateLinks();

	return 0;
}


void CConnectDlg::UpdateLinks() 
{
	CString sTypeName;
	CStringA sPort;
	CStringA sInst;
	CString sType;
	CString sComment;
	CStringA sLink;
	UINT iCnt;
	HPROPERTY prop;

	iCnt = m_LeftctrlList.GetCount();
	
	for (UINT i=0;i<iCnt;i++) {
		prop = m_LeftctrlList.GetProperty(i);
		if (prop->GetKind() != PROPKIND_CATEGORY) {
			sLink = prop->GetValue(0);
			sInst = prop->GetValue(3);
			sPort = prop->GetName();

			sType = m_builder->mInsts[sInst]->mLinks[sPort].wType;

			CAtlMap<CStringA,TWire>::CPair *xp = m_builder->mWires.Lookup(sLink);

			if (xp == NULL) {
				sTypeName = "";
				if (sType=="c" || sType=="n") 
					sComment = ("");
				else 
					sComment = "Error:not connected";
			}
			else { 
				sTypeName = xp->m_value.GetTypeName();
				sComment = "";
			}
			prop->SetValue(sTypeName,1);
			prop->SetValue(sComment,2);
		}		
	}	

	iCnt = m_RightctrlList.GetCount();

	for (UINT i=0;i<iCnt;i++) {
		prop = m_RightctrlList.GetProperty(i);
		if (prop->GetKind() != PROPKIND_CATEGORY) {
			sLink = prop->GetValue(0);
			sInst = prop->GetValue(3);
			sPort = prop->GetName();

			sType = m_builder->mInsts[sInst]->mLinks[sPort].wType;

			CAtlMap<CStringA,TWire>::CPair *xp = m_builder->mWires.Lookup(sLink);

			if (xp == NULL) {
				sTypeName = "";
				if (sType=="c" || sType=="n") 
					sComment = "";
				else 
					sComment = "Error:not connected";
			}
			else { 
				sTypeName = xp->m_value.GetTypeName();
				sComment = "";
			}
			prop->SetValue(sTypeName,1);
			prop->SetValue(sComment,2);
		}		
	}	

	m_LeftctrlList.Invalidate();
	m_RightctrlList.Invalidate();
	
}


LRESULT CConnectDlg::OnUpdateList(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{

	switch(wID){
		case IDC_LEFTALL : 
			m_pos=0;
			m_lType=0;
			break;
		case IDC_LEFTNC  : 
			m_pos=0;
			m_lType=1;
			break;
		case IDC_LEFTERR :
			m_pos=0;
			m_lType=2;
			break;
		case IDC_RIGHTALL : 
			m_pos=1;
			m_rType=0;
			break;
		case IDC_RIGHTNC  : 
			m_pos=1;
			m_rType=1;
			break;
		case IDC_RIGHTERR : 
			m_pos=1;
			m_rType=2;
			break;
	}

	if(m_pos==0){
		GetDlgItemText(IDC_LEFTFILTER,m_sFilter);
		UpdatePropertyList(0,m_lType,m_sFilter);
	}
	else {
		GetDlgItemText(IDC_RIGHTFILTER,m_sFilter);
		UpdatePropertyList(1,m_rType,m_sFilter);
	}



	return 0;
}
	
void CConnectDlg::OnFilterChange(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if(nID == IDC_LEFTFILTER) {
		GetDlgItemText(IDC_LEFTFILTER,m_sFilter);
		UpdatePropertyList(0,m_lType,m_sFilter);
	}
	else {
		GetDlgItemText(IDC_LEFTFILTER,m_sFilter);
		UpdatePropertyList(1,m_rType,m_sFilter);
	}
}