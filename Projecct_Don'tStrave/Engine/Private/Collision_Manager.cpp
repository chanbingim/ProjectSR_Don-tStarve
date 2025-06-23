#include "Collision_Manager.h"
#include "Box_Collision_Component.h"
#include "Sphere_Collision_Component.h"

IMPLEMENT_SINGLETON(CCollision_Manager);

CCollision_Manager::CCollision_Manager()
{
}

HRESULT CCollision_Manager::Initialize()
{
    m_bColCheck.resize(200, false);

    return S_OK;
}

void CCollision_Manager::Update()
{
    for (_uint i = 0; i < ENUM_CLASS(COLLISION_TYPE::END); ++i)
    {
        _uint SrcIndex{}; 

        if (m_bColCheck.size() < m_pCol_List[i].size())
            m_bColCheck.resize(m_pCol_List[i].size(), false);
        else
            fill(m_bColCheck.begin(), m_bColCheck.end(), false);

        for (auto& Src : m_pCol_List[i])
        {
            if (m_bColCheck[SrcIndex])
                continue;

            _uint DstIndex{};
            for (auto& Dst : m_pCol_List[i])
            {
                if (Src == Dst)
                    break;

                _bool ColFlag = false;
                switch (i)
                {
                case 0 :
                    if (AxisAlignedBoundBox(Src, Dst))
                    {
                        ColFlag = true;
                    }
                    break;

                case 1:
                    if (SphereCol(Src, Dst))
                    {
                        ColFlag = true;
                    }
                    break;
                }

                if (ColFlag)
                {
                    Src->ADDHitGroup(Dst->GetOwner());
                    Dst->ADDHitGroup(Src->GetOwner());

                    m_bColCheck[DstIndex] = true;
                    m_bColCheck[SrcIndex] = true;
                }
                DstIndex++;
            }
            SrcIndex++;
        }
    }
}

void CCollision_Manager::ADD_ColList(CCollision_Component* pCol_Component)
{
    _uint typeIndex = ENUM_CLASS(pCol_Component->GetColType());

    auto iter = find_if(m_pCol_List[typeIndex].begin(), m_pCol_List[typeIndex].end(), [&](CCollision_Component* Collison)
                {
                     return pCol_Component == Collison ? true : false;
                });

    if (iter == m_pCol_List[typeIndex].end())
    {
        m_pCol_List[typeIndex].push_back(pCol_Component);
        Safe_AddRef(pCol_Component);
    }
}

void CCollision_Manager::Remove_ColList(CCollision_Component* pCol_Component)
{
    _uint typeIndex = ENUM_CLASS(pCol_Component->GetColType());

    auto iter = find_if(m_pCol_List[typeIndex].begin(), m_pCol_List[typeIndex].end(), [&](CCollision_Component* Collison)
        {
            return pCol_Component == Collison ? true : false;
        });

    if (iter == m_pCol_List[typeIndex].end())
        return;
    else
    {
        m_pCol_List->erase(iter);
        Safe_Release(*iter);
    }
}

_bool CCollision_Manager::AxisAlignedBoundBox(CCollision_Component* pCol, CCollision_Component* pOtherCol)
{
    //AABB 충돌 로직구현
    //경계 상자의 크기를 계산해 주는 함수
    //월드 기준의 위치 필요
    _float3 SrcMin, SrcMax, DstMin, DstMax;
    if (FAILED(static_cast<CBox_Collision_Component*>(pCol)->ComputeBounding(&SrcMin, &SrcMax)))
        return false;

    if (FAILED(static_cast<CBox_Collision_Component*>(pOtherCol)->ComputeBounding(&DstMin, &DstMax)))
        return false;

    if ((SrcMin.x <= DstMin.x && DstMin.x <= SrcMax.x) ||
        (SrcMin.y <= DstMin.y && DstMin.y <= SrcMax.y) ||
        (SrcMin.z <= DstMin.z && DstMin.z <= SrcMax.z) ||
        (SrcMin.x <= DstMax.x && DstMax.x <= SrcMax.x) ||
        (SrcMin.y <= DstMax.y && DstMax.y <= SrcMax.y) ||
        (SrcMin.z <= DstMax.z && DstMax.z <= SrcMax.z))
        return true;

    return false;
}

_bool CCollision_Manager::SphereCol(CCollision_Component* pCol, CCollision_Component* pOtherCol)
{
    //AABB 충돌 로직구현
    //구의 경계 상자의 크기를 계산해 주는 함수 
    _float3 SrcCenter{}, DstCenter{};
    _float  SrcRadius{}, DstRadius{};

    static_cast<CSphere_Collision_Component*>(pCol)->ComputeBounding(&SrcCenter, &SrcRadius);
    static_cast<CSphere_Collision_Component*>(pOtherCol)->ComputeBounding(&DstCenter, &DstRadius);

    _float3 Dist = SrcCenter - DstCenter;
    _float  Length = D3DXVec3Length(&Dist);

    if ((DstRadius + SrcRadius) >= Length)
        return true;

    return false;
}

_bool CCollision_Manager::OrientedBoundingBox(CCollision_Component* pCol, CCollision_Component* pOtherCol)
{
    //OBB 충돌 로직 구현

    return false;
}

void CCollision_Manager::Free()
{
    __super::Free();

    for (_uint i = 0; i < ENUM_CLASS(COLLISION_TYPE::END); ++i)
    {
        for (auto iter : m_pCol_List[i])
        {
            Safe_Release(iter);
        }
        m_pCol_List[i].clear();
    }
}
