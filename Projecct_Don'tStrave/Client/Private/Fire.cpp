#include "Fire.h"

#include "GameInstance.h"

#include "Slot.h"
#include "Inventory.h"
#include "Mouse.h"
#include "ITemState.h"
#include "Camera.h"

CFire::CFire(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CItem{ pGraphic_Device }
{
}

CFire::CFire(const CFire& Prototype)
	: CItem{ Prototype }
{
}

HRESULT CFire::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFire::Initialize(void* pArg)
{


	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pAnimController->ChangeState(m_pFireState_Com[m_iFireLevel]);

	m_pTransformCom->SetScale(_float3(1.f, 1.f, 1.f));
	m_vScale = _float3(1.f, 1.f, 1.f);
	_float3 vPos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
	m_pTransformCom->SetPosition(_float3(vPos.x, vPos.y, vPos.z));
	m_bEnableBillboard = true;

	Setting_Shader(L"BillBoard.fx");

	/*LightDesc.Type = D3DLIGHT_POINT;

	LightDesc.Diffuse = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.Ambient = D3DXCOLOR(0.8f, 0.8f, 1.f, 1.f);
	LightDesc.Position = _float3(vPos.x, vPos.y + 0.5f, vPos.z);
	LightDesc.Range = 5.f;
	LightDesc.Attenuation0 = 0.f;
	LightDesc.Attenuation2 = 0.3f;
	LightDesc.Attenuation2 = 0.8f;*/

	//m_pLight_Com->SetLightType(D3DLIGHT_POINT);

	m_Color = D3DXCOLOR(1.0f, 0.8f, 0.8f, 1.f);

	m_pLight_Com->SetDiffuseColor(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));
	m_pLight_Com->SetAmbientColor(m_Color);
	m_pLight_Com->SetPosition(vPos);
	m_pLight_Com->SetMaxRange(3.f);
	m_pLight_Com->SetAttenuation(0.f, 0.3f, 0.8f);

	//m_pGraphic_Device->SetLight(1, &m_Light);
	m_pGraphic_Device->LightEnable(1, true);
	m_pGraphic_Device->LightEnable(2, true);
	m_pGraphic_Device->LightEnable(3, true);
	m_pGraphic_Device->LightEnable(5, true);
	m_pGraphic_Device->LightEnable(4, true);

	m_pLight_Com->Render_Light();

	return S_OK;
}

void CFire::Priority_Update(_float fTimeDelta)
{
}

void CFire::Update(_float fTimeDelta)
{
	m_pAnimController->Tick(fTimeDelta);

	SetUp_OnTerrain(m_pTransformCom, 0.f);

}

void CFire::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CFire::Render()
{
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_World());
	class CGameObject* Obj = m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Camera"));
	auto Camera = dynamic_cast<CCamera*>(Obj);
	if (nullptr == Camera)
		return E_FAIL;


	LPDIRECT3DBASETEXTURE9 pTex = { nullptr };

	m_pGraphic_Device->GetTexture(0, &pTex);
	Excute_Billboard(Camera->GetInvViewMat(), pTex);

	m_pAnimController->Render();

	CLandObject::Render();

	m_pVIBuffer_Com->Render();
	End_Billboard();


	return S_OK;
}

HRESULT CFire::ADD_Components()
{

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"),
		reinterpret_cast<CComponent**>(&m_pVIBuffer_Com))))
		return E_FAIL;

	Engine::CTransform::TRANSFORM_DESC Transform_Desc = { 5.f, D3DXToRadian(90.f) };

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"),
		reinterpret_cast<CComponent**>(&m_pTransformCom), &Transform_Desc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_AnimController"),
		TEXT("Com_AnimController"), (CComponent**)&m_pAnimController)))
		return E_FAIL;
	
	CLightComponent::LIGHT_DESC Light_Desc = {};

	m_Light.Type = D3DLIGHT_POINT;

	m_Light.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_Light.Ambient = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.f);
	m_Light.Direction = _float3(0.f, -1.f, 0.f);

	Light_Desc.LightData = m_Light;
	Light_Desc.pOwner = this;

	// Transform Component
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Light"),
	    TEXT("Com_Light"),
	    reinterpret_cast<CComponent**>(&m_pLight_Com), &Light_Desc)))
	    return E_FAIL;

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_CampFire_Fire"),
		TEXT("Com_FireTexture"),
		reinterpret_cast<CComponent**>(&m_pTexture_Com))))
		return E_FAIL;

	CState::FRAME_DESC Desc = {};

	Desc.iStartFrame = 0;
	Desc.iEndFrame = 9;
	Desc.fTimeRate = 1.f;
	Desc.pAnimTexture = m_pTexture_Com;
	Desc.bIsLoop = true;

	m_pFireState_Com[0] = CItemState::Create(&Desc);

	Desc.iStartFrame = 0;
	Desc.iEndFrame = 9;
	Desc.fTimeRate = 0.5f;
	Desc.pAnimTexture = m_pTexture_Com;
	Desc.bIsLoop = true;

	m_pFireState_Com[1] = CItemState::Create(&Desc);

	Desc.iStartFrame = 0;
	Desc.iEndFrame = 9;
	Desc.fTimeRate = 0.5f;
	Desc.pAnimTexture = m_pTexture_Com;
	Desc.bIsLoop = true;

	m_pFireState_Com[2] = CItemState::Create(&Desc);

	return S_OK;
}

void CFire::Update_Fire(_float fValue)
{
	
}

CFire* CFire::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFire* pInstance = new CFire(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone CFire");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CFire::Clone(void* pArg)
{
	CGameObject* pInstance = new CFire(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CFire");
		return nullptr;
	}

	return pInstance;
}

void CFire::Free()
{
	__super::Free();

	Safe_Release(m_pAnimController);

	for (auto pState : m_pFireState_Com)
		Safe_Release(pState);
}
