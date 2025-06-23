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

	CPlayerAnim::PLAYER_DESC AnimDesc;
	AnimDesc.pTransformCom = m_pTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[0];
	AnimDesc.StartPoint = _float3(0.f, 0.f, 0.f);
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 137;
	AnimDesc.Frame.fTimeRate = 0.5f;

	m_pPlayerAnim[0] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pTransformCom = m_pTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[1];
	AnimDesc.StartPoint = _float3(0.f, 0.f, 0.f);
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 135;
	AnimDesc.Frame.fTimeRate = 0.5f;

	m_pPlayerAnim[1] = CPlayerAnim::Create(&AnimDesc);

	AnimDesc.pTransformCom = m_pTransformCom;
	AnimDesc.Frame.pAnimTexture = m_pTextureCom[2];
	AnimDesc.StartPoint = _float3(0.f, 0.f, 0.f);
	AnimDesc.Frame.bIsLoop = true;
	AnimDesc.Frame.iStartFrame = 0;
	AnimDesc.Frame.iEndFrame = 137;
	AnimDesc.Frame.fTimeRate = 0.5f;

	m_pPlayerAnim[2] = CPlayerAnim::Create(&AnimDesc);
	m_pAnimController->ChangeState(m_pPlayerAnim[0]);
	

	m_pSwapObjectTransformCom->SetPosition(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION));

	CSwapObject::SWAPOBEJCT_DESC SwapDesc;
	SwapDesc.pParentTransformCom = m_pTransformCom;
	SwapDesc.pTransformCom = m_pSwapObjectTransformCom;
	SwapDesc.Frame.pAnimTexture = m_pSwapObjectTextureCom[0];
	SwapDesc.StartPoint = _float3(0.22f, -0.14f, 0.f);
	SwapDesc.Frame.bIsLoop = true;
	SwapDesc.Frame.iStartFrame = 0;
	SwapDesc.Frame.iEndFrame = 49;
	SwapDesc.Frame.fTimeRate = 0.5f;

	m_pSwapObjectPlayerAnim[0] = CSwapObject::Create(&SwapDesc);
	m_pSwapObjectAnimController->ChangeState(m_pSwapObjectPlayerAnim[0]);
	
	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	m_pAnimController->Tick(fTimeDelta);
	m_pSwapObjectAnimController->Tick(fTimeDelta);
}

void CPlayer::Update(_float fTimeDelta)
{
	if (GetKeyState('W') & 0x8000)
	{
		_float3		vPosition = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
		_float3		vLook = m_pTransformCom->GetWorldState(WORLDSTATE::LOOK);

		vPosition += *D3DXVec3Normalize(&vLook, &vLook) * 1.5f * fTimeDelta;

		m_pTransformCom->SetPosition(vPosition);
	}
	if (GetKeyState('S') & 0x8000)
	{
		_float3		vPosition = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
		_float3		vLook = m_pTransformCom->GetWorldState(WORLDSTATE::LOOK);

		vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * 1.5f * fTimeDelta;

		m_pTransformCom->SetPosition(vPosition);
	}
	if (GetKeyState('A') & 0x8000)
	{
		_float3		vPosition = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
		_float3		vLook = m_pTransformCom->GetWorldState(WORLDSTATE::RIGHT);

		vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * 1.5f * fTimeDelta;

		m_pTransformCom->SetPosition(vPosition);
	}
	if (GetKeyState('D') & 0x8000)
	{
		_float3		vPosition = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
		_float3		vLook = m_pTransformCom->GetWorldState(WORLDSTATE::RIGHT);

		vPosition += *D3DXVec3Normalize(&vLook, &vLook) * 1.5f * fTimeDelta;

		m_pTransformCom->SetPosition(vPosition);
	}
	if (GetKeyState('E') & 0x8000)
	{
		m_pTransformCom->TurnRate(_float3(0.f, 1.f, 0.f), fTimeDelta);
	}
	if (GetKeyState('Q') & 0x8000)
	{
		m_pTransformCom->TurnRate(_float3(0.f, -1.f, 0.f), fTimeDelta);
	}
	if (GetKeyState('Z') & 0x8000)
	{
		m_pTransformCom->TurnRate(_float3(0.f, -1.f, 0.f), fTimeDelta);
	}
	if (GetKeyState('X') & 0x8000)
	{
		m_iMotion = 0;
		m_pAnimController->ChangeState(m_pPlayerAnim[m_iMotion]);
	}
	if (GetKeyState('Z') & 0x8000)
	{
		m_iMotion = 1;
		m_pAnimController->ChangeState(m_pPlayerAnim[m_iMotion]);
	}
	if (GetKeyState('C') & 0x8000)
	{
		m_iMotion = 2;
		m_pAnimController->ChangeState(m_pPlayerAnim[m_iMotion]);
	}

	SetUp_OnTerrain(m_pTransformCom, 0.5f);
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	

	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CPlayer::Render()
{
	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	m_pAnimController->Render();
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_World());
	m_pVIBufferCom->Render();
	m_pPlayerAnim[m_iMotion]->Render_End();


	m_pSwapObjectAnimController->Render();
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pSwapObjectTransformCom->Get_World());
	m_pSwapObjectVIBufferCom->Render();

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
		TEXT("Com_Idle_Loop_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom[0]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Idle_Loop_Side"),
		TEXT("Com_Idle_Loop_Side"), reinterpret_cast<CComponent**>(&m_pTextureCom[1]))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Idle_Loop_Up"),
		TEXT("Com_Idle_Loop_Up"), reinterpret_cast<CComponent**>(&m_pTextureCom[2]))))
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
	
		/* Com_VIBuffer */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
			TEXT("Com_VIBuffer2"), reinterpret_cast<CComponent**>(&m_pSwapObjectVIBufferCom))))
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
	Safe_Release(m_pSwapObjectTransformCom);
	Safe_Release(m_pSwapObjectVIBufferCom);
	Safe_Release(m_pSwapObjectAnimController);
	for (int i = 0; i < 100; ++i) {
		if (m_pTextureCom[i]) {
			Safe_Release(m_pTextureCom[i]);
		}
		if (m_pPlayerAnim[i]) {
			Safe_Release(m_pPlayerAnim[i]);
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
