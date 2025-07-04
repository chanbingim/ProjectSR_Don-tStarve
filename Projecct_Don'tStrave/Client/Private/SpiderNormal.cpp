#include "SpiderNormal.h"
#include "GameInstance.h"
#include "SpiderHouse.h"
#include "SpiderQueen.h"
#include "Camera.h"

CSpiderNormal::CSpiderNormal(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CSpider{ pGraphic_Device }
{

}

CSpiderNormal::CSpiderNormal(const CSpiderNormal& Prototype)
	: CSpider{ Prototype }
{
}

HRESULT CSpiderNormal::Initialize_Prototype()
{
	AddTexture("../Bin/Resources/Textures/Monster/Spider/spider.scml", L"../Bin/Resources/Textures/Monster/Spider/");
	LoadScml("../Bin/Resources/Textures/Monster/Spider/spider.scml");
	return S_OK;
}

HRESULT CSpiderNormal::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	LoadImageFile();
	_float3 pos = m_pMonsterData->fPos;
	m_pTransformCom->SetPosition(pos + _float3(((rand() % 10) / 20.f) - ((rand() % 10) / 20.f), 0.f, ((rand() % 10) / 20.f) - ((rand() % 10) / 20.f)));


	SetAnimation(m_tDir, MOTION::IDLE);
	m_bMove = false;
	m_fMoveTIme = 0.f;

	m_pCollision_Com->SetCollisionSize({ 0.2f, 0.f ,0.f });

	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& _Dir) { BeginHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& _Dir) { OverlapHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& _Dir) { EndHitActor(HitActor, _Dir); });

	return S_OK;
}


void CSpiderNormal::Priority_Update(_float fTimeDelta)
{
	if (m_bOutHouse) {
		__super::Priority_Update(fTimeDelta);
		m_pTarget = nullptr;
		for (auto target : m_pCharacterInstance->Get_NearObject(this, 3.f)) {
			if (!dynamic_cast<CSpider*>(target) && !dynamic_cast<CSpiderHouse*>(target) && !dynamic_cast<CSpiderQueen*>(target)) {
				m_pTarget = dynamic_cast<CCharacter*>(target);
			}
		}
	}
}

