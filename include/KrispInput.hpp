#ifndef KRISP_INPUT_H
#define KRISP_INPUT_H

#include <array>
#include <functional>

#include <SDL_scancode.h>
#include <SDL_events.h>

namespace KrispUI {
    using KeyCallback = std::function<void()>;

    class KrispInput {
        private:
            static constexpr uint8_t ONE_TIME_PRESS_MASK =  0b00000001;
            static constexpr uint8_t TOGGLE_FLAG_MASK =     0b00000010;
            static constexpr uint8_t TOGGLE_STATE_MASK =    0b00000100;
            static constexpr uint8_t KEY_PRESSED_MASK =     0b00001000;
            static constexpr uint8_t DEFAULT_STATE_MASK =   ONE_TIME_PRESS_MASK | TOGGLE_STATE_MASK;

            struct Key {
                KeyCallback callback = nullptr;
                // bool pressed = false;
                uint8_t data = DEFAULT_STATE_MASK;
            };

            static constexpr size_t NUM_KEYS = SDL_NUM_SCANCODES;
            static std::array<Key, NUM_KEYS> keys;

            static int MouseX, MouseY;

            static bool LeftClick, MiddleClick, RightClick;


        public:
            static bool IsCapturingText;
            static void ProcessKeys(const Uint8* keystates);

            static void SetKeyCallback(SDL_Scancode scancode, KeyCallback callback);
            static void SetKeyOneTimePress(SDL_Scancode scancode, bool oneTimePress);
            static void SetKeyToggle(SDL_Scancode scancode, bool toggle);

            static bool GetKeyToggleState(SDL_Scancode scancode) {return keys[scancode].data & TOGGLE_STATE_MASK;}
            static bool GetKeyToggleFlag(SDL_Scancode scancode) {return keys[scancode].data & TOGGLE_FLAG_MASK;}
    };
}

#endif