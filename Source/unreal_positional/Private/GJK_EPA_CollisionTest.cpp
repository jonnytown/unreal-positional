// Fill out your copyright notice in the Description page of Project Settings.


#include "GJK_EPA_CollisionTest.h"
#include "PositionalUtil.h"
#include "PositionalDrawDebug.h"
#include "collision/narrowphase/Simplex.h"
#include "collision/narrowphase/Polytope.h"
#include "collision/narrowphase/Penetration.h"

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
		Collision::GJK_EPA_CSO simplex;
		simplex.vertCount = Count;
		simplex.vertices[0].p = ToVec3(A.Get()->GetActorLocation());
		simplex.vertices[1].p = ToVec3(B.Get()->GetActorLocation());
		simplex.vertices[2].p = ToVec3(C.Get()->GetActorLocation());
		simplex.vertices[3].p = ToVec3(D.Get()->GetActorLocation());

		DrawDebugSimplex(GetWorld(), simplex.vertices, Count - 1, 0, FColor::Black, 1U, 1.0F);

		UInt8 nearDim, nearIdx;
		const Vec3 near = Collision::Simplex::nearest(simplex, nearDim, nearIdx);
		DrawDebugPoint(GetWorld(), ToFVector(near), 4.0F, FColor::Black, false, -1, 3U);
		DrawDebugSimplex(GetWorld(), simplex.vertices, nearDim, nearIdx, FColor::Green, 2U, 0.5F);
		
	}
}

bool AGJK_EPA_CollisionTest::ShouldTickIfViewportsOnly() const
{
	return true;
}

