// Fill out your copyright notice in the Description page of Project Settings.

// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Speech.h"
#include "Serialization/JsonReader.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"



// Sets default values
ASpeech::ASpeech() :
	Result{}
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASpeech::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpeech::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpeech::SpeechInit()
{
	FAutoDeleteAsyncTask<FSpeechTask>* SpeechTask = new FAutoDeleteAsyncTask<FSpeechTask>();

	if (SpeechTask)
	{
		SpeechTask->StartBackgroundTask();
	}
	else
	{
		UE_LOG(SnowingError, Error, TEXT("XunFei task object could not be create !"));
		return;
	}

	UE_LOG(SnowingWarning, Warning, TEXT("XunFei Task Stopped !"));
	return;
}

void ASpeech::SpeechOpen()
{
	xunfeispeech->SetStart();
	return;
}

void ASpeech::SpeechStop()
{
	xunfeispeech->SetStop();
	return;
}

void ASpeech::SpeechQuit()
{
	xunfeispeech->SetQuit();
	Sleep(300);
	return;
}

FString ASpeech::SpeechResult()
{
	Result = FString(UTF8_TO_TCHAR(xunfeispeech->get_result()));
	FString LajiString("{\"sn\":2,\"ls\":true,\"bg\":0,\"ed\":0,\"ws\":[{\"bg\":0,\"cw\":[{\"sc\":0.00,\"w\":\"\"}]}]}");
	int32 LajiIndex = Result.Find(*LajiString);
	if (LajiIndex != -1)
	{
		Result.RemoveFromEnd(LajiString);
	}
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef< TJsonReader<TCHAR> > Reader = TJsonReaderFactory<TCHAR>::Create(Result);
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		Result.Reset();
		TArray< TSharedPtr<FJsonValue> > TempArray = JsonObject->GetArrayField("ws");
		for (auto rs : TempArray)
		{
			Result.Append((rs->AsObject()->GetArrayField("cw"))[0]->AsObject()->GetStringField("w"));
		}
	}
	UE_LOG(SnowingError, Error, TEXT("%s"), *Result);

	return Result;

}


