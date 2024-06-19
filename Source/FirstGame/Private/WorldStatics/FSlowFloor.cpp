// FSlowFloor.cpp

#include "WorldStatics/FSlowFloor.h"
#include "Characters/FRPGCharacter.h"
#include "Components/FBuffComponent.h"

AFSlowFloor::AFSlowFloor()
{
 	PrimaryActorTick.bCanEverTick = false;
}

void AFSlowFloor::CheckOverlap()
{
	// 플레이어 캐릭터 장판 접촉 확인
	AFRPGCharacter* OwningPlayerCharacter =
		Cast<AFRPGCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (false == ::IsValid(OwningPlayerCharacter)) {
		return;
	}

	UFBuffComponent* BuffComponent = OwningPlayerCharacter->GetBuffComponent();
	if (false == ::IsValid(BuffComponent)) {
		return;
	}

	// 플레이어 캐릭터 버프 갱신
	if (true == IsOverlappingActor(OwningPlayerCharacter)) {
		// 이미 다른 버프가 존재 시 해당 버프 지우기
		if (BuffComponent->GetBuffName() != "Slow") {
			BuffComponent->SetBuffName("Slow");
			BuffComponent->SetCurrentStack(0);
		}
		BuffComponent->SetCurrentStack((BuffComponent->GetCurrentStack()) + 1);
	}
}

void AFSlowFloor::BeginPlay()
{
	Super::BeginPlay();

	// 1초에 한 번씩 디버프 장판 접촉 확인
	FTimerHandle overlapTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(overlapTimerHandle, this, &ThisClass::CheckOverlap, 1.0f, true);
}

