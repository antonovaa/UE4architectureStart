// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "Runtime/Networking/Public/Networking.h"
#include "Runtime/Sockets/Public/Sockets.h"
#include "Runtime/Sockets/Public/SocketSubsystem.h"
#include "ConstantineJSONSerializerBPLibrary.generated.h"


/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/

UENUM(BlueprintType)
enum class EGenderType : uint8
{
	Man 	UMETA(DisplayName = "Man"),
	Woman 	UMETA(DisplayName = "Woman"),
};

UENUM(BlueprintType)
enum class EJSONType : uint8
{
	Server 	UMETA(DisplayName = "Server"),
	Client 	UMETA(DisplayName = "Client")
};

UENUM(BlueprintType)
enum class EReleCommand : uint8
{
	On 	UMETA(DisplayName = "On"),
	Off 	UMETA(DisplayName = "Off")
};

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EGenderType PlayerGender = EGenderType::Man;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString PlayerIP;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PlayerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PlayerPort = 7778;
};

USTRUCT(BlueprintType)
struct FJSONAnswer
{
	GENERATED_USTRUCT_BODY()

};

USTRUCT(BlueprintType)
struct FJSONServerAnswer : public FJSONAnswer
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PlayersNum = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MapID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Language = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ServerPortAdmin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ServerIpAdmin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray <FPlayerInfo> Players;
};

USTRUCT(BlueprintType)
struct FJSONClientAnswer
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ServerIP = "192.168.0.101";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ServerPort = 8888;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString SelfClientIP = "";


};


UCLASS()
class UConstantineJSONSerializerBPLibrary : public UBlueprintFunctionLibrary
{

	GENERATED_UCLASS_BODY()

		static FSocket* ListenerSocket;

	UFUNCTION(BlueprintCallable, Category = "JSONParsing")
		static FJSONServerAnswer DeserializeServerJSON();

	UFUNCTION(BlueprintCallable, Category = "JSONParsing")
		static FJSONClientAnswer DeserializeClientJSON();
	
	UFUNCTION(BlueprintCallable, Category = "GetDocumentPath")
		static FString GetDocumentPath();

	UFUNCTION(BlueprintCallable, Category = Sockets)
		static bool CheckGamePlayLevel(FString ServerIpAdmin, int32 ServerPortAdmin);

	UFUNCTION(BlueprintCallable, Category = Sockets)
		static bool SetStringByPlayLevel(FString ServerIpAdmin, int32 ServerPortAdmin);

	UFUNCTION(BlueprintCallable, Category = Sockets)
		static FPlayerInfo GetPlayerInfoByClientIP(TArray <FPlayerInfo> Players, FString PlayerIP);

	UFUNCTION(BlueprintCallable, Category = Sockets)
		static FString GetRemoteAddressByController(APlayerController* ControllerRef);

	UFUNCTION(BlueprintCallable, Category = Sockets)
		static FPlayerInfo GetPlayerInfoByController(TArray <FPlayerInfo> Players, APlayerController* ControllerRef);

	UFUNCTION(BlueprintCallable, Category = "Logs")
		static void PrintLogString(FString Message);

};