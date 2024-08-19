#ifndef KRISP_UI_BUTTON_H
#define KRISP_UI_BUTTON_H

#include <functional>
#include <SDL2/SDL_image.h>
#include "UILabel.hpp"

namespace KrispUI {
    class UIButton : public UILabel{
        public:

        #define NO_FUNCTION []{}
        using ButtonEventCallback = std::function<void()>;

        struct FunctionData {
            public:
                ButtonEventCallback OnLeftDown = NO_FUNCTION;
                ButtonEventCallback OnLeftHold = NO_FUNCTION;
                ButtonEventCallback OnLeftClick = NO_FUNCTION;
                ButtonEventCallback OnMiddleDown = NO_FUNCTION;
                ButtonEventCallback OnMiddleHold = NO_FUNCTION;
                ButtonEventCallback OnMiddleClick = NO_FUNCTION;
                ButtonEventCallback OnRightDown = NO_FUNCTION;
                ButtonEventCallback OnRightHold = NO_FUNCTION;
                ButtonEventCallback OnRightClick = NO_FUNCTION;

                static const FunctionData DEFAULT;
        };

        FunctionData functionData;
        
        UIButton() = delete;  
        ~UIButton() {};

        UIButton(string _cName, int _posX, int _posY, int _w, int _h, SDL_Color _backCol = {0,0,0,0}, SDL_Texture* _sprite = nullptr, FunctionData _functionData = FunctionData::DEFAULT)
            : UILabel(_cName, _posX, _posY, _w, _h, _backCol, _sprite), functionData(_functionData) {};

        UIButton(string _cName, int _posX, int _posY, int _w, int _h, SDL_Color _backCol, string _displayText = "", FunctionData _functionData = FunctionData::DEFAULT)
            : UILabel(_cName, _posX, _posY, _w, _h, SDL_Color {255,255,255,255}, _backCol, _displayText), functionData(_functionData) {};
    };

}

#endif