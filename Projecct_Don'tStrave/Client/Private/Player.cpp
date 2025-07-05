#include "Player.h"
#include "GameInstance.h"
#include "Monster.h"
#include "Item.h"

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
	LoadScml("../Bin/Resources/Textures/Player/Wilson/ghost_wilson.scml");
	//AddTexture("../Bin/Resources/Textures/Objects/Evergreen/evergreen_new.scml", L"../Bin/Resources/Textures/Objects/Evergreen/");
	//LoadScml("../Bin/Resources/Textures/Objects/Evergreen/evergreen_new.scml");
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	LoadImageFile();

	//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][DIR::DOWN][m_tMotion]);


	PLAYER_DESC data = *static_cast<PLAYER_DESC*>(pArg);
	m_pPlayer = new PLAYER_DATA;
	
	m_pPlayer->iMaxHp = data.iMaxHp;
	m_pPlayer->iMaxHunger = data.iMaxHunger;
	m_pPlayer->iMaxMental = data.iMaxMental;
	m_pPlayer->iHp = data.iMaxHp;
	m_pPlayer->iHunger = data.iMaxHunger;
	m_pPlayer->iMental = data.iMaxMental;
	m_pPlayer->tItem = SWAPOBJECT::NONE;
	m_pPlayer->fSpeed = 4.f;

	m_pPlayer->iTemp = 0;
	m_pPlayer->fAtkRatio = data.fAtk;
	m_pPlayer->fDefRatio = data.fDef;
	m_pPlayer->iAtk = 50;
	m_pPlayer->iDef = 0;
	m_pPlayer->iMaxHit = 10;
	m_pPlayer->iHit = 10;
	m_pPlayer->fPos = data.fPos;
	m_pPlayer->pWorkObject = nullptr;
	SetAnimation(DIR::DIR_END, MOTION::BUCKED);

	m_pChar = m_pPlayer;

	m_pTransformCom->SetPosition({ 0.f, 0.f, 0.f });

	m_pCollision_Com->SetCollisionSize({ 0.2f, 0.f ,0.f });

	m_bControll = true;
	m_bIsGhost = false;

	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& _Dir) { BeginHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& _Dir) { OverlapHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& _Dir) { EndHitActor(HitActor, _Dir); });
	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	if (0 >= m_pPlayer->iHunger) {
		Death();
	}
}

