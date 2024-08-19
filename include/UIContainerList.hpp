#pragma once

#include "UIContainer.hpp"

namespace KrispUI{
    /**
     * UIContainer List,
     * Like the navBar, structures buttons and other containers when added automatically.
     * has Layout Options:
     * Horizontal - Child containers are automatically positioned horizontally.
     * Vertical - Child containers are automatically positioned vertically.
     * 
     * ListLayout layout - defaults to VERTICAL [1]
     * boolean wrap - determines whether child containers overflow the bounds of the Parent Container or wrap aorund. defaults to TRUE
     * int padding - the space between each button, and the edge of the containers [px]. defaults to 5px.
    */
    class UIContainerList : public UIContainer{
        private:

        public:
            enum ListLayout{
                HORIZONTAL,
                VERTICAL
            };

            ListLayout layout;
            bool wrap = true;

            UIContainerList() {}
            ~UIContainerList() {}

            UIContainerList(string _cname, int _posX, int _posY, int _width, int _height, SDL_Color _backCol, ListLayout _layout = VERTICAL, bool _wrap = true)
                : UIContainer(_cname, _posX, _posY, _width, _height, _backCol), layout(_layout), wrap(_wrap) {}

            void SetLayout(ListLayout newLayout){
                if(newLayout != this->layout)
                    this->RegenerateChildren();
            }

            void SetWrap(bool newWrap){
                if(newWrap != this->wrap)
                    this->RegenerateChildren();
            }

            void RegenerateChildren();

            void AddChild(UIContainer* child, bool relativePos = false){
               UIContainer::AddChild(child, relativePos);
            }
    };
}