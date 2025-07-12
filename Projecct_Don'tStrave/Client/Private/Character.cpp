#include "Character.h"
#include "Terrain.h"
#include "Terrian_Manager.h"
#include "XML_Manager.h"
#include "GameInstance.h"
#include "Clock.h"
#include "House.h"
#include "Enviornment_Object.h"

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
    Desc.pLandTransform = static_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"), TEXT("Com_Transform")));
    if (Desc.pLandTransform) {
        Desc.pLandVIBuffer = static_cast<CVIBuffer*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"), TEXT("Com_VIBuffer")));
    }
    else {
        Desc.pLandTransform = static_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"), TEXT("Com_Transform")));
        Desc.pLandVIBuffer = static_cast<CVIBuffer*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"), TEXT("Com_VIBuffer")));
    }

    m_fAngle = rand() % 360;
    //Setting_Shader(L"BillBoard.fx");
    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;
    if (FAILED(Ready_Components()))
        return E_FAIL;
    return S_OK;
}

HRESULT CCharacter::Initialize_Late()
{
    auto GroundObejcts = m_pGameInstance->GetAllObejctsToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_UserInterface"));
    if (!GroundObejcts->empty()) {
        for (auto& object : (*GroundObejcts)) {
            if (dynamic_cast<CClock*>(object)) {
                m_pDate = dynamic_cast<CClock*>(object)->Get_Date();
                m_pTime = dynamic_cast<CClock*>(object)->Get_Time();
            }
        }
    }
    return S_OK;
}

void CCharacter::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);

    m_tDamage.Direaction.x = (90 <= m_fAngle && 270 > m_fAngle) ? -1.f : 1.f;
    m_tDamage.Direaction.z = 180 >= m_fAngle ? -1.f : 1.f;

    m_fMoving = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
}

void CCharacter::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CCharacter::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
    if (!m_isDead)
    {
        auto Terrian = m_pTerrian_Manager->GetOnTerrian(m_pChar->fPos);
        if (Terrian)
        {
            m_pLandVIBuffer = Terrian->GetCurVIBuffer();
            m_pLandTransform = Terrian->GetTransfrom();

            SetUp_OnTerrain(m_pTransformCom, 0.f);
            m_pChar->fPos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
        }
        else {
            m_pChar->fPos = m_fMoving;
            m_pTransformCom->SetPosition(m_pChar->fPos);
        }
        m_bCol = false;
    }
}

HRESULT CCharacter::Render()
{
    __super::Render();
    return S_OK;
}

void CCharacter::Damage(void* pArg)
{
    __super::Damage(pArg);
    if (0 <= m_pChar->iHp) {
        DAMAGE_DATA_BASE DamageBase = {};
        if (nullptr != pArg)
            DamageBase = *static_cast<DAMAGE_DATA_BASE*>(pArg);
        m_pChar->iHp -= max(0, DamageBase.Damage);
        if (0 >= m_pChar->iHp) {
            Death();
        }
        else {
            m_pChar->iHit -= max(0, DamageBase.Damage);
            if (0 >= m_pChar->iHit) {
                Hit();
                m_pChar->iHit = m_pChar->iMaxHit;
            }
        }
    }
}

void CCharacter::SetDir()
{
    
    _float3 fDir = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION) - m_fMoving;
    if (0.01f < abs(fDir.x) + abs(fDir.z)) {
        m_fAngle = D3DXToDegree(acosf(fDir.x / D3DXVec3Length(&fDir)));
        if (0 < fDir.z) {
            m_fAngle = 360.f - m_fAngle;
        }
    }
    D3DMATRIX view;
    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &view);
    _float3 look = view.m[2];
    look.z *= -1;
    _float lookAngle = D3DXToDegree(acosf(look.x / D3DXVec3Length(&look)));
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

