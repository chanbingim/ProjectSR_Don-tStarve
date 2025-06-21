#pragma once

#include "Engine_Defines.h"

NS_BEGIN(Engine)

class ENGINE_DLL CMath
{
public :
	static _float3		LerpVec3(_float3& StartVec, _float3& EndVec, _float t);
	static _float		LerpTurn(_float StartAngle, _float EndAngle, _float t);

	template<typename T>
	static T			Lerp(T first, T second, _float t);
};
NS_END

// t = 0.f ~ 1.f°ª
template<typename T>
inline T CMath::Lerp(T first, T second, _float t)
{
	return first + (second - first) * t;
}
