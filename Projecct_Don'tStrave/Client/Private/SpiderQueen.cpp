#include "SpiderQueen.h"
#include "Spider.h"
#include "SpiderHouse.h"
#include "GameInstance.h"
#include "XML_Manager.h"
#include "Camera.h"

CSpiderQueen::CSpiderQueen(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{

}

CSpiderQueen::CSpiderQueen(const CSpiderQueen& Prototype)
	: CMonster{ Prototype }
{
}

HRESULT CSpiderQueen::Initialize_Prototype()
{
	CXML_Manager::GetInstance()->AddTexture("../Bin/Resources/Textures/Monster/SpiderQueen/spider_queen.scml", L"../Bin/Resources/Textures/Monster/SpiderQueen/", &m_tImageVec);
	CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/SpiderQueen/spider_queen.scml", &m_tAnimation);
	CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/SpiderQueen/spider_queen_2.scml", &m_tAnimation);

	//AddTexture("../Bin/Resources/Textures/Monster/SpiderQueen/spider_queen.scml", L"../Bin/Resources/Textures/Monster/SpiderQueen/");
	//LoadScml("../Bin/Resources/Textures/Monster/SpiderQueen/spider_queen.scml");
	//LoadScml("../Bin/Resources/Textures/Monster/SpiderQueen/spider_queen_2.scml");
	return S_OK;
}

HRESULT CSpiderQueen::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	LoadImageFile();

	SetAnimation(m_tDir, MOTION::IDLE);
	m_bMove = false;


	m_pCollision_Com->SetCollisionSize({ 1.f, 0.f ,0.f });

	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& _Dir) { BeginHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& _Dir) { OverlapHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& _Dir) { EndHitActor(HitActor, _Dir); });

	return S_OK;
}


void CSpiderQueen::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	m_pTarget = nullptr;
	for (auto target : m_pCharacterInstance->Get_NearObject(this, 7.f, FIELDOBJECT::CREATURE)) {
		if (!dynamic_cast<CSpider*>(target) && !dynamic_cast<CSpiderHouse*>(target) && !dynamic_cast<CSpiderQueen*>(target)) {
			m_pTarget = dynamic_cast<CCharacter*>(target);
		}
	}
}

void CSpiderQueen::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	switch (m_tMotion)
	{
	case IDLE:
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
		if ((abs(move.x) + abs(move.z)) / 2.f < 5) {
			if (m_tMotion != MOTION::RUN && m_tMotion != MOTION::IDLE_TO_RUN) {
				switch (m_tMotion)
				{
				case MOTION::DAMAGE:
				case MOTION::ATTACK:
					if (m_iLength <= m_fAniTime) {
						SetAnimation(m_tDir, MOTION::IDLE);
					}
					break;
				default:
					SetAnimation(m_tDir, MOTION::IDLE_TO_RUN);
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

void CSpiderQueen::Late_Update(_float fTimeDelta)
{

	SetDir();
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);

}

HRESULT CSpiderQueen::Render()
{
	__super::Render();


	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	RenderAnimation(m_sAnim, m_tAnimation, m_tImageVec);

	if (FAILED(End_RenderState()))
		return E_FAIL;
	//class CGameObject* Obj = m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Camera"));
	//auto Camera = dynamic_cast<CCamera*>(Obj);
	//if (nullptr == Camera)
	//	return E_FAIL;
	///*if (FAILED(Begin_RenderState()))
	//	return E_FAIL;*/
	//LPDIRECT3DBASETEXTURE9 pTex = { nullptr };
	//m_pGraphic_Device->GetTexture(0, &pTex);
	//
	//Excute_Billboard(Camera->GetInvViewMat(), pTex);
	//m_pVIBufferCom->Render();
	//
	//Safe_Release(pTex);
	//End_Billboard();

	/*if (FAILED(End_RenderState()))
		return E_FAIL;*/

	return S_OK;
}

void CSpiderQueen::Damage(void* pArg)
{
	__super::Damage(pArg);
	SetAnimation(m_tDir, MOTION::DAMAGE);
}

void CSpiderQueen::Attack()
{
	m_bAttack = true;
	SetAnimation(m_tDir, MOTION::ATTACK);
}

void CSpiderQueen::Death()
{
	SetAnimation(DIR::DIR_END, MOTION::DEATH);
}

HRESULT CSpiderQueen::SetAnimation(DIR dir, MOTION motion)
{
	if (DIR::DIR_END == dir || (MOTION::IDLE == motion && DIR::SIDE == dir)) {
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


HRESULT CSpiderQueen::Begin_RenderState()
{
	/* 알파 테스트 : 픽셀의 알파를 비교해서 그린다 안그린다를 설정. */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CSpiderQueen::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CSpiderQueen::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CSpiderQueen::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
	if (HitActor == m_pTarget) {
		if (!dynamic_cast<CMonster*>(HitActor) && m_tMotion != DAMAGE && m_tMotion != DEATH) {
			if (m_tMotion != ATTACK) {
				Attack();
			}
			else if (m_tMotion == ATTACK && m_bAttack && 960 <= (int)m_fAniTime) {
				m_pTarget->Get_Damage(m_pMonsterData->iAtk);
				m_bAttack = false;
			}
		}
	}
}

void CSpiderQueen::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

CSpiderQueen* CSpiderQueen::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSpiderQueen* pInstance = new CSpiderQueen(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpiderQueen::Clone(void* pArg)
{
	CSpiderQueen* pInstance = new CSpiderQueen(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSpiderQueen");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpiderQueen::Free()
{
	__super::Free();
}
