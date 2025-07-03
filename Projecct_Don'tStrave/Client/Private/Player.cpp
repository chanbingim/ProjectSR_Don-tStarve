#include "Player.h"
#include "GameInstance.h"
#include "Monster.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCharacter{ pGraphic_Device }
{

}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CCharacter{ Prototype }
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	AddTexture("../Bin/Resources/Textures/Player/Wilson/wilson_idle.scml", L"../Bin/Resources/Textures/Player/Wilson/");
	LoadScml("../Bin/Resources/Textures/Player/Wilson/wilson_idle.scml");
	LoadScml("../Bin/Resources/Textures/Player/Wilson/wilson_atk.scml");
	LoadScml("../Bin/Resources/Textures/Player/Wilson/wilson_item.scml");
	LoadScml("../Bin/Resources/Textures/Player/Wilson/wilson_run.scml");
	LoadScml("../Bin/Resources/Textures/Player/Wilson/wilson_action.scml");
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	LoadImageFile();

	m_pTransformCom->SetPosition(_float3(0.f, 0.f, 0.f));

	m_iSwapObject = 0;
	m_tItem = SWAPOBJECT::SWAPOBJECT_NONE;
	SetAnimation(m_iSwapObject, DIR::DIR_END, MOTION::BUCKED);
	//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][DIR::DOWN][m_tMotion]);

	m_iMaxHp = 150;
	m_iHunger = 100;
	m_iHp = m_iMaxHp;
	m_iTemp = 0;
	m_iAtk = 50;
	m_iDef = 0;
	m_iMaxHit = 10;
	m_iHit = m_iMaxHit;
	m_bControll = false;


	m_pCollision_Com->SetCollisionSize({ 0.2f, 0.f ,0.f });

	m_bIsGhost = false;

	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& _Dir) { BeginHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& _Dir) { OverlapHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& _Dir) { EndHitActor(HitActor, _Dir); });
	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	if (0 >= m_iHunger) {
		Death();
	}
}

