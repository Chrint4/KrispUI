#include "KrispInput.hpp"

namespace KrispUI{
    std::array<KrispInput::Key, KrispInput::NUM_KEYS> KrispInput::keys;
    bool KrispInput::IsCapturingText = 0;

    void KrispInput::SetKeyCallback(SDL_Scancode scancode, KeyCallback callback) {
        keys[scancode].callback = callback;
    }

    void KrispInput::SetKeyOneTimePress(SDL_Scancode scancode, bool oneTimePress){
        if(oneTimePress) 
            keys[scancode].data |= ONE_TIME_PRESS_MASK;
        else
            keys[scancode].data &= ~ONE_TIME_PRESS_MASK;
    }

    void KrispInput::SetKeyToggle(SDL_Scancode scancode, bool toggle) {
        if(toggle)
            keys[scancode].data |= TOGGLE_FLAG_MASK;
        else
            keys[scancode].data &= ~TOGGLE_FLAG_MASK;
    }

    void KrispInput::ProcessKeys(const Uint8* keystates){
        for(size_t i = 0; i < NUM_KEYS; ++i) {
            bool keyPressed = keystates[i];
            Key& key = keys[i];

            if(keyPressed) {
                if(!(key.data & KEY_PRESSED_MASK)) {
                    if(key.callback) key.callback();
                    if(key.data & (TOGGLE_FLAG_MASK | ONE_TIME_PRESS_MASK)) key.data ^= TOGGLE_STATE_MASK;
                    if(key.data & ONE_TIME_PRESS_MASK) key.data |= KEY_PRESSED_MASK;
                }
            } else {
                key.data &= ~KEY_PRESSED_MASK;
            }
        }
    }
}