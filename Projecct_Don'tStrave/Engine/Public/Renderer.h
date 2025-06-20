#pragma once

/* ��ü�� �����ÿ� �׷������� ��ü��� ������Ʈ �Ŵ������� �߰��ϰ�, ���������� �߰��Ѵ�.(x)*/
/* �� �����Ӵ� ��ü�ȿ��� �׷������ϴ°��� �Ǵ��ϰ� �׷������Ѵٶ�� �������� ����ϴ� �۾��� �����Ѵ�. */

/* ȭ�鿡 �׷������� ��ü���� �׸��� ������� �з��Ͽ� �����Ѵ�. */
/* �����ϰ� �մ� ��ü���� ������ ������� �����Լ��� ȣ���� �ش�. �����̳ʸ� Ŭ�����ع�����. */
#include "Base.h"

NS_BEGIN(Engine)

class CRenderer final : public CBase
{
private:
	CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CRenderer() = default;

public:
	HRESULT Initialize();
	HRESULT Add_RenderGroup(RENDER eRenderGroup, class CGameObject* pRenderObject);
	void Render();

private:
	LPDIRECT3DDEVICE9					m_pGraphic_Device = { nullptr };
	list<class CGameObject*>			m_RenderObjects[ENUM_CLASS(RENDER::END)];

	_float4x4							m_IndentiyViewMat;
	_float4x4							m_OrtTHOMat;

private:
	void Render_Priority();
	void Render_NonBlend();
	void Render_Blend();
	
	void Render_UI();
	void Render_Projection_UI();
	void Render_Ortho_UI();

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

NS_END