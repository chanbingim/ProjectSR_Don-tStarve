#include "CUtility.h"

const char* CUtility::ConvertWideToUTF(const WCHAR* _wstr, char* _str)
{
    int iLength = WideCharToMultiByte(CP_UTF8, 0, _wstr, -1, NULL, 0, NULL, NULL);
    
    memset(_str, 0, sizeof(char) * iLength);
    WideCharToMultiByte(CP_ACP, 0, _wstr, -1, _str, iLength, NULL, NULL);
    return _str;
}

const WCHAR* CUtility::ConvertUTFToWide(const char* _str, WCHAR* _outwstr)
{
    int iLength = MultiByteToWideChar(CP_UTF8, 0, _str, -1, NULL, NULL);

    memset(_outwstr, 0, sizeof(WCHAR) * iLength);
    MultiByteToWideChar(CP_ACP, 0, _str, -1, _outwstr, iLength);
    return _outwstr;
}
