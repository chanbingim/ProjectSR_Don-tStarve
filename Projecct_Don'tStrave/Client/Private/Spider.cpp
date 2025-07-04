#include "Spider.h"
#include "SpiderHouse.h"
#include "GameInstance.h"

CSpider::CSpider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CSpider::CSpider(const CSpider& Prototype)
	: CMonster{ Prototype }
{
}

HRESULT CSpider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSpider::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	MONSTER_DESC data = *static_cast<MONSTER_DESC*>(pArg);
	m_pTransformCom->SetPosition(data.fPos);

	for (auto target : m_pCharacterInstance->Get_NearObject(this, 0.1f)) {
		if (m_pHouse = dynamic_cast<CSpiderHouse*>(target)) {
			m_pHouse->EnterSpider(this);
			m_bOutHouse = false;
			break;
		}
	}
	return S_OK;
}

HRESULT CSpider::Initialize_Late()
{
	return S_OK;
}

void CSpider::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CSpider::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CSpider::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (m_pHouse && m_pHouse->isDead()) {
		m_pHouse = nullptr;
	}
}

HRESULT CSpider::Render()
{
	__super::Render();

	return S_OK;
}

void CSpider::Damage()
{
	for (auto target : m_pCharacterInstance->Get_NearObject(this, 3.f)) {
		if (m_pHouse = dynamic_cast<CSpiderHouse*>(target)) {
			m_pHouse->Emergency();
		}
	}
}

void CSpider::Free()
{
	__super::Free();
}
