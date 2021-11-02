#include "unreal_positional_editor.h"
#include "PositionalGenericJointVisualizer.h"
#include "PositionalColliderVisualizer.h"
#include "Editor/UnrealEd/Classes/Editor/UnrealEdEngine.h"
#include "UnrealEdGlobals.h"

#include "PositionalBoxCollider.h"
#include "PositionalSphereCollider.h"
#include "PositionalCapsuleCollider.h"
#include "PositionalCylinderCollider.h"

IMPLEMENT_GAME_MODULE(FPositionalEditorModule, unreal_positional_editor);

DEFINE_LOG_CATEGORY(PositionalEditor);

void FPositionalEditorModule::StartupModule()
{
    if (GUnrealEd != nullptr)
    {
        TSharedPtr<FPositionalGenericJointVisualizer> genericJointVisualizer = MakeShareable(new FPositionalGenericJointVisualizer);
        if (genericJointVisualizer.IsValid())
        {
            GUnrealEd->RegisterComponentVisualizer(UPositionalGenericJoint::StaticClass()->GetFName(), genericJointVisualizer);
            genericJointVisualizer->OnRegister();
        }

        TSharedPtr<FPositionalColliderVisualizer> colliderVisualizer = MakeShareable(new FPositionalColliderVisualizer);
        if (colliderVisualizer.IsValid())
        {
            GUnrealEd->RegisterComponentVisualizer(UPositionalBoxCollider::StaticClass()->GetFName(), colliderVisualizer);
            GUnrealEd->RegisterComponentVisualizer(UPositionalSphereCollider::StaticClass()->GetFName(), colliderVisualizer);
            GUnrealEd->RegisterComponentVisualizer(UPositionalCapsuleCollider::StaticClass()->GetFName(), colliderVisualizer);
            GUnrealEd->RegisterComponentVisualizer(UPositionalCylinderCollider::StaticClass()->GetFName(), colliderVisualizer);
            colliderVisualizer->OnRegister();
        }
    }
}

void FPositionalEditorModule::ShutdownModule()
{
    if (GUnrealEd != nullptr)
    {
        GUnrealEd->UnregisterComponentVisualizer(UPositionalGenericJoint::StaticClass()->GetFName());
        GUnrealEd->UnregisterComponentVisualizer(UPositionalBoxCollider::StaticClass()->GetFName());
        GUnrealEd->UnregisterComponentVisualizer(UPositionalSphereCollider::StaticClass()->GetFName());
        GUnrealEd->UnregisterComponentVisualizer(UPositionalCapsuleCollider::StaticClass()->GetFName());
        GUnrealEd->UnregisterComponentVisualizer(UPositionalCylinderCollider::StaticClass()->GetFName());
    }
}