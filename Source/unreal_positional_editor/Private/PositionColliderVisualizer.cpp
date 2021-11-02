// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionalColliderVisualizer.h"
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

#include "PositionalBoxCollider.h"
#include "PositionalSphereCollider.h"
#include "PositionalCapsuleCollider.h"
#include "PositionalCylinderCollider.h"

FPositionalColliderVisualizer::FPositionalColliderVisualizer()
	: FComponentVisualizer()
{
}

FPositionalColliderVisualizer::~FPositionalColliderVisualizer()
{
	
}

#pragma region FComponentVisualizer
void FPositionalColliderVisualizer::DrawVisualization(const UActorComponent *Component, const FSceneView *View, FPrimitiveDrawInterface *PDI)
{
	if (const auto* box = Cast<UPositionalBoxCollider>(Component))
	{
		PDI->SetHitProxy(new HComponentVisProxy(Component, HPP_Wireframe));

		auto transform = box->GetOwner()->GetActorTransform();
		auto ext = box->Extents;
		auto pos = transform.TransformPositionNoScale(box->Center);
		auto rot = transform.GetRotation() * box->Rotation.Quaternion();

		DrawBox(PDI, pos, rot, ext);

		PDI->SetHitProxy(nullptr);
		return;
	}

	if (const auto *sphere = Cast<UPositionalSphereCollider>(Component))
	{
		PDI->SetHitProxy(new HComponentVisProxy(Component, HPP_Wireframe));

		auto transform = sphere->GetOwner()->GetActorTransform();
		auto rad = sphere->Radius;
		auto pos = transform.TransformPositionNoScale(sphere->Center);
		auto rot = transform.GetRotation();

		DrawSphere(PDI, pos, rot, rad);

		PDI->SetHitProxy(nullptr);
		return;
	}

	if (const auto *capsule = Cast<UPositionalCapsuleCollider>(Component))
	{
		PDI->SetHitProxy(new HComponentVisProxy(Component, HPP_Wireframe));

		auto transform = capsule->GetOwner()->GetActorTransform();
		auto rad = capsule->Radius;
		auto len = capsule->Length;
		auto pos = transform.TransformPositionNoScale(capsule->Center);
		auto rot = transform.GetRotation() * capsule->Rotation.Quaternion();

		DrawCapsule(PDI, pos, rot, rad, len);

		PDI->SetHitProxy(nullptr);
		return;
	}

	if (const auto *cylinder = Cast<UPositionalCylinderCollider>(Component))
	{
		PDI->SetHitProxy(new HComponentVisProxy(Component, HPP_Wireframe));

		auto transform = cylinder->GetOwner()->GetActorTransform();
		auto rad = cylinder->Radius;
		auto len = cylinder->Length;
		auto pos = transform.TransformPositionNoScale(cylinder->Center);
		auto rot = transform.GetRotation() * cylinder->Rotation.Quaternion();

		DrawCylinder(PDI, pos, rot, rad, len);

		PDI->SetHitProxy(nullptr);
		return;
	}
}

bool FPositionalColliderVisualizer::VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click)
{
	if (VisProxy && VisProxy->Component.IsValid())
	{

	}
	return false;
}
#pragma endregion // FComponentVisualizer

void FPositionalColliderVisualizer::DrawCircle(FPrimitiveDrawInterface *PDI, const FVector &pos, const FQuat &rot, const float &radius, const float &degrees, const uint8 &segments)
{
	const float step = degrees / segments;
	for (uint32 i = 0; i < segments; ++i)
	{
		float t0 = step * i;
		auto p0 = pos + rot * FVector(FMath::Cos(t0) * radius, FMath::Sin(t0) * radius, 0.F);
		float t1 = step * (i + 1);
		auto p1 = pos + rot * FVector(FMath::Cos(t1) * radius, FMath::Sin(t1) * radius, 0.F);
		PDI->DrawLine(p0, p1, FLinearColor::Black, SDPG_Foreground);
	}
}

void FPositionalColliderVisualizer::DrawSphere(FPrimitiveDrawInterface *PDI, const FVector &pos, const FQuat &rot, const float &radius)
{
	DrawCircle(PDI, pos, rot, radius, PI*2.F, 32);
	DrawCircle(PDI, pos, rot * FQuat(FVector::XAxisVector, PI * 0.5F), radius, PI * 2.F, 32);
	DrawCircle(PDI, pos, rot * FQuat(FVector::YAxisVector, PI * 0.5F), radius, PI * 2.F, 32);
}

