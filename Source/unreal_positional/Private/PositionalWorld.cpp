// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionalWorld.h"
#include "PositionalUtil.h"
#include "DrawDebugHelpers.h"
#include "PositionalCollider.h"
#include "PositionalRigidBody.h"
#include "PositionalConstraint.h"
#include "collision/narrowphase/Penetration.h"

// Sets default values
APositionalWorld::APositionalWorld()
{
	DeltaTime = 0.0166666666667;
	SubSteps = 20;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	m_World = new World();
	m_World->gravity = ToVec3(Gravity);
}

APositionalWorld::~APositionalWorld()
{
	delete m_World;
}


void APositionalWorld::BeginPlay()
{
	Super::BeginPlay();

	m_World->gravity = ToVec3(Gravity);
	SyncTransforms();
	m_Simulating = true;
	GetWorldTimerManager().SetTimer(m_SimulationTimer, this, &APositionalWorld::FireSimulation, DeltaTime, true, DeltaTime);
}

void APositionalWorld::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	m_Simulating = false;
	GetWorldTimerManager().ClearTimer(m_SimulationTimer);
}


void APositionalWorld::FireSimulation()
{
	if (m_Simulating)
	{
#if WITH_EDITOR
		if (DebugManuallyStepSimulation)
		{
			if (DebugStep)
			{
				DebugStep = false;
				Simulate();
			}
		} else
#endif // WITH_EDITOR
			Simulate();
	}
}

void APositionalWorld::Simulate()
{
	m_World->simulate(DeltaTime, SubSteps);

	for (const auto& elem : m_RigidBodies)
	{
		elem.Value->UpdateTransform();
	}
}

// Called every frame
void APositionalWorld::Tick(float dt)
{
	Super::Tick(dt);

#if WITH_EDITOR
	if (!m_Simulating && (ShowDebugBoundsTree || ShowDebugCollisions || ShowDebugCollisionCSOs || ShowDebugMassProperties))
	{
		SyncTransforms();
		m_World->updateBroadphase();
	}

	if (ShowDebugBoundsTree)
	{
		m_World->forEachBoundsNode([&, this](const Bounds& bounds)
		{
			DrawDebugBox(GetWorld(), ToFVector(bounds.center), ToFVector(bounds.extents()), FColor(0.5F, 0.5F, 0.5F, 0.5F), false, -1, 0, 1);
		});
	}

	if (ShowDebugVelocities)
	{
		m_World->forEachBody([&, this](const Ref<Body>& ref)
		{
			const Body &body = ref.get();
			auto com = body.pose.transform(body.massPose.position);
			DrawDebugDirectionalArrow(
					GetWorld(),
					ToFVector(com),
					ToFVector(com + body.velocity.linear),
					10,
					FColor::Black,
					false,
					-1,
					2);
		});
	}

	if (ShowDebugCollisions)
	{
		m_World->forEachCollision([&, this](const CollisionResult& collision)
		{
			if (collision.first.valid() && collision.second.valid())
			{
				const Bounds& bounds1 = collision.first.get().bounds();
				const Bounds& bounds2 = collision.second.get().bounds();
				DrawDebugBox(GetWorld(), ToFVector(bounds1.center), ToFVector(bounds1.extents()), FColor::Blue, false, -1, 1);
				DrawDebugBox(GetWorld(), ToFVector(bounds2.center), ToFVector(bounds2.extents()), FColor::Green, false, -1, 1);

				const auto& contact = collision.contact;
				const auto a = Body::pointToWorld(collision.first.get().body(), contact.pointA);
				const auto b = Body::pointToWorld(collision.second.get().body(), contact.pointB);

				DrawDebugDirectionalArrow(
					GetWorld(),
					ToFVector(a),
					ToFVector(a + contact.normal * contact.depth),
					10,
					FColor::Red,
					false,
					-1,
					2);
				DrawDebugPoint(GetWorld(), ToFVector(a), 5, FColor::Blue, false, -1, 2);
				DrawDebugPoint(GetWorld(), ToFVector(b), 5, FColor::Green, false, -1, 2);
			}
		});
	}

	if (ShowDebugCollisionCSOs)
	{
		m_World->forEachBroadPair([&, this](const std::pair<Ref<Collider>, Ref<Collider>>& pair)
		{
			if (pair.first.valid() && pair.second.valid())
			{
				const Collider& a = pair.first.get();
				const Collider& b = pair.second.get();

				Collision::Simplex simplex;
				bool colliding = Collision::Penetration::gjk(a, b, simplex);

				UInt8 nearDim, nearIdx;
				Vec3 near = simplex.nearest(nearDim, nearIdx);

				if (colliding)
				{
					Collision::Polytope polytope(simplex.vertices, simplex.verticesA, simplex.verticesB);
					ContactPoint contact;
					Collision::Penetration::epa(a, b, simplex.count, polytope, contact);

					Collision::DrawDebugPolytope(GetWorld(), polytope, FColor::Magenta, 1U, 1.0F);
				}

				DrawDebugPoint(GetWorld(), FVector::ZeroVector, 5.0f, FColor::Black, false, -1, 1U);

				const auto color = colliding ? FColor::Green : FColor::Red;
				Collision::DrawDebugSimplex(GetWorld(), simplex.vertices, simplex.count - 1, 0, color, 2U, 0.5f);
				DrawDebugPoint(GetWorld(), ToFVector(near), 5.0f, color, false, -1, 1);
			}
		});
	}

	if (ShowDebugMassProperties)
	{
		m_World->forEachBody([&, this](const Ref<Body>& ref)
		{
			const Body& body = ref.get();
			auto com = ToFVector(body.pose.transform(body.massPose.position));

			auto inertia = 1.0 / body.invInertia;
			for (uint8 i = 0; i < 3u; ++i)
			{
				Vec3 v;
				v[i] = inertia[i];
				auto inertiaI = ToFVector(body.pose.rotate(body.massPose.rotate(v)));
				DrawDebugLine(GetWorld(), com, com + inertiaI, FColor::Cyan, false, -1.0F, 1U, 1.0F);
			}
		});
	}

#endif // WITH_EDITOR
}

bool APositionalWorld::ShouldTickIfViewportsOnly() const
{
	return !m_Simulating;
}

void APositionalWorld::SyncTransforms()
{
	for (const auto& elem : m_Colliders)
	{
		elem.Value->SyncTransform();
	}

	for (const auto& elem : m_RigidBodies)
	{
		elem.Value->SyncTransform();
	}
}

void APositionalWorld::Raycast(const FVector& rayOrigin, const FVector& rayNormal, const unsigned int& mask, const float& maxDistance, TArray<PositionalRaycastResult>& results) const
{
	vector<RaycastResult> worldResults;
	m_World->raycast(Ray(ToVec3(rayOrigin), ToVec3(rayNormal)), mask, maxDistance, worldResults);

	for (int i = 0, count = worldResults.size(); i < count; ++i)
	{
		auto comp = m_Colliders.Find(worldResults[i].collider.id());
		if (comp)
		{
			results.Emplace(*comp, ToFVector(worldResults[i].point), ToFVector(worldResults[i].normal), worldResults[i].distance);
		}
	}
}


Ref<Body> APositionalWorld::CreateRigidBody(APositionalRigidBody* actor, const FVector& pos, const FQuat& rot)
{
	auto ref = m_World->createBody<RigidBody>(ToVec3(pos), ToQuat(rot));
	m_RigidBodies.Add(ref.id(), actor);
	return ref;
}

void APositionalWorld::DestroyRigidBody(const Ref<Body>& ref)
{
	m_RigidBodies.Remove(ref.id());
	m_World->destroyBody(ref);
}

Ref<Collider> APositionalWorld::CreateSphereCollider(APositionalRigidBody* body, UPositionalCollider* component, const FVector& pos, const float& radius, const float& density, const float& staticFriction, const float& dynamicFriction, const float& bounciness)
{
	auto ref = m_World->createSphereCollider(body ? body->GetRef() : Body::null, ToVec3(pos), radius, density, staticFriction, dynamicFriction, bounciness);
	m_Colliders.Add(ref.id(), component);
	return ref;
}

Ref<Collider> APositionalWorld::CreateCapsuleCollider(APositionalRigidBody* body, UPositionalCollider* component, const FVector& pos, const FQuat& rot, const float& radius, const float& length, const float& density, const float& staticFriction, const float& dynamicFriction, const float& bounciness)
{
	auto ref = m_World->createCapsuleCollider(body ? body->GetRef() : Body::null, ToVec3(pos), ToQuat(rot), radius, length, density, staticFriction, dynamicFriction, bounciness);
	m_Colliders.Add(ref.id(), component);
	return ref;
}

Ref<Collider> APositionalWorld::CreateBoxCollider(APositionalRigidBody* body, UPositionalCollider* component, const FVector& pos, const FQuat& rot, const FVector& extents, const float& density, const float& staticFriction, const float& dynamicFriction, const float& bounciness)
{
	auto ref = m_World->createBoxCollider(body ? body->GetRef() : Body::null, ToVec3(pos), ToQuat(rot), ToVec3(extents), density, staticFriction, dynamicFriction, bounciness);
	m_Colliders.Add(ref.id(), component);
	return ref;
}

void APositionalWorld::DestroyCollider(const Ref<Collider> &ref)
{
	m_Colliders.Remove(ref.id());
	if (ref.valid())
	{
		m_World->destroyCollider(ref);
	}
}

void APositionalWorld::DestroyConstraint(const Ref<Constraint> &ref)
{
	m_Constraints.Remove(ref.id());
	if (ref.valid())
	{
		m_World->destroyConstraint(ref);
	}
}
