#pragma once
#include "math/Math.h"
#include "Math/Vector.h"
#include "Math/Quat.h"
#include "DrawDebugHelpers.h"

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


	inline FQuat ToFQuaternion(const Quat& q)
	{
		return FQuat(q.x, q.y, q.z, q.w);
	}

	namespace Collision
	{
		struct Polytope;
		int DrawDebugSimplex(UWorld* world, const Vec3 simplex[4], const uint8& dim, const uint8& index, const FColor& color, const uint8& depthPiority, const float& thickness);
		int DrawDebugPolytope(UWorld* world, const  Polytope& polytope, const FColor& color, const uint8& depthPiority, const float& thickness);
	}
}