#include "KrispRenderer.hpp"

#include "KrispData.hpp"

namespace KrispUI {
    void SDL_SetRenderDrawColor(SDL_Renderer *renderer, SDL_Color &col){
        SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
    }

    void KrispRenderer::LoadTextToTexture(SDL_Texture *&texture, SDL_Rect &dstRect, TTF_Font *&font, string message, SDL_Color col, int alignMode, int wrapLength, float scale){
        if(message == ""){
            SDL_DestroyTexture(texture);
            texture = nullptr;
            return;
        }

        switch (alignMode) {
            case 0: TTF_SetFontWrappedAlign(font, TTF_WRAPPED_ALIGN_LEFT); break;
            case 1: TTF_SetFontWrappedAlign(font, TTF_WRAPPED_ALIGN_CENTER); break;
            case 2: TTF_SetFontWrappedAlign(font, TTF_WRAPPED_ALIGN_RIGHT); break;
            default: TTF_SetFontWrappedAlign(font, TTF_WRAPPED_ALIGN_LEFT); break;
        }

        SDL_Surface *surfaceText = TTF_RenderText_Blended_Wrapped(font, message.c_str(), col, wrapLength);
        SDL_DestroyTexture(texture);
        texture = SDL_CreateTextureFromSurface(masterRenderer, surfaceText);
        if(!texture) cout << "ERROR: LoadTextToTexture | " << SDL_GetError();
        dstRect = {
            dstRect.x,
            dstRect.y,
            (int)(surfaceText->w * scale),
            (int)(surfaceText->h * scale)
        };
        SDL_FreeSurface(surfaceText);
        TTF_SetFontWrappedAlign(font, TTF_WRAPPED_ALIGN_LEFT);
    }
    
    void KrispRenderer::LoadTextToTexture(SDL_Texture *&texture, SDL_Rect &dstRect, string message, SDL_Color col, int alignMode, int wrapLength, float scale){
        LoadTextToTexture(texture, dstRect, font32, message, col, alignMode, wrapLength, scale);
    }

    void KrispRenderer::LoadTextToLabel(UILabel &container, string message) {
        LoadTextToTexture(
            container.Texture, 
            container.TextureSize, 
            font32, message, 
            container.FontCol, 
            0, 
            0, 
            1.0f
        );

        // cout << "|BEFORE|" << endl;
        // cout << container.Size.x << ", " <<
        //         container.Size.y << ", " <<
        //         container.Size.h << ", " <<
        //         container.Size.w << endl <<
        //         container.TextureSize.x << ", " <<
        //         container.TextureSize.y << ", " <<
        //         container.TextureSize.h << ", " <<
        //         container.TextureSize.w << endl << endl;

        container.Size.h = container.TextureSize.h + 2 * container.padding;
        container.Size.w = container.TextureSize.w + 2 * container.padding;

        container.TextureSize.x = container.Size.x + container.padding;
        container.TextureSize.y = container.Size.y + container.padding;

        // cout << "|AFTER|" << endl;
        // cout << container.Size.x << ", " <<
        //         container.Size.y << ", " <<
        //         container.Size.h << ", " <<
        //         container.Size.w << endl <<
        //         container.TextureSize.x << ", " <<
        //         container.TextureSize.y << ", " <<
        //         container.TextureSize.h << ", " <<
        //         container.TextureSize.w << endl << endl;
    }


    SDL_Texture* KrispRenderer::LoadSprite(string path){
        return SDL_CreateTextureFromSurface(masterRenderer, IMG_Load(path.c_str()));
    }

    void KrispRenderer::RenderUIContainer(UIContainer &ctn, SDL_Texture* targetBuffer){

        if(ctn.ContainerName == "container1-content"){
            // SDL_SetRenderTarget(masterRenderer, targetBuffer);
            SDL_Color c = SDL_Color {255,0,0,255};
            SDL_SetRenderDrawColor(masterRenderer, ctn.BackgroundCol);
            SDL_RenderFillRect(masterRenderer, &(ctn.Size));
            // return;
        }
            // cout << "Hello!" << endl;

        SDL_SetRenderTarget(masterRenderer, targetBuffer);
        SDL_SetRenderDrawColor(masterRenderer, ctn.BackgroundCol);
        SDL_RenderFillRect(masterRenderer, &(ctn.Size));

        ProcessUIChildren(ctn.Children, targetBuffer);
    }

    void KrispRenderer::RenderUIButton(UIButton &btn, SDL_Texture* targetBuffer){
        RenderUILabel(btn, targetBuffer);
    }

    void KrispRenderer::RenderUILabel(UILabel &txt, SDL_Texture* targetBuffer){
        if(txt.updateTexture){
            LoadTextToLabel(txt, txt.DisplayText);
            txt.updateTexture = false;
        }
        SDL_SetRenderTarget(masterRenderer, targetBuffer);
        SDL_SetRenderDrawColor(masterRenderer, txt.BackgroundCol);
        SDL_RenderFillRect(masterRenderer, &(txt.Size));
        if(txt.Texture)
            if(0 != SDL_RenderCopy(masterRenderer, txt.Texture, NULL, &(txt.TextureSize)))
                cout << SDL_GetError() << " | " << txt.ContainerName << endl;

        ProcessUIChildren(txt.Children, targetBuffer);
    }

