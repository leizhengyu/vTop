#include "StdAfx.h"
#include "Builder.h"
#include "RxUtil.h"
#include "Inst.h"


CBuilder::CBuilder(void)
{
	strBegin[0] = "// =====HEAD BEGIN ===============";
	strBegin[1] = "// =====PORTS BEGIN ==============";
	strBegin[2] = "// =====WIRES BEGIN ==============";
	strBegin[3] = "// =====USERDEFINED BEGIN ========";
	strBegin[4] = "// =====INSTANCES BEGIN ==========";
	strBegin[5] = "// =====DEFINES BEGIN ============";
	strBegin[6] = "// =====UI BEGIN =================";

	strEnd[0] =	"// =====HEAD END =================";	
	strEnd[1] =	"// =====PORTS END ================";	
	strEnd[2] =	"// =====WIRES END ================";	
	strEnd[3] =	"// =====USERDEFINED END ==========";	
	strEnd[4] =	"// =====INSTANCES END ============";	
	strEnd[5] =	"// =====DEFINES END ==============";	
	strEnd[6] =	"// =====UI END ===================";
	mInsts.InitHashTable(1024);
}
CBuilder::CBuilder(const CBuilder& C)
{	
}

CBuilder::~CBuilder(void)
{
//	CAtlMap<CStringA,CInst*>::CPair *px_inst;

	POSITION pos = mInsts.GetStartPosition();

	while (pos !=0)
	{
		delete mInsts.GetNext(pos)->m_value;
	}

	mInsts.RemoveAll();
}

int CBuilder::Load(CString fn) 
{
	CModule::Load(fn);

	GetSegments();
	GetWireDef();
	GetPorts(mSegments[1].mLines);
	GetInstDef();
	UpdateUIContext();
	//SaveAs("E:\\NewTest.v");
	//AddNewInst(fn);
	return 0;
}
int CBuilder::UpdateUIContext(void)
{
	CStringA strFmt;
	CStringA strTemp;
	CStringA strSub[6];
	int  pos=0;
	int iNum = mSegments[6].mLines.GetSize();
	for (int i=0;i<iNum;i++) {
		strFmt = mSegments[6].mLines[i].Trim();
		if(strFmt == "") break;
		int pos=3;		
		int cnt=0;

		//strSub[0] = strFmt.Tokenize(_T(";"),pos);

		while ((strTemp=strFmt.Tokenize(";",pos)) != "") {
			strSub[cnt++] = strTemp;
		}
		
		if ( mInsts.Lookup(strSub[0]) != NULL)
		{
			CInst *inst = mInsts[strSub[0]];
			inst->mRect.left = atoi(strSub[1]);
			inst->mRect.top = atoi(strSub[2]);
			inst->mRect.right = atoi(strSub[3]);
			inst->mRect.bottom = atoi(strSub[4]);
		}		
	}
	return 0;
}

//--------------------------------------------------
// Store context in Segments
//--------------------------------------------------
int CBuilder::GetSegments(void)
{
	CAtlRegExp<CAtlRECharTraitsA>  re;
	CAtlREMatchContext<CAtlRECharTraitsA> mc ;

	int segFlag =0;
	int segIndex=0;
	CStringA strLine;


	for(int i0=0;i0<mLines.GetSize();i0++) {
		strLine = mLines[i0].Trim();
		
		if (segFlag == 0) {
			for (int i1=0;i1<7;i1++) {
				re.Parse(strBegin[i1]);
				if(re.Match(strLine,&mc)) {
					segFlag = 1;
					segIndex = i1;
					break;
				}
			}
			continue;
		}
		else {
			re.Parse(strEnd[segIndex]);
			if(re.Match(strLine,&mc)) {
				segFlag = 0;
			}
			else {
				mSegments[segIndex].mLines.Add(strLine);
			}	
		}
	}
	return 0;

}


//--------------------------------------
// Parse Wire define
//--------------------------------------
int CBuilder::GetWireDef(void)
{
	CAtlRegExp<CAtlRECharTraitsA>  re;
	CAtlRegExp<CAtlRECharTraitsA>  re2;
	CAtlREMatchContext<CAtlRECharTraitsA> mc ;
	CAtlREMatchContext<CAtlRECharTraitsA> mc2 ;
	const CAtlREMatchContext<CAtlRECharTraitsA>::RECHAR * szStart = 0;
	const CAtlREMatchContext<CAtlRECharTraitsA>::RECHAR * szEnd = 0;
	
	int nLen =0;
	int nSize =mSegments[2].mLines.GetSize();

	re.Parse("^wire {.*};");
	re2.Parse(" *\\[{.*}:{.*}\\] *{[a-zA-Z0-9_]* *}");

	for(int i0=0;i0<nSize;i0++)
	{
		CStringA strLine = mSegments[2].mLines[i0].Trim();

		if (re.Match(strLine,&mc))
		{
			mc.GetMatch(0, &szStart, &szEnd);			
			nLen = szEnd - szStart;
			
			CStringA strTemp = szStart;
			CStringA strRange = strTemp.Left(nLen);
			TWire  wItem;
			wItem.sType = "w";

			if(re2.Match(strRange,&mc2))
			{
				mc2.GetMatch(0, &szStart, &szEnd);
				strTemp = szStart;
				wItem.iMsb = CalcParaValue(strTemp.Left(szEnd-szStart).Trim());
				mc2.GetMatch(1, &szStart, &szEnd);
				strTemp = szStart;
				wItem.iLsb = CalcParaValue(strTemp.Left(szEnd-szStart).Trim());

				mc2.GetMatch(2, &szStart, &szEnd);		
				strTemp = szStart;
				wItem.sName = strTemp.Left(szEnd-szStart).Trim();

				mWires.SetAt(wItem.sName,wItem);
			}
			else {
				wItem.sName = strTemp.Left(szEnd-szStart).Trim();
				mWires.SetAt(wItem.sName,wItem);
			}
		}		
	}

	return 0;
}

