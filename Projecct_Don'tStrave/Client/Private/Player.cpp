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
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->SetPosition(_float3(50.f, 0.f, 50.f));
	m_pAnimTransformCom->SetPosition(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION));

	
	CPlayerAnim::PLAYER_DESC AnimDesc;
	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.pVIBufferCom = m_pVIBufferCom;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.bIsLoop = true;
	
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < DIR::DIR_END; ++j) {
			for (int k = 0; k < MOTION::MOTION_END; ++k) {
				if (m_pTextureCom[i][j][k]) {
					AnimDesc.Frame.pAnimTexture = m_pTextureCom[i][j][k];
					m_pPlayerAnim[i][j][k] = CPlayerAnim::Create(&AnimDesc);
				}
			}
		}
	}
	AnimDesc.pTransformCom = m_pSwapObjectTransformCom;
	for (int i = 0; i < SWAPOBJECT::SWAPOBJECT_END; ++i) {
		for (int j = 0; j < DIR::DIR_END; ++j) {
			for (int k = 0; k < MOTION::MOTION_END; ++k) {
				if (m_pSwapObjectTextureCom[i][j][k]) {
					AnimDesc.Frame.pAnimTexture = m_pSwapObjectTextureCom[i][j][k];
					m_pSwapObjectPlayerAnim[i][j][k] = CPlayerAnim::Create(&AnimDesc);
				}
			}
		}
	}


	m_iSwapObject = 0;
	m_tMotion = MOTION::BUCKED;
	m_tItem = SWAPOBJECT::SWAPOBJECT_NONE;
	SetAnimation(m_iSwapObject, DIR::DIR_END, m_tMotion);
	//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][DIR::DOWN][m_tMotion]);

	m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][DIR::DOWN][m_tMotion]);
	m_iMaxHp = 150;
	m_iHunger = 100;
	m_iHp = m_iMaxHp;
	m_iTemp = 0;
	m_iAtk = 50;
	m_iDef = 0;
	m_iMaxHit = 10;
	m_iHit = m_iMaxHit;
	m_bControll = false;


	m_pCollision_Com->SetCollisionSize({1.f, 1.f ,1.f});

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
				if (m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]->IsEnd())
					m_tMotion = MOTION::RUN;
			case MOTION::RUN:
				break;
			default:
				m_tMotion = MOTION::IDLE_TO_RUN;
				break;
			}
			if (GetKeyState('W') & 0x8000) {
				_float3		vPosition = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
				_float3		vLook = m_pTransformCom->GetWorldState(WORLDSTATE::LOOK);

				vPosition += *D3DXVec3Normalize(&vLook, &vLook) * 2.f * fTimeDelta;

				m_pTransformCom->SetPosition(vPosition);
			}
			if (GetKeyState('S') & 0x8000)
			{
				_float3		vPosition = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
				_float3		vLook = m_pTransformCom->GetWorldState(WORLDSTATE::LOOK);

				vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * 2.f * fTimeDelta;

				m_pTransformCom->SetPosition(vPosition);
			}
			if (GetKeyState('A') & 0x8000)
			{
				_float3		vPosition = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
				_float3		vLook = m_pTransformCom->GetWorldState(WORLDSTATE::RIGHT);

				vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * 2.f * fTimeDelta;
				m_pTransformCom->SetPosition(vPosition);
			}
			if (GetKeyState('D') & 0x8000)
			{
				_float3		vPosition = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
				_float3		vLook = m_pTransformCom->GetWorldState(WORLDSTATE::RIGHT);

				vPosition += *D3DXVec3Normalize(&vLook, &vLook) * 2.f * fTimeDelta;
				m_pTransformCom->SetPosition(vPosition);
			}
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
				if (m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]->IsEnd())
				{
					m_tMotion = MOTION::RUN_TO_IDLE;

					m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][m_tDir][m_tMotion]);
					SetAnimation(m_iSwapObject, m_tDir, m_tMotion);
					//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
				}
				break;
			case MOTION::RUN_TO_IDLE:
			case MOTION::ATTACK:
				if (m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]->IsEnd())
				{
					m_tMotion = MOTION::IDLE;
					m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][m_tDir][m_tMotion]);
					SetAnimation(m_iSwapObject, m_tDir, m_tMotion);
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
			list<CGameObject*> objects = m_pCharacterInstance->Get_NearObject(this);
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
			if (m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]->IsEnd())
			{
				m_tMotion = MOTION::BUCK_PST;
				m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][m_tDir][m_tMotion]);
				SetAnimation(m_iSwapObject, DIR::DIR_END, MOTION::BUCK_PST);
				//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
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
			if (m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]->IsEnd()) {
				m_tMotion = MOTION::IDLE;
				m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][m_tDir][m_tMotion]);
				SetAnimation(m_iSwapObject, m_tDir, m_tMotion);
				//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
				m_bControll = true;
			}
			break;
		case MOTION::DEATH1:
		case MOTION::DEATH2:
			if (m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]->IsEnd()) {
				m_tMotion = MOTION::GHOST_APPEAR;
				m_iSwapObject = 0;
				m_tDir = DIR::DOWN;
				m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[SWAPOBJECT::SWAPOBJECT_NONE][0][0]);
				SetAnimation(m_iSwapObject, DIR::DIR_END, m_tMotion);
				//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
			}
			break;
		case MOTION::GHOST_APPEAR:
			if (m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]->IsEnd()) {
				m_tMotion = MOTION::GHOST_IDLE;
				SetAnimation(m_iSwapObject, DIR::DIR_END, m_tMotion);
				//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
				m_bControll = true;
			}
			break;
		}
	}

	if (GetKeyState('R') & 0x8000)
	{
		m_tMotion = MOTION::DIAL;
		SetAnimation(m_iSwapObject, DIR::DIR_END, m_tMotion);
		m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][m_tDir][m_tMotion]);
		//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
		m_bControll = false;
	}
	if (GetKeyState('P') & 0x8000)
	{
		m_isDead = true;
	}
	SetAnimation(m_iSwapObject, m_tDir, m_tMotion);
	m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][m_tDir][m_tMotion]);

	if (m_pGameInstance->KeyPressed('E'))
	{
		m_pTransformCom->TurnRate(_float3(0.f, 1.f, 0.f), fTimeDelta);
		m_pAnimTransformCom->TurnRate(_float3(0.f, 1.f, 0.f), fTimeDelta);
		m_pSwapObjectTransformCom->TurnRate(_float3(0.f, 1.f, 0.f), fTimeDelta);
	}
	if (m_pGameInstance->KeyPressed('Q'))
	{
		m_pTransformCom->TurnRate(_float3(0.f, -1.f, 0.f), fTimeDelta);
		m_pAnimTransformCom->TurnRate(_float3(0.f, -1.f, 0.f), fTimeDelta);
		m_pSwapObjectTransformCom->TurnRate(_float3(0.f, -1.f, 0.f), fTimeDelta);
	}
	if (m_pGameInstance->KeyPressed('Y'))
	{
		m_iSwapObject = 0;
		m_tItem = SWAPOBJECT::SWAPOBJECT_NONE;
		m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][m_tDir][m_tMotion]);
		SetAnimation(m_iSwapObject, m_tDir, m_tMotion);
		//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
	}
	if (GetKeyState('X') & 0x8000)
	{
		m_iSwapObject = 1;
		m_tItem = SWAPOBJECT::SWAPOBJECT_AXE;
		m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][m_tDir][m_tMotion]);
		SetAnimation(m_iSwapObject, m_tDir, m_tMotion);
		//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
	}
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pAnimController->Tick(fTimeDelta);
	if (m_tItem != SWAPOBJECT::SWAPOBJECT_NONE) {
		m_pSwapObjectAnimController->Tick(fTimeDelta);
	}
	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
	SetDir();
}

