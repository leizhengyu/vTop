#pragma once

#include "resource.h"
#include "Inst.h"
#include "PropertyList.h"



class CInstSettingsDlg : public CDialogImpl<CInstSettingsDlg>
{
public:
	enum { IDD = IDD_PROPERTY };
public:
	CInstSettingsDlg();
	~CInstSettingsDlg();

	BEGIN_MSG_MAP(CInstSettingsDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOKCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void SetInst(CInst *inst);

public:
	CInst*            m_pInst;
	CPropertyListCtrl m_ParaList;
};
