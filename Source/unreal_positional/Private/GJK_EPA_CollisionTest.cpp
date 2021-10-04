// Fill out your copyright notice in the Description page of Project Settings.


#include "GJK_EPA_CollisionTest.h"
#include "PositionalUtil.h"
#include "collision/narrowphase/Simplex.h"

namespace Positional
{
	namespace Collision
	{
		int DrawDebugSimplex(UWorld* world, const Vec3 simplex[4], const uint8& dim, const uint8& index, const FColor& color, const uint8& depthPiority, const float& thickness)
		{
			switch (dim)
			{
			case 0:
			{
				DrawDebugPoint(world, ToFVector(simplex[index]), 8.0f, color, false, -1, depthPiority);
				break;
			}
			case 1:
			{
				auto seg = Collision::Simplex::segs[index];
				DrawDebugLine(world, ToFVector(simplex[seg[0]]), ToFVector(simplex[seg[1]]), color, false, -1, depthPiority, thickness);
				break;
			}
			case 2:
			{
				auto tri = Collision::Simplex::tris[index];
				DrawDebugLine(world, ToFVector(simplex[tri[0]]), ToFVector(simplex[tri[1]]), color, false, -1, depthPiority, thickness);
				DrawDebugLine(world, ToFVector(simplex[tri[0]]), ToFVector(simplex[tri[2]]), color, false, -1, depthPiority, thickness);
				DrawDebugLine(world, ToFVector(simplex[tri[1]]), ToFVector(simplex[tri[2]]), color, false, -1, depthPiority, thickness);
				break;
			}
			case 3:
			{
				DrawDebugLine(world, ToFVector(simplex[0]), ToFVector(simplex[1]), color, false, -1, depthPiority, thickness);
				DrawDebugLine(world, ToFVector(simplex[0]), ToFVector(simplex[2]), color, false, -1, depthPiority, thickness);
				DrawDebugLine(world, ToFVector(simplex[0]), ToFVector(simplex[3]), color, false, -1, depthPiority, thickness);
				DrawDebugLine(world, ToFVector(simplex[1]), ToFVector(simplex[2]), color, false, -1, depthPiority, thickness);
				DrawDebugLine(world, ToFVector(simplex[1]), ToFVector(simplex[3]), color, false, -1, depthPiority, thickness);
				DrawDebugLine(world, ToFVector(simplex[2]), ToFVector(simplex[3]), color, false, -1, depthPiority, thickness);
				break;
			}
			default:
				break;
			}
			return 0;
		}

		int DrawDebugPolytope(UWorld* world, const Collision::Polytope& polytope, const FColor& color, const uint8& depthPiority, const float& thickness)
		{

			for (int i = 0; i < polytope.tris.size(); i += 3)
			{
				const auto a = ToFVector(polytope.vertices[polytope.tris[i]]);
				const auto b = ToFVector(polytope.vertices[polytope.tris[i + 1]]);
				const auto c = ToFVector(polytope.vertices[polytope.tris[i + 2]]);
				DrawDebugLine(world, a, b, color, false, -1, depthPiority, thickness);
				DrawDebugLine(world, b, c, color, false, -1, depthPiority, thickness);
				DrawDebugLine(world, c, a, color, false, -1, depthPiority, thickness);

				const auto o = (a + b + c) * 0.3333333333f;

				DrawDebugDirectionalArrow(world, o, o + ToFVector(polytope.normals[i / 3].normalized()) * 10, 10, color, false, -1, depthPiority, 0);
			}

			Float lenSq; UInt16 idx;
			const Vec3 near = polytope.nearest(lenSq, idx);
			DrawDebugPoint(world, ToFVector(near), thickness * 5, color, false, -1, depthPiority);
			return 0;
		}
	}
}

// Sets default values
AGJK_EPA_CollisionTest::AGJK_EPA_CollisionTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Count = 4;
}

// Called every frame
void AGJK_EPA_CollisionTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (A.IsValid() && B.IsValid() && C.IsValid() && D.IsValid() && Count > 0 && Count <= 4)
	{
		Collision::Simplex simplex;
		simplex.count = Count;
		simplex.vertices[0] = ToVec3(A.Get()->GetActorLocation());
		simplex.vertices[1] = ToVec3(B.Get()->GetActorLocation());
		simplex.vertices[2] = ToVec3(C.Get()->GetActorLocation());
		simplex.vertices[3] = ToVec3(D.Get()->GetActorLocation());

		Collision::DrawDebugSimplex(GetWorld(), simplex.vertices, Count - 1, 0, FColor::Black, 1U, 1.0F);

		UInt8 nearDim, nearIdx;
		const Vec3 near = simplex.nearest(nearDim, nearIdx);
		DrawDebugPoint(GetWorld(), ToFVector(near), 4.0F, FColor::Black, false, -1, 3U);
		Collision::DrawDebugSimplex(GetWorld(), simplex.vertices, nearDim, nearIdx, FColor::Green, 2U, 0.5F);
		
	}
}

bool AGJK_EPA_CollisionTest::ShouldTickIfViewportsOnly() const
{
	return true;
}

