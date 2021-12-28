#pragma once
#include "math/Math.h"
#include "Math/Vector.h"
#include "Math/Quat.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"
#include "collision/narrowphase/Penetration.h"

class UWorld;

namespace Positional
{
	inline Positional::Vec3 ToVec3(const FVector& v)
	{
		return Positional::Vec3(v.X, v.Y, v.Z);
	}


	inline FVector ToFVector(const Vec3& v)
	{
		return FVector(v.x, v.y, v.z);
	}


	inline Quat ToQuat(const FQuat& q)
	{
		return Quat(q.X, q.Y, q.Z, q.W);
	}

	inline Quat ToQuat(const FRotator &r)
	{
		auto q = r.Quaternion();
		return Quat(q.X, q.Y, q.Z, q.W);
	}

	inline FQuat ToFQuaternion(const Quat& q)
	{
		return FQuat(q.x, q.y, q.z, q.w);
	}
}