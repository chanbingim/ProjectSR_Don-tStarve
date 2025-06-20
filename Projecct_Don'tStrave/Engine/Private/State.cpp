#include "State.h"
#include "Texture.h"

HRESULT CState::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CState::Initialize(void* pArg)
{
    if (pArg)
    {
        m_Frame = *static_cast<FRAME_DESC*>(pArg);
        m_Notify.resize(m_Frame.iEndFrame, nullptr);

        if (m_Frame.pAnimTexture)
            Safe_AddRef(m_Frame.pAnimTexture);

        m_fFrameTime = m_Frame.fTimeRate / m_Frame.iEndFrame;
    }

    return S_OK;
}

HRESULT CState::Reset_StateData()
{
    m_AccTime = 0;
    m_Frame.iStartFrame = 0;
    m_Frame.fTimeRate = 1.0f;

    return S_OK;
}

void CState::Tick(_float fTimeDelta)
{
    if (m_AccTime >= m_fFrameTime)
    {
        if (m_Frame.iStartFrame < m_Frame.iEndFrame)
            m_Frame.iStartFrame++;
        else
        {
            if (m_Frame.bIsLoop)
            {
                m_Frame.iStartFrame = 0;
            }
        }
        m_AccTime = 0;
    }
    else
        m_AccTime += fTimeDelta;

    if (m_Notify[m_Frame.iStartFrame])
        m_Notify[m_Frame.iStartFrame]();

}

void CState::Render()
{

}

void CState::SetFrameRate(_float _ftimeRate)
{
    m_Frame.fTimeRate = _ftimeRate;
}

void CState::Free()
{
    __super::Free();

    Safe_Release(m_Frame.pAnimTexture);
}
