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
	UTestGCObject* TestGCObj;  //׷�ٲ��ɴ��GCʱ�ᱻ����
	UPROPERTY()
	UTestGCObject* TestGCObj_UPROPERTY;  //ʹ�� UPROPERTY ���Σ�׷�ٿɴ��GCʱ���ᱻ����


	//FTestGCStruct
	//FTestGCStruct û���� USTRUCT() ���Σ������κ���ʽ�ı���������ʹ�� UPROPERTY()����
	//�������¼��ֱ����� ׷�ٲ��ɴ��GCʱ����� UMyGCObject* Obj �ᱻ����
	FTestGCStruct TestGCStruct;
	FTestGCStruct* TestGCStructP;
	TSharedPtr<FTestGCStruct> TestGCStructPtr;
	TArray<FTestGCStruct> TestGCStructArray;
	TArray<TSharedPtr<FTestGCStruct>> TestGCStructPtrArray;


	//FTestGCStruct ʹ���� USTRUCT() ����
	//ֻ������ 2 ����ʽ�ı������ſ���ʹ�� UPROPERTY()����
	//��ʹ�� UPROPERTY() ����ʱ��׷�ٿɴ��GCʱ����� UMyGCObject* Obj ���ᱻ����
	UPROPERTY()
	FTestGCUStruct TestGCUStruct_UPROPERTY;
	UPROPERTY()
	TArray<FTestGCUStruct> TestGCUStructArray_UPROPERTY;

	//���¼��ֱ��� ׷�ٲ��ɴ��GCʱ����� UMyGCObject* Obj �ᱻ����
	FTestGCUStruct TestGCUStruct;
	FTestGCUStruct* TestGCUStructP;
	TSharedPtr<FTestGCUStruct> TestGCUStructPtr;
	TArray<FTestGCUStruct> TestGCUStructArray;
	TArray<TSharedPtr<FTestGCUStruct>> TestGCUStructPtrArray;

};
