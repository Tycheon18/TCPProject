// Fill out your copyright notice in the Description page of Project Settings.
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/prewindowsapi.h"

#include <WinSock2.h>
#include <iostream>

#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"

#include "TCPSocketActor.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"

#include "FTCPThread.h"

#pragma comment(lib, "ws2_32.lib")

// Sets default values
ATCPSocketActor::ATCPSocketActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

bool ATCPSocketActor::ConnectServer()
{
	//WSAData	wsaData;
	//WSAStartup(MAKEWORD(2, 2), &wsaData);

	//Socket = socket(AF_INET, SOCK_STREAM, 0);

	//SOCKADDR_IN SockAddr;

	//memset(&SockAddr, 0, sizeof(SockAddr));

	//SockAddr.sin_family = PF_INET;
	//SockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//SockAddr.sin_port = htons(3001);

	//int Connected = connect(Socket, (SOCKADDR*)&SockAddr, sizeof(SockAddr));
	//Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));


	ClientSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("DefaultSocket"), false);
	ClientAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	int32 port = 3001;

	FString IP = TEXT("127.0.0.1");
	FIPv4Address TemporaryAddr;
	FIPv4Address::Parse(IP, TemporaryAddr);

	ClientAddress->SetPort(port);
	ClientAddress->SetIp(TemporaryAddr.Value);
	
	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, FString::Printf(TEXT("Trying to connect.")));
	
	return (ClientSocket->Connect(*ClientAddress));
}

void ATCPSocketActor::SendText()
{
	
	//char* Buffer = new char[512];
	//char Recv[512] = { 0, };
	
	char* ClientText = new char[512];
	ClientText = TCHAR_TO_UTF8(*Text);

	uint8_t Buffer[512];
	memset(Buffer, '\0', std::size(Buffer));

	for (int i = 0; i < strlen(ClientText); ++i)
	{
		Buffer[i] = ClientText[i];
	}

	//uint8 Buffer = FString::uint8

	//memcpy(Buffer, &ClientText, sizeof(char));

	int32 BytesSent = 0;
	//if(ClientSocket->GetConnectionState() == ESocketConnectionState::SCS_Connected)
	//{
	ClientSocket->Send(Buffer, std::size(Buffer), BytesSent);
	//}

	//uint8_t RecvBuf[512];

	//int32 BytesRecv = 0;
	//ClientSocket->Recv(RecvBuf, 512, BytesSent);

	//char* RecvClientText = new char[512];
	//memset(RecvClientText, '\0', strlen(RecvClientText));

	//for (int i = 0; i < std::size(RecvBuf); ++i)
	//{
	//	RecvClientText[i] = RecvBuf[i];
	//}

	//RecvText = RecvClientText;

	//SendByte = send(Socket, Buffer, strlen(Buffer) + 1, 0);

	//RecvByte = recv(Socket, Recv, 512, 0);

	//RecvText = FString(UTF8_TO_TCHAR(Recv));

	//if (RecvText == "1" || RecvText == "2")
	//{
	//	RecvByte = recv(Socket, Recv, 512, 0);
	//	RecvText = FString(UTF8_TO_TCHAR(Recv));

	//	char* IDBuf = new char[512];
	//	char* PWBuf = new char[512];

	//	IDBuf = TCHAR_TO_UTF8(*ID);
	//	PWBuf = TCHAR_TO_UTF8(*PW);

	//	SendByte = send(Socket, IDBuf, strlen(IDBuf) + 1, 0);
	//	SendByte = send(Socket, PWBuf, strlen(PWBuf) + 1, 0);

	//	char Check[512] = { 0, };

	//	RecvByte = recv(Socket, Check, 512, 0);
	//	CheckText = FString(UTF8_TO_TCHAR(Check));
	//}

	//delete[] ClientText;
}

void ATCPSocketActor::BeginPlay()
{
	
	if (ConnectServer())
	{
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, FString::Printf(TEXT("Connect Succeed!")));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, FString::Printf(TEXT("Connect Falied")));
	}
	
}

void ATCPSocketActor::Tick(float DeltaTime)
{
}

void ATCPSocketActor::Send(uint32 Type, const FString& SendText)
{
	SCOPE_CYCLE_COUNTER(STAT_Send)
		FTCHARToUTF8 Convert(*Text);
	FArrayWriter WriterArray;
	WriterArray.Serialize((UTF8CHAR*)Convert.Get(), Convert.Length());

	if (/*ATCPSocketActor::SendPacket(Socket, Type, WriterArray)*/1)
	{
		UE_LOG(LogTemp, Log, TEXT("Sent Text : %s  Size : %d"), *Text, WriterArray.Num());
	}
}

