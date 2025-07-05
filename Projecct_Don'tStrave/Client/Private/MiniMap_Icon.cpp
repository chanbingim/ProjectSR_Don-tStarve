#include "MiniMap_Icon.h"

#include "GameInstance.h"

CMiniMap_Icon::CMiniMap_Icon(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUserInterface{ pGraphic_Device }
{
}

CMiniMap_Icon::CMiniMap_Icon(const CMiniMap_Icon& Prototype)
	: CUserInterface{ Prototype }
{
}

HRESULT CMiniMap_Icon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMiniMap_Icon::Initialize(void* pArg)
{
	CMiniMap_Icon::ICON_DESC* pIcon_Desc = static_cast<ICON_DESC*>(pArg);

	m_fRX = pIcon_Desc->fRX;
	m_fRY = pIcon_Desc->fRY;

	m_iTextureNum = pIcon_Desc->iTextureNum;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	m_pTransform_Com->SetPosition(_float3(m_fRX * pIcon_Desc->fParentCX, m_fRY * pIcon_Desc->fParentCY, 0.f));

	__super::UpdatePosition();


	return S_OK;
}

void CMiniMap_Icon::Priority_Update(_float fTimeDelta)
{

}

void CMiniMap_Icon::Update(_float fTimeDelta)
{

}
void CMiniMap_Icon::Late_Update(_float fTimeDelta)
{
}

HRESULT CMiniMap_Icon::Render()
{
	m_pTexture_Com->Set_Texture(m_iTextureNum);

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

	m_pVIBuffer_Com->Render();

	return S_OK;
}

void CMiniMap_Icon::Update_Position(CTransform* pTransform)
{
	_float3 vPosition = pTransform->GetWorldState(WORLDSTATE::POSITION);
	_float3 vScale = pTransform->GetScale();

	m_pTransform_Com->SetPosition(_float3(vPosition.x + m_fRX * vScale.x, vPosition.y + m_fRY * vScale.y, 0.f));
}

void CMiniMap_Icon::Update_Scale(_float fScale, CTransform* pTransform)
{
	_float3 vPosition = pTransform->GetWorldState(WORLDSTATE::POSITION);
	_float3 vScale = pTransform->GetScale();

	m_pTransform_Com->SetScale(_float3(m_fSizeX * fScale, m_fSizeY * fScale, 1.f));
	m_pTransform_Com->SetPosition(_float3(vPosition.x + m_fRX * vScale.x, vPosition.y + m_fRY * vScale.y, 0.f));
	
}

HRESULT CMiniMap_Icon::ADD_Components()
{
	// Texture Component
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_MiniMap_Icon"),
		TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTexture_Com))))
		return E_FAIL;


	// VIBuffer_Rect Component
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"),
		reinterpret_cast<CComponent**>(&m_pVIBuffer_Com))))
		return E_FAIL;

	Engine::CTransform::TRANSFORM_DESC Transform_Desc = { 5.f, D3DXToRadian(90.f) };

	// Transform Component
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"),
		reinterpret_cast<CComponent**>(&m_pTransform_Com), &Transform_Desc)))
		return E_FAIL;

	return S_OK;
}

CMiniMap_Icon* CMiniMap_Icon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMiniMap_Icon* pInstance = new CMiniMap_Icon(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMiniMap_Icon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMiniMap_Icon::Clone(void* pArg)
{
	CGameObject* pInstance = new CMiniMap_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CMiniMap_Icon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMiniMap_Icon::Free()
{
	__super::Free();

	Safe_Release(m_pTexture_Com);
	Safe_Release(m_pTransform_Com);
	Safe_Release(m_pVIBuffer_Com);
}