void CCharacter::RenderAnimation(const wstring& animName, Entity& tEntity, vector<IMAGE_FOLDER_DESC>& tImageVec)
{
    const SCML_ANIMATION_DESC* pAnim = nullptr;

    _matrix matRotY{}, matPos{}, matBillboard{};

    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matBillboard);
    matBillboard._41 = matBillboard._42 = matBillboard._43 = 0.0f;
    D3DXMatrixTranspose(&matBillboard, &matBillboard);

    _float3 pos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
    D3DXMatrixTranslation(&matPos, pos.x, pos.y, pos.z);
   
    D3DXMatrixRotationY(&matRotY, MOVE_DIR::MOVE_LEFT == m_tMoveDIr ? D3DXToRadian(180) : 0);
    for (auto& anim : tEntity.tAnimationsVec) {
        if (0 == wcsncmp(anim.szName.c_str(), animName.c_str(), max(animName.size(), anim.szName.size()))) {      // 이름 같은 애니메이션 찾기
            pAnim = &anim;
            break;
        }
    }
    if (!pAnim) return;                                                                     // 이름 같은거 없으면 사라지기
    m_iLength = pAnim->iLength;                                                             // 애니메이션 끝나는 시간 가져오기
    m_fAniTime =m_fAniTime % m_iLength;                                       // 현재 애니메이션 시간이 애니메이션 끝나는 시간보다 크면 % 계산한 느낌으로 값을 남겨줌

    vector<OBJECT_REF_DESC> timeVec = {};

    for (auto& ref : pAnim->tMainlinesVec.tKeysVec[0].tRefVec) {
        timeVec.push_back(ref);                                                             
    }                                                                                      
    for (auto& mainKey : pAnim->tMainlinesVec.tKeysVec) {
        if (mainKey.iTime <= m_fAniTime) {                          // Mainlines에 있는 현재 시간 기준으로 나오는 오브젝트들을 가져와줌
            timeVec.clear();                                        // 얘네가 그냥 애니메이션의 타임라인은 모든 시간대로 박혀있어서 몇몇 오브젝트가 사라지는걸 구현 못해서 이렇게 걸러줌
            for (auto& ref : mainKey.tRefVec) {
                timeVec.push_back(ref);
            }
        }
        if (mainKey.iTime > m_fAniTime) {
            break;
        }
    } 

    sort(timeVec.begin(), timeVec.end(), [](OBJECT_REF_DESC pSour, OBJECT_REF_DESC pDest)->_bool    // ZIndex 정렬
        {
            return pSour.iZindex < pDest.iZindex;
        });
    for (OBJECT_REF_DESC timelineId : timeVec)  // 위에서 걸러서 나올 놈들만 나오는 오브젝트들만 for문에 돌리기
    {
        const KEY_DESC* pPrevKey = nullptr;
        const KEY_DESC* pNextKey = nullptr;

        for (size_t i = 0; i < pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec.size(); ++i)     // 해당 오브젝트에서 현재 시간에 쓸 타임라인들 가져오기
        {
            if (pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec[i].iTime > m_fAniTime) {
                pPrevKey = i == 0 ? &pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec.back() : &pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec[i - 1];
                pNextKey = &pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec[i];
                break;
            }
        }

        if (!pPrevKey || !pNextKey)                 // null이 있는지 확인
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

        // t <- 첫시간과 다음 시간 사이의 현재 시간 비율

        OBJECT_DESC a = pPrevKey->tObj;
        OBJECT_DESC b = pNextKey->tObj;
        OBJECT_DESC object;
        object.iFolder = a.iFolder;
        object.iFile = a.iFile;
        object.fPos = a.fPos + (b.fPos - a.fPos) * t;
        object.fScale = a.fScale + ((b.fScale - a.fScale) * t);
        object.fAngle = a.fAngle + (fmodf(b.fAngle - a.fAngle + 540.f, 360.f) - 180.f) * t;

        // 구해둔 t로 첫 시간과 다음 시간 사이 현재 시간일때 나올 값들을 구해줌
        // Angle은 가끔 360도 확 돌아서 여러가지 시도해보다가 저리 됨

            if (object.iFolder >= tImageVec.size() || object.iFile >= tImageVec[object.iFolder].tFilesVec.size()) continue;
            IMAGE_FILE_DESC image = tImageVec[object.iFolder].tFilesVec[object.iFile];

        D3DXMATRIX  matPivot, matScale, matRotZ, matTrans, matWorld;

        // 왼쪽 오른쪽 확인해서 180도 회전
        D3DXMatrixTranslation(&matPivot, 0.5f - image.fPivot.x, 0.5f - image.fPivot.y, 0.f);                // pivot 적용 기본이 사실상 0.5로 들어가있어서 0.5에서 빼줘야함
        D3DXMatrixScaling(&matScale, image.fSize.x * object.fScale.x * 0.0025f, image.fSize.y * object.fScale.y * 0.0025f, 1.f);    // 이미지 크기와 애니메이션에서 조정한 scale 적용 그냥 쓰면 너무 커서 400으로 나눔
        D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(object.fAngle));                                         // 애니메이션에 들어간 회전 적용
        D3DXMatrixTranslation(&matTrans, object.fPos.x * 0.0025f, object.fPos.y * 0.0025f, 0.f);                // 애니메이션에 들어간 이동 적용 이것도 그냥 넣으면 너무 커서 400으로 나눔

        matWorld = matPivot * matScale * matRotZ * matTrans * matRotY * matBillboard * matPos;              // 이제 전부 적용


        image.pTexture->Set_Texture(0);                                                                     // 이 오브젝트에서 쓰는 텍스쳐를 Set_Texture
        m_pGraphic_Device->SetTransform(D3DTS_WORLD, &matWorld);                                            // 적용
        m_pVIBufferCom->Render();                                                                           // 랜더
    }
}

