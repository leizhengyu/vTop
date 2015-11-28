#pragma once
#include "Wire.h"
#include "Module.h"

class CInst
{
public:
	CInst(void) { mRect=0;}
    CInst(const CInst& c) {
		const CAtlMap<CStringA,TLink>::CPair* px_link =0;
		const CAtlMap<CStringA,UINT>::CPair* px_param =0;

		POSITION pos = c.mLinks.GetStartPosition();

		while (pos !=0)
		{
			px_link = c.mLinks.GetNext(pos);
			mLinks.SetAt(px_link->m_key,px_link->m_value);
		}

		pos = c.mParams.GetStartPosition();
		while (pos !=0)
		{
			px_param = c.mParams.GetNext(pos);
			mParams.SetAt(px_param->m_key,px_param->m_value);
		}

		FilePath   = c.FilePath;
		InstName   = c.InstName;
		ModuleName = c.ModuleName;
		mRect      = c.mRect;
	}

	~CInst(void){}
public:
	int GenInstanceCode(CSimpleArray<CStringA> &Lines) {
		CStringA strFmt;
		CStringA strParam = " ";

		POSITION pos = mParams.GetStartPosition();
		if (pos!=0) strParam = " #(";
		while(pos!=0) {
			CAtlMap<CStringA,UINT>::CPair *p = mParams.GetNext(pos);
			strFmt.Format(".%s(%d)%s ",p->m_key,p->m_value,pos==0?")":",");
			strParam = strParam + strFmt;
		}
		Lines.Add(ModuleName + strParam + InstName + " (");
		pos = mLinks.GetStartPosition();
		while(pos!=0) {
			CAtlMap<CStringA,TLink>::CPair *link = mLinks.GetNext(pos);
			CStringA sComma = pos==0 ? "" : ",";
			strFmt.Format("    .%-20s ( %-20s)%s",link->m_value.ioName,link->m_value.wName,sComma);
			Lines.Add(strFmt);
		}
		Lines.Add("    );");
		Lines.Add("");
		return 0;
	}

	int GenInstanceText(CSimpleArray<CStringA> &Lines) {
		CStringA strFmt;

		strFmt.Format("// INSTANCE(%s,%s,%s)", ModuleName,InstName , FilePath);
		
		Lines.Add(strFmt);
		
		POSITION pos = mParams.GetStartPosition();
		while(pos!=0) {
			CAtlMap<CStringA,UINT>::CPair *p = mParams.GetNext(pos);
			strFmt.Format("// PARAMETER(%20s ,%20d)",p->m_key,p->m_value);
			Lines.Add(strFmt);
		}
		pos = mLinks.GetStartPosition();
		while(pos!=0) {
			CAtlMap<CStringA,TLink>::CPair *link = mLinks.GetNext(pos);
			strFmt.Format("// CONNECT(%20s , %3d , %s ,%20s , %s)",link->m_value.ioName,link->m_value.iWidth,link->m_value.ioType,link->m_value.wName,link->m_value.wType);
			Lines.Add(strFmt);
		}
		Lines.Add("");

		return 0;
	}
	void GenUIContext(CSimpleArray<CStringA> &Lines) {
		CStringA strContext;
		strContext.Format("// %s;%d;%d;%d;%d",InstName,mRect.left,mRect.top,mRect.right,mRect.bottom);
		Lines.Add(strContext);
	}
	void UpdateParam(CStringA sKey,UINT iVal) {
		mParams.SetAt(sKey,iVal);
	}
	void UpdateLinks() {
		CModule mod;
		mod.Load(FilePath);
		mod.SetParams(&mParams);
		mod.GetPorts(mod.mLines);
		POSITION pos = mod.mWires.GetStartPosition();
		while (pos!=0) {
			CAtlMap<CStringA,TWire>::CPair *xp =mod.mWires.GetNext(pos);
			CAtlMap<CStringA,TLink>::CPair *xl=mLinks.Lookup(xp->m_key);
			if (xl == NULL) {
				TLink lnk;
				lnk.ioName = xp->m_key;
				lnk.ioType = xp->m_value.sType;
				lnk.iWidth = xp->m_value.iMsb - xp->m_value.iLsb + 1;
				lnk.wName  = "";
				lnk.wType  = "e";
			}
			else {
				xl->m_value.iWidth = xp->m_value.iMsb - xp->m_value.iLsb + 1;
			}
		}
	}

public:
	CStringA ModuleName;
	CStringA InstName;
	CString  FilePath;
	CRect   mRect;

	CAtlMap<CStringA,TLink> mLinks;
	CAtlMap<CStringA,UINT> mParams;
};



