#include "Font_Manager.h"

CFont_Manager::CFont_Manager(LPDIRECT3DDEVICE9 pGraphic_Device)
    :m_pGraphic_Device{ pGraphic_Device }
{
    Safe_AddRef(m_pGraphic_Device);
}

HRESULT CFont_Manager::Initialize()
{
    if (FAILED(D3DXCreateSprite(m_pGraphic_Device, &m_pSprite)))
        return E_FAIL;

    return S_OK;
}

HRESULT CFont_Manager::Add_Font(const _wstring strFontTag, _uint iSize, const _tchar* pFontName)
{
    auto iter = m_pFonts.find(strFontTag);

    if (iter != m_pFonts.end())
        return E_FAIL;

    LPD3DXFONT	pFont = { nullptr };

    D3DXFONT_DESC           FontDesc{};
    FontDesc.Height = iSize;
    FontDesc.Width = 0;
    FontDesc.Weight = FW_HEAVY;
    FontDesc.MipLevels = 1;
    FontDesc.Italic = 0;
    FontDesc.CharSet = 0;
    lstrcpy(FontDesc.FaceName, pFontName);

    if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &FontDesc, &pFont)))
        return E_FAIL;

    m_pFonts.emplace(strFontTag, pFont);

    return S_OK;
}

void CFont_Manager::Render_Font(const _wstring strFontTag, const _tchar* pText, RECT* pRect, const D3DXCOLOR& Color, DWORD Fomat)
{
    m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

    auto iter = m_pFonts.find(strFontTag);
    
    //DWORD Fomat = DT_CENTER | DT_VCENTER;

    // DT_CENTER
    if(iter != m_pFonts.end())
        (*iter).second->DrawTextW(m_pSprite, pText, lstrlen(pText), pRect, Fomat, Color);
    
    m_pSprite->End();
}

CFont_Manager* CFont_Manager::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CFont_Manager* pInstance = new CFont_Manager(pGraphic_Device);

    if (FAILED(pInstance->Initialize()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CFont_Manager");
        return nullptr;
    }

    return pInstance;
}

void CFont_Manager::Free()
{
    __super::Free();

    Safe_Release(m_pGraphic_Device);
}