D3DXMATRIX CCharacter::GetTorchAnimation(const wstring& animName, Entity tEntity, vector<IMAGE_FOLDER_DESC> tImageVec)
{
    D3DXMATRIX torchMat = {};
    wstring torch = TEXT("../Bin/Resources/Textures/Player/Item/torch/swap_torch");
    const SCML_ANIMATION_DESC* pAnim = nullptr;
    for (auto& anim : tEntity.tAnimationsVec) {
        if (0 == wcsncmp(anim.szName.c_str(), animName.c_str(), max(animName.size(), anim.szName.size()))) {      // 이름 같은 애니메이션 찾기
            pAnim = &anim;
            break;
        }
    }
    if (!pAnim) return torchMat;                                                                     // 이름 같은거 없으면 사라지기
    m_iLength = pAnim->iLength;                                                             // 애니메이션 끝나는 시간 가져오기
    m_fAniTime = m_fAniTime % m_iLength;                                       // 현재 애니메이션 시간이 애니메이션 끝나는 시간보다 크면 % 계산한 느낌으로 값을 남겨줌

    vector<OBJECT_REF_DESC> timeVec = {};

    for (auto& ref : pAnim->tMainlinesVec.tKeysVec[0].tRefVec) {
        timeVec.push_back(ref);
    }
    for (auto& mainKey : pAnim->tMainlinesVec.tKeysVec) {
        if (mainKey.iTime <= m_fAniTime) {                          // Mainlines에 있는 현재 시간 기준으로 나오는 오브젝트들을 가져와줌
            timeVec.clear();                                        // 얘네가 그냥 애니메이션의 타임라인은 모든 시간대로 박혀있어서 몇몇 오브젝트가 사라지는걸 구현 못해서 이렇게 걸러줌
            for (auto& ref : mainKey.tRefVec) {
                timeVec.push_back(ref);
            }
        }
        if (mainKey.iTime > m_fAniTime) {
            break;
        }
    }

    sort(timeVec.begin(), timeVec.end(), [](OBJECT_REF_DESC pSour, OBJECT_REF_DESC pDest)->_bool    // ZIndex 정렬
        {
            return pSour.iZindex < pDest.iZindex;
        });
    for (OBJECT_REF_DESC timelineId : timeVec)  // 위에서 걸러서 나올 놈들만 나오는 오브젝트들만 for문에 돌리기
    {
        const KEY_DESC* pPrevKey = nullptr;
        const KEY_DESC* pNextKey = nullptr;

        for (size_t i = 0; i < pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec.size(); ++i)     // 해당 오브젝트에서 현재 시간에 쓸 타임라인들 가져오기
        {
            if (pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec[i].iTime > m_fAniTime) {
                pPrevKey = i == 0 ? &pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec.back() : &pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec[i - 1];
                pNextKey = &pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec[i];
                break;
            }
        }

        if (!pPrevKey || !pNextKey)                 // null이 있는지 확인
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

        // t <- 첫시간과 다음 시간 사이의 현재 시간 비율

        OBJECT_DESC a = pPrevKey->tObj;
        OBJECT_DESC b = pNextKey->tObj;
        OBJECT_DESC object;
        object.iFolder = a.iFolder;
        object.iFile = a.iFile;
        object.fPos = a.fPos + (b.fPos - a.fPos) * t;
        object.fScale = a.fScale + ((b.fScale - a.fScale) * t);
        object.fAngle = a.fAngle + (fmodf(b.fAngle - a.fAngle + 540.f, 360.f) - 180.f) * t;

        // 구해둔 t로 첫 시간과 다음 시간 사이 현재 시간일때 나올 값들을 구해줌
        // Angle은 가끔 360도 확 돌아서 여러가지 시도해보다가 저리 됨

        if (object.iFolder >= tImageVec.size() || object.iFile >= tImageVec[object.iFolder].tFilesVec.size()) continue;
        IMAGE_FILE_DESC image = tImageVec[object.iFolder].tFilesVec[object.iFile];

        D3DXMATRIX matRotY, matPivot, matScale, matRotZ, matTrans, matBillboard, matPos, matWorld;

        _float3 pos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);

        D3DXMatrixRotationY(&matRotY, MOVE_DIR::MOVE_LEFT == m_tMoveDIr ? D3DXToRadian(180) : 0);           // 왼쪽 오른쪽 확인해서 180도 회전
        D3DXMatrixTranslation(&matPivot, 0.5f - image.fPivot.x, 0.5f - image.fPivot.y, 0.f);                // pivot 적용 기본이 사실상 0.5로 들어가있어서 0.5에서 빼줘야함
        D3DXMatrixScaling(&matScale, image.fSize.x * object.fScale.x / 400.f, image.fSize.y * object.fScale.y / 400.f, 1.f);    // 이미지 크기와 애니메이션에서 조정한 scale 적용 그냥 쓰면 너무 커서 400으로 나눔
        D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(object.fAngle));                                         // 애니메이션에 들어간 회전 적용
        D3DXMatrixTranslation(&matTrans, object.fPos.x / 400.f, object.fPos.y / 400.f, 0.f);                // 애니메이션에 들어간 이동 적용 이것도 그냥 넣으면 너무 커서 400으로 나눔

        m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matBillboard);
        matBillboard._41 = matBillboard._42 = matBillboard._43 = 0.0f;
        D3DXMatrixTranspose(&matBillboard, &matBillboard);

                                                    // 빌보드 코드 짠거


        matWorld = matPivot * matScale * matRotZ * matTrans * matRotY * matBillboard * matPos;              // 이제 전부 적용
        
        image.pTexture->Set_Texture(0);                                                                     // 이 오브젝트에서 쓰는 텍스쳐를 Set_Texture
        m_pGraphic_Device->SetTransform(D3DTS_WORLD, &matWorld);                                            // 적용

        if (0 == wcsncmp(image.szName.c_str(), torch.c_str(), torch.size())) {
            D3DXMatrixTranslation(&matPivot, 0.f, 1.f, 0.f);
            D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 1.f);
            D3DXMATRIX matRotV, matPivot2, matPivot3;
            D3DXMatrixTranslation(&matPivot2, 0.f, 0.4f, 0.f);
            D3DXMatrixTranslation(&matPivot3, 0.f, -0.4f, 0.f);
            D3DXMatrixRotationZ(&matRotV, D3DXToRadian(-object.fAngle));
            torchMat = matPivot2 * matRotV * matPivot3 * matPivot * matScale * matRotZ * matTrans * matRotY * matBillboard * matPos;
        }
        m_pVIBufferCom->Render();                                                                           // 랜더
    }
    return torchMat;
}