void FPositionalColliderVisualizer::DrawBox(FPrimitiveDrawInterface *PDI, const FVector &pos, const FQuat &rot, const FVector &ext)
{
	auto bll = pos + rot * FVector(-ext.X, -ext.Y, -ext.Z);
	auto blr = pos + rot * FVector(ext.X, -ext.Y, -ext.Z);
	auto bul = pos + rot * FVector(-ext.X, ext.Y, -ext.Z);
	auto bur = pos + rot * FVector(ext.X, ext.Y, -ext.Z);

	auto tll = pos + rot * FVector(-ext.X, -ext.Y, ext.Z);
	auto tlr = pos + rot * FVector(ext.X, -ext.Y, ext.Z);
	auto tul = pos + rot * FVector(-ext.X, ext.Y, ext.Z);
	auto tur = pos + rot * FVector(ext.X, ext.Y, ext.Z);

	PDI->DrawLine(bll, blr, FLinearColor::Black, SDPG_Foreground);
	PDI->DrawLine(blr, bur, FLinearColor::Black, SDPG_Foreground);
	PDI->DrawLine(bur, bul, FLinearColor::Black, SDPG_Foreground);
	PDI->DrawLine(bul, bll, FLinearColor::Black, SDPG_Foreground);

	PDI->DrawLine(tll, tlr, FLinearColor::Black, SDPG_Foreground);
	PDI->DrawLine(tlr, tur, FLinearColor::Black, SDPG_Foreground);
	PDI->DrawLine(tur, tul, FLinearColor::Black, SDPG_Foreground);
	PDI->DrawLine(tul, tll, FLinearColor::Black, SDPG_Foreground);

	PDI->DrawLine(bll, tll, FLinearColor::Black, SDPG_Foreground);
	PDI->DrawLine(blr, tlr, FLinearColor::Black, SDPG_Foreground);
	PDI->DrawLine(bul, tul, FLinearColor::Black, SDPG_Foreground);
	PDI->DrawLine(bur, tur, FLinearColor::Black, SDPG_Foreground);
}

void FPositionalColliderVisualizer::DrawCapsule(FPrimitiveDrawInterface *PDI, const FVector &pos, const FQuat &rot, const float &radius, const float &length)
{
	auto v = rot * FVector(length * 0.5F, 0.F, 0.F);
	auto p0 = pos + v;

	auto xRot = FQuat(FVector::XAxisVector, PI * 0.5F);
	auto yRot = rot * FQuat(FVector::YAxisVector, PI * 0.5F);
	auto zRot = rot * FQuat(FVector::ZAxisVector, PI * 0.5F);

	DrawCircle(PDI, p0, yRot, radius, PI * 2.F, 32);
	DrawCircle(PDI, p0, zRot, radius, -PI, 16);
	DrawCircle(PDI, p0, yRot * xRot, radius, PI, 16);

	auto p1 = pos - v;
	DrawCircle(PDI, p1, yRot, radius, PI * 2.F, 32);
	DrawCircle(PDI, p1, zRot, radius, PI, 16);
	DrawCircle(PDI, p1, yRot * xRot, radius, -PI, 16);

	auto x = rot * FVector(0.F, radius, 0.F);
	PDI->DrawLine(p0 + x, p1 + x, FLinearColor::Black, SDPG_Foreground);
	PDI->DrawLine(p0 - x, p1 - x, FLinearColor::Black, SDPG_Foreground);

	auto z = rot * FVector(0.F, 0.F, radius);
	PDI->DrawLine(p0 + z, p1 + z, FLinearColor::Black, SDPG_Foreground);
	PDI->DrawLine(p0 - z, p1 - z, FLinearColor::Black, SDPG_Foreground);
}

void FPositionalColliderVisualizer::DrawCylinder(FPrimitiveDrawInterface *PDI, const FVector &pos, const FQuat &rot, const float &radius, const float &length)
{
	auto v = rot * FVector(length * 0.5F, 0.F, 0.F);
	auto p0 = pos + v;

	auto yRot = rot * FQuat(FVector::YAxisVector, PI * 0.5F);
	DrawCircle(PDI, p0, yRot, radius, PI * 2.F, 32);

	auto p1 = pos - v;
	DrawCircle(PDI, p1, yRot, radius, PI * 2.F, 32);

	auto x = rot * FVector(0.F, radius, 0.F);
	PDI->DrawLine(p0 + x, p1 + x, FLinearColor::Black, SDPG_Foreground);
	PDI->DrawLine(p0 - x, p1 - x, FLinearColor::Black, SDPG_Foreground);

	auto z = rot * FVector(0.F, 0.F, radius);
	PDI->DrawLine(p0 + z, p1 + z, FLinearColor::Black, SDPG_Foreground);
	PDI->DrawLine(p0 - z, p1 - z, FLinearColor::Black, SDPG_Foreground);
}
