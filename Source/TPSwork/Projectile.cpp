// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "TPSworkGameMode.h"
#include "Engine/Engine.h"
#include "TPSworkCharacter.h"
#include "Kismet/GamePlayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // ʹ������������ײ��
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));


    CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));

    CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);


    // this->OnActorHit.AddDynamic(this, &AProjectile::OnActorHit);

     // �����������ײ�뾶��
    CollisionComponent->InitSphereRadius(15.0f);
    // ����ײ�����Ϊ�������
    RootComponent = CollisionComponent;

    // ʹ�ô���������˷�������˶���
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
    ProjectileMovementComponent->InitialSpeed = 5000.0f;
    ProjectileMovementComponent->MaxSpeed = 5000.0f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = true;
    ProjectileMovementComponent->Bounciness = 0.3f;

    // 3 ���������
    InitialLifeSpan = 3.0f;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
    Super::BeginPlay();

    // CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}


// �ڷ��䷽�������÷�������ٶȵĺ�����
void AProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}


// ��������������ʱ���õĺ�����
void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{

    

      ATPSworkCharacter* MyCharacter = Cast<ATPSworkCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

    if (!MyCharacter)
    {

        GEngine->AddOnScreenDebugMessage(0, 10.0f, FColor::Yellow, "MyCharacter is null");
       return;
    }



    else 
    {
        if (OtherActor != this && OtherActor->ActorHasTag("CubeTarget"))
        {

            // ��ʾ������Ϣ���롣 
        // -1"��"ֵ���׸�������˵������������»�ˢ�´���Ϣ��
            if (Hit.Location.X >= 592 && Hit.Location.X <= 608 && Hit.Location.Z <= 380 && Hit.Location.Z >= 260)
                AddScore = 10;
            else if (Hit.Location.X >= 582 && Hit.Location.X <= 618 && Hit.Location.Z <= 390 && Hit.Location.Z >= 250)
                AddScore = 9;
            else if (Hit.Location.X >= 572 && Hit.Location.X <= 628 && Hit.Location.Z <= 400 && Hit.Location.Z >= 240)
                AddScore = 8;
            else if (Hit.Location.X >= 562 && Hit.Location.X <= 638 && Hit.Location.Z <= 410 && Hit.Location.Z >= 230)
                AddScore = 7;
            else if (Hit.Location.X >= 532 && Hit.Location.X <= 668 && Hit.Location.Z <= 440 && Hit.Location.Z >= 200)
                AddScore = 6;
            MyCharacter->Score += AddScore;
          //  GEngine->AddOnScreenDebugMessage(0, 99999.0f, FColor::Yellow, "Get Score " + FString::FromInt(AddScore) + "! Your total score is " + FString::FromInt(MyCharacter->Score) + ".");
            MyCharacter->UpdateScore();
            return;
        }

        else if (OtherActor != this && OtherActor->ActorHasTag("CubeTarget1"))
        {

            // ��ʾ������Ϣ���롣 
        // -1"��"ֵ���׸�������˵������������»�ˢ�´���Ϣ��
            if (Hit.Location.X >= 592 - 700 && Hit.Location.X <= 608 - 700 && Hit.Location.Z <= 380 && Hit.Location.Z >= 260)
                AddScore = 10;
            else if (Hit.Location.X >= 582 - 700 && Hit.Location.X <= 618 - 700 && Hit.Location.Z <= 390 && Hit.Location.Z >= 250)
                AddScore = 9;
            else if (Hit.Location.X >= 572 - 700 && Hit.Location.X <= 628 - 700 && Hit.Location.Z <= 400 && Hit.Location.Z >= 240)
                AddScore = 8;
            else if (Hit.Location.X >= 562 - 700 && Hit.Location.X <= 638 - 700 && Hit.Location.Z <= 410 && Hit.Location.Z >= 230)
                AddScore = 7;
            else if (Hit.Location.X >= 532 - 700 && Hit.Location.X <= 668 - 700 && Hit.Location.Z <= 440 && Hit.Location.Z >= 200)
                AddScore = 6;
            MyCharacter->Score += AddScore;
          //  GEngine->AddOnScreenDebugMessage(0, 99999.0f, FColor::Yellow, "Get Score " + FString::FromInt(AddScore) + "! Your total score is " + FString::FromInt(MyCharacter->Score) + ".");
            MyCharacter->UpdateScore();
            return;

        }

        else if (OtherActor != this && OtherActor->ActorHasTag("CubeTarget2"))
        {

            // ��ʾ������Ϣ���롣 
        // -1"��"ֵ���׸�������˵������������»�ˢ�´���Ϣ��
            if (Hit.Location.X >= 592 - 700 * 2 && Hit.Location.X <= 608 - 700 * 2 && Hit.Location.Z <= 380 && Hit.Location.Z >= 260)
                AddScore = 10;
            else if (Hit.Location.X >= 582 - 700 * 2 && Hit.Location.X <= 618 - 700 * 2 && Hit.Location.Z <= 390 && Hit.Location.Z >= 250)
                AddScore = 9;
            else if (Hit.Location.X >= 572 - 700 * 2 && Hit.Location.X <= 628 - 700 * 2 && Hit.Location.Z <= 400 && Hit.Location.Z >= 240)
                AddScore = 8;
            else if (Hit.Location.X >= 562 - 700 * 2 && Hit.Location.X <= 638 - 700 * 2 && Hit.Location.Z <= 410 && Hit.Location.Z >= 230)
                AddScore = 7;
            else if (Hit.Location.X >= 532 - 700 * 2 && Hit.Location.X <= 668 - 700 * 2 && Hit.Location.Z <= 440 && Hit.Location.Z >= 200)
                AddScore = 6;
            MyCharacter->Score += AddScore;
           // GEngine->AddOnScreenDebugMessage(0, 99999.0f, FColor::Yellow, "Get Score " + FString::FromInt(AddScore) + "! Your total score is " + FString::FromInt(MyCharacter->Score) + ".");
            MyCharacter->UpdateScore();
            return;

        }

        else if (OtherActor != this && OtherActor->ActorHasTag("CubeTarget3"))
        {

            // ��ʾ������Ϣ���롣 
        // -1"��"ֵ���׸�������˵������������»�ˢ�´���Ϣ��
            if (Hit.Location.X >= 592 - 700 * 3 && Hit.Location.X <= 608 - 700 * 3 && Hit.Location.Z <= 380 && Hit.Location.Z >= 260)
                AddScore = 10;
            else if (Hit.Location.X >= 582 - 700 * 3 && Hit.Location.X <= 618 - 700 * 3 && Hit.Location.Z <= 390 && Hit.Location.Z >= 250)
                AddScore = 9;
            else if (Hit.Location.X >= 572 - 700 * 3 && Hit.Location.X <= 628 - 700 * 3 && Hit.Location.Z <= 400 && Hit.Location.Z >= 240)
                AddScore = 8;
            else if (Hit.Location.X >= 562 - 700 * 3 && Hit.Location.X <= 638 - 700 * 3 && Hit.Location.Z <= 410 && Hit.Location.Z >= 230)
                AddScore = 7;
            else if (Hit.Location.X >= 532 - 700 * 3 && Hit.Location.X <= 668 - 700 * 3 && Hit.Location.Z <= 440 && Hit.Location.Z >= 200)
                AddScore = 6;

            MyCharacter->Score += AddScore;
         //   GEngine->AddOnScreenDebugMessage(0, 99999.0f, FColor::Yellow, "Get Score " + FString::FromInt(AddScore) + "! Your total score is " + FString::FromInt(MyCharacter->Score) + ".");
            MyCharacter->UpdateScore();
            return;

        }
        
    }
}

