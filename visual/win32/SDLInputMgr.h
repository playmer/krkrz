#pragma once

#include <array>
#include <map>
#include <memory>
#include <vector>

#include <Windows.h>

#include "tjsTypes.h"


#include "SDL.h"
//struct SDL_GameController;
//struct SDL_ControllerAxisEvent;
//struct SDL_ControllerButtonEvent;
//
//typedef uint8_t Uint8;
//typedef int32_t Sint32;
//typedef Sint32 SDL_JoystickID;
//typedef int16_t Sint16;

const char* ConvertToString(Uint8 aButton);
size_t ConvertToIndex(Uint8 aButton);

class Controller
{
public:
    Controller()
    {
        __debugbreak();
    }

    Controller(SDL_GameController* aGameController);

    void Reset();
    float ToFloat(Sint16 aValue);
    void HandleMotion(SDL_ControllerAxisEvent& aEvent);
    void HandleButton(SDL_ControllerButtonEvent& aEvent);

    SDL_GameController* mGameController;
    const char* mName;

    std::array<bool, 15> mPreviousButtons;
    std::array<bool, 15> mCurrentButtons;

    //glm::vec2 LeftStick;
    //glm::vec2 RightStick;
    float LeftTrigger;
    float RightTrigger;

    const std::vector<WORD>& GetUppedKeys() const { return UppedKeys; }
    const std::vector<WORD>& GetDownedKeys() const { return DownedKeys; }
    const std::vector<WORD>& GetRepeatKeys() const { return RepeatKeys; }

    std::vector<WORD> UppedKeys;
    std::vector<WORD> DownedKeys;
    std::vector<WORD> RepeatKeys;
};

class SdlInputMgr
{
public:
	SdlInputMgr(HWND handle);
	~SdlInputMgr();

	void Update();
    static SdlInputMgr* sInstance;
    std::map<SDL_JoystickID, Controller> mControllers;
private:
};

bool SdlGetJoyPadAsyncState(tjs_uint keycode, bool getcurrent);
