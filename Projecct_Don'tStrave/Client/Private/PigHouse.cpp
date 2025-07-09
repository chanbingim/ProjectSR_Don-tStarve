#include "PigHouse.h"
#include "GameInstance.h"
#include "XML_Manager.h"
#include "MonsterData_Manager.h"
#include "Pig.h"
#include "Camera.h"
#include "Player.h"

CPigHouse::CPigHouse(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CHouse{ pGraphic_Device }
{
}

CPigHouse::CPigHouse(const CPigHouse& Prototype)
	: CHouse{ Prototype }
{
}

HRESULT CPigHouse::Initialize_Prototype()
{
	CXML_Manager::GetInstance()->AddTexture("../Bin/Resources/Textures/Monster/PigHouse/pighouse.scml", L"../Bin/Resources/Textures/Monster/PigHouse/", &m_tImageVec);
	CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/PigHouse/pighouse.scml", &m_tAnimation);
	return S_OK;
}

HRESULT CPigHouse::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	LoadImageFile();

	SetAnimation(MOTION::ADD);

	MONSTER_DESC data = CMonsterData_Manager::GetInstance()->Get_MonsterData(106);

	data.fPos = m_pMonsterData->fPos;;
	m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), data.strPath.c_str(), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Monster"), &data);

	m_pCollision_Com->SetCollisionSize({ 0.3f, 0.f ,0.f });

	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& _Dir) { BeginHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& _Dir) { OverlapHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& _Dir) { EndHitActor(HitActor, _Dir); });

	return S_OK;
}

void CPigHouse::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CPigHouse::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	switch (m_tMotion)
	{
	case CPigHouse::DAMAGE:
	case CPigHouse::ADD:
		if (m_iLength <= m_fAniTime) {
			SetAnimation(MOTION::IDLE);
		}
		break;
	}
}

void CPigHouse::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (m_pCamera->IsInObject(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION)))
		m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);
}

HRESULT CPigHouse::Render()
{
	__super::Render();
	if (!m_isDead) {
		XMLRenderAnimation(m_sAnim);
	}
	return S_OK;
}

HRESULT CPigHouse::SetAnimation(MOTION motion)
{
	if (motion != m_tMotion) {
		m_fAniTime = 0;
	}
	m_tMotion = motion;
	switch (motion)
	{
	case MOTION::ADD:
		m_sAnim = L"place";
		break;
	case MOTION::IDLE:
		m_sAnim = L"idle";
		break;
	case MOTION::LIGHT:
		m_sAnim = L"lit";
		break;
	case MOTION::DAMAGE:
		m_sAnim = L"hit";
		break;
	case MOTION::DEATH:
		m_sAnim = L"unbuilt";
		break;
	case MOTION::BURN:
		m_sAnim = L"burnt";
		break;
	}
	return S_OK;
}

void CPigHouse::Damage(void* pArg)
{
	__super::Damage(pArg);
	DAMAGE_DATA_BASE DamageBase = {};
	if (nullptr != pArg) {
		DamageBase = *static_cast<DAMAGE_DATA_BASE*>(pArg);
		if (DamageBase.Attacker) {
			CCharacter* character = static_cast<CCharacter*>(pArg);
			if (dynamic_cast<CPlayer*>(character)) {
				m_pPig->Get_Monster()->iHostile = 1;
			}
		}
	}
	Emergency();
}

void CPigHouse::Hit()
{
	SetAnimation(m_tMotion);
}

void CPigHouse::Attack()
{
}

void CPigHouse::Death()
{
	SetAnimation(m_tMotion);
}

void CPigHouse::EnterPig(CPig* pMonster)
{
}

void CPigHouse::Emergency()
{
	m_pPig->OutHouse();
	m_pPig = nullptr;
}

void CPigHouse::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CPigHouse::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CPigHouse::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

CPigHouse* CPigHouse::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPigHouse* pInstance = new CPigHouse(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPigHouse::Clone(void* pArg)
{
	CPigHouse* pInstance = new CPigHouse(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPigHouse");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPigHouse::Free()
{
	__super::Free();
}
