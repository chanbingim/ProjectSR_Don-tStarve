#include "SpiderHouse.h"
#include "GameInstance.h"

#include "XML_Manager.h"
#include "MonsterData_Manager.h"
#include "Spider.h"

CSpiderHouse::CSpiderHouse(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CSpiderHouse::CSpiderHouse(const CSpiderHouse& Prototype)
	: CMonster{ Prototype }
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

	MONSTER_DESC data = CMonsterData_Manager::GetInstance()->Get_MonsterData(0);
	size_t max = (rand() % 4) + 3;
	data.fPos = m_pMonsterData->fPos;;
	for (size_t i = 0; i < max; i++)
	{
		m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), data.strPath.c_str(), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Monster"), &data);
	}
	
	//m_pMonsterVec.push_back();

	m_pCollision_Com->SetCollisionSize({ 1.f, 1.f ,1.f });

	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& _Dir) { BeginHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& _Dir) { OverlapHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& _Dir) { EndHitActor(HitActor, _Dir); });

	return S_OK;
}

void CSpiderHouse::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	m_fTimeAcc += fTimeDelta;
	if (m_fTimeAcc >= 5.f) {

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

			data = CMonsterData_Manager::GetInstance()->Get_MonsterData(4);
			m_pMonsterData->iId = data.iId;
			m_pMonsterData->iMaxHp = data.iMaxHp;
			m_pMonsterData->iHp = data.iMaxHp;

			size_t max = (rand() % 4);
			for (size_t i = 0; i < max; i++)
			{
				if (2 < rand() % 10) {
					data = CMonsterData_Manager::GetInstance()->Get_MonsterData(0);
					data.fPos = m_pMonsterData->fPos;;
				}
				else {
					data = CMonsterData_Manager::GetInstance()->Get_MonsterData(1);
					data.fPos = m_pMonsterData->fPos;;
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
			data = CMonsterData_Manager::GetInstance()->Get_MonsterData(5);
			m_pMonsterData->iId = data.iId;
			m_pMonsterData->iMaxHp = data.iMaxHp;
			m_pMonsterData->iHp = data.iMaxHp;

			data = CMonsterData_Manager::GetInstance()->Get_MonsterData(1);
			data.fPos = m_pMonsterData->fPos;;
			size_t max = (rand() % 4);
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
			m_isDead = true;
			data = CMonsterData_Manager::GetInstance()->Get_MonsterData(2);
			data.fPos= m_pMonsterData->fPos;;
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
}

void CSpiderHouse::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CSpiderHouse::Render()
{
	__super::Render();

	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	RenderAnimation(m_sAnim);

	if (FAILED(End_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpiderHouse::SetAnimation(MOTION motion)
{
	if (motion != m_tMotion) {
		m_fAniTime = 0.f;
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

void CSpiderHouse::Damage()
{
	switch (m_tMotion) {
	case SMALL:
		m_tMotion = MOTION::SMALL_DAMAGE;
		break;
	case MEDIUM:
		m_tMotion = MOTION::MEDIUM_DAMAGE;
		break;
	case LARGE:
		m_tMotion = MOTION::LARGE_DAMAGE;
		break;
	}
	Emergency();
	SetAnimation(m_tMotion);
}

void CSpiderHouse::Attack()
{
}

void CSpiderHouse::Death()
{
	m_fTimeAcc = 0.f;
	m_tMotion = MOTION::DEATH;
	SetAnimation(m_tMotion);
}

void CSpiderHouse::EnterSpider(CSpider* pMonster)
{
	m_pMonsterVec.push_back(pMonster);
}

void CSpiderHouse::Emergency()
{
	for (auto iter = m_pMonsterVec.begin(); iter != m_pMonsterVec.end();)
	{
		(*iter)->OutHouse();
		iter = m_pMonsterVec.erase(iter);
	}
}

HRESULT CSpiderHouse::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	return S_OK;
}

HRESULT CSpiderHouse::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	return S_OK;
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
