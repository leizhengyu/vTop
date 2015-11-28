#pragma once

class CSegment
{
public:
	CSimpleArray<CStringA> mLines;

public:
	CSegment(void)	{}
	~CSegment(void)	{}
public:
	int Refresh();
};
