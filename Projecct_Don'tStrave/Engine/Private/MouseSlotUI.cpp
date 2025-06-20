#include "MouseSlotUI.h"
#include "VIBuffer.h"
#include "Texture.h"

CMouseSlotUI::CMouseSlotUI(LPDIRECT3DDEVICE9 pGraphic_Device) : 
    CUserInterface(pGraphic_Device)
{
}

CMouseSlotUI::CMouseSlotUI(const CMouseSlotUI& rhs) :
    CUserInterface(rhs)
{

}

HRESULT CMouseSlotUI::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMouseSlotUI::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CMouseSlotUI::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CMouseSlotUI::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CMouseSlotUI::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CMouseSlotUI::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CMouseSlotUI::UpdateSlotData(CGameObject* pBase, const _wstring& BaseVIBufferTag, const _wstring& BaseTextureTag)
{
    if (nullptr == pBase)
        return E_FAIL;

    m_pVISlotBuffer_Com = dynamic_cast<CVIBuffer*>(pBase->Find_Component(BaseVIBufferTag));
    if (nullptr == m_pVISlotBuffer_Com)
        return E_FAIL;

    m_pSlotTexture_Com = dynamic_cast<CTexture*>(pBase->Find_Component(BaseTextureTag));
    if (nullptr == m_pSlotTexture_Com)
        return E_FAIL;

    Safe_AddRef(m_pVISlotBuffer_Com);
    Safe_AddRef(m_pSlotTexture_Com);
    return S_OK;
}

CGameObject* CMouseSlotUI::Clone(void* pArg)
{
    return nullptr;
}

void CMouseSlotUI::Free()
{
    __super::Free();

    Safe_Release(m_pVISlotBuffer_Com);
    Safe_Release(m_pSlotTexture_Com);
}
