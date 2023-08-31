// Fill out your copyright notice in the Description page of Project Settings.


#include "WebRequestActor.h"
#include "WebSocketGameInstance.h"

// Sets default values
AWebRequestActor::AWebRequestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWebRequestActor::BeginPlay()
{
	Super::BeginPlay();
	webgm = Cast<UWebSocketGameInstance>(GetGameInstance());

	NotifyServer();
}

// Called every frame
void AWebRequestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWebRequestActor::NotifyServer()
{
	if (webgm == nullptr) return;
	if (!webgm->WebSocket->IsConnected()) return;

	webgm->WebSocket->Send("ImMannnnnnnngnngggu");
}

void AWebRequestActor::SendRequest(const FString MessageString)
{
	
}

