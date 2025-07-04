#include "Character.h"

#include "Terrain.h"
#include "Terrian_Manager.h"

#include "XML_Manager.h"
#include "Character_Manager.h"
#include "GameInstance.h"

#include "Camera.h"


CCharacter::CCharacter(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CAinimationObject{ pGraphic_Device }
{
}

CCharacter::CCharacter(const CCharacter& Prototype)
    : CAinimationObject{ Prototype }
{
}

HRESULT CCharacter::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCharacter::Initialize(void* pArg)
{
    CLandObject::LANDOBJECT_DESC			Desc{};
    Desc.pLandTransform = static_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::TUTORIAL), TEXT("Layer_BackGround"), TEXT("Com_Transform")));
    if (Desc.pLandTransform) {
        Desc.pLandVIBuffer = static_cast<CVIBuffer*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::TUTORIAL), TEXT("Layer_BackGround"), TEXT("Com_VIBuffer")));
    }
    else {
        Desc.pLandTransform = static_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"), TEXT("Com_Transform")));
        Desc.pLandVIBuffer = static_cast<CVIBuffer*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"), TEXT("Com_VIBuffer")));
    }

    m_pCharacterInstance = CCharacter_Manager::GetInstance();

    m_pCharacterInstance->AddRef();
    m_pCharacterInstance->Add_Object(this);
    m_fAngle = 90;
    //Setting_Shader(L"BillBoard.fx");
    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;
    if (FAILED(Ready_Components()))
        return E_FAIL;


    return S_OK;
}

void CCharacter::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
  

    m_fAniTime += fTimeDelta * 500;
    m_fMoving = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
}

void CCharacter::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CCharacter::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
    _float3 Character_Pos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
    auto Terrian = m_pTerrian_Manager->GetOnTerrian(Character_Pos);
    if (Terrian)
    {
        m_pLandVIBuffer = Terrian->GetCurVIBuffer();
        m_pLandTransform = Terrian->GetTransfrom();

        SetUp_OnTerrain(m_pTransformCom, 0.f);
    }
}

HRESULT CCharacter::Render()
{
    __super::Render();

    return S_OK;
}

void CCharacter::Get_Damage(_uint iAtk)
{
    if (0 <= m_iHp) {
        m_iHp -= max(0, iAtk);
        if (0 >= m_iHp) {
            Death();
        }
        else {
            m_iHit -= max(0, iAtk);
            if (0 >= m_iHit) {
                Damage();
                m_iHit = m_iMaxHit;
            }
        }
    }
}

void CCharacter::SetDir()
{
    m_fMoving = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION) - m_fMoving;
    if (0.01f < abs(m_fMoving.x) + abs(m_fMoving.z)) {
        m_fAngle = D3DXToDegree(acosf(m_fMoving.x / sqrtf(powf(m_fMoving.x, 2) + powf(m_fMoving.z, 2))));
        if (0 < m_fMoving.z) {
            m_fAngle = 360.f - m_fAngle;
        }
    }
    D3DMATRIX view;
    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &view);
    _float3 look = view.m[2];
    look.z *= -1;
    _float lookAngle = D3DXToDegree(acosf(look.x / sqrtf(powf(look.x, 2) + powf(look.z, 2))));
    lookAngle += 180;
    if (0 < look.z) {
        lookAngle = 360.f - lookAngle;
    }
    _float fAngle = lookAngle - m_fAngle;
    if (0 > fAngle) {
        fAngle += 360;
    }
    if ((0.f <= fAngle && fAngle < 40.f) || (fAngle < 360.f && fAngle >= 310.f)) {
        m_tMoveDIr = MOVE_DIR::MOVE_UP;
    }
    else if ((fAngle < 130.f && fAngle >= 40.f)) {
        m_tMoveDIr = MOVE_DIR::MOVE_LEFT;
    }
    else if (fAngle < 220.f && fAngle >= 130.f) {
        m_tMoveDIr = MOVE_DIR::MOVE_DOWN;
    }
    else if (fAngle < 310.f && fAngle >= 220.f) {
        m_tMoveDIr = MOVE_DIR::MOVE_RIGHT;
    }
}

