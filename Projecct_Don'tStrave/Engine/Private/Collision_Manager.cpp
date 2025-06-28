#include "Collision_Manager.h"
#include "GameObject.h"
#include "Box_Collision_Component.h"
#include "Sphere_Collision_Component.h"

IMPLEMENT_SINGLETON(CCollision_Manager);

CCollision_Manager::CCollision_Manager()
{
}

HRESULT CCollision_Manager::Initialize()
{
    return S_OK;
}

void CCollision_Manager::Update()
{
    for (_uint i = 0; i < ENUM_CLASS(COLLISION_TYPE::END); ++i)
    {
        for (auto& Src : m_pCol_List[i])
        {
            for (auto& Dst : m_pCol_List[i])
            {
                if (Src == Dst)
                    continue;

                _bool ColFlag = false;
                switch (i)
                {
                case 0:
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
                    ADD_UpdateList(Dst);
                }
            }
        }
    }

    CompareSphereListToBoxList(&m_pCol_List[ENUM_CLASS(COLLISION_TYPE::SPHERE)], &m_pCol_List[ENUM_CLASS(COLLISION_TYPE::BOX)]);

    for (auto iter : m_UpdateList)
        iter->Update();

    m_UpdateList.clear();
}

void CCollision_Manager::ADD_ColList(CCollision_Component* pCol_Component)
{
    _uint typeIndex = ENUM_CLASS(pCol_Component->GetColType());

    auto iter = find(m_pCol_List[typeIndex].begin(), m_pCol_List[typeIndex].end(), pCol_Component);

    if (iter == m_pCol_List[typeIndex].end())
    {
        m_pCol_List[typeIndex].push_back(pCol_Component);
    }
}

void CCollision_Manager::Remove_ColList(CCollision_Component* pCol_Component)
{
    _uint typeIndex = ENUM_CLASS(pCol_Component->GetColType());

    auto iter = find(m_pCol_List[typeIndex].begin(), m_pCol_List[typeIndex].end(), pCol_Component);
    if (iter == m_pCol_List[typeIndex].end())
        return;
    else
    {
        m_pCol_List[typeIndex].erase(iter);
    }
}

void CCollision_Manager::ADD_UpdateList(CCollision_Component* pCol)
{
    auto iter = find(m_UpdateList.begin(), m_UpdateList.end(), pCol);
    if (iter == m_UpdateList.end())
        m_UpdateList.push_back(pCol);
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

    if ((DstMax.x < SrcMin.x || SrcMax.x < DstMin.x))
        return false;

    if ((DstMax.y < SrcMin.y || SrcMax.y < DstMin.y))
        return false;

    if ((DstMax.z < SrcMin.z || SrcMax.z < DstMin.z))
        return false;

    return true;
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

_bool CCollision_Manager::CompareBoxToSphere(CCollision_Component* pCol, CCollision_Component* pOtherCol)
{
    //AABB 충돌 로직구현
   //구의 경계 상자의 크기를 계산해 주는 함수 
    _float3 SrcCenter{};
    _float  SrcRadius{};
    _float3 DstMin, DstMax;
    _float3 BoxNearPoint = {};

    static_cast<CSphere_Collision_Component*>(pCol)->ComputeBounding(&SrcCenter, &SrcRadius);
    if (FAILED(static_cast<CBox_Collision_Component*>(pOtherCol)->ComputeBounding(&DstMin, &DstMax)))
        return false;

    BoxNearPoint.x = max(DstMin.x, min(DstMax.x, SrcCenter.x));
    BoxNearPoint.y = max(DstMin.y, min(DstMax.y, SrcCenter.y));
    BoxNearPoint.z = max(DstMin.z, min(DstMax.z, SrcCenter.z));

    _float3 Dist = SrcCenter - BoxNearPoint;
    _float  Length = D3DXVec3Length(&Dist);

    if (SrcRadius >= Length)
        return true;

    return false;
}

void CCollision_Manager::CompareSphereListToBoxList(list<CCollision_Component*>* pSrcList, list<CCollision_Component*>* pDstList)
{
    for (auto& Src : *pSrcList)
    {
        for (auto& Dst : *pDstList)
        {
            if (CompareBoxToSphere(Src, Dst))
            {
                Src->ADDHitGroup(Dst->GetOwner());
                Dst->ADDHitGroup(Src->GetOwner());

                ADD_UpdateList(Dst);
                ADD_UpdateList(Src);
            }
        }
    }
}

void CCollision_Manager::Free()
{
    __super::Free();

   /* for (_uint i = 0; i < ENUM_CLASS(COLLISION_TYPE::END); ++i)
    {
        for (auto iter : m_pCol_List[i])
        {
            Safe_Release(iter);
        }
        m_pCol_List[i].clear();
    }*/
}
