#include "StdAfx.h"
#include "Module.h"
#include "Inst.h"

CModule::CModule(void)
{
}

CModule::~CModule(void)
{
}

CModule::CModule(const CModule &c )
{
	const CAtlMap<CStringA,TWire>::CPair* px_port =0;
	const CAtlMap<CStringA,UINT>::CPair* px_param =0;

	POSITION pos = c.mWires.GetStartPosition();
	while (pos !=0)
	{
		px_port = c.mWires.GetNext(pos);
		mWires.SetAt(px_port->m_key,px_port->m_value);
	}

	pos = c.mParams.GetStartPosition();
	while (pos !=0)
	{
		px_param = c.mParams.GetNext(pos);
		mParams.SetAt(px_param->m_key,px_param->m_value);
	}

	mFileName = c.mFileName;
	mModuleName   = c.mModuleName;
	mInstName = c.mInstName;
}
int CModule::Load(CString fn)
{
	CStringA     strNew;
	ULONGLONG   len;
	CAtlFile	hFile;

	HRESULT hr = hFile.Create(fn,GENERIC_READ,FILE_SHARE_READ,OPEN_EXISTING);

	hFile.GetSize(len);
	hFile.Read(strCode.GetBufferSetLength(len),len);
	strCode.Replace("\r\n","\n");
	hFile.Close();

//	TrimComment(strCode,strNew);
	GetCodeLine();
//	GetPorts(mLines);
	GetParams();
	GetModuleName();

	mFileName = fn;
	return 0;
}

int CModule::GetCodeLine(void)
{
	CStringA strLine;
	int pos=0;
	int prepos=0;

	while((pos=strCode.Find("\n",prepos)) != -1)
	{
		strLine = strCode.Mid(prepos,pos-prepos);
		prepos=pos+1;
		mLines.Add(strLine);
	}
	return 0;
}

void CModule::TrimComment(CStringA &strSrc,CStringA &strDst)
{
	enum State
	{
		PROCESS,
		SLASH1,
		SLASH2,
		STAR1,
		STAR2,
		QUOTE,
		ESCAPE,
		SLASH_EXTEND1,
		SLASH_EXTEND2,
		COMMENT
	};

	WCHAR ch;
	CStringA outStr;
	State state = PROCESS;

	int len = strSrc.GetLength();

	for(int i=0;i<len;i++)
	{
		//Action
		switch (state)
		{
		case PROCESS:
			strDst += outStr;
			outStr = "";
			break;
		case COMMENT:
			outStr = "";
			if(ch == '\r'|| ch == '\n' || ch == '\r\n')
			{
				strDst += "\r\n";
				//fout << std::endl;
			}
			break;
		}

		ch = strSrc.GetAt(i);
		outStr += ch;

		//Change state
		switch(state)
		{
		case PROCESS:
			if ( ch == '/')
				state = SLASH1;
			else if ( ch == '\"')
				state = QUOTE;
			break;
		case SLASH1:
			if (ch == '/')
				state = SLASH2;
			else if (ch == '\"')
				state = QUOTE;
			else if (ch == '*')
				state = STAR1;
			else
				state = PROCESS;
			break;
		case SLASH2:
			if (ch == '\\')
				state = SLASH_EXTEND1;
			else if (ch == '\n' || ch == '\r\n' || ch == '\r')
				state = COMMENT;
			break;
		case STAR1:
			if (ch == '*')
				state = STAR2;
			break;
		case STAR2:
			if (ch == '/')
				state = COMMENT;
			else if (ch == '*')
				state = STAR2;
			else
				state = STAR1;
			break;
		case QUOTE:
			if (ch == '\"')
				state = PROCESS;
			else if (ch == '\\')
				state = ESCAPE;
			break;
		case ESCAPE:
			state = QUOTE;
			break;
		case SLASH_EXTEND1:
			if (ch == '\r' || ch == '\r\n' || ch == '\n')
				state = SLASH_EXTEND2;
			else if (ch == '\\')
				state = SLASH_EXTEND1;
			else
				state = SLASH2;
			break;
		case SLASH_EXTEND2:
			if (ch == '\r' || ch == '\r\n' || ch == '\n')
				state = COMMENT;
			else if (ch == '\\')
				state = SLASH_EXTEND1;
			break;
		case COMMENT:
			if (ch == '/')
				state = SLASH1;
			else if (ch == '\"')
				state = QUOTE;
			else
				state = PROCESS;
			break;
		}
	}
}


