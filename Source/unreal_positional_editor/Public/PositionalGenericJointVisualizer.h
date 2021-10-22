// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "unreal_positional_editor.h"
#include "ComponentVisualizer.h"
#include "PositionalGenericJoint.h"

/**
 * 
 */
class UNREAL_POSITIONAL_EDITOR_API FPositionalGenericJointVisualizer : public FComponentVisualizer
{
public:
	FPositionalGenericJointVisualizer();
	virtual ~FPositionalGenericJointVisualizer();

    // Begin FComponentVisualizer interface
    virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
    // End FComponentVisualizer interface

private:
    void DrawAnchor(FPrimitiveDrawInterface *PDI, const FTransform &transform, const FVector &anchorPos, const FQuat &anchorRot, const FLinearColor &color);
};
