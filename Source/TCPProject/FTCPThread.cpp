// Fill out your copyright notice in the Description page of Project Settings.


#include "FTCPThread.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "IPAddress.h"

FTCPThread::FTCPThread()
{
	Thread = FRunnableThread::Create(this, TEXT("Network Thread"));
}

FTCPThread::~FTCPThread()
{
	if (Thread)
	{
		Thread->WaitForCompletion();
		Thread->Kill();
		delete Thread;
	}
}

bool FTCPThread::Init()
{
	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, FString::Printf(TEXT("Thread has been initialized.")));

	ServerSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("DefaultSocket"), false);
	ServerAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	int32 port = 3001;

	FString IP = TEXT("127.0.0.1");
	FIPv4Address TemporaryAddr;
	FIPv4Address::Parse(IP, TemporaryAddr);

	ServerAddress->SetPort(port);
	ServerAddress->SetIp(TemporaryAddr.Value);




	return true;
}

uint32 FTCPThread::Run()
{
	return uint32();
}

void FTCPThread::Exit()
{
}

