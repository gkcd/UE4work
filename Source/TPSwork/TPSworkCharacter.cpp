// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSworkCharacter.h"
#include "Projectile.h"
#include "ProjectBomb.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "TPSworkGameMode.h"
#include "Kismet/GamePlayStatics.h"


//////////////////////////////////////////////////////////////////////////
// ATPSworkCharacter

ATPSworkCharacter::ATPSworkCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 1000.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	Ammo = MaxAmmo=20;

	IsFire = false;
	IsFight = false;
	CanMove = true;

	//��ʼ���������ֵ
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

	//��ʼ������
	FireRate = 0.6f;

	//��ʼ������
	ReBirthRate = 2.0f;

	//��ʼ������
	FightRate = 0.3f;

	Score = 0;
	WeaponType = 0;

	DeadCount = 0;
	KillCount = 0;
	
	SetTime();

}

//////////////////////////////////////////////////////////////////////////
// Input

void ATPSworkCharacter::SetTime_Implementation()
{
	ATPSworkGameMode* MyGamemode = Cast<ATPSworkGameMode>(UGameplayStatics::GetGameMode(this));

	if (MyGamemode)
	{
		CGameOverTime = MyGamemode->GameOverTime;
	}
	//else
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Gamemode NULL");
	//}
}

void ATPSworkCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATPSworkCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPSworkCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATPSworkCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATPSworkCharacter::LookUpAtRate);

	// handle touch devices
	//PlayerInputComponent->BindTouch(IE_Pressed, this, &ATPSworkCharacter::TouchStarted);
	//PlayerInputComponent->BindTouch(IE_Released, this, &ATPSworkCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ATPSworkCharacter::OnResetVR);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATPSworkCharacter::Fire);

	PlayerInputComponent->BindAction("Fight", IE_Pressed, this, &ATPSworkCharacter::Fight);

}


void ATPSworkCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ATPSworkCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ATPSworkCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ATPSworkCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATPSworkCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATPSworkCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f)&&CanMove)
	{

		// find out which way is forward
	//	const FRotator Rotation = Controller->GetControlRotation();
		const FRotator Rotation = GetActorRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATPSworkCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && CanMove)
	{
		// find out which way is right
		//const FRotator Rotation = Controller->GetControlRotation();
		const FRotator Rotation = GetActorRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


void ATPSworkCharacter::Fight()
{

	IsFight = true;
	SetFight();
//	UWorld* World = GetWorld();
//	World->GetTimerManager().SetTimer(FightTimer, this, &ATPSworkCharacter::StopFight, FightRate, false);
	
}

void ATPSworkCharacter::Fire()
{
	// ���Է������塣
	if (ProjectileClass)
	{
		if (GetVelocity().IsZero()) 
		{
			if (Ammo > 0)
			{
				if (!IsFire && !IsFight)
				{
					IsFire = true;
					CanMove = false;
					HandleFire();
					UWorld* World = GetWorld();
					World->GetTimerManager().SetTimer(FiringTimer, this, &ATPSworkCharacter::StopFire, FireRate, false);
					
					
					Ammo--;
					UpdateUI();

				}
			}
		}
	}
}



void ATPSworkCharacter::FireWithWeapon0()
{
	// find out which way is forward
	//const FRotator Rotation = Controller->GetControlRotation();
	//const FRotator YawRotation(0, Rotation.Yaw, 0);


//	SetActorRotation(YawRotation);

//	MoveForward(0.1);
	// ��ȡ������任��

	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	// �� MuzzleOffset ��������ռ�任������ռ䡣
	FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
	FRotator MuzzleRotation = CameraRotation;
	// ��׼����΢��̧��
	MuzzleRotation.Pitch += 5.0f;
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this->GetInstigator();
		// ��ǹ�ڴ����ɷ����

		AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		// ���÷�����ĳ�ʼ�����
		FVector LaunchDirection = MuzzleRotation.Vector();
		//	Projectile->MyCharacter = this;
		Projectile->FireInDirection(LaunchDirection);



	}

}



