#pragma once

#include "Base.h"


NS_BEGIN(Engine)
class CGameObject;

typedef struct QuadNode
{
	RECT				QuadSize = {};
	list<CGameObject*>	ObejctList;
	QuadNode*			pNextNode[4];
}QUAD_NODE;

class ENGINE_DLL CQuadTree final : public CBase
{
private:
	CQuadTree();
	virtual ~CQuadTree() = default;

public:
	HRESULT					Initialize(_float2 WorldSize);
	
	HRESULT					ADD_Item(CGameObject* pGameObject);
	HRESULT					Remove_Item(CGameObject* pGameObject);

	list<CGameObject*>		GetCandidate(const RECT& Range);

private :
	QuadNode*				Root = { nullptr };
	list<CGameObject*>		m_CandidateList = { };

private :
	void					MakeTree();
	QuadNode*				Insert_NodeData();


public :
	virtual		void		Free() override;

};

NS_END