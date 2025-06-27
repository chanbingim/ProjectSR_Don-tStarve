#pragma once

#include <process.h>
#include "../Default/framework.h"

namespace Client
{
	const unsigned int			g_iWinSizeX = 1280;
	const unsigned int			g_iWinSizeY = 720;

	enum class LEVEL { STATIC, LOADING, LOGO, GAMEPLAY, END };
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

/* ���콺��ŷ. */
/* ����. */
/* ��ī�̹ڽ� */
/* ���ļ���. */
/* ��Ʈ */


























/* 1. ��ġ�ӿ� �����е��� ������.  */
/* 1. �˽ѳ�(��). */
/* 1. �����ǽİ�.(��) */
/* 1. ��õ����.(��) */
/* 1. ���̻�, ���̻� */
/* 1. ���¹����ڻ��� 1, 2 .(��) */
/* 1. �߰׸�����2��.(��) */
/* 1. ���ѻڶ���.(��) */
/* 1. ���չ��� ���̵�(����) */
/* 1. ����.(��) */
/* 1. �ڽ�����.(��, ��) */
/* 1. ���Ʈ��. */
/* 1. ������Ӿ�ī���� */
/* 1. ����� ���� */
/* 1. ����� Ż��. (���, �ְ� ������, �ְ� ī��, ) */

