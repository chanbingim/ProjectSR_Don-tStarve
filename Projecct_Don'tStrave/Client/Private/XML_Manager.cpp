#include "XML_Manager.h"

#include "GameInstance.h"
#include "XMLDataStruct.h"

#include "CUtility.h"

IMPLEMENT_SINGLETON(CXML_Manager);

CXML_Manager::CXML_Manager() :
    m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pGameInstance);
}

HRESULT CXML_Manager::Initialize(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    m_pGraphic_Device = pGraphic_Device;
    Safe_AddRef(m_pGraphic_Device);

    return S_OK;
}

HRESULT CXML_Manager::LoadScml(const char* filename, void* tAnimation)
{
    if (nullptr == tAnimation)
        return E_FAIL;

    Entity* m_tAnimation = static_cast<Entity*>(tAnimation);

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename) != tinyxml2::XML_SUCCESS) {
        return E_FAIL;
    }
    tinyxml2::XMLElement* root = doc.FirstChildElement("spriter_data");
    tinyxml2::XMLElement* entityElem = root->FirstChildElement("entity");
    CUtility::ConvertUTFToWide(entityElem->Attribute("name"), m_ConvertStr);
    (*m_tAnimation).szName = m_ConvertStr;

    for (tinyxml2::XMLElement* animElem = entityElem->FirstChildElement("animation"); animElem; animElem = animElem->NextSiblingElement("animation")) {
        SCML_ANIMATION_DESC anim;
        CUtility::ConvertUTFToWide(animElem->Attribute("name"), m_ConvertStr);
        anim.szName = m_ConvertStr;
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
            CUtility::ConvertUTFToWide(timelineElem->Attribute("name"), m_ConvertStr);
            timeline.szName = m_ConvertStr;

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
        }
        (*m_tAnimation).tAnimationsVec.push_back(anim);
    }
    return S_OK;
}

HRESULT CXML_Manager::AddTexture(const _char* pScmlFilePath, const _tchar* pTextureFilePath, void*	pOutImageVec)
{
    if (nullptr == pOutImageVec)
        return E_FAIL;

    auto ImageVec = static_cast<vector<IMAGE_FOLDER_DESC>*>(pOutImageVec);

    tinyxml2::XMLDocument doc;
    if (tinyxml2::XML_SUCCESS != doc.LoadFile(pScmlFilePath)) {
        return E_FAIL;
    }

    tinyxml2::XMLElement* root = doc.FirstChildElement("spriter_data");

    for (tinyxml2::XMLElement* folderElem = root->FirstChildElement("folder"); folderElem; folderElem = folderElem->NextSiblingElement("folder")) {
        IMAGE_FOLDER_DESC folder;
        folder.iId = folderElem->IntAttribute("id");

        CUtility::ConvertUTFToWide(folderElem->Attribute("name"), m_ConvertStr);
        folder.szName = m_ConvertStr;

        for (tinyxml2::XMLElement* fileElem = folderElem->FirstChildElement("file"); fileElem; fileElem = fileElem->NextSiblingElement("file")) {

            IMAGE_FILE_DESC file;
            file.iId = fileElem->IntAttribute("id");
            file.szName = pTextureFilePath;
            CUtility::ConvertUTFToWide(fileElem->Attribute("name"), m_ConvertStr);
            wstring conv_Wstring = m_ConvertStr;
            file.szName += conv_Wstring;
            file.pTexture = nullptr;

            m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, file.szName.c_str()));

            file.fSize.x = fileElem->FloatAttribute("width");
            file.fSize.y = fileElem->FloatAttribute("height");
            file.fPivot.x = fileElem->FloatAttribute("pivot_x");
            file.fPivot.y = fileElem->FloatAttribute("pivot_y");
            folder.tFilesVec.push_back(file);
        }
        (*ImageVec).push_back(folder);
    }
    return S_OK;
}

void CXML_Manager::Free()
{
    __super::Free();

    Safe_Release(m_pGameInstance);
    Safe_Release(m_pGraphic_Device);
}
