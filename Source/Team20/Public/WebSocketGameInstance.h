// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "IWebSocket.h"
#include "WebSocketGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TEAM20_API UWebSocketGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void Shutdown() override;

	TSharedPtr<IWebSocket> WebSocket;
	//FString baseURLTest = "http://172.16.17.11:5000";
	void ReceivedData(const FString& MessageString);

	UPROPERTY()
	class ABaseCharacter* me;
};
