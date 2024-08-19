#ifndef KRISP_UI_LABEL_H
#define KRISP_UI_LABEL_H

#include <string>
#include "UIContainer.hpp"

namespace KrispUI {

    //NOT USED HERE (RENDERER HAS OWN FORMATTER).. should be here rly..
    // enum UITextAlignMode {
    //     LEFT,
    //     CENTER,
    //     RIGHT
    // };

    class UILabel : public UIContainer{
    public:
        string DisplayText;
        SDL_Color FontCol;
        bool updateTexture;
        SDL_Texture* Texture = nullptr;

        SDL_Rect TextureSize = {0,0,0,0};

        UILabel(string _cName, int _posX, int _posY, int _w, int _h, SDL_Color _fontCol, SDL_Color _backCol = {0,0,0,0}, string _txt = "")
            : UIContainer(_cName, _posX, _posY, _w, _h, _backCol), DisplayText(_txt), FontCol(_fontCol), updateTexture(true) {
            };

        UILabel(string _cName, int _posX, int _posY, int _w, int _h, SDL_Color _fontCol, string _txt = "")
            : UIContainer(_cName, _posX, _posY, _w, _h, SDL_Color {0,0,0,0}), DisplayText(_txt), FontCol(_fontCol), updateTexture(true) {
            };

        UILabel(string _cName, int _posX, int _posY, int _w, int _h, SDL_Color _backCol = {0,0,0,0}, SDL_Texture* _sprite = nullptr)
            : UIContainer(_cName, _posX, _posY, _w, _h, _backCol), Texture (_sprite), updateTexture(false) {
                this->TextureSize = this->Size;
            };

        UILabel(){};
        ~UILabel(){};

        void ChangeText(string msg){
            DisplayText = msg;
            updateTexture = true;
            // requiresPadding = Text == "";
        }

        void AppendText(string msg) {
            DisplayText += msg;
            updateTexture = true;
        }

        void PopCharacter() {
            DisplayText.pop_back();
            updateTexture = true;
        }

        void SetSprite(SDL_Texture* newTex){
            SDL_DestroyTexture(this->Texture);
            Texture = newTex;
            updateTexture = false;
        }

        void Translate(int amountX, int amountY) {
            UIContainer::Translate(amountX, amountY);
            this->TextureSize.x += amountX;
            this->TextureSize.y += amountY;
        }

        void RegenerateChildren(){
            this->TextureSize.x = this->Size.x + this->padding;
            this->TextureSize.y = this->Size.y + this->padding;
        }
    };
}

#endif