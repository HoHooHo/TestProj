// Fill out your copyright notice in the Description page of Project Settings.


#include "EdGraphSchema_K2_MyBlueprint.h"

UK2Node_VariableGet* UEdGraphSchema_K2_MyBlueprint::SpawnVariableGetNode(const FVector2D GraphPosition, class UEdGraph* ParentGraph, FName VariableName, UStruct* Source) const
{
	return Super::SpawnVariableGetNode(GraphPosition, ParentGraph, VariableName, Source);
}

UK2Node_VariableSet* UEdGraphSchema_K2_MyBlueprint::SpawnVariableSetNode(const FVector2D GraphPosition, class UEdGraph* ParentGraph, FName VariableName, UStruct* Source) const
{
	return Super::SpawnVariableSetNode(GraphPosition, ParentGraph, VariableName, Source);
}