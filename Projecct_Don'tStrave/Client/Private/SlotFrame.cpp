#include "SlotFrame.h"

#include "GameInstance.h"
#include "Slot.h"

CSlotFrame::CSlotFrame(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUserInterface{ pGraphic_Device }
{
}

CSlotFrame::CSlotFrame(const CSlotFrame& Prototype)
    : CUserInterface{ Prototype }
{
}

HRESULT CSlotFrame::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSlotFrame::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    __super::UpdatePosition();

    CUserInterface::UIOBJECT_DESC Desc = {};

    Desc.fSizeX = m_fSizeX;
    Desc.fSizeY = m_fSizeY;
    Desc.fX = m_fX;
    Desc.fY = m_fY;

    m_pSlot = static_cast<class CSlot*>(m_pGameInstance->Clone_Prototype(
        PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Slot"), &Desc));

    if (nullptr == m_pSlot)
        return E_FAIL;

    return S_OK;
}

void CSlotFrame::Priority_Update(_float fTimeDelta)
{
    m_pSlot->Priority_Update(fTimeDelta);
}

void CSlotFrame::Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);

    RECT rc = { m_fX - m_fSizeX * 0.5f, m_fY - m_fSizeY * 0.5f, m_fX + m_fSizeX * 0.5f, m_fY + m_fSizeY * 0.5f };

    POINT pt{};
    GetCursorPos(&pt);
    ScreenToClient(g_hWnd, &pt);

    if (PtInRect(&rc, pt))
    {
        m_pTransform_Com->SetScale(_float3(m_fSizeX * 1.2f, m_fSizeY * 1.2f, 1.f));

        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
        {
            if (!m_bClick)
            {
                if (m_pSlot->Get_ItemID() == dynamic_cast<CSlot*>(m_pGameInstance->Chagne_Slot())->Get_ItemID())
                    m_pSlot->Merge_Item(dynamic_cast<CSlot*>(m_pGameInstance->Chagne_Slot()));
                else
                    m_pSlot = dynamic_cast<CSlot*>(m_pGameInstance->Chagne_Slot(m_pSlot));
            }
            m_bClick = true;
        }
        else
        {
            m_bClick = false;
        }
    }
    else
    {
        m_pTransform_Com->SetScale(_float3(m_fSizeX, m_fSizeY, 1.f));
    }

    Key_Input();

    m_pSlot->Update(fTimeDelta);
}

void CSlotFrame::Late_Update(_float fTimeDelta)
{
    m_pSlot->Late_Update(fTimeDelta);
}

HRESULT CSlotFrame::Render()
{
    m_pTexture_Com->Set_Texture(0);

    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pVIBuffer_Com->Render();

    m_pSlot->Render_ItemState();

    m_pSlot->Render();


    return S_OK;
}

HRESULT CSlotFrame::ADD_Components()
{
    // Texture Component
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Slot"),
        TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTexture_Com))))
        return E_FAIL;

    // VIBuffer_Rect Component
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"),
        reinterpret_cast<CComponent**>(&m_pVIBuffer_Com))))
        return E_FAIL;


    // Transform Component
    Engine::CTransform::TRANSFORM_DESC Desc = { 5.f, D3DXToRadian(90.f) };

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"),
        reinterpret_cast<CComponent**>(&m_pTransform_Com), &Desc)))
        return E_FAIL;


    return S_OK;
}

void CSlotFrame::Key_Input()
{

}

CSlotFrame* CSlotFrame::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CSlotFrame* pInstance = new CSlotFrame(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CSlotFrame");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSlotFrame::Clone(void* pArg)
{
    CSlotFrame* pInstance = new CSlotFrame(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CSlotFrame");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSlotFrame::Free()
{
    __super::Free();

    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
    Safe_Release(m_pSlot);
}
