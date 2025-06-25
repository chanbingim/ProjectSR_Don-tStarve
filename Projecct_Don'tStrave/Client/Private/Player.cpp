#include "Player.h"
#include "GameInstance.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLandObject{ pGraphic_Device }
{

}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CLandObject{ Prototype }
{

}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	CLandObject::LANDOBJECT_DESC			Desc{};
	Desc.pLandTransform = static_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"), TEXT("Com_Transform")));
	Desc.pLandVIBuffer = static_cast<CVIBuffer*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"), TEXT("Com_VIBuffer")));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->SetPosition(_float3(rand() % 20, 0.f, rand() % 20));
	m_pAnimTransformCom->SetPosition(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION));
	
	CPlayerAnim::PLAYER_DESC AnimDesc;
	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][DIR::DOWN][MOTION::IDLE];
	AnimDesc.pVIBufferCom = m_pVIBufferCom;
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 137;
	

	m_pPlayerAnim[0][DIR::DOWN][MOTION::IDLE] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][DIR::SIDE][MOTION::IDLE];
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 135;
	

	m_pPlayerAnim[0][DIR::SIDE][MOTION::IDLE] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][DIR::UP][MOTION::IDLE];
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 137;
	

	m_pPlayerAnim[0][DIR::UP][MOTION::IDLE] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][DIR::DOWN][MOTION::IDLE_TO_RUN];
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 8;
	

	m_pPlayerAnim[0][DIR::DOWN][MOTION::IDLE_TO_RUN] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][DIR::SIDE][MOTION::IDLE_TO_RUN];
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 8;
	

	m_pPlayerAnim[0][DIR::SIDE][MOTION::IDLE_TO_RUN] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][DIR::UP][MOTION::IDLE_TO_RUN];
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 8;
	

	m_pPlayerAnim[0][DIR::UP][MOTION::IDLE_TO_RUN] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][DIR::DOWN][MOTION::RUN];
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 33;
	

	m_pPlayerAnim[0][DIR::DOWN][MOTION::RUN] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][DIR::SIDE][MOTION::RUN];
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 33;
	

	m_pPlayerAnim[0][DIR::SIDE][MOTION::RUN] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][DIR::UP][MOTION::RUN];
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 33;

	m_pPlayerAnim[0][DIR::UP][MOTION::RUN] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][DIR::DOWN][MOTION::RUN_TO_IDLE];
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 16;
	m_pPlayerAnim[0][DIR::DOWN][MOTION::RUN_TO_IDLE] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][DIR::SIDE][MOTION::RUN_TO_IDLE];
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 16;
	m_pPlayerAnim[0][DIR::SIDE][MOTION::RUN_TO_IDLE] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][DIR::UP][MOTION::RUN_TO_IDLE];
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 16;
	
	m_pPlayerAnim[0][DIR::UP][MOTION::RUN_TO_IDLE] = CPlayerAnim::Create(&AnimDesc);





	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[1][DIR::DOWN][MOTION::IDLE];
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 137;

	m_pPlayerAnim[1][DIR::DOWN][MOTION::IDLE] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[1][DIR::SIDE][MOTION::IDLE];
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 135;

	m_pPlayerAnim[1][DIR::SIDE][MOTION::IDLE] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[1][DIR::UP][MOTION::IDLE];
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 137;

	m_pPlayerAnim[1][DIR::UP][MOTION::IDLE] = CPlayerAnim::Create(&AnimDesc);



	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[1][DIR::DOWN][MOTION::IDLE_TO_RUN];
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 8;
	

	m_pPlayerAnim[1][DIR::DOWN][MOTION::IDLE_TO_RUN] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[1][DIR::SIDE][MOTION::IDLE_TO_RUN];
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 8;
	

	m_pPlayerAnim[1][DIR::SIDE][MOTION::IDLE_TO_RUN] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[1][DIR::UP][MOTION::IDLE_TO_RUN];
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 8;
	

	m_pPlayerAnim[1][DIR::UP][MOTION::IDLE_TO_RUN] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[1][DIR::DOWN][MOTION::RUN];
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 33;
	

	m_pPlayerAnim[1][DIR::DOWN][MOTION::RUN] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[1][DIR::SIDE][MOTION::RUN];
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 33;
	

	m_pPlayerAnim[1][DIR::SIDE][MOTION::RUN] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[1][DIR::UP][MOTION::RUN];
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 33;
	

	m_pPlayerAnim[1][DIR::UP][MOTION::RUN] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[1][DIR::DOWN][MOTION::RUN_TO_IDLE];
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 16;
	
	m_pPlayerAnim[1][DIR::DOWN][MOTION::RUN_TO_IDLE] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[1][DIR::SIDE][MOTION::RUN_TO_IDLE];
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 16;
	
	m_pPlayerAnim[1][DIR::SIDE][MOTION::RUN_TO_IDLE] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[1][DIR::UP][MOTION::RUN_TO_IDLE];
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 16;
	
	m_pPlayerAnim[1][DIR::UP][MOTION::RUN_TO_IDLE] = CPlayerAnim::Create(&AnimDesc);








	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pSwapObjectTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pSwapObjectTextureCom[0][DIR::DOWN][MOTION::IDLE];
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 137;
	

	m_pSwapObjectPlayerAnim[0][DIR::DOWN][MOTION::IDLE] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pSwapObjectTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pSwapObjectTextureCom[0][DIR::SIDE][MOTION::IDLE];
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 135;
	

	m_pSwapObjectPlayerAnim[0][DIR::SIDE][MOTION::IDLE] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pSwapObjectTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pSwapObjectTextureCom[0][DIR::UP][MOTION::IDLE];
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 137;
	AnimDesc.Frame.fTimeRate = 0.5f;

	m_pSwapObjectPlayerAnim[0][DIR::UP][MOTION::IDLE] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pSwapObjectTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pSwapObjectTextureCom[0][DIR::DOWN][MOTION::IDLE_TO_RUN];
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 8;
	

	m_pSwapObjectPlayerAnim[0][DIR::DOWN][MOTION::IDLE_TO_RUN] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pSwapObjectTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pSwapObjectTextureCom[0][DIR::SIDE][MOTION::IDLE_TO_RUN];
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 8;
	

	m_pSwapObjectPlayerAnim[0][DIR::SIDE][MOTION::IDLE_TO_RUN] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pSwapObjectTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pSwapObjectTextureCom[0][DIR::UP][MOTION::IDLE_TO_RUN];
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 8;
	

	m_pSwapObjectPlayerAnim[0][DIR::UP][MOTION::IDLE_TO_RUN] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pSwapObjectTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pSwapObjectTextureCom[0][DIR::DOWN][MOTION::RUN];
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 33;
	

	m_pSwapObjectPlayerAnim[0][DIR::DOWN][MOTION::RUN] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pSwapObjectTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pSwapObjectTextureCom[0][DIR::SIDE][MOTION::RUN];
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 33;
	

	m_pSwapObjectPlayerAnim[0][DIR::SIDE][MOTION::RUN] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pSwapObjectTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pSwapObjectTextureCom[0][DIR::UP][MOTION::RUN];
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 33;
	

	m_pSwapObjectPlayerAnim[0][DIR::UP][MOTION::RUN] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pSwapObjectTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pSwapObjectTextureCom[0][DIR::DOWN][MOTION::RUN_TO_IDLE];
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 16;
	
	m_pSwapObjectPlayerAnim[0][DIR::DOWN][MOTION::RUN_TO_IDLE] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pSwapObjectTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pSwapObjectTextureCom[0][DIR::SIDE][MOTION::RUN_TO_IDLE];
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 16;
	
	m_pSwapObjectPlayerAnim[0][DIR::SIDE][MOTION::RUN_TO_IDLE] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pSwapObjectTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pSwapObjectTextureCom[0][DIR::UP][MOTION::RUN_TO_IDLE];
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 16;
	
	m_pSwapObjectPlayerAnim[0][DIR::UP][MOTION::RUN_TO_IDLE] = CPlayerAnim::Create(&AnimDesc);



	m_iSwapObject = 1;
	m_pAnimController->ChangeState(m_pPlayerAnim[1][DIR::DOWN][MOTION::IDLE]);

	m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[0][DIR::DOWN][MOTION::IDLE]);




	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
}

