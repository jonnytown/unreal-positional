#include "unreal_positional_editor.h"
#include "PositionalGenericJointVisualizer.h"
#include "Editor/UnrealEd/Classes/Editor/UnrealEdEngine.h"
#include "UnrealEdGlobals.h"

IMPLEMENT_GAME_MODULE(FPositionalEditorModule, unreal_positional_editor);

DEFINE_LOG_CATEGORY(PositionalEditor);

void FPositionalEditorModule::StartupModule()
{
    UE_LOG(PositionalEditor, Warning, TEXT("PositionalEditor: Log Started"));
    if (GUnrealEd != nullptr)
    {
        TSharedPtr<FPositionalGenericJointVisualizer> visualizer = MakeShareable(new FPositionalGenericJointVisualizer);
        if (visualizer.IsValid())
        {
            GUnrealEd->UnregisterComponentVisualizer(UPositionalGenericJoint::StaticClass()->GetFName());
            GUnrealEd->RegisterComponentVisualizer(UPositionalGenericJoint::StaticClass()->GetFName(), visualizer);
            visualizer->OnRegister();
            UE_LOG(PositionalEditor, Warning, TEXT("Generic Joint Visualizer Registered"));
        }

    }
}

void FPositionalEditorModule::ShutdownModule()
{
    if (GUnrealEd != nullptr)
    {
        GUnrealEd->UnregisterComponentVisualizer(UPositionalGenericJoint::StaticClass()->GetFName());
    }
    UE_LOG(PositionalEditor, Warning, TEXT("Generic Joint Visualizer Unregistered"));
}