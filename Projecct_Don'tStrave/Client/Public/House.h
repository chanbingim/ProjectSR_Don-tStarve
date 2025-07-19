#pragma once
#include "Monster.h"
class CHouse :public CMonster
{
protected:
	CHouse(LPDIRECT3DDEVICE9 pGraphic_Device);
	CHouse(const CHouse& Prototype);
	virtual ~CHouse() = default;
protected:
	_bool	m_bIsBroken;
public:
	_bool	GetBroken() { return m_bIsBroken; }
};

