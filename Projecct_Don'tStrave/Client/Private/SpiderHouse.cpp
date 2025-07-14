#include "SpiderHouse.h"
#include "GameInstance.h"
#include "XML_Manager.h"
#include "MonsterData_Manager.h"
#include "Spider.h"
#include "Camera.h"

CSpiderHouse::CSpiderHouse(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CHouse{ pGraphic_Device }
{
}

CSpiderHouse::CSpiderHouse(const CSpiderHouse& Prototype)
	: CHouse{ Prototype }
{
}

HRESULT CSpiderHouse::Initialize_Prototype()
{
	CXML_Manager::GetInstance()->AddTexture("../Bin/Resources/Textures/Monster/SpiderHouse/spiderhouse.scml", L"../Bin/Resources/Textures/Monster/SpiderHouse/", &m_tImageVec);
	CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/SpiderHouse/spiderhouse.scml", &m_tAnimation);
	return S_OK;
}

HRESULT CSpiderHouse::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	LoadImageFile();
	m_pMonsterVec.clear();

	SetAnimation(MOTION::SMALL);

	MONSTER_DESC data = CMonsterData_Manager::GetInstance()->Get_MonsterData(100);
	size_t max = (rand() % 3) + 1;
	data.fPos = m_pMonsterData->fPos;
	for (size_t i = 0; i < max; i++)
	{
		m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), data.strPath.c_str(), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Monster"), &data);
	}
	
	m_pCollision_Com->SetCollisionSize({ 0.3f, 0.f ,0.f });

	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& _Dir) { BeginHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& _Dir) { OverlapHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& _Dir) { EndHitActor(HitActor, _Dir); });
	m_bRecon = false;
	return S_OK;
}

void CSpiderHouse::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	m_fTimeAcc += fTimeDelta;
	if (m_fTimeAcc >= 120.f) {

		switch (m_tMotion) {
		case SMALL:
			SetAnimation(MOTION::SMALL_TO_MEDIUM);
			break;
		case MEDIUM:
			SetAnimation(MOTION::MEDIUM_TO_LARGE);
			break;
		case LARGE:
			SetAnimation(MOTION::LARGE_TO_QUEEN);
			break;
		}
		m_fTimeAcc = 0.f;
	}
}

void CSpiderHouse::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	MONSTER_DESC data;
	switch (m_tMotion)
	{
	case MOTION::SMALL_DAMAGE:
		if (m_iLength <= m_fAniTime) {
			SetAnimation(MOTION::SMALL);
		}
		break;
	case MOTION::SMALL_TO_MEDIUM:
		if (m_iLength <= m_fAniTime) {

			data = CMonsterData_Manager::GetInstance()->Get_MonsterData(104);
			m_pMonsterData->iId = data.iId;
			m_pMonsterData->iMaxHp = data.iMaxHp;
			m_pMonsterData->iHp = data.iMaxHp;

			size_t max = (rand() % 2);
			for (size_t i = 0; i < max; i++)
			{
				if (2 < rand() % 10) {
					data = CMonsterData_Manager::GetInstance()->Get_MonsterData(100);
					data.fPos = m_pMonsterData->fPos;
					data.iHostile = 0;
				}
				else {
					data = CMonsterData_Manager::GetInstance()->Get_MonsterData(101);
					data.fPos = m_pMonsterData->fPos;
					data.iHostile = 0;
				}
				m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), data.strPath.c_str() , ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Monster"), &data);
			}
		}
	case MOTION::MEDIUM_DAMAGE:
		if (m_iLength <= m_fAniTime) {
			SetAnimation(MOTION::MEDIUM);
		}
		break;
	case MOTION::MEDIUM_TO_LARGE:
		if (m_iLength <= m_fAniTime) {
			data = CMonsterData_Manager::GetInstance()->Get_MonsterData(105);
			m_pMonsterData->iId = data.iId;
			m_pMonsterData->iMaxHp = data.iMaxHp;
			m_pMonsterData->iHp = data.iMaxHp;

			data = CMonsterData_Manager::GetInstance()->Get_MonsterData(101);
			data.fPos = m_pMonsterData->fPos;
			data.iHostile = 0;
			size_t max = (rand() % 3);
			for (size_t i = 0; i < max; i++)
			{
				m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), data.strPath.c_str(), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Monster"), &data);
			}
		}
	case MOTION::LARGE_DAMAGE:
		if (m_iLength <= m_fAniTime) {
			SetAnimation(MOTION::LARGE);
		}
		break;
	case MOTION::LARGE_TO_QUEEN:
		if (m_iLength <= m_fAniTime) {
			for (auto iter = m_pMonsterVec.begin(); iter != m_pMonsterVec.end();)
			{
				(*iter)->SetDead();
				iter = m_pMonsterVec.erase(iter);
			}
			m_isDead = true;
			data = CMonsterData_Manager::GetInstance()->Get_MonsterData(102);
			data.fPos= m_pMonsterData->fPos;
			m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), data.strPath.c_str(), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Monster"), &data);
		}
		break;
	case MOTION::LARGE_TO_SMALL:
		if (m_iLength <= m_fAniTime) {
			SetAnimation(MOTION::SMALL);
		}
		break;
	case MOTION::DEATH:
		if (m_fTimeAcc >= 5.f) {
			m_isDead = true;
		}
		break;
	}
	if (0 != m_pMonsterVec.size() && m_bRecon && 30 < *m_pTime) {
		(*m_pMonsterVec.begin())->OutHouse(nullptr);
		m_pMonsterVec.erase(m_pMonsterVec.begin());
		m_bRecon = false;
	}
	else if(30 >= *m_pTime){
		m_bRecon = true;
	}
}

