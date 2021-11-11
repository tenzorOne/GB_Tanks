// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "TankPlayerController.h"
#include "TankAIController.h"
#include "Math/Vector.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include <Engine/TargetPoint.h>
#include <Components/WidgetComponent.h>
#include <Materials/MaterialInstanceDynamic.h>
#include <Kismet/KismetMaterialLibrary.h>
#include "InventoryManagerComponent.h"
#include <Materials/MaterialParameterCollection.h>
#include <UObject/ReflectedTypeAccessors.h>
#include <Kismet/KismetStringLibrary.h>


ATankPawn::ATankPawn()
{	
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	WorldWidget = CreateDefaultSubobject<UWidgetComponent>("HPBar");
	WorldWidget->SetWidgetClass(UWidgetComponent::StaticClass());
	WorldWidget->SetupAttachment(RootComponent);
	WorldWidget->SetUsingAbsoluteRotation(true);

}


void ATankPawn::EquipItem(int32 SlotIndex, FName ItemID)
{
	Super::EquipItem(SlotIndex, ItemID);

	if (UMaterialInstanceDynamic* MainMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, DefaultMaterial, FName()))
	{
		TArray<UActorComponent*> ActorComponents;
		GetComponents(UInventoryManagerComponent::StaticClass(), ActorComponents);
		if (ActorComponents.Num() != 0)
		{
			if (UInventoryManagerComponent* InventoryManagerComponent = Cast<UInventoryManagerComponent>(ActorComponents[0]))
			{
				EItemRarity ItemRarity = InventoryManagerComponent->GetItemRarity(ItemID);

				if (ItemRarity != EItemRarity::IR_None && MaterialParameters)
				{
					FName SlotName = StaticEnum<EEquipSlot>()->GetNameByValue(SlotIndex == 99 ? 0 : SlotIndex);
					FString StringSlotName = SlotName.ToString();
					StringSlotName = StringSlotName.RightChop(12);

					switch (ItemRarity)
					{
					case EItemRarity::IR_None:
						break;
					case EItemRarity::IR_Common:
						MainMaterialInstance->SetVectorParameterValue(FName(StringSlotName), UKismetMaterialLibrary::GetVectorParameterValue(this, MaterialParameters, "Common"));
						MainMaterialInstance->SetScalarParameterValue(FName(StringSlotName), 1.f);
						DefaultMaterial->OverrideVectorParameterDefault(FName(StringSlotName), UKismetMaterialLibrary::GetVectorParameterValue(this, MaterialParameters, "Common"), true, ERHIFeatureLevel::SM5);
						DefaultMaterial->OverrideScalarParameterDefault(FName(StringSlotName), 1.f, true, ERHIFeatureLevel::SM5);
						break;
					case EItemRarity::IR_Uncommon:
						MainMaterialInstance->SetVectorParameterValue(FName(StringSlotName), UKismetMaterialLibrary::GetVectorParameterValue(this, MaterialParameters, "Uncommon"));
						MainMaterialInstance->SetScalarParameterValue(FName(StringSlotName), 1.f);
						DefaultMaterial->OverrideVectorParameterDefault(FName(StringSlotName), UKismetMaterialLibrary::GetVectorParameterValue(this, MaterialParameters, "Uncommon"), true, ERHIFeatureLevel::SM5);
						DefaultMaterial->OverrideScalarParameterDefault(FName(StringSlotName), 1.f, true, ERHIFeatureLevel::SM5);
						break;
					case EItemRarity::IR_Rare:
						MainMaterialInstance->SetVectorParameterValue(FName(StringSlotName), UKismetMaterialLibrary::GetVectorParameterValue(this, MaterialParameters, "Rare"));
						MainMaterialInstance->SetScalarParameterValue(FName(StringSlotName), 1.f);
						DefaultMaterial->OverrideVectorParameterDefault(FName(StringSlotName), UKismetMaterialLibrary::GetVectorParameterValue(this, MaterialParameters, "Rare"), true, ERHIFeatureLevel::SM5);
						DefaultMaterial->OverrideScalarParameterDefault(FName(StringSlotName), 1.f, true, ERHIFeatureLevel::SM5);
						break;
					case EItemRarity::IR_Epic:
						MainMaterialInstance->SetVectorParameterValue(FName(StringSlotName), UKismetMaterialLibrary::GetVectorParameterValue(this, MaterialParameters, "Epic"));
						MainMaterialInstance->SetScalarParameterValue(FName(StringSlotName), 1.f);
						DefaultMaterial->OverrideVectorParameterDefault(FName(StringSlotName), UKismetMaterialLibrary::GetVectorParameterValue(this, MaterialParameters, "Epic"), true, ERHIFeatureLevel::SM5);
						DefaultMaterial->OverrideScalarParameterDefault(FName(StringSlotName), 1.f, true, ERHIFeatureLevel::SM5);
						break;
					case EItemRarity::IR_Legendary:
						MainMaterialInstance->SetVectorParameterValue(FName(StringSlotName), UKismetMaterialLibrary::GetVectorParameterValue(this, MaterialParameters, "Legendary"));
						MainMaterialInstance->SetScalarParameterValue(FName(StringSlotName), 1.f);
						DefaultMaterial->OverrideVectorParameterDefault(FName(StringSlotName), UKismetMaterialLibrary::GetVectorParameterValue(this, MaterialParameters, "Legendary"), true, ERHIFeatureLevel::SM5);
						DefaultMaterial->OverrideScalarParameterDefault(FName(StringSlotName), 1.f, true, ERHIFeatureLevel::SM5);
						break;
					}

					BodyMesh->SetMaterial(0, MainMaterialInstance);
					TurretMesh->SetMaterial(0, MainMaterialInstance);

				}
			}
		}
	}

}

