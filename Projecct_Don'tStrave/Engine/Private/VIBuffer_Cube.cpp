#include "VIBuffer_Cube.h"

CVIBuffer_Cube::CVIBuffer_Cube(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Cube::CVIBuffer_Cube(const CVIBuffer_Cube& Prototype) :
    CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_Cube::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CVIBuffer_Cube::Initialize(void* pArg)
{
    return S_OK;
}

CVIBuffer_Cube* CVIBuffer_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : VIBUFFER CUBE");
    }
    return pInstance;
}

CComponent* CVIBuffer_Cube::Clone(void* pArg)
{
    CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : VIBUFFER CUBE");
    }
    return pInstance;
}

void CVIBuffer_Cube::Free()
{
    __super::Free();
}
