// FGameMode.cpp

#include "Game/FGameMode.h"
#include "Controllers/FPlayerController.h"

AFGameMode::AFGameMode()
{
	// 플레이어 컨트롤러 설정 -- Class 정보 초기화
	// Object 생성하지 않고 Class 정보로 지정하는 이유:
	// 멀티플레이 고려 - Class 정보로 플레이어 입장마다 Instance를 생성함
	PlayerControllerClass = AFPlayerController::StaticClass();
}
