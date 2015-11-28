// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "NewDlg.h"
#include "VBuilderView.h"
#include "MainFrm.h"
#include "Builder.h"
#include "DrawRect.h"
#include "connectdlg.h"

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(dockwins::CDockingFrameImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;
	
	return m_view.PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	UIUpdateToolBar();

	UISetCheck(ID_VIEW_TREEPANE, m_foldersDockWnd.IsVisible());
	UISetCheck(ID_VIEW_OUTPUT, m_outputDockWnd.IsVisible());

	return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// create command bar window
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	//m_CmdBar.LoadImages(IDR_MAINFRAME);

	// remove old menu
	SetMenu(NULL);
	
	

	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_TOOLBAR, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

	CreateSimpleStatusBar();


	m_hWndClient = m_view.Create(m_hWnd, rcDefault,  NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		

	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	CMenuHandle menuMain = m_CmdBar.GetMenu();
	
	m_view.SetWindowMenu(menuMain.GetSubMenu(WINDOW_MENU_POSITION));




	//////////////////////////////////////////////////////////////////////////
	InitializeDockingFrame();

	DWORD dwStyle= WS_OVERLAPPEDWINDOW | WS_POPUP| WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	CRect rcDef(0,0,100,100);

	m_foldersDockWnd.Create(m_hWnd,rcDef,_T("Ä£¿é"),dwStyle);
	DockWindow(m_foldersDockWnd,dockwins::CDockingSide(dockwins::CDockingSide::sLeft),
		0/*nBar*/,float(0.0)/*fPctPos*/,200/*nWidth*/,100/* nHeight*/);

	m_outputDockWnd.Create(m_hWnd,rcDef,_T("Êä³ö´°¿Ú"),dwStyle);
	DockWindow(m_outputDockWnd,	dockwins::CDockingSide(dockwins::CDockingSide::sBottom),
		0/*nBar*/,float(0.0)/*fPctPos*/,200/*nWidth*/,100/* nHeight*/);


	//////////////////////////////////////////////////////////////////////////
	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	for (int i=0;i<ModArr.GetSize();i++) {
		delete ModArr[i];
	}
	for (int i=0;i<DTArr.GetSize();i++) {
		delete DTArr[i];
	}

	bHandled = FALSE;
	return 1;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CNewDlg dlg;
	CStringA strTmp;
	CString sFileName;

	if(IDOK==dlg.DoModal()) {
		CBuilder *bld = new CBuilder;
		CDrawTool *dt = new CDrawTool;
		TCHAR sFullPath[1024]={0};
		//bld->Load("VerilogTemplate.v");
		
		sFileName = dlg.m_modName.MakeLower().Trim() + ".v" ;

		bld->mFileName = ::PathCombine(sFullPath,dlg.m_Path,sFileName);
		bld->mModuleName = dlg.m_modName.MakeUpper().Trim();

		strTmp.Format("// FileName    : %s",sFileName);
		bld->mSegments[0].mLines.Add(strTmp);
		bld->mSegments[0].mLines.Add("// Author      : VBuilder");
	    SYSTEMTIME stUTC;  
		::GetSystemTime(&stUTC);
		CTime ct;
		ct = CTime::GetCurrentTime();
		CString tmp = ct.Format(_T("%Y-%m-%d %H:%M:%S"));

		strTmp.Format("// Create Time : %s",tmp);
		bld->mSegments[0].mLines.Add(strTmp);

		ModArr.Add(bld);
		DTArr.Add(dt);
		

		
		CVBuilderView* pView = new CVBuilderView;

		pView->Create(m_view, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |WS_VSCROLL | WS_HSCROLL, 0);
		pView->SetScrollSize(2000, 1000);
		pView->SetScrollLine(10, 10);
		pView->SetScrollPage(100, 100);

		pView->AssignDrawTool(dt);
		pView->AssignBuilder(bld);

		m_view.AddPage(pView->m_hWnd, dlg.m_modName);

		CString sName = bld->mModuleName;
		m_foldersDockWnd.m_tree.InsertItem(sName,0,1,TVI_ROOT,TVI_LAST);
	}


	// TODO: add code to initialize document
	//CBuilder bd;

	//bd.Load("E:\\test.v");


	return 0;
}
LRESULT CMainFrame::OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//CShellFileOpenDialog openDlg;

	TCHAR  strFileFilters[1024]=_T("Verilog Files(*.v)\0*.v\0\0");  

	CFileDialog   openDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strFileFilters,this->m_hWnd);  

	TCHAR  temp[1024];  
	ZeroMemory(temp,sizeof(TCHAR)*1024);  
	openDlg.m_ofn.lpstrFile=temp;   
	openDlg.m_ofn.nMaxFile=1024;  

	if (IDOK == openDlg.DoModal()) {
		CString fn = openDlg.m_ofn.lpstrFile;

		CBuilder *bld = new CBuilder;
		CDrawTool *dt = new CDrawTool;
		
		ModArr.Add(bld);
		DTArr.Add(dt);

		bld->Load(fn);

		int nInst = bld->mInsts.GetCount();
		POSITION pos = bld->mInsts.GetStartPosition();
		while (pos !=0)
		{
			CRect re(100,100,200,200);
			CDrawRect *di= new CDrawRect(re);
			CInst *inst = bld->mInsts.GetValueAt(pos);
			di->sModule = inst->ModuleName;
			di->m_position = inst->mRect;
			di->m_pInst= inst;
			di->sInst =  bld->mInsts.GetNextKey(pos);
			dt->Add(di);
		}

		CVBuilderView* pView = new CVBuilderView;

		pView->Create(m_view, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |WS_VSCROLL | WS_HSCROLL, 0);
		pView->SetScrollSize(2000, 1000);
		pView->SetScrollLine(10, 10);
		pView->SetScrollPage(100, 100);

		pView->AssignDrawTool(dt);
		pView->AssignBuilder(bld);
		CString sName = bld->mModuleName;
		m_view.AddPage(pView->m_hWnd, sName);

		UpdateInstanceList();
	}

	return 0;
}


