#pragma warning(disable : 4244)
#include "UIScrollContainer.hpp"
#include "algorithm"

namespace KrispUI {
    void UIScrollContainer::RegenerateChildren() {

        UIContainer* content = this->Children[0];
        content->Size.x = this->Size.x + this->padding;
        content->Size.y = this->Size.y + this->padding;
        content->RegenerateChildren();

        int containerHeight = 3 * content->padding;
        for(UIContainer* c : content->Children)
            containerHeight += c->Size.h + padding;
        this->totalContentHeight = containerHeight;


        UIContainer* scroller = this->Children[1];
        scroller->Size.w = SCROLLER_WIDTH;
        scroller->Size.x = this->Size.x + padding + content->Size.w + padding;
        scroller->Size.y = this->Size.y + padding + scroller->yOffset;

        int p = content->Size.h;
        if (totalContentHeight == 0) {
            scroller->Size.h = p;
        } else {
            scroller->Size.h = static_cast<int>((p * p) / max(p, totalContentHeight));
        }
    }

    UIScrollContainer::UIScrollContainer(string _cname,int _xPos, int _yPos, int _w, int _h, SDL_Color _backCol)
        : UIContainer(_cname, _xPos, _yPos, _w, _h, _backCol) {
            SDL_Color contentCol = {
                (Uint8)min(255, _backCol.r + 20),
                (Uint8)min(255, _backCol.g + 15),
                (Uint8)min(255, _backCol.b + 15),
                255
            };

            this->Children.push_back(
                new UIContainerList(
                    _cname + "-content",
                    0,0,
                    this->Size.w - 2 * padding - 20,
                    this->Size.h - 2 * padding,
                    contentCol,
                    UIContainerList::ListLayout::VERTICAL,
                    false
                )
            );
            
            this->Children.push_back(
                new UIButton(
                    _cname + "-scroller",
                    0,0,SCROLLER_WIDTH,0,
                    contentCol, "",
                    UIButton::FunctionData {
                        .OnLeftHold = [this](){
                            using namespace KrispUI::KrispUI_DATA;
                            UIContainer* scroller = this->Children[1];
                            UIContainerList* content = dynamic_cast<UIContainerList*>(this->Children[0]);

                            int attatchPoint = scroller->Size.h / 2 ;

                            int lowerY = this->Size.y + this->padding + attatchPoint;
                            float pos = K_MOUSE_Y - lowerY ;
                            float diffY = this->Size.h - 2 * padding - scroller->Size.h;
                            this->percent = clamp(pos/diffY, 0.0f, 1.0f);


                            content->yOffset = -percent * (totalContentHeight - this->Size.h);
                            scroller->yOffset = percent * (diffY);
                            this->RegenerateChildren(); 
                        }
                    }
                )
            );

            this->Children[0]->CropToSelection = true;

            this->Children[0]->parentContainer = this;
            this->Children[1]->parentContainer = this;
        };
}