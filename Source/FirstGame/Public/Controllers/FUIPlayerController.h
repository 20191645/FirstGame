// FUIPlayerController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FUIPlayerController.generated.h"

UCLASS()
class FIRSTGAME_API AFUIPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

private:
	// 위젯 객체를 만들기 위한 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AFUIPlayerController", meta = (AllowPrivateAccess))
	TSubclassOf<class UUserWidget> UIWidgetClass;

	// 위젯 객체
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AFUIPlayerController", meta = (AllowPrivateAccess))
	TObjectPtr<class UUserWidget> UIWidgetInstance;
};
