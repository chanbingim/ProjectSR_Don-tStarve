#include "CMath.h"

_float3 CMath::LerpVec3(_float3& StartVec, _float3& EndVec, _float t)
{
    _float3 NewVec3 = {};
    D3DXVec3Lerp(&NewVec3, &StartVec, &EndVec, Clamp<_float>(t, 0.f, 1.f));

    return NewVec3;
}

_float CMath::LerpTurn(_float StartAngle, _float EndAngle, _float t)
{
    return Lerp<_float>(StartAngle, EndAngle, Clamp<_float>(t, 0.f, 1.f));
}