LRESULT CMainFrame::OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int nActivePage = m_view.GetActivePage();
	if(nActivePage != -1) {
		CBuilder *bld = ModArr[nActivePage];
		CDrawTool *dt = DTArr[nActivePage];
		int nCount = dt->m_objects.GetCount();
		POSITION pos = dt->m_objects.GetHeadPosition();
		CDrawRect* di;
		while( pos!= NULL) {
			di = dt->m_objects.GetNext(pos);
			bld->mInsts[di->sInst]->mRect = di->m_position;
		}

		if (bld->mFileName == _T(""))
		{
			TCHAR  strFileFilters[1024]=_T("Verilog Files(*.v)\0*.v\0\0");  

			CFileDialog   openDlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strFileFilters,this->m_hWnd);  

			TCHAR  temp[1024];  
			ZeroMemory(temp,sizeof(TCHAR)*1024);  
			openDlg.m_ofn.lpstrFile=temp;   
			openDlg.m_ofn.nMaxFile=1024;  

			if (IDOK == openDlg.DoModal()) {
				CString fn = openDlg.m_ofn.lpstrFile;
				bld->mFileName = fn;
				bld->SaveAs(bld->mFileName);
			}
			
		}
		else {
			bld->SaveAs(bld->mFileName);
		}
	}
	return 0;
}




LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static BOOL bVisible = TRUE;	// initially visible
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainFrame::OnWindowClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int nActivePage = m_view.GetActivePage();
	if(nActivePage != -1) {
		m_view.RemovePage(nActivePage);
		delete ModArr[nActivePage];
		delete DTArr[nActivePage];
		ModArr.RemoveAt(nActivePage);
		DTArr.RemoveAt(nActivePage);
		UpdateInstanceList();
	}
	else
		::MessageBeep((UINT)-1);

	return 0;
}

