#include "CSVReader.h"

#include <fstream>
#include <streambuf>
#include <sstream>

HRESULT CCSVReader::ReadCSVData(const char* FileURL, const char delimiter, vector<string>* OutLoadVec)
{
	ifstream fFileStream(FileURL);
	vector<string>	ReadDataVec = {};
	vector<_float3>	ReadVec = {};

	ReadDataVec.reserve(50);
	string Row, Col, Cell;

	if (!fFileStream.is_open())
		return E_FAIL;

	while (getline(fFileStream, Col))
	{
		stringstream sStream(Col);

		while (getline(sStream, Row, delimiter))
		{
			(*OutLoadVec).push_back(Row);
		}

		/*BASE_DATA_STRUCT ReadData;
		while (getline(sStream, Row, delimiter))
		{
			stringstream sCellStream(Row);
			_float3 vec = {};

			if (sCellStream >> vec.x >> vec.y >> vec.z)
			{
				ReadVec.push_back(vec);
			}
			else
				ReadData.szTexturePath = Row;
		}

		ReadData.Scale = ReadVec[0];
		ReadData.Rotation = ReadVec[1];
		ReadData.Position = ReadVec[2];
		(*OutLoadVec).push_back(ReadData);

		ReadDataVec.clear();
		ReadVec.clear();*/
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

