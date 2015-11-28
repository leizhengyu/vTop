#pragma once
#include "stdafx.h"
#include "Inst.h"
#include "Segment.h"
#include "Module.h"


class CBuilder:public CModule
{
public:
	CBuilder();
	CBuilder(const CBuilder& C);
	~CBuilder();

public:
	int Load(CString file);
	int Refresh(void);

	int GetSegments(void);
	int GetWireDef(void);
	int GetInstDef(void);
	int UpdateUIContext(void);
	int GetModuleHead(CSimpleArray<CStringA> &lines);
	int GetPortsDef(CSimpleArray<CStringA> &lines);
	int GetWiresDef(CSimpleArray<CStringA> &lines);

	CInst* AddNewInst(CString fn);
	int Update();
	int SaveAs(CString fn);


public:
	CAtlMap<CStringA,CInst*> mInsts;
	CSegment mSegments[8];

	CStringA strCode;
	CStringA strBegin[8];
	CStringA strEnd[8];

};
