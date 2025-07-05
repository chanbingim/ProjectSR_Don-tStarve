#include "Character.h"

#include "Terrain.h"
#include "Terrian_Manager.h"

#include "GameInstance.h"
#include "Camera.h"


CCharacter::CCharacter(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CAlphaObject{ pGraphic_Device }
{
}

CCharacter::CCharacter(const CCharacter& Prototype)
    : CAlphaObject{ Prototype },
    m_tAnimation{ Prototype.m_tAnimation },
    m_tImageVec{ Prototype.m_tImageVec }
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

    m_pTerrian_Manager = CTerrian_Manager::GetInstance();
    Safe_AddRef(m_pTerrian_Manager);

    m_pCharacterInstance = CCharacter_Manager::GetInstance();
    

    m_pCharacterInstance->AddRef();
    m_pCharacterInstance->Add_Object(this, FIELDOBJECT::CREATURE);
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

    Compute_CamDistance(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION));
}

HRESULT CCharacter::Render()
{
    __super::Render();

    return S_OK;
}

void CCharacter::Get_Damage(_uint iAtk)
{
    if (0 <= m_pChar->iHp) {
        m_pChar->iHp -= max(0, iAtk);
        if (0 >= m_pChar->iHp) {
            Death();
        }
        else {
            m_pChar->iHit -= max(0, iAtk);
            if (0 >= m_pChar->iHit) {
                Damage();
                m_pChar->iHit = m_pChar->iMaxHit;
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

HRESULT CCharacter::AddTexture(const _char* pScmlFilePath, const _tchar* pTextureFilePath)
{
    tinyxml2::XMLDocument doc;
    if (tinyxml2::XML_SUCCESS != doc.LoadFile(pScmlFilePath)) {
        return E_FAIL;
    }

    tinyxml2::XMLElement* root = doc.FirstChildElement("spriter_data");

    for (tinyxml2::XMLElement* folderElem = root->FirstChildElement("folder"); folderElem; folderElem = folderElem->NextSiblingElement("folder")) {
        IMAGE_FOLDER_DESC folder;
        folder.iId = folderElem->IntAttribute("id");

        folder.szName = Get_wstring(folderElem->Attribute("name"));
        for (tinyxml2::XMLElement* fileElem = folderElem->FirstChildElement("file"); fileElem; fileElem = fileElem->NextSiblingElement("file")) {

            IMAGE_FILE_DESC file;
            file.iId = fileElem->IntAttribute("id");
            file.szName = pTextureFilePath;
            file.szName += Get_wstring(fileElem->Attribute("name"));
            //if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
            //    TEXT("Com_" + file.szName), reinterpret_cast<CComponent**>(&file.pTexture))))
            //{
            //    m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
            //        CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, file.szName.c_str()));
            //    __super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
            //        TEXT("Com_" + file.szName), reinterpret_cast<CComponent**>(&file.pTexture));
            //}
            file.pTexture = nullptr;
            m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
                CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, file.szName.c_str()));
            
            file.fSize.x = fileElem->FloatAttribute("width");
            file.fSize.y = fileElem->FloatAttribute("height");
            file.fPivot.x = fileElem->FloatAttribute("pivot_x");
            file.fPivot.y = fileElem->FloatAttribute("pivot_y");
            folder.tFilesVec.push_back(file);
        }
        m_tImageVec.push_back(folder);
    }
    return S_OK;
}

HRESULT CCharacter::LoadImageFile()
{
    for (auto& folder : m_tImageVec) {
        for (auto& file : folder.tFilesVec) {
            if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
                TEXT("Com_" + file.szName), reinterpret_cast<CComponent**>(&file.pTexture))))
            {
                m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
                    CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, file.szName.c_str()));
                __super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
                    TEXT("Com_" + file.szName), reinterpret_cast<CComponent**>(&file.pTexture));
            }
        }
    }
    m_fAniTime = 0;
    m_iLength = 1000;
    return S_OK;
}

