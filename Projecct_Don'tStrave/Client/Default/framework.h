#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <commdlg.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_dx9.h"
#include "IMGUI/imgui_impl_win32.h"

//STL
#include <vector>
#include <map>
#include <unordered_map>
#include <list>
#include <queue>
#include <string>
#include <algorithm>

namespace Client {};

using namespace std;
using namespace Client;