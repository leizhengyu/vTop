// NewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "NewDlg.h"



CNewDlg::CNewDlg():m_rtnflag(IDCANCEL)
{

}

LRESULT CNewDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
	TCHAR chPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,chPath);
	SetDlgItemText(IDC_PATH,chPath);
	GetDlgItem(IDC_MODULE).SetFocus();

	return TRUE;
}
LRESULT CNewDlg::OnEditChange(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	CString modName;
	
	GetDlgItemText(IDC_MODULE,modName);
	
	SetDlgItemText(IDC_PATH,modName.MakeLower()) ;

	return 0;
}

INT_PTR CNewDlg::DoModal() 
{
	CDialogImpl<CNewDlg>::DoModal();
	return m_rtnflag; 
}

LRESULT CNewDlg::OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{

	CString modName;
	GetDlgItemText(IDC_MODULE,modName);
	if(modName.IsEmpty()) {
		MessageBox(_T("Module名称不能为空!"),_T("提示"));
		GetDlgItem(IDC_MODULE).SetFocus();
		return 0;
	}

	GetDlgItemText(IDC_MODULE,m_modName);
	GetDlgItemText(IDC_PATH,m_Path);
	EndDialog(wID);

	m_rtnflag = IDOK; 
	return 0;
}
LRESULT CNewDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);

	m_rtnflag = IDCANCEL; 
	return 0;
}


LRESULT CNewDlg::OnSelectFile(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	TCHAR  strFileFilters[1024]=_T("Verilog Files(*.v)\0*.v\0\0");  
	CFolderDialog folderDlg(this->m_hWnd,_T("选择文件存放路径"));
	//CFileDialog   selDialog(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strFileFilters,this->m_hWnd);  

	//TCHAR  temp[1024];  
	//ZeroMemory(temp,sizeof(TCHAR)*1024);  
	//selDialog.m_ofn.lpstrFile=temp;   
	//selDialog.m_ofn.nMaxFile=1024;   

	TCHAR chPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,chPath);
	folderDlg.m_lpstrInitialFolder=chPath;


	if (IDOK==folderDlg.DoModal())  
	{  
		CString   fn = folderDlg.m_szFolderPath;  
		SetDlgItemText(IDC_PATH,fn);
	}  

	return 0;
}



