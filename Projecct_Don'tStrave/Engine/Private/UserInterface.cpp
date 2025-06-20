#include "UserInterface.h"
#include "Transform.h"

CUserInterface::CUserInterface(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CGameObject(pGraphic_Device)
{
}

CUserInterface::CUserInterface(const CUserInterface& rhs) :
    CGameObject(rhs)
{
}

HRESULT CUserInterface::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUserInterface::Initialize(void* pArg)
{
    UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

    m_fX = pDesc->fX;
    m_fY = pDesc->fY;
    m_fSizeX = pDesc->fSizeX;
    m_fSizeY = pDesc->fSizeY;

    D3DVIEWPORT9		ViewportDesc{};
    m_pGraphic_Device->GetViewport(&ViewportDesc);

    m_fWinSizeX = (_float)ViewportDesc.Width;
    m_fWinSizeY = (_float)ViewportDesc.Height;

    return S_OK;
}

void CUserInterface::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CUserInterface::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CUserInterface::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CUserInterface::Render()
{
    __super::Render();
    return S_OK;
}

void CUserInterface::HoverEevent()
{

}

void CUserInterface::ClickedEevent()
{
}

void CUserInterface::UpdatePosition()
{
    m_pTransform_Com->SetScale(_float3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransform_Com->SetPosition(_float3(m_fX - m_fWinSizeX * 0.5f, -m_fY + m_fWinSizeY * 0.5f, 0.f));
}

CGameObject* CUserInterface::Clone(void* pArg)
{
    return nullptr;
}

void CUserInterface::Free()
{
    __super::Free();
}
