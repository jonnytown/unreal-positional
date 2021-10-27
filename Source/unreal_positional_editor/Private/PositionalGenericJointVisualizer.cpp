// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionalGenericJointVisualizer.h"
#include "SceneManagement.h"
#include "HitProxies.h"
#include "ScopedTransaction.h"
#include "Framework/Commands/Commands.h"
#include "Framework/Commands/InputChord.h"
#include "Framework/Commands/UICommandList.h"
#include "Framework/Commands/UIAction.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Editor.h"
#include "EditorViewportClient.h"
#include "EditorViewportCommands.h"
#include "EditorStyleSet.h"
#include "Widgets/SWidget.h"

#define LOCTEXT_NAMESPACE "PositionalGenericJointVisualizer"
IMPLEMENT_HIT_PROXY(HAnchorProxy, HComponentVisProxy);

class FPositionalGenericJointVisualizerCommands : public TCommands<FPositionalGenericJointVisualizerCommands>
{
public:
	FPositionalGenericJointVisualizerCommands() 
		: TCommands<FPositionalGenericJointVisualizerCommands>(
			"PositionalGenericJointVisualizer",				// Context name for fast lookup
			INVTEXT("Positional Generic Joint Visualizer"), // Invariant context name for displaying
			NAME_None,										// Parent
			FEditorStyle::GetStyleSetName())
	{
	}

	virtual void RegisterCommands() override
	{
		UI_COMMAND(SnapAnchor, "Snap to Other", "Snap the currently selected anchor to the other one.", EUserInterfaceActionType::Button, FInputChord(EKeys::S));
	}

public:
	/** Snap anchor */
	TSharedPtr<FUICommandInfo> SnapAnchor;
};

FPositionalGenericJointVisualizer::FPositionalGenericJointVisualizer()
	: FComponentVisualizer()
{
	m_Actions = MakeShareable(new FUICommandList);
	m_Selection = NewObject<UAnchorSelectionState>(GetTransientPackage(), TEXT("PositionalAnchorSelectionState"), RF_Transactional);
	FPositionalGenericJointVisualizerCommands::Register();
}

