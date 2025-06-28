#pragma once
#include "Engine_Defines.h"

NS_BEGIN(Engine)
class ENGINE_DLL CUtility
{
public :
	static const char* ConvertWideToUTF(const WCHAR* _wstr, char* _Outstr);
	static const WCHAR* ConvertUTFToWide(const char* _str, WCHAR* _outwstr);
};
NS_END