#include "SpiderWarrior.h"
#include "GameInstance.h"

CSpiderWarrior::CSpiderWarrior(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{

}

CSpiderWarrior::CSpiderWarrior(const CSpiderWarrior& Prototype)
	: CMonster{ Prototype }
{
}

HRESULT CSpiderWarrior::Initialize_Prototype()
{
	AddTexture("../Bin/Resources/Textures/Monster/SpiderWarrior/spider.scml", L"../Bin/Resources/Textures/Monster/SpiderWarrior/");
	LoadScml("../Bin/Resources/Textures/Monster/SpiderWarrior/spider.scml");
	LoadScml("../Bin/Resources/Textures/Monster/SpiderWarrior/spiderwarrior.scml");
	return S_OK;
}

HRESULT CSpiderWarrior::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	LoadImageFile();

	m_pTransformCom->SetPosition(_float3(rand() % 20, 0.f, rand() % 20));


	SetAnimation(m_tDir, MOTION::IDLE);
	m_iMaxHp = 100;
	m_iHp = m_iMaxHp;
	m_iTemp = 0;
	m_iAtk = 30;
	m_iDef = 0;
	m_iMaxHit = 10;
	m_iHit = m_iMaxHit;
	m_fAtkCool = 5.f;
	m_bMove = false;


	m_pCollision_Com->SetCollisionSize({ 1.f, 1.f ,1.f });

	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& _Dir) { BeginHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& _Dir) { OverlapHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& _Dir) { EndHitActor(HitActor, _Dir); });

	return S_OK;
}


void CSpiderWarrior::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	m_pTarget = nullptr;
	for (auto target : m_pCharacterInstance->Get_NearObject(this)) {
		if (!dynamic_cast<CSpiderWarrior*>(target)) {
			m_pTarget = dynamic_cast<CCharacter*>(target);
		}
	}
}

void CSpiderWarrior::Update(_float fTimeDelta)
{
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
			_float3		vPosition = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
			vPosition += m_fDash * fTimeDelta;
			m_pTransformCom->SetPosition(vPosition);
		}
	}
	else if (m_pTarget) {
		_float3 move = m_pTarget->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
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
					SetAnimation(DIR::DIR_END, MOTION::TAUNT);
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
					SetAnimation(DIR::DIR_END, MOTION::TAUNT);
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
				_float3		vPosition = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
				vPosition += move * fTimeDelta;
				m_pTransformCom->SetPosition(vPosition);
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

void CSpiderWarrior::Late_Update(_float fTimeDelta)
{

	SetDir();
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);

}

HRESULT CSpiderWarrior::Render()
{
	__super::Render();


	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	RenderAnimation(m_sAnim);

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

void CSpiderWarrior::Damage()
{
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

HRESULT CSpiderWarrior::SetAnimation(DIR dir, MOTION motion)
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

HRESULT CSpiderWarrior::Ready_Components()
{
	/* Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Collision */
	CBox_Collision_Component::Collision_Desc Col_Desc = {};
	Col_Desc.pOwner = this;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_BoxCollision"),
		TEXT("Prototype_Component_BoxCollision"), reinterpret_cast<CComponent**>(&m_pCollision_Com), &Col_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpiderWarrior::Begin_RenderState()
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

HRESULT CSpiderWarrior::End_RenderState()
{
	// m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pVIBufferCom->SetUV(1, 1, 1, 0, 1, false);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CSpiderWarrior::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
	if (dynamic_cast<CCharacter*>(HitActor)) {
		if (!dynamic_cast<CMonster*>(HitActor) && m_tMotion == DASH_ATTACK) {
			m_pTarget->Get_Damage(m_iAtk);
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
				m_pTarget->Get_Damage(m_iAtk);
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
	Safe_Release(m_pCollision_Com);
}
