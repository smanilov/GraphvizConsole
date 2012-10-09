// GraphvizConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <atlbase.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "WinGraphviz.h"

using namespace std;

const IID IID_IDOT = {0xA1080582,0xD33F,0x486E,{0xBD,0x5F,0x58,0x19,0x89,0xA3,0xC7,0xE9}};
						
const CLSID CLSID_DOT = {0x55811839,0x47B0,0x4854,{0x81,0xB5,0x0A,0x00,0x31,0xB8,0xAC,0xEC}};

void init()
{
	HRESULT hr = CoInitialize(NULL);

	if (FAILED(hr)) {
		cout << "CoInitialize Failed: " << hr << "\n\n";
		exit(1);
	}
	else {
		cout << "CoInitialize succeeded\n";
	}
}

IDOT *createDOT()
{
	IDOT *pIDOT;
	HRESULT hr = CoCreateInstance(CLSID_DOT, NULL, CLSCTX_ALL, IID_IDOT, reinterpret_cast<LPVOID *>(&pIDOT));

	if (FAILED(hr)) {
	  cout << "CoCreateInstance Failed: " << hr << "\n\n";
	  exit(1);
	}
	else {
	  cout << "CoCreateInstance succeeded\n";
	}
	return pIDOT;
}

void generatePNG(IDOT *pIDOT, BSTR *dotString, BSTR *pngName) 
{
	IBinaryImage *png;

	HRESULT hr = pIDOT->ToPNG(*dotString, &png);

	if (FAILED(hr)) {
		cout << (char *)pngName << '\n';
		cout << "ToPNG Failed: " << hr << "\n\n";
		exit(1);
	}
	else {
	  cout << "ToPNG succeeded\n";
	}

	VARIANT_BOOL b(true);
	png->Save(*pngName, &b);
}

string readDotString(char *dotFileName)
{
	ifstream fin(dotFileName);
	if (!fin) {
		cout << "DOT file " << dotFileName << " does not exist.";
		exit(1);
	}
	stringstream ss;
	ss << fin.rdbuf();	
	fin.close();
	return ss.str();
}

int _tmain(int argc, _TCHAR* argv[])
{
	USES_CONVERSION;

	char *dotFileName;
	BSTR pngFileName;

	for (int i = 0; i < argc; ++i) {
		cout << CW2A(argv[i]) << '\n';
	}

	if (argc > 1) {
		dotFileName = CW2A(argv[1]);
	} else {
		dotFileName = "a.dot";
	}

	if (argc > 2) {
		pngFileName = CComBSTR(CW2A(argv[2]));
	} else {
		pngFileName = CComBSTR("a.png");
	}

	init();
	IDOT *pIDOT = createDOT();
	
	BSTR dotString = CComBSTR(readDotString(dotFileName).c_str());
	generatePNG(pIDOT, &dotString, &pngFileName);
	
	CoUninitialize();
	return 0;
}