#pragma once
#include "UIContainer.hpp"

namespace KrispUI {
    class UINavigationBar : public UIContainer {
        public:
            UINavigationBar() {}
            ~UINavigationBar() {}
            UINavigationBar(string _cname, int _posX, int _posY, int _width, int _height, SDL_Color _bkgCol)
            : UIContainer(_cname, _posX, _posY, _width, _height, _bkgCol) {}
            void AddChild(UIContainer* child, bool relativePos){
                if(relativePos){
                    child->Size.x += this->Size.x;
                    child->Size.y += this->Size.y;
                }

                child->Size.y = 5;
                child->parentContainer = this;

                Children.push_back(child);

                int currentStartPos = 5;
                for(int i = 0; i < this->Children.size(); i++){
                    this->Children[i]->Size.x = currentStartPos;
                    currentStartPos += this->Children[i]->Size.w + 5;
                }
            }
    };
}