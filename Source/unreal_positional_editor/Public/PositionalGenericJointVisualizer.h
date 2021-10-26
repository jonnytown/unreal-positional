// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "unreal_positional_editor.h"
#include "ComponentVisualizer.h"
#include "PositionalGenericJoint.h"
#include "Math/Quat.h"

/**
 * 
 */
class FPositionalGenericJointVisualizer : public FComponentVisualizer
{
public:
	FPositionalGenericJointVisualizer();
	virtual ~FPositionalGenericJointVisualizer();

    // Begin FComponentVisualizer interface
    virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
    virtual bool VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click) override;
    virtual UActorComponent* GetEditedComponent() const override;
    virtual bool GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const override;
    virtual bool GetCustomInputCoordinateSystem(const FEditorViewportClient *ViewportClient, FMatrix &OutMatrix) const override;
    virtual bool HandleInputDelta(FEditorViewportClient *ViewportClient, FViewport *Viewport, FVector &DeltaTranslate, FRotator &DeltaRotate, FVector &DeltaScale) override;
    virtual void EndEditing() override;
    // End FComponentVisualizer interface

private:
    void DrawAnchor(FPrimitiveDrawInterface *PDI, const FTransform &transform, const FVector &anchorPos, const FQuat &anchorRot, const FLinearColor &color);

    TWeakObjectPtr<UPositionalGenericJoint> m_EditedComponent;
    uint8 m_SelectedAnchor;
};

struct HAnchorProxy : public HComponentVisProxy
{
    DECLARE_HIT_PROXY();

    HAnchorProxy(const UActorComponent *component, const uint8 &anchor)
        : HComponentVisProxy(component, HPP_Wireframe), Anchor(anchor) {}

    uint8 Anchor;
};