void CPlayer::Update(_float fTimeDelta)
{
	if (m_bControll) {
		switch (m_tMotion)
		{
		case CPlayer::IDLE:
		case CPlayer::IDLE_TO_RUN:
		case CPlayer::RUN:
		case CPlayer::RUN_TO_IDLE:
		case CPlayer::IDLE_TO_BUILD:
		case CPlayer::BUILD:
		case CPlayer::BUILD_TO_IDLE:
		case CPlayer::IDLE_TO_AXE:
		case CPlayer::AXE:
		case CPlayer::IDLE_TO_PICKAXE:
		case CPlayer::PICKAXE:
		case CPlayer::PICKAXE_TO_IDLE:
		case CPlayer::IDLE_TO_SHOVEL:
		case CPlayer::SHOVEL:
		case CPlayer::SHOVEL_TO_IDLE:
		case CPlayer::ATTACK:
		case CPlayer::PICKUP:
		case CPlayer::GIVE:
		case CPlayer::DAMAGE:
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
			}
			SetAnimation(m_iSwapObject, m_tDir, m_tMotion);
		}
		if (GetKeyState('W') & 0x8000 || GetKeyState('S') & 0x8000 || GetKeyState('D') & 0x8000 || GetKeyState('A') & 0x8000)
		{
			switch (m_tMotion)
			{
			case MOTION::GHOST_IDLE:
				break;
			case MOTION::IDLE_TO_RUN:
				if (m_iLength <= m_fAniTime) {
					SetAnimation(m_iSwapObject, m_tDir, MOTION::RUN);
				}
			case MOTION::RUN:
				break;
			default:
				SetAnimation(m_iSwapObject, m_tDir, MOTION::IDLE_TO_RUN);
				break;
			}
			_float3		vPosition = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
			_float3		vMove = {0.f ,0.f ,0.f};
			if (GetKeyState('W') & 0x8000) {
				_float3		vLook = m_pTransformCom->GetWorldState(WORLDSTATE::LOOK);

				vMove += *D3DXVec3Normalize(&vLook, &vLook);
			}
			if (GetKeyState('S') & 0x8000)
			{
				_float3		vLook = m_pTransformCom->GetWorldState(WORLDSTATE::LOOK);

				vMove -= *D3DXVec3Normalize(&vLook, &vLook);
			}
			if (GetKeyState('A') & 0x8000)
			{
				_float3		vLook = m_pTransformCom->GetWorldState(WORLDSTATE::RIGHT);

				vMove -= *D3DXVec3Normalize(&vLook, &vLook);
			}
			if (GetKeyState('D') & 0x8000)
			{
				_float3		vLook = m_pTransformCom->GetWorldState(WORLDSTATE::RIGHT);

				vMove += *D3DXVec3Normalize(&vLook, &vLook);
			}

			vPosition += *D3DXVec3Normalize(&vMove, &vMove) * 2.f * fTimeDelta;
			m_pTransformCom->SetPosition(vPosition);
		}
		else {
			if (nullptr != m_pWorkObject) {
				_float3		vPosition = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
				_float3 move = m_pWorkObject->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);

				vPosition += *D3DXVec3Normalize(&move, &move) * 2.f * fTimeDelta;
				m_pTransformCom->SetPosition(vPosition);
			}
			switch (m_tMotion)
			{
			case  MOTION::IDLE_TO_RUN:
			case  MOTION::RUN:
				if (m_iLength <= m_fAniTime)
				{
					SetAnimation(m_iSwapObject, m_tDir, MOTION::RUN_TO_IDLE);
					//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
				}
				break;
			case MOTION::RUN_TO_IDLE:
			case MOTION::ATTACK:
				if (m_iLength <= m_fAniTime)
				{
					SetAnimation(m_iSwapObject, m_tDir, MOTION::IDLE);
					//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
					m_bControll = false;
				}
				break;
			default:
				break;
			}
		}
		if (GetKeyState(VK_SPACE) & 0x8000)
		{
			list<CGameObject*> objects = m_pCharacterInstance->Get_NearObject(this, 3.f);
			if (0 < objects.size()) {
				m_pWorkObject = objects.front();
			}
			//Attack();
			//m_tMotion = MOTION::ATTACK;
			//m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][m_tDir][m_tMotion]);
			//SetAnimation(m_iSwapObject, m_tDir, m_tMotion);
			////m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
			//m_bControll = false;
		}
		if (m_pGameInstance->KeyDown('F'))
		{
			Get_Damage(50);
		}
	}
	else {
		switch (m_tMotion)
		{
		case MOTION::IDLE:
			m_bControll = true;
			break;
		case MOTION::BUCKED:
			if (m_iLength < m_fAniTime)
			{
				SetAnimation(m_iSwapObject, DIR::DIR_END, MOTION::BUCK_PST);
			}
			break;
		case MOTION::BUCK_PST:
		case MOTION::DIAL:
		case MOTION::RUN_TO_IDLE:
		case MOTION::BUILD_TO_IDLE:
		case MOTION::PICKAXE_TO_IDLE:
		case MOTION::SHOVEL_TO_IDLE:
		case MOTION::ATTACK:
		case MOTION::PICKUP:
		case MOTION::GIVE:
		case MOTION::DAMAGE:
			if (m_iLength <= m_fAniTime) {
				SetAnimation(m_iSwapObject, m_tDir, MOTION::IDLE);
				//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
				m_bControll = true;
			}
			break;
		case MOTION::DEATH1:
		case MOTION::DEATH2:
			if (m_iLength <= m_fAniTime) {
				m_iSwapObject = 0;
				SetAnimation(m_iSwapObject, DIR::DIR_END, MOTION::GHOST_APPEAR);
				//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
			}
			break;
		case MOTION::GHOST_APPEAR:
			if (m_iLength <= m_fAniTime) {
				SetAnimation(m_iSwapObject, DIR::DIR_END, MOTION::GHOST_IDLE);
				//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
				m_bControll = true;
			}
			break;
		}
	}

	if (GetKeyState('R') & 0x8000)
	{
		SetAnimation(m_iSwapObject, DIR::DIR_END, MOTION::DIAL);
		//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
		m_bControll = false;
	}
	if (GetKeyState('P') & 0x8000)
	{
		m_isDead = true;
	}
	SetAnimation(m_iSwapObject, m_tDir, m_tMotion);

	if (m_pGameInstance->KeyPressed('E'))
	{
		m_pTransformCom->TurnRate(_float3(0.f, 1.f, 0.f), fTimeDelta);
	}
	if (m_pGameInstance->KeyPressed('Q'))
	{
		m_pTransformCom->TurnRate(_float3(0.f, -1.f, 0.f), fTimeDelta);
	}
	if (m_pGameInstance->KeyPressed('Y'))
	{
		m_iSwapObject = 0;
		m_tItem = SWAPOBJECT::SWAPOBJECT_NONE;
		SetAnimation(m_iSwapObject, m_tDir, m_tMotion);
		//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
	}
	if (GetKeyState('X') & 0x8000)
	{
		m_iSwapObject = 1;
		m_tItem = SWAPOBJECT::SWAPOBJECT_AXE;
		SetAnimation(m_iSwapObject, m_tDir, m_tMotion);
		//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
	}
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	SetDir();
	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CPlayer::Render()
{
	if (FAILED(Begin_RenderState()))
		return E_FAIL;
	if (DIR::DOWN == m_tDir) {
		RenderAnimation(m_sAnim);
	}
	else {
		RenderAnimation(m_sAnim);
	}
	if (FAILED(End_RenderState()))
		return E_FAIL;

	//m_pCollision_Com->Render();

	return S_OK;
}

