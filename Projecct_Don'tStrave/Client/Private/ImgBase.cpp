#include "ImgBase.h"
#include "GameInstance.h"

CImgBase::CImgBase() : m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pGameInstance);
}

HRESULT CImgBase::Initialize()
{
    return E_NOTIMPL;
}

void CImgBase::Update(_float fDeletaTime)
{
    
}

CImgBase* CImgBase::Clone(void* pArg)
{
    return nullptr;
}

void CImgBase::Free()
{
    __super::Free();

    Safe_Release(m_pGameInstance);
}