int CModule::GetPorts(CSimpleArray<CStringA> &Lines )
{
	CAtlRegExp<CAtlRECharTraitsA>  re;
	CAtlREMatchContext<CAtlRECharTraitsA> mc ;
	CAtlRegExp<CAtlRECharTraitsA>  re2;
	CAtlREMatchContext<CAtlRECharTraitsA> mc2 ;
	const CAtlREMatchContext<CAtlRECharTraitsA>::RECHAR * szStart = 0;
	const CAtlREMatchContext<CAtlRECharTraitsA>::RECHAR * szEnd = 0;

	int nLen =0;
	int nSize = Lines.GetSize();
	TWire  wItem;

	re.Parse("^{(input)|(output)}( *reg )?{.*}");


	for(int i0=0;i0<nSize;i0++)
	{
		CStringA strLine = Lines[i0].Trim();

		if (re.Match(strLine,&mc))
		{
			mc.GetMatch(0, &szStart, &szEnd);
			CStringA strType = CStringA(szStart).Left(szEnd - szStart);
			wItem.sType = (strType=="input") ? "i":"o";
			
			mc.GetMatch(1, &szStart, &szEnd);			

			CStringA strRange = CStringA(szStart).Left(szEnd - szStart).Trim();
			
			re2.Parse("\\[{.*}:{.*}\\] +{[a-zA-Z0-9_]*}\\b*[,;]?");
			if(re2.Match(strRange,&mc2)){
				mc2.GetMatch(0, &szStart, &szEnd);

				wItem.iMsb = CalcParaValue(CStringA(szStart).Left(szEnd-szStart).Trim());
				mc2.GetMatch(1, &szStart, &szEnd);
				wItem.iLsb = CalcParaValue(CStringA(szStart).Left(szEnd-szStart).Trim());
				mc2.GetMatch(2, &szStart, &szEnd);
				wItem.sName = CStringA(szStart).Left(szEnd-szStart).Trim();
			}
			else 
			{	re2.Parse(" *{[a-zA-Z0-9_]*}\\b*[,;]?");
				re2.Match(strRange,&mc2);
				mc2.GetMatch(0, &szStart, &szEnd);
				wItem.sName = CStringA(szStart).Left(szEnd-szStart).Trim();
			}
		
			mWires.SetAt(wItem.sName,wItem);
		}
	}
	return 0;
}
int CModule::GetParams()
{
	CAtlRegExp<CAtlRECharTraitsA>  re;
	CAtlRegExp<CAtlRECharTraitsA>  re2;

	CAtlREMatchContext<CAtlRECharTraitsA> mc ;
	const CAtlREMatchContext<CAtlRECharTraitsA>::RECHAR * szStart = 0;
	const CAtlREMatchContext<CAtlRECharTraitsA>::RECHAR * szEnd = 0;

	int nLen =0;
	int nSize = mLines.GetSize();

	CStringA paramName;
	CStringA strName;
	CStringA strValue;
	CStringA paramValue;
	CStringA strType;
	UINT iVal=0;

	re.Parse("^parameter\\b*{\\a*}\\b*=\\b*{[a-zA-Z0-9_'+*/\\-]+}");
	re2.Parse("^(\\d*'{[hdb]})?{\\d*}");

	for(int i0=0;i0<nSize;i0++)	{
		CStringA strLine = mLines[i0].Trim();


		if (re.Match(strLine,&mc))	{
			mc.GetMatch(0, &szStart, &szEnd);			
			paramName = CStringA(szStart).Left(szEnd - szStart);
	
			mc.GetMatch(1, &szStart, &szEnd);			
			strValue = CStringA(szStart).Left(szEnd - szStart);

			iVal = CalcParaValue(strValue);
			
			mParams.SetAt(paramName,iVal);
		}
	}

	return 0;
}
void CModule::SetParams(CAtlMap<CStringA,UINT> *ps) {
	mParams.RemoveAll();
	POSITION pos=ps->GetStartPosition();
	while (pos!=0) {
		CAtlMap<CStringA,UINT>::CPair *xp=ps->GetNext(pos);
		mParams.SetAt(xp->m_key,xp->m_value);
	}
}
int CModule::CalcParaValue(CStringA strExpr)
{
	CAtlRegExp<CAtlRECharTraitsA>  re;
	CAtlRegExp<CAtlRECharTraitsA>  re2;

	CAtlREMatchContext<CAtlRECharTraitsA> mc ;
	const CAtlREMatchContext<CAtlRECharTraitsA>::RECHAR * szStart = 0;
	const CAtlREMatchContext<CAtlRECharTraitsA>::RECHAR * szEnd = 0;

	CStringA sPara;
	CStringA sRest;
	CStringA sItem;
	CStringA sType;
	CStringA sVal;

	UINT    iVal;
	TCHAR   op='+';
	UINT    iRlt=0;

	CStringA sExp = strExpr;

	re.Parse("{[a-zA-Z0-9_']+}");
	re2.Parse("^(\\d*'{[hdb]})?{\\d+}");
	
	while (re.Match(sExp,&mc)) {
		mc.GetMatch(0, &szStart, &szEnd);
		sItem = CStringA(szStart).Left(szEnd - szStart).Trim();
		sRest = szEnd;

		if (re2.Match(sItem,&mc)) {
			mc.GetMatch(0, &szStart, &szEnd);
			sType = CStringA(szStart).Left(szEnd - szStart);
			mc.GetMatch(1, &szStart, &szEnd);

			sVal =  CStringA(szStart).Left(szEnd - szStart);				


			if (sType == "" || sType == "d") {
				sscanf(sVal,"%d",&iVal);
			}
			else if (sType =="h"){
				sscanf(sVal,"%x",&iVal);
			}		
		}
		else {
			CAtlMap<CStringA,UINT>::CPair *xp = mParams.Lookup(sItem);
			if (xp == NULL) {
				MessageBox(NULL,_T("²ÎÊý½âÎö´íÎó£¡"),_T("´íÎó"),0);
				return 0;
			}
			iVal = xp->m_value;
		}

		switch(op) {
			case '+':     iRlt += iVal; break;
			case '-':	  iRlt -= iVal; break;
			case '*':	  iRlt *= iVal; break;
			case '/':	  iRlt /= iVal; break;
			default: iRlt += iVal; 
		}

		if (sRest.GetLength() > 0) {
			op = sRest.GetAt(0);
			sExp = sRest.Mid(1);
		}
		else {
			break;
		}
	}
	
	return iRlt;
}



