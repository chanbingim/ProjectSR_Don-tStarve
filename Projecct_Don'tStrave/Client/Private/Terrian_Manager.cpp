#include "Terrian_Manager.h"

#include "Terrain.h"
#include "Transform.h"

IMPLEMENT_SINGLETON(CTerrian_Manager);

CTerrian_Manager::CTerrian_Manager()
{

}

HRESULT CTerrian_Manager::Initialize(const _float2& TileSize, const _float2& vecSize)
{
    m_TileSize = TileSize;
    m_vecSize = vecSize;

    m_pTerrian.resize((_uint)m_vecSize.y * (_uint)m_vecSize.x, nullptr);
    m_IgnoreID.reserve(50);

    m_IgnoreID.push_back(1003);
    return S_OK;
}

CTerrain* CTerrian_Manager::GetOnTerrian(_float3& Pos)
{
    _float IndexX = Pos.x / m_TileSize.x;
    _float IndexY = Pos.z / m_TileSize.y;

    if (IndexX >= m_vecSize.x || 0 > IndexX || IndexY >= m_vecSize.y || 0 > IndexY)
        return nullptr;

    CTerrain* FindTile = m_pTerrian[(_uint)IndexY * (_uint)m_vecSize.x + (_uint)IndexX];
    if (nullptr == FindTile)
        return nullptr;

    auto iter = find(m_IgnoreID.begin(), m_IgnoreID.end(), FindTile->Get_ObjectID());

    if (iter == m_IgnoreID.end())
        return FindTile;

    return nullptr;
}

vector<CTerrain*>* CTerrian_Manager::GetTerrains()
{
    return &m_pTerrian;
}

void CTerrian_Manager::ADD_Terrian(CTerrain* pTerrian)
{
    if (nullptr == pTerrian)
        return;

    CTransform* TerrianPos = pTerrian->GetTransfrom();
    if (nullptr == TerrianPos)
        return;

    _float3 Pos = TerrianPos->GetWorldState(WORLDSTATE::POSITION);
    _uint IndexX = _uint(Pos.x / m_TileSize.x);
    _uint IndexY = _uint(Pos.z / m_TileSize.y);

    if (IndexX >= m_vecSize.x || 0 > IndexX || IndexY >= m_vecSize.y || 0 > IndexY)
        return;

    m_pTerrian[IndexY * m_vecSize.x + IndexX] = pTerrian;
    Safe_AddRef(pTerrian);
}

void CTerrian_Manager::Clear()
{
    int i = 0;
    for (auto iter : m_pTerrian)
    {
        Safe_Release(iter);
        m_pTerrian[i] = nullptr;
        i++;
    }
}

void CTerrian_Manager::Free()
{
    __super::Free();

    if(m_pTerrian.size() > 0)
        Clear();
}
