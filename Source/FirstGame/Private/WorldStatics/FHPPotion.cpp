// FHPPotion.cpp

#include "WorldStatics/FHPPotion.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Characters/FRPGCharacter.h"
#include "Components/FStatComponent.h"

AFHPPotion::AFHPPotion()
{
 	PrimaryActorTick.bCanEverTick = false;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    SetRootComponent(BoxComponent);
    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFHPPotion::OnOverlapBegin);
    BoxComponent->SetCollisionProfileName(FName("FItem"));

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMeshComponent->SetupAttachment(GetRootComponent());
    StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

    ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
    ParticleSystemComponent->SetupAttachment(GetRootComponent());
    ParticleSystemComponent->SetAutoActivate(false);
}

void AFHPPotion::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
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

    // 캐릭터 HP 회복 (50)
    StatComponent->SetCurrentHP(StatComponent->GetCurrentHP() + 50.0f);
    
    // Particle 효과 및 액터 처리
    ParticleSystemComponent->Activate(true);
    StaticMeshComponent->SetHiddenInGame(true);
    SetActorEnableCollision(false);
    ParticleSystemComponent->OnSystemFinished.AddDynamic(this, &AFHPPotion::OnEffectFinish);
}

void AFHPPotion::OnEffectFinish(UParticleSystemComponent* ParticleSystem)
{
    // 액터 삭제
    Destroy();
}