void CCharacter::RenderAnimation(const wstring& animName)
{
    const SCML_ANIMATION_DESC* pAnim = nullptr;
    for (auto& anim : m_tAnimation.tAnimationsVec) {
        if (0 == wcsncmp(anim.szName.c_str(), animName.c_str(), anim.szName.size())) {
            pAnim = &anim;
            break;
        }
    }
    if (!pAnim) return;
    m_iLength = pAnim->iLength;
    m_fAniTime = fmod(m_fAniTime, (_float)m_iLength);

    const KEY_DESC* pPrevKey = nullptr;
    const KEY_DESC* pNextKey = nullptr;
    vector<OBJECT_REF_DESC> timeVec = {};

    for (auto& ref : pAnim->tMainlinesVec.tKeysVec[0].tRefVec) {
        timeVec.push_back(ref);
    }
    for (auto& mainKey : pAnim->tMainlinesVec.tKeysVec) {
        if (mainKey.iTime <= m_fAniTime) {
            timeVec.clear();
            for (auto& ref : mainKey.tRefVec) {
                timeVec.push_back(ref);
            }
        }
        if (mainKey.iTime > m_fAniTime) {
            break;
        }
    }

    sort(timeVec.begin(), timeVec.end(), [](OBJECT_REF_DESC pSour, OBJECT_REF_DESC pDest)->_bool
        {
            return pSour.iZindex < pDest.iZindex;
        });
    for (OBJECT_REF_DESC timelineId : timeVec)
    {
        const KEY_DESC* pPrevKey = nullptr;
        const KEY_DESC* pNextKey = nullptr;

        for (size_t i = 0; i < pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec.size(); ++i)
        {
            if (pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec[i].iTime > m_fAniTime) {
                pPrevKey = i == 0 ? &pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec.back() : &pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec[i - 1];
                pNextKey = &pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec[i];
                break;
            }
        }

        if (!pPrevKey || !pNextKey)
        {
            if (pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec.empty()) {
                continue;
            }
            pPrevKey = &pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec.back();
            pNextKey = &pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec.front();
        }

        float t = 0.f;
        int timeDiff = pNextKey->iTime - pPrevKey->iTime;
        if (timeDiff != 0)
        {
            int timePassed = static_cast<int>(m_fAniTime) - pPrevKey->iTime;
            t = static_cast<float>(timePassed) / timeDiff;
        }

        OBJECT_DESC a = pPrevKey->tObj;
        OBJECT_DESC b = pNextKey->tObj;
        OBJECT_DESC object;
        object.iFolder = a.iFolder;
        object.iFile = a.iFile;
        object.fPos = a.fPos + (b.fPos - a.fPos) * t;
        object.fScale = a.fScale + ((b.fScale - a.fScale) * t);
        object.fAngle = a.fAngle + (fmodf(b.fAngle - a.fAngle + 540.f, 360.f) - 180.f) * t;
        if (object.iFolder >= m_tImageVec.size() || object.iFile >= m_tImageVec[object.iFolder].tFilesVec.size()) continue;
        IMAGE_FILE_DESC image = m_tImageVec[object.iFolder].tFilesVec[object.iFile];

        _float x = image.fSize.x / (image.fSize.x + image.fSize.y);
        _float y = image.fSize.y / (image.fSize.x + image.fSize.y);

        D3DXMATRIX matRotY, matPivot, matScale, matRotZ, matTrans, matBillboard, matPos, matWorld;

        _float3 pos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);

        D3DXMatrixRotationY(&matRotY, MOVE_DIR::MOVE_LEFT == m_tMoveDIr ? D3DXToRadian(180) : 0);
        D3DXMatrixTranslation(&matPivot, 0.5f - image.fPivot.x, 0.5f - image.fPivot.y, 0.f);
        D3DXMatrixScaling(&matScale, image.fSize.x * object.fScale.x / 400.f, image.fSize.y * object.fScale.y / 400.f, 1.f);
        D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(object.fAngle));
        D3DXMatrixTranslation(&matTrans, object.fPos.x / 400.f, object.fPos.y / 400.f, 0.f);

        m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matBillboard);
        matBillboard._41 = matBillboard._42 = matBillboard._43 = 0.0f;
        D3DXMatrixTranspose(&matBillboard, &matBillboard);

        D3DXMatrixTranslation(&matPos, pos.x, pos.y, pos.z);


        matWorld = matPivot * matScale * matRotZ * matTrans * matRotY * matBillboard * matPos;
        image.pTexture->Set_Texture(0);
        m_pGraphic_Device->SetTransform(D3DTS_WORLD, &matWorld);
        m_pVIBufferCom->Render();
    }
}


HRESULT CCharacter::Ready_Components()
{
    /* Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    /* Com_VIBuffer */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* Com_Collision */
    CSphere_Collision_Component::Collision_Desc Col_Desc = {};
    Col_Desc.pOwner = this;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SphereCollision"),
        TEXT("Com_SphereCollision"), reinterpret_cast<CComponent**>(&m_pCollision_Com), &Col_Desc)))
        return E_FAIL;

    return S_OK;
}

void CCharacter::Free()
{
    __super::Free();
    if (m_pCharacterInstance)
    {
        m_pCharacterInstance->Remove_Object(this);
        Safe_Release(m_pCharacterInstance);
    }
    for (auto& folder : m_tImageVec) {
        for (auto& file : folder.tFilesVec) {
            Safe_Release(file.pTexture);
        }
    }

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pCollision_Com);
}
