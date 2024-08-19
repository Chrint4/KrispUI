#pragma once
#include "UIContainer.hpp"

namespace KrispUI{
    class UIImage : public UIContainer{
    public:
        SDL_Texture* Image = nullptr;

        SDL_Rect ImageSize = {0,0,0,0};

        enum ImageFormat{
            NONE,
            STRETCH,
            FIT,
            FORCE_UI_RESCALE
        };

        ImageFormat imgFormat = NONE;

        ~UIImage(){};
        UIImage(string _cName, int _posX, int _posY, int _w, int _h, SDL_Color _backCol, SDL_Texture* _img = nullptr, ImageFormat _format = NONE)
            : UIContainer(_cName, _posX, _posY, _w, _h, _backCol) {
                SetImage(_img, _format);
            };

        void SetImage(SDL_Texture *tex, ImageFormat format = NONE){
            this->imgFormat = format;
            this->Image = tex;
            int texWidth, texHeight;
            SDL_QueryTexture(tex, NULL, NULL, &texWidth, &texHeight);
            switch (imgFormat) {
            case STRETCH:
            case FIT:
            case FORCE_UI_RESCALE:
            case NONE:
            default:
                ImageSize = {0,0,texWidth, texHeight};
            }
        }
    };
}