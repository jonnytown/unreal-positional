// Fill out your copyright notice in the Description page of Project Settings.


#include "RaycastGeomTest.h"
#include "DrawDebugHelpers.h"
#include "math/Math.h"
#include "PositionalCollider.h"
#include "PositionalUtil.h"

const float NormalLength = 20.0f;

// Sets default values for this component's properties
ARaycastGeomTest::ARaycastGeomTest()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;
	// ...
	Distance = 1000;
}

// Called every frame
void ARaycastGeomTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// ...
	if (RayOrigin.IsValid() && RayDirection.IsValid())
	{
		auto o = RayOrigin.Get()->GetActorLocation();
		auto e = RayDirection.Get()->GetActorLocation();
		auto n = e - o;
		n.Normalize();

		TArray<PositionalRaycastResult> results;
		if (World.IsValid())
		{
			World.Get()->Raycast(o, n, 0xFFFFFFFFui32, Distance, results);

			for (const auto& r : results)
			{
				DrawDebugLine(GetWorld(), r.point, r.point + r.normal * NormalLength, FColor::Green, false, -1.0F, 0, 1.0F);

				FVector center, extents;
				r.collider->GetBounds(center, extents);
				DrawDebugBox(GetWorld(), center, extents, FColor::Green, false, -1, 1, 1);
			}
		}

		DrawDebugDirectionalArrow(GetWorld(), o, o + n*Distance, 10.F, results.Num() ? FColor::Green : FColor::Black, false, 0, 0, 1);
	}
}


bool ARaycastGeomTest::ShouldTickIfViewportsOnly() const
{
	return true;
}

