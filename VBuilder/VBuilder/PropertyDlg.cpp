// PropertyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PropertyDlg.h"


// CPropertyDlg �Ի���

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


// CPropertyDlg ��Ϣ�������
