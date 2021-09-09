// Fill out your copyright notice in the Description page of Project Settings.


#include "MapLoader.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMapLoader::AMapLoader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneRoot;

	GatesMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gates Mesh"));
	GatesMesh->SetupAttachment(RootComponent);

	ActivatedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Activated Lights"));
	ActivatedLight->SetupAttachment(RootComponent);

	DeactivatedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Deactivated Lights"));
	DeactivatedLight->SetupAttachment(RootComponent);

	TriggerCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Collider"));
	TriggerCollider->SetupAttachment(RootComponent);

	TriggerCollider->OnComponentBeginOverlap.AddDynamic(this, &AMapLoader::OnTriggerOverlapBegin);

	SetActiveLights();

}

// Called when the game starts or when spawned
void AMapLoader::BeginPlay()
{
	Super::BeginPlay();

	SetActiveLights();
	
}

void AMapLoader::SetIsActivated(bool NewIsActivated)
{
	bIsActivated = NewIsActivated;

	SetActiveLights();

}

void AMapLoader::SetActiveLights()
{
	ActivatedLight->SetHiddenInGame(!bIsActivated);
	DeactivatedLight->SetHiddenInGame(bIsActivated);

}

void AMapLoader::OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
									   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsActivated)
	{
		return;
	}

	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (OtherActor == PlayerPawn)
	{
		UGameplayStatics::OpenLevel(GetWorld(), LoadLevelName);
	}

}