void CSpiderNormal::Update(_float fTimeDelta)
{
	if (m_bOutHouse) {
		__super::Update(fTimeDelta);
		switch (m_tMotion)
		{
		case IDLE:
			switch (m_tMoveDIr)
			{
			case MOVE_DIR::MOVE_DOWN:
				m_tDir = DIR::DOWN;
				break;
			case MOVE_DIR::MOVE_UP:
				m_tDir = DIR::UP;
				break;
			default:
				break;
			}
			SetAnimation(m_tDir, m_tMotion);
			break;
		case MOTION::IDLE_TO_RUN:
		case MOTION::RUN:
		case MOTION::RUN_TO_IDLE:
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
		}
		else if (m_pTarget) {
			_float3 move = m_pTarget->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pMonsterData->fPos;;
			if ((abs(move.x) + abs(move.z)) / 2.f < 2) {
				if (m_tMotion != MOTION::RUN && m_tMotion != MOTION::IDLE_TO_RUN) {
					switch (m_tMotion)
					{
					case MOTION::TAUNT:
						if (m_iLength <= m_fAniTime) {
							SetAnimation(m_tDir, MOTION::IDLE_TO_RUN);
						}
						break;
					case MOTION::DAMAGE:
					case MOTION::ATTACK:
						if (m_iLength <= m_fAniTime) {
							SetAnimation(m_tDir, MOTION::IDLE);
						}
						break;
					default:
						SetAnimation(m_tDir, MOTION::TAUNT);
						m_sAnim;
						break;
					}
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

				m_fMoveTIme += fTimeDelta;
				if (m_fMoveTIme > 10) {
					if (m_fMoveTIme > 40) {
						m_pMonsterData->fPos += m_fRandomMove * m_pMonsterData->fSpeed * fTimeDelta;
						m_pTransformCom->SetPosition(m_pMonsterData->fPos);
						if (MOTION::IDLE_TO_RUN == m_tMotion && m_iLength <= m_fAniTime) {
							SetAnimation(m_tDir, MOTION::RUN);
						}
						if (m_fMoveTIme > 70) {
							m_fMoveTIme = 0;
						}
					}
					else {
						m_fRandomMove = { (_float)rand() - rand() , 0.f, (_float)rand() - rand() };
						D3DXVec3Normalize(&m_fRandomMove, &m_fRandomMove);
						m_fMoveTIme += 30 + (rand() % 20);
						SetAnimation(m_tDir, MOTION::IDLE_TO_RUN);
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
		}
		else {
			m_fMoveTIme += fTimeDelta;
			if (m_fMoveTIme > 10) {
				if (m_fMoveTIme > 40) {

					m_pMonsterData->fPos += m_fRandomMove * m_pMonsterData->fSpeed * fTimeDelta;
					if (m_pMonsterData->fPos.x < 0 || m_pMonsterData->fPos.z < 0) {
						m_fMoveTIme = 0;
					}
					m_pTransformCom->SetPosition(m_pMonsterData->fPos);
					if (MOTION::IDLE_TO_RUN == m_tMotion && m_iLength <= m_fAniTime) {
						SetAnimation(m_tDir, MOTION::RUN);
					}
					if (m_fMoveTIme > 70) {
						m_fMoveTIme = 0;
					}
				}
				else {
					m_fRandomMove = { (_float)rand() - rand() , 0.f, (_float)rand() - rand() };
					D3DXVec3Normalize(&m_fRandomMove, &m_fRandomMove);
					m_fMoveTIme += 30 + (rand() % 20);
					SetAnimation(m_tDir, MOTION::IDLE_TO_RUN);
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
	}
}

void CSpiderNormal::Late_Update(_float fTimeDelta)
{
	if (m_bOutHouse) {
		SetDir();
		__super::Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
	}

}

HRESULT CSpiderNormal::Render()
{
	if (m_bOutHouse) {
		__super::Render();

		if (FAILED(Begin_RenderState()))
			return E_FAIL;

		RenderAnimation(m_sAnim);

		if (FAILED(End_RenderState()))
			return E_FAIL;
	}
	return S_OK;
}

void CSpiderNormal::Damage()
{
	__super::Damage();
	SetAnimation(m_tDir, MOTION::DAMAGE);
}

void CSpiderNormal::Attack()
{
	m_bAttack = true;
	SetAnimation(m_tDir, MOTION::ATTACK);
}

void CSpiderNormal::Death()
{
	SetAnimation(DIR::DIR_END, MOTION::DEATH);
}

void CSpiderNormal::OutHouse()
{
	m_bOutHouse = true;
}

HRESULT CSpiderNormal::SetAnimation(DIR dir, MOTION motion)
{
	if (DIR::DIR_END == dir || ((MOTION::IDLE == motion || MOTION::DAMAGE == motion || MOTION::TAUNT == motion) && DIR::SIDE == dir)) {
		m_tDir = DIR::DOWN;
	}
	if (motion != m_tMotion) {
		m_fAniTime = 0.f;
	}
	m_tMotion = motion;
	switch (motion)
	{
	case MOTION::IDLE:
		m_sAnim = L"idle";
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
	case MOTION::IDLE_TO_EAT:
		m_sAnim = L"eat";
		break;
	case MOTION::EAT:
		m_sAnim = L"eat_loop";
		break;
	case MOTION::EAT_TO_IDLE:
		m_sAnim = L"eat_pst";
		break;
	case MOTION::DAMAGE:
		m_sAnim = L"hit";
		break;
	case MOTION::IDLE_TO_COWER:
		m_sAnim = L"cower_pre";
		break;
	case MOTION::COWER:
		m_sAnim = L"cower_loop";
		break;
	case MOTION::COWER_TO_IDLE:
		m_sAnim = L"cower_pst";
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

HRESULT CSpiderNormal::Begin_RenderState()
{
	/* 렌더링할 때 알파값을 기준으로 섞어준다.*/

	/*
	float4		vSourColor, vDestColor;
	vSourColor.rgb * vSourColor.a + vDestColor.rgb * (1.f - vSourColor.a);
	*/

	//
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//

	/* 알파 테스트 : 픽셀의 알파를 비교해서 그린다 안그린다를 설정. */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CSpiderNormal::End_RenderState()
{
	// m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pVIBufferCom->SetUV(1, 1, 1, 0, 1, false);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CSpiderNormal::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CSpiderNormal::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
	if (HitActor == m_pTarget) {
		if (!dynamic_cast<CMonster*>(HitActor) && m_tMotion != DAMAGE && m_tMotion != DEATH) {
			if (m_tMotion != ATTACK) {
				Attack();
			}
			else if (m_tMotion == ATTACK && m_bAttack && 840 <= (int)m_fAniTime) {
				m_pTarget->Get_Damage(m_pMonsterData->iAtk);
				m_bAttack = false;
			}
		}
	}
}

void CSpiderNormal::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

CSpiderNormal* CSpiderNormal::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSpiderNormal* pInstance = new CSpiderNormal(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpiderNormal::Clone(void* pArg)
{
	CSpiderNormal* pInstance = new CSpiderNormal(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSpiderNormal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpiderNormal::Free()
{
	__super::Free();
	Safe_Release(m_pCollision_Com);
}
