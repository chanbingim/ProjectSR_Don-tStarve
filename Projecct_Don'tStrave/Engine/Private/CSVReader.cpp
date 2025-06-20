#include "CSVReader.h"

#include <fstream>
#include <streambuf>
#include <sstream>

HRESULT CCSVReader::ReadCSVData(const char* FileURL, const char delimiter, vector<string>* OutListVec)
{
	ifstream fFileStream(FileURL);
	string Row, Col;

	if (!fFileStream.is_open())
		return E_FAIL;

	while (getline(fFileStream, Col))
	{
		stringstream sStream(Col);
		while (getline(sStream, Row, delimiter))
		{
			(*OutListVec).push_back(Row);
		}
	}

	fFileStream.close();
	return S_OK;
}

HRESULT CCSVReader::SaveCSVData(const char* SaveFilePath, vector<BASE_DATA_STRUCT>* vecSaveData)
{
	if (0 >= (*vecSaveData).size())
		return E_FAIL;

	//trunc 객체 삭제후 열기
	ofstream fFileStream(SaveFilePath, ios_base::out | ios_base::trunc);
	if (!fFileStream)
		return E_FAIL;

	for (auto& iter : *vecSaveData)
	{
		/*for (auto& point : iter.vecVertices)
		{
			fFileStream << point.vPosition.x << " " << point.vPosition.y << " " << point.vPosition.z << ",";
			fFileStream << point.vNormal.x << " " << point.vNormal.y << " " << point.vNormal.z << ",";
			fFileStream << point.vTexUV.x << " " << point.vTexUV.y << ",";
		}*/
		
		fFileStream << iter.Scale.x << " " << iter.Scale.y << " " << iter.Scale.z << ",";
		fFileStream << iter.Rotation.x << " " << iter.Rotation.y << " " << iter.Rotation.z << ",";
		fFileStream << iter.Position.x << " " << iter.Position.y << " " << iter.Position.z << ",";
		
		fFileStream << iter.szTexturePath << endl;
	}
	
	fFileStream.close();
	return S_OK;
}

