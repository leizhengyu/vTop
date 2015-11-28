#pragma once
#include "stdafx.h"
#include "Builder.h"

class CBuilderMgr
{
public:

	CBuilderMgr(void){}

	~CBuilderMgr(void){}
public:
	CSimpleArray<CBuilder *> ModList;
};
