#include "Item_Manager.h"
#include "CUtility.h"
IMPLEMENT_SINGLETON(CItem_Manager)

CItem_Manager::CItem_Manager()
{
}

const ITEM_DATA& CItem_Manager::Get_ItemData(_uint iItemID) const
{
	if (m_ItemDataSize > iItemID)
		return m_ItemDatas[0];

	return m_ItemDatas[iItemID];
}

void CItem_Manager::LoadItemData(const char* MapFilePath)
{
    CFile Reader = {};
    vector<string>      ReadData = {};

    ReadData.reserve(500);
    Reader.ReadCSVData(MapFilePath, ',', &ReadData);

    _uint iDataSize = ReadData.size();

    for (_uint i = 0; i < iDataSize; )
    {
        ITEM_DATA Data;

        Data.iItemID = static_cast<unsigned int>(std::stoi(ReadData[i++]));

        WCHAR szData[128];
        CUtility::ConvertUTFToWide(ReadData[i++].c_str(), szData);
        Data.strName = szData;

        CUtility::ConvertUTFToWide(ReadData[i++].c_str(), szData);
        Data.strKorName = szData;

        //Data.strName = ReadData[i++];
        //Data.strKorName = ReadData[i++];
        Data.eItemType = static_cast<ITEM_TYPE>(std::stoi(ReadData[i++]));
        Data.eSlot = static_cast<SLOT>(std::stoi(ReadData[i++]));
        Data.iMaxStack = static_cast<unsigned int>(std::stoi(ReadData[i++]));
        Data.iMaterialID_1 = static_cast<unsigned int>(std::stoi(ReadData[i++]));
        Data.iNumMaterial_1 = static_cast<unsigned int>(std::stoi(ReadData[i++]));
        Data.iMaterialID_2 = static_cast<unsigned int>(std::stoi(ReadData[i++]));
        Data.iNumMaterial_2 = static_cast<unsigned int>(std::stoi(ReadData[i++]));
        Data.iMaterialID_3 = static_cast<unsigned int>(std::stoi(ReadData[i++]));
        Data.iNumMaterial_3 = static_cast<unsigned int>(std::stoi(ReadData[i++]));

        CUtility::ConvertUTFToWide(ReadData[i++].c_str(), szData);
        Data.strExplanation = szData;

        CUtility::ConvertUTFToWide(ReadData[i++].c_str(), szData);
        Data.strLines = szData;

        //Data.strExplanation = ReadData[i++];
        //Data.strLines = ReadData[i++];
        Data.iHealthChange = static_cast<unsigned int>(std::stoi(ReadData[i++]));
        Data.iHungerChange = static_cast<unsigned int>(std::stoi(ReadData[i++]));
        Data.iSanityChange = static_cast<unsigned int>(std::stoi(ReadData[i++]));
        Data.iRotTime = static_cast<unsigned int>(std::stoi(ReadData[i++]));
        Data.eFoodtype = static_cast<FOOD>(std::stoi(ReadData[i++]));
        
        m_ItemDatas.push_back(Data);
    }

    ITEM_DATA DataTemp = m_ItemDatas[1];
}

void CItem_Manager::Free()
{
	__super::Free();
}