HRESULT CPlayer::Render()
{
	if (FAILED(Begin_RenderState()))
		return E_FAIL;
	if (DIR::DOWN == m_tDir) {
		m_pAnimController->Render();
		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pAnimTransformCom->Get_World());
		m_pVIBufferCom->Render();

		//m_pSwapObjectAnimController->Render();
		//m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pSwapObjectTransformCom->Get_World());
		//m_pVIBufferCom->Render();
	}
	else {
		//m_pSwapObjectAnimController->Render();
		//m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pSwapObjectTransformCom->Get_World());
		//m_pVIBufferCom->Render();
		m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]->Render(MOVE_DIR::MOVE_LEFT == m_tMoveDIr);
		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pAnimTransformCom->Get_World());
		m_pVIBufferCom->Render();
	}
	if (FAILED(End_RenderState()))
		return E_FAIL;

	//m_pCollision_Com->Render();

	return S_OK;
}

void CPlayer::Damage()
{
	m_bControll = false;
	m_tMotion = MOTION::DAMAGE;
	m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][m_tDir][m_tMotion]);
	SetAnimation(m_iSwapObject, m_tDir, m_tMotion);
	//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
}

void CPlayer::Attack()
{
	m_bAttack = true;
	m_bControll = false;
	m_tMotion = MOTION::ATTACK;
	m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][m_tDir][m_tMotion]);
	SetAnimation(m_iSwapObject, m_tDir, m_tMotion);
	//m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]);
}

