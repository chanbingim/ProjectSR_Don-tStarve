#include "GameObject.h"
#include "GameInstance.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device { pGraphic_Device }
	, m_pGameInstance { CGameInstance::GetInstance() }
{
	D3DXMatrixIdentity(&m_BillboardMat);
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);
}

CGameObject::CGameObject(const CGameObject& Prototype)
	: m_pGraphic_Device{ Prototype.m_pGraphic_Device }
	, m_pGameInstance{ Prototype.m_pGameInstance }
	, m_isDead { Prototype.m_isDead }
	, m_BillboardMat { Prototype.m_BillboardMat }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	

	return S_OK;
}

HRESULT CGameObject::Initialize_Late()
{
	return S_OK;
}

void CGameObject::Priority_Update(_float fTimeDelta)
{
}

void CGameObject::Update(_float fTimeDelta)
{

}

void CGameObject::Late_Update(_float fTimeDelta)
{

}

HRESULT CGameObject::Render()
{

	return S_OK;
}

void CGameObject::Damage()
{
}

void CGameObject::Death()
{
}


_float CGameObject::Get_CameraDistance()
{
	_float3			vCamPos = {};
	_float4x4		ViewMatrix = {};

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	
	memcpy(&vCamPos, &ViewMatrix.m[3], sizeof(_float3));

	_float3			vDir = vCamPos - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
	return D3DXVec3Length(&vDir);
}

CComponent* CGameObject::Find_Component(const _wstring& strComponentTag)
{
	auto	iter = m_Components.find(strComponentTag);
	if(iter == m_Components.end())
		return nullptr;

	return iter->second;
}

HRESULT CGameObject::Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg)
{
	if (nullptr != Find_Component(strComponentTag))
		return E_FAIL;

	CComponent*			pComponent = dynamic_cast<CComponent*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(strComponentTag, pComponent);

	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	return S_OK;
}

HRESULT CGameObject::Setting_Shader(const WCHAR* ShaderName)
{
	LPD3DXBUFFER		pErr = NULL;
	if (FAILED(D3DXCreateEffectFromFile(m_pGraphic_Device, ShaderName, NULL, NULL, 0, NULL, &m_pEffect, &pErr)))
	{
		if (pErr)
		{
			OutputDebugStringA((char*)pErr->GetBufferPointer());
			MessageBoxA(nullptr, (char*)pErr->GetBufferPointer(), "FX Load Error", MB_OK);
			pErr->Release();
		}
		return E_FAIL;
	}
	else
	{
		D3DXDeclaratorFromFVF(D3DFVF_CUSTOMVERTEX, decl);
		m_hTechnique = m_pEffect->GetTechniqueByName("main");
	}

	return S_OK;
}

void CGameObject::Excute_Billboard(const _matrix& _InvWorldMat, LPDIRECT3DBASETEXTURE9 pTex)
{
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &m_ViewMat);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &m_ProMat);
	m_pGraphic_Device->CreateVertexDeclaration(decl, &m_pDecl);
	//m_pEffect->SetTechnique(m_hTechnique);
	
	_matrix testMat = _InvWorldMat;
	_float3 pos =  m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
	_float4 Size = {};

	memcpy(Size, m_pTransformCom->GetScale(), sizeof(_float3));
	memcpy((_float3*)&testMat.m[3], pos, sizeof(_float3));

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &testMat);
	/*m_pEffect->SetVector("vScale", &Size);
	m_pEffect->SetMatrix("WorldMat", &testMat);
	m_pEffect->SetMatrix("ViewMat", &m_ViewMat);
	m_pEffect->SetMatrix("ProjdMat", &m_ProMat);

	m_pEffect->SetTexture("Tex", pTex);

	m_pGraphic_Device->SetVertexDeclaration(m_pDecl);
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);*/
}

void CGameObject::End_Billboard()
{
	//m_pEffect->EndPass();
	//m_pEffect->End();
	//m_pEffect->OnResetDevice();
}

void CGameObject::Free()
{
	__super::Free();

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);

	m_Components.clear();

	if (m_pEffect)
		m_pEffect->OnResetDevice();
	Safe_Release(m_pEffect);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);
}
