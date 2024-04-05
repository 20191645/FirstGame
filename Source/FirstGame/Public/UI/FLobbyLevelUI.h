// FLobbyLevelUI.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game/FPlayerStateSave.h"
#include "FLobbyLevelUI.generated.h"

UCLASS()
class FIRSTGAME_API UFLobbyLevelUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	// 'SubmitButton' 클릭 시 호출되는 함수
	UFUNCTION()
	void OnSubmitButtonClicked();

	// PlayerName 값을 'PlayerStateSave' 클래스에 저장할 함수
	// -- Save Data 만들기
	void SaveInitializedSaveData();

protected:
	// 플레이어 이름을 타이핑으로 입력받을 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFLobbyLevelUI", meta = (AllowPriaveAccess, BindWidget))
	TObjectPtr<class UEditableText> EditPlayerName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFLobbyLevelUI", meta = (AllowPriaveAccess, BindWidget))
	TObjectPtr<class UButton> SubmitButton;

	// 플레이어 이름을 입력받을 속성
	FString PlayerName = TEXT("Player0");
};
