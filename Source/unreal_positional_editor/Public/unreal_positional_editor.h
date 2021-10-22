#pragma once

#include "Engine.h"
#include "UnrealEd.h"

DECLARE_LOG_CATEGORY_EXTERN(PositionalEditor, All, All);

class FPositionalEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};