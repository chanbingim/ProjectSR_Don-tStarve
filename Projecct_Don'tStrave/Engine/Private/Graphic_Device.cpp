#include"Graphic_Device.h"

CGraphic_Device::CGraphic_Device() : m_pSDK(nullptr), m_pGraphicDev(nullptr)
{
}

HRESULT CGraphic_Device::Ready_GraphicDev(HWND hWnd, WINMODE eMode, _uint iSizeX, _uint iSizeY, LPDIRECT3DDEVICE9* ppGraphicDev)
{

    m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

    if (nullptr == m_pSDK)
        return E_FAIL;

    D3DCAPS9        DeviceCaps;
    ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

    if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
    {
        return E_FAIL;
    }

    _ulong  dwFlag(0);

    if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        dwFlag |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

    else
        dwFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

    D3DPRESENT_PARAMETERS       d3dpp;
    ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

    d3dpp.BackBufferWidth = iSizeX;
    d3dpp.BackBufferHeight = iSizeY;

    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dpp.BackBufferCount = 1;
    
    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3dpp.MultiSampleQuality = 0;

    // D3DSWAPEFFECT_DISCARD : 스왑 체인
    // D3DSWAPEFFECT_FLIP : 버퍼 하나를 번갈아 사용
    // D3DSWAPEFFECT_COPY : 더블 버퍼링과 흡사한 복사 방식
    
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

    d3dpp.hDeviceWindow = hWnd;
    d3dpp.Windowed = static_cast<_bool>(eMode);     // 창 모드 또는 전체화면

    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
    
    // 모니터 주사율
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


    if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, dwFlag, &d3dpp, &m_pGraphicDev)))
        return E_FAIL;

    *ppGraphicDev = m_pGraphicDev;

    Safe_AddRef(m_pGraphicDev);

    return S_OK;
}

// 후면 버퍼의 동작
// 1. 기존의 텍스처를 지운다
// 2. 다음 화면에 그릴 텍스처를 준비한다.

void CGraphic_Device::Render_Begin(D3DXCOLOR Color)
{
    m_pGraphicDev->Clear(0,                     // 렉트의 개수
                        nullptr,                   // 렉트 배열의 첫 번째 주소
                        D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 
                        Color,  // 백 버퍼 색상
                        1.f,    // z버퍼의 초기화 값
                        0);     // 스텐실 버퍼의 초기화 값
    
    m_pGraphicDev->BeginScene();   

}

// 3. 그리기 할 대상을 종합한다.
// 4. 서피스를 교환한다.
void CGraphic_Device::Render_End(HWND hWnd)
{
    m_pGraphicDev->EndScene();
    m_pGraphicDev->Present(NULL, NULL, hWnd, NULL);
}

CGraphic_Device* CGraphic_Device::Create(HWND hWnd, WINMODE eMode, _uint iSizeX, _uint iSizeY, LPDIRECT3DDEVICE9* ppGraphicDev)
{
    CGraphic_Device* pInstance = new CGraphic_Device();

    if (FAILED(pInstance->Ready_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppGraphicDev)))
    {
        MSG_BOX("Failed to Created : CGraphic_Device");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CGraphic_Device::Free()
{
    if (0 != Safe_Release(m_pGraphicDev))
        int a = 10;

    Safe_Release(m_pSDK);

}

