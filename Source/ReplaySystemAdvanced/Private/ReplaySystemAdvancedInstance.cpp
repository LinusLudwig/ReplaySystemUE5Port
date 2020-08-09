// Copyright (c) 2020 Epgenix

#
#include "ReplaySystemAdvancedInstance.h"
#include "ReplaySystemAdvanced.h"
#include "Misc/Paths.h"
#include "NullNetworkReplayStreaming/Public/NullNetworkReplayStreaming.h"
#include "NetworkReplayStreaming.h"
#include "LocalTimestampDirectoryVisitor.h"
#include <stdio.h>
#include "Misc/NetworkVersion.h"
#include "FileManager.h"
#include "FileManagerGeneric.h"
#include "Misc/DateTime.h"

UReplaySystemAdvancedInstance::UReplaySystemAdvancedInstance()
{
	RecordingName = "MyReplay";
	FriendlyRecordingName = "MyReplay";
}

void UReplaySystemAdvancedInstance::Init()
{
	UGameInstance::Init();

	// create a ReplayStreamer for FindReplays() and DeleteReplay(..)
	EnumerateStreamsPtr = FNetworkReplayStreaming::Get().GetFactory().CreateReplayStreamer();

	// Link DeleteReplay() delegate to function
	OnDeleteFinishedStreamCompleteDelegate = FOnDeleteFinishedStreamComplete::CreateUObject(this, &UReplaySystemAdvancedInstance::OnDeleteFinishedStreamComplete);
}

void UReplaySystemAdvancedInstance::OnDeleteFinishedStreamComplete(const bool bDeleteSucceeded)
{
}

void UReplaySystemAdvancedInstance::StartRecording(FString ReplayName, FString FriendlyName)
{
	FriendlyName = FString::Printf(TEXT("Replay-%d-%d-%d"),
		FDateTime::Now().GetYear(), FDateTime::Now().GetMonth(), FDateTime::Now().GetDay());
	StartRecordingReplay(ReplayName, FriendlyName);
	UE_LOG(LogTemp, Display, TEXT("Started replay recording to '%s'"), *FriendlyName);
}

void UReplaySystemAdvancedInstance::StopRecording()
{
	StopRecordingReplay();
	UE_LOG(LogTemp, Display, TEXT("Finished replay recording!"));
}

void UReplaySystemAdvancedInstance::StartReplay(FString ReplayName)
{
	UE_LOG(LogTemp, Display, TEXT("Playing replay recording from '%s'"), *ReplayName);
	PlayReplay(ReplayName);


}

void UReplaySystemAdvancedInstance::ListReplays(FString Directory, FString FilesStartingWith, FString FileExtensions, const bool FullPath, TArray<FString> DirectoriesToSkip)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	TArray<FString> Files;
	FLocalTimestampDirectoryVisitor Visitor(PlatformFile, DirectoriesToSkip, DirectoriesToSkip, false);

	PlatformFile.IterateDirectory(*Directory, Visitor);
	for (TMap<FString, FDateTime>::TIterator TimestampIt(Visitor.FileTimes); TimestampIt; ++TimestampIt)
	{
		const FString filePath = TimestampIt.Key();
		const FString fileName = FPaths::GetCleanFilename(filePath);
		bool shouldAddFile = true;

		// Check if filename starts with required characters
		if (!FilesStartingWith.IsEmpty())
		{
			const FString left = fileName.Left(FilesStartingWith.Len());

			if (!(fileName.Left(FilesStartingWith.Len()).Equals(FilesStartingWith)))
				shouldAddFile = false;
		}

		// Check if file extension is required characters
		if (!FilesStartingWith.IsEmpty())
		{
			if (!(FPaths::GetExtension(fileName, false).Equals(FilesStartingWith, ESearchCase::IgnoreCase)))
				shouldAddFile = false;
		}

		// Add full path to results
		if (shouldAddFile)
		{
			Files.Add(FullPath ? filePath : fileName);
		}
	}
	
	OnListReplaysComplete(Files);
}

void UReplaySystemAdvancedInstance::RenameReplay(const FString& ReplayName, const FString& NewReplayName)
{

}

void UReplaySystemAdvancedInstance::DeleteReplay(const FString& ReplayName)
{
	if (EnumerateStreamsPtr.Get())
	{
		EnumerateStreamsPtr.Get()->DeleteFinishedStream(ReplayName, FDeleteFinishedStreamCallback());
	}
}