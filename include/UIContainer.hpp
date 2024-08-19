#pragma once
#include <string>
#include <vector>
#include <SDL2\SDL.h>
#include <iostream>

using namespace std;

namespace KrispUI {
    class UIContainer{
    private:
    
    public:
        string ContainerName;
        SDL_Color BackgroundCol;
        SDL_Rect Size;
        vector<UIContainer*> Children = vector<UIContainer*>();

        UIContainer* parentContainer = nullptr;

        bool CanHaveChildren = true;
        bool Active = true;
        bool CropToSelection = false;

        int xOffset = 0;
        int yOffset = 0;

        int padding = 5;


        UIContainer(){};
        UIContainer(string _cName, int _posX, int _posY, int _w, int _h, SDL_Color _bkgCol){
            ContainerName = _cName;
            Size = {_posX, _posY, _w, _h};
            BackgroundCol =  _bkgCol;
        };

        //position is relative to parent.
        virtual void AddChild(UIContainer* child, bool relativePos = true){
            if(!this->CanHaveChildren) return;
            if(relativePos){
                child->Size.x += this->Size.x;
                child->Size.y += this->Size.y;
            }
            child->parentContainer = this;
            Children.push_back(child);
        }

        virtual void SetActive(bool status){
            if(this->Active == status) return;
            this->Active = status;
            for(UIContainer* child : Children)
                child->SetActive(status);
        }

        virtual void RegenerateChildren() {
            for(UIContainer* child : Children)
                child->RegenerateChildren();
        };

        virtual void Translate(int amountX, int amountY){
            this->Size.x += amountX;
            this->Size.y += amountY;
            for(UIContainer* child : this->Children)
                child->Translate(amountX, amountY);
        }

        virtual void Move(int posX, int posY){
            this->Size.x = posX;
            this->Size.y = posY;
            // for(UIContainer* child : this->Children)
                // child->Move(posX, posY);
            this->RegenerateChildren();
        }

        virtual void ChangeParent(UIContainer* newParent){
            this->parentContainer = newParent;
        }

        virtual ~UIContainer() {};
    };
}