// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraphSchema_K2.h"
#include "EdGraphSchema_K2_MyBlueprint.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI)
class UEdGraphSchema_K2_MyBlueprint : public UEdGraphSchema_K2
{
	GENERATED_BODY()
	
public:
	/**
	* Creates a new variable getter node and adds it to ParentGraph
	*
	* @param	GraphPosition		The location of the new node inside the graph
	* @param	ParentGraph			The graph to spawn the new node in
	* @param	VariableName		The name of the variable
	* @param	Source				The source of the variable
	* @return	A pointer to the newly spawned node
	*/
	virtual UK2Node_VariableGet* SpawnVariableGetNode(const FVector2D GraphPosition, class UEdGraph* ParentGraph, FName VariableName, UStruct* Source) const override;

	/**
	* Creates a new variable setter node and adds it to ParentGraph
	*
	* @param	GraphPosition		The location of the new node inside the graph
	* @param	ParentGraph			The graph to spawn the new node in
	* @param	VariableName		The name of the variable
	* @param	Source				The source of the variable
	* @return	A pointer to the newly spawned node
	*/
	virtual UK2Node_VariableSet* SpawnVariableSetNode(const FVector2D GraphPosition, class UEdGraph* ParentGraph, FName VariableName, UStruct* Source) const override;

	/* When a node is removed, this method determines whether we should remove it immediately or use the old (slower) code path that results in all node being recreated: */
	virtual bool ShouldAlwaysPurgeOnModification() const override { return true; };
};
