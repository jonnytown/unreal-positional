#include "PositionalDrawDebug.h"
#include "PositionalUtil.h"
#include "collision/narrowphase/Simplex.h"
#include "collision/narrowphase/Polytope.h"

namespace Positional
{

	int DrawDebugSimplex(UWorld *world, const Collision::Vertex simplex[4], const uint8 &dim, const uint8 &index, const FColor &color, const uint8 &depthPiority, const float &thickness)
	{
		switch (dim)
		{
		case 0:
		{
			DrawDebugPoint(world, ToFVector(simplex[index].p), 8.0f, color, false, -1, depthPiority);
			break;
		}
		case 1:
		{
			auto seg = Collision::Simplex::segs[index];
			DrawDebugLine(world, ToFVector(simplex[seg[0]].p), ToFVector(simplex[seg[1]].p), color, false, -1, depthPiority, thickness);
			break;
		}
		case 2:
		{
			auto tri = Collision::Simplex::tris[index];
			DrawDebugLine(world, ToFVector(simplex[tri[0]].p), ToFVector(simplex[tri[1]].p), color, false, -1, depthPiority, thickness);
			DrawDebugLine(world, ToFVector(simplex[tri[0]].p), ToFVector(simplex[tri[2]].p), color, false, -1, depthPiority, thickness);
			DrawDebugLine(world, ToFVector(simplex[tri[1]].p), ToFVector(simplex[tri[2]].p), color, false, -1, depthPiority, thickness);
			break;
		}
		case 3:
		{
			DrawDebugLine(world, ToFVector(simplex[0].p), ToFVector(simplex[1].p), color, false, -1, depthPiority, thickness);
			DrawDebugLine(world, ToFVector(simplex[0].p), ToFVector(simplex[2].p), color, false, -1, depthPiority, thickness);
			DrawDebugLine(world, ToFVector(simplex[0].p), ToFVector(simplex[3].p), color, false, -1, depthPiority, thickness);
			DrawDebugLine(world, ToFVector(simplex[1].p), ToFVector(simplex[2].p), color, false, -1, depthPiority, thickness);
			DrawDebugLine(world, ToFVector(simplex[1].p), ToFVector(simplex[3].p), color, false, -1, depthPiority, thickness);
			DrawDebugLine(world, ToFVector(simplex[2].p), ToFVector(simplex[3].p), color, false, -1, depthPiority, thickness);
			break;
		}
		default:
			break;
		}
		return 0;
	}

	int DrawDebugPolytope(UWorld *world, const Collision::GJK_EPA_CSO &polytope, const FColor &color, const uint8 &depthPiority, const float &thickness)
	{
		for (int i = 0, count = polytope.triCount * 3; i < count; i += 3)
		{
			const auto a = ToFVector(polytope.vertices[polytope.tris[i]].p);
			const auto b = ToFVector(polytope.vertices[polytope.tris[i + 1]].p);
			const auto c = ToFVector(polytope.vertices[polytope.tris[i + 2]].p);
			DrawDebugLine(world, a, b, color, false, -1, depthPiority, thickness);
			DrawDebugLine(world, b, c, color, false, -1, depthPiority, thickness);
			DrawDebugLine(world, c, a, color, false, -1, depthPiority, thickness);

			const auto o = (a + b + c) * 0.3333333333f;

			DrawDebugDirectionalArrow(world, o, o + ToFVector(polytope.normals[i / 3].normalized()) * 10, 10, color, false, -1, depthPiority, 0);
		}

		Float lenSq;
		UInt32 idx;
		const Vec3 near = Collision::Polytope::nearest(polytope, lenSq, idx);
		DrawDebugPoint(world, ToFVector(near), thickness * 5, color, false, -1, depthPiority);
		return 0;
	}

}