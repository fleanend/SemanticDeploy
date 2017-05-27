// Fill out your copyright notice in the Description page of Project Settings.

#include "RobbieTheRobot.h"
#include "TCP.h"


// Sets default values
ATCP::ATCP()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATCP::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATCP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATCP::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (this->connectionSocket) {
		this->connectionSocket->Close();
	}

	if (this->listenerSocket) {
		this->listenerSocket->Close();
	}
}

bool ATCP::CreateSocket(const FString& socketName, const FString& ip, const int32 port, const int32 receiveBufferSize = 2 * 1024 * 1024)
{
	uint8 ipNums[4];
	this->formatIP4ToNumber(ip, ipNums);
	FIPv4Endpoint endpoint(FIPv4Address(ipNums[0], ipNums[1], ipNums[2], ipNums[3]), port);
	this->listenerSocket = FTcpSocketBuilder(*socketName)
		.AsReusable()
		.BoundToEndpoint(endpoint)
		.Listening(8);

	if (this->listenerSocket == NULL) {
		return false;
	}


	int32 newSize = 0;
	listenerSocket->SetReceiveBufferSize(receiveBufferSize, newSize);

	this->GetWorldTimerManager().SetTimer(this->TimerHandle_LifeSpanExpired, this, &ATCP::connectionListener, 0.10f, true);

	return true;
}

bool ATCP::formatIP4ToNumber(const FString& ip, uint8(&out)[4])
{
	TArray<FString> parts;

	// Remove spaces.
	ip.Replace(TEXT(" "), TEXT(""));

	ip.ParseIntoArray(parts, TEXT("."), true);
	if (parts.Num() != 4)
		return false;

	//String to Number Parts
	for (int32 i = 0; i < 4; ++i)
	{
		out[i] = FCString::Atoi(*parts[i]);
	}

	return true;
}

void ATCP::connectionListener()
{
	if (!this->listenerSocket) return;

	//Remote address
	TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool pending;

	// handle incoming connections
	if (this->listenerSocket->HasPendingConnection(pending) && pending)
	{
		//Already have a Connection? destroy previous
		if (this->connectionSocket)
		{
			this->connectionSocket->Close();
			ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(this->connectionSocket);
		}

		//New Connection receive!
		this->connectionSocket = this->listenerSocket->Accept(*RemoteAddress, TEXT("RamaTCP Received Socket Connection"));

		if (this->connectionSocket != NULL)
		{
			//Global cache of current Remote Address
			this->remoteAddressForConnection = FIPv4Endpoint(RemoteAddress);

			this->GetWorldTimerManager().SetTimer(this->timer, this, &ATCP::socketListener, 0.01f, true);
		}
	}
}

void ATCP::socketListener()
{
	if (!this->listenerSocket) return;

	TArray<uint8> receivedData;
	uint32 size;

	while (this->connectionSocket->HasPendingData(size))
	{
		receivedData.Init(FMath::Min(size, 65507u), size);

		int32 read = 0;
		this->connectionSocket->Recv(receivedData.GetData(), receivedData.Num(), read);
	}

	if (receivedData.Num() <= 0)
	{
		return;
	}

	receivedData.Add(0);
	FString receivedUE4String = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(receivedData.GetData())));
	this->OnMessageReceived.Broadcast(receivedUE4String);
}
