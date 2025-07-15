#include "IceSpike.h"
#include "GameInstance.h"
#include "XML_Manager.h"
#include "Deerclops.h"
#include "Terrain.h"
#include "Terrian_Manager.h"
#include "Camera.h"

CIceSpike::CIceSpike(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CIceSpike::CIceSpike(const CIceSpike& Prototype)
	: CMonster{ Prototype }
{
}

HRESULT CIceSpike::Initialize_Prototype()
{
	auto XML_Instance = CXML_Manager::GetInstance();

	XML_Instance->AddTexture("../Bin/Resources/Textures/Particles/IceSpike/deerclops_icespike.scml", L"../Bin/Resources/Textures/Particles/IceSpike/", &m_tImageVec);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Particles/IceSpike/deerclops_icespike.scml", &m_tAnimation);

	return S_OK;
}

HRESULT CIceSpike::Initialize(void* pArg)
{
	ICESPIKE_DESC value = *static_cast<ICESPIKE_DESC*>(pArg);

	if (FAILED(__super::Initialize(&value.tDesc)))
		return E_FAIL;
	LoadImageFile();
	m_fAngle = value.fAngle;
	m_iSpike = value.iType;
	m_tDamage.Attacker = value.pAttacker;
	m_tDamage.DamageType = ATTACK_TYPE::ICE;
	m_bAttack = true;
	m_pTransformCom->SetPosition(value.tDesc.fPos);
	m_iLength = 500;
	m_fAniTime = 0.f;
	m_pCamera->ShakeCamera(m_iSpike / 50.f);

	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& _Dir) { BeginHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& _Dir) { OverlapHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& _Dir) { EndHitActor(HitActor, _Dir); });

	__super::Initialize_Late();

	_float volume = Get_Sound();
	if (0.f < volume) {
		switch (m_iSpike)
		{
		case 1:
			m_pGameInstance->Manager_PlaySound(L"Deerclops_iceattack_big_1.wav", CHANNELID::SOUND_EFFECT, volume);
			break;
		case 2:
		case 3:
			m_pGameInstance->Manager_PlaySound(L"Deerclops_iceattack_big_2.wav", CHANNELID::SOUND_EFFECT, volume);
			break;
		case 4:
			m_pGameInstance->Manager_PlaySound(L"Deerclops_iceattack_big_3.wav", CHANNELID::SOUND_EFFECT, volume);
			break;
		}
	}
	return S_OK;
}

void CIceSpike::Priority_Update(_float fTimeDelta)
{
	if (300 <= (int)m_fAniTime) {
		m_bAttack = false;
	}
	__super::Priority_Update(fTimeDelta);
	if (m_iLength <= m_fAniTime) {
		m_isDead = true;
	}
}

void CIceSpike::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	switch (m_tMoveDIr)
	{
	case MOVE_DIR::MOVE_DOWN:
		m_tDir = DIR::DOWN;
		break;
	case MOVE_DIR::MOVE_LEFT:
	case MOVE_DIR::MOVE_RIGHT:
		m_tDir = DIR::SIDE;
		break;
	case MOVE_DIR::MOVE_UP:
		m_tDir = DIR::UP;
		break;
	default:
		break;
	}
	switch (m_iSpike)
	{
	case 1:
		m_sAnim = L"spike_big1";
		break;
	case 2:
		m_sAnim = L"spike_big2";
		break;
	case 3:
		m_sAnim = L"spike_big3";
		break;
	case 4:
		m_sAnim = L"spike_big4";
		break;
	}
	switch (m_tDir)
	{
	case DIR::DOWN:
		m_sAnim += L"_down";
		break;
	case DIR::SIDE:
		m_sAnim += L"_side";
		break;
	case DIR::UP:
		m_sAnim += L"_up";
		break;
	}
}

void CIceSpike::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (!m_isDead && m_pCamera->IsInObject(m_pMonsterData->fPos, 10))
	{
		SetDir();
		m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);
	}
}

HRESULT CIceSpike::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	RenderAnimation(m_sAnim, m_tAnimation, m_tImageVec);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	return S_OK;
}

void CIceSpike::Damage(void* pArg)
{
}

void CIceSpike::Hit()
{
}

void CIceSpike::Attack()
{
}

void CIceSpike::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CIceSpike::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
	__super::OverlapHitActor(HitActor, _Dir);
	if (m_bAttack && !dynamic_cast<CDeerclops*>(HitActor) && 300 <= (int)m_fAniTime) {
		HitActor->Damage(&m_tDamage);
	}
}

void CIceSpike::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

CIceSpike* CIceSpike::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CIceSpike* pInstance = new CIceSpike(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CIceSpike::Clone(void* pArg)
{
	CIceSpike* pInstance = new CIceSpike(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CIceSpike");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIceSpike::Free()
{
	__super::Free();

	Safe_Release(m_pCollision_Com);
}
