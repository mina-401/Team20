// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpRequestActor.h"
#include "HttpModule.h"
#include "HttpGameModeBase.h"
#include "JsonParseLibrary.h"
#include "ImageUtils.h"

AHttpRequestActor::AHttpRequestActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AHttpRequestActor::BeginPlay()
{
	Super::BeginPlay();

	gm = GetWorld()->GetAuthGameMode<AHttpGameModeBase>();

	//FString baseURLTest = "http://192.168.1.60:5000";
	GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Purple, FString::Printf(TEXT("BaseURL: %s"), *baseURLTest), true, FVector2D(1, 1));
	SendRequest(baseURLTest);
}

void AHttpRequestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//req->ProcessRequest();
}

void AHttpRequestActor::SendRequest(const FString url)
{
	// ����� �����ϰ� request �ν��Ͻ��� �����Ѵ�.
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedPtr<IHttpRequest> req = httpModule.CreateRequest();

	// ��û�ϱ� ���� ������ �����Ѵ�.
	req->SetURL(url);
	req->SetVerb(TEXT("GET"));
	req->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// ��û�� �Ϸ�Ǿ��� �� ����� �Լ��� ���ε��Ѵ�.
	//req->OnProcessRequestComplete().BindUFunction(this, FName("OnReceivedData"));
	req->OnProcessRequestComplete().BindUObject(this, &AHttpRequestActor::OnReceivedData);

	// ������ ��û�� ������.
	req->ProcessRequest();
}

void AHttpRequestActor::OnReceivedData(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	UE_LOG(LogTemp, Warning, TEXT("Received Data!"));

	if (bConnectedSuccessfully)
	{
		FString res = Response->GetContentAsString(); //�迭�� �ޱ�?
		//FString parsedData = UJsonParseLibrary::JsonParse(res);

		//GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Purple, FString::Printf(TEXT("Request: %s"), *res), true, FVector2D(1, 1));
		GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Purple, FString::Printf(TEXT("Request Successfull!")), true, FVector2D(1, 1));
		//gm->SetLogText(parsedData);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Purple, FString::Printf(TEXT("Request Failed...")), true, FVector2D(1, 1));

		//gm->SetLogText("Failed Response from server...");
	}
}

// POST �޼ҵ� ��û�ϱ�
void AHttpRequestActor::PostRequest(const FString url)
{
	TMap<FString, FString> studentData;
	studentData.Add("Name", "AAA");
	studentData.Add("Age", "50");
	studentData.Add("Height", "200");

	FString myJsonData = UJsonParseLibrary::MakeJson(studentData);
	//gm->SetLogText(myJsonData);

	// ��û ����
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();
	req->SetURL(url);
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	req->SetContentAsString(myJsonData);
	req->OnProcessRequestComplete().BindUObject(this, &AHttpRequestActor::OnPostData);
	req->ProcessRequest();
}

// Json ���� �����ϱ�
void AHttpRequestActor::SaveJson(const FString jsonData)
{
	// ����, ������ �̸��� ������ ������ �ش� ������ �����.
	FPlatformFileManager& fileManager = FPlatformFileManager::Get();
	IPlatformFile& platformFile = fileManager.GetPlatformFile();

	FString dirPath = FPaths::ProjectContentDir() + "/JsonData";
	if (!platformFile.DirectoryExists(*dirPath))
	{
		platformFile.CreateDirectory(*dirPath);
	}

	// json �����͸� ���Ϸ� �����Ѵ�.
	FString fullPath = dirPath + "/MyJson.json";
	UE_LOG(LogTemp, Warning, TEXT("save Path: %s"), *fullPath);
	bool bIsSaved = FFileHelper::SaveStringToFile(jsonData, *fullPath);
	/*gm->SetLogText(FString::Printf(TEXT("%s"), bIsSaved ? *FString("Json Saved Successfully!") : *FString("Failed saving file...")));*/
}


