#include "Renderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "Light_Manager.h"

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device)
    : m_pGraphic_Device { pGraphic_Device }
{
    Safe_AddRef(m_pGraphic_Device);
}

HRESULT CRenderer::Initialize()
{
	D3DVIEWPORT9        Viewport{};
	m_pGraphic_Device->GetViewport(&Viewport);
	D3DXMatrixOrthoLH(&m_OrtTHOMat, (FLOAT)Viewport.Width, (FLOAT)Viewport.Height, 0.f, 1.f);

	D3DXMatrixIdentity(&m_IndentiyViewMat);

	_D3DVIEWPORT9 ScreenSize = {};
	m_pGraphic_Device->GetViewport(&ScreenSize);

	//CreateTexture 함수로 텍스처를 생성한다.
	m_pGraphic_Device->CreateTexture(ScreenSize.Width, ScreenSize.Height, 1, D3DUSAGE_RENDERTARGET,
		D3DFMT_A16B16G16R16, D3DPOOL_DEFAULT, &BakcBufferTexture, nullptr);

    return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDER eRenderGroup, CGameObject* pRenderObject)
{
	if (nullptr == pRenderObject)
		return E_FAIL;

	m_RenderObjects[ENUM_CLASS(eRenderGroup)].push_back(pRenderObject);

	Safe_AddRef(pRenderObject);

	return S_OK;
}

void CRenderer::Render()
{
	Render_Priority();

	CLight_Manager::GetInstance()->Enable_Light();
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
#pragma region NONE_BLEND
	Render_NonBlend();
	Render_AlphaTest();
#pragma endregion

#pragma region BLEND
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	Render_Blend();
	Render_Particle();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
#pragma endregion

	Render_UI();
	SaveRenderTarget();
}

void CRenderer::ResetRenderer()
{
	_uint EndIndex = ENUM_CLASS(RENDER::END);
	for (_uint i = 0; i < EndIndex; ++i)
	{
		for (auto& pRenderObject : m_RenderObjects[i])
			Safe_Release(pRenderObject);

		m_RenderObjects[i].clear();
	}
}

void CRenderer::Render_Priority()
{
	for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::PRIORITY)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_CLASS(RENDER::PRIORITY)].clear();
}

void CRenderer::Render_NonBlend()
{
	for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::NONBLEND)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_CLASS(RENDER::NONBLEND)].clear();
}

void CRenderer::Render_AlphaTest()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	
	m_RenderObjects[ENUM_CLASS(RENDER::ALPHATEST)].sort([](CGameObject* pSour, CGameObject* pDest)->_bool
		{
			return pSour->Get_CameraDistance() > pDest->Get_CameraDistance();
		});

	for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::ALPHATEST)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_CLASS(RENDER::ALPHATEST)].clear();
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	
}

void CRenderer::Render_Blend()
{
	m_RenderObjects[ENUM_CLASS(RENDER::BLEND)].sort([](CGameObject* pSour, CGameObject* pDest)->_bool
		{
			return pSour->Get_CameraDistance() > pDest->Get_CameraDistance();
		});

	for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::BLEND)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_CLASS(RENDER::BLEND)].clear();
}

void CRenderer::Render_Particle()
{
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);

	for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::PARTICLE)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_CLASS(RENDER::PARTICLE)].clear();

	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
}

void CRenderer::Render_UI()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	Render_Projection_UI();
	Render_Ortho_UI();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Render_Alpha_UI();
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	
}

void CRenderer::Render_Projection_UI()
{
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_IndentiyViewMat);
	for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::PROJECTION_UI)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_CLASS(RENDER::PROJECTION_UI)].clear();
}

void CRenderer::Render_Ortho_UI()
{
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_OrtTHOMat);
	for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::ORTTHO_UI)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}
	m_RenderObjects[ENUM_CLASS(RENDER::ORTTHO_UI)].clear();
}

void CRenderer::Render_Alpha_UI()
{
	for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::BLENDUI)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}
	m_RenderObjects[ENUM_CLASS(RENDER::BLENDUI)].clear();
}

void CRenderer::SaveRenderTarget()
{
	IDirect3DSurface9* pSurface = nullptr;
	//백버퍼에서 현재까지 그려진 정보를 가져옴
	m_pGraphic_Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSurface);

	// Suface를 통해서 현재 레벨의 표면을 가져오고 그걸 복사한다.
	// bit blt 생각하면 된다. 그러면 텍스쳐를 얻어 RenderTarget의 형태가 가능하다.
	LPDIRECT3DSURFACE9  pTextureSurface = nullptr;
	BakcBufferTexture->GetSurfaceLevel(0, &pTextureSurface);
	m_pGraphic_Device->StretchRect(pSurface, nullptr, pTextureSurface, nullptr, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetTexture(7, BakcBufferTexture);

	Safe_Release(pSurface);
	Safe_Release(pTextureSurface);
}

CRenderer* CRenderer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRenderer* pInstance = new CRenderer(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderer::Free()
{
    __super::Free();

	for (auto& RenderObjects : m_RenderObjects)
	{
		for (auto& pRenderObject : RenderObjects)
			Safe_Release(pRenderObject);
		RenderObjects.clear();
	}

	Safe_Release(m_pGraphic_Device);
	Safe_Release(BakcBufferTexture);
}
