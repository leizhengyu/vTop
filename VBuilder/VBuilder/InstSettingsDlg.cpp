// InstSettingsPage.cpp : 实现文件
//

#include "stdafx.h"
#include "InstSettingsDlg.h"

// CInstSettingsPage 对话框
CInstSettingsDlg::CInstSettingsDlg()
{

}
CInstSettingsDlg::~CInstSettingsDlg()
{

}
LRESULT CInstSettingsDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CString sName = m_pInst->InstName;
	SetDlgItemText(IDC_SETNAME,sName);
	
	m_ParaList.SubclassWindow(GetDlgItem(IDC_PARALIST));
	m_ParaList.SetExtendedListStyle(PLS_EX_CATEGORIZED | PLS_EX_XPLOOK|PLS_EX_SHOWSELALWAYS);
	m_ParaList.SetColumNum(2);
//	m_ParaList.AddItem( PropCreateCategory(_T("")) );

	POSITION pos = m_pInst->mParams.GetStartPosition();
	while (pos!=0) {
		CAtlMap<CStringA,UINT>::CPair *xp = m_pInst->mParams.GetNext(pos);
		CSimpleArray<CString> sVal;
		CString sTmp;
		sTmp.Format(_T("%d"),xp->m_value);
		sVal.Add(sTmp);
		CString sKey = xp->m_key;
		m_ParaList.AddItem( PropCreateSimple(sKey,sVal) );
		
	}

	CenterWindow(GetParent());
	return TRUE;
}

LRESULT CInstSettingsDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CInstSettingsDlg::OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UINT iNum;
	HPROPERTY prop ;
	CStringA sName;
	CStringA sValue;


	iNum = m_ParaList.GetCount();

	for (UINT i=0;i<iNum;i++) {
		prop = m_ParaList.GetProperty(i);
		sName = prop->GetName();
		sValue = prop->GetValue(0);
		m_pInst->mParams.Lookup(sName)->m_value = atoi(sValue);
	}
	CString wName;
	GetDlgItemText(IDC_SETNAME,wName);
	sName = wName;
	m_pInst->InstName = sName;

	return EndDialog(wID);
}
void CInstSettingsDlg::SetInst(CInst *inst)
{
	m_pInst = inst;
}