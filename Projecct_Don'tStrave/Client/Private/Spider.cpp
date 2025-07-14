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
	if (0 == data.iHostile) {
		data.iHostile = 1;
		m_pMonsterData->iHostile = data.iHostile;
		if (FAILED(Initialize_Late()))
			return E_FAIL;
	}

	m_bNeedItem = false;
	return S_OK;
}

HRESULT CSpider::Initialize_Late()
{
	__super::Initialize_Late();
	auto GroundObejcts = m_pGameInstance->GetAllObejctsToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Monster"));
	if (GroundObejcts && !GroundObejcts->empty()) {
		for (auto& object : (*GroundObejcts)) {
			_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
			_float distance = D3DXVec3Length(&transform);
			if (0.1f > distance) {
				if (m_pHouse = dynamic_cast<CSpiderHouse*>(object)) {
					m_pMonsterData->fPos += +_float3(((rand() % 10) / 20.f) - ((rand() % 10) / 20.f), 0.f, ((rand() % 10) / 20.f) - ((rand() % 10) / 20.f));
					m_pTransformCom->SetPosition(m_pMonsterData->fPos);
					m_pHouse->EnterSpider(this);
					m_bActive = false;
					m_bHouse = true;
					break;
				}
			}
		}
	}
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

void CSpider::OutHouse(CCharacter* pCharacter)
{
	if (pCharacter && !pCharacter->isDead()) {
		m_pNearTarget = pCharacter;
		_float3 transform = m_pMonsterData->fPos - *m_pPlayerPos;
		m_fNearDistance = D3DXVec3Length(&transform);
	}
	m_bActive = true;
}

void CSpider::Damage(void* pArg)
{
	__super::Damage(pArg);


	list<CGameObject*> NearObjects;

	auto GroundObejcts = m_pGameInstance->GetAllObejctsToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Monster"));
	if (GroundObejcts && !GroundObejcts->empty()) {
		for (auto& object : (*GroundObejcts)) {
			_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
			_float distance = D3DXVec3Length(&transform);
			if (3.f > distance) {
				CSpiderHouse* pHouse = {};
				if (pHouse = dynamic_cast<CSpiderHouse*>(object)) {
					if (pArg) {
						DamageBaseDesc desc;
						desc = *static_cast<DamageBaseDesc*>(pArg);
						pHouse->Emergency(static_cast<CCharacter*>(pArg));
					}
				}
			}
		}
	}
}

void CSpider::Free()
{
	__super::Free();
}
