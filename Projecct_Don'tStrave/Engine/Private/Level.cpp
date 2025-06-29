#include "Level.h"
#include "GameInstance.h"
#include "File.h"

#include "CUtility.h"
#include "GameObject.h"

CLevel::CLevel(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iLevelID)
    : m_pGraphic_Device { pGraphic_Device }
    , m_pGameInstance { CGameInstance::GetInstance() }
    , m_iID { iLevelID }
{
    Safe_AddRef(m_pGameInstance);
    Safe_AddRef(m_pGraphic_Device);
}

HRESULT CLevel::Initialize()
{
    return S_OK;
}

void CLevel::Update(_float fTimeDelta)
{
}

HRESULT CLevel::Render()
{
    return S_OK;
}

HRESULT CLevel::Parse_ObejectData(const char* LoatFilePath, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLayerLevelIndex, const _wstring& strLayerTag)
{
    //¸Ê µ¥ÀÌÅÍ °¡Á®¿Í¼­ ÆÄ½Ì
    vector<BASE_DATA_STRUCT> vecBaseData;
    LoadMapData(LoatFilePath, &vecBaseData);

    for (size_t i = 0; i < vecBaseData.size(); ++i)
    {
        CGameObject::GAMEOBJECT_DESC ObjectDesc = {};
        WCHAR TexPath[MAX_PATH] = {};

        ObjectDesc.vScale = vecBaseData[i].Scale;
        ObjectDesc.vRotation = vecBaseData[i].Rotation;
        ObjectDesc.vPosition = vecBaseData[i].Position;

        CUtility::ConvertUTFToWide(vecBaseData[i].szTexturePath.c_str(), TexPath);
        ObjectDesc.TextruePath = TexPath;

        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(iPrototypeLevelIndex, strPrototypeTag, iLayerLevelIndex, strLayerTag, &ObjectDesc)))
            return E_FAIL;
    }

    return S_OK;
}

void CLevel::LoadMapData(const char* MapFilePath, vector<BASE_DATA_STRUCT>* pOut)
{
    CFile Reader = {};
    vector<string>      ReadData = {};
    vector<_float3>     ReadVec = {};

    ReadData.reserve(500);
    Reader.ReadCSVData(MapFilePath, ',', &ReadData);

    for (_uint i = 0; i < ReadData.size(); i += 5)
    {
        BASE_DATA_STRUCT Data;

        for (_uint j = i; j < i + 4; ++j)
        {
            _float3 vec;
            stringstream CellStream(ReadData[j]);

            if(j == i + 3)
                CellStream >> Data.iID;
            else
            {
                if (CellStream >> vec.x >> vec.y >> vec.z)
                    ReadVec.push_back(vec);
            }

        }

        Data.Scale = ReadVec[0];
        Data.Rotation = ReadVec[1];
        Data.szTexturePath = ReadData[i + 4];
        Data.Position = ReadVec[2];
        
        (*pOut).push_back(Data);
        ReadVec.clear();
    }
}

void CLevel::Free()
{
    __super::Free();

    Safe_Release(m_pGameInstance);
    Safe_Release(m_pGraphic_Device);
}
