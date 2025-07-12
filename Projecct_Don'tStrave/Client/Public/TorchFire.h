#pragma once
#include "Client_Defines.h"
#include "LandObject.h"
NS_BEGIN(Engine);
class CLightComponent;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client);

class CTorchFire : public CLandObject
{
private:
    CTorchFire(LPDIRECT3DDEVICE9 pGraphic_Device);
    CTorchFire(const CTorchFire& Prototype);
    virtual ~CTorchFire() = default;

public:
    virtual HRESULT  Initialize_Prototype();
    virtual HRESULT  Initialize(void* pArg);
    virtual HRESULT  Initialize_Late();
    virtual void     Priority_Update(_float fTimeDelta);
    virtual void     Update(_float fTimeDelta);
    virtual void     Late_Update(_float fTimeDelta);
    HRESULT     Render(D3DXMATRIX mat);

    void Update_TorchFire(_bool bFire);

private:
    CVIBuffer_Rect*     m_pVIBufferCom = { nullptr };

    _float                m_fDurability = {};
    _float              m_fTime;
    _float              m_fAniTime;
    _bool               m_bLight;
    D3DLIGHT9           m_Light{};
    CLightComponent*    m_pLight_Com = { nullptr };
    CTexture*           m_pAlphaTex = { nullptr };
    CTexture*           m_pFireTex = { nullptr };
    CTexture*           m_pEffectFire = { nullptr };

    D3DXCOLOR           m_Color = {};
private:
    HRESULT Ready_Components();

public:
    static CTorchFire* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};

NS_END