#include "EventButton.h"

CEventButton::CEventButton(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CButton(pGraphic_Device)
{
}

CEventButton::CEventButton(const CEventButton& rhs) :
    CButton(rhs)
{
}

HRESULT CEventButton::Initialize_Prototype()
{
    return E_NOTIMPL;
}

HRESULT CEventButton::Initialize(void* pArg)
{
    return E_NOTIMPL;
}

void CEventButton::Priority_Update(_float fTimeDelta)
{
}

void CEventButton::Update(_float fTimeDelta)
{
}

void CEventButton::Late_Update(_float fTimeDelta)
{
}

HRESULT CEventButton::Render()
{
    return E_NOTIMPL;
}

CEventButton* CEventButton::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    return nullptr;
}

CGameObject* CEventButton::Clone(void* pArg)
{
    return nullptr;
}

void CEventButton::Free()
{
}