void ATankPawn::UnequipItem(int32 SlotIndex, FName ItemID)
{
	Super::UnequipItem(SlotIndex, ItemID);

	FName SlotName = StaticEnum<EEquipSlot>()->GetNameByValue(SlotIndex == 99 ? 0 : SlotIndex);
	FString StringSlotName = SlotName.ToString();
	StringSlotName = StringSlotName.RightChop(12);

	if (UMaterialInstanceDynamic* MainMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, DefaultMaterial, FName()))
	{
		MainMaterialInstance->SetVectorParameterValue(FName(StringSlotName), FLinearColor::Black);
		MainMaterialInstance->SetScalarParameterValue(FName(StringSlotName), 0.f);
		DefaultMaterial->OverrideVectorParameterDefault(FName(StringSlotName), FLinearColor::Black, false, ERHIFeatureLevel::SM5);
		DefaultMaterial->OverrideScalarParameterDefault(FName(StringSlotName), 0.f, false, ERHIFeatureLevel::SM5);

		BodyMesh->SetMaterial(0, MainMaterialInstance);
		TurretMesh->SetMaterial(0, MainMaterialInstance);
	}

}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	
	TankController = Cast<ATankPlayerController>(GetController());
	TankAIController = Cast<ATankAIController>(GetController());

	FirstCannon = Cannon;
	if (TankController)
	{
		SetupAnotherCannon(SecondCannonClass, false);
	}
	
	TempStopFactor = GetDefaultStopFactor();

}



void ATankPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (DefaultMaterial)
	{
		DefaultMaterial->OverrideVectorParameterDefault(FName("EQ_CannonSlot"), FLinearColor::Black, false, ERHIFeatureLevel::SM5);
		DefaultMaterial->OverrideScalarParameterDefault(FName("EQ_CannonSlot"), 0.f, false, ERHIFeatureLevel::SM5);
		DefaultMaterial->OverrideVectorParameterDefault(FName("EQ_ArmorSlot"), FLinearColor::Black, false, ERHIFeatureLevel::SM5);
		DefaultMaterial->OverrideScalarParameterDefault(FName("EQ_ArmorSlot"), 0.f, false, ERHIFeatureLevel::SM5);
		DefaultMaterial->OverrideVectorParameterDefault(FName("EQ_TowerSlot"), FLinearColor::Black, false, ERHIFeatureLevel::SM5);
		DefaultMaterial->OverrideScalarParameterDefault(FName("EQ_TowerSlot"), 0.f, false, ERHIFeatureLevel::SM5);
		DefaultMaterial->OverrideVectorParameterDefault(FName("EQ_EngineSlot"), FLinearColor::Black, false, ERHIFeatureLevel::SM5);
		DefaultMaterial->OverrideScalarParameterDefault(FName("EQ_EngineSlot"), 0.f, false, ERHIFeatureLevel::SM5);
	}

	Super::EndPlay(EndPlayReason);

}

TArray<FVector> ATankPawn::GetPatrollingPoints()
{
	TArray<FVector> Points;
	for (ATargetPoint* Point : PatrollingPoints)
	{
		Points.Add(Point->GetActorLocation());
	}

	return Points;

}

void ATankPawn::SetPatrollingPoints(TArray<ATargetPoint*> NewPatrollingPoints)
{
	PatrollingPoints = NewPatrollingPoints;

}

void ATankPawn::SetupAnotherCannon(TSubclassOf<ACannon> CannonClassToSetup, bool bPickupNewCannon)
{
	if (Cannon)
	{		
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;

		ACannon* TempCannon = GetWorld()->SpawnActor<ACannon>(CannonClassToSetup, SpawnParameters);
		TempCannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		TempCannon->SetActorHiddenInGame(!bPickupNewCannon ? true : false);
		TempCannon->SetActorEnableCollision(!bPickupNewCannon ? false : true);

		if (bPickupNewCannon)
		{
			Cannon->Destroy();
			if (Cannon == FirstCannon)
			{
				Cannon = FirstCannon = TempCannon;
			}
			else
			{
				Cannon = SecondCannon = TempCannon;
			}
		}
		else
		{
			SecondCannon = TempCannon;
		}
	}

}