LRESULT CMainFrame::OnWindowCloseAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_view.RemoveAllPages();
	for (int i=0;i<ModArr.GetSize();i++) {
		delete ModArr[i];
	}
	for (int i=0;i<DTArr.GetSize();i++) {
		delete DTArr[i];
	}
	
	UpdateInstanceList();

	return 0;
}

LRESULT CMainFrame::OnWindowActivate(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int nPage = wID - ID_WINDOW_TABFIRST;
	m_view.SetActivePage(nPage);

	return 0;
}
LRESULT CMainFrame::OnViewTreePane(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	bool bShow = m_foldersDockWnd.IsVisible();
	if (bShow)
		m_foldersDockWnd.Hide();
	else
		m_foldersDockWnd.Show();
	
	UpdateLayout();
	UISetCheck(ID_VIEW_TREEPANE, bShow);

	return 0;
}


LRESULT CMainFrame::OnViewOutput(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	bool bShow = m_outputDockWnd.IsVisible();
	if (bShow)
		m_outputDockWnd.Hide();
	else
		m_outputDockWnd.Show();

	UpdateLayout();
	UISetCheck(ID_VIEW_OUTPUTBAR, bShow);

	return 0;
}
LRESULT CMainFrame::OnAddInst(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int nActivePage = m_view.GetActivePage();

	if (nActivePage != -1) {
		CRect re(100,100,200,200);
		CDrawTool *dt= DTArr[nActivePage];
		CBuilder  *bld = ModArr[nActivePage];
		TCHAR  strFileFilters[1024]=_T("Verilog Files(*.v)\0*.v\0\0");  

		CFileDialog   openDlg(TRUE,_T("Verilog Files(*.v)"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT|OFN_ENABLESIZING,strFileFilters,this->m_hWnd);  

		//TCHAR  temp[1024];  
		//ZeroMemory(temp,sizeof(TCHAR)*1024); 
		//CString strFiles;
		//openDlg.m_ofn.lpstrFile=strFiles.GetBufferSetLength(10240);   
		//openDlg.m_ofn.nMaxFile=10240;  

		const int nMaxFiles = 100; 
		const int nBufferSize = (nMaxFiles * (MAX_PATH + 1)) + 1; 
		TCHAR szFileNamesBuffer[nBufferSize] = {0}; 

		openDlg.m_ofn.lpstrFile = szFileNamesBuffer; 
		openDlg.m_ofn.nMaxFile = nBufferSize; 


		TCHAR sFullPath[MAX_PATH] ={0};

		if (IDOK == openDlg.DoModal()) {
			if (openDlg.m_ofn.nFileOffset<::_tcslen(szFileNamesBuffer))
			{
				TCHAR *sFullPath = szFileNamesBuffer;

				CInst *inst = bld->AddNewInst(sFullPath);
				CDrawRect *di = new CDrawRect;
				CRect rect(100,100,200,200);
				di->m_position = rect;
				di->sInst = inst->InstName;
				di->sModule= inst->ModuleName;
				di->m_pInst=inst;

				dt->Add(di);
			}
			else 
			{
				TCHAR *sDir = szFileNamesBuffer;
				TCHAR *sFileName = szFileNamesBuffer + openDlg.m_ofn.nFileOffset;
		
				
				while(*sFileName != '\0') 
				{
					sFullPath[0]='\0';
					::PathCombine(sFullPath,sDir,sFileName);
					sFileName = sFileName + _tcslen(sFileName) + 1;


					CInst *inst = bld->AddNewInst(sFullPath);
					CDrawRect *di = new CDrawRect;
					CRect rect(100,100,200,200);
					di->m_position = rect;
					di->sInst = inst->InstName;
					di->sModule= inst->ModuleName;
					di->m_pInst =inst;
					dt->Add(di);
				}

			}
			
			UpdateInstanceList();


			m_view.GetNextDlgTabItem(m_view.GetPageHWND(nActivePage)).Invalidate();
			
		}

	}
	//CDC dc(::GetWindowDC(m_view.GetPageHWND(nActivePage)));
	//
	//CDrawRect dr;
	//dr.Draw(&dc);

	return 0;
}
LRESULT CMainFrame::OnBrowse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CBuilder *bld;
	CString sFileName;

	int nActivePage = m_view.GetActivePage();

	if (nActivePage != -1) {
		bld = ModArr[nActivePage];
		bld->SaveAs(bld->mFileName);
		sFileName = bld->mFileName;
		if(ShellExecute (NULL,_T("open"),sFileName,NULL,NULL,SW_SHOW) <(HANDLE)32) {
			MessageBox(_T("Open File Fail"));
		}

	}



	return 0;
}
LRESULT CMainFrame::OnAutoConnect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CBuilder *bld;
	CInst *inst;
	POSITION posInst,posLink;
	CAtlMap<CStringA,TWire>::CPair *wp;


	int nActivePage = m_view.GetActivePage();

	if (nActivePage != -1 ) {
		bld = ModArr[nActivePage];
		posInst = bld->mInsts.GetStartPosition();
		while (posInst != 0) {
			inst = bld->mInsts.GetNextValue(posInst);
			posLink = inst->mLinks.GetStartPosition();
			while (posLink != 0) {
				TLink &link = inst->mLinks.GetNextValue(posLink);
				if (link.wName == "") {
					link.wName = link.ioName;
					wp = bld->mWires.Lookup(link.wName);
					if (wp == NULL) {
						TWire wire;
						wire.sName = link.ioName;
						wire.sType = "w";
						wire.iMsb  = link.iWidth - 1;
						wire.iLsb  = 0;

						link.wType = "w";
						bld->mWires.SetAt(wire.sName,wire);
					}
				}
			}
		}
	}
	return 0;
}
LRESULT CMainFrame::OnManual(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{

	int nActivePage = m_view.GetActivePage();


	if (nActivePage != -1) {
		CBuilder *builder = ModArr[nActivePage];

		CConnectDlg dlg;

		dlg.SetBuilder(builder);
		dlg.DoModal();
	}

	return 0;
}
LRESULT CMainFrame::OnDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int nActivePage = m_view.GetActivePage();


	if (nActivePage != -1) {
		CBuilder *builder = ModArr[nActivePage];
		CDrawTool *dt = DTArr[nActivePage];

	
		POSITION pos = dt->m_selection.GetHeadPosition();
		while (pos != NULL)
		{
			CDrawRect* pObj = dt->m_selection.GetNext(pos);
			builder->mInsts.RemoveKey(pObj->sInst);
			POSITION idx = dt->m_objects.Find(pObj);
			dt->m_objects.RemoveAt(idx);
			pObj->Remove();
		}

		dt->m_selection.RemoveAll();

		UpdateInstanceList();
		m_view.GetNextDlgTabItem(m_view.GetPageHWND(nActivePage)).Invalidate();
	}

	return 0;
}
void CMainFrame::UpdateInstanceList() 
{
	m_foldersDockWnd.m_tree.DeleteAllItems();

	CBuilder *bld;
	for (int i=0;i<ModArr.GetSize();i++) {
		bld = ModArr[i];
		CString sName = bld->mModuleName;
		HTREEITEM hItem= m_foldersDockWnd.m_tree.InsertItem(sName,0,1,TVI_ROOT,TVI_LAST);
		
		POSITION pos = bld->mInsts.GetStartPosition();
		while (pos!=0) {
			CInst *inst = bld->mInsts.GetNextValue(pos);
			CString sItemName = inst->InstName + "(" + inst->ModuleName +")";
			m_foldersDockWnd.m_tree.InsertItem(sItemName,0,1,hItem,TVI_LAST);
		}
	}
}