HRESULT CCharacter::LoadScml(const char* filename)
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename) != tinyxml2::XML_SUCCESS) {
        return E_FAIL;
    }
    tinyxml2::XMLElement* root = doc.FirstChildElement("spriter_data");
    tinyxml2::XMLElement* entityElem = root->FirstChildElement("entity");
    m_tAnimation.szName = Get_wstring(entityElem->Attribute("name"));
    for (tinyxml2::XMLElement* animElem = entityElem->FirstChildElement("animation"); animElem; animElem = animElem->NextSiblingElement("animation")) {
        SCML_ANIMATION_DESC anim;
        anim.szName = Get_wstring(animElem->Attribute("name"));
        anim.iLength = animElem->IntAttribute("length");

        tinyxml2::XMLElement* mainlineElem = animElem->FirstChildElement("mainline");
        for (tinyxml2::XMLElement* KeyElem = mainlineElem->FirstChildElement("key"); KeyElem; KeyElem = KeyElem->NextSiblingElement("key")) {
            MAINKEY_DESC key;
            key.iTime = KeyElem->IntAttribute("time");
            for (tinyxml2::XMLElement* objectRef = KeyElem->FirstChildElement("object_ref"); objectRef; objectRef = objectRef->NextSiblingElement("object_ref")) {
                OBJECT_REF_DESC ref;
                ref.iTimeline = objectRef->IntAttribute("timeline");
                ref.iZindex = objectRef->IntAttribute("z_index");
                key.tRefVec.push_back(ref);
            }
            anim.tMainlinesVec.tKeysVec.push_back(key);
        }
        for (tinyxml2::XMLElement* timelineElem = animElem->FirstChildElement("timeline"); timelineElem; timelineElem = timelineElem->NextSiblingElement("timeline")) {
            TIMELINE_DESC timeline;
            timeline.iId = timelineElem->IntAttribute("id");
            timeline.szName = Get_wstring(timelineElem->Attribute("name"));
            for (tinyxml2::XMLElement* keyElem = timelineElem->FirstChildElement("key"); keyElem; keyElem = keyElem->NextSiblingElement("key")) {
                KEY_DESC key;
                key.iId = keyElem->IntAttribute("id");
                key.iTime = keyElem->IntAttribute("time");
                tinyxml2::XMLElement* objElem = keyElem->FirstChildElement("object");
                if (objElem) {
                    key.tObj.iFolder = objElem->IntAttribute("folder");
                    key.tObj.iFile = objElem->IntAttribute("file");
                    key.tObj.fPos.x = objElem->FloatAttribute("x");
                    key.tObj.fPos.y = objElem->FloatAttribute("y");
                    key.tObj.fAngle = objElem->FloatAttribute("angle");
                    key.tObj.fScale.x = objElem->FloatAttribute("scale_x", 1.0f);
                    key.tObj.fScale.y = objElem->FloatAttribute("scale_y", 1.0f);
                }
                timeline.tKeysVec.push_back(key);
            }
            anim.tTimelinesVec.push_back(timeline);
            //reverse(anim.tTimelinesVec.begin(), anim.tTimelinesVec.end());
        }
        m_tAnimation.tAnimationsVec.push_back(anim);
    }
    return S_OK;
}

void CCharacter::RenderAnimation(const wstring& animName)
{
    const SCML_ANIMATION_DESC* pAnim = nullptr;
    for (auto& anim : m_tAnimation.tAnimationsVec) {
        if (0 == wcsncmp(anim.szName.c_str(), animName.c_str(), anim.szName.size())) {      // 이름 같은 애니메이션 찾기
            pAnim = &anim;
            break;
        }
    }
    if (!pAnim) return;                                                                     // 이름 같은거 없으면 사라지기
    m_iLength = pAnim->iLength;                                                             // 애니메이션 끝나는 시간 가져오기
    m_fAniTime = fmod(m_fAniTime, (_float)m_iLength);                                       // 현재 애니메이션 시간이 애니메이션 끝나는 시간보다 크면 % 계산한 느낌으로 값을 남겨줌

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

        if (object.iFolder >= m_tImageVec.size() || object.iFile >= m_tImageVec[object.iFolder].tFilesVec.size()) continue;
        IMAGE_FILE_DESC image = m_tImageVec[object.iFolder].tFilesVec[object.iFile];

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

        D3DXMatrixTranslation(&matPos, pos.x, pos.y, pos.z);                                                // 빌보드 코드 짠거


        matWorld = matPivot * matScale * matRotZ * matTrans * matRotY * matBillboard * matPos;              // 이제 전부 적용
        image.pTexture->Set_Texture(0);                                                                     // 이 오브젝트에서 쓰는 텍스쳐를 Set_Texture
        m_pGraphic_Device->SetTransform(D3DTS_WORLD, &matWorld);                                            // 적용
        m_pVIBufferCom->Render();                                                                           // 랜더
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
        m_pCharacterInstance->Remove_Object(this, FIELDOBJECT::CREATURE);
        Safe_Release(m_pCharacterInstance);
    }
    for (auto& folder : m_tImageVec) {
        for (auto& file : folder.tFilesVec) {
            Safe_Release(file.pTexture);
        }
    }
    if(m_pTerrian_Manager)
        Safe_Release(m_pTerrian_Manager);

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pCollision_Com);
}
