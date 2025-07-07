#include "Pig.h"
#include "GameInstance.h"
#include "SpiderHouse.h"
#include "XML_Manager.h"
#include "SpiderQueen.h"
#include "Camera.h"

CPig::CPig(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{

}

CPig::CPig(const CPig& Prototype)
	: CMonster{ Prototype }
{
}

HRESULT CPig::Initialize_Prototype()
{
	CXML_Manager::GetInstance()->AddTexture("../Bin/Resources/Textures/Monster/Pig/pig_basic.scml", L"../Bin/Resources/Textures/Monster/Pig/", &m_tImageVec);
	CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/Pig/pig_basic.scml", &m_tAnimation);
	CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/Pig/pig_action.scml", &m_tAnimation);
	CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/Pig/pig_attack.scml", &m_tAnimation);
	return S_OK;
}

HRESULT CPig::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	LoadImageFile();

	SetAnimation(m_tDir, MOTION::IDLE);
	m_bMove = false;
	m_fMoveTIme = 0.f;


	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& _Dir) { BeginHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& _Dir) { OverlapHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& _Dir) { EndHitActor(HitActor, _Dir); });

	return S_OK;
}


void CPig::Priority_Update(_float fTimeDelta)
{
		__super::Priority_Update(fTimeDelta);
		m_pTarget = nullptr;

		list<CGameObject*> NearObjects;
		if (m_pMonsterData->bHostile) {
			NearObjects.push_back(m_pGameInstance->GetAllObejctsToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player"))->front());
		}
		auto GroundObejcts = m_pGameInstance->GetAllObejctsToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Monster"));
		if (!GroundObejcts->empty()) {
			for (auto& object : (*GroundObejcts)) {
				_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
				_float distance = sqrtf(pow(transform.x, 2) + pow(transform.z, 2));
				if (!dynamic_cast<CPig*>(object))
					if (5.f > distance) {
						NearObjects.push_back(object);
					}
			}
		}
		NearObjects.sort([this](CGameObject* pSour, CGameObject* pDest)->_bool
			{
				_float3 transform = pSour->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - this->m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
				_float3 transform2 = pDest->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - this->m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
				_float distance = sqrtf(pow(transform.x, 2) + pow(transform.z, 2));
				_float distance2 = sqrtf(pow(transform2.x, 2) + pow(transform2.z, 2));
				return distance < distance2;
			});

		if (!NearObjects.empty()) {
			CGameObject* object = NearObjects.front();
			if (object) {
				m_pTarget = object;
			}
		}
}

void CPig::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);




	__super::Update(fTimeDelta);
	switch (m_tMotion)
	{
	case MOTION::IDLE:
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
				case MOTION::DAMAGE:
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
						SetAnimation(m_tDir, MOTION::IDLE_TO_RUN);
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
			//m_fMoveTIme += fTimeDelta;
			//if (m_fMoveTIme > 10) {
			//	if (m_fMoveTIme > 40) {
			//		m_pMonsterData->fPos += m_fRandomMove * m_pMonsterData->fSpeed * fTimeDelta;
			//		m_pTransformCom->SetPosition(m_pMonsterData->fPos);
			//		if (MOTION::IDLE_TO_RUN == m_tMotion && m_iLength <= m_fAniTime) {
			//			SetAnimation(m_tDir, MOTION::RUN);
			//		}
			//		if (m_fMoveTIme > 70) {
			//			m_fMoveTIme = 0;
			//		}
			//	}
			//	else {
			//		m_fRandomMove = { (_float)rand() - rand() , 0.f, (_float)rand() - rand() };
			//		D3DXVec3Normalize(&m_fRandomMove, &m_fRandomMove);
			//		m_fMoveTIme += 30 + (rand() % 20);
			//		SetAnimation(m_tDir, MOTION::IDLE_TO_RUN);
			//	}
			//}
			//else {
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
				case MOTION::DAMAGE:
					if (m_iLength <= m_fAniTime) {
						SetAnimation(m_tDir, MOTION::IDLE);
					}
					break;
				default:
					break;
				}
			//}
		}
	}
	else {
		//m_fMoveTIme += fTimeDelta;
		//if (m_fMoveTIme > 10) {
		//	if (m_fMoveTIme > 40) {
		//
		//		m_pMonsterData->fPos += m_fRandomMove * m_pMonsterData->fSpeed * fTimeDelta;
		//		if (m_pMonsterData->fPos.x < 0 || m_pMonsterData->fPos.z < 0) {
		//			m_fMoveTIme = 0;
		//		}
		//		m_pTransformCom->SetPosition(m_pMonsterData->fPos);
		//		if (MOTION::IDLE_TO_RUN == m_tMotion && m_iLength <= m_fAniTime) {
		//			SetAnimation(m_tDir, MOTION::RUN);
		//		}
		//		if (m_fMoveTIme > 70) {
		//			m_fMoveTIme = 0;
		//		}
		//	}
		//	else {
		//		m_fRandomMove = { (_float)rand() - rand() , 0.f, (_float)rand() - rand() };
		//		D3DXVec3Normalize(&m_fRandomMove, &m_fRandomMove);
		//		m_fMoveTIme += 30 + (rand() % 20);
		//		SetAnimation(m_tDir, MOTION::IDLE_TO_RUN);
		//	}
		//}
		//else {
		switch (m_tMotion)
		{
		case MOTION::RUN:
			if (m_iLength <= m_fAniTime) {
				SetAnimation(m_tDir, MOTION::RUN_TO_IDLE);
			}
			break;
		case MOTION::ATTACK:
		case MOTION::RUN_TO_IDLE:
		case MOTION::DAMAGE:
			if (m_iLength <= m_fAniTime) {
				SetAnimation(m_tDir, MOTION::IDLE);
			}
			break;
		default:
			break;
		}
		//}
	}
}

