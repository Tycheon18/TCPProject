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

#pragma comment(lib, "ws2_32.lib")

// Sets default values
ATCPSocketActor::ATCPSocketActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

bool ATCPSocketActor::ConnectServer()
{
	WSAData	wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	Socket = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN SockAddr;

	memset(&SockAddr, 0, sizeof(SockAddr));

	SockAddr.sin_family = PF_INET;
	SockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	SockAddr.sin_port = htons(3001);

	int Connected = connect(Socket, (SOCKADDR*)&SockAddr, sizeof(SockAddr));
	//Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));



	//FString address = TEXT("127.0.0.1");
	//FIPv4Address ip;
	//FIPv4Address::Parse(address, ip);

	//int32 port = 3001;

	//TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	//addr->SetIp(ip.Value);
	//addr->SetPort(port);

	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, FString::Printf(TEXT("Trying to connect.")));

	
	return (Connected == 0);
}

void ATCPSocketActor::SendText()
{
	
	char* Buffer = new char[512];
	char Recv[512] = { 0, };
	
	Buffer = TCHAR_TO_UTF8(*Text);

	SendByte = send(Socket, Buffer, strlen(Buffer) + 1, 0);

	RecvByte = recv(Socket, Recv, 512, 0);

	RecvText = FString(UTF8_TO_TCHAR(Recv));

	if (RecvText == "1" || RecvText == "2")
	{
		RecvByte = recv(Socket, Recv, 512, 0);
		RecvText = FString(UTF8_TO_TCHAR(Recv));

		char* IDBuf = new char[512];
		char* PWBuf = new char[512];

		IDBuf = TCHAR_TO_UTF8(*ID);
		PWBuf = TCHAR_TO_UTF8(*PW);

		SendByte = send(Socket, IDBuf, strlen(IDBuf) + 1, 0);
		SendByte = send(Socket, PWBuf, strlen(PWBuf) + 1, 0);

		char Check[512] = { 0, };

		RecvByte = recv(Socket, Check, 512, 0);
		CheckText = FString(UTF8_TO_TCHAR(Check));
	}
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

