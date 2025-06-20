#pragma once

#include "FileReader.h"
#include "BaseStruct.h"

NS_BEGIN(Engine)
class CCSVReader final : public CFileReader
{
public:
	HRESULT	ReadCSVData(const char* FileURL, const char delimiter, vector<string>* OutListVec);

	HRESULT SaveCSVData(const char* SaveFilePath, vector<BASE_DATA_STRUCT>* vecSaveData );
};
NS_END
