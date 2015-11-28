// PropertyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PropertyDlg.h"


// CPropertyDlg 对话框

IMPLEMENT_DYNAMIC(CPropertyDlg, CPropertyPage)

CPropertyDlg::CPropertyDlg()
	: CPropertyPage(CPropertyDlg::IDD)
{

}

CPropertyDlg::~CPropertyDlg()
{
}

void CPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPropertyDlg, CPropertyPage)
END_MESSAGE_MAP()


// CPropertyDlg 消息处理程序
