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
	int DrawDebugSimplex(UWorld* world, const Collision::Vertex simplex[4], const uint8& dim, const uint8& index, const FColor& color, const uint8& depthPiority, const float& thickness);
	int DrawDebugPolytope(UWorld* world, const Collision::GJK_EPA_CSO& polytope, const FColor& color, const uint8& depthPiority, const float& thickness);
}