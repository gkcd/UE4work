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

	/** 属性复制 */
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

	/** 玩家的最大生命值。这是玩家的最高生命值，也是出生时的生命值。*/
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth;


	/** 玩家的当前生命值。降到0就表示死亡。*/
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth;


	/** RepNotify，用于同步对当前生命值所做的更改。*/
	UFUNCTION()
	void OnRep_CurrentHealth();

	/** 响应要更新的生命值。修改后，立即在服务器上调用，并在客户端上调用以响应RepNotify*/
	void OnHealthUpdate();



	/** 射击之间的延迟，单位为秒。用于控制测试发射物的射击速度，还可防止服务器函数的溢出导致将SpawnProjectile直接绑定至输入。*/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float FireRate;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float ReBirthRate;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float FightRate;

	/** 用于结束武器射击的函数。一旦调用这段代码，玩家可再次使用StartFire。*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopFight();

	/** 用于生成投射物的服务器函数。*/
	UFUNCTION(Server, Reliable)
	void HandleFire();



	/** 定时器句柄，用于提供生成间隔时间内的射速延迟。*/
	FTimerHandle FiringTimer;

	/** 定时器句柄，用于提供生成间隔时间内的射速延迟。*/
	FTimerHandle ReBirthTimer;

	/** 定时器句柄，用于提供生成间隔时间内的射速延迟。*/
	FTimerHandle FightTimer;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	// 处理开火的函数。
	UFUNCTION()
	void Fire();

	// 处理开火的函数。
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


	// 从摄像机位置的枪口偏移。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	// 生成的发射物类。
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


	/** 最大生命值的取值函数。*/
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	/** 当前生命值的取值函数。*/
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	/** 当前生命值的存值函数。将此值的范围限定在0到MaxHealth之间，并调用OnHealthUpdate。仅在服务器上调用。*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float healthValue);

	/** 承受伤害的事件。从APawn覆盖。*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

};

