#include "Goat.h"
#include "GameInstance.h"
#include "SpiderHouse.h"
#include "XML_Manager.h"
#include "SpiderQueen.h"
#include "Camera.h"

CGoat::CGoat(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{

}

CGoat::CGoat(const CGoat& Prototype)
	: CMonster{ Prototype }
{
}

HRESULT CGoat::Initialize_Prototype()
{
	CXML_Manager::GetInstance()->AddTexture("../Bin/Resources/Textures/Monster/Goat/goat.scml", L"../Bin/Resources/Textures/Monster/Goat/", &m_tImageVec);
	CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/Goat/goat.scml", &m_tAnimation);
	CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/Goat/goat_action.scml", &m_tAnimation);
	return S_OK;
}

HRESULT CGoat::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	LoadImageFile();
	SetAnimation(m_tDir, MOTION::IDLE);

	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& _Dir) { BeginHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& _Dir) { OverlapHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& _Dir) { EndHitActor(HitActor, _Dir); });

	return S_OK;
}


void CGoat::Priority_Update(_float fTimeDelta)
{
	if (!m_bActive) {
		return;
	}
	__super::Priority_Update(fTimeDelta);
	m_pTarget = nullptr;
	list<CGameObject*> NearObjects;
	if (m_pMonsterData->iHostile) {
		NearObjects.push_back(m_pGameInstance->GetAllObejctsToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player"))->front());
	}
	auto GroundObejcts = m_pGameInstance->GetAllObejctsToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Monster"));
	if (!GroundObejcts->empty()) {
		for (auto& object : (*GroundObejcts)) {
			_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
			_float distance = sqrtf(powf(transform.x, 2) + powf(transform.z, 2));
			if (!dynamic_cast<CGoat*>(object) && 1 == dynamic_cast<CMonster*>(object)->Get_Monster()->iHostile)
				if (5.f > distance) {
					NearObjects.push_back(object);
				}
		}
	}
	NearObjects.sort([this](CGameObject* pSour, CGameObject* pDest)->_bool
		{
			_float3 transform = pSour->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - this->m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
			_float3 transform2 = pDest->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - this->m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
			_float distance = sqrtf(powf(transform.x, 2) + powf(transform.z, 2));
			_float distance2 = sqrtf(powf(transform2.x, 2) + powf(transform2.z, 2));
			return distance < distance2;
		});

	if (!NearObjects.empty()) {
		CGameObject* object = NearObjects.front();
		if (object) {
			m_pTarget = object;
		}
	}
}

void CGoat::Update(_float fTimeDelta)
{
	if (!m_bActive) {
		return;
	}
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
			return;
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

void CGoat::Late_Update(_float fTimeDelta)
{
	if (!m_bActive) {
		return;
	}
	__super::Late_Update(fTimeDelta);
	SetDir();
	m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);
}

HRESULT CGoat::Render()
{
	__super::Render();
	if (m_bActive && !m_isDead) {
		RenderAnimation(m_sAnim, m_tAnimation, m_tImageVec);
	}
	return S_OK;
}

void CGoat::Hit()
{
	SetAnimation(DIR::DIR_END, MOTION::DAMAGE);
}

void CGoat::Attack()
{
	m_bAttack = true;
	SetAnimation(m_tDir, MOTION::ATTACK);
}

void CGoat::Death()
{
	SetAnimation(DIR::DIR_END, MOTION::DEATH);
}
void CGoat::OutHouse()
{
	m_bActive = true;
}
HRESULT CGoat::SetAnimation(DIR dir, MOTION motion)
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

void CGoat::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CGoat::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
	if (HitActor == m_pTarget && m_tMotion != DAMAGE && m_tMotion != DEATH) {
		_float3 transform = HitActor->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
		_float distance = sqrtf(powf(transform.x, 2) + powf(transform.z, 2));
		if ((m_pMonsterData->iAtkDistance / 2.f) >= distance || (dynamic_cast<CMonster*>(HitActor) && (m_pMonsterData->iAtkDistance / 2.f) >= distance - (dynamic_cast<CMonster*>(HitActor)->Get_Monster()->iAtkDistance / 2.f))) {
			m_bCol = true;
			if (dynamic_cast<CCharacter*>(HitActor) && m_tMotion != ATTACK && m_pMonsterData->iAtkSpeed <= m_fAttackTime) {
				Attack();
			}
		}
		if (m_tMotion == ATTACK && m_bAttack && 400 <= (int)m_fAniTime) {
			m_bAttack = false;
			HitActor->Damage(&m_tDamage);
		}
	}
}

void CGoat::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

CGoat* CGoat::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGoat* pInstance = new CGoat(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGoat::Clone(void* pArg)
{
	CGoat* pInstance = new CGoat(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGoat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGoat::Free()
{
	__super::Free();
}
