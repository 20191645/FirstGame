// FMenuUI.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMenuUI.generated.h"

UCLASS()
class FIRSTGAME_API UFMenuUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnResumeGameButtonClicked();
	UFUNCTION()
	void OnReturnTitleButtonClicked();
	UFUNCTION()
	void OnExitGameButtonClicked();

private:
	// 게임으로 돌아가기 버튼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFMenuWidget", meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> ResumeGameButton;
	// 타이틀 레벨 이동 버튼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFMenuWidget", meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> ReturnTitleButton;
	// 게임 종료 버튼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFMenuWidget", meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> ExitGameButton;
};