void CPlayer::Damage()
{
	m_bControll = false;
	SetAnimation(m_iSwapObject, m_tDir, MOTION::DAMAGE);
	//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
}

void CPlayer::Attack()
{
	m_bAttack = true;
	m_bControll = false;
	SetAnimation(m_iSwapObject, m_tDir, MOTION::ATTACK);
	//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
}

void CPlayer::Death()
{
	m_bControll = false;
	m_bIsGhost = true;
	m_tDir = DIR::DOWN;
	//m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][DIR::SIDE][m_tMotion]);
	SetAnimation(m_iSwapObject, DIR::DIR_END, MOTION::DEATH2);
	//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][DIR::SIDE][m_tMotion]);
}

_uint CPlayer::Get_Hp()
{
	return m_iHp;
}

_uint CPlayer::Get_Hunger()
{
	return m_iHunger;
}

void CPlayer::SetItem(SWAPOBJECT tItem)
{
}

HRESULT CPlayer::SetAnimation(_uint i, DIR dir, MOTION motion)
{
	if (motion != m_tMotion) {
		m_fAniTime = 0.f;
	}
	switch (motion)
	{
	case MOTION::BUCKED:
		m_sAnim = L"bucked";
		break;
	case MOTION::BUCK_PST:
		m_sAnim = L"buck_pst";
		break;
	case MOTION::IDLE:
		m_sAnim = L"idle_loop";
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
	case MOTION::DIAL:
		m_sAnim = L"dial";
		break;
	case MOTION::IDLE_TO_BUILD:
		m_sAnim = L"build_pre";
		break;
	case MOTION::BUILD:
		m_sAnim = L"build_loop";
		break;
	case MOTION::BUILD_TO_IDLE:
		m_sAnim = L"build_pst";
		break;
	case MOTION::IDLE_TO_AXE:
		m_sAnim = L"chop_pre";
		break;
	case MOTION::AXE:
		m_sAnim = L"chop_loop";
		break;
	case MOTION::IDLE_TO_PICKAXE:
		m_sAnim = L"pickaxe_pre";
		break;
	case MOTION::PICKAXE:
		m_sAnim = L"pickaxe_loop";
		break;
	case MOTION::PICKAXE_TO_IDLE:
		m_sAnim = L"pickaxe_pst";
		break;
	case MOTION::IDLE_TO_SHOVEL:
		m_sAnim = L"shovel_pre";
		break;
	case MOTION::SHOVEL:
		m_sAnim = L"shovel_loop";
		break;
	case MOTION::SHOVEL_TO_IDLE:
		m_sAnim = L"shovel_pst";
		break;
	case MOTION::ATTACK:
		m_sAnim = L"atk";
		break;
	case MOTION::PICKUP:
		m_sAnim = L"pickup";
		break;
	case MOTION::GIVE:
		m_sAnim = L"give";
		break;
	case MOTION::DAMAGE:
		m_sAnim = L"hit";
		break;
	case MOTION::DEATH1:
		m_sAnim = L"death";
		break;
	case MOTION::DEATH2:
		m_sAnim = L"death2";
		break;
	case MOTION::GHOST_APPEAR:
		m_sAnim = L"ghost_appear";
		break;
	case MOTION::GHOST_IDLE:
		m_sAnim = L"ghost_idle";
		break;
	case MOTION::GHOST_DISSIPATE:
		m_sAnim = L"ghost_dissipate";
		break;
	}
	m_tMotion = motion;
	switch (motion)
	{
	case MOTION::BUCKED:
	case MOTION::BUCK_PST:
	case MOTION::DIAL:
	case MOTION::DEATH1:
	case MOTION::DEATH2:
	case MOTION::GHOST_APPEAR:
	case MOTION::GHOST_IDLE:
	case MOTION::GHOST_DISSIPATE:
		m_tDir = DIR::DOWN;
		break;
	default:
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
		m_tDir = dir;
	}
	if (i) {
		m_sAnim += L"_item";
	}
	return S_OK;
}

HRESULT CPlayer::Ready_Components()
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

	CSphere_Collision_Component::Collision_Desc Col_Desc = {};
	Col_Desc.pOwner = this;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SphereCollision"),
		TEXT("Com_SphereCollision"), reinterpret_cast<CComponent**>(&m_pCollision_Com), &Col_Desc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CPlayer::Begin_RenderState()
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

HRESULT CPlayer::End_RenderState()
{
	// m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pVIBufferCom->SetUV(1, 1, 1, 0, 1, false);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CPlayer::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CPlayer::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
	//if (HitActor == m_pWorkObject) {
	if (dynamic_cast<CMonster*>(HitActor) && m_bAttack && m_tMotion == MOTION::ATTACK && 330 <= (int)m_fAniTime) {
		dynamic_cast<CMonster*>(HitActor)->Get_Damage(m_iAtk);
		m_bAttack = false;
	}
	//}
}

void CPlayer::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer* pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pCollision_Com);
}
