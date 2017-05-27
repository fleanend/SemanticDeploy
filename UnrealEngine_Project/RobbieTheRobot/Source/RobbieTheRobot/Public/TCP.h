// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Networking.h"

#include "GameFramework/Actor.h"
#include "TCP.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageReceived, FString, Message);

UCLASS(Blueprintable, BlueprintType)
class ROBBIETHEROBOT_API ATCP : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATCP();
	FSocket* listenerSocket;
	FSocket* connectionSocket;
	FTimerHandle timer = FTimerHandle();
	FIPv4Endpoint remoteAddressForConnection;

	UFUNCTION(BlueprintCallable, Category = "Networking")
	bool CreateSocket(const FString& socketName, const FString& address, const int32 port, int32 receiveBufferSize);

	UPROPERTY(BlueprintAssignable, Category = "TCP|Event")
	FOnMessageReceived OnMessageReceived;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool formatIP4ToNumber(const FString& ip, uint8(&out)[4]);
	void connectionListener();
	void socketListener();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
