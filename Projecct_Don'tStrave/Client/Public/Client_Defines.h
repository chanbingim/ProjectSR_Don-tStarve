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
	const unsigned int			g_iTileCnt = 32;



	enum class LEVEL { STATIC, GAMEPLAY_STATIC, LOADING, LOGO, TUTORIAL, GAMEPLAY, END };
	enum class ITEM_TYPE { MERTARIAL, FOOD, EQUIPMENT, STRUCTURE};
	enum class SLOT { NORMAL, HAND, HAT, ARMOR, INFO };
	enum class CATEGORY { TOOL, FIRE, MACHINE, WEAPON, FOOD, STRUCTURE, END };
	enum class FOOD { MEAT, FRUIT, END };
	enum class SWAPOBJECT { AXE, GOLDAXE, PICKAXE, GOLDPICKAXE, SHOVEL, GOLDSHOVEL, SPEAR, TORCH, NONE, END };
	enum class FIELDOBJECT { OBJECT, CREATURE, END };

	typedef struct Item_Desc
	{
		ITEM_TYPE		eItemType{};
		SLOT			eSlot{};
		unsigned int	iItemID{};
		unsigned int	iNumItem{};
		float			fDurability{};
		D3DXVECTOR3		vPosition{};
		unsigned int	iItemEffect{}; // 0 ~ 진호형이 원하는 거까지 이펙트 연동
	}ITEM_DESC;

	typedef struct Item_Data
	{
		unsigned int	iItemID{};
		wstring			strName{};
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
		int	iHealthChange{};
		int	iHungerChange{};
		int	iSanityChange{};
		unsigned int	iRotTime{};
		FOOD			eFoodtype{};
	}ITEM_DATA;

#pragma region FILE_LOAD_DATA
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
		_float			iAtkSpeed = {};
		_uint			iMaxHit = {};
		_uint			iHostile = {};
		wstring			strWilsonDial = {};
		wstring			strWigfridDial = {};
		_float3			fPos = { 0.f,0.f,0.f };
	}MONSTER_DESC;
#pragma endregion

#pragma region INGAME_DATA
	typedef struct Object_Base_Desc
	{
		_int			iHp = {};
		_uint			iMaxHp = {};

		_int			iHit = {};
		_uint			iMaxHit = {};
	}BASE_DESC;

	typedef struct Character_Data : BASE_DESC {
		_uint			iTemp = {};
		_uint			iAtk = {};
		_float			fSpeed = {};
		_float3			fPos = {};
		_bool			bIsDead = {};
	}CHARACTER_DATA;

	typedef struct Player_Data : Character_Data {
		_uint					iId = {};
		_float					fAtkRatio = {};
		_float					fDefRatio = {};
		_uint					iMaxHunger = {};
		_uint					iMaxMental = {};
		_uint					iDef = {};
		_int					iHunger = {};
		_int					iMental = {};
		SWAPOBJECT				tItem = {};
		CGameObject*			pWorkObject = {};
	}PLAYER_DATA;

	typedef struct Monster_Data : Character_Data {
		_uint		iId = {};
		_float		iAtkDistance = {};
		_float		iAtkSpeed = {};
		_uint		iHostile = {};
	}MONSTER_DATA;

	typedef struct DamageBaseDesc{
		void*					Attacker = { nullptr };
		_int					Damage = {};
		_float3					Direaction = {};
	}DAMAGE_DATA_BASE;
#pragma endregion
}

using namespace Client;
extern HWND g_hWnd;
extern short g_Wheel;
extern	GAME_SETTING_DESC		g_GameSetting;