//-------------------------------------------
// 从文件中提取模块例化信息
//-------------------------------------------
int CBuilder::GetInstDef(void)
{
	CAtlRegExp<CAtlRECharTraitsA>  re;
	CAtlRegExp<CAtlRECharTraitsA>  re2;
	CAtlRegExp<CAtlRECharTraitsA>  re3;

	CAtlREMatchContext<CAtlRECharTraitsA> mc ;
	CAtlREMatchContext<CAtlRECharTraitsA> mc2 ;

	const CAtlREMatchContext<CAtlRECharTraitsA>::RECHAR * szStart = 0;
	const CAtlREMatchContext<CAtlRECharTraitsA>::RECHAR * szEnd = 0;

	int nLen =0;
	int instFlag =0;
	CInst *inst;

	int nSize = mSegments[4].mLines.GetSize();
	
	re.Parse("// INSTANCE\\({.*},{.*},{.*}\\)");//module,instname,file
	re2.Parse("// PARAMETER\\({.*},{.*}\\)");//parameter,value
	re3.Parse("// CONNECT\\({.*},{.*}, {[io]} ,{.*}, {[iowcne]}\\)"); //input,output,wire,const,notcare,error

	for (int i0=0;i0<nSize;i0++) {
		CStringA strLine = mSegments[4].mLines[i0].Trim();		

		if (instFlag == 1) {
			if(strLine.Trim()=="") {
				instFlag = 0;
				mInsts.SetAt(inst->InstName,inst);
			}
			else if (re2.Match(strLine,&mc)) {   //PARAMETER
				CStringA paramName;
				CStringA paramValue;

				mc.GetMatch(0, &szStart, &szEnd);	
				paramName = (CStringA(szStart)).Left(szEnd - szStart).Trim();
				
				mc.GetMatch(1, &szStart, &szEnd);			
				paramValue = (CStringA(szStart)).Left(szEnd - szStart).Trim();

				inst->mParams.SetAt(paramName,atoi(paramValue));
			}
			else if (re3.Match(strLine,&mc)) {   //PORTS
				TLink link;

				mc.GetMatch(0, &szStart, &szEnd);
				link.ioName = (CStringA(szStart)).Left(szEnd - szStart).Trim();
				
				mc.GetMatch(1, &szStart, &szEnd);
				link.iWidth = atoi((CStringA(szStart)).Left(szEnd - szStart).Trim());
				
				mc.GetMatch(2, &szStart, &szEnd);
				link.ioType = (CStringA(szStart)).Left(szEnd - szStart).Trim();

				mc.GetMatch(3, &szStart, &szEnd);
				link.wName = (CStringA(szStart)).Left(szEnd - szStart).Trim();

				mc.GetMatch(4, &szStart, &szEnd);
				link.wType = (CStringA(szStart)).Left(szEnd - szStart).Trim();

				inst->mLinks.SetAt(link.ioName,link);
			}
		}
		else if (re.Match(strLine,&mc)) {   //INSTANCE
			instFlag = 1;
			inst = new CInst();
			
			mc.GetMatch(0, &szStart, &szEnd);			
			nLen = szEnd - szStart;
			CStringA strTemp = szStart;
			strTemp = strTemp.Left(nLen);

			inst->ModuleName = (CStringA(szStart)).Left(szEnd - szStart).Trim();

			mc.GetMatch(1, &szStart, &szEnd);	
			inst->InstName = (CStringA(szStart)).Left(szEnd - szStart).Trim();

			mc.GetMatch(2, &szStart, &szEnd);	
			inst->FilePath = (CStringA(szStart)).Left(szEnd - szStart).Trim();
		}

	}

	return 0;
}

//-----------------------------------------------------
//添加新的例化单元
//-----------------------------------------------------
CInst* CBuilder::AddNewInst(CString fn)
{
	
	CModule module;
	int     i=0;
	module.Load(fn);
	module.GetPorts(module.mLines);


	CInst *inst=new CInst();
	module.GenInstance(inst);
	

	while(mInsts.Lookup(inst->InstName) != NULL) {
		i++;
		CStringA strIdx;
		strIdx.Format("%d",i);
		inst->InstName = "U" + strIdx  + "_" + inst->ModuleName;
	}

	mInsts[inst->InstName] = inst;



	return inst;
}

