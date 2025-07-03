#include "Terrian_Manager.h"

#include "Terrain.h"
#include "Transform.h"

IMPLEMENT_SINGLETON(CTerrian_Manager);

CTerrian_Manager::CTerrian_Manager()
{

}

HRESULT CTerrian_Manager::Initialize(const _float2& TileSize)
{
    m_Size = TileSize;

    m_pTerrian.resize(m_Size.y * m_Size.x, nullptr);
    return S_OK;
}

CTerrain* CTerrian_Manager::GetOnTerrian(_float3& Pos)
{
    _uint IndexX = Pos.x / m_Size.x;
    _uint IndexY = Pos.z / m_Size.y;

    if (IndexX >= m_Size.x || 0 > IndexX || IndexY >= m_Size.y || 0 > IndexY)
        return nullptr;

    if (IndexY == 1)
        int a = 10;

    return m_pTerrian[IndexY * m_Size.x + IndexX];
}

void CTerrian_Manager::ADD_Terrian(CTerrain* pTerrian)
{
    if (nullptr == pTerrian)
        return;

    CTransform* TerrianPos = pTerrian->GetTransfrom();
    if (nullptr == TerrianPos)
        return;

    _float3 Pos = TerrianPos->GetWorldState(WORLDSTATE::POSITION);
    _uint IndexX = Pos.x / m_Size.x;
    _uint IndexY = Pos.z / m_Size.y;

    if (IndexX >= m_Size.x || 0 > IndexX || IndexY >= m_Size.y || 0 > IndexY)
        return;

    m_pTerrian[IndexY * m_Size.x + IndexX] = pTerrian;
}

void CTerrian_Manager::Clear()
{
    for (auto iter : m_pTerrian)
        Safe_Release(iter);
}

void CTerrian_Manager::Free()
{
    __super::Free();

    Clear();
}
