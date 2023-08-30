// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpRequestActor.generated.h"

UCLASS()
class TEAM20_API AHttpRequestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AHttpRequestActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SendRequest(const FString url);
	void PostRequest(const FString url);
	void SaveJson(const FString jsonData);
	void GetImage(const FString url);
	void SaveImage(const UTexture2D* tex);
	void PostImage(const FString url, const UTexture2D* tex);

private:
	void OnReceivedData(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void OnPostData(TSharedPtr<IHttpRequest> Request, TSharedPtr<IHttpResponse> Response, bool bConnectedSuccessfully);
	void OnGetImageData(TSharedPtr<IHttpRequest> Request, TSharedPtr<IHttpResponse> Response, bool bConnectedSuccessfully);
	void OnPostTextureData(TSharedPtr<IHttpRequest> Request, TSharedPtr<IHttpResponse> Response, bool bConnectedSuccessfully);


	UPROPERTY()
	class AHttpGameModeBase* gm;

	//FString baseURLTest = "http://192.168.1.60:5000";
	FString baseURLTest = "http://127.0.0.1:8000";

};