void CPlayer::Death()
{
	m_bControll = false;
	m_bIsGhost = true;
	m_tMotion = MOTION::DEATH2;
	m_tDir = DIR::DOWN;
	//m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][DIR::SIDE][m_tMotion]);
	SetAnimation(m_iSwapObject, DIR::DIR_END, m_tMotion);
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
	AddAnimation(i, dir, motion);
	DIR _dir = dir;
	if (DIR::DIR_END == dir)
		_dir = DIR::DOWN;
	m_pAnimController->ChangeState(m_pPlayerAnim[i][_dir][motion]);
	return S_OK;
}

HRESULT CPlayer::AddAnimation(_uint i, DIR dir, MOTION motion)
{
	DIR _dir = dir;
	if (DIR::DIR_END == dir)
		_dir = DIR::DOWN;
	if (m_pPlayerAnim[m_iSwapObject][_dir][m_tMotion] == nullptr) {
		wstring str = L"../Bin/Resources/Textures/Player/Wilson";
		switch (i)
		{
		case 0:
			str += L"/NoItem";
			break;
		case 1:
			str += L"/Item";
			break;
		}
		switch (motion)
		{
		case MOTION::BUCKED:
			str += L"/bucked";
			break;
		case MOTION::BUCK_PST:
			str += L"/buck_pst";
			break;
		case MOTION::IDLE:
			str += L"/idle_loop";
			break;
		case MOTION::IDLE_TO_RUN:
			str += L"/run_pre";
			break;
		case MOTION::RUN:
			str += L"/run_loop";
			break;
		case MOTION::RUN_TO_IDLE:
			str += L"/run_pst";
			break;
		case MOTION::DIAL:
			str += L"/dial";
			break;
		case MOTION::IDLE_TO_BUILD:
			str += L"/build_pre";
			break;
		case MOTION::BUILD:
			str += L"/build_loop";
			break;
		case MOTION::BUILD_TO_IDLE:
			str += L"/build_pst";
			break;
		case MOTION::IDLE_TO_AXE:
			str += L"/chop_pre";
			break;
		case MOTION::AXE:
			str += L"/chop_loop";
			break;
		case MOTION::IDLE_TO_PICKAXE:
			str += L"/pickaxe_pre";
			break;
		case MOTION::PICKAXE:
			str += L"/pickaxe_loop";
			break;
		case MOTION::PICKAXE_TO_IDLE:
			str += L"/pickaxe_pst";
			break;
		case MOTION::IDLE_TO_SHOVEL:
			str += L"/shovel_pre";
			break;
		case MOTION::SHOVEL:
			str += L"/shovel_loop";
			break;
		case MOTION::SHOVEL_TO_IDLE:
			str += L"/shovel_pst";
			break;
		case MOTION::ATTACK:
			str += L"/atk";
			break;
		case MOTION::PICKUP:
			str += L"/pickup";
			break;
		case MOTION::GIVE:
			str += L"/give";
			break;
		case MOTION::DAMAGE:
			str += L"/damage";
			break;
		case MOTION::DEATH1:
			str += L"/death";
			break;
		case MOTION::DEATH2:
			str += L"/death2";
			break;
		case MOTION::GHOST_APPEAR:
			str += L"/ghost_appear";
			break;
		case MOTION::GHOST_IDLE:
			str += L"/ghost_idle";
			break;
		case MOTION::GHOST_DISSIPATE:
			str += L"/ghost_dissipate";
			break;
		}
		switch (dir)
		{
		case DIR::DOWN:
			str += L"_down";
			break;
		case DIR::SIDE:
			str += L"_side";
			break;
		case DIR::UP:
			str += L"_up";
			break;
		}
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_" + str),
			TEXT("Com_" + str), reinterpret_cast<CComponent**>(&m_pTextureCom[i][_dir][motion]))))
		{
			m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_" + str),
				CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, str.c_str()));
			__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_" + str),
				TEXT("Com_" + str), reinterpret_cast<CComponent**>(&m_pTextureCom[i][_dir][motion]));

		}

		CPlayerAnim::PLAYER_DESC AnimDesc;
		AnimDesc.pParentTransformCom = m_pTransformCom;
		AnimDesc.pTransformCom = m_pAnimTransformCom;
		AnimDesc.pVIBufferCom = m_pVIBufferCom;
		AnimDesc.Frame.iStartFrame = 0;
		AnimDesc.Frame.bIsLoop = true;

		AnimDesc.Frame.pAnimTexture = m_pTextureCom[i][_dir][motion];
		m_pPlayerAnim[i][_dir][motion] = CPlayerAnim::Create(&AnimDesc);
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

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Idle_Loop_Down"),
		TEXT("Com_Idle_Loop_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::DOWN][MOTION::IDLE]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Idle_Loop_Side"),
		TEXT("Com_Idle_Loop_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::SIDE][MOTION::IDLE]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Run_Pre_Down"),
		TEXT("Com_Run_Pre_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::DOWN][MOTION::IDLE_TO_RUN]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Run_Pre_Side"),
		TEXT("Com_Run_Pre_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::SIDE][MOTION::IDLE_TO_RUN]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Run_Pre_Up"),
		TEXT("Com_Run_Pre_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::UP][MOTION::IDLE_TO_RUN]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Run_Loop_Down"),
		TEXT("Com_Run_Loop_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::DOWN][MOTION::RUN]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Run_Loop_Side"),
		TEXT("Com_Run_Loop_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::SIDE][MOTION::RUN]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Run_Loop_Up"),
		TEXT("Com_Run_Loop_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::UP][MOTION::RUN]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Run_Pst_Down"),
		TEXT("Com_Run_Pst_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::DOWN][MOTION::RUN_TO_IDLE]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Run_Pst_Side"),
		TEXT("Com_Run_Pst_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::SIDE][MOTION::RUN_TO_IDLE]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Run_Pst_Up"),
		TEXT("Com_Run_Pst_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::UP][MOTION::RUN_TO_IDLE]))))
		return E_FAIL;

		/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Death1"),
		TEXT("Com_Death1"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::SIDE][MOTION::DEATH1]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Death2"),
		TEXT("Com_Death2"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::SIDE][MOTION::DEATH2]))))
		return E_FAIL;

		/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Item_Idle_Loop_Down"),
		TEXT("Com_Item_Idle_Loop_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::DOWN][MOTION::IDLE]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Item_Idle_Loop_Side"),
		TEXT("Com_Item_Idle_Loop_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::SIDE][MOTION::IDLE]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Item_Idle_Loop_Up"),
		TEXT("Com_Item_Idle_Loop_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::UP][MOTION::IDLE]))))
		return E_FAIL;


	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Item_Run_Pre_Down"),
		TEXT("Com_Item_Run_Pre_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::DOWN][MOTION::IDLE_TO_RUN]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Item_Run_Pre_Side"),
		TEXT("Com_Item_Run_Pre_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::SIDE][MOTION::IDLE_TO_RUN]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Item_Run_Pre_Up"),
		TEXT("Com_Item_Run_Pre_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::UP][MOTION::IDLE_TO_RUN]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Item_Run_Loop_Down"),
		TEXT("Com_Item_Run_Loop_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::DOWN][MOTION::RUN]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Item_Run_Loop_Side"),
		TEXT("Com_Item_Run_Loop_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::SIDE][MOTION::RUN]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Item_Run_Loop_Up"),
		TEXT("Com_Item_Run_Loop_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::UP][MOTION::RUN]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Item_Run_Pst_Down"),
		TEXT("Com_Item_Run_Pst_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::DOWN][MOTION::RUN_TO_IDLE]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Item_Run_Pst_Side"),
		TEXT("Com_Item_Run_Pst_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::SIDE][MOTION::RUN_TO_IDLE]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Item_Run_Pst_Up"),
		TEXT("Com_Item_Run_Pst_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::UP][MOTION::RUN_TO_IDLE]))))
		return E_FAIL;

		/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Item_Death1"),
		TEXT("Com_Item_Death1"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::SIDE][MOTION::DEATH1]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::OBJECT), TEXT("Prototype_Component_Texture_Player_Item_Death2"),
		TEXT("Com_Item_Death2"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::SIDE][MOTION::DEATH2]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Anim_Transform"), reinterpret_cast<CComponent**>(&m_pAnimTransformCom), &TransformDesc)))
		return E_FAIL;


	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_AnimController"),
		TEXT("Com_AnimController"), (CComponent**)&m_pAnimController)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform2"), reinterpret_cast<CComponent**>(&m_pSwapObjectTransformCom), &TransformDesc)))
		return E_FAIL;


	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_AnimController"),
		TEXT("Com_AnimController2"), (CComponent**)&m_pSwapObjectAnimController)))
		return E_FAIL;

	CBox_Collision_Component::Collision_Desc Col_Desc = {};
	Col_Desc.pOwner = this;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_BoxCollision"),
		TEXT("Com_BoxCollision"), reinterpret_cast<CComponent**>(&m_pCollision_Com), &Col_Desc)))
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
		if (dynamic_cast<CMonster*>(HitActor) && m_bAttack && m_tMotion == MOTION::ATTACK && m_pPlayerAnim[m_iSwapObject][m_tDir][m_tMotion]->IsAttack(10)) {
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
	Safe_Release(m_pSwapObjectTransformCom);
	Safe_Release(m_pSwapObjectAnimController);


	Safe_Release(m_pCollision_Com);

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < DIR::DIR_END; ++j) {
			for (int k = 0; k < MOTION::MOTION_END; ++k) {
				if (m_pTextureCom[i][j][k]) {
					Safe_Release(m_pTextureCom[i][j][k]);
				}
				if (m_pPlayerAnim[i][j][k]) {
					Safe_Release(m_pPlayerAnim[i][j][k]);
				}
			}
		}
	}
	for (int i = 0; i < SWAPOBJECT::SWAPOBJECT_END; ++i) {
		for (int j = 0; j < DIR::DIR_END; ++j) {
			for (int k = 0; k < MOTION::MOTION_END; ++k) {
				if (m_pSwapObjectTextureCom[i][j][k]) {
					Safe_Release(m_pSwapObjectTextureCom[i][j][k]);
				}
				if (m_pSwapObjectPlayerAnim[i][j][k]) {
					Safe_Release(m_pSwapObjectPlayerAnim[i][j][k]);
				}
			}
		}
	}
}
