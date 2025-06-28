#pragma once

#include "Base.h"

NS_BEGIN(Engine)
class CCollision_Component;

class CCollision_Manager final : public CBase
{
	DECLARE_SINGLETON(CCollision_Manager);

private :
	CCollision_Manager();
	virtual ~CCollision_Manager() = default;

public :
	HRESULT		Initialize();
	void		Update();

	void		ADD_ColList(CCollision_Component* pCol_Component);
	void		Remove_ColList(CCollision_Component* pCol_Component);

private :
	list<CCollision_Component*>		m_pCol_List[ENUM_CLASS(COLLISION_TYPE::END)] = {};
	list<CCollision_Component*>		m_UpdateList;

private :
	void		ADD_UpdateList(CCollision_Component* pCol);

	_bool		AxisAlignedBoundBox(CCollision_Component* pCol, CCollision_Component* pOtherCol);
	_bool		SphereCol(CCollision_Component* pCol, CCollision_Component* pOtherCol);
	_bool		OrientedBoundingBox(CCollision_Component* pCol, CCollision_Component* pOtherCol);
	_bool		CompareBoxToSphere(CCollision_Component* pCol, CCollision_Component* pOtherCol);
	void		CompareSphereListToBoxList(list<CCollision_Component*>* pSrcList, list<CCollision_Component*>* pDstList);

public :
	virtual		void				Free();
};
NS_END