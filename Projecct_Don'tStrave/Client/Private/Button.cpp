#include "Button.h"

#include "GameInstance.h"

CButton::CButton(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CUserInterface{pGraphic_Device}
{
}

CButton::CButton(const CButton& Prototype)
    :CUserInterface{Prototype}
{
}

HRESULT CButton::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CButton::Initialize(void* pArg)
{
    m_isClicked = false;
    m_isHovered = false;

    BUTTON_DESC* pDesc = static_cast<BUTTON_DESC*>(pArg);

    CUserInterface::UIOBJECT_DESC Desc = {};

    m_iTextureIndex = pDesc->iTextureIndex;

    Desc.fX = pDesc->fX + pDesc->fRelativeX;
    Desc.fY = pDesc->fY + pDesc->fRelativeY;
    Desc.fSizeX = pDesc->fSizeX;
    Desc.fSizeY = pDesc->fSizeY;

    m_fRelativeX = pDesc->fRelativeX;
    m_fRelativeY = pDesc->fRelativeY;

    m_pParentTransform = pDesc->pParentTransform;

    Safe_AddRef(m_pParentTransform);
    
        

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    return S_OK;
}

void CButton::Priority_Update(_float fTimeDelta)
{
}

void CButton::Update(_float fTimeDelta)
{
    // 부모 상대위치로 업데이트
    if (nullptr != m_pParentTransform)
    {
        _float3 vParentPos = {};
        vParentPos = m_pParentTransform->GetWorldState(WORLDSTATE::POSITION);
        vParentPos.x += m_fRelativeX;
        vParentPos.y += m_fRelativeY;
        m_pTransform_Com->SetPosition(vParentPos);

        m_fX = vParentPos.x + g_iWinSizeX * 0.5f;
        m_fY = (vParentPos.y - g_iWinSizeY * 0.5f) * -1.f;
    }

}

void CButton::Late_Update(_float fTimeDelta)
{
}

HRESULT CButton::Render()
{
    return S_OK;
}

HRESULT CButton::ADD_Components()
{
    return S_OK;
}

_bool CButton::isMouseOver()
{
    RECT rect = { m_fX - m_fSizeX * 0.5f, m_fY - m_fSizeY * 0.5f, m_fX + m_fSizeX * 0.5f, m_fY + m_fSizeY * 0.5f };

    POINT pt{};
    GetCursorPos(&pt);
    ScreenToClient(g_hWnd, &pt);

    return m_isHovered = PtInRect(&rect, pt);
}

void CButton::Free()
{
    __super::Free();

    Safe_Release(m_pParentTransform);
}
