// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JsonParseLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TEAM20_API UJsonParseLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static FString JsonParse(const FString& originData);

	static FString MakeJson(const TMap<FString, FString> source);
};