int CModule::GetModuleName()
{
	CAtlRegExp<CAtlRECharTraitsA>  re;
	CAtlREMatchContext<CAtlRECharTraitsA> mc ;
	const CAtlREMatchContext<CAtlRECharTraitsA>::RECHAR * szStart = 0;
	const CAtlREMatchContext<CAtlRECharTraitsA>::RECHAR * szEnd = 0;

	int nLen =0;
	int nSize = mLines.GetSize();

	re.Parse("^module\\b*{[a-zA-Z0-9_]+} *.*");

	for(int i0=0;i0<nSize;i0++)	{
		CStringA strLine = mLines[i0].Trim();
		if (re.Match(strLine,&mc))	{
			mc.GetMatch(0, &szStart, &szEnd);			
			mModuleName = CStringA(szStart).Left(szEnd - szStart);
		}
	}
	return 0;
}
int CModule::GenInstance(CInst *inst)
{
	const CAtlMap<CStringA,TWire>::CPair* px_port =0;
	const CAtlMap<CStringA,UINT>::CPair* px_param =0;

	POSITION pos = mWires.GetStartPosition();

	while (pos !=0)
	{
		px_port = mWires.GetNext(pos);
		TLink link;
		link.ioName = px_port->m_key;
		link.ioType = px_port->m_value.sType;		
		link.iWidth  = px_port->m_value.iMsb - px_port->m_value.iLsb +1;
		link.wName  = "";
		link.wType = "e";
		inst->mLinks.SetAt(link.ioName,link);
	}

	pos = mParams.GetStartPosition();
	while (pos !=0)
	{
		px_param = mParams.GetNext(pos);
		inst->mParams.SetAt(px_param->m_key,px_param->m_value);
	}

	inst->FilePath = mFileName;
	inst->InstName = "U0_" + mModuleName;
	inst->ModuleName = mModuleName;

	return 0;
}