void CPig::Late_Update(_float fTimeDelta)
{
		SetDir();
		__super::Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CPig::Render()
{
		__super::Render();

		if (FAILED(Begin_RenderState()))
			return E_FAIL;
		RenderAnimation(m_sAnim, m_tAnimation, m_tImageVec);

		if (FAILED(End_RenderState()))
			return E_FAIL;
	return S_OK;
}

void CPig::Hit()
{
	SetAnimation(DIR::DIR_END, MOTION::DAMAGE);
}

void CPig::Attack()
{
	m_bAttack = true;
	SetAnimation(m_tDir, MOTION::ATTACK);
}

void CPig::Death()
{
	SetAnimation(DIR::DIR_END, MOTION::DEATH);
}
HRESULT CPig::SetAnimation(DIR dir, MOTION motion)
{
	if (DIR::DIR_END == dir) {
		m_tDir = DIR::DOWN;
	}
	if (motion != m_tMotion) {
		m_fAniTime = 0.f;
	}
	m_tMotion = motion;
	switch (motion)
	{
	case MOTION::IDLE:
		m_sAnim = L"idle_loop";
		break;
	case MOTION::IDLE_TO_WALK:
		m_sAnim = L"walk_pre";
		break;
	case MOTION::WALK:
		m_sAnim = L"walk_loop";
		break;
	case MOTION::WALK_TO_IDLE:
		m_sAnim = L"walk_pst";
		break;
	case MOTION::IDLE_TO_RUN:
		m_sAnim = L"run_pre";
		break;
	case MOTION::RUN:
		m_sAnim = L"run_loop";
		break;
	case MOTION::RUN_TO_IDLE:
		m_sAnim = L"run_pst";
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
	case MOTION::EAT:
		m_sAnim = L"eat";
		break;
	case MOTION::DAMAGE:
		m_sAnim = L"hit";
		break;
	case MOTION::ABANDON:
		m_sAnim = L"abandon";
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

HRESULT CPig::Begin_RenderState()
{
	/* 알파 테스트 : 픽셀의 알파를 비교해서 그린다 안그린다를 설정. */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CPig::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CPig::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CPig::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
	if (HitActor == m_pTarget) {
		m_bCol = true;
		if (dynamic_cast<CCharacter*>(HitActor) && m_pMonsterData->iAtkSpeed <= m_fAttackTime && m_tMotion != DAMAGE && m_tMotion != DEATH) {
			if (m_tMotion != ATTACK) {
				Attack();
			}
			else if (m_tMotion == ATTACK && m_bAttack && 470 <= (int)m_fAniTime) {
				m_pTarget->Damage(&m_tDamage);
				m_bAttack = false;
			}
		}
	}
}

void CPig::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

CPig* CPig::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPig* pInstance = new CPig(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPig::Clone(void* pArg)
{
	CPig* pInstance = new CPig(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPig");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPig::Free()
{
	__super::Free();
}
