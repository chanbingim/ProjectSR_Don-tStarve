#pragma once

#include <process.h>
#include "../Default/framework.h"
#include "File.h"

#define D3DFVF_PARTICLEVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_PSIZE) // 변경 했음

NS_BEGIN(Engine)
class CGameObject;
NS_END

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
	enum class ITEM_TYPE { MERTARIAL, FOOD, EQUIPMENT, STRUCTURE};
	enum class SLOT { NORMAL, HAND, HAT, ARMOR, INFO };
	enum class CATEGORY { TOOL, FIRE, MACHINE, WEAPON, FOOD, STRUCTURE, END };
	enum class FOOD { MEAT, FRUIT, END };
	enum class SWAPOBJECT { NONE, AXE, GOLDAXE, PICKAXE, GOLDPICKAXE, SHOVEL, TORCH, END };

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
		wstring		strName{};
		wstring			strKorName{};
		ITEM_TYPE		eItemType{};
		SLOT			eSlot{};
		unsigned int	iMaxStack{};
		unsigned int	iMaterialID_1{};
		unsigned int	iNumMaterial_1{};
		unsigned int	iMaterialID_2{};
		unsigned int	iNumMaterial_2{};
		unsigned int	iMaterialID_3{};
		unsigned int	iNumMaterial_3{};
		wstring			strExplanation = {};
		wstring			strLines = {};
		unsigned int	iHealthChange{};
		unsigned int	iHungerChange{};
		unsigned int	iSanityChange{};
		unsigned int	iRotTime{};
		FOOD			eFoodtype{};
	}ITEM_DATA;

	typedef struct Player_Desc {
		_uint			iId = {};
		wstring			strName = {};
		_uint			iMaxHp = {};
		_uint			iMaxHunger = {};
		_uint			iMaxMental = {};
		_float			fAtk = {};
		_float			fDef = {};
		_float3			fPos = { 0.f,0.f,0.f };
	}PLAYER_DESC;

	typedef struct Monster_Desc {
		_uint			iId = {};
		wstring			strPath = {};
		wstring			strName = {};
		_uint			iMaxHp = {};
		_uint			iAtk = {};
		_float			fSpeed = {};
		_uint			iAtkDistance = {};
		_uint			iAtkSpeed = {};
		_uint			iMaxHit = {};
		wstring			strWilsonDial = {};
		wstring			strWigfridDial = {};
		_float3			fPos = { 0.f,0.f,0.f };
	}MONSTER_DESC;

	typedef struct Character_Data {
		_uint			iMaxHp = {};
		_uint			iTemp = {};
		_uint			iAtk = {};
		_uint			iMaxHit = {};
		_float			fSpeed = {};
		_int				iHp = {};
		_int				iHit = {};
		_float3			fPos = {};
	}CHARACTER_DATA;

	typedef struct Player_Data : Character_Data {
		_float					fAtkRatio = {};
		_float					fDefRatio = {};
		_uint					iMaxHunger = {};
		_uint					iMaxMental = {};
		_uint					iDef = {};
		_uint					iHunger = {};
		_uint					iMental = {};
		SWAPOBJECT				tItem = {};
		CGameObject*				pWorkObject = {};
	}PLAYER_DATA;

	typedef struct Monster_Data : Character_Data {
		_uint		iId = {};
		_float		iAtkDistance = {};
		_uint		iAtkSpeed = {};
	}MONSTER_DATA;
}

using namespace Client;
extern HWND g_hWnd;
extern	GAME_SETTING_DESC		g_GameSetting;