#include "Terrain.h"

#include "GameInstance.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{

}

CTerrain::CTerrain(const CTerrain& Prototype)
	: CGameObject { Prototype }
{

}

HRESULT CTerrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrain::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	GAMEOBJECT_DESC* TeerrainDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->SetPosition(TeerrainDesc->vPosition);
	m_pTransformCom->SetScale(TeerrainDesc->vScale);
	m_pTransformCom->SetRotation(TeerrainDesc->vRotation);

	return S_OK;
}

void CTerrain::Priority_Update(_float fTimeDelta)
{
	
}

void CTerrain::Update(_float fTimeDelta)
{
	if (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		_float3		vPickPos{};
		if (m_pVIBufferCom->Picking(m_pTransformCom, &vPickPos))
			int a = 10;
	}
	
}


void CTerrain::Late_Update(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CTerrain::Render()
{
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CTerrain::Ready_Components()
{
	/* Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_Component_Texture_Terrain"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_Component_VIBuffer_Terrain"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;	

	return S_OK;
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrain* pInstance = new CTerrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTerrain::Clone(void* pArg)
{
	CTerrain* pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);

}