    void KrispRenderer::RenderUINavigationBar(UINavigationBar &nav, SDL_Texture* targetBuffer){
        RenderUIContainer(nav, targetBuffer);
    }

    void KrispRenderer::RenderUIContainerList(UIContainerList &cli, SDL_Texture* targetBuffer){
        if(!cli.CropToSelection)
            RenderUIContainer(cli, targetBuffer);
        else {
            using namespace KrispUI::KrispUI_DATA;
            SDL_Texture* subBuffer = SDL_CreateTexture(masterRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, K_WINDOW_WIDTH, K_WINDOW_HEIHGT);

            RenderUIContainer(cli, subBuffer);
            SDL_SetRenderTarget(masterRenderer, targetBuffer);
            SDL_RenderCopy(masterRenderer, subBuffer, &(cli.Size), &(cli.Size));
            SDL_DestroyTexture(subBuffer);
        }
    }

    void KrispRenderer::RenderUIImage(UIImage *img, SDL_Texture* targetBuffer){
        SDL_SetRenderTarget(masterRenderer, targetBuffer);
        SDL_SetRenderDrawColor(masterRenderer, img->BackgroundCol);
        SDL_RenderFillRect(masterRenderer, &(img->Size));
        if(img->Image)
            if(img->imgFormat == UIImage::ImageFormat::FIT)
                SDL_RenderCopy(masterRenderer, img->Image, &(img->ImageSize), &(img->Size));
            else if(img->imgFormat == UIImage::ImageFormat::NONE)
                SDL_RenderCopy(masterRenderer, img->Image, NULL, &(img->Size));
            else 
                SDL_RenderCopy(masterRenderer, img->Image, NULL, &(img->Size));
        ProcessUIChildren(img->Children);
    }

    void KrispRenderer::RenderUIScrollContainer(UIScrollContainer &scr, SDL_Texture* targetBuffer) {
        RenderUIContainer(scr, targetBuffer);
    }

    void KrispRenderer::RenderUISlider(UISlider &sli, SDL_Texture* targetBuffer){
        SDL_SetRenderTarget(masterRenderer, targetBuffer);
        SDL_SetRenderDrawColor(masterRenderer, sli.BackgroundCol);
        SDL_RenderFillRect(masterRenderer, &(sli.Size));
    }

    void KrispRenderer::RenderUITextBox(UITextBox& textBox, SDL_Texture* targetBuffer){

        if(textBox.updateTexture){
            LoadTextToLabel(textBox, textBox.DisplayText);
            textBox.updateTexture = false;
        }

        // cout << textBox.DisplayText << endl;

        SDL_SetRenderTarget(masterRenderer, targetBuffer);
        if(textBox.isCapturingText){
            SDL_Color newCol = {
                (Uint8)min(255, textBox.BackgroundCol.r + 20),
                (Uint8)min(255, textBox.BackgroundCol.r + 15),
                (Uint8)min(255, textBox.BackgroundCol.b + 15),
                255
            };
            SDL_SetRenderDrawColor(masterRenderer, newCol);
        } else {
            SDL_SetRenderDrawColor(masterRenderer, textBox.BackgroundCol);
        }

        SDL_RenderFillRect(masterRenderer, &(textBox.Size));
        if(textBox.Texture)
            if(0 != SDL_RenderCopy(masterRenderer, textBox.Texture, NULL, &(textBox.TextureSize)))
                cout << SDL_GetError() << " | " << textBox.ContainerName << endl;

        ProcessUIChildren(textBox.Children, targetBuffer);
    }

    void KrispRenderer::ProcessUIChild(UIContainer* child, SDL_Texture* targetBuffer){
        if(!targetBuffer) targetBuffer = this->UITextureBuffer;
        if(child->Active)
            if(dynamic_cast<UITextBox*>(child))
                RenderUITextBox(*dynamic_cast<UITextBox*>(child), targetBuffer);
            else if(dynamic_cast<UILabel*>(child))
                RenderUILabel(*dynamic_cast<UILabel*>(child), targetBuffer);
            else if(dynamic_cast<UIButton*>(child))
                RenderUIButton(*dynamic_cast<UIButton*>(child), targetBuffer);
            else if(dynamic_cast<UINavigationBar*>(child))
                RenderUINavigationBar(*dynamic_cast<UINavigationBar*>(child), targetBuffer);

            else if(dynamic_cast<UIContainerList*>(child))
                RenderUIContainerList(*dynamic_cast<UIContainerList*>(child), targetBuffer);

            else if(dynamic_cast<UIImage*>(child))
                RenderUIImage(dynamic_cast<UIImage*>(child), targetBuffer);

            else if(dynamic_cast<UIScrollContainer*>(child))
                RenderUIScrollContainer(*dynamic_cast<UIScrollContainer*>(child), targetBuffer);

            else RenderUIContainer(*child, targetBuffer);
    }

    void KrispRenderer::ProcessUIChildren(vector<UIContainer*> &children, SDL_Texture* targetBuffer){
        for(UIContainer* child : children)
            ProcessUIChild(child, targetBuffer);
    }
}