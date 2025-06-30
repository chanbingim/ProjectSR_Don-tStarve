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

	enum class LEVEL { STATIC, GAMEPLAY_STATIC, LOADING, LOGO, TUTORIAL, GAMEPLAY, OBJECT, END };
	enum class ITEM_TYPE { MERTARIAL, FOOD, EQUIPMENT, STRUCTURE};
	enum class SLOT { NORMAL, HAND, HAT, ARMOR, INFO };
	enum class CATEGORY { TOOL, FIRE, MACHINE, WEAPON, FOOD, STRUCTURE, END };

	typedef struct Item_Desc
	{
		ITEM_TYPE		eItemType{};
		SLOT			eSlot{};
		unsigned int	iItemID{};
		unsigned int	iNumItem{};
		float			fDurability{};
		D3DXVECTOR3		vPosition{};
	}ITEM_DESC;

	typedef struct Item_Data
	{
		unsigned int	iItemID{};
		string			strName{};
		ITEM_TYPE		eItemType{};
		string			strKorName{};
		SLOT			eSlot{};
		string			strExplanation = {};
		unsigned int    iMaterial_Count{};
		unsigned int	iMaterialID_1{};
		unsigned int	iNumMaterial_1{};
		unsigned int	iMaterialID_2{};
		unsigned int	iNumMaterial_2{};
		unsigned int	iMaterialID_3{};
		unsigned int	iNumMaterial_3{};
	}ITEM_DATA;
}

using namespace Client;
extern HWND g_hWnd;
extern	GAME_SETTING_DESC		g_GameSetting;