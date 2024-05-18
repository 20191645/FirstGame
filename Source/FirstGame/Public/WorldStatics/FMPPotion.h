// FMPPotion.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FMPPotion.generated.h"

UCLASS()
class FIRSTGAME_API AFMPPotion : public AActor
{
	GENERATED_BODY()
	
public:
	AFMPPotion();

private:
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

    UFUNCTION()
    void OnEffectFinish(class UParticleSystemComponent* ParticleSystem);

private:
    UPROPERTY(VisibleAnywhere, Category = ASItemBox)
    TObjectPtr<class UBoxComponent> BoxComponent;

    UPROPERTY(EditAnywhere, Category = ASItemBox)
    TObjectPtr<class UStaticMeshComponent> StaticMeshComponent;

    UPROPERTY(EditAnywhere, Category = ASItemBox)
    TObjectPtr<class UParticleSystemComponent> ParticleSystemComponent;
};
