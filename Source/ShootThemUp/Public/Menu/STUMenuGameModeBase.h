// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "GameFramework/GameModeBase.h"
#include "STUMenuGameModeBase.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUMenuGameModeBase : public AGameModeBase
{
    GENERATED_BODY()
public:
    ASTUMenuGameModeBase();

    FOnMenuStateChangedSignature OnMenuStateChanged;

    virtual void StartPlay() override;

    void ChooseLevel() { SetMenuState(ESTUMenuState::ChooseLevel); }
    void MainMenu() { SetMenuState(ESTUMenuState::MenuStart); }
    void SetOptions() { SetMenuState(ESTUMenuState::Options); }

private:
    ESTUMenuState MenuState = ESTUMenuState::WaitingToStart;

    void SetMenuState(ESTUMenuState State);
};
