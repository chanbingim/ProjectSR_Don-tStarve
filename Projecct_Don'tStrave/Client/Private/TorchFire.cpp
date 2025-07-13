#include "TorchFire.h"

#include "GameInstance.h"
#include "XML_Manager.h"

#include "Slot.h"
#include "Inventory.h"
#include "Mouse.h"
#include "ITemState.h"
#include "Camera.h"
#include "AnimController.h"
#include "Texture.h"

CTorchFire::CTorchFire(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLandObject{ pGraphic_Device }
{
}

CTorchFire::CTorchFire(const CTorchFire& Prototype)
	: CLandObject{ Prototype }
{
}

HRESULT CTorchFire::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTorchFire::Initialize(void* pArg)
{
	m_fDurability = 100.f;
	if (FAILED(Ready_Components()))
		return E_FAIL;


	CLandObject::LANDOBJECT_DESC			Desc{};
		Desc.pLandTransform = static_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"), TEXT("Com_Transform")));
		Desc.pLandVIBuffer = static_cast<CVIBuffer*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"), TEXT("Com_VIBuffer")));


	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;
	Setting_Shader(L"FireParticle.fx");
	m_fAniTime = 0;
	m_fTime = 0.f;
	return S_OK;
}

HRESULT CTorchFire::Initialize_Late()
{

	CLightComponent::LIGHT_DESC Light_Desc = {};

	m_Light.Type = D3DLIGHT_POINT;

	m_Light.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_Light.Ambient = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.f);

	Light_Desc.LightData = m_Light;
	Light_Desc.pOwner = this;
	Light_Desc.PlayerPoint = m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player"))->GetTransfrom();

	// Light Component
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Light"),
		TEXT("Com_Light"),
		reinterpret_cast<CComponent**>(&m_pLight_Com), &Light_Desc)))
		return E_FAIL;



	m_pLight_Com->SetLightType(D3DLIGHT_POINT);

	m_Color = D3DXCOLOR(1.0f, 0.8f, 0.8f, 1.f);
	_float3 pos = { 9.64023876f, 0.f, 5.869989f };
	m_pTransformCom->SetPosition(pos);
	_float3	vPos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
	m_pLight_Com->SetDiffuseColor(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));
	m_pLight_Com->SetAmbientColor(D3DXCOLOR(0.f, 0.f, 0.f, 0.f));
	m_pLight_Com->SetPosition(vPos);
	m_pLight_Com->SetMaxRange(3.3f);
	m_pLight_Com->SetAttenuation(0.f, 0.3f, 0.8f);
	m_bLight = true;
	Update_TorchFire(false);
	return S_OK;
}

void CTorchFire::Priority_Update(_float fTimeDelta)
{
	m_fAniTime += fTimeDelta * 10;
	if (m_fAniTime >= 4) {
		m_fAniTime = 0;
	}
	m_fTime += fTimeDelta * 2;
}

void CTorchFire::Update(_float fTimeDelta)
{
	_float fValue = m_fDurability * 0.01f;
	m_pLight_Com->SetAmbientColor(D3DXCOLOR(m_Color.r * fValue, m_Color.g * fValue, m_Color.b * fValue, m_Color.b));
}

void CTorchFire::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CTorchFire::Render(D3DXMATRIX mat)
{
	_float3 pos = {};
	pos.x = mat._41;
	pos.y = mat._42;
	pos.z = mat._43;
	m_pTransformCom->SetPosition(pos);
	m_pLight_Com->SetPosition(pos);
	m_pGraphic_Device->CreateVertexDeclaration(decl, &m_pDecl);
	LPDIRECT3DBASETEXTURE9 BackTex = {};
	LPDIRECT3DBASETEXTURE9 Tex = {};
	LPDIRECT3DBASETEXTURE9 EffectTex = {};

	m_pFireTex->Set_Texture(0);
	m_pGraphic_Device->GetTexture(0, &BackTex);
	m_pAlphaTex->Set_Texture((_int)m_fAniTime);
	m_pGraphic_Device->GetTexture(0, &Tex);

	m_pEffectFire->Set_Texture(0);
	m_pGraphic_Device->GetTexture(0, &EffectTex);

	m_pEffect->SetFloat("fTime", m_fTime);

	m_pEffect->SetTexture("TexSrc", BackTex);
	m_pEffect->SetTexture("TexDst", Tex);
	m_pEffect->SetTexture("TexArg", EffectTex);

	m_pGraphic_Device->SetVertexDeclaration(m_pDecl);
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	m_pVIBufferCom->Render();

	m_pEffect->EndPass();
	m_pEffect->End();

	Safe_Release(Tex);
	Safe_Release(BackTex);


	return S_OK;
}

void CTorchFire::Update_TorchFire(_bool bFire)
{
	if (m_bLight != bFire) {
		m_bLight = bFire;
		m_pLight_Com->SetLight(bFire);
	}
}

HRESULT CTorchFire::Ready_Components()
{

	/* Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TorchFire"),
		TEXT("Com_Fire"), reinterpret_cast<CComponent**>(&m_pFireTex))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TorchFireAlpha"),
		TEXT("Com_FireAlpha"), reinterpret_cast<CComponent**>(&m_pAlphaTex))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_FireEffect"),
		TEXT("Com_FireEffect"), reinterpret_cast<CComponent**>(&m_pEffectFire))))
		return E_FAIL;

	return S_OK;
}

CTorchFire* CTorchFire::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTorchFire* pInstance = new CTorchFire(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone CTorchFire");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CTorchFire::Clone(void* pArg)
{
	CGameObject* pInstance = new CTorchFire(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CTorchFire");
		return nullptr;
	}

	return pInstance;
}

void CTorchFire::Free()
{
	__super::Free();

	Safe_Release(m_pLight_Com);
	Safe_Release(m_pAlphaTex);
	Safe_Release(m_pFireTex);
	Safe_Release(m_pEffectFire);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
}