// POST ��û �Լ�
void AHttpRequestActor::OnPostData(TSharedPtr<IHttpRequest> Request, TSharedPtr<IHttpResponse> Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		FString receivedData = Response->GetContentAsString();

		// ���� �����͸� ȭ�鿡 ����Ѵ�.
		//gm->SetLogText(receivedData);

		// ���� �����͸� ���Ϸ� �����Ѵ�.
		SaveJson(receivedData);
	}
	else
	{
		// ��û ���� ���� Ȯ��
		EHttpRequestStatus::Type status = Request->GetStatus();
		switch (status)
		{
		case EHttpRequestStatus::NotStarted:
			break;
		case EHttpRequestStatus::Processing:
			break;
		case EHttpRequestStatus::Failed:
			break;
		case EHttpRequestStatus::Failed_ConnectionError:
			break;
		case EHttpRequestStatus::Succeeded:
			break;
		default:
			break;
		}

		// ���� �ڵ� Ȯ��
		int32 responseCode = Response->GetResponseCode();
		//gm->SetLogText(FString::Printf(TEXT("Response Code: %d"), responseCode));

	}
}

// �̹��� ��û �Լ�
void AHttpRequestActor::GetImage(const FString url)
{
	TSharedRef<IHttpRequest> req = FHttpModule::Get().CreateRequest();
	req->SetURL(url);
	req->SetVerb("GET");
	req->SetHeader(TEXT("Content-Type"), TEXT("image/jpeg"));
	req->OnProcessRequestComplete().BindUObject(this, &AHttpRequestActor::OnGetImageData);
	req->ProcessRequest();
}

void AHttpRequestActor::OnGetImageData(TSharedPtr<IHttpRequest> Request, TSharedPtr<IHttpResponse> Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		TArray<uint8> texBites = Response->GetContent();
		FString imagePath = FPaths::ProjectPersistentDownloadDir() + "/MyGetImage.jpg";
		FFileHelper::SaveArrayToFile(texBites, *imagePath);
		UTexture2D* realTex = FImageUtils::ImportBufferAsTexture2D(texBites);
		/*gm->SetImageTexture(realTex);
		gm->SetLogText("Get Image Successfully!");*/
	}
	else
	{
		//gm->SetLogText(FString::Printf(TEXT("Failed...\nResponse Code: %d"), Response->GetResponseCode()));
	}
}

void AHttpRequestActor::SaveImage(const UTexture2D* tex)
{

}

// �ؽ��� Post �Լ�
void AHttpRequestActor::PostImage(const FString url, const UTexture2D* tex)
{
	TArray<uint8> convertedImage;

	// �ؽ����� �� �ȼ� �÷� ������ �迭�� ��´�.
	FTexture2DMipMap mipData = tex->GetPlatformData()->Mips[0];

	TArray<FColor> imgArr;
	int32 width = mipData.SizeX;
	int32 height = mipData.SizeY;
	imgArr.AddUninitialized(width * height * sizeof(FColor));

	void* pixelData = mipData.BulkData.Lock(LOCK_READ_ONLY);

	if (pixelData != nullptr)
	{
		FMemory::Memcpy(imgArr.GetData(), pixelData, imgArr.Num());
	}
	mipData.BulkData.Unlock();

	// �ؽ����� �ȼ� �÷� �迭�� �̹��� �������� �����Ѵ�.
	FImageUtils::ThumbnailCompressImageArray(width, height, imgArr, convertedImage);

	// �̹��� ����Ʈ �迭�� ����Ʈ �Ѵ�.
	TSharedRef<IHttpRequest> req = FHttpModule::Get().CreateRequest();
	req->SetURL(url);
	req->SetVerb("POST");
	req->SetHeader(TEXT("Content-Type"), TEXT("image/jpeg"));
	req->SetContent(convertedImage);
	req->OnProcessRequestComplete().BindUObject(this, &AHttpRequestActor::OnPostTextureData);
	req->ProcessRequest();
}

void AHttpRequestActor::OnPostTextureData(TSharedPtr<IHttpRequest> Request, TSharedPtr<IHttpResponse> Response, bool bConnectedSuccessfully)
{
	/*gm->SetLogText(FString::Printf(TEXT("%s"), bConnectedSuccessfully ? *Response->GetContentAsString() : *FString(TEXT("Reponse code: %d"), Response->GetResponseCode())));*/
}