//------------------------------------------------------
// 保存文件
//------------------------------------------------------
int CBuilder::Update()
{
	return 0;
}
//------------------------------------------------------
// 保存文件
//------------------------------------------------------
int CBuilder::SaveAs(CString fn)
{
	CStringA line;
	CSimpleArray<CStringA> lines;
	int nNum;
	CAtlFile hFile;	

	mSegments[1].mLines.RemoveAll();  //ports
	mSegments[2].mLines.RemoveAll();  //wires
	mSegments[4].mLines.RemoveAll();  //insts
	mSegments[5].mLines.RemoveAll();  //insts
	mSegments[6].mLines.RemoveAll();  //position
	
	
	GetPortsDef(mSegments[1].mLines);
	GetWiresDef(mSegments[2].mLines);

	POSITION pos = mInsts.GetStartPosition();
	CInst* inst;
	while(pos != NULL) {
		inst = mInsts.GetNextValue(pos);
		inst->GenInstanceText(mSegments[4].mLines);
		inst->GenInstanceCode(mSegments[5].mLines);
		inst->GenUIContext(mSegments[6].mLines);
	}


	HRESULT hr = hFile.Create(fn,GENERIC_WRITE,FILE_SHARE_WRITE,CREATE_ALWAYS);
	
	for(int i0=0;i0<7;i0++) {
		nNum = mSegments[i0].mLines.GetSize();
		hFile.Write(strBegin[i0]+"\n",strBegin[i0].GetLength()+1);
		for (int i1=0;i1<nNum;i1++)	{
			line = mSegments[i0].mLines[i1] + "\n";
			hFile.Write(line.GetBuffer(),line.GetLength());			
		}
		hFile.Write(strEnd[i0]+"\n",strEnd[i0].GetLength()+1);
		hFile.Write("\n",1);
		if (i0==0) { 
			GetModuleHead(lines);
			nNum = lines.GetSize();
			for (int i1=0;i1<nNum;i1++)	{
				line = lines[i1] + "\n";
				hFile.Write(line.GetBuffer(),line.GetLength());			
			}
		}
		else if (i0==1)
		{
			hFile.Write(");\n",3);
		}
	}

	hFile.Write("endmodule\n",10);
	hFile.Close();

	return 0;
}

//------------------------------------------------------
// 生成模块名和参数定义
//------------------------------------------------------
int CBuilder::GetModuleHead(CSimpleArray<CStringA> &lines)
{
	CStringA strComma;
	CStringA strFmt;
	lines.Add("module " + mModuleName );
	if (mParams.GetCount()>0) {
		lines.Add("    #(");
		POSITION pos = mParams.GetStartPosition();
		while(pos!=0) {
			CAtlMap<CStringA,UINT>::CPair *p= mParams.GetNext(pos);
			strComma = ( pos == 0 ) ? "" : ",";	
			strFmt.Format("    parameter %-20s = %-20d%s",p->m_key,p->m_value,strComma);
			lines.Add(strFmt);
		}
		lines.Add("    )");
	}
	lines.Add("(");
	
	return 0;
}

//------------------------------------------------------
// 生成端口申明
//------------------------------------------------------
int CBuilder::GetPortsDef(CSimpleArray<CStringA> &lines)
{
	CStringA strComma;
	CStringA strDef;
	CStringA strRange;
	CStringA strInOut;
	CStringA strName;
	CStringA strTemp;
	if (mWires.GetCount()>0) {
		POSITION pos = mWires.GetStartPosition();
		while(pos!=0) {
			CAtlMap<CStringA,TWire>::CPair *p= mWires.GetNext(pos);
			if (p->m_value.sType != "w") {  //NOT WIRE
				strInOut = p->m_value.sType == "i" ? "input         " : "output        ";
				strTemp.Format("[%3d:%d]",p->m_value.iMsb,p->m_value.iLsb);
				strRange.Format("%10s",p->m_value.iMsb == p->m_value.iLsb ? "" : strTemp);
				strName.Format("  %-20s",p->m_value.sName);
				strComma = ( pos == 0 ) ? "" : ",";	
				strDef   = strInOut + strRange + strName + strComma; 

				lines.Add(strDef);
			}
		}
	}

	return 0;
}

//------------------------------------------------------
// 生成Wire信号申明
//------------------------------------------------------
int CBuilder::GetWiresDef(CSimpleArray<CStringA> &lines)
{
	CStringA strDef;
	CStringA strRange;
	CStringA strInOut;
	CStringA strName;
	CStringA strTemp;
	if (mWires.GetCount()>0) {
		POSITION pos = mWires.GetStartPosition();
		while(pos!=0) {
			CAtlMap<CStringA,TWire>::CPair *p= mWires.GetNext(pos);
			if (p->m_value.sType == "w") {
				strTemp.Format("[%3d:%d]",p->m_value.iMsb,p->m_value.iLsb);
				strRange.Format("%19s",p->m_value.iMsb == p->m_value.iLsb ? "" : strTemp);
				strName.Format("  %-20s",p->m_value.sName);

				strDef   = "wire " + strRange + strName + ";"; 
				lines.Add(strDef);
			}
		}
	}

	return 0;
}