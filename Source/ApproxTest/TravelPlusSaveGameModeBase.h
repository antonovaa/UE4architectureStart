// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TravelPlusSaveGameModeBase.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EGenderTypeBase : uint8
{
	Man 	UMETA(DisplayName = "Man"),
	Woman 	UMETA(DisplayName = "Woman"),
};

USTRUCT(BlueprintType)
struct APPROXTEST_API FPlayerInformation
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EGenderTypeBase PlayerGender = EGenderTypeBase::Man;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString PlayerIP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PlayerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PlayerPort = 7778;
};

USTRUCT(BlueprintType)
struct APPROXTEST_API FAdminInformation
{
	GENERATED_USTRUCT_BODY()
public:

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
		TArray <FPlayerInformation> Players;

};


UCLASS()
class APPROXTEST_API ATravelPlusSaveGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;


public:
	ATravelPlusSaveGameModeBase();


		UFUNCTION(BlueprintCallable, Category = "JSONParsing Admin Information")
		FAdminInformation GetAdminInformation();

		UPROPERTY(BlueprintReadOnly)
			FAdminInformation AdminInformation;

		UPROPERTY(BlueprintReadOnly)
		float timeConstructed;

		UPROPERTY(BlueprintReadOnly)
		FString MyDocuments;

		UPROPERTY(BlueprintReadOnly)
		float timeBeginPlay;

		UFUNCTION(BlueprintCallable, Category = "Networking", meta = (DisplayName = "Server Travel"))
		void MyServerTravel(FString mapName);
	
		UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MyDocumentsPath", meta = (DisplayName = "Get My Documents Address"))
			void GetMyDocuments(FString& MyDocumentsPath);

			void InitParams();

};
