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

Ref<Constraint> UPositionalGenericJoint::CreateConstraint(APositionalWorld *world, APositionalRigidBody *bodyA, APositionalRigidBody *bodyB)
{
	Ref<Constraint> constraint = world->CreateConstraint<GenericJointConstraint, GenericJointConstraint::Data>(
		this,
		bodyA,
		bodyB,
		Pose(ToVec3(AnchorPositionA), ToQuat(AnchorRotationA.Quaternion())),
		Pose(ToVec3(AnchorPositionB), ToQuat(AnchorRotationB.Quaternion())),
		DOF,
		HasLimits,
		PositionCompliance,
		PositionDamping,
		LinearLimit,
		RotationCompliance,
		RotationDamping,
		MinTwist,
		MaxTwist,
		MinSwing,
		MaxSwing);
	return constraint;
}
