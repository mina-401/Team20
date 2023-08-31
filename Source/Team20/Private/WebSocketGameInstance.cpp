// Fill out your copyright notice in the Description page of Project Settings.


#include "WebSocketGameInstance.h"
#include "WebSocketsModule.h"
#include "IWebSocketsManager.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"


void UWebSocketGameInstance::Init()
{
	Super::Init();

	if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}

	const FString ServerURL = TEXT("ws://172.16.17.11:5555/"); // Your server URL. You can use ws, wss or wss+insecure.
	const FString ServerProtocol = TEXT("ws");              // The WebServer protocol you want to use.

	WebSocket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);
	//WebSocket = FWebSocketsModule::Get().CreateWebSocket(baseURLTest);

	WebSocket->OnConnected().AddLambda([]()
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Successfully connected");
		});

	WebSocket->OnConnectionError().AddLambda([](const FString& Error)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, Error);
		});

	WebSocket->OnClosed().AddLambda([](int32 StatusCode, const FString& Reason, bool bWasClean)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, bWasClean ? FColor::Green : FColor::Red, "Connection closed " + Reason);
		});

	WebSocket->OnMessage().AddLambda([](const FString& MessageString)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "Received message: " + MessageString);

			//TSharedPtr<FJsonObject> JsonObject = MakeSharexable(new FJsonObject());
			//TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(MessageString);

			//if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid()) {
			//	FString InputString;

			//	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "message: " + InputString);*/
			//}

			if (MessageString =="1"){
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "received: " + 1);
			}else if (MessageString == "2"){
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "received: " + 2);
			}
		});

	WebSocket->OnMessageSent().AddLambda([](const FString& MessageString)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Sent message: " + MessageString);
		});

	WebSocket->Connect();
}

void UWebSocketGameInstance::Shutdown()
{
	if (WebSocket->IsConnected())
	{
		WebSocket->Close();
	}
	Super::Shutdown();
}

void UWebSocketGameInstance::ReceivedData(const FString& MessageString)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "call Received message: " + MessageString);
}
