#include "Deerclops.h"
#include "GameInstance.h"
#include "House.h"
#include "XML_Manager.h"
#include "Camera.h"
#include "CharacterManager.h"
#include "DropItemComponent.h"
#include "IceSpike.h"
#include "MonsterData_Manager.h"

CDeerclops::CDeerclops(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{

}

CDeerclops::CDeerclops(const CDeerclops& Prototype)
	: CMonster{ Prototype }
{
}

HRESULT CDeerclops::Initialize_Prototype()
{
	CXML_Manager::GetInstance()->AddTexture("../Bin/Resources/Textures/Monster/Deerclops/deerclops.scml", L"../Bin/Resources/Textures/Monster/Deerclops/", &m_tImageVec);
	CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/Deerclops/deerclops.scml", &m_tAnimation);
	CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/Deerclops/deerclops_action.scml", &m_tAnimation);
	return S_OK;
}

HRESULT CDeerclops::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	LoadImageFile();
	SetAnimation(m_tDir, MOTION::IDLE);

	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& _Dir) { BeginHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& _Dir) { OverlapHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& _Dir) { EndHitActor(HitActor, _Dir); });

	m_pDropItem_Com->ADD_ItemData(46, 7);
	m_pDropItem_Com->SetCreateEffect(1);
	m_fIceSpikeTime = 10.f;
	m_iIceSpike = 1;
	m_iPattern = 0;
	return S_OK;
}


void CDeerclops::Priority_Update(_float fTimeDelta)
{
	if (m_tMotion == TAUNT && m_bAttack && 560 <= (int)m_fAniTime) {
		m_pCamera->ShakeCamera(-1.f);
		m_pCamera->ShakeCamera(1.5f);
		switch (m_iPattern)
		{
		case 2:
			m_fIceSpikeTime = 0.f;
			m_iPattern++;
			break;
		case 5:
			m_fIceFallTime = 5.f;
			m_iIceFall = 5;
			m_iPattern++;
			break;
		}
		m_fIceSpike = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
		_float volume = Get_Sound();
		if (0.f < volume)
			m_pGameInstance->Manager_PlaySound(L"Deerclops_taunt.wav", CHANNELID::BADMONSTER_SOUND, volume);
		m_bAttack = false;
	}
	if (m_tMotion == ATTACK && m_bAttack && 1200 <= (int)m_fAniTime) {
		_float volume = Get_Sound();
		if (0.f < volume)
			m_pGameInstance->Manager_PlaySound(L"Deerclops_Ice.wav", CHANNELID::BADMONSTER_SOUND, volume);
		m_bAttack = false;
		m_pCamera->ShakeCamera(1.5f);
		m_fIceSpikeTime = 0.f;
		m_iPattern++;
	}
	__super::Priority_Update(fTimeDelta);
	ResetTarget(6.f);
	if (MOTION::ATTACK != m_tMotion)
		m_bDir = true;
	switch (m_iPattern)
	{
	case 1:
	case 2:
	case 4:
	case 5:
		if (5.f > m_fIceSpikeTime) {
			m_fIceSpikeTime += fTimeDelta * 5;
			if (m_iIceSpike <= m_fIceSpikeTime) {
				MONSTER_DESC data = CMonsterData_Manager::GetInstance()->Get_MonsterData(113);
				CIceSpike::ICESPIKE_DESC iceSpike;
				iceSpike.pAttacker = this;
				iceSpike.fAngle = m_fAngle;
				iceSpike.iType = m_iIceSpike;
				switch (m_iIceSpike)
				{
				case 1:
					data.fPos = m_pMonsterData->fPos + (m_fIceSpike * m_iIceSpike * 0.5f);
					iceSpike.tDesc = data;
					m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), data.strPath.c_str(), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &iceSpike);
					break;
				case 2:
					data.fPos = m_pMonsterData->fPos + ((m_fIceSpike - m_fIceSpikeRight * 0.15f) * m_iIceSpike * 0.5f);
					iceSpike.tDesc = data;
					m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), data.strPath.c_str(), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &iceSpike);
					data.fPos = m_pMonsterData->fPos + ((m_fIceSpike + m_fIceSpikeRight * 0.15f) * m_iIceSpike * 0.5f);
					iceSpike.tDesc = data;
					m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), data.strPath.c_str(), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &iceSpike);
					break;
				case 3:
					data.fPos = m_pMonsterData->fPos + ((m_fIceSpike - m_fIceSpikeRight * 0.3f) * m_iIceSpike * 0.5f);
					iceSpike.tDesc = data;
					m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), data.strPath.c_str(), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &iceSpike);
					data.fPos = m_pMonsterData->fPos + (m_fIceSpike * m_iIceSpike * 0.5f);
					iceSpike.tDesc = data;
					m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), data.strPath.c_str(), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &iceSpike);
					data.fPos = m_pMonsterData->fPos + ((m_fIceSpike + m_fIceSpikeRight * 0.3f) * m_iIceSpike * 0.5f);
					iceSpike.tDesc = data;
					m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), data.strPath.c_str(), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &iceSpike);
					break;
				case 4:
					data.fPos = m_pMonsterData->fPos + ((m_fIceSpike - m_fIceSpikeRight * 0.15f) * m_iIceSpike * 0.5f);
					iceSpike.tDesc = data;
					m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), data.strPath.c_str(), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &iceSpike);
					data.fPos = m_pMonsterData->fPos + ((m_fIceSpike + m_fIceSpikeRight * 0.15f) * m_iIceSpike * 0.5f);
					iceSpike.tDesc = data;
					m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), data.strPath.c_str(), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &iceSpike);
					data.fPos = m_pMonsterData->fPos + ((m_fIceSpike - m_fIceSpikeRight * 0.3f) * m_iIceSpike * 0.5f);
					iceSpike.tDesc = data;
					m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), data.strPath.c_str(), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &iceSpike);
					data.fPos = m_pMonsterData->fPos + ((m_fIceSpike + m_fIceSpikeRight * 0.3f) * m_iIceSpike * 0.5f);
					iceSpike.tDesc = data;
					m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), data.strPath.c_str(), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &iceSpike);
					break;
				default:
					break;
				}
				m_iIceSpike++;
			}
		}
		else {
			m_iIceSpike = 1;
		}
		break;
	case 3:
		if (30.f > m_fIceSpikeTime) {
			m_fIceSpikeTime += fTimeDelta * 10;
			if (m_iIceSpike <= m_fIceSpikeTime) {
				MONSTER_DESC data = CMonsterData_Manager::GetInstance()->Get_MonsterData(113);
				CIceSpike::ICESPIKE_DESC iceSpike;
				iceSpike.pAttacker = this;
				iceSpike.iType = m_iIceSpike / 8 + 1;
				data.fPos = m_fIceSpike;
				data.fPos.x += cosf(m_fIceSpikeTime / 2) * m_fIceSpikeTime / 5;
				data.fPos.z += sinf(m_fIceSpikeTime / 2) * m_fIceSpikeTime / 5;
				_float3 fDir = data.fPos - m_fIceSpike;
				m_fAngle = D3DXToDegree(acosf(fDir.x / D3DXVec3Length(&fDir)));
				if (0 < fDir.z) {
					iceSpike.fAngle = 360.f - m_fAngle;
				}
				iceSpike.tDesc = data;
				m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), data.strPath.c_str(), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &iceSpike);

				m_iIceSpike = (_int)m_fIceSpikeTime + 1;
			}
		}
		else {
			m_iIceSpike = 1;
		}
		break;
	case 6:
		m_iPattern = 0;
		break;
	}
}

