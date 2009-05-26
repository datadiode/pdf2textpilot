#pragma once

#ifndef __CMDLINE_H__
#define __CMDLINE_H__


struct CDocumentSettings
{
	CString Password;
	bool AllToOne;
	bool Author;
	bool Subject;
	bool Title;
	bool Keywords;
};

bool String2Params(LPSTR lpstrCmdLine, CAtlArray<CString>* arrParams);
int Params2Settings(CAtlArray<CString>* arrParams, UINT iStartIndex, CDocumentSettings& FileSettings);
int ConvertFile(CString in, CString out, CDocumentSettings FileSettings);
int ConvertFileList2Dir(CString in, CString out, CDocumentSettings FileSettings);
int ConvertFileList2FileList(CString in, CString out, CDocumentSettings FileSettings);
bool File2Array(CAtlArray<CString>* arrFiles, CString strInFileList);
CString MakeAbsolutePath(LPCTSTR strFilePath, LPCTSTR strBasePath = NULL);
CString GetNextParam(CAtlArray<CString>* arrParams, UINT iIndex);

#endif