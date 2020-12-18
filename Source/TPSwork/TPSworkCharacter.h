// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

//#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
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

	/** ���Ը��� */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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

	/** ��ҵ��������ֵ��������ҵ��������ֵ��Ҳ�ǳ���ʱ������ֵ��*/
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth;


	/** ��ҵĵ�ǰ����ֵ������0�ͱ�ʾ������*/
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth;


	/** RepNotify������ͬ���Ե�ǰ����ֵ�����ĸ��ġ�*/
	UFUNCTION()
	void OnRep_CurrentHealth();

	/** ��ӦҪ���µ�����ֵ���޸ĺ������ڷ������ϵ��ã����ڿͻ����ϵ�������ӦRepNotify*/
	void OnHealthUpdate();



	/** ���֮����ӳ٣���λΪ�롣���ڿ��Ʋ��Է����������ٶȣ����ɷ�ֹ������������������½�SpawnProjectileֱ�Ӱ������롣*/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float FireRate;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float ReBirthRate;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float FightRate;

	/** ���ڽ�����������ĺ�����һ��������δ��룬��ҿ��ٴ�ʹ��StartFire��*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopFight();

	/** ��������Ͷ����ķ�����������*/
	UFUNCTION(Server, Reliable)
	void HandleFire();



	/** ��ʱ������������ṩ���ɼ��ʱ���ڵ������ӳ١�*/
	FTimerHandle FiringTimer;

	/** ��ʱ������������ṩ���ɼ��ʱ���ڵ������ӳ١�*/
	FTimerHandle ReBirthTimer;

	/** ��ʱ������������ṩ���ɼ��ʱ���ڵ������ӳ١�*/
	FTimerHandle FightTimer;

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
	void Fight();


	UFUNCTION()
	void FireWithWeapon0();

	UFUNCTION(BlueprintImplementableEvent)
	void FireWithWeapon1();

	UFUNCTION(BlueprintCallable)
    void EnemyFire();

	UFUNCTION()
	void FireWithWeapon2();

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void UpdateUI();


	// �������λ�õ�ǹ��ƫ�ơ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	// ���ɵķ������ࡣ
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectBomb> ProjectBombClass;




	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    int32 Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 DeadCount;

	UPROPERTY(EditAnywhere, Replicated,BlueprintReadWrite, Category = Gameplay)
	int32 KillCount;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = Gameplay)
	float CGameOverTime;


	UFUNCTION()
	void AddKillCount ();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float Health;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = Gameplay)
	int32 Score;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite,Category = Gameplay)
    bool IsFire;


	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = Gameplay)
	bool IsDead;

	UFUNCTION(Server, Reliable)
	void SetDead();

	UFUNCTION(Server, Reliable)
	void SetFight();

	UFUNCTION(Server, Reliable,BlueprintCallable)
	void SetTime();


	UFUNCTION(BlueprintCallable)
	void ReBirth();


	UPROPERTY(EditAnywhere, Replicated,  BlueprintReadWrite,Category = Gameplay)
	bool CanMove;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = Gameplay)
	bool IsFight;


	UPROPERTY(EditAnywhere, Replicated,BlueprintReadWrite, Category = Gameplay)
	int32 WeaponType;


	/** �������ֵ��ȡֵ������*/
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	/** ��ǰ����ֵ��ȡֵ������*/
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	/** ��ǰ����ֵ�Ĵ�ֵ����������ֵ�ķ�Χ�޶���0��MaxHealth֮�䣬������OnHealthUpdate�����ڷ������ϵ��á�*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float healthValue);

	/** �����˺����¼�����APawn���ǡ�*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

};

