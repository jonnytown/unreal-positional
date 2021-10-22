// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionalGenericJointVisualizer.h"
#include "SceneManagement.h"

FPositionalGenericJointVisualizer::FPositionalGenericJointVisualizer()
{
}

FPositionalGenericJointVisualizer::~FPositionalGenericJointVisualizer()
{
}

#pragma region FComponentVisualizer
void FPositionalGenericJointVisualizer::DrawAnchor(FPrimitiveDrawInterface* PDI, const FTransform& transform, const FVector& anchorPos, const FQuat& anchorRot, const FLinearColor &color)
{
	auto pos = transform.TransformPosition(anchorPos);
	auto rot = transform.TransformRotation(anchorRot);

	auto mat = FBasisVectorMatrix(rot * FVector::XAxisVector, rot * FVector::YAxisVector, rot * FVector::ZAxisVector, FVector::ZeroVector);
	mat.M[3][0] = pos.X;
	mat.M[3][1] = pos.Y;
	mat.M[3][2] = pos.Z;
	DrawDirectionalArrow(PDI, mat, color, 30.F, 3.F, SDPG_Foreground, 1.F);

	rot = FQuat(FVector::ZAxisVector, PI / -2.F) * rot;
	mat = FBasisVectorMatrix(rot * FVector::XAxisVector, rot * FVector::YAxisVector, rot * FVector::ZAxisVector, FVector::ZeroVector);
	mat.M[3][0] = pos.X;
	mat.M[3][1] = pos.Y;
	mat.M[3][2] = pos.Z;

	DrawDirectionalArrow(PDI, mat, color, 15.F, 3.F, SDPG_Foreground, 1.F);
}

void FPositionalGenericJointVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	if (const auto* joint = Cast<UPositionalGenericJoint>(Component))
	{
		auto transformA = joint->GetOwner()->GetActorTransform();
		transformA.RemoveScaling();
		DrawAnchor(PDI, transformA, joint->AnchorPositionA, joint->AnchorRotationA.Quaternion(), FLinearColor(1.F, 0.F, 1.F));

		if (joint->ConnectedBody.IsValid())
		{
			auto transformB = joint->ConnectedBody.Get()->GetActorTransform();
			transformB.RemoveScaling();
			DrawAnchor(PDI, transformB, joint->AnchorPositionB, joint->AnchorRotationB.Quaternion(), FLinearColor(0.5F, 0.F, 0.5F));
		}

	}
}
#pragma endregion // FComponentVisualizer
