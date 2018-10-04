// Fill out your copyright notice in the Description page of Project Settings.

#include "TravelPlusSaveGameModeBase.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Json/Public/Serialization/JsonReader.h"
#include "Runtime/Json/Public/Serialization/JsonSerializer.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"
#include "Runtime/Json/Public/Dom/JsonObject.h"
#include <string>
#include <shlobj.h>


ATravelPlusSaveGameModeBase::ATravelPlusSaveGameModeBase() {
	timeConstructed =FDateTime::Now().GetTimeOfDay().GetTotalMicroseconds();
	ATravelPlusSaveGameModeBase::InitParams();
	//GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::Yellow, FString::Printf(TEXT("GM Constructed timeConstructed = %f"), timeConstructed));
	
}

FAdminInformation ATravelPlusSaveGameModeBase::GetAdminInformation()
{
	FAdminInformation ServerAnswer;
	return ServerAnswer;
}

void ATravelPlusSaveGameModeBase::BeginPlay()
{
	
	timeBeginPlay= FDateTime::Now().GetTimeOfDay().GetTotalMicroseconds();
	GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::Blue, FString::Printf(TEXT("GM Constructed time from constructed to begin play (seconds) = %f"), (timeBeginPlay- timeConstructed)/1000000));

	Super::BeginPlay();


}

void ATravelPlusSaveGameModeBase::MyServerTravel(FString mapName)
{
	GetWorld()->ServerTravel(FString(mapName));
}

void ATravelPlusSaveGameModeBase::GetMyDocuments(FString & MyDocumentsPath)
{
	MyDocumentsPath = MyDocuments;
}

void ATravelPlusSaveGameModeBase::InitParams()
{

	wchar_t Folder[1024];
	char str[1024];
	HRESULT hr = SHGetFolderPathW(0, CSIDL_MYDOCUMENTS, 0, 0, Folder);
	if (SUCCEEDED(hr))
	{
		sprintf(str, "%ws", Folder);
	}
	FString StringTemp(str);
	FString JsonString;
	MyDocuments = StringTemp;
	FString FileNamePath3 = MyDocuments + "/Narratex/serverJson.json";
	if (FFileHelper::LoadFileToString(JsonString, *FileNamePath3))
	{
		UE_LOG(LogTemp, Warning, TEXT("Settings is : %s"), *JsonString);
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
		FString teststr;

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{

			AdminInformation.Language = JsonObject->GetIntegerField(TEXT("language"));
			AdminInformation.PlayersNum = JsonObject->GetIntegerField(("playerCount"));
			AdminInformation.MapID = JsonObject->GetIntegerField(("mapId"));
			AdminInformation.ServerPortAdmin = JsonObject->GetIntegerField(("ServerPortAdmin"));
			AdminInformation.ServerIpAdmin = JsonObject->GetStringField(("ServerIpAdmin"));


			auto objects = JsonObject->GetArrayField("infoPlayersInGames");
			for (int i = 0; i<objects.Num(); i++)
			{
				TSharedPtr<FJsonObject> PlayerJsonObject = objects[i]->AsObject();
				FPlayerInformation playerInformation;

				playerInformation.PlayerGender = static_cast<EGenderTypeBase>(PlayerJsonObject->GetIntegerField(TEXT("playerGender")));
				playerInformation.PlayerIP = PlayerJsonObject->GetStringField(TEXT("playerIp"));
				playerInformation.PlayerId = PlayerJsonObject->GetIntegerField(TEXT("playerId"));

				AdminInformation.Players.Add(playerInformation);
			}
			UE_LOG(LogTemp, Warning, TEXT("Server JSON deserialized!"));

		}

		else
		{
			UE_LOG(LogTemp, Warning, TEXT("JSON aren't deserialized!"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("File open error!"));
	}
}
