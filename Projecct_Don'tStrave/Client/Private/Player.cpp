#include "Player.h"
#include "GameInstance.h"

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

	m_pTransformCom->SetPosition(_float3(rand() % 20, 0.f, rand() % 20));
	m_pAnimTransformCom->SetPosition(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION));

	CPlayerAnim::PLAYER_DESC AnimDesc;
	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.pVIBufferCom = m_pVIBufferCom;
	AnimDesc.Frame.iStartFrame = 0;

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
	m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][DIR::DOWN][m_tMotion]);

	m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][DIR::DOWN][m_tMotion]);
	m_iMaxHp = 150;
	m_iHp = m_iMaxHp;
	m_iTemp = 0;
	m_iAtk = 5;
	m_iDef = 0;
	m_iMaxHit = 10;
	m_iHit = m_iMaxHit;
	m_bControll = false;

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
}

void CPlayer::Update(_float fTimeDelta)
{
	if (m_bControll) {

		if (GetKeyState('W') & 0x8000 || GetKeyState('S') & 0x8000 || GetKeyState('D') & 0x8000 || GetKeyState('A') & 0x8000)
		{
			switch (m_tMotion)
			{
			case MOTION::IDLE_TO_RUN:
				if (m_pPlayerAnim[m_iSwapObject][m_iDirection][m_tMotion]->IsEnd())
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

				vPosition += *D3DXVec3Normalize(&vLook, &vLook) * 1.5f * fTimeDelta;

				m_pTransformCom->SetPosition(vPosition);
				m_tDir = DIR::UP;
				m_iDirection = 2;
			}
			if (GetKeyState('S') & 0x8000)
			{
				_float3		vPosition = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
				_float3		vLook = m_pTransformCom->GetWorldState(WORLDSTATE::LOOK);

				vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * 1.5f * fTimeDelta;

				m_pTransformCom->SetPosition(vPosition);
				m_tDir = DIR::DOWN;
				m_iDirection = 0;
			}
			if (GetKeyState('A') & 0x8000)
			{
				_float3		vPosition = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
				_float3		vLook = m_pTransformCom->GetWorldState(WORLDSTATE::RIGHT);

				vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * 1.5f * fTimeDelta;
				m_pTransformCom->SetPosition(vPosition);

				m_iDirection = 1;
			}
			if (GetKeyState('D') & 0x8000)
			{
				_float3		vPosition = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
				_float3		vLook = m_pTransformCom->GetWorldState(WORLDSTATE::RIGHT);

				vPosition += *D3DXVec3Normalize(&vLook, &vLook) * 1.5f * fTimeDelta;
				m_pTransformCom->SetPosition(vPosition);
				m_iDirection = 1;
			}

		}
		else {
			switch (m_tMotion)
			{
			case  MOTION::IDLE_TO_RUN:
			case  MOTION::RUN:
				if (m_pPlayerAnim[m_iSwapObject][m_iDirection][m_tMotion]->IsEnd())
				{
					m_tMotion = MOTION::RUN_TO_IDLE;

					m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][m_iDirection][m_tMotion]);
					m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_iDirection][m_tMotion]);
				}
				break;
			case MOTION::RUN_TO_IDLE:
			case MOTION::ATTACK:
				if (m_pPlayerAnim[m_iSwapObject][m_iDirection][m_tMotion]->IsEnd())
				{
					m_tMotion = MOTION::IDLE;
					m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][m_iDirection][m_tMotion]);
					m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_iDirection][m_tMotion]);
					m_bControll = false;
				}
				break;
			default:
				break;
			}
		}
		if (GetKeyState(VK_SPACE) & 0x8000)
		{
			m_tMotion = MOTION::ATTACK;
			m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][m_iDirection][m_tMotion]);
			m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_iDirection][m_tMotion]);
			m_bControll = false;
		}
		if (GetKeyState('F') & 0x8000)
		{
			Get_Damage(50);
		}
	}
	else {
		switch (m_tMotion)
		{
		case MOTION::BUCKED:
			if (m_pPlayerAnim[m_iSwapObject][m_iDirection][m_tMotion]->IsEnd())
			{
				m_tMotion = MOTION::BUCK_PST;
				m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][m_iDirection][m_tMotion]);
				m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_iDirection][m_tMotion]);
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
			if (m_pPlayerAnim[m_iSwapObject][m_iDirection][m_tMotion]->IsEnd()) {
				m_tMotion = MOTION::IDLE;
				m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][m_iDirection][m_tMotion]);
				m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_iDirection][m_tMotion]);
				m_bControll = true;
			}
			break;
		case MOTION::IDLE:
			m_bControll = true;
			break;
		}
	}

	if (GetKeyState('R') & 0x8000)
	{
		m_iDirection = 0;
		m_tMotion = MOTION::DIAL;
		m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][m_iDirection][m_tMotion]);
		m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_iDirection][m_tMotion]);
		m_bControll = false;
	}
	m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][m_iDirection][m_tMotion]);
	m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_iDirection][m_tMotion]);
	if (m_pGameInstance->KeyUp('E'))
	{
		m_pTransformCom->TurnRate(_float3(0.f, 1.f, 0.f), fTimeDelta);
		m_pAnimTransformCom->TurnRate(_float3(0.f, 1.f, 0.f), fTimeDelta);
		m_pSwapObjectTransformCom->TurnRate(_float3(0.f, 1.f, 0.f), fTimeDelta);
	}
	if (m_pGameInstance->KeyDown('Q'))
	{
		m_pTransformCom->TurnRate(_float3(0.f, -1.f, 0.f), fTimeDelta);
		m_pAnimTransformCom->TurnRate(_float3(0.f, -1.f, 0.f), fTimeDelta);
		m_pSwapObjectTransformCom->TurnRate(_float3(0.f, -1.f, 0.f), fTimeDelta);
	}
	if (m_pGameInstance->KeyPressed('Z'))
	{
		m_iSwapObject = 0;
		m_tItem = SWAPOBJECT::SWAPOBJECT_NONE;
		m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][m_iDirection][m_tMotion]);
		m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_iDirection][m_tMotion]);
	}
	if (GetKeyState('X') & 0x8000)
	{
		m_iSwapObject = 1;
		m_tItem = SWAPOBJECT::SWAPOBJECT_AXE;
		m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][m_iDirection][m_tMotion]);
		m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_iDirection][m_tMotion]);
	}

	SetUp_OnTerrain(m_pTransformCom, 0.f);
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	m_pAnimController->Tick(fTimeDelta);
	if (m_tItem != SWAPOBJECT::SWAPOBJECT_NONE) {
		m_pSwapObjectAnimController->Tick(fTimeDelta);
	}
	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CPlayer::Render()
{
	if (FAILED(Begin_RenderState()))
		return E_FAIL;
	if (m_iDirection == 0) {

		m_pAnimController->Render();
		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pAnimTransformCom->Get_World());
		m_pVIBufferCom->Render();

		m_pSwapObjectAnimController->Render();
		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pSwapObjectTransformCom->Get_World());
		m_pVIBufferCom->Render();
	}
	else {

		m_pSwapObjectAnimController->Render();
		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pSwapObjectTransformCom->Get_World());
		m_pVIBufferCom->Render();
		m_pAnimController->Render();
		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pAnimTransformCom->Get_World());
		m_pVIBufferCom->Render();
	}


	if (FAILED(End_RenderState()))
		return E_FAIL;
	
	return S_OK;
}