void ATCPSocketActor::StartThread()
{
	if(!TCPThread)
	{
		TCPThread = FRunnableThread::Create(FTCPThreadInstance, TEXT("TCPThread"));
		while (!FTCPThreadInstance->IsThreadRunning())
		{
			FPlatformProcess::Sleep(0.1f);
		}
	}
}

void ATCPSocketActor::Login()
{
	uint8_t Buffer[512];
	memset(Buffer, '\0', std::size(Buffer));

	Buffer[0] = '1';

	int32 BytesSent = 0;
	ClientSocket->Send(Buffer, std::size(Buffer), BytesSent);

	char* ClientText = new char[512];
	ClientText = TCHAR_TO_UTF8(*ID);


	memset(Buffer, '\0', std::size(Buffer));

	for (int i = 0; i < strlen(ClientText); ++i)
	{
		Buffer[i] = ClientText[i];
	}

	ClientSocket->Send(Buffer, std::size(Buffer), BytesSent);

	ClientText = TCHAR_TO_UTF8(*PW);

	memset(Buffer, '\0', std::size(Buffer));

	for (int i = 0; i < strlen(ClientText); ++i)
	{
		Buffer[i] = ClientText[i];
	}

	ClientSocket->Send(Buffer, std::size(Buffer), BytesSent);

	RecvText = "";

	uint8_t RecvBuf[512];
	RecvByte = 0;

	ClientSocket->Recv(RecvBuf, 512, RecvByte);

	char* RecvClientText = new char[512];
	memset(RecvClientText, '\0', strlen(RecvClientText));

	for (int i = 0; i < std::size(RecvBuf); ++i)
	{
		RecvClientText[i] = RecvBuf[i];
	}

	RecvText = RecvClientText;

	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, RecvText);

	if (RecvText == "Login Success!")
	{
		isLogin = true;
	}
}

void ATCPSocketActor::SignUp()
{
	uint8_t Buffer[512];
	memset(Buffer, '\0', std::size(Buffer));

	Buffer[0] = '2';
	int32 BytesSent = 0;
	ClientSocket->Send(Buffer, std::size(Buffer), BytesSent);

	char* ClientText = new char[512];
	ClientText = TCHAR_TO_UTF8(*ID);


	memset(Buffer, '\0', std::size(Buffer));

	for (int i = 0; i < strlen(ClientText); ++i)
	{
		Buffer[i] = ClientText[i];
	}

	ClientSocket->Send(Buffer, std::size(Buffer), BytesSent);

	ClientText = TCHAR_TO_UTF8(*PW);

	memset(Buffer, '\0', std::size(Buffer));

	for (int i = 0; i < strlen(ClientText); ++i)
	{
		Buffer[i] = ClientText[i];
	}

	ClientSocket->Send(Buffer, std::size(Buffer), BytesSent);


	RecvText = "";

	uint8_t RecvBuf[512];
	RecvByte = 0;

	ClientSocket->Recv(RecvBuf, 512, RecvByte);

	char* RecvClientText = new char[512];
	memset(RecvClientText, '\0', strlen(RecvClientText));

	for (int i = 0; i < std::size(RecvBuf); ++i)
	{
		RecvClientText[i] = RecvBuf[i];
	}

	RecvText = RecvClientText;

	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, RecvText);


}

void ATCPSocketActor::StopThread()
{
	if (FTCPThreadInstance->IsThreadRunning())
	{
		FTCPThreadInstance->Stop();
		TCPThread->WaitForCompletion();

		delete FTCPThreadInstance;
		TCPThread = nullptr;
	}
}

void ATCPSocketActor::SendToThread()
{
	char* ClientText = new char[512];
	ClientText = TCHAR_TO_UTF8(*Text);

	uint8_t Buffer[512];
	memset(Buffer, '\0', std::size(Buffer));

	for (int i = 0; i < strlen(ClientText); ++i)
	{
		Buffer[i] = ClientText[i];
	}

	int32 BytesSent = 0;

	FSocket* ThreadSocket = FTCPThreadInstance->ServerSocket;

	ThreadSocket->Send(Buffer, std::size(Buffer), BytesSent);
}

void ATCPSocketActor::ReceiveToThread()
{
	RecvText = FTCPThreadInstance->RecvText;
}

bool ATCPSocketActor::SuccessLogin()
{
	return isLogin;
}
