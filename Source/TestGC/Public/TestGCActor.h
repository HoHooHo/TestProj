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
	UTestGCObject* TestGCObj;  //׷�ٲ��ɴ�ᱻ�Զ��ͷ�
	UPROPERTY()
	UTestGCObject* TestGCObj_UPROPERTY;  //ʹ�� UPROPERTY ���Σ�׷�ٿɴ���ᱻ�Զ��ͷ�


	//FTestGCStruct
	//FTestGCStruct û���� USTRUCT() ���Σ������κ���ʽ�ı���������ʹ�� UPROPERTY()����
	//�������¼��ֱ����� ׷�ٲ��ɴ����� UMyGCObject* Obj �ᱻ�Զ��ͷ�
	FTestGCStruct TestGCStruct;
	FTestGCStruct* TestGCStructP;
	TSharedPtr<FTestGCStruct> TestGCStructPtr;
	TArray<FTestGCStruct> TestGCStructArray;
	TArray<TSharedPtr<FTestGCStruct>> TestGCStructPtrArray;


	//FTestGCStruct ʹ���� USTRUCT() ����
	//ֻ������ 2 ����ʽ�ı������ſ���ʹ�� UPROPERTY()����
	//��ʹ�� UPROPERTY() ����ʱ��׷�ٿɴ����� UMyGCObject* Obj ���ᱻ�Զ��ͷ�
	UPROPERTY()
	FTestGCUStruct TestGCUStruct_UPROPERTY;
	UPROPERTY()
	TArray<FTestGCUStruct> TestGCUStructArray_UPROPERTY;

	//���¼��ֱ��� ׷�ٲ��ɴ����� UMyGCObject* Obj �ᱻ�Զ��ͷ�
	FTestGCUStruct TestGCUStruct;
	FTestGCUStruct* TestGCUStructP;
	TSharedPtr<FTestGCUStruct> TestGCUStructPtr;
	TArray<FTestGCUStruct> TestGCUStructArray;
	TArray<TSharedPtr<FTestGCUStruct>> TestGCUStructPtrArray;

};