void ATPSworkCharacter::FireWithWeapon2()
{
	// find out which way is forward
	//const FRotator Rotation = Controller->GetControlRotation();
	//const FRotator YawRotation(0, Rotation.Yaw, 0);


//	SetActorRotation(YawRotation);

//	MoveForward(0.1);
	// ��ȡ������任��

	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	// �� MuzzleOffset ��������ռ�任������ռ䡣
	FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
	FRotator MuzzleRotation = CameraRotation;
	// ��׼����΢��̧��
	MuzzleRotation.Pitch += 15.0f;
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this->GetInstigator();
		// ��ǹ�ڴ����ɷ����

		AProjectBomb* ProjectBomb = World->SpawnActor<AProjectBomb>(ProjectBombClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		// ���÷�����ĳ�ʼ�����
		FVector LaunchDirection = MuzzleRotation.Vector();
		//	Projectile->MyCharacter = this;
		ProjectBomb->FireInDirection(LaunchDirection);



	}

}



//////////////////////////////////////////////////////////////////////////
// ���Ƶ�����

void ATPSworkCharacter::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//���Ƶ�ǰ����ֵ��
	DOREPLIFETIME(ATPSworkCharacter, IsFight);
	DOREPLIFETIME(ATPSworkCharacter, CurrentHealth);
	DOREPLIFETIME(ATPSworkCharacter, IsFire);
	DOREPLIFETIME(ATPSworkCharacter, IsDead);
	DOREPLIFETIME(ATPSworkCharacter, CanMove);
//	DOREPLIFETIME(ATPSworkCharacter, Score);
	DOREPLIFETIME(ATPSworkCharacter, KillCount);
	DOREPLIFETIME(ATPSworkCharacter, CGameOverTime);
}

void ATPSworkCharacter::AddKillCount()
{
	KillCount++;
}

void ATPSworkCharacter::OnHealthUpdate()
{

	////�ͻ����ض��Ĺ���
	//if (IsLocallyControlled())
	//{
	//	//FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
	//	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
	//	
	//	if (CurrentHealth <= 0)
	//	{
	//		//FString deathMessage = FString::Printf(TEXT("You have been killed."));
	//		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
	//		//IsDead = true;
	//		
	//	}
	//}

	////�������ض��Ĺ���
	//if (GetLocalRole() == ROLE_Authority)
	//{
	//	//FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
	//	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
	//	//if (CurrentHealth <= 0)
	//	//{
	//	//	SetDead();
	//	//}
	//}

	//�����л����϶�ִ�еĺ����� 
	/*
		���κ����˺������������������⹦�ܶ�Ӧ�������*/

	
	if (CurrentHealth <= 0)
	{
		IsDead = true;
		SetDead();
		DeadCount++;
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(ReBirthTimer, this, &ATPSworkCharacter::ReBirth, ReBirthRate, false);
		
	}
	UpdateUI();
}

void ATPSworkCharacter::SetDead_Implementation()
{
	IsDead = true;
	UpdateUI();
}

void ATPSworkCharacter::SetFight_Implementation()
{
	IsFight = true;
}

void ATPSworkCharacter::ReBirth()
{

	CurrentHealth = 100.0f;
	UpdateUI();
	IsDead = false;
}

void ATPSworkCharacter::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

void ATPSworkCharacter::SetCurrentHealth(float healthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
		OnHealthUpdate();
	}
}

float ATPSworkCharacter::TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damageApplied = CurrentHealth - DamageTaken;
	SetCurrentHealth(damageApplied);
	return damageApplied;
}

void ATPSworkCharacter::StopFire()
{
	IsFire = false;
	CanMove = true;
}

void ATPSworkCharacter::StopFight()
{
	IsFight = false;
}

void ATPSworkCharacter::HandleFire_Implementation()
{
	IsFire = true;
	CanMove = false;
	switch (WeaponType)
	{
	case 0:
		FireWithWeapon0();
		break;
	case 1:
		FireWithWeapon1();
		break;
	case 2:
		FireWithWeapon2();
		break;
	}
}