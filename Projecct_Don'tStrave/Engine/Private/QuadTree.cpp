#include "QuadTree.h"

#include "GameObject.h"
#include "Transform.h"

CQuadTree::CQuadTree()
{
}

HRESULT CQuadTree::Initialize(_float2 WorldSize)
{
	Root = new QuadNode;

	MakeTree();
	return S_OK;
}

void CQuadTree::MakeTree()
{

}

QuadNode* CQuadTree::Insert_NodeData()
{



	return nullptr;
}

HRESULT CQuadTree::ADD_Item(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	auto Component = pGameObject->GetTransfrom();
	if (nullptr == Component)
		return E_FAIL;

	auto Transform = static_cast<CTransform *>(Component);
	_float3	ObjectPos = Transform->GetWorldState(WORLDSTATE::POSITION);



	return S_OK;
}

HRESULT CQuadTree::Remove_Item(CGameObject* pGameObject)
{
	return S_OK;
}

list<CGameObject*> CQuadTree::GetCandidate(const RECT& Range)
{
	return list<CGameObject*>();
}

void CQuadTree::Free()
{
}