void ATankPawn::SwitchCannon()
{
	if (Cannon)
	{
		if (!bSwitchCannon)
		{
			Cannon->SetActorHiddenInGame(true);
			Cannon->SetActorEnableCollision(false);
			Cannon->SetActorLocation({ 0.f, 0.f, 0.f });
			Cannon = SecondCannon;
			Cannon->SetActorHiddenInGame(false);
			Cannon->SetActorEnableCollision(true);
			Cannon->SetActorLocation(CannonSetupPoint->GetComponentLocation());
			bSwitchCannon = true;
		}
		else
		{
			Cannon->SetActorHiddenInGame(true);
			Cannon->SetActorEnableCollision(false);
			Cannon->SetActorLocation({ 0.f, 0.f, 0.f });
			Cannon = FirstCannon;
			Cannon->SetActorHiddenInGame(false);
			Cannon->SetActorEnableCollision(true);
			Cannon->SetActorLocation(CannonSetupPoint->GetComponentLocation());
			bSwitchCannon = false;
		}
	}
	else
	{
		return;
	}

}

void ATankPawn::Die()
{
	Cannon->Destroy();
	Cannon = FirstCannon = nullptr;

	Super::Die();

}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Tank Movement //
	FVector CurrentLocation = GetActorLocation();
	FVector PreviousLocation = PreviousLocation == CurrentLocation ? PreviousLocation : CurrentLocation;
	FVector ForwardVector = GetActorForwardVector();
	FVector MovePosition;
	
	if (TankController)
	{
		if (TargetForwardAxisValue == 0)
		{
			MovementInterp = MovementSmoothnes = FMath::FInterpTo(MovementSmoothnes, 0.f, DeltaTime, 6.f);

			AddActorWorldOffset(UKismetMathLibrary::Multiply_VectorFloat(UKismetMathLibrary::Multiply_VectorFloat(ForwardVector, LastForwardAxisValue),
					FMath::FInterpConstantTo(StopInertiaFactor = FMath::Clamp(StopInertiaFactor - 0.01f, 0.f, StopInertiaFactor), 0.f, DeltaTime, MovementMomentum)), true);
		}
		else
		{
			StopInertiaFactor = FMath::FInterpTo(StopInertiaFactor, TempStopFactor, DeltaTime, StopFactorInterpSpeed);
			LastForwardAxisValue = TankController->GetInputAxisValue("MoveForward");

			MovePosition = FMath::VInterpTo(PreviousLocation, CurrentLocation + (ForwardVector * TargetForwardAxisValue) * MoveSpeed, DeltaTime,
											MovementInterp = FMath::FInterpConstantTo(0.f, UKismetMathLibrary::SafeDivide(MoveSpeed, 100.f), DeltaTime,
											MovementSmoothnes = FMath::Clamp(MovementSmoothnes + MovementMomentum, 0.f, UKismetMathLibrary::SafeDivide(MoveSpeed, 100.f))));

			SetActorLocation(MovePosition, true);
		}
	}
	else if (TankAIController)
	{
		MovePosition = CurrentLocation + (ForwardVector * TargetForwardAxisValue) * MoveSpeed / 100;
		SetActorLocation(MovePosition, false);
	}
	//

	// Tank Rotation//
	if (TankController)
	{
		if (bUseBaseConstantRotationSmoothness)
		{
			CurrentRightAxisValue = FMath::FInterpConstantTo(CurrentRightAxisValue, TargetRightAxisValue, DeltaTime, RotationSmoothness);
		}
		else
		{
			CurrentRightAxisValue = FMath::FInterpTo(CurrentRightAxisValue, TargetRightAxisValue, DeltaTime, RotationSmoothness);
		}

		float YawRotation = CurrentRightAxisValue * RotationSpeed * DeltaTime;
		YawRotation += GetActorRotation().Yaw;
		SetActorRotation({ 0.f, YawRotation, 0.f });
	}
	else if (TankAIController)
	{
		float YawRotation = TargetRightAxisValue * RotationSpeed * DeltaTime;
		YawRotation += GetActorRotation().Yaw;
		SetActorRotation({ 0.f, YawRotation, 0.f });
	}
	//

	// TurretRotation //
	if (TankController)
	{
		FVector MousePos = TankController->GetMousePos();
		RotateTurretTo(MousePos);
	}
	//

}

FVector ATankPawn::GetTurretForwardVector()
{
	return TurretMesh->GetForwardVector();

}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATankPawn::MoveForward(float AxisValue)
{
	TargetForwardAxisValue = AxisValue;

}

void ATankPawn::RotateRight(float AxisValue)
{
	if (TargetForwardAxisValue != 0)
	{
		AxisValue = TargetForwardAxisValue >= 0 ? AxisValue : AxisValue * -1;
	}
	TargetRightAxisValue = AxisValue;

}
