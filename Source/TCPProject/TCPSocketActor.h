// Fill out your copyright notice in the Description page of Project Settings.
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma once

#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/prewindowsapi.h"

#include <WinSock2.h>
#include <iostream>

#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "TCPSocketActor.generated.h"

#pragma comment(lib, "ws2_32.lib")

UCLASS()
class TCPPROJECT_API ATCPSocketActor : public AActor
{
	GENERATED_BODY()
	
	
public:	
	// Sets default values for this actor's properties
	ATCPSocketActor();

	bool ConnectServer();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TCP")
	FString Text = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TCP")
	FString RecvText = "";

	SOCKET Socket;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TCP")
	FString ID = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TCP")
	FString PW = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TCP")
	FString CheckText = "";


	int SendByte = 0;

	int RecvByte = 0;

	UFUNCTION(BlueprintCallable)
	void SendText();


protected:
	// Called when the game starts or when spawnedz
	virtual void BeginPlay() override;

	//FSocket* Socket;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
