// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Blueprint.h"
#include "MyBlueprint.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMBLUEPRINT_API UMyBlueprint : public UBlueprint
{
	GENERATED_BODY()
	
#if WITH_EDITOR
public:
	// UBlueprint interface
	virtual bool SupportedByDefaultBlueprintFactory() const override
	{
		return false;
	}
	// End of UBlueprint interface

	/** Returns the most base UMyBlueprint for a given blueprint (if it is inherited from another UMyBlueprint, returning null if only native / non UMyBlueprint classes are it's parent) */
	UMyBlueprint* FindRootMyBlueprint();

#endif
};
