// Copyright (c) 2020 Epgenix

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NetworkReplayStreaming.h"
#include "ReplaySystemAdvancedInstance.generated.h"


/**
 *
 */
USTRUCT(BlueprintType)
struct FS_ReplaySystemReplayInfo
{
    GENERATED_BODY()

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replay")
        FString ReplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replay")
        FString FriendlyName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replay")
        FDateTime Timestamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replay")
        int32 LengthInMS;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replay")
        bool bIsValid;

    FS_ReplaySystemReplayInfo(FString NewName, FString NewFriendlyName, FDateTime NewTimestamp, int32 NewLengthInMS)
    {
        ReplayName = NewName;
        FriendlyName = NewFriendlyName;
        Timestamp = NewTimestamp;
        LengthInMS = NewLengthInMS;
        bIsValid = true;
    }

    FS_ReplaySystemReplayInfo()
    {
        ReplayName = "Replay";
        FriendlyName = "Replay";
        Timestamp = FDateTime::MinValue();
        LengthInMS = 0;
        bIsValid = false;
    }
};

UCLASS()
class REPLAYSYSTEMADVANCED_API UReplaySystemAdvancedInstance : public UGameInstance
{
    GENERATED_BODY()

public:

    UReplaySystemAdvancedInstance();
	

    UPROPERTY(EditDefaultsOnly, Category = "Replays")
        FString RecordingName;

    UPROPERTY(EditDefaultsOnly, Category = "Replays")
        FString FriendlyRecordingName;

    UFUNCTION(BlueprintCallable, Category = "Replays")
        void StartRecording(FString ReplayName, FString FriendlyName);

    UFUNCTION(BlueprintCallable, Category = "Replays")
        void StopRecording();

    UFUNCTION(BlueprintCallable, Category = "Replays")
        void StartReplay(FString ReplayName);

    UFUNCTION(BlueprintCallable, Category = "Replays")
        void ListReplays(FString Directory, FString FilesStartingWith, FString FileExtensions, const bool FullPath, TArray<FString> DirectoriesToSkip);

    /*Apply a new custom name to the replay (for UI only) */
    UFUNCTION(BlueprintCallable, Category = "Replays")
        void RenameReplay(const FString& ReplayName, const FString& NewReplayName);

    UFUNCTION(BlueprintCallable, Category = "Replays")
        void DeleteReplay(const FString& ReplayName);


    virtual void Init() override;

protected:

    UFUNCTION(BlueprintImplementableEvent, Category = "Replays")
        void OnListReplaysComplete(const TArray<FString>& ListedReplays);
private:

    // for FindReplays() 
    TSharedPtr<INetworkReplayStreamer> EnumerateStreamsPtr;
    FOnEnumerateStreamsComplete OnEnumerateStreamsCompleteDelegate;

    FOnDeleteFinishedStreamComplete OnDeleteFinishedStreamCompleteDelegate;

    void OnDeleteFinishedStreamComplete(const bool bDeleteSucceeded);
};
