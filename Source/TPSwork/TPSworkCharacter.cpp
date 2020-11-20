// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSworkCharacter.h"
#include "Projectile.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"


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
}

//////////////////////////////////////////////////////////////////////////
// Input

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
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ATPSworkCharacter::EndFire);

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


void ATPSworkCharacter::Fire()
{
	// 尝试发射物体。
	if (ProjectileClass)
	{
		if (GetVelocity().IsZero()) 
		{
			if (Ammo > 0)
			{
				// find out which way is forward
				//const FRotator Rotation = Controller->GetControlRotation();
				//const FRotator YawRotation(0, Rotation.Yaw, 0);


			//	SetActorRotation(YawRotation);

			//	MoveForward(0.1);
				// 获取摄像机变换。
				FVector CameraLocation;
				FRotator CameraRotation;
				GetActorEyesViewPoint(CameraLocation, CameraRotation);

				// 将 MuzzleOffset 从摄像机空间变换到世界空间。
				FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
				FRotator MuzzleRotation = CameraRotation;
				// 将准星稍微上抬。
				MuzzleRotation.Pitch += 5.0f;
				UWorld* World = GetWorld();
				if (World)
				{
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = this;
					SpawnParams.Instigator = this->GetInstigator();
					// 在枪口处生成发射物。
					
					if (!IsFire&&!IsFight)
					{
						IsFire = true;
						CanMove = false;
						AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
						// 设置发射物的初始轨道。
						FVector LaunchDirection = MuzzleRotation.Vector();
						//	Projectile->MyCharacter = this;
						Projectile->FireInDirection(LaunchDirection);
						Ammo--;
						UpdateAmmo();
						
					}
				}
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, "You Have No Ammo.\n You can use ESC to open the menu.");
				GameOver();
			}
		}
	}
}


void ATPSworkCharacter::EndFire()
{

}