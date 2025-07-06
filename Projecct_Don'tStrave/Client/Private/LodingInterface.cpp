#include "LodingInterface.h"

#include "GameInstance.h"
#include "LodingSpinner.h"

CLodingInterface::CLodingInterface(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CUserInterface(pGraphic_Device)
{
}

CLodingInterface::CLodingInterface(const CLodingInterface& rhs) :
    CUserInterface(rhs)
{
}

HRESULT CLodingInterface::Initialize_Prototype()
{
   

    return S_OK;
}

HRESULT CLodingInterface::Initialize(void* pArg)
{
    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(ADD_Tips()))
        return E_FAIL;

  

    m_iTextureIndex = rand() % m_pTexture_Com->StoredTextureCount();

    m_fSizeX = g_iWinSizeX;
    m_fSizeY = g_iWinSizeY;

    m_fX = 0;
    m_fY = 0;
    
    Update_Tips();
    return S_OK;
}

void CLodingInterface::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CLodingInterface::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    if (m_pGameInstance->KeyDown(VK_LBUTTON))
    {
        Update_Tips();
    }
    m_pSinner->Update(fTimeDelta);
}

void CLodingInterface::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    UpdatePosition();
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);
}

HRESULT CLodingInterface::Render()
{
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());
    m_pTexture_Com->Set_Texture(m_iTextureIndex);
    m_pVIBuffer_Com->Render();

    D3DXCOLOR white = { 1.f,1.f,1.f,1.f };
    m_pGameInstance->Render_Font(TEXT("Date_40"), m_printTEXT.c_str(), &m_FontRect, white);

    if (!m_bFinishedLoad)
        m_pSinner->Render();
    return S_OK;
}

void CLodingInterface::Finished_Loading(_bool IsFinish)
{
    m_bFinishedLoad = IsFinish;
}

HRESULT CLodingInterface::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"), 
        TEXT("Com_Transform"), (CComponent**)&m_pTransform_Com)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer_Com)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Loading_Texture"),
        TEXT("Com_Texture"), (CComponent**)&m_pTexture_Com)))
        return E_FAIL;

    CLodingSpinner::SPINNER_DESC SpinnerData = {};
    m_pSinner = CLodingSpinner::Create(m_pGraphic_Device, nullptr);

    return S_OK;
}

HRESULT CLodingInterface::ADD_Tips()
{
    m_vecTips.reserve(20);

    m_vecTips.emplace_back(L"첫날엔 Flint, 풀, 나뭇가지부터 챙기세요.");
    m_vecTips.emplace_back(L"꽃 12개로 머리장식을 만들어 정신력을 유지하세요.");
    m_vecTips.emplace_back(L"음식은 그냥 먹기보다 크록팟에 요리하세요.");
    m_vecTips.emplace_back(L"밤엔 반드시 불빛이 있어야 합니다. 횃불 필수!");
    m_vecTips.emplace_back(L"과학기계는 초반 발전의 핵심입니다.");
    m_vecTips.emplace_back(L"고기+채소+과일 조합으로 미트볼을 만드세요.");
    m_vecTips.emplace_back(L"하운드가 오면 돼지나 소 떼에게 유도하세요.");
    m_vecTips.emplace_back(L"정신력이 낮아지면 환각 적이 공격합니다.");
    m_vecTips.emplace_back(L"지도에 있는 웜홀은 빠른 이동에 유용합니다.");
    m_vecTips.emplace_back(L"초기 캠프는 초원, 숲 근처에 만드는 게 좋습니다.");

    return S_OK;
}

void CLodingInterface::Update_Tips()
{
    _uint       iIndex = rand() % m_vecTips.size();
    m_printTEXT = m_vecTips[iIndex];

    m_FontCenter = { m_fSizeX * 0.5f, m_fSizeY * 0.5f  + 200};
    size_t       iFontLength = m_printTEXT.length();
    _float2     StrsizeHalf = { 30.f * iFontLength, 15.f};

    m_FontRect = {
       static_cast<long>(m_FontCenter.x - StrsizeHalf.x),
       static_cast<long>(m_FontCenter.y - StrsizeHalf.y),
       static_cast<long>(m_FontCenter.x + StrsizeHalf.x),
       static_cast<long>(m_FontCenter.y + StrsizeHalf.y),
    };
}

CLodingInterface* CLodingInterface::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CLodingInterface* pInstance = new CLodingInterface(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : LOADING INTERFACE");
    }

    return pInstance;
}

CUserInterface* CLodingInterface::Clone(void* pArg)
{
    CLodingInterface* pInstance = new CLodingInterface(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : LOADING INTERFACE");
    }

    return pInstance;
}

void CLodingInterface::Free()
{
    __super::Free();

    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pVIBuffer_Com);

    Safe_Release(m_pSinner);
}
