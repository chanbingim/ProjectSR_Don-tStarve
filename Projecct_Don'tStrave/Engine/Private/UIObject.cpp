#include "UIObject.h"

#include "Transform.h"

CUIObject::CUIObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{
}

CUIObject::CUIObject(const CUIObject& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CUIObject::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUIObject::Initialize(void* pArg)
{
	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	m_fX = pDesc->fX;
	m_fY = pDesc->fY;
	m_fSizeX = pDesc->fSizeX;
	m_fSizeY = pDesc->fSizeY;


	D3DVIEWPORT9		ViewportDesc{};
	m_pGraphic_Device->GetViewport(&ViewportDesc);

	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, ViewportDesc.Width, ViewportDesc.Height, 0.0f, 1.f);

	m_fWinSizeX = ViewportDesc.Width;
	m_fWinSizeY = ViewportDesc.Height;

	return S_OK;
}

void CUIObject::Priority_Update(_float fTimeDelta)
{
}

void CUIObject::Update(_float fTimeDelta)
{
	
}

void CUIObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CUIObject::Render()
{
	return S_OK;
}


void CUIObject::Update_Transform(CTransform* pTransform)
{
	pTransform->Set_Scale(_float3(m_fSizeX, m_fSizeY, 1.f));
	pTransform->Set_State(STATE::POSITION, _float3(m_fX - m_fWinSizeX * 0.5f, -m_fY + m_fWinSizeY * 0.5f, 0.f));

}

HRESULT CUIObject::Begin()
{	
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &m_OldViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &m_OldProjMatrix);

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	return S_OK;
}

HRESULT CUIObject::End()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_OldViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_OldProjMatrix);

	return S_OK;
}

void CUIObject::Free()
{
	__super::Free();
}
