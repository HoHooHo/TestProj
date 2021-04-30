// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGCActor.h"

// Sets default values
ATestGCActor::ATestGCActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

ATestGCActor::~ATestGCActor()
{
	if (TestGCStructP)
	{
		delete TestGCStructP;
	}

	if (TestGCUStructP)
	{
		delete TestGCUStructP;
	}
}

// Called when the game starts or when spawned
void ATestGCActor::BeginPlay()
{
	Super::BeginPlay();
	
	/*
	TestGCObj = NewObject<UTestGCObject>(this);
	TestGCObj_UPROPERTY = NewObject<UTestGCObject>();


	TestGCObj->Obj = NewObject<UMyGCObject>(TestGCObj);
	TestGCObj_UPROPERTY->Obj = NewObject<UMyGCObject>();

	TestGCObj->SetMyName(TEXT("TestGCObj"));
	TestGCObj_UPROPERTY->SetMyName(TEXT("TestGCObj_UPROPERTY"));
	*/

	/*
	TestGCStruct.Obj = NewObject<UMyGCObject>();
	TestGCUStruct.Obj = NewObject<UMyGCObject>();
	TestGCUStruct_UPROPERTY.Obj = NewObject<UMyGCObject>();

	TestGCStruct.SetMyName(TEXT("TestGCStruct"));
	TestGCUStruct.SetMyName(TEXT("TestGCUStruct"));
	TestGCUStruct_UPROPERTY.SetMyName(TEXT("TestGCUStruct_UPROPERTY"));
	*/

	/*
	TestGCStructP = new FTestGCStruct();
	TestGCUStructP = new FTestGCUStruct();

	TestGCStructP->Obj = NewObject<UMyGCObject>();
	TestGCUStructP->Obj = NewObject<UMyGCObject>();

	TestGCStructP->SetMyName(TEXT("TestGCStructP"));
	TestGCUStructP->SetMyName(TEXT("TestGCUStructP"));
	*/

	/*
	TestGCStructPtr = MakeShareable(new FTestGCStruct());
	TestGCUStructPtr = MakeShareable(new FTestGCUStruct());

	TestGCStructPtr->Obj = NewObject<UMyGCObject>();
	TestGCUStructPtr->Obj = NewObject<UMyGCObject>();

	TestGCStructPtr->SetMyName(TEXT("TestGCStructPtr"));
	TestGCUStructPtr->SetMyName(TEXT("TestGCUStructPtr"));
	*/

	/*
	FTestGCStruct TGCStruct;
	FTestGCUStruct TGCUStruct;
	FTestGCUStruct TGCUStruct_UPROPERTY;

	TGCStruct.Obj = NewObject<UMyGCObject>();
	TGCUStruct.Obj = NewObject<UMyGCObject>();
	TGCUStruct_UPROPERTY.Obj = NewObject<UMyGCObject>();

	TGCStruct.SetMyName(TEXT("TGCStruct"));
	TGCUStruct.SetMyName(TEXT("TGCUStruct"));
	TGCUStruct_UPROPERTY.SetMyName(TEXT("TGCUStruct_UPROPERTY"));

	TestGCStructArray.Emplace(TGCStruct);
	TestGCUStructArray.Add(TGCUStruct);
	TestGCUStructArray_UPROPERTY.Add(TGCUStruct_UPROPERTY);
	*/

	/*
	TestGCStructPtrArray.Add(TestGCStructPtr);
	TestGCUStructPtrArray.Add(TestGCUStructPtr);
	*/

}

// Called every frame
void ATestGCActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

