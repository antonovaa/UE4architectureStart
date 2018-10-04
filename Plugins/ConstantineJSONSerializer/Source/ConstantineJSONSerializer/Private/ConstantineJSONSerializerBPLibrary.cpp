// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ConstantineJSONSerializer.h"
#include "ConstantineJSONSerializerBPLibrary.h"
#include <string>
#include <shlobj.h>


UConstantineJSONSerializerBPLibrary::UConstantineJSONSerializerBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

bool UConstantineJSONSerializerBPLibrary::CheckGamePlayLevel(FString ServerIpAdmin,int32 ServerPortAdmin)
{
		FSocket* Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("TCP_TEST"), false);
		FIPv4Address ip;
		FIPv4Address::Parse(ServerIpAdmin, ip);
		TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr(ip.Value, ServerPortAdmin);

		bool connected = Socket->Connect(*addr);

		if (connected)
		{
			FString serialized = "startLevel";
			TCHAR* serializedChar = serialized.GetCharArray().GetData();
			int32 size = FCString::Strlen(serializedChar);
			int32 sent = 0;

			bool successful = Socket->Send((uint8*)TCHAR_TO_UTF8(serializedChar), size, sent);
			if (successful)
			{
				return true;
			}
			return false;
		}
		return false;
}


bool UConstantineJSONSerializerBPLibrary::SetStringByPlayLevel(FString ServerIpAdmin, int32 ServerPortAdmin)
{
	FSocket* Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("TCP_TEST"), false);
	FIPv4Address ip;
	FIPv4Address::Parse(ServerIpAdmin, ip);
	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr(ip.Value, ServerPortAdmin);

	bool connected = Socket->Connect(*addr);

	if (connected)
	{
		FString serialized = "startLevel";
		TCHAR* serializedChar = serialized.GetCharArray().GetData();
		int32 size = FCString::Strlen(serializedChar);
		int32 sent = 0;

		bool successful = Socket->Send((uint8*)TCHAR_TO_UTF8(serializedChar), size, sent);
		if (successful)
		{
			return true;
		}
		return false;
	}
	return false;
}

FString UConstantineJSONSerializerBPLibrary::GetDocumentPath() {
	wchar_t Folder[1024];
	char str[1024];
	HRESULT hr = SHGetFolderPathW(0, CSIDL_MYDOCUMENTS, 0, 0, Folder);
	if (SUCCEEDED(hr))
	{
		sprintf(str, "%ws", Folder);
	}

	FString FileNamePath(str);
	return FileNamePath;
}

FJSONServerAnswer UConstantineJSONSerializerBPLibrary::DeserializeServerJSON()
{
	FString JsonString;
	FJSONServerAnswer ServerAnswer;
	
	FString FileNamePath = GetDocumentPath() + "/Narratex/serverJson.json";
	if (FFileHelper::LoadFileToString(JsonString, *FileNamePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Settings is : %s"), *JsonString);

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{

			ServerAnswer.Language = JsonObject->GetIntegerField(TEXT("language"));
			ServerAnswer.PlayersNum = JsonObject->GetIntegerField(TEXT("playerCount"));
			ServerAnswer.MapID = JsonObject->GetIntegerField(TEXT("mapId"));
			ServerAnswer.ServerPortAdmin = JsonObject->GetIntegerField(TEXT("ServerPortAdmin"));
			ServerAnswer.ServerIpAdmin = JsonObject->GetStringField(TEXT("ServerIpAdmin"));

			auto objects = JsonObject->GetArrayField("infoPlayersInGames");
			for (int i = 0; i<objects.Num(); i++)
			{
				TSharedPtr<FJsonObject> PlayerJsonObject = objects[i]->AsObject();
				FPlayerInfo PlayerInfo;

				PlayerInfo.PlayerGender = static_cast<EGenderType>(PlayerJsonObject->GetIntegerField(TEXT("playerGender")));
				PlayerInfo.PlayerIP = PlayerJsonObject->GetStringField(TEXT("playerIp"));
				PlayerInfo.PlayerId = PlayerJsonObject->GetIntegerField(TEXT("playerId"));

				ServerAnswer.Players.Add(PlayerInfo);
			}
			UE_LOG(LogTemp, Warning, TEXT("Server JSON deserialized!"));
			return ServerAnswer;
		}

		else
		{
			UE_LOG(LogTemp, Warning, TEXT("JSON aren't deserialized!"));
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("File open error!"));
	return ServerAnswer;
}

FJSONClientAnswer UConstantineJSONSerializerBPLibrary::DeserializeClientJSON()
{
	FString JsonString;
	FJSONClientAnswer ClientAnswer;

	FString FileNamePath = GetDocumentPath() + "/Narratex/clientJson.json";

	if (FFileHelper::LoadFileToString(JsonString, *FileNamePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Settings is : %s"), *JsonString);

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory <>::Create(JsonString);

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			ClientAnswer.ServerIP = JsonObject->GetStringField(TEXT("ServerIP"));
			ClientAnswer.ServerPort = JsonObject->GetIntegerField(TEXT("ServerPort"));
			ClientAnswer.SelfClientIP = JsonObject->GetStringField(TEXT("SelfClientIP"));

			UE_LOG(LogTemp, Warning, TEXT("Client JSON deserialized!"));
			return ClientAnswer;
		}

		else
		{
			UE_LOG(LogTemp, Warning, TEXT("JSON aren't deserialized!"));
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("File open error!"));
	return ClientAnswer;
}

FString UConstantineJSONSerializerBPLibrary::GetRemoteAddressByController(APlayerController* ControllerRef)
{
	FString Address = "127.0.0.1";

	if (ControllerRef && ControllerRef->GetNetConnection())
	{
		UE_LOG(LogTemp, Warning, TEXT("Ip is valid"));
		Address = ControllerRef->GetNetConnection()->LowLevelGetRemoteAddress();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid PC"));
	}

	return Address;
}

FPlayerInfo UConstantineJSONSerializerBPLibrary::GetPlayerInfoByController(TArray <FPlayerInfo> Players, APlayerController* ControllerRef)
{
	FString  PlayerIP = GetRemoteAddressByController(ControllerRef);
	for (int i = 0; i < Players.Num(); i++)
	{
		if (PlayerIP == Players[i].PlayerIP)
		{
			return Players[i];
		}
	}

	FPlayerInfo PlayerByDefault;
	return PlayerByDefault;
}

FPlayerInfo UConstantineJSONSerializerBPLibrary::GetPlayerInfoByClientIP(TArray <FPlayerInfo> Players, FString PlayerIP)
{
	for (int i = 0; i < Players.Num(); i++)
	{
		if (PlayerIP == Players[i].PlayerIP)
		{
			return Players[i];
		}
	}

	FPlayerInfo PlayerByDefault;
	return PlayerByDefault;
}

void UConstantineJSONSerializerBPLibrary::PrintLogString(FString Message)
{
	UE_LOG(LogTemp, Log, TEXT("Message: %s"), *Message);
}