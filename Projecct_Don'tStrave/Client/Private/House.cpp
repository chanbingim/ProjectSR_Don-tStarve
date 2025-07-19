#include "House.h"

CHouse::CHouse(LPDIRECT3DDEVICE9 pGraphic_Device):
	CMonster{pGraphic_Device}
{
}

CHouse::CHouse(const CHouse& Prototype):
	CMonster{ Prototype }
{
}
