// FMPPotion.cpp

#include "WorldStatics/FMPPotion.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Characters/FRPGCharacter.h"
#include "Components/FStatComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AFMPPotion::AFMPPotion()
{
 	PrimaryActorTick.bCanEverTick = false;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    SetRootComponent(BoxComponent);
    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFMPPotion::OnOverlapBegin);
    BoxComponent->SetCollisionProfileName(FName("FItem"));

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMeshComponent->SetupAttachment(GetRootComponent());
    StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

    ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
    ParticleSystemComponent->SetupAttachment(GetRootComponent());
    ParticleSystemComponent->SetAutoActivate(false);
}

void AFMPPotion::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
    // 접촉한 액터가 플레이어 캐릭터인지 확인
    TArray<AActor*> OverlappingActors;
    GetOverlappingActors(OverlappingActors, TSubclassOf<AFRPGCharacter>());
    if (OverlappingActors.IsEmpty()) {
        return;
    }

    AFRPGCharacter* RPGC = Cast<AFRPGCharacter>(OverlappingActors[0]);
    if (false == ::IsValid(RPGC)) {
        return;
    }
    UFStatComponent* StatComponent = RPGC->GetStatComponent();
    if (false == ::IsValid(StatComponent)) {
        return;
    }

    // 캐릭터 MP 회복 (100)
    StatComponent->SetCurrentMP(StatComponent->GetCurrentMP() + 100.0f);

    // Particle 효과 및 액터 처리
    ParticleSystemComponent->Activate(true);
    StaticMeshComponent->SetHiddenInGame(true);
    SetActorEnableCollision(false);
    ParticleSystemComponent->OnSystemFinished.AddDynamic(this, &AFMPPotion::OnEffectFinish);
}

void AFMPPotion::OnEffectFinish(UParticleSystemComponent* ParticleSystem)
{
    // 액터 삭제
    Destroy();
}
