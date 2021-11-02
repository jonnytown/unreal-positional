// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "unreal_positional_editor.h"
#include "ComponentVisualizer.h"

/**
 * 
 */
class UNREAL_POSITIONAL_EDITOR_API FPositionalColliderVisualizer : public FComponentVisualizer
{
public:
	FPositionalColliderVisualizer();
	virtual ~FPositionalColliderVisualizer();

    // Begin FComponentVisualizer interface
    virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
    virtual bool VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click) override;
    // End FComponentVisualizer interface

private:
    static void DrawCircle(FPrimitiveDrawInterface *PDI, const FVector &pos, const FQuat &rot, const float &radius, const float &degrees, const uint8 &segments);
    static void DrawSphere(FPrimitiveDrawInterface *PDI, const FVector &pos, const FQuat &rot, const float &radius);
    static void DrawBox(FPrimitiveDrawInterface *PDI, const FVector &pos, const FQuat &rot, const FVector &extents);
    static void DrawCapsule(FPrimitiveDrawInterface *PDI, const FVector &pos, const FQuat &rot, const float &radius, const float &length);
    static void DrawCylinder(FPrimitiveDrawInterface *PDI, const FVector &pos, const FQuat &rot, const float &radius, const float &length);
};