FPositionalGenericJointVisualizer::~FPositionalGenericJointVisualizer()
{
	FPositionalGenericJointVisualizerCommands::Unregister();
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
void FPositionalGenericJointVisualizer::OnRegister()
{
	const auto &Commands = FPositionalGenericJointVisualizerCommands::Get();

	m_Actions->MapAction(
		Commands.SnapAnchor,
		FExecuteAction::CreateSP(this, &FPositionalGenericJointVisualizer::OnSnapAnchor),
		FCanExecuteAction::CreateSP(this, &FPositionalGenericJointVisualizer::CanSnapAnchor));
}

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
		PDI->SetHitProxy(new HAnchorProxy(Component, 1));
		DrawAnchor(PDI, GetBodyTransform(joint, 1), joint->AnchorPositionA, joint->AnchorRotationA.Quaternion(), m_Selection->GetAnchor() == 1 ? FLinearColor::Yellow : FLinearColor(1.F, 0.F, 1.F));

		PDI->SetHitProxy(new HAnchorProxy(Component, 2));
		DrawAnchor(PDI, GetBodyTransform(joint, 2), joint->AnchorPositionB, joint->AnchorRotationB.Quaternion(), m_Selection->GetAnchor() == 2 ? FLinearColor::Yellow : FLinearColor(0.5F, 0.F, 0.5F));
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
			transform = GetBodyTransform(joint, 1);
			loc = joint->AnchorPositionA;
			
		}
		else if (m_Selection->GetAnchor() == 2)
		{
			transform = GetBodyTransform(joint, 2);
			loc = joint->AnchorPositionB;
		}

		OutLocation = transform.TransformPosition(loc);
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
			auto rot = GetBodyTransform(joint, 1).TransformRotation(joint->AnchorRotationA.Quaternion());
			OutMatrix = FRotationMatrix::Make(rot);
			return true;
		}

		if (m_Selection->GetAnchor() == 2)
		{
			auto rot = GetBodyTransform(joint, 2).TransformRotation(joint->AnchorRotationB.Quaternion());
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
		uint8 anchor = m_Selection->GetAnchor();

		if (!DeltaTranslate.IsZero())
		{
			auto posProp = GetAnchorPositionProperty(joint, anchor);
			if (posProp)
			{
				joint->Modify();
				m_Selection->Modify();

				auto translate = GetBodyTransform(joint, anchor).InverseTransformVectorNoScale(DeltaTranslate);

				FVector *valuePtr = posProp->ContainerPtrToValuePtr<FVector>(joint);
				FVector newValue = GetAnchorPosition(joint, anchor) + translate;
				posProp->CopyCompleteValue(valuePtr, &newValue);
				NotifyPropertyModified(joint, posProp, EPropertyChangeType::Interactive);

				return true;
			}
		}

		if (!DeltaRotate.IsZero())
		{
			auto rotProp = GetAnchorRotationProperty(joint, anchor);
			if (rotProp)
			{
				joint->Modify();
				m_Selection->Modify();

				auto bodyRot = GetBodyTransform(joint, anchor).GetRotation();
				auto worldRot = bodyRot * GetAnchorRotation(joint, anchor);
				FRotator newValue(bodyRot.Inverse() * (DeltaRotate.Quaternion() * worldRot));

				FRotator *valuePtr = rotProp->ContainerPtrToValuePtr<FRotator>(joint);
				rotProp->CopyCompleteValue(valuePtr, &newValue);
				NotifyPropertyModified(joint, rotProp, EPropertyChangeType::Interactive);

				return true;
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

TSharedPtr<SWidget> FPositionalGenericJointVisualizer::GenerateContextMenu() const
{
	FMenuBuilder builder(true, m_Actions);
	builder.BeginSection("GenericJointEdit", INVTEXT("Generic Joint Anchor"));
	builder.AddMenuEntry(FPositionalGenericJointVisualizerCommands::Get().SnapAnchor);
	builder.EndSection();
	TSharedPtr<SWidget> widget = builder.MakeWidget();
	return widget;
}

bool FPositionalGenericJointVisualizer::HandleInputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event)
{
	if (m_Selection->IsValid() && Event == IE_Pressed)
	{
		return m_Actions->ProcessCommandBindings(Key, FSlateApplication::Get().GetModifierKeys(), false);
	}
	return false;
}
#pragma endregion // FComponentVisualizer

FTransform FPositionalGenericJointVisualizer::GetBodyTransform(const UPositionalGenericJoint *joint, const uint8 &anchor) const
{
	if (joint)
	{
		if (anchor == 1)
		{
			auto transform = joint->GetOwner()->GetActorTransform();
			transform.RemoveScaling();
			return transform;
		}

		if (anchor == 2 && joint->ConnectedBody.IsValid())
		{
			auto transform = joint->ConnectedBody.Get()->GetActorTransform();
			transform.RemoveScaling();
			return transform;
		}
	}
	return FTransform::Identity;
}

FVector FPositionalGenericJointVisualizer::GetAnchorPosition(const UPositionalGenericJoint *joint, const uint8 &anchor) const
{
	if (joint)
	{
		switch (anchor)
		{
		case 1:
			return joint->AnchorPositionA;
		case 2:
			return joint->AnchorPositionB;
		}
	}
	return FVector::ZeroVector;
}

FQuat FPositionalGenericJointVisualizer::GetAnchorRotation(const UPositionalGenericJoint *joint, const uint8 &anchor) const
{
	if (joint)
	{
		switch (anchor)
		{
		case 1:
			return joint->AnchorRotationA.Quaternion();
		case 2:
			return joint->AnchorRotationB.Quaternion();
		}
	}
	return FQuat::Identity;
}

FStructProperty *FPositionalGenericJointVisualizer::GetAnchorPositionProperty(const UPositionalGenericJoint *joint, const uint8 &anchor) const
{
	if (joint)
	{
		switch (anchor)
		{
		case 1:
			return FindFProperty<FStructProperty>(joint->GetClass(), FName("AnchorPositionA"));
		case 2:
			return FindFProperty<FStructProperty>(joint->GetClass(), FName("AnchorPositionB"));
		}
	}
	return nullptr;
}

FStructProperty * FPositionalGenericJointVisualizer::GetAnchorRotationProperty(const UPositionalGenericJoint *joint, const uint8 &anchor) const
{
	if (joint)
	{
		switch (anchor)
		{
		case 1:
			return FindFProperty<FStructProperty>(joint->GetClass(), FName("AnchorRotationA"));
		case 2:
			return FindFProperty<FStructProperty>(joint->GetClass(), FName("AnchorRotationB"));
		}
	}
	return nullptr;
}

void FPositionalGenericJointVisualizer::OnSnapAnchor()
{
	if (m_Selection->IsValid())
	{
		auto *joint = Cast<UPositionalGenericJoint>(m_Selection->GetPath().GetComponent());
		uint8 anchor = m_Selection->GetAnchor();

		const uint8 other = 1 + (2 - anchor);

		FTransform otherTransform = GetBodyTransform(joint, other);
		FVector otherPos = otherTransform.TransformPosition(GetAnchorPosition(joint, other));
		FQuat otherRot = otherTransform.GetRotation() * GetAnchorRotation(joint, other);

		FTransform transform = GetBodyTransform(joint, anchor);
		FVector newPos = transform.InverseTransformPosition(otherPos);
		FRotator newRot = FRotator(transform.GetRotation().Inverse() * otherRot);

		const FScopedTransaction transaction(INVTEXT("Snap Joint Anchor"));
		joint->Modify();
		m_Selection->Modify();

		auto posProp = GetAnchorPositionProperty(joint, anchor);
		FVector *posPtr = posProp->ContainerPtrToValuePtr<FVector>(joint);
		posProp->CopyCompleteValue(posPtr, &newPos);
		NotifyPropertyModified(joint, posProp, EPropertyChangeType::Interactive);

		auto rotProp = GetAnchorRotationProperty(joint, anchor);
		FRotator *rotPtr = rotProp->ContainerPtrToValuePtr<FRotator>(joint);
		rotProp->CopyCompleteValue(rotPtr, &newRot);
		NotifyPropertyModified(joint, rotProp, EPropertyChangeType::Interactive);
	}
}

bool FPositionalGenericJointVisualizer::CanSnapAnchor() const
{
	return m_Selection->IsValid();
}