void CPlayer::Update(_float fTimeDelta)
{
	if (m_bControll) {
		if (MOTION::BUCKED == m_tMotion) {
			m_fAniTime = 0.f;
			m_bControll = false;
		}
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
			SetAnimation(m_tDir, m_tMotion);
		}
		if (m_pGameInstance->KeyPressed('W') || m_pGameInstance->KeyPressed('S') || m_pGameInstance->KeyPressed('D') || m_pGameInstance->KeyPressed('A'))
		{
			switch (m_tMotion)
			{
			case MOTION::GHOST_IDLE:
				break;
			case MOTION::IDLE_TO_RUN:
				if (m_iLength <= m_fAniTime) {
					SetAnimation(m_tDir, MOTION::RUN);
				}
			case MOTION::RUN:
				break;
			default:
				SetAnimation(m_tDir, MOTION::IDLE_TO_RUN);
				break;
			}
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
			m_pPlayer->pWorkObject = nullptr;
			m_pPlayer->fPos += *D3DXVec3Normalize(&vMove, &vMove) * 2.f * fTimeDelta;
			m_pTransformCom->SetPosition(m_pPlayer->fPos);
		}
		else {
			if (nullptr != m_pPlayer->pWorkObject) {
				_float3 move = m_pPlayer->pWorkObject->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pPlayer->fPos;
				m_pPlayer->fPos += *D3DXVec3Normalize(&move, &move) * 2.f * fTimeDelta;
				m_pTransformCom->SetPosition(m_pPlayer->fPos);
				switch (m_tMotion)
				{
				case MOTION::GHOST_IDLE:
					break;
				case MOTION::IDLE_TO_RUN:
					if (m_iLength <= m_fAniTime) {
						SetAnimation(m_tDir, MOTION::RUN);
					}
				case MOTION::RUN:
					break;
				default:
					SetAnimation(m_tDir, MOTION::IDLE_TO_RUN);
					break;
				}
			}
			else {
				switch (m_tMotion)
				{
				case  MOTION::IDLE_TO_RUN:
				case  MOTION::RUN:
					if (m_iLength <= m_fAniTime)
					{
						SetAnimation(m_tDir, MOTION::RUN_TO_IDLE);
						//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
					}
					break;
				case MOTION::RUN_TO_IDLE:
				case MOTION::ATTACK:
					if (m_iLength <= m_fAniTime)
					{
						SetAnimation(m_tDir, MOTION::IDLE);
						//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
						m_bControll = false;
					}
					break;
				default:
					break;
				}
			}
		}
		if (GetKeyState(VK_SPACE) & 0x8000)
		{
			CGameObject* object = m_pCharacterInstance->Get_NearObject(this, 5.f, FIELDOBJECT::OBJECT).front();
			if (object) {
				m_pPlayer->pWorkObject = object;
			}
		}
		if (m_pGameInstance->KeyDown(VK_CONTROL))
		{

			CGameObject* object = m_pCharacterInstance->Get_NearObject(this, 5.f, FIELDOBJECT::MONSTER).front();
			if (object) {
				m_pPlayer->pWorkObject = object;
			}
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
				SetAnimation(DIR::DIR_END, MOTION::BUCK_PST);
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
				SetAnimation(m_tDir, MOTION::IDLE);
				//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
				m_bControll = true;
			}
			break;
		case MOTION::DEATH1:
		case MOTION::DEATH2:
			if (m_iLength <= m_fAniTime) {
				SetAnimation(DIR::DIR_END, MOTION::GHOST_APPEAR);
				//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
			}
			break;
		case MOTION::GHOST_APPEAR:
			if (m_iLength <= m_fAniTime) {
				SetAnimation(DIR::DIR_END, MOTION::GHOST_IDLE);
				//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
				m_bControll = true;
			}
			break;
		}
	}

	if (GetKeyState('R') & 0x8000)
	{
		SetAnimation(DIR::DIR_END, MOTION::DIAL);
		//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
		m_bControll = false;
	}
	if (GetKeyState('P') & 0x8000)
	{
		m_isDead = true;
	}
	SetAnimation(m_tDir, m_tMotion);

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
		m_pPlayer->tItem = SWAPOBJECT::NONE;
		SetAnimation(m_tDir, m_tMotion);
		//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
	}
	if (GetKeyState('X') & 0x8000)
	{
		m_pPlayer->tItem = SWAPOBJECT::AXE;
		SetAnimation(m_tDir, m_tMotion);
		//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
	}
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	SetDir();
	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
	if (m_pPlayer->pWorkObject && m_pPlayer->pWorkObject->isDead()) {
		m_pPlayer->pWorkObject = nullptr;
	}
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
	SetAnimation(m_tDir, MOTION::DAMAGE);
	//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
}

void CPlayer::Attack()
{
	m_bAttack = true;
	m_bControll = false;
	SetAnimation(m_tDir, MOTION::ATTACK);
	//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
}

void CPlayer::Death()
{
	m_bControll = false;
	m_bIsGhost = true;
	m_tDir = DIR::DOWN;
	//m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][DIR::SIDE][m_tMotion]);
	SetAnimation(DIR::DIR_END, MOTION::DEATH2);
	//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][DIR::SIDE][m_tMotion]);
}

void CPlayer::Get_Damage(_uint iAtk)
{
	__super::Get_Damage(max(0, iAtk - (m_pPlayer->iDef * m_pPlayer->fDefRatio)));
}

PLAYER_DATA* CPlayer::Get_Player()
{
	return m_pPlayer;
}

void CPlayer::SetItem(SWAPOBJECT tItem)
{
}

HRESULT CPlayer::SetAnimation(DIR dir, MOTION motion)
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
	if (SWAPOBJECT::NONE != m_pPlayer->tItem) {
		m_sAnim += L"_item";
	}
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
	if (HitActor == m_pPlayer->pWorkObject) {
		if (dynamic_cast<CMonster*>(HitActor)) {
			if (!m_bAttack && m_tMotion != MOTION::ATTACK) {
				Attack();
			}
			if (m_bAttack && m_tMotion == MOTION::ATTACK && (SWAPOBJECT::NONE != m_pPlayer->tItem ? 330 : 200) <= (int)m_fAniTime) {
				dynamic_cast<CMonster*>(HitActor)->Get_Damage(m_pPlayer->iAtk * m_pPlayer->fAtkRatio);
				m_bAttack = false;
				if (0 >= dynamic_cast<CMonster*>(HitActor)->Get_Monster()->iHp) {
					m_pPlayer->pWorkObject = nullptr;
				}
			}
		}
		if (dynamic_cast<CItem*>(HitActor)) {
			SetAnimation(m_tDir, MOTION::PICKUP);
			m_pPlayer->pWorkObject = nullptr;
		}
	}
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
	Safe_Delete(m_pPlayer);
}
