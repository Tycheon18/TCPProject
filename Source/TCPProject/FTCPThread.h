// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"

/**
 * 
 */
class TCPPROJECT_API FTCPThread : public FRunnable
{
public:
	FTCPThread();
	~FTCPThread() override;

	bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	bool IsThreadRunning() const;
	
	class FSocket* ServerSocket = nullptr;

	FString RecvText = "";

private:
	TSharedPtr<FInternetAddr> ServerAddress = nullptr;
	
	FRunnableThread* Thread = nullptr;

	bool bIsRunThread = false;

	bool bIsThreadRunning = false;
};