void CCharacter::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
    _float3 pos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
    _float3 pos2 = HitActor->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION);
    _float3 transform = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION) - HitActor->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION);
    _float distance = D3DXVec3Length(&transform);
    if (distance < 0.3f) {
        if (dynamic_cast<CCharacter*>(HitActor) && !dynamic_cast<CHouse*>(HitActor)) {
            m_pChar->fPos += *D3DXVec3Normalize(&transform, &transform) * ((0.3f - distance) / 2);
            m_pTransformCom->SetPosition(m_pChar->fPos);
            CCharacter* actor = dynamic_cast<CCharacter*>(HitActor);
            actor->Get_Char()->fPos -= *D3DXVec3Normalize(&transform, &transform) * ((0.3f - distance) / 2);
            actor->GetTransfrom()->SetPosition(actor->Get_Char()->fPos);
        }
        else if(!dynamic_cast<CEnviornment_Object*>(HitActor) || CEnviornment_Object::Enviornment_TYPE::GRASS != dynamic_cast<CEnviornment_Object*>(HitActor)->GetEnviornMentType()) {
            m_pChar->fPos += *D3DXVec3Normalize(&transform, &transform) * (0.3f - distance);
            m_pTransformCom->SetPosition(m_pChar->fPos);
        }
    }
}


HRESULT CCharacter::Ready_Components()
{
    /* Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.0f) };
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

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pCollision_Com);
}
