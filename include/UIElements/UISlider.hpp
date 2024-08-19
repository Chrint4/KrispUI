#pragma once
#pragma warning(disable: 4244)
#include "UIButton.hpp"
#include "algorithm"

namespace KrispUI{
    class UISlider : public UIContainer{
        public:

            float percent;
            int min, max, current;

            string labelPrefix;

            UISlider(string _cname, int _posX, int _posY, int _w, SDL_Color _backCol, int _min, int _max, int _start, string _prefix = "")
                : UIContainer(_cname, _posX, _posY, _w, 29, _backCol), min(_min), max(_max), labelPrefix(_prefix) {
                    SDL_Color contentCol = {
                        (Uint8)std::min(255, _backCol.r + 20),
                        (Uint8)std::min(255, _backCol.g + 15),
                        (Uint8)std::min(255, _backCol.b + 15),
                        255
                    };
                    this->Children.push_back(
                        new UIButton(
                            _cname + "-scroller",
                            0,0,15, 29 - 2 * padding,
                            contentCol,
                            nullptr,
                            UIButton::FunctionData {
                                .OnLeftHold = [this]{
                                    UIContainer* slider = this->Children[0];

                                    int attatchPoint = slider->Size.w / 2 ;

                                    int lowerX = this->Size.x + this->padding + attatchPoint;
                                    float pos = KrispUI_DATA::K_MOUSE_X - lowerX ;
                                    float diffX = this->Size.w - 2 * padding - slider->Size.w;
                                    this->percent = clamp(pos/diffX, 0.0f, 1.0f);
                                    this->current = (int)((float)(percent * max) + min);

                                    slider->xOffset = percent * (diffX);

                                    UILabel* label = dynamic_cast<UILabel*>(this->Children[1]);
                                    label->ChangeText(labelPrefix + to_string(this->current));
                                }
                            }
                        )
                    );
                    this->Children.push_back(
                        new UILabel(
                            _cname + "-label",
                            0,0,100,100, SDL_Color{0,0,0,0},SDL_Color{0,0,0,0} , ""
                        )
                    );
                    this->Children[0]->parentContainer = this;
                    this->Children[1]->parentContainer = this;

                    SetValue(_start);
                };

            UISlider() {};
            ~UISlider() {};

            void SetValue(int val){
                if(val < min || val > max) return;
                current = val;

                this->percent = ((float)val / (float)max);

                UIContainer* slider = this->Children[0];
                slider->xOffset = percent * (this->Size.w - 2 * padding - slider->Size.w);

                UILabel* label = dynamic_cast<UILabel*>(this->Children[1]);
                label->ChangeText(labelPrefix + to_string(current));
            }

            void RegenerateChildren(){
                UIContainer* slider = this->Children[0];
                slider->Size.x = this->Size.x + padding + slider->xOffset;
                slider->Size.y = this->Size.y + padding;

                UILabel* label = dynamic_cast<UILabel*>(this->Children[1]);
                label->Size.x = this->Size.x + this->Size.w + padding;
                label->Size.y = this->Size.y + padding;
            }
    };
}