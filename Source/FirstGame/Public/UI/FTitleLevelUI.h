// FTitleLevelUI.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FTitleLevelUI.generated.h"

UCLASS()
class FIRSTGAME_API UFTitleLevelUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFTitleLevelUI(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void NativeConstruct() override;

private:
	// 'NewGame' 버튼 클릭 시 호출되는 함수
	UFUNCTION()
	void OnNewGameButtonClicked();
	// 'ExitGame' 버튼 클릭 시 호출되는 함수
	UFUNCTION()
	void OnExitGameButtonClicked();
	// 'SavedGame' 버튼 클릭 시 호출되는 함수
	UFUNCTION()
	void OnSavedGameButtonClicked();

private:
	// 새 게임 시작 버튼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFTitleWidget", meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> NewGameButton;
	// 게임 종료 버튼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFTitleWidget", meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> ExitGameButton;
	// 저장된 게임 불러오기 버튼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFTitleWidget", meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> SavedGameButton;
};