void CPlayer::Damage()
{
	m_bControll = false;
	m_tMotion = MOTION::DAMAGE;
	m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][m_iDirection][m_tMotion]);
	m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_iDirection][m_tMotion]);
}

void CPlayer::Attack()
{
	m_bControll = false;
	m_tMotion = MOTION::ATTACK;
	m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][m_iDirection][m_tMotion]);
	m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_iDirection][m_tMotion]);
}

void CPlayer::Death()
{
	m_bControll = false;
	m_tMotion = MOTION::DEATH2;
	m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[m_tItem][DIR::SIDE][m_tMotion]);
	m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][DIR::SIDE][m_tMotion]);
}

void CPlayer::SetItem(SWAPOBJECT tItem)
{
}

HRESULT CPlayer::Ready_Components()
{
	/* Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Idle_Loop_Down"),
		TEXT("Com_Idle_Loop_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::DOWN][MOTION::IDLE]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Idle_Loop_Side"),
		TEXT("Com_Idle_Loop_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::SIDE][MOTION::IDLE]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Idle_Loop_Up"),
		TEXT("Com_Idle_Loop_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::UP][MOTION::IDLE]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pre_Down"),
		TEXT("Com_Run_Pre_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::DOWN][MOTION::IDLE_TO_RUN]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pre_Side"),
		TEXT("Com_Run_Pre_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::SIDE][MOTION::IDLE_TO_RUN]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pre_Up"),
		TEXT("Com_Run_Pre_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::UP][MOTION::IDLE_TO_RUN]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Loop_Down"),
		TEXT("Com_Run_Loop_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::DOWN][MOTION::RUN]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Loop_Side"),
		TEXT("Com_Run_Loop_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::SIDE][MOTION::RUN]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Loop_Up"),
		TEXT("Com_Run_Loop_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::UP][MOTION::RUN]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pst_Down"),
		TEXT("Com_Run_Pst_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::DOWN][MOTION::RUN_TO_IDLE]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pst_Side"),
		TEXT("Com_Run_Pst_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::SIDE][MOTION::RUN_TO_IDLE]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pst_Up"),
		TEXT("Com_Run_Pst_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::UP][MOTION::RUN_TO_IDLE]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Atk_Down"),
		TEXT("Com_Atk_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::DOWN][MOTION::ATTACK]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Atk_Side"),
		TEXT("Com_Atk_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::SIDE][MOTION::ATTACK]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Atk_Up"),
		TEXT("Com_Atk_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::UP][MOTION::ATTACK]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Bucked"),
		TEXT("Com_Bucked"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::DOWN][MOTION::BUCKED]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Buck_Pst"),
		TEXT("Com_Buck_Pst"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::DOWN][MOTION::BUCK_PST]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Damage_Down"),
		TEXT("Com_Damage_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::DOWN][MOTION::DAMAGE]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Damage_Side"),
		TEXT("Com_Damage_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::SIDE][MOTION::DAMAGE]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Damage_Up"),
		TEXT("Com_Damage_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::UP][MOTION::DAMAGE]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Death1"),
		TEXT("Com_Death1"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::SIDE][MOTION::DEATH1]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Death2"),
		TEXT("Com_Death2"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::SIDE][MOTION::DEATH2]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Dial"),
		TEXT("Com_Dial"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::DOWN][MOTION::DIAL]))))
		return E_FAIL;




	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Idle_Loop_Down"),
		TEXT("Com_Item_Idle_Loop_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::DOWN][MOTION::IDLE]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Idle_Loop_Side"),
		TEXT("Com_Item_Idle_Loop_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::SIDE][MOTION::IDLE]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Idle_Loop_Up"),
		TEXT("Com_Item_Idle_Loop_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::UP][MOTION::IDLE]))))
		return E_FAIL;


	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Pre_Down"),
		TEXT("Com_Item_Run_Pre_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::DOWN][MOTION::IDLE_TO_RUN]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Pre_Side"),
		TEXT("Com_Item_Run_Pre_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::SIDE][MOTION::IDLE_TO_RUN]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Pre_Up"),
		TEXT("Com_Item_Run_Pre_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::UP][MOTION::IDLE_TO_RUN]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Loop_Down"),
		TEXT("Com_Item_Run_Loop_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::DOWN][MOTION::RUN]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Loop_Side"),
		TEXT("Com_Item_Run_Loop_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::SIDE][MOTION::RUN]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Loop_Up"),
		TEXT("Com_Item_Run_Loop_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::UP][MOTION::RUN]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Pst_Down"),
		TEXT("Com_Item_Run_Pst_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::DOWN][MOTION::RUN_TO_IDLE]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Pst_Side"),
		TEXT("Com_Item_Run_Pst_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::SIDE][MOTION::RUN_TO_IDLE]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Pst_Up"),
		TEXT("Com_Item_Run_Pst_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::UP][MOTION::RUN_TO_IDLE]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Atk_Down"),
		TEXT("Com_Item_Atk_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::DOWN][MOTION::ATTACK]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Atk_Side"),
		TEXT("Com_Item_Atk_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::SIDE][MOTION::ATTACK]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Atk_Up"),
		TEXT("Com_Item_Atk_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::UP][MOTION::ATTACK]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Damage_Down"),
		TEXT("Com_Item_Damage_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::DOWN][MOTION::DAMAGE]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Damage_Side"),
		TEXT("Com_Item_Damage_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::SIDE][MOTION::DAMAGE]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Damage_Up"),
		TEXT("Com_Item_Damage_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::UP][MOTION::DAMAGE]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Death1"),
		TEXT("Com_Item_Death1"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::SIDE][MOTION::DEATH1]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Death2"),
		TEXT("Com_Item_Death2"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::SIDE][MOTION::DEATH2]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Dial"),
		TEXT("Com_Item_Dial"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::DOWN][MOTION::DIAL]))))
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

		/* Com_Transform */
	//CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
			TEXT("Com_Transform2"), reinterpret_cast<CComponent**>(&m_pSwapObjectTransformCom), &TransformDesc)))
			return E_FAIL;
	
	
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_AnimController"),
			TEXT("Com_AnimController2"), (CComponent**)&m_pSwapObjectAnimController)))
			return E_FAIL;



		/* Com_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Idle_Loop_Down"),
			TEXT("Com_Axe_Idle_Loop_Down"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[SWAPOBJECT::SWAPOBJECT_AXE][DIR::DOWN][MOTION::IDLE]))))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Idle_Loop_Side"),
			TEXT("Com_Axe_Idle_Loop_Side"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[SWAPOBJECT::SWAPOBJECT_AXE][DIR::SIDE][MOTION::IDLE]))))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Idle_Loop_Up"),
			TEXT("Com_Axe_Idle_Loop_Up"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[SWAPOBJECT::SWAPOBJECT_AXE][DIR::UP][MOTION::IDLE]))))
			return E_FAIL;


		/* Com_Axe_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Run_Pre_Down"),
			TEXT("Com_Axe_Run_Pre_Down"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[SWAPOBJECT::SWAPOBJECT_AXE][DIR::DOWN][MOTION::IDLE_TO_RUN]))))
			return E_FAIL;

		/* Com_Axe_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Run_Pre_Side"),
			TEXT("Com_Axe_Run_Pre_Side"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[SWAPOBJECT::SWAPOBJECT_AXE][DIR::SIDE][MOTION::IDLE_TO_RUN]))))
			return E_FAIL;

		/* Com_Axe_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Run_Pre_Up"),
			TEXT("Com_Axe_Run_Pre_Up"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[SWAPOBJECT::SWAPOBJECT_AXE][DIR::UP][MOTION::IDLE_TO_RUN]))))
			return E_FAIL;

		/* Com_Axe_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Run_Loop_Down"),
			TEXT("Com_Axe_Run_Loop_Down"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[SWAPOBJECT::SWAPOBJECT_AXE][DIR::DOWN][MOTION::RUN]))))
			return E_FAIL;

		/* Com_Axe_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Run_Loop_Side"),
			TEXT("Com_Axe_Run_Loop_Side"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[SWAPOBJECT::SWAPOBJECT_AXE][DIR::SIDE][MOTION::RUN]))))
			return E_FAIL;

		/* Com_Axe_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Run_Loop_Up"),
			TEXT("Com_Axe_Run_Loop_Up"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[SWAPOBJECT::SWAPOBJECT_AXE][DIR::UP][MOTION::RUN]))))
			return E_FAIL;

		/* Com_Axe_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Run_Pst_Down"),
			TEXT("Com_Axe_Run_Pst_Down"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[SWAPOBJECT::SWAPOBJECT_AXE][DIR::DOWN][MOTION::RUN_TO_IDLE]))))
			return E_FAIL;

		/* Com_Axe_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Run_Pst_Side"),
			TEXT("Com_Axe_Run_Pst_Side"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[SWAPOBJECT::SWAPOBJECT_AXE][DIR::SIDE][MOTION::RUN_TO_IDLE]))))
			return E_FAIL;

		/* Com_Axe_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Run_Pst_Up"),
			TEXT("Com_Axe_Run_Pst_Up"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[SWAPOBJECT::SWAPOBJECT_AXE][DIR::UP][MOTION::RUN_TO_IDLE]))))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Atk_Down"),
			TEXT("Com_Axe_Atk_Down"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[SWAPOBJECT::SWAPOBJECT_AXE][DIR::DOWN][MOTION::ATTACK]))))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Atk_Side"),
			TEXT("Com_Axe_Atk_Side"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[SWAPOBJECT::SWAPOBJECT_AXE][DIR::SIDE][MOTION::ATTACK]))))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Atk_Up"),
			TEXT("Com_Axe_Atk_Up"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[SWAPOBJECT::SWAPOBJECT_AXE][DIR::UP][MOTION::ATTACK]))))
			return E_FAIL;

		/* Com_Axe_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Damage_Down"),
			TEXT("Com_Axe_Damage_Down"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[SWAPOBJECT::SWAPOBJECT_AXE][DIR::DOWN][MOTION::DAMAGE]))))
			return E_FAIL;

		/* Com_Axe_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Damage_Side"),
			TEXT("Com_Axe_Damage_Side"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[SWAPOBJECT::SWAPOBJECT_AXE][DIR::SIDE][MOTION::DAMAGE]))))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Damage_Up"),
			TEXT("Com_Axe_Damage_Up"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[SWAPOBJECT::SWAPOBJECT_AXE][DIR::UP][MOTION::DAMAGE]))))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Death1"),
			TEXT("Com_Axe_Death1"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[SWAPOBJECT::SWAPOBJECT_AXE][DIR::SIDE][MOTION::DEATH1]))))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Death2"),
			TEXT("Com_Axe_Death2"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[SWAPOBJECT::SWAPOBJECT_AXE][DIR::SIDE][MOTION::DEATH2]))))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Dial"),
			TEXT("Com_Axe_Dial"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[SWAPOBJECT::SWAPOBJECT_AXE][DIR::DOWN][MOTION::DIAL]))))
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
	m_pVIBufferCom->SetUV(1, 1, 1, 0, 1);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
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

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pAnimController);
	Safe_Release(m_pAnimTransformCom);
	Safe_Release(m_pSwapObjectTransformCom);
	Safe_Release(m_pSwapObjectAnimController);
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
