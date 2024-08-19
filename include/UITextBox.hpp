#ifndef KRISP_UI_TEXTBOX_H
#define KRISP_UI_TEXTBOX_H

#include "UILabel.hpp"

namespace KrispUI {
    class UITextBox : public UILabel {
        public:
            bool isCapturingText;

            UITextBox(string _cName, int _posX, int _posY, int _w, int _h, SDL_Color _fontCol, SDL_Color _backCol = {0,0,0,0}, string _txt = "")
                : UILabel(_cName, _posX, _posY, _w, _h, _fontCol, _backCol, _txt), isCapturingText(false) {}

            UITextBox(string _cName, int _posX, int _posY, int _w, int _h, SDL_Color _fontCol, string _txt = "")
                : UILabel(_cName, _posX, _posY, _w, _h, _fontCol, _txt), isCapturingText(false) {}

            UITextBox() 
                : isCapturingText(false) {}
    };
}

#endif