void CPlayer::Update(_float fTimeDelta)
{
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

		m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[0][m_iDirection][m_tMotion]);
		m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_iDirection][m_tMotion]);
	}
	else {
		switch (m_tMotion)
		{
		case  MOTION::IDLE_TO_RUN:
		case  MOTION::RUN:
			if (m_pPlayerAnim[m_iSwapObject][m_iDirection][m_tMotion]->IsEnd())
			{
				m_tMotion = MOTION::RUN_TO_IDLE;

				m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[0][m_iDirection][m_tMotion]);
				m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_iDirection][m_tMotion]);
			}
			break;
		case MOTION::RUN_TO_IDLE:
			if (m_pPlayerAnim[m_iSwapObject][m_iDirection][m_tMotion]->IsEnd())
			{
				m_tMotion = MOTION::IDLE;
				m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[0][m_iDirection][m_tMotion]);
				m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_iDirection][m_tMotion]);
			}
			break;
		default:
			break;
		}
	}
	if (GetKeyState('E') & 0x8000)
	{
		m_pTransformCom->TurnRate(_float3(0.f, 1.f, 0.f), fTimeDelta);
		m_pAnimTransformCom->TurnRate(_float3(0.f, 1.f, 0.f), fTimeDelta);
		m_pSwapObjectTransformCom->TurnRate(_float3(0.f, 1.f, 0.f), fTimeDelta);
	}
	if (GetKeyState('Q') & 0x8000)
	{
		m_pTransformCom->TurnRate(_float3(0.f, -1.f, 0.f), fTimeDelta);
		m_pAnimTransformCom->TurnRate(_float3(0.f, -1.f, 0.f), fTimeDelta);
		m_pSwapObjectTransformCom->TurnRate(_float3(0.f, -1.f, 0.f), fTimeDelta);
	}
	if (GetKeyState('Z') & 0x8000)
	{
		m_iSwapObject = 0;
		m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_iDirection][m_tMotion]);
	}
	if (GetKeyState('X') & 0x8000)
	{
		m_iSwapObject = 1;
		m_pAnimController->ChangeState(m_pPlayerAnim[m_iSwapObject][m_iDirection][m_tMotion]);
	}

	SetUp_OnTerrain(m_pTransformCom, 0.f);
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	m_pAnimController->Tick(fTimeDelta);
	m_pSwapObjectAnimController->Tick(fTimeDelta);
	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CPlayer::Render()
{
	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	m_pAnimController->Render();
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pAnimTransformCom->Get_World());
	m_pVIBufferCom->Render();

	m_pSwapObjectAnimController->Render();
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pSwapObjectTransformCom->Get_World());
	m_pVIBufferCom->Render();

	if (FAILED(End_RenderState()))
		return E_FAIL;

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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Idle_Loop_Down"),
		TEXT("Com_Idle_Loop_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::DOWN][0]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Idle_Loop_Side"),
		TEXT("Com_Idle_Loop_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::SIDE][0]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Idle_Loop_Up"),
		TEXT("Com_Idle_Loop_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::UP][0]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pre_Down"),
		TEXT("Com_Run_Pre_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::DOWN][1]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pre_Side"),
		TEXT("Com_Run_Pre_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::SIDE][1]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pre_Up"),
		TEXT("Com_Run_Pre_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::UP][1]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Loop_Down"),
		TEXT("Com_Run_Loop_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::DOWN][2]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Loop_Side"),
		TEXT("Com_Run_Loop_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::SIDE][2]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Loop_Up"),
		TEXT("Com_Run_Loop_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::UP][2]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pst_Down"),
		TEXT("Com_Run_Pst_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::DOWN][3]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pst_Side"),
		TEXT("Com_Run_Pst_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::SIDE][3]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pst_Up"),
		TEXT("Com_Run_Pst_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][DIR::UP][3]))))
		return E_FAIL;




	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Idle_Loop_Down"),
		TEXT("Com_Item_Idle_Loop_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::DOWN][0]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Idle_Loop_Side"),
		TEXT("Com_Item_Idle_Loop_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::SIDE][0]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Idle_Loop_Up"),
		TEXT("Com_Item_Idle_Loop_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::UP][0]))))
		return E_FAIL;


	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Pre_Down"),
		TEXT("Com_Item_Run_Pre_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::DOWN][1]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Pre_Side"),
		TEXT("Com_Item_Run_Pre_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::SIDE][1]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Pre_Up"),
		TEXT("Com_Item_Run_Pre_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::UP][1]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Loop_Down"),
		TEXT("Com_Item_Run_Loop_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::DOWN][2]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Loop_Side"),
		TEXT("Com_Item_Run_Loop_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::SIDE][2]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Loop_Up"),
		TEXT("Com_Item_Run_Loop_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::UP][2]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Pst_Down"),
		TEXT("Com_Item_Run_Pst_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::DOWN][3]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Pst_Side"),
		TEXT("Com_Item_Run_Pst_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::SIDE][3]))))
		return E_FAIL;

	/* Com_Item_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Pst_Up"),
		TEXT("Com_Item_Run_Pst_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][DIR::UP][3]))))
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
			TEXT("Com_Axe_Idle_Loop_Down"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[0][DIR::DOWN][0]))))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Idle_Loop_Side"),
			TEXT("Com_Axe_Idle_Loop_Side"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[0][DIR::SIDE][0]))))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Idle_Loop_Up"),
			TEXT("Com_Axe_Idle_Loop_Up"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[0][DIR::UP][0]))))
			return E_FAIL;


		/* Com_Axe_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Run_Pre_Down"),
			TEXT("Com_Axe_Run_Pre_Down"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[0][DIR::DOWN][1]))))
			return E_FAIL;

		/* Com_Axe_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Run_Pre_Side"),
			TEXT("Com_Axe_Run_Pre_Side"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[0][DIR::SIDE][1]))))
			return E_FAIL;

		/* Com_Axe_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Run_Pre_Up"),
			TEXT("Com_Axe_Run_Pre_Up"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[0][DIR::UP][1]))))
			return E_FAIL;

		/* Com_Axe_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Run_Loop_Down"),
			TEXT("Com_Axe_Run_Loop_Down"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[0][DIR::DOWN][2]))))
			return E_FAIL;

		/* Com_Axe_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Run_Loop_Side"),
			TEXT("Com_Axe_Run_Loop_Side"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[0][DIR::SIDE][2]))))
			return E_FAIL;

		/* Com_Axe_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Run_Loop_Up"),
			TEXT("Com_Axe_Run_Loop_Up"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[0][DIR::UP][2]))))
			return E_FAIL;

		/* Com_Axe_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Run_Pst_Down"),
			TEXT("Com_Axe_Run_Pst_Down"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[0][DIR::DOWN][3]))))
			return E_FAIL;

		/* Com_Axe_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Run_Pst_Side"),
			TEXT("Com_Axe_Run_Pst_Side"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[0][DIR::SIDE][3]))))
			return E_FAIL;

		/* Com_Axe_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Run_Pst_Up"),
			TEXT("Com_Axe_Run_Pst_Up"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[0][DIR::UP][3]))))
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
	for (int i = 0; i < 10; ++i) {
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
