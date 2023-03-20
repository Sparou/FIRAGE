// Shoot Them Up Game, All Rights Reserved.


#include "STUGameInstance.h"
#include "STUSoundFuncLib.h"

void USTUGameInstance::ToggleVolume()
{
    USTUSoundFuncLib::ToggleSoundClassVolume(MasterSoundClass);
}
