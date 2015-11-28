#pragma once

class TWire
{
public:
	CStringA sType;
	CStringA sName;
	UINT	iLsb;
	UINT	iMsb;

	CStringA GetTypeName(){
		if(sType == _T("i")) 
			return _T("input"); 
		else if (sType == _T("o"))
			return _T("output"); 
		else if (sType == _T("w"))
			return _T("wire"); 
		else 
			return _T("");
	}
} ;

//typedef struct  
//{
//
//	CStringA ioName;
//	UINT    ioMsb;
//	UINT    ioLsb;
//} TPort;

class TLink  
{
public:
	CStringA ioName;
	CStringA ioType;
	UINT    iWidth;
	CStringA wName;
	CStringA wType;
} ;

typedef struct
{
	CStringA paramName;
	CStringA paramValue;
} TParam;


