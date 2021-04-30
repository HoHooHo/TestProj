// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TestGC.h"

DEFINE_LOG_CATEGORY(TestGC)

void FTestGC::StartupModule()
{
	UE_LOG(TestGC, Warning, TEXT("TestGC Module Start")); 
}


void FTestGC::ShutdownModule()
{

	UE_LOG(TestGC, Warning, TEXT("TestGC Module Shutdown")); 
}


IMPLEMENT_GAME_MODULE(FTestGC, TestGC);

