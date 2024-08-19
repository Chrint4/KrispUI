#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>

#include "UIContainer.hpp"
#include "UILabel.hpp"
#include "UIButton.hpp"
#include "UIImage.hpp"
#include "UINavigationBar.hpp"
#include "UIContainerList.hpp"
#include "UIScrollContainer.hpp"
#include "UISlider.hpp"
#include "UITextBox.hpp"

namespace KrispUI {
    class KrispRenderer {

    private:
        SDL_Renderer* masterRenderer = nullptr;
        SDL_Texture* UITextureBuffer = nullptr;
        TTF_Font* font32 = nullptr;

    public:
        KrispRenderer() {};
        KrispRenderer(SDL_Renderer* _renderer, SDL_Texture* _outputBuffer, TTF_Font* _font) 
            : masterRenderer(_renderer), UITextureBuffer(_outputBuffer), font32(_font) {}
        ~KrispRenderer() {};

        void LoadTextToTexture(SDL_Texture *&texture, SDL_Rect &dstRect, TTF_Font *&font, string message, SDL_Color col, int alignMode, int wrapLength, float scale = 1.0);
        void LoadTextToTexture(SDL_Texture *&texture, SDL_Rect &dstRect, string message, SDL_Color col, int alignMode, int wrapLength, float scale);
        void LoadTextToLabel(UILabel &container, string message);
        SDL_Texture* LoadSprite(string path);

        void RenderUIButton(UIButton &btn, SDL_Texture* targetBuffer = nullptr);
        void RenderUIContainer(UIContainer &ctn, SDL_Texture* targetBuffer = nullptr);
        void RenderUIImage(UIImage *img, SDL_Texture* targetBuffer = nullptr);
        void RenderUILabel(UILabel &txt, SDL_Texture* targetBuffer = nullptr);
        void RenderUINavigationBar(UINavigationBar &nav, SDL_Texture* targetBuffer = nullptr);
        void RenderUIContainerList(UIContainerList &cli, SDL_Texture* targetBuffer = nullptr);
        void RenderUIScrollContainer(UIScrollContainer &scr, SDL_Texture* targetBuffer = nullptr);
        void RenderUISlider(UISlider &scr, SDL_Texture* targetBuffer = nullptr);
        void RenderUITextBox(UITextBox &textBox, SDL_Texture* targetBuffer = nullptr);

        void ProcessUIChild(UIContainer* child, SDL_Texture* targetBuffer = nullptr);
        void ProcessUIChildren(vector<UIContainer*> &children, SDL_Texture* targetBuffer = nullptr);

        SDL_Texture* GetUITextureBuffer(){
            return this->UITextureBuffer;
        }
    };
}