void CDeerclops::Update(_float fTimeDelta)
{
		__super::Update(fTimeDelta);
		switch (m_tMotion)
		{
		case MOTION::IDLE:
		case MOTION::IDLE_TO_RUN:
		case MOTION::RUN:
		case MOTION::RUN_TO_IDLE:
		case MOTION::ATTACK:
		case MOTION::DAMAGE:
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
				return;
			}
		}
		else if (m_pNearTarget) {
			if (m_pNearTarget->isDead())
				return;

			if (0 < m_fIceFallTime) {
				m_fIceFallTime -= fTimeDelta / 2;
				if (m_iIceFall >= m_fIceFallTime) {
					m_iIceFall--;
					GAMEOBJECT_DESC desc;
					desc.vPosition = m_pNearTarget->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION);
					desc.vRotation = { 0.f, 0.f, 0.f };
					desc.vScale = { 1.f, 1.f, 1.f };
					m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_GameObject_IceFall"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &desc);
				}
			}
			if (MOTION::IDLE == m_tMotion && (2 == m_iPattern || 5 == m_iPattern)) {
				SetAnimation(DIR::DIR_END, MOTION::TAUNT);
				m_bTarget = true;
				m_bAttack = true;
			}
			_float3 move = m_pNearTarget->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pMonsterData->fPos;
			if (D3DXVec3Length(&move) < 5.5f) {
				if (m_tMotion != MOTION::RUN && m_tMotion != MOTION::IDLE_TO_RUN) {
					switch (m_tMotion)
					{
					case MOTION::TAUNT:
						if (m_iLength <= m_fAniTime) {
							SetAnimation(m_tDir, MOTION::IDLE_TO_RUN);
						}
						break;
					case MOTION::DAMAGE:
						if (m_iLength <= m_fAniTime) {
							SetAnimation(m_tDir, MOTION::IDLE);
						}
						break;
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
								SetAnimation(DIR::DIR_END, MOTION::TAUNT);
								m_bTarget = true;
								m_bAttack = true;
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
				case MOTION::IDLE_TO_RUN:
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
				case MOTION::DAMAGE:
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
			case MOTION::IDLE_TO_RUN:
			case MOTION::RUN:
				if (m_iLength <= m_fAniTime) {
					SetAnimation(m_tDir, MOTION::RUN_TO_IDLE);
				}
				break;
			case MOTION::ATTACK:
			case MOTION::RUN_TO_IDLE:
			case MOTION::TAUNT:
			case MOTION::DAMAGE:
				if (m_iLength <= m_fAniTime) {
					SetAnimation(m_tDir, MOTION::IDLE);
				}
				break;
			default:
				break;
			}
		}
}

