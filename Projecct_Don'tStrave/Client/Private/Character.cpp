#include "Character.h"
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

    m_pCharacterInstance = CCharacter_Manager::GetInstance();
    m_pCharacterInstance->AddRef();
    m_pCharacterInstance->Add_Object(this);
    m_fAngle = 90;
    //Setting_Shader(L"BillBoard.fx");
    if (FAILED(__super::Initialize(&Desc)))
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

    SetUp_OnTerrain(m_pTransformCom, 0.f);
    Compute_CamDistance(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION));
}

HRESULT CCharacter::Render()
{
    __super::Render();

    return S_OK;
}

void CCharacter::Get_Damage(_uint iAtk)
{
    if (0 <= m_iHp) {
        m_iHp -= max(0, iAtk - m_iDef);
        if (0 >= m_iHp) {
            Death();
        }
        else {
            m_iHit -= max(0, iAtk - m_iDef);
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
}
