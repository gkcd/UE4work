// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSworkCharacter.generated.h"

UCLASS(config = Game)
class ATPSworkCharacter : public ACharacter
{
	GENERATED_BODY()

		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
public:
	ATPSworkCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	// ������ĺ�����
	UFUNCTION()
	void Fire();

	// ������ĺ�����
	UFUNCTION()
		void EndFire();


	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHealth();

	UFUNCTION()
	void FireWithWeapon0();

	UFUNCTION(BlueprintImplementableEvent)
	void FireWithWeapon1();

	UFUNCTION()
	void FireWithWeapon2();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAmmo();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateScore();




	UFUNCTION(BlueprintImplementableEvent)
	void GameOver();


	// �������λ�õ�ǹ��ƫ�ơ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	// ���ɵķ������ࡣ
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectBomb> ProjectBombClass;




	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 MaxAmmo=20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    int32 Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 Score=0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    bool IsFire;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool CanMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool IsFight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 WeaponType=0;
};