void CDeerclops::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	if (!m_isDead && m_pCamera->IsInObject(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION), 10))
	{
		SetDir();
		m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);
		m_pCharacterManager->AddObject(this);
	}

}

HRESULT CDeerclops::Render()
{
	__super::Render();
	if (!m_isDead) {
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		RenderAnimation(m_sAnim, m_tAnimation, m_tImageVec);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	}
	return S_OK;
}

void CDeerclops::Hit()
{
	SetAnimation(m_tDir, MOTION::DAMAGE);
	_float volume = Get_Sound();
	if (0.f < volume)
		m_pGameInstance->Manager_PlaySound(L"Deerclops_hurt.wav", CHANNELID::BADMONSTER_SOUND, volume);
}

void CDeerclops::Attack()
{
	m_bDir = false;
	m_bAttack = true;
	SetAnimation(m_tDir, MOTION::ATTACK);
	_float volume = Get_Sound();
	if (0.f < volume)
		m_pGameInstance->Manager_PlaySound(L"Deerclops_attack.wav", CHANNELID::BADMONSTER_SOUND, volume);
}

void CDeerclops::Death()
{
	__super::Death();
	SetAnimation(DIR::DIR_END, MOTION::DEATH);
	_float volume = Get_Sound();
	if (0.f < volume)
		m_pGameInstance->Manager_PlaySound(L"Deerclops_death.wav", CHANNELID::BADMONSTER_SOUND, volume);
}

void CDeerclops::GetTarget(CGameObject* actor, _float distance)
{
	if (6.f > distance && m_fNearDistance / 2 > distance) {
		if (dynamic_cast<CCharacter*>(actor)) {
			if ((200 <= dynamic_cast<CCharacter*>(actor)->Get_Char()->iId) ||
				(dynamic_cast<CMonster*>(actor) && dynamic_cast<CMonster*>(actor)->Get_Active() && 2 != dynamic_cast<CMonster*>(actor)->Get_Monster()->iHostile && !dynamic_cast<CHouse*>(actor))) {
				m_pNearTarget = actor;
				m_fNearDistance = distance;
			}
		}
	}
}

HRESULT CDeerclops::SetAnimation(DIR dir, MOTION motion)
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
	case MOTION::IDLE_TO_SLEEP:
		m_sAnim = L"sleep_pre";
		break;
	case MOTION::SLEEP:
		m_sAnim = L"sleep_loop";
		break;
	case MOTION::SLEEP_TO_IDLE:
		m_sAnim = L"sleep_pst";
		break;
	case MOTION::DAMAGE:
		m_sAnim = L"hit";
		break;
	case MOTION::TAUNT:
		m_sAnim = L"taunt";
		break;
	case MOTION::DEATH:
		m_sAnim = L"death";
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

void CDeerclops::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CDeerclops::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
	__super::OverlapHitActor(HitActor, _Dir);
	if (HitActor == m_pNearTarget && m_tMotion != DAMAGE && m_tMotion != DEATH) {
		_float3 transform = HitActor->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
		_float distance = D3DXVec3Length(&transform);
		if ((m_pMonsterData->iAtkDistance / 2.f) >= distance || (dynamic_cast<CMonster*>(HitActor) && (m_pMonsterData->iAtkDistance / 2.f) >= distance - (dynamic_cast<CMonster*>(HitActor)->Get_Monster()->iAtkDistance / 2.f))) {
			m_bCol = true;
			if (dynamic_cast<CCharacter*>(HitActor) && m_tMotion != ATTACK && m_pMonsterData->iAtkSpeed <= m_fAttackTime) {
				Attack();
				_float3 LookVec;
				_float3	vUp{ 0.f, 1.f, 0.f };
				switch (m_iPattern)
				{
				case 0:
				case 1:
				case 3:
				case 4:
					D3DXVec3Normalize(&m_fIceSpike, &transform);
					LookVec = m_fIceSpike;
					D3DXVec3Normalize(&LookVec, &LookVec);
					D3DXVec3Cross(&m_fIceSpikeRight, &vUp, &LookVec);
					D3DXVec3Normalize(&m_fIceSpikeRight, &m_fIceSpikeRight);
					break;
				case 5:
					break;
				default:
					m_iPattern = 0;
					break;
				}
			}
		}
	}
	if (m_tMotion == ATTACK && m_bAttack && 1200 <= (int)m_fAniTime) {
		HitActor->Damage(&m_tDamage);
	}
}

void CDeerclops::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

CDeerclops* CDeerclops::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDeerclops* pInstance = new CDeerclops(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDeerclops::Clone(void* pArg)
{
	CDeerclops* pInstance = new CDeerclops(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDeerclops");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDeerclops::Free()
{
	__super::Free();
}
