// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "unreal_positional_editor.h"
#include "ComponentVisualizer.h"
#include "PositionalGenericJoint.h"
#include "PositionalGenericJointVisualizer.generated.h"

class FUICommandList;
/**
 * 
 */
UCLASS(Transient)
class UNREAL_POSITIONAL_EDITOR_API UAnchorSelectionState : public UObject
{
	GENERATED_BODY()

public:
    void Reset()
    {
        Anchor = 0;
        Path.Reset();
    }

    bool IsValid() const
    {
        return Path.IsValid() && (Anchor == 1 || Anchor == 2);
    }

    uint8 GetAnchor() const { return Anchor; }
    void SetAnchor(uint8 anchor)
    {
        Anchor = anchor;
    }

    FComponentPropertyPath GetPath() const { return Path; }
    void SetPath(const FComponentPropertyPath &path)
    {
        Path = path;
    }
protected:
    UPROPERTY()
    uint8 Anchor;

    UPROPERTY()
    FComponentPropertyPath Path;
};

/**
 * 
 */
class UNREAL_POSITIONAL_EDITOR_API FPositionalGenericJointVisualizer : public FComponentVisualizer, public FGCObject
{
public:
	FPositionalGenericJointVisualizer();
	virtual ~FPositionalGenericJointVisualizer();

    // Begin FComponentVisualizer interface
    virtual void OnRegister() override;
    virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
    virtual bool VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click) override;
    virtual UActorComponent* GetEditedComponent() const override;
    virtual bool GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const override;
    virtual bool GetCustomInputCoordinateSystem(const FEditorViewportClient *ViewportClient, FMatrix &OutMatrix) const override;
    virtual bool HandleInputDelta(FEditorViewportClient *ViewportClient, FViewport *Viewport, FVector &DeltaTranslate, FRotator &DeltaRotate, FVector &DeltaScale) override;
    virtual void EndEditing() override;
    virtual TSharedPtr<SWidget> GenerateContextMenu() const override;
    virtual bool HandleInputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;
    // End FComponentVisualizer interface

    // Begin FComponentVisualizer interface
    virtual void AddReferencedObjects(FReferenceCollector &Collector) override;
    // End FComponentVisualizer interface

private:
    void DrawAnchor(FPrimitiveDrawInterface *PDI, const FTransform &transform, const FVector &anchorPos, const FQuat &anchorRot, const FLinearColor &color);
    FTransform GetBodyTransform(const UPositionalGenericJoint *joint, const uint8 &anchor) const;
    FVector GetAnchorPosition(const UPositionalGenericJoint *joint, const uint8 &anchor) const;
    FQuat GetAnchorRotation(const UPositionalGenericJoint *joint, const uint8 &anchor) const;
    FStructProperty * GetAnchorPositionProperty(const UPositionalGenericJoint *joint, const uint8 &anchor) const;
    FStructProperty * GetAnchorRotationProperty(const UPositionalGenericJoint *joint, const uint8 &anchor) const;
    void OnSnapAnchor();
    bool CanSnapAnchor() const;

    UAnchorSelectionState *m_Selection;
    TSharedPtr<FUICommandList> m_Actions;
};

struct HAnchorProxy : public HComponentVisProxy
{
    DECLARE_HIT_PROXY();

    HAnchorProxy(const UActorComponent *component, const uint8 &anchor)
        : HComponentVisProxy(component, HPP_Wireframe), Anchor(anchor) {}

    uint8 Anchor;
};
