#pragma once

#include <process.h>
#include "../Default/framework.h"
#include "File.h"

namespace Client
{
	typedef struct GamePlayStr
	{
		unsigned int			iMaxFrame = { 60 };
		float					fGameDayLength = { 60.f };
		float					fBgmVolume = { 20.f };
	}GAME_SETTING_DESC;

	const unsigned int			g_iWinSizeX = 1280;
	const unsigned int			g_iWinSizeY = 720;

	enum class LEVEL { STATIC, GAMEPLAY_STATIC, LOADING, LOGO, TUTORIAL, GAMEPLAY, END };
	enum class ITEM_TYPE { MERTARIAL, FOOD, EQUIPMENT };
	enum class SLOT { NORMAL, HAND, HAT, ARMOR, INFO };

	typedef struct Item_Desc
	{
		ITEM_TYPE		eItemType{};
		SLOT			eSlot{};
		unsigned int	iItemID{};
		unsigned int	iNumItem{};
		float			fDurability{};
		D3DXVECTOR3		vPosition{};
	}ITEM_DESC;
}

using namespace Client;
extern HWND g_hWnd;
extern	GAME_SETTING_DESC		g_GameSetting;