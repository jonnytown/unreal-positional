// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionalGenericJoint.h"
#include "constraints/GenericJointConstraint.h"
#include "PositionalUtil.h"
// Sets default values for this component's properties
UPositionalGenericJoint::UPositionalGenericJoint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

Ref<Constraint> UPositionalGenericJoint::CreateConstraint(Positional::World *world, const Ref<Body> &bodyA, const Ref<Body> &bodyB)
{
	return world->createConstraint<GenericJointConstraint, GenericJointConstraint::Data>(
		bodyA,
		bodyB,
		IgnoreCollisions,
		Pose(ToVec3(AnchorPositionA), ToQuat(AnchorRotationA.Quaternion())),
		Pose(ToVec3(AnchorPositionB), ToQuat(AnchorRotationB.Quaternion())),
		DOF,
		HasLimits,
		PositionCompliance,
		PositionDamping,
		LinearLimit,
		RotationCompliance,
		RotationDamping,
		FMath::DegreesToRadians(MinTwist),
		FMath::DegreesToRadians(MaxTwist),
		FMath::DegreesToRadians(MinSwing),
		FMath::DegreesToRadians(MaxSwing));
}

#if WITH_EDITOR
void UPositionalGenericJoint::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FString name;
	PropertyChangedEvent.MemberProperty->GetName(name);
	if (m_Constraint.valid())
	{
		auto data = m_Constraint.get().getData<GenericJointConstraint::Data>();

		if (name == TEXT("AnchorPositionA"))
		{
			data->poseA.position = ToVec3(AnchorPositionA);
		}
		else if (name == TEXT("AnchorRotationA"))
		{
			data->poseA.rotation = ToQuat(AnchorRotationA);
		}
		else if (name == TEXT("AnchorPositionB"))
		{
			data->poseA.position = ToVec3(AnchorPositionB);
		}
		else if (name == TEXT("AnchorRotationB"))
		{
			data->poseA.rotation = ToQuat(AnchorRotationB);
		}
		else if (name == TEXT("DOF"))
		{
			data->dof = DOF;
		}
		else if (name == TEXT("HasLimits"))
		{
			data->hasLimits = HasLimits;
		}
		else if (name == TEXT("PositionCompliance"))
		{
			data->positionCompliance = PositionCompliance;
		}
		else if (name == TEXT("PositionDamping"))
		{
			data->positionDamping = PositionDamping;
		}
		else if (name == TEXT("LinearLimit"))
		{
			data->linearLimit = LinearLimit;
		}
		else if (name == TEXT("RotationCompliance"))
		{
			data->rotationCompliance = RotationCompliance;
		}
		else if (name == TEXT("RotationDamping"))
		{
			data->rotationDamping = RotationDamping;
		}
		else if (name == TEXT("MinTwist"))
		{
			data->minTwist = FMath::DegreesToRadians(MinTwist);
		}
		else if (name == TEXT("MaxTwist"))
		{
			data->maxTwist = FMath::DegreesToRadians(MaxTwist);
		}
		else if (name == TEXT("MinSwing"))
		{
			data->minSwing = FMath::DegreesToRadians(MinSwing);
		}
		else if (name == TEXT("MaxSwing"))
		{
			data->maxSwing = FMath::DegreesToRadians(MaxSwing);
		}
	}
}
#endif //WITH_EDITOR