void CSpiderHouse::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (m_pCamera->IsInObject(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION), 10))
		m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);
}

HRESULT CSpiderHouse::Render()
{
	__super::Render();
	if (!m_isDead) {
		XMLRenderAnimation(m_sAnim);
	}
	return S_OK;
}

HRESULT CSpiderHouse::SetAnimation(MOTION motion)
{
	if (motion != m_tMotion) {
		m_fAniTime = 0;
	}
	switch (motion) {
	case SMALL:
		m_sAnim = L"cocoon_small";
		break;
	case SMALL_DAMAGE:
		m_sAnim = L"cocoon_small_hit";
		break;
	case SMALL_TO_MEDIUM:
		m_sAnim = L"grow_small_to_medium";
		break;
	case MEDIUM:
		m_sAnim = L"cocoon_medium";
		break;
	case MEDIUM_DAMAGE:
		m_sAnim = L"cocoon_medium_hit";
		break;
	case MEDIUM_TO_LARGE:
		m_sAnim = L"grow_medium_to_large";
		break;
	case LARGE:
		m_sAnim = L"cocoon_large";
		break;
	case LARGE_DAMAGE:
		m_sAnim = L"cocoon_large_hit";
		break;
	case LARGE_TO_QUEEN:
		m_sAnim = L"cocoon_large_burst";
		break;
	case LARGE_TO_SMALL:
		m_sAnim = L"cocoon_large_burst_pst";
		break;
	case DEATH:
		m_sAnim = L"cocoon_dead";
		break;
	}
	m_tMotion = motion;
	return S_OK;
}

void CSpiderHouse::Damage(void* pArg)
{
	__super::Damage(pArg);
	DAMAGE_DATA_BASE DamageBase = {};
	if (nullptr != pArg) {
		DamageBase = *static_cast<DAMAGE_DATA_BASE*>(pArg);
		CCharacter* target = static_cast<CCharacter*>(DamageBase.Attacker);
		Emergency(target);
	}
}

void CSpiderHouse::Hit()
{
	switch (m_tMotion) {
	case SMALL:
		SetAnimation(MOTION::SMALL_DAMAGE);
		break;
	case MEDIUM:
		SetAnimation(MOTION::MEDIUM_DAMAGE);
		break;
	case LARGE:
		SetAnimation(MOTION::LARGE_DAMAGE);
		break;
	}
}

void CSpiderHouse::Attack()
{
}

void CSpiderHouse::Death()
{
	__super::Death();
	m_fTimeAcc = 0.f;
	SetAnimation(MOTION::DEATH);
}

void CSpiderHouse::EnterSpider(CSpider* pMonster)
{
	m_pMonsterVec.push_back(pMonster);
}

void CSpiderHouse::Emergency(CCharacter* pCharacter)
{
	for (auto iter = m_pMonsterVec.begin(); iter != m_pMonsterVec.end();)
	{
		(*iter)->OutHouse(pCharacter);
		iter = m_pMonsterVec.erase(iter);
	}
}

void CSpiderHouse::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CSpiderHouse::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CSpiderHouse::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

CSpiderHouse* CSpiderHouse::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSpiderHouse* pInstance = new CSpiderHouse(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpiderHouse::Clone(void* pArg)
{
	CSpiderHouse* pInstance = new CSpiderHouse(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSpiderHouse");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpiderHouse::Free()
{
	__super::Free();
}
