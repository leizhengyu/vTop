#pragma once

#include "Wire.h"

class CInst;


class CModule
{
public:
	CModule(void);
	CModule(const CModule &c );
	~CModule(void);
	int Load(CString fn);
	void TrimComment(CStringA &strSrc,CStringA &strDst);
	int GetCodeLine(void);
	int GetPorts(CSimpleArray<CStringA> &Lines);
	int GetParams();
	int GetModuleName();
	int	GenInstance(CInst *inst);
	int CalcParaValue(CStringA strExpr);
	void SetParams(CAtlMap<CStringA,UINT> *ps);

public:
	CStringA					 strCode;
	CSimpleArray<CStringA>	 mLines;

	CAtlMap<CStringA,UINT>    mParams;
	CAtlMap<CStringA,TWire>	 mWires;

	CStringA					 mModuleName;
	CStringA					 mInstName;
	CString					     mFileName;
};
