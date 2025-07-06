#include "SpiderWarrior.h"
#include "SpiderHouse.h"
#include "SpiderQueen.h"
#include "GameInstance.h"
#include "XML_Manager.h"
#include "Camera.h"

CSpiderWarrior::CSpiderWarrior(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CSpider{ pGraphic_Device }
{

}

CSpiderWarrior::CSpiderWarrior(const CSpiderWarrior& Prototype)
	: CSpider{ Prototype }
{
}

HRESULT CSpiderWarrior::Initialize_Prototype()
{
	CXML_Manager::GetInstance()->AddTexture("../Bin/Resources/Textures/Monster/SpiderWarrior/spider.scml", L"../Bin/Resources/Textures/Monster/SpiderWarrior/", &m_tImageVec);
	CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/SpiderWarrior/spider.scml", &m_tAnimation);
	CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/SpiderWarrior/spiderwarrior.scml", &m_tAnimation);
	return S_OK;
}

HRESULT CSpiderWarrior::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	LoadImageFile();

	_float3 pos = m_pMonsterData->fPos;;
	m_pTransformCom->SetPosition(pos + _float3(((rand() % 10) / 20.f) - ((rand() % 10) / 20.f), 0.f, ((rand() % 10) / 20.f) - ((rand() % 10) / 20.f)));


	m_pCollision_Com->SetCollisionSize({ 0.2f, 0.f ,0.f });

	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& _Dir) { BeginHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& _Dir) { OverlapHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& _Dir) { EndHitActor(HitActor, _Dir); });

	return S_OK;
}


void CSpiderWarrior::Priority_Update(_float fTimeDelta)
{
	if (m_bOutHouse) {
		__super::Priority_Update(fTimeDelta);
		m_pTarget = nullptr;
		for (auto target : m_pCharacterInstance->Get_NearObject(this, 3.f, FIELDOBJECT::CREATURE)) {
			if (!dynamic_cast<CSpider*>(target) && !dynamic_cast<CSpiderHouse*>(target) && !dynamic_cast<CSpiderQueen*>(target)) {
				m_pTarget = dynamic_cast<CCharacter*>(target);
			}
		}
	}
}

void CSpiderWarrior::Update(_float fTimeDelta)
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
		case MOTION::DASH_ATTACK:
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
		else if (m_tMotion == MOTION::DASH_ATTACK) {
			if (m_iLength <= m_fAniTime) {
				m_tMotion = MOTION::IDLE;
				SetAnimation(m_tDir, m_tMotion);
			}
			else if (267 <= m_fAniTime && 600 >= m_fAniTime) {
				m_pMonsterData->fPos += m_fDash * fTimeDelta;
				m_pTransformCom->SetPosition(m_pMonsterData->fPos);
			}
		}
		else if (m_pTarget) {
			_float3 move = m_pTarget->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pMonsterData->fPos;;
			if ((abs(move.x) + abs(move.z)) / 2.f < 2) {
				m_fAtkCool -= fTimeDelta;
				if (0.f >= m_fAtkCool) {
					if (MOTION::TAUNT == m_tMotion && m_iLength <= m_fAniTime) {
						SetAnimation(m_tDir, MOTION::DASH_ATTACK);
						m_fAtkCool = 5.f;
						D3DXVec3Normalize(&move, &move);
						m_fDash = move * 5;
					}
					else {
						SetAnimation(m_tDir, MOTION::TAUNT);
					}
				}
				else if (m_tMotion != MOTION::RUN && m_tMotion != MOTION::IDLE_TO_RUN) {
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

void CSpiderWarrior::Late_Update(_float fTimeDelta)
{
	if (m_bOutHouse) {
		
		__super::Late_Update(fTimeDelta);
		if (m_pCamera->IsInObject(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION)))
		{
			SetDir();
			m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
		}
	}
		
}

HRESULT CSpiderWarrior::Render()
{
	if (m_bOutHouse) {
		__super::Render();
		if (FAILED(Begin_RenderState()))
			return E_FAIL;

		RenderAnimation(m_sAnim, m_tAnimation, m_tImageVec);

		if (FAILED(End_RenderState()))
			return E_FAIL;
	}

	return S_OK;
}

void CSpiderWarrior::Damage(void* pArg)
{
	__super::Damage(pArg);
	SetAnimation(m_tDir, MOTION::DAMAGE);
}

void CSpiderWarrior::Attack()
{
	m_bAttack = true;
	SetAnimation(m_tDir, MOTION::ATTACK);
}

void CSpiderWarrior::Death()
{
	SetAnimation(DIR::DIR_END, MOTION::DEATH);
}

void CSpiderWarrior::OutHouse()
{
	m_bOutHouse = true;
}

HRESULT CSpiderWarrior::SetAnimation(DIR dir, MOTION motion)
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
	case MOTION::DASH_ATTACK:
		m_sAnim = L"warrior_atk";
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

HRESULT CSpiderWarrior::Begin_RenderState()
{
	/* 알파 테스트 : 픽셀의 알파를 비교해서 그린다 안그린다를 설정. */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CSpiderWarrior::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CSpiderWarrior::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
	if (dynamic_cast<CCharacter*>(HitActor)) {
		if (!dynamic_cast<CMonster*>(HitActor) && m_tMotion == DASH_ATTACK) {
			m_pTarget->Get_Damage(m_pMonsterData->iAtk);
		}
	}
}

void CSpiderWarrior::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
	if (HitActor == m_pTarget) {
		if (!dynamic_cast<CMonster*>(HitActor) && m_tMotion != DASH_ATTACK && m_tMotion != DAMAGE && m_tMotion != DEATH) {
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

void CSpiderWarrior::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

CSpiderWarrior* CSpiderWarrior::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSpiderWarrior* pInstance = new CSpiderWarrior(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpiderWarrior::Clone(void* pArg)
{
	CSpiderWarrior* pInstance = new CSpiderWarrior(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSpiderWarrior");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpiderWarrior::Free()
{
	__super::Free();
}
