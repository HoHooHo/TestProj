// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprint.h"

#if WITH_EDITOR

UMyBlueprint* UMyBlueprint::FindRootMyBlueprint()
{
	UMyBlueprint* ParentBP = nullptr;

	for (UClass* SupertClass = ParentClass; SupertClass != UObject::StaticClass(); SupertClass = SupertClass->GetSuperClass())
	{
		if (UMyBlueprint* GeneratedBy = Cast<UMyBlueprint>(SupertClass->ClassGeneratedBy))
		{
			ParentBP = GeneratedBy;
		}
	}

	return ParentBP;
}

#endif