// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TCPPROJECT_API FTCPThread : public FRunnable
{
public:
	FTCPThread();
	~FTCPThread() override;

	bool Init() override;
	uint32 Run() override;
	void Exit() override;

	
private:
	TSharedPtr<FInternetAddr> ServerAddress;
	class FSocket* ServerSocket;
	FRunnableThread* Thread;
};
