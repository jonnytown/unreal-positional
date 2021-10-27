#include "unreal_positional_editor.h"
#include "PositionalGenericJointVisualizer.h"
#include "Editor/UnrealEd/Classes/Editor/UnrealEdEngine.h"
#include "UnrealEdGlobals.h"

IMPLEMENT_GAME_MODULE(FPositionalEditorModule, unreal_positional_editor);

DEFINE_LOG_CATEGORY(PositionalEditor);

void FPositionalEditorModule::StartupModule()
{
    if (GUnrealEd != nullptr)
    {
        TSharedPtr<FPositionalGenericJointVisualizer> visualizer = MakeShareable(new FPositionalGenericJointVisualizer);
        if (visualizer.IsValid())
        {
            GUnrealEd->RegisterComponentVisualizer(UPositionalGenericJoint::StaticClass()->GetFName(), visualizer);
            visualizer->OnRegister();
        }

    }
}

void FPositionalEditorModule::ShutdownModule()
{
    if (GUnrealEd != nullptr)
    {
        GUnrealEd->UnregisterComponentVisualizer(UPositionalGenericJoint::StaticClass()->GetFName());
    }
}