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
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][0][0];
	AnimDesc.fSize = _float2(244.f, 391.f);
	AnimDesc.fPoint = _float3(-9.f, -4.f, 0.f);
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 137;
	AnimDesc.Frame.fTimeRate = AnimDesc.Frame.iEndFrame / 62.5f;

	m_pPlayerAnim[0][0][0] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][1][0];
	AnimDesc.fSize = _float2(235.f, 403.f);
	AnimDesc.fPoint = _float3(-43.f, -1.f, 0.f);
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 135;
	AnimDesc.Frame.fTimeRate = AnimDesc.Frame.iEndFrame / 62.5f;

	m_pPlayerAnim[0][1][0] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][2][0];
	AnimDesc.fSize = _float2(271.f, 398.f);
	AnimDesc.fPoint = _float3(0.f, -0.f, 0.f);
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 137;
	AnimDesc.Frame.fTimeRate = AnimDesc.Frame.iEndFrame / 62.5f;

	m_pPlayerAnim[0][2][0] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[1][0][0];
	AnimDesc.fSize = _float2(263.f, 391.f);
	AnimDesc.fPoint = _float3(0.f, -13.f, 0.f);
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 0;//137;
	AnimDesc.Frame.fTimeRate = 0.5f;

	m_pPlayerAnim[1][0][0] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[1][1][0];
	AnimDesc.fSize = _float2(284.f, 403.f);
	AnimDesc.fPoint = _float3(-18.f, -9.f, 0.f);
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 0;//137;
	AnimDesc.Frame.fTimeRate = 0.5f;

	m_pPlayerAnim[1][1][0] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[1][2][0];
	AnimDesc.fSize = _float2(271.f, 398.f);
	AnimDesc.fPoint = _float3(-1.5f, -7.f, 0.f);
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 0;//137;
	AnimDesc.Frame.fTimeRate = 0.5f;

	m_pPlayerAnim[1][2][0] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][0][1];
	AnimDesc.fSize = _float2(237.f, 394.f);
	AnimDesc.fPoint = _float3(-9.f, -9.f, 0.f);
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 8;
	AnimDesc.Frame.fTimeRate = AnimDesc.Frame.iEndFrame / 62.5f;

	m_pPlayerAnim[0][0][1] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][1][1];
	AnimDesc.fSize = _float2(246.f, 401.f);
	AnimDesc.fPoint = _float3(-29.f, -2.f, 0.f);
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 8;
	AnimDesc.Frame.fTimeRate = AnimDesc.Frame.iEndFrame / 62.5f;

	m_pPlayerAnim[0][1][1] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][2][1];
	AnimDesc.fSize = _float2(263.f, 406.f);
	AnimDesc.fPoint = _float3(-1.f, -4.f, 0.f);
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 8;
	AnimDesc.Frame.fTimeRate = AnimDesc.Frame.iEndFrame / 62.5f;

	m_pPlayerAnim[0][2][1] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][0][2];
	AnimDesc.fSize = _float2(264.f, 467.f);
	AnimDesc.fPoint = _float3(-6.f, 19.f, 0.f);
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 33;
	AnimDesc.Frame.fTimeRate = AnimDesc.Frame.iEndFrame / 62.5f;

	m_pPlayerAnim[0][0][2] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][1][2];
	AnimDesc.fSize = _float2(295.f, 475.f);
	AnimDesc.fPoint = _float3(-10.f, 21.f, 0.f);
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 33;
	AnimDesc.Frame.fTimeRate = AnimDesc.Frame.iEndFrame / 62.5f;

	m_pPlayerAnim[0][1][2] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][2][2];
	AnimDesc.fSize = _float2(279.f, 481.f);
	AnimDesc.fPoint = _float3(0.f, 18.f, 0.f);
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 33;
	AnimDesc.Frame.fTimeRate = AnimDesc.Frame.iEndFrame / 62.5f;

	m_pPlayerAnim[0][2][2] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][0][3];
	AnimDesc.fSize = _float2(261.f, 391.f);
	AnimDesc.fPoint = _float3(-7.f, -7.f, 0.f);
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 16;
	AnimDesc.Frame.fTimeRate = AnimDesc.Frame.iEndFrame / 62.5f;
	m_pPlayerAnim[0][0][3] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][1][3];
	AnimDesc.fSize = _float2(274.f, 410.f);
	AnimDesc.fPoint = _float3(-30.f, -7.f, 0.f);
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 16;
	AnimDesc.Frame.fTimeRate = AnimDesc.Frame.iEndFrame / 62.5f;
	m_pPlayerAnim[0][1][3] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pAnimTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0][2][3];
	AnimDesc.fSize = _float2(282.f, 412.f);
	AnimDesc.fPoint = _float3(0.f, -7.f, 0.f);
	AnimDesc.Frame.bIsLoop = false;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 16;
	AnimDesc.Frame.fTimeRate = AnimDesc.Frame.iEndFrame / 62.5f;
	m_pPlayerAnim[0][2][3] = CPlayerAnim::Create(&AnimDesc);

	m_pAnimController->ChangeState(m_pPlayerAnim[0][0][0]);
	

	m_pSwapObjectTransformCom->SetPosition(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION));

	AnimDesc.pParentTransformCom = m_pTransformCom;
	AnimDesc.pTransformCom = m_pSwapObjectTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pSwapObjectTextureCom[0];
	AnimDesc.fSize = _float2(428.f, 594.f);
	AnimDesc.fPoint = _float3(0.22f, -0.14f, 0.f);
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 0;//49;
	AnimDesc.Frame.fTimeRate = 0.5f;

	m_pSwapObjectPlayerAnim[0] = CPlayerAnim::Create(&AnimDesc);
	m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[0]);
	
	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
}

void CPlayer::Update(_float fTimeDelta)
{
	if (GetKeyState('W') & 0x8000 || GetKeyState('S') & 0x8000 || GetKeyState('D') & 0x8000 || GetKeyState('A') & 0x8000)
	{
		switch (m_iMotion)
		{
		case 1:
			if (m_pPlayerAnim[0][m_iDirection][m_iMotion]->IsEnd())
				m_iMotion++;
		case 2:
			break;
		default:
			m_iMotion = 1;
			break;
		}
		if (GetKeyState('W') & 0x8000) {
			_float3		vPosition = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
			_float3		vLook = m_pTransformCom->GetWorldState(WORLDSTATE::LOOK);

			vPosition += *D3DXVec3Normalize(&vLook, &vLook) * 1.5f * fTimeDelta;

			m_pTransformCom->SetPosition(vPosition);
			m_iDirection = 2;
		}
		if (GetKeyState('S') & 0x8000)
		{
			_float3		vPosition = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
			_float3		vLook = m_pTransformCom->GetWorldState(WORLDSTATE::LOOK);

			vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * 1.5f * fTimeDelta;

			m_pTransformCom->SetPosition(vPosition);
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
		m_pAnimController->ChangeState(m_pPlayerAnim[0][m_iDirection][m_iMotion]);
	}
	else {
		switch (m_iMotion)
		{
		case 2:
			m_iMotion++;
			m_pAnimController->ChangeState(m_pPlayerAnim[0][m_iDirection][m_iMotion]);
			break;
		case 3:
			if (m_pPlayerAnim[0][m_iDirection][m_iMotion]->IsEnd())
			{
				m_iMotion = 0;
				m_pAnimController->ChangeState(m_pPlayerAnim[0][m_iDirection][m_iMotion]);
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
	if (GetKeyState('K') & 0x8000)
	{
		m_iMotion = 0;
		m_pAnimController->ChangeState(m_pPlayerAnim[0][m_iDirection][m_iMotion]);
	}
	if (GetKeyState('L') & 0x8000)
	{
		m_iMotion = 1;
		m_pAnimController->ChangeState(m_pPlayerAnim[0][m_iDirection][m_iMotion]);
	}
	if (GetKeyState('I') & 0x8000)
	{
		m_iMotion = 2;
		m_pAnimController->ChangeState(m_pPlayerAnim[0][m_iDirection][m_iMotion]);
	}
	if (GetKeyState('Z') & 0x8000)
	{
		m_iMotion = 3;
		m_pAnimController->ChangeState(m_pPlayerAnim[0][m_iDirection][m_iMotion]);
	}
	if (GetKeyState('X') & 0x8000)
	{
		//m_iMotion = 4;
		m_pAnimController->ChangeState(m_pPlayerAnim[0][m_iDirection][m_iMotion]);
	}
	if (GetKeyState('C') & 0x8000)
	{
		m_iMotion = 5;
		m_pAnimController->ChangeState(m_pPlayerAnim[0][m_iDirection][m_iMotion]);
	}

	SetUp_OnTerrain(m_pTransformCom, 0.5f);
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
		TEXT("Com_Idle_Loop_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][0][0]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Idle_Loop_Side"),
		TEXT("Com_Idle_Loop_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][1][0]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Idle_Loop_Up"),
		TEXT("Com_Idle_Loop_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][2][0]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pre_Down"),
		TEXT("Com_Run_Pre_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][0][1]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pre_Side"),
		TEXT("Com_Run_Pre_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][1][1]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pre_Up"),
		TEXT("Com_Run_Pre_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][2][1]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Loop_Down"),
		TEXT("Com_Run_Loop_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][0][2]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Loop_Side"),
		TEXT("Com_Run_Loop_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][1][2]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Loop_Up"),
		TEXT("Com_Run_Loop_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][2][2]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pst_Down"),
		TEXT("Com_Run_Pst_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][0][3]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pst_Side"),
		TEXT("Com_Run_Pst_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][1][3]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pst_Up"),
		TEXT("Com_Run_Pst_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[0][2][3]))))
		return E_FAIL;




	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Idle_Loop_Down"),
		TEXT("Com_Item_Idle_Loop_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][0][0]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Idle_Loop_Side"),
		TEXT("Com_Item_Idle_Loop_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][1][0]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Idle_Loop_Up"),
		TEXT("Com_Item_Idle_Loop_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[1][2][0]))))
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Axe_Atk_Down"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pSwapObjectTextureCom[0]))))
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



	return S_OK;
}

HRESULT CPlayer::End_RenderState()
{
	// m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

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
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < 100; ++k) {
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
		if (m_pSwapObjectTextureCom[i]) {
			Safe_Release(m_pSwapObjectTextureCom[i]);
		}
		if (m_pSwapObjectPlayerAnim[i]) {
			Safe_Release(m_pSwapObjectPlayerAnim[i]);
		}
	}
}
