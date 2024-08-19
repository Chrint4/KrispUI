#include "UIContainerList.hpp"

namespace KrispUI {
    void UIContainerList::RegenerateChildren() {
        int currentX = this->Size.x + this->padding + xOffset;
        int currentY = this->Size.y + this->padding + yOffset;
        int maxContainerHeightInRow = 0;
        int maxContainerWidthInColumn = 0;

        for (int i = 0; i < this->Children.size(); i++) {
            UIContainer* currentContainer = this->Children[i];

            if (this->wrap) {
                if (this->layout == VERTICAL && currentY + currentContainer->Size.h > this->Size.y + this->Size.h) {
                    currentY = this->Size.y + this->padding + yOffset;
                    currentX += maxContainerWidthInColumn + this->padding;
                    maxContainerWidthInColumn = 0; // Reset for the new column
                } else if (this->layout == HORIZONTAL && currentX + currentContainer->Size.w > this->Size.x + this->Size.w) {
                    currentX = this->Size.x + this->padding + xOffset;
                    currentY += maxContainerHeightInRow + this->padding;
                    maxContainerHeightInRow = 0; // Reset for the new row
                }
            }

            currentContainer->Move(currentX, currentY);

            if (this->layout == HORIZONTAL) {
                currentX += currentContainer->Size.w + this->padding;
                if (currentContainer->Size.h > maxContainerHeightInRow) {
                    maxContainerHeightInRow = currentContainer->Size.h;
                }
            } else if (this->layout == VERTICAL) {
                currentY += currentContainer->Size.h + this->padding;
                if (currentContainer->Size.w > maxContainerWidthInColumn) {
                    maxContainerWidthInColumn = currentContainer->Size.w;
                }
            }
        }
    }



}