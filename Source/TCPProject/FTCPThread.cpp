// Fill out your copyright notice in the Description page of Project Settings.


#include "FTCPThread.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "IPAddress.h"

FTCPThread::FTCPThread()
{
	//Thread = FRunnableThread::Create(this, TEXT("Network Thread"));
	bIsThreadRunning = false;
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
	bIsRunThread = true;

	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, FString::Printf(TEXT("Thread has been initialized.")));

	ServerSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("DefaultSocket"), false);
	ServerAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	int32 port = 27041;

	FString IP = TEXT("127.0.0.1");
	FIPv4Address TemporaryAddr;
	FIPv4Address::Parse(IP, TemporaryAddr);

	ServerAddress->SetPort(port);
	ServerAddress->SetIp(TemporaryAddr.Value);

	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, FString::Printf(TEXT("Trying to connect.")));

	if (ServerSocket->Connect(*ServerAddress))
	{
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, FString::Printf(TEXT("Thread Connect Success!")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, FString::Printf(TEXT("Thread Connect Falied!")));
	}

	return true;
}

uint32 FTCPThread::Run()
{
	bIsThreadRunning = true;

	while (bIsRunThread)
	{
		if (ServerSocket->GetConnectionState() != ESocketConnectionState::SCS_Connected) break;

		RecvText = "";

		uint8_t RecvBuf[512];
		int32 RecvByte = 0;

		ServerSocket->Recv(RecvBuf, 512, RecvByte);

		char* RecvClientText = new char[512];
		memset(RecvClientText, '\0', strlen(RecvClientText));

		for (int i = 0; i < std::size(RecvBuf); ++i)
		{
			RecvClientText[i] = RecvBuf[i];
		}

		RecvText = RecvClientText;

		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, RecvText);

		//delete[] RecvClientText;

		FPlatformProcess::Sleep(0.1f);
	}

	return 0;
}

void FTCPThread::Stop()
{
	bIsRunThread = false;

	//if (ServerSocket)
	//{
	//	ServerSocket->Close();
	//	delete ServerSocket;
	//	ServerSocket = nullptr;
	//}
}

bool FTCPThread::IsThreadRunning() const
{
	return bIsThreadRunning;
}

