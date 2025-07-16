#include "Shadowterrorbeak.h"
#include "Player.h"
#include "GameInstance.h"
#include "XML_Manager.h"
#include "Camera.h"

CShadowterrorbeak::CShadowterrorbeak(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{

}

CShadowterrorbeak::CShadowterrorbeak(const CShadowterrorbeak& Prototype)
	: CMonster{ Prototype }
{
}

HRESULT CShadowterrorbeak::Initialize_Prototype()
{
	CXML_Manager::GetInstance()->AddTexture("../Bin/Resources/Textures/Monster/Shadowterrorbeak/shadowterrorbeak.scml", L"../Bin/Resources/Textures/Monster/Shadowterrorbeak/", &m_tImageVec);
	CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/Shadowterrorbeak/shadowterrorbeak.scml", &m_tAnimation);
	return S_OK;
}

HRESULT CShadowterrorbeak::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	LoadImageFile();
	SetAnimation(DIR::DIR_END, MOTION::APPEAR);
	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& _Dir) { BeginHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& _Dir) { OverlapHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& _Dir) { EndHitActor(HitActor, _Dir); });

	__super::Initialize_Late();

	_float volume = Get_Sound();
	if (0.f < volume)
		m_pGameInstance->Manager_PlaySound(L"nightmare_add.wav", CHANNELID::BADMONSTER_SOUND, volume * 0.1f);

	return S_OK;
}


void CShadowterrorbeak::Priority_Update(_float fTimeDelta)
{
	if (m_tMotion == ATTACK && m_bAttack && 240 <= (int)m_fAniTime) {
		m_bAttack = false;
	}
	__super::Priority_Update(fTimeDelta);
	m_pNearTarget = nullptr;
	auto GroundObejcts = m_pGameInstance->GetAllObejctsToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player"));
	if (GroundObejcts && !GroundObejcts->empty() && 0 < dynamic_cast<CCharacter*>(GroundObejcts->front())->Get_Char()->iHp) {
		CGameObject* object = GroundObejcts->front();
		_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
		_float distance = D3DXVec3Length(&transform);
		if (10.f > distance) {
			m_pNearTarget = object;
		}
		else {
			m_bTarget = false;
		}
	}
}

void CShadowterrorbeak::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	switch (m_tMotion)
	{
	case MOTION::IDLE:
	case MOTION::IDLE_TO_RUN:
	case MOTION::RUN:
	case MOTION::RUN_TO_IDLE:
	case MOTION::IDLE_TO_ATTACK:
	case MOTION::ATTACK:
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
		SetAnimation(m_tDir, m_tMotion);
		break;
	}
	if (m_tMotion == MOTION::DEATH) {
		if (m_iLength <= m_fAniTime) {
			m_isDead = true;
		}
		return;
	}
	else if (m_pNearTarget) {
		_float3 move = m_pNearTarget->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pMonsterData->fPos;;
		if ((abs(move.x) + abs(move.z)) / 2.f < 10) {
			if (m_tMotion != MOTION::RUN && m_tMotion != MOTION::IDLE_TO_RUN) {
				switch (m_tMotion)
				{
				case MOTION::TAUNT:
					if (m_iLength <= m_fAniTime) {
						SetAnimation(m_tDir, MOTION::IDLE_TO_RUN);
					}
					break;
				case MOTION::APPEAR:
				case MOTION::ATTACK:
					if (m_iLength <= m_fAniTime) {
						m_fAttackTime = 0;
						SetAnimation(m_tDir, MOTION::IDLE);
					}
					break;
				default:
					if (m_bCol) {
						SetAnimation(m_tDir, MOTION::IDLE);
					}
					else {
						if (m_bTarget) {
							SetAnimation(m_tDir, MOTION::IDLE_TO_RUN);
						}
						else {
							_float volume = Get_Sound();
							if (0.f < volume)
								m_pGameInstance->Manager_PlaySound(L"nightmare_taunt.wav", CHANNELID::BADMONSTER_SOUND, volume * 0.1f);
							SetAnimation(DIR::DIR_END, MOTION::TAUNT);
							m_bTarget = true;
						}
					}
					break;
				}
			}
			else if (m_bCol) {
				SetAnimation(m_tDir, MOTION::IDLE);
			}
			else {
				if (m_tMotion == MOTION::IDLE_TO_RUN && m_iLength <= m_fAniTime)
				{
					SetAnimation(m_tDir, MOTION::RUN);
				}
				D3DXVec3Normalize(&move, &move);

				m_pMonsterData->fPos += move * m_pMonsterData->fSpeed * fTimeDelta;
				m_pTransformCom->SetPosition(m_pMonsterData->fPos);
			}
		}
		else {
			switch (m_tMotion)
			{
			case MOTION::RUN:
				if (m_iLength <= m_fAniTime) {
					SetAnimation(m_tDir, MOTION::RUN_TO_IDLE);
				}
				else {
					D3DXVec3Normalize(&move, &move);
					m_pMonsterData->fPos += move * m_pMonsterData->fSpeed * fTimeDelta;
					m_pTransformCom->SetPosition(m_pMonsterData->fPos);
				}
				break;
			case MOTION::ATTACK:
			case MOTION::RUN_TO_IDLE:
			case MOTION::TAUNT:
				if (m_iLength <= m_fAniTime) {
					SetAnimation(m_tDir, MOTION::IDLE);
				}
				break;
			default:
				break;
			}
		}
	}
	else {
		switch (m_tMotion)
		{
		case MOTION::RUN:
			if (m_iLength <= m_fAniTime) {
				SetAnimation(m_tDir, MOTION::RUN_TO_IDLE);
			}
			break;
		case MOTION::APPEAR:
		case MOTION::ATTACK:
		case MOTION::RUN_TO_IDLE:
		case MOTION::TAUNT:
			if (m_iLength <= m_fAniTime) {
				SetAnimation(m_tDir, MOTION::IDLE);
			}
			break;
		default:
			break;
		}
	}
}

void CShadowterrorbeak::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (m_pCamera->IsInObject(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION), 10))
	{
		SetDir();
		m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);
	}
}

HRESULT CShadowterrorbeak::Render()
{
	__super::Render();
	if (!m_isDead) {

		m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);


		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
		m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(100, 255, 255, 255));
		RenderAnimation(m_sAnim, m_tAnimation, m_tImageVec);

		m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));

		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	}
	return S_OK;
}

void CShadowterrorbeak::Hit()
{
}

void CShadowterrorbeak::Attack()
{
	m_bAttack = true;
	_float volume = Get_Sound();
	if (0.f < volume)
		m_pGameInstance->Manager_PlaySound(L"nightmare_attack.wav", CHANNELID::BADMONSTER_SOUND, volume * 0.1f);
	SetAnimation(m_tDir, MOTION::ATTACK);
}

void CShadowterrorbeak::Death()
{
	__super::Death();
	_float volume = Get_Sound();
	if (0.f < volume)
		m_pGameInstance->Manager_PlaySound(L"nightmare_die.wav", CHANNELID::BADMONSTER_SOUND, volume * 0.1f);
	if (0 >= m_pMonsterData->iHp) {
		auto GroundObejcts = m_pGameInstance->GetAllObejctsToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player"));
		if (GroundObejcts && !GroundObejcts->empty() && 0 < dynamic_cast<CCharacter*>(GroundObejcts->front())->Get_Char()->iHp) {
			CPlayer* player = dynamic_cast<CPlayer*>(GroundObejcts->front());
			player->Get_Player()->iMental = max(0, min(player->Get_Player()->iMaxMental, player->Get_Player()->iMental + 33));
		}
	}
	SetAnimation(DIR::DIR_END, MOTION::DEATH);
}

HRESULT CShadowterrorbeak::SetAnimation(DIR dir, MOTION motion)
{
	if (DIR::DIR_END == dir) {
		m_tDir = DIR::DOWN;
	}
	if (motion != m_tMotion) {
		m_fAniTime = 0;
	}
	m_tMotion = motion;
	switch (motion)
	{
	case MOTION::APPEAR:
		m_sAnim = L"appear";
		break;
	case MOTION::IDLE:
		m_sAnim = L"idle_loop";
		break;
	case MOTION::IDLE_TO_RUN:
		m_sAnim = L"walk_pre";
		break;
	case MOTION::RUN:
		m_sAnim = L"walk_loop";
		break;
	case MOTION::RUN_TO_IDLE:
		m_sAnim = L"walk_pst";
		break;
	case MOTION::ATTACK:
		m_sAnim = L"atk";
		break;
	case MOTION::TAUNT:
		m_sAnim = L"taunt";
		break;
	case MOTION::DEATH:
		m_sAnim = L"disappear";
		break;
	}
	switch (dir)
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
	return S_OK;
}

void CShadowterrorbeak::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CShadowterrorbeak::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
	if (HitActor == m_pNearTarget && m_tMotion != APPEAR && m_tMotion != DEATH) {
		_float3 transform = HitActor->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
		_float distance = D3DXVec3Length(&transform);
		if ((m_pMonsterData->iAtkDistance) >= distance) {
			m_bCol = true;
			if (dynamic_cast<CCharacter*>(HitActor) && m_tMotion != ATTACK && m_pMonsterData->iAtkSpeed <= m_fAttackTime) {
				Attack();
			}
		}
		if (m_tMotion == ATTACK && m_bAttack && 240 <= (int)m_fAniTime) {
			HitActor->Damage(&m_tDamage);
		}
	}
}

void CShadowterrorbeak::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

CShadowterrorbeak* CShadowterrorbeak::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CShadowterrorbeak* pInstance = new CShadowterrorbeak(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CShadowterrorbeak::Clone(void* pArg)
{
	CShadowterrorbeak* pInstance = new CShadowterrorbeak(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CShadowterrorbeak");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShadowterrorbeak::Free()
{
	__super::Free();
}