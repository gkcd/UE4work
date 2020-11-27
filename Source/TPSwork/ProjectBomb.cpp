// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBomb.h"
#include "TPSworkGameMode.h"
#include "Engine/Engine.h"
#include "TPSworkCharacter.h"
#include "Kismet/GamePlayStatics.h"

// Sets default values
AProjectBomb::AProjectBomb()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // 使用球体代表简单碰撞。
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));


    CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));

    CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectBomb::OnHit);


    // this->OnActorHit.AddDynamic(this, &AProjectBomb::OnActorHit);

     // 设置球体的碰撞半径。
    CollisionComponent->InitSphereRadius(15.0f);
    // 将碰撞组件设为根组件。
    RootComponent = CollisionComponent;

    // 使用此组件驱动此发射物的运动。
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
    ProjectileMovementComponent->InitialSpeed = 1500.0f;
    ProjectileMovementComponent->MaxSpeed = 1500.0f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = true;
    ProjectileMovementComponent->Bounciness = 0.3f;

    // 3 秒后消亡。
    InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AProjectBomb::BeginPlay()
{
    Super::BeginPlay();

    // CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectBomb::OnHit);

}

// Called every frame
void AProjectBomb::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}


// 在发射方向上设置发射物初速度的函数。
void AProjectBomb::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}


// 发射物命中物体时调用的函数。
void AProjectBomb::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
}

