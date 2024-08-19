#pragma once
#include "UIContainer.hpp"
#include "UIContainerList.hpp"
#include "UIButton.hpp"
#include "KrispData.hpp"

namespace KrispUI {
    class UIScrollContainer : public UIContainer {
        private:
            const int SCROLLER_WIDTH = 15;

        public:
            int totalContentHeight = 0;
            
            float percent = 0;

            void AddChild(UIContainer* child, bool relative = true){
                if(!this->CanHaveChildren) return;
                child->parentContainer = this->Children[0];
                this->Children[0]->AddChild(child);
            }
            
            void RegenerateChildren();

            UIScrollContainer() {};
            ~UIScrollContainer() {};

            UIScrollContainer(string _cname,int _xPos, int _yPos, int _w, int _h, SDL_Color _backCol);
        

    };
}