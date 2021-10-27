// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionalGenericJointVisualizer.h"
#include "SceneManagement.h"
#include "HitProxies.h"
#include "EditorViewportClient.h"
#include "ScopedTransaction.h"

IMPLEMENT_HIT_PROXY(HAnchorProxy, HComponentVisProxy);

FPositionalGenericJointVisualizer::FPositionalGenericJointVisualizer()
	: FComponentVisualizer()
{
	m_Selection = NewObject<UAnchorSelectionState>(GetTransientPackage(), TEXT("PositionalAnchorSelectionState"), RF_Transactional);
}

FPositionalGenericJointVisualizer::~FPositionalGenericJointVisualizer()
{
}

#pragma region FGCObject
void FPositionalGenericJointVisualizer::AddReferencedObjects(FReferenceCollector& Collector)
{
	if (m_Selection)
	{
		Collector.AddReferencedObject(m_Selection);
	}
}
#pragma endregion // FGCObject

#pragma region FComponentVisualizer
void FPositionalGenericJointVisualizer::DrawAnchor(FPrimitiveDrawInterface* PDI, const FTransform& transform, const FVector& anchorPos, const FQuat& anchorRot, const FLinearColor &color)
{
	auto pos = transform.TransformPosition(anchorPos);
	auto rot = transform.GetRotation() * anchorRot;

	auto mat = FRotationTranslationMatrix::Make(FRotator(rot), pos);
	DrawDirectionalArrow(PDI, mat, color, 30.F, 3.F, SDPG_Foreground, 1.F);

	rot = rot * FQuat(FVector::ZAxisVector, PI / 2.F);
	mat = FRotationTranslationMatrix::Make(FRotator(rot), pos);

	DrawDirectionalArrow(PDI, mat, color, 15.F, 3.F, SDPG_Foreground, 1.F);
}

void FPositionalGenericJointVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	if (const auto* joint = Cast<UPositionalGenericJoint>(Component))
	{
		auto transformA = joint->GetOwner()->GetActorTransform();
		transformA.RemoveScaling();

		PDI->SetHitProxy(new HAnchorProxy(Component, 1));
		DrawAnchor(PDI, transformA, joint->AnchorPositionA, joint->AnchorRotationA.Quaternion(), m_Selection->GetAnchor() == 1 ? FLinearColor::Yellow : FLinearColor(1.F, 0.F, 1.F));

		FTransform transformB = FTransform::Identity;
		if (joint->ConnectedBody.IsValid())
		{
			transformB = joint->ConnectedBody.Get()->GetActorTransform();
			transformB.RemoveScaling();
		}
		
		PDI->SetHitProxy(new HAnchorProxy(Component, 2));
		DrawAnchor(PDI, transformB, joint->AnchorPositionB, joint->AnchorRotationB.Quaternion(), m_Selection->GetAnchor() == 2 ? FLinearColor::Yellow : FLinearColor(0.5F, 0.F, 0.5F));
		PDI->SetHitProxy(nullptr);
	}
}

bool FPositionalGenericJointVisualizer::VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click)
{
	if (VisProxy && VisProxy->Component.IsValid())
	{
		check(m_Selection);
		m_Selection->Modify();
		m_Selection->SetPath(FComponentPropertyPath(VisProxy->Component.Get()));

		if (VisProxy->IsA(HAnchorProxy::StaticGetType()))
		{
			auto joint = Cast<UPositionalGenericJoint>(VisProxy->Component.Get());
			if (joint)
			{
				const FScopedTransaction transaction(INVTEXT("Select Joint Anchor"));
				check(m_Selection);
				m_Selection->Modify();
				m_Selection->SetAnchor(static_cast<HAnchorProxy *>(VisProxy)->Anchor);
				return true;
			}
		}
	}

	m_Selection->Reset();
	return false;
}

UActorComponent* FPositionalGenericJointVisualizer::GetEditedComponent() const
{
	return m_Selection->GetPath().GetComponent();
}

bool FPositionalGenericJointVisualizer::GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const
{
	if (m_Selection->IsValid())
	{
		const auto *joint = Cast<UPositionalGenericJoint>(m_Selection->GetPath().GetComponent());

		FTransform transform = FTransform::Identity;
		FVector loc = FVector::ZeroVector;
		if (m_Selection->GetAnchor() == 1)
		{
			transform = joint->GetOwner()->GetActorTransform();
			loc = joint->AnchorPositionA;
			
		}
		else if (m_Selection->GetAnchor() == 2)
		{
			loc = joint->AnchorPositionB;
			if (joint->ConnectedBody.IsValid())
			{
				transform = joint->ConnectedBody.Get()->GetActorTransform();
			}
		}


		OutLocation = transform.TransformPositionNoScale(loc);
		return true;
	}
	return false;
}

bool FPositionalGenericJointVisualizer::GetCustomInputCoordinateSystem(const FEditorViewportClient *ViewportClient, FMatrix &OutMatrix) const
{
	if (m_Selection->IsValid() && ViewportClient->GetWidgetCoordSystemSpace() == COORD_Local)
	{
		const auto *joint = Cast<UPositionalGenericJoint>(m_Selection->GetPath().GetComponent());
		if (m_Selection->GetAnchor() == 1)
		{
			auto rot = joint->GetOwner()->GetActorTransform().TransformRotation(joint->AnchorRotationA.Quaternion());
			OutMatrix = FRotationMatrix::Make(rot);
			return true;
		}

		if (m_Selection->GetAnchor() == 2)
		{
			auto rot = joint->ConnectedBody->GetActorTransform().TransformRotation(joint->AnchorRotationB.Quaternion());
			OutMatrix = FRotationMatrix::Make(rot);
			return true;
		}
	}

	return false;
}

bool FPositionalGenericJointVisualizer::HandleInputDelta(
	FEditorViewportClient *ViewportClient,
	FViewport *Viewport,
	FVector &DeltaTranslate,
	FRotator &DeltaRotate,
	FVector &DeltaScale)
{
	if (m_Selection->IsValid())
	{
		auto *joint = Cast<UPositionalGenericJoint>(m_Selection->GetPath().GetComponent());

		if (m_Selection->GetAnchor() == 1)
		{
			if (!DeltaTranslate.IsZero())
			{
				auto posProp = FindFProperty<FStructProperty>(joint->GetClass(), FName("AnchorPositionA"));
				if (posProp)
				{
					joint->Modify();
					m_Selection->Modify();

					auto translate = joint->GetOwner()->GetActorTransform().InverseTransformVectorNoScale(DeltaTranslate);

					FVector *valuePtr = posProp->ContainerPtrToValuePtr<FVector>(joint);
					FVector newValue = joint->AnchorPositionA + translate;
					posProp->CopyCompleteValue(valuePtr, &newValue);
					NotifyPropertyModified(joint, posProp, EPropertyChangeType::Interactive);

					return true;
				}
			}

			if (!DeltaRotate.IsZero())
			{
				auto rotProp = FindFProperty<FStructProperty>(joint->GetClass(), FName("AnchorRotationA"));
				if (rotProp)
				{
					joint->Modify();
					m_Selection->Modify();

					auto bodyRot = joint->GetOwner()->GetActorTransform().GetRotation();
					auto worldRot = bodyRot * joint->AnchorRotationA.Quaternion();
					FRotator newValue(bodyRot.Inverse() * (DeltaRotate.Quaternion() * worldRot));

					FRotator *valuePtr = rotProp->ContainerPtrToValuePtr<FRotator>(joint);
					rotProp->CopyCompleteValue(valuePtr, &newValue);
					NotifyPropertyModified(joint, rotProp, EPropertyChangeType::Interactive);

					return true;
				}
			}
		}
		else if (m_Selection->GetAnchor() == 2)
		{
			if (!DeltaTranslate.IsZero())
			{
				auto posProp = FindFProperty<FStructProperty>(joint->GetClass(), FName("AnchorPositionB"));
				if (posProp)
				{
					joint->Modify();
					m_Selection->Modify();

					auto translate = DeltaTranslate;
					if (joint->ConnectedBody.IsValid())
					 	translate = joint->ConnectedBody.Get()->GetActorTransform().InverseTransformVectorNoScale(DeltaTranslate);

					FVector *valuePtr = posProp->ContainerPtrToValuePtr<FVector>(joint);
					FVector newValue = joint->AnchorPositionB + translate;
					posProp->CopyCompleteValue(valuePtr, &newValue);
					NotifyPropertyModified(joint, posProp, EPropertyChangeType::Interactive);


					return true;
				}
			}

			if (!DeltaRotate.IsZero())
			{
				auto rotProp = FindFProperty<FStructProperty>(joint->GetClass(), FName("AnchorRotationB"));
				if (rotProp)
				{
					joint->Modify();
					m_Selection->Modify();

					auto bodyRot = joint->ConnectedBody.IsValid() ? joint->ConnectedBody.Get()->GetActorTransform().GetRotation() : FQuat::Identity;
					auto worldRot = bodyRot * joint->AnchorRotationB.Quaternion();
					FRotator newValue(bodyRot.Inverse() * (DeltaRotate.Quaternion() * worldRot));

					FRotator *valuePtr = rotProp->ContainerPtrToValuePtr<FRotator>(joint);
					rotProp->CopyCompleteValue(valuePtr, &newValue);
					NotifyPropertyModified(joint, rotProp, EPropertyChangeType::Interactive);

					return true;
				}
			}
		}
	}

	return false;
}

void FPositionalGenericJointVisualizer::EndEditing()
{
	check(m_Selection);
	m_Selection->Modify();
	m_Selection->Reset();
}

#pragma endregion // FComponentVisualizer
