#pragma once


// CNewDlg ¶Ô»°¿ò

class CNewDlg : public CDialogImpl<CNewDlg>
{
public:
	enum { IDD = IDD_NEWTOP };
	int   m_rtnflag;
	CString m_modName;
	CString m_Path;

	CNewDlg();



	BEGIN_MSG_MAP(CNewDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		NOTIFY_HANDLER(IDC_MODULE,EN_CHANGE,OnEditChange)
		COMMAND_ID_HANDLER(IDC_SELECT, OnSelectFile)
		COMMAND_ID_HANDLER(IDC_OK, OnOKCmd)
		COMMAND_ID_HANDLER(IDC_CANCEL, OnCloseCmd)
	END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)


	INT_PTR DoModal();

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSelectFile(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditChange(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
	

};
