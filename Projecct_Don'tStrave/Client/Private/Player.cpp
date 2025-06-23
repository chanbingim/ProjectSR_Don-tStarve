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

	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& _Dir) { BeginHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& _Dir) { OverlapHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& _Dir) { EndHitActor(HitActor, _Dir); });

	m_pCamera = *(m_pGameInstance->GetAllObejctsToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Camera"))->begin());
	Safe_AddRef(m_pCamera);
	
	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	

}

void CPlayer::Update(_float fTimeDelta)
{
	if (GetKeyState(VK_UP) & 0x8000)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		m_pTransformCom->Go_Backward(-fTimeDelta);
	}
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		m_pTransformCom->TurnRate(_float3(0.f, 1.f, 0.f), -fTimeDelta);
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		m_pTransformCom->TurnRate(_float3(0.f, 1.f, 0.f), fTimeDelta);
	}

	SetUp_OnTerrain(m_pTransformCom, 0.5f);
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CPlayer::Render()
{
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(0);

	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();
	

	if (FAILED(End_RenderState()))
		return E_FAIL;
	m_pCollision_Com->Render();
	return S_OK;
}

void CPlayer::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
	int a = 10;
}

void CPlayer::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
	int a = 10;
}

void CPlayer::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
	int a = 10;
}

HRESULT CPlayer::Ready_Components()
{
	/* Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;	

	/* Com_Collision */
	CBox_Collision_Component::COL_DESC Col_Desc = {};
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

	Safe_Release(m_pCamera);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pCollision_Com);
}
