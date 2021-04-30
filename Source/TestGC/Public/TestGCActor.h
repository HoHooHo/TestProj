// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestGCObject.h"
#include "TestGCActor.generated.h"

UCLASS()
class ATestGCActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestGCActor();

	~ATestGCActor();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UTestGCObject
	UTestGCObject* TestGCObj;  //追踪不可达，当GC时会被回收
	UPROPERTY()
	UTestGCObject* TestGCObj_UPROPERTY;  //使用 UPROPERTY 修饰，追踪可达，当GC时不会被回收


	//FTestGCStruct
	//FTestGCStruct 没有用 USTRUCT() 修饰，所以任何形式的变量都不能使用 UPROPERTY()修饰
	//所以以下几种变量均 追踪不可达，当GC时其变量 UMyGCObject* Obj 会被回收
	FTestGCStruct TestGCStruct;
	FTestGCStruct* TestGCStructP;
	TSharedPtr<FTestGCStruct> TestGCStructPtr;
	TArray<FTestGCStruct> TestGCStructArray;
	TArray<TSharedPtr<FTestGCStruct>> TestGCStructPtrArray;


	//FTestGCStruct 使用了 USTRUCT() 修饰
	//只有以下 2 种形式的变量，才可以使用 UPROPERTY()修饰
	//当使用 UPROPERTY() 修饰时，追踪可达，当GC时其变量 UMyGCObject* Obj 不会被回收
	UPROPERTY()
	FTestGCUStruct TestGCUStruct_UPROPERTY;
	UPROPERTY()
	TArray<FTestGCUStruct> TestGCUStructArray_UPROPERTY;

	//以下几种变量 追踪不可达，当GC时其变量 UMyGCObject* Obj 会被回收
	FTestGCUStruct TestGCUStruct;
	FTestGCUStruct* TestGCUStructP;
	TSharedPtr<FTestGCUStruct> TestGCUStructPtr;
	TArray<FTestGCUStruct> TestGCUStructArray;
	TArray<TSharedPtr<FTestGCUStruct>> TestGCUStructPtrArray;

};
