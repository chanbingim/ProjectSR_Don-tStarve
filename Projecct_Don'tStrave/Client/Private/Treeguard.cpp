#include "Treeguard.h"
#include "GameInstance.h"
#include "XML_Manager.h"
#include "EffectPoolManager.h"

#include "Camera.h"
#include "House.h"
#include "CharacterManager.h"
#include "DropItemComponent.h"
#include "SpriteEffect.h"
#include "LeafEffect.h"

CTreeguard::CTreeguard(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{

}

CTreeguard::CTreeguard(const CTreeguard& Prototype)
	: CMonster{ Prototype }
{
}

HRESULT CTreeguard::Initialize_Prototype()
{
	CXML_Manager::GetInstance()->AddTexture("../Bin/Resources/Textures/Monster/Treeguard/treeguard_idle.scml", L"../Bin/Resources/Textures/Monster/Treeguard/", &m_tImageVec);
	CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/Treeguard/treeguard_idle.scml", &m_tAnimation);
	CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/Treeguard/treeguard_action.scml", &m_tAnimation);
	CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/Treeguard/treeguard_walk.scml", &m_tAnimation);
	CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/Treeguard/treeguard_attack.scml", &m_tAnimation);
	return S_OK;
}

HRESULT CTreeguard::Initialize(void* pArg)
{
	m_iAttackCnt = 0;
	m_fAttackDelay = 0.f;
	m_iVineCnt = 0;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	LoadImageFile();
	SetAnimation(DIR::DIR_END, MOTION::TRANSFORM);

	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& _Dir) { BeginHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& _Dir) { OverlapHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& _Dir) { EndHitActor(HitActor, _Dir); });

	m_pDropItem_Com->ADD_ItemData(16, 2);
	m_pDropItem_Com->ADD_ItemData(38, 3);
	m_pDropItem_Com->SetCreateEffect(1);

	return S_OK;
}


void CTreeguard::Priority_Update(_float fTimeDelta)
{
	if (m_tMotion == ATTACK && m_bAttack && 850 <= (int)m_fAniTime) {
		m_bAttack = false;
	}
	__super::Priority_Update(fTimeDelta);
	ResetTarget(6.f);
}

void CTreeguard::Update(_float fTimeDelta)
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
		_float3 move = m_pNearTarget->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pMonsterData->fPos;
		if (D3DXVec3Length(&move) < 5.5f) {
			if (m_tMotion != MOTION::RUN && m_tMotion != MOTION::IDLE_TO_RUN) {
				switch (m_tMotion)
				{
				case MOTION::DAMAGE:
					if (m_iLength <= m_fAniTime) {
						SetAnimation(m_tDir, MOTION::IDLE);
					}
					break;
				case MOTION::TRANSFORM:
					if(1400 < m_fAniTime)
						SetAnimation(m_tDir, MOTION::IDLE);
					break;
				case MOTION::ATTACK:
					if (2 == m_iAttackCnt)
					{
						SetAnimation(DIR::DIR_END, MOTION::TRANSFORM_TREE);
						
						break;
					}
					if (3 == m_iAttackCnt)
					{
						SetAnimation(DIR::DIR_END, MOTION::TRANSFORM_MAD);
						break;
					}
					if (m_iLength <= m_fAniTime) {
						m_fAttackTime = 0;
						SetAnimation(m_tDir, MOTION::IDLE);
						m_iAttackCnt++;
					}
					break;
				case MOTION::TRANSFORM_TREE:
					if(1200 < m_fAniTime)
					{
						SetAnimation(DIR::DIR_END, MOTION::TREE);
						m_PrePlayerPos = static_cast<CTransform*>(m_pGameInstance->Get_Component(
							ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform"), 0))->GetWorldState(WORLDSTATE::POSITION);
					}
					break;
				
				case MOTION::TRANSFORM_MAD:
					if(1600 < m_fAniTime)
					{
						Leaf_Attack();
						m_iAttackCnt = 0;
						SetAnimation(m_tDir, MOTION::IDLE);
						m_PrePlayerPos = static_cast<CTransform*>(m_pGameInstance->Get_Component(
							ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform"), 0))->GetWorldState(WORLDSTATE::POSITION);
					}
					break;

				case MOTION::TREE:
					if(!Vine_Attack(fTimeDelta))
					{
						m_fAttackTime = 0;
						m_iAttackCnt++;
						SetAnimation(DIR::DIR_END, MOTION::TRANSFORM);
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
			case MOTION::TRANSFORM:
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
		case MOTION::IDLE_TO_RUN:
		case MOTION::RUN:
			if (m_iLength <= m_fAniTime) {
				SetAnimation(m_tDir, MOTION::RUN_TO_IDLE);
			}
			break;
		case MOTION::TRANSFORM:
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

void CTreeguard::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	if (!m_isDead && m_pCamera->IsInObject(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION)))
	{
		SetDir();
		m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);
		m_pCharacterManager->AddObject(this);
	}

}

HRESULT CTreeguard::Render()
{
	__super::Render();
	if (!m_isDead) {
		RenderAnimation(m_sAnim, m_tAnimation, m_tImageVec);
	}
	return S_OK;
}

void CTreeguard::Hit()
{
	SetAnimation(m_tDir, MOTION::DAMAGE);
}

void CTreeguard::Attack()
{
	m_bAttack = true;
	SetAnimation(m_tDir, MOTION::ATTACK);
}

void CTreeguard::Death()
{
	SetAnimation(DIR::DIR_END, MOTION::DEATH);
}

void CTreeguard::GetTarget(CGameObject* actor, _float distance)
{
	if (6.f > distance && m_fNearDistance / 2 > distance) {
		if (dynamic_cast<CCharacter*>(actor)) {
			if ((200 <= dynamic_cast<CCharacter*>(actor)->Get_Char()->iId) ||
				(!dynamic_cast<CTreeguard*>(actor) && !dynamic_cast<CHouse*>(actor) && dynamic_cast<CMonster*>(actor) && dynamic_cast<CMonster*>(actor)->Get_Active() && 2 != dynamic_cast<CMonster*>(actor)->Get_Monster()->iHostile)) {
				m_pNearTarget = actor;
				m_fNearDistance = distance;
			}
		}
	}
}

HRESULT CTreeguard::SetAnimation(DIR dir, MOTION motion)
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
	case MOTION::TRANSFORM:
		m_sAnim = L"transform_ent";
		break;
	case MOTION::TRANSFORM_MAD:
		
		m_sAnim = L"transform_ent_mad";
		break;
	case MOTION::TRANSFORM_TREE:
		m_sAnim = L"transform_tree";
		break;
	case MOTION::TREE:
		m_sAnim = L"tree";
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
	case MOTION::IDLE_TO_PANIC:
		m_sAnim = L"panic_pre";
		break;
	case MOTION::PANIC:
		m_sAnim = L"panic_loop";
		break;
	case MOTION::PANIC_TO_IDLE:
		m_sAnim = L"panic_post";
		break;
	case MOTION::ATTACK:
		m_sAnim = L"atk";
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

void CTreeguard::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CTreeguard::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
	if (HitActor == m_pNearTarget && m_tMotion != DAMAGE && m_tMotion != DEATH) {
		_float3 transform = HitActor->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
		_float distance = D3DXVec3Length(&transform);
		if ((m_pMonsterData->iAtkDistance / 2.f) >= distance || (dynamic_cast<CMonster*>(HitActor) && (m_pMonsterData->iAtkDistance / 2.f) >= distance - (dynamic_cast<CMonster*>(HitActor)->Get_Monster()->iAtkDistance / 2.f))) {
			m_bCol = true;
			if (dynamic_cast<CCharacter*>(HitActor) && m_tMotion != ATTACK && m_pMonsterData->iAtkSpeed <= m_fAttackTime) {
				Attack();
			}
		}
	}
	if (m_tMotion == ATTACK && m_bAttack && 850 <= (int)m_fAniTime) {
		HitActor->Damage(&m_tDamage);
	}
}

void CTreeguard::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

_bool CTreeguard::Vine_Attack(_float fTimeDelta)
{
	if (m_fAttackDelay > 0.5f)
	{
		CSpriteEffect::GAMEOBJECT_DESC desc;

		auto Effect = CEffectPoolManager::GetInstance()->Add_ActiveEffect(4, (CAinimationObject**)&m_pSpirteEffect, &desc);

		m_pSpirteEffect->ReadyEffect(L"spawn3");
		m_pSpirteEffect->GetTransfrom()->SetPosition(m_PrePlayerPos);

		m_fAttackDelay = 0.f;
		m_iVineCnt++;

		m_PrePlayerPos = static_cast<CTransform*>(m_pGameInstance->Get_Component(
			ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform"), 0))->GetWorldState(WORLDSTATE::POSITION);
	}
	if (5 < m_iVineCnt)
	{
		m_iVineCnt = 0;
		return false;
	}
	m_fAttackDelay += fTimeDelta;


	return true;
}

void CTreeguard::Leaf_Attack()
{
	_float3 vPos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
	_float3 vRight = m_pTransformCom->GetWorldState(WORLDSTATE::RIGHT);
	_float3 vUp = m_pTransformCom->GetWorldState(WORLDSTATE::UP);
	_float3 vLook = m_pTransformCom->GetWorldState(WORLDSTATE::LOOK);
	CLeafEffect::LEAF_DESC Desc = {};
	for (_uint i = 0; i < 30; ++i)
	{
		Desc.iIndex = i;
		Desc.vMovePos = _float3(vPos.x, vPos.y + 0.5f, vPos.z);
		Desc.vPosition = vPos + vRight * m_pGameInstance->Random(-0.5f, 0.5f) + vUp * m_pGameInstance->Random(1.f, 1.5f) + vLook;
		m_pGameInstance->Add_GameObject_ToLayer(
			EnumToInt(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_GameObject_LeafEffect"), EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_LeafEffect"), &Desc);
	}
}

CTreeguard* CTreeguard::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTreeguard* pInstance = new CTreeguard(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTreeguard::Clone(void* pArg)
{
	CTreeguard* pInstance = new CTreeguard(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTreeguard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTreeguard::Free()
{
	__super::Free();
}
