// FAnimInstance.cpp

#include "Animations/FAnimInstance.h"
#include "Characters/FRPGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UFAnimInstance::UFAnimInstance()
{
}

void UFAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    // 속성값 초기화
    CurrentSpeed = 0.f;
    Velocity = FVector::ZeroVector;
    bIsFalling = false;
    bIsCrouching = false;
}

void UFAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    // TryGetPawnOwner(): AnimInstance의 주인, 즉 Pawn Owner 가져오기 시도
    // 실행 순서: 입력 로직 - 게임 로직 - 애니메이션 로직
    // 이 함수는 애니메이션 로직이기 때문에, 게임 로직에서 Pawn Owner가 죽었을 경우 가져오기 실패
    AFCharacter* OwnerCharacter = Cast<AFCharacter>(TryGetPawnOwner());
    if (true == ::IsValid(OwnerCharacter)) {
        UCharacterMovementComponent* CharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
        if (true == ::IsValid(CharacterMovementComponent)) {
            Velocity = CharacterMovementComponent->GetLastUpdateVelocity();
            // Velocity().Size(): 속도에서 속력 구하기
            CurrentSpeed = Velocity.Size();
            bIsFalling = CharacterMovementComponent->IsFalling();
            bIsCrouching = CharacterMovementComponent->IsCrouching();
        }
    }
}

void UFAnimInstance::PlayAttackAnimMontage()
{
    // Animation Montage 등록 확인
    if (true == ::IsValid(AttackAnimMontage)) {
        // Montage_IsPlaying(): Animation Montage가 재생중인지 확인
        if (false == Montage_IsPlaying(AttackAnimMontage)) {
            // Montage_Play(): Animation Montage를 재생한다
            Montage_Play(AttackAnimMontage);
        }
    }
}
