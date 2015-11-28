// connectdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "PropertyList.h"
#include "Builder.h"

class CConnectDlg : 
	public CDialogImpl<CConnectDlg>,
	public CDialogResize<CConnectDlg>
{
public:
	enum { IDD = IDD_CONNECT };

public:
	CButton   mbtnConnect;
	CButton   mbtnLeftTieZero;
	CButton   mbtnLeftTieOne;
	CButton   mbtnLeftAutoPort;
	CButton   mbtnLeftNotCare;

	CButton   mbtnRightTieZero;
	CButton   mbtnRightTieOne;
	CButton   mbtnRightAutoPort;
	CButton   mbtnRightNotCare;

	CPropertyListCtrl m_LeftctrlList;
	CPropertyListCtrl m_RightctrlList;

	CBuilder *m_builder;

	int m_pos;
	int m_lType;
	int m_rType;
	CString m_sFilter;


	BEGIN_MSG_MAP(CConnectDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)

		MSG_WM_SIZE(OnSize)

		NOTIFY_CODE_HANDLER(PIN_ITEMCHANGED, OnPropertyItemChanged)
//		NOTIFY_CODE_HANDLER(PIN_ITEMCHANGED, OnPropertyItemChanged)

		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDC_CLOSE, OnCloseCmd)


		COMMAND_ID_HANDLER(IDC_LINK, OnConnect)
		COMMAND_ID_HANDLER(IDC_LCLEAR, OnLinkTo)
		COMMAND_ID_HANDLER(IDC_RCLEAR, OnLinkTo)
		COMMAND_ID_HANDLER(IDC_LTIE0, OnLinkTo)
		COMMAND_ID_HANDLER(IDC_RTIE0, OnLinkTo)
		COMMAND_ID_HANDLER(IDC_LTIE1, OnLinkTo)
		COMMAND_ID_HANDLER(IDC_RTIE1, OnLinkTo)

		COMMAND_ID_HANDLER(IDC_LAUTOPORT, OnAutoPort)
		COMMAND_ID_HANDLER(IDC_RAUTOPORT, OnAutoPort)		
		
		COMMAND_ID_HANDLER(IDC_LEFTALL, OnUpdateList)	
		COMMAND_ID_HANDLER(IDC_LEFTNC, OnUpdateList)	
		COMMAND_ID_HANDLER(IDC_LEFTERR, OnUpdateList)	
		COMMAND_HANDLER_EX(IDC_LEFTFILTER, EN_CHANGE,OnFilterChange)

		COMMAND_ID_HANDLER(IDC_RIGHTALL, OnUpdateList)	
		COMMAND_ID_HANDLER(IDC_RIGHTNC, OnUpdateList)	
		COMMAND_ID_HANDLER(IDC_RIGHTERR, OnUpdateList)	
		COMMAND_HANDLER_EX(IDC_RIGHTFILTER, EN_CHANGE,OnFilterChange)

		CHAIN_MSG_MAP( CDialogResize<CConnectDlg> )
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DLGRESIZE_MAP(CConnectDlg)
		DLGRESIZE_CONTROL(IDC_LEFTLIST, DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_RIGHTLIST, DLSZ_SIZE_Y)

		DLGRESIZE_CONTROL(IDC_SIGLIST,     DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_CLOSE, DLSZ_MOVE_Y)

		DLGRESIZE_CONTROL(IDC_LINK, DLSZ_CENTER_X)		

		DLGRESIZE_CONTROL(IDC_SPLINE, DLSZ_SIZE_X|DLSZ_MOVE_Y)		


	END_DLGRESIZE_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnPropertyItemChanged(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);

	void OnSize(UINT nType, CSize size);
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnConnect(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnLinkTo(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAutoPort(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnUpdateList(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	
	void OnFilterChange(UINT uNotifyCode, int nID, CWindow wndCtl);

	void InitPropertyList();
	void UpdatePropertyList(int LeftRight,int nType ,CString sFilter);
	void SetBuilder(CBuilder* builder);
	void UpdateLinks();
		
};
