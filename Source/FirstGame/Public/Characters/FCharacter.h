// FCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FCharacter.generated.h"

UCLASS()
class FIRSTGAME_API AFCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFCharacter();

protected:
	// SpringArm, Camera Component는 Character Class에 포함되어 있지 않기 때문에 직접 추가
	// SpringArmComponent: 3인칭 시점 카메라 구도 설정 돕는 컴포넌트 - 카메라 봉 길이, 컴포넌트 회전 설정
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AFCharacter", meta = (AllowPrivateAccess))
	TObjectPtr<class USpringArmComponent> SpringArmComponent;

	// CameraComponenet: 가상 세계 모습을 플레이어 화면에 전송하는 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AFCharacter", meta = (AllowPrivateAccess))
	TObjectPtr<class UCameraComponent> CameraComponent;
};
