#pragma warning(disable : 4244)
#pragma warning(disable : 4189) // 'attribute': local variable is initialized but not referenced
#pragma warning(disable : 4505)
#pragma warning(disable : 26495)
#pragma warning(disable : 26819)

#ifndef KRISP_UI_H
#define KRISP_UI_H

// #define INCLUDE_FIND_PARENT_CONTAINER

    #include "KrispData.hpp"
    #include "KrispElements.hpp"
    #include "KrispRenderer.hpp"
    #include "KrispInput.hpp"

    /**
     * UI TRANSFORMATION IS RELATIVE TO PARENT!!!!
     *  technically, direct children of RootUI have global transformation, as RootUI spans the entire window.. but we ball
    */
   // ^^^ THIS IS A LIE, IT IS JUST ALL GLOBAL BUT IS RELATIVE "OFFSET"

    namespace KrispUI {

        typedef void (*ButtonEventCallback)();
        static UIContainer RootUI;
        static KrispRenderer UIRenderer;

        //SHOULD NOT BE DIRECTLY ACCESSED
        //SHOULD ONLY BE CHANGED USING THE METHODS BELOW...
        namespace P_UIContainerCache{
            static UIContainer* TargetContainer = nullptr;
            static UIContainer* LastTargetContainer = nullptr;
            static UIContainer* ClickedContainer = nullptr;
            static UIContainer* LastClickedContainer = nullptr;
            static UIContainer* SelectedContainer = nullptr;
            static UIContainer* LastSelectedContainer = nullptr;

            string GetContainerCacheReport() {
                return
                    "TargetContainer: " + (TargetContainer ? TargetContainer->ContainerName : "NULL") + "\n"
                    "LastTargetContainer: " + (LastTargetContainer ? LastTargetContainer->ContainerName : "NULL") + "\n"
                    "----------------------------------------\n"
                    "SelectedContainer: " + (SelectedContainer ? SelectedContainer->ContainerName : "NULL") + "\n"
                    "LastSelectedContainer: " + (LastSelectedContainer ? LastSelectedContainer->ContainerName : "NULL") + "\n"
                    "----------------------------------------\n"
                    "ClickedContainer: " + (ClickedContainer ? ClickedContainer->ContainerName : "NULL") + "\n"
                    "LastClickedContainer: " + (LastClickedContainer ? LastClickedContainer->ContainerName : "NULL");
            }
        } // namespace P_UIContainerCache

        template <typename T>
        static void SetSelectedContainer(T* newSelectedContainer) {
            using namespace P_UIContainerCache;
            static_assert(std::is_base_of<UIContainer, T>::value, "T must inherit from UIContainer");
            if(!SelectedContainer){
                SelectedContainer = static_cast<UIContainer*>(newSelectedContainer);
            } else if(newSelectedContainer != SelectedContainer) {
                LastSelectedContainer = SelectedContainer;
                SelectedContainer = static_cast<UIContainer*>(newSelectedContainer);
            }
        }

        template <typename T>
        static void SetTargetContainer(T* newTargetContainer) {
            using namespace P_UIContainerCache;
            static_assert(std::is_base_of<UIContainer, T>::value, "T must inherit from UIContainer");
            if(newTargetContainer != TargetContainer) {
                LastTargetContainer = TargetContainer;
                TargetContainer = static_cast<UIContainer*>(newTargetContainer);
            }
        }

        template <typename T>
        static void SetClickedContainer(T* newClickedContainer) {
            using namespace P_UIContainerCache;
            static_assert(std::is_base_of<UIContainer, T>::value, "T must inherit from UIContainer");
            if(newClickedContainer != ClickedContainer) {
                LastClickedContainer = ClickedContainer;
                ClickedContainer = static_cast<UIContainer*>(newClickedContainer);
            }
        }

        static void ClearSelectedContainer() {
            using namespace P_UIContainerCache;
            if(SelectedContainer != nullptr) {
                LastSelectedContainer = SelectedContainer;
                SelectedContainer = nullptr;
            }
        }

        static void ClearTargetContainer() {
            using namespace P_UIContainerCache;
            if(TargetContainer != nullptr) {
                LastTargetContainer = TargetContainer;
                TargetContainer = nullptr;
            }
        }

        static UIContainer* GetTargetContainer() { return P_UIContainerCache::TargetContainer; }
        static UIContainer* GetSelectedContainer() { return P_UIContainerCache::SelectedContainer; }
        static UIContainer* GetLastSelectedContainer() { return P_UIContainerCache::LastSelectedContainer; }
        static UIContainer* GetLastTargetContainer() { return P_UIContainerCache::LastTargetContainer; }
        static UIContainer* GetClickedContainer() { return P_UIContainerCache::ClickedContainer; }
        static UIContainer* GetLastClickedContainer() {return P_UIContainerCache::LastClickedContainer;}

        SDL_Color RANDOMCOL(){
            SDL_Color col;
            col.r = rand() % 255;
            col.g = rand() % 255;
            col.b = rand() % 255;
            col.a = 255;
            return col;
        }

        namespace UIFactories {
            static UIContainer* CreateUIContainer(string _cName, int _posX, int _posY, int _w, int _h, SDL_Color _bkgCol){
                return new UIContainer(_cName, _posX, _posY, _w, _h, _bkgCol);
            }

            static UIButton* CreateUIButton(string _cName, int _posX, int _posY, int _w, int _h, SDL_Color _backCol, SDL_Texture* _sprite = nullptr, UIButton::FunctionData _functionData = UIButton::FunctionData::DEFAULT) {
                return new UIButton(_cName, _posX, _posY, _w, _h, _backCol, _sprite, _functionData);
            }

            static UIButton* CreateUIButton(string _cName, int _posX, int _posY, int _w, int _h, SDL_Color _backCol, string _displayText = "", UIButton::FunctionData _functionData = UIButton::FunctionData::DEFAULT) {
                return new UIButton(_cName, _posX, _posY, _w, _h, _backCol, _displayText, _functionData);
            }

            static UIButton* CreateUIButton(string _cName, int _posX, int _posY, SDL_Color _backCol, SDL_Texture* _sprite = nullptr, UIButton::FunctionData _functionData = UIButton::FunctionData::DEFAULT) {
                return new UIButton(_cName, _posX, _posY, 0, 0, _backCol, _sprite, _functionData);
            }

            static UIButton* CreateUIButton(string _cName, int _posX, int _posY, SDL_Color _backCol, string _displayText = "", UIButton::FunctionData _functionData = UIButton::FunctionData::DEFAULT) {
                return new UIButton(_cName, _posX, _posY, 0, 0, _backCol, _displayText, _functionData);
            }

            static UIButton* CreateUIButton(string _cName, string message = "", UIButton::FunctionData _functionData = UIButton::FunctionData::DEFAULT){
                return CreateUIButton(_cName, 0,0,0,0, RANDOMCOL(), message, _functionData);
            }

            static UIButton* CreateUIButton(string _cName, SDL_Color _backCol, string message = "", UIButton::FunctionData _functionData = UIButton::FunctionData::DEFAULT){
                return CreateUIButton(_cName, 0,0,0,0, _backCol, message, _functionData);
            }

            static vector<UIContainer*> CreateUIButtons(string _cName, SDL_Color _backCol, string message = "", int num = 1){
                vector<UIContainer*> buttons;
                for(int i = 0; i < num; i++)
                    buttons.push_back(CreateUIButton(_cName + "-" + to_string(i), 0,0,0,0, RANDOMCOL(), message, UIButton::FunctionData::DEFAULT));
                return buttons;
            }

            // static UILabel* CreateUILabel(string _cName, int _posX, int _posY, int _w, int _h, SDL_Color _backCol, string _txt = ""){
            //     return new UILabel(_cName, _posX, _posY, _w, _h, _backCol, _txt);
            // }

            // static UILabel* CreateUILabel(string _cName, int _posX, int _posY, int _w, int _h, SDL_Color _fontCol, SDL_Color _backCol, string _txt = ""){
            //     return new UILabel(_cName, _posX, _posY, _w, _h, _fontCol, _backCol, _txt);
            // }

            static UILabel* CreateUILabel(string _cName,SDL_Color _fontCol, SDL_Color _backCol, string _txt = ""){
                return new UILabel(_cName, 0, 0, 0, 0, _fontCol, _backCol, _txt);
            }

            // static UILabel* CreateUILabel(string _cName, SDL_Color _backCol, string _txt = ""){
            //     return new UILabel(_cName, 0, 0, 0, 0, _backCol, _txt);
            // }

            static UIContainerList* CreateUIContainerList(string _cname, int _posX, int _posY, int _width, int _height, SDL_Color _backCol, UIContainerList::ListLayout _layout, bool _wrap){
                return new UIContainerList(_cname, _posX, _posY, _width, _height, _backCol, _layout, _wrap);
            }

            static UIContainerList* CreateUIContainerList(string _cname, int _posX, int _posY, int _width, int _height, SDL_Color _backCol, UIContainerList::ListLayout _layout){
                return new UIContainerList(_cname, _posX, _posY, _width, _height, _backCol, _layout);
            }

            static UIContainerList* CreateUIContainerList(string _cname, int _posX, int _posY, int _width, int _height, SDL_Color _backCol){
                return new UIContainerList(_cname, _posX, _posY, _width, _height, _backCol);
            }

            static UIContainerList* CreateUIContainerList(string _cname, int _posX, int _posY, int _width, int _height, SDL_Color _backCol, bool _wrap){
                return new UIContainerList(_cname, _posX, _posY, _width, _height, _backCol, UIContainerList::ListLayout::VERTICAL, _wrap);
            }

            static UIContainerList* CreateUINavigationBar(string _cname, int _posX, int _posY, SDL_Color _backCol){
                return CreateUIContainerList(_cname, _posX, _posY, KrispUI_DATA::K_WINDOW_WIDTH, 39, _backCol, UIContainerList::ListLayout::HORIZONTAL, true);
            }

            static UIImage* CreateUIImage(string _cname, int _posX, int _posY, int _w, int _h, SDL_Color _backCol, SDL_Texture* _img = nullptr, UIImage::ImageFormat _format = UIImage::ImageFormat::NONE){
                return new UIImage(_cname, _posX, _posY, _w, _h, _backCol, _img, _format);
            }

            static UIScrollContainer* CreateUIScrollContainer(string _cname, int _posX, int _posY, int _w, int _h, SDL_Color _backCol) {
                return new UIScrollContainer(_cname, _posX, _posY, _w, _h, _backCol);
            }

            static UISlider* CreateUISlider(string _cname, int _posX, int _posY, int _w, SDL_Color _backCol, int _min, int _max, int _start){
                return new UISlider(_cname, _posX, _posY, _w, _backCol, _min, _max, _start);
            }

            static UISlider* CreateUISlider(string _cname, int _posX, int _posY, int _w, SDL_Color _backCol, int _min, int _max, int _start, string _prefix){
                return new UISlider(_cname, _posX, _posY, _w, _backCol, _min, _max, _start, _prefix);
            }

            static UITextBox* CreateUITextBox(string _cName,SDL_Color _fontCol, SDL_Color _backCol, string _txt = ""){
                return new UITextBox(_cName, 0, 0, 0, 0, _fontCol, _backCol, _txt);
            }
        } // namespace UIFactories

        using namespace UIFactories;

        namespace P_UISearchRecursiveCalls {
            // DFS search through UITree for container most under Mouse.
            static UIContainer* R_FindContainerAtMouse(const SDL_Point &mousePos, UIContainer *currentContainer = &RootUI){
                if(SDL_PointInRect(&mousePos, &(currentContainer->Size))){
                    for(UIContainer *child : currentContainer->Children){
                        if(!child->Active) continue;
                        UIContainer* result = R_FindContainerAtMouse(mousePos, child);
                        if(result != nullptr)
                            return result;
                    }
                    return currentContainer;
                }
                return nullptr;
            }

            //DFS search through UITree for first container with matching name.
            static UIContainer* R_FindContainerByName(string name, UIContainer *currentContainer = &RootUI){
                if(currentContainer->ContainerName == name) return currentContainer;
                for(UIContainer *child : currentContainer->Children){
                    UIContainer* result = R_FindContainerByName(name, child);
                    if(result != nullptr)
                        return result;
                }
                return nullptr;
            }

            #ifdef INCLUDE_FIND_PARENT_CONTAINER
                //DFS search through UITree for parent of first container with matching name.
                static UIContainer* R_FindParentContainer(string name, UIContainer *currentContainer = &RootUI){
                    for(UIContainer* child : currentContainer->Children){
                        if(child->ContainerName == name) return currentContainer;
                        return R_FindParentContainer(name, child);
                    }
                    return nullptr;
                }
            #endif
        } // namespace P_UISearchRecursiveCalls

        static UIContainer* FindContainer(const SDL_Point &mousePos){
            UIContainer* result = P_UISearchRecursiveCalls::R_FindContainerAtMouse(mousePos);
            if(result == nullptr)
                return nullptr;
            return result;
        }

        template <typename T>
        static T* FindContainer(const std::string& ContainerName) {
            static_assert(std::is_base_of<UIContainer, T>::value, "T must inherit from UIContainer");

            UIContainer* result = P_UISearchRecursiveCalls::R_FindContainerByName(ContainerName);
            if(result == nullptr)
                return nullptr;
            
            return static_cast<T*>(result);
        }

        static UIContainer* FindContainer(string ContainerName){
            return FindContainer<UIContainer>(ContainerName);
        }

        #define GetContainer FindContainer

        #ifdef INCLUDE_FIND_PARENT_CONTAINER
            //returns the parent of the given container
            static UIContainer* FindParentContainer(string ContainerName){
                if(ContainerName == RootUI.ContainerName) return nullptr;
                UIContainer* result = P_UISearchRecursiveCalls::R_FindParentContainer(ContainerName);
                if(result == nullptr){
                    //error message
                    return nullptr;
                }
                return result;
            }
        #endif

        static UIContainer* AddChildToContainer(string parentContainerName, UIContainer* container, bool relativePos = false){
            UIContainer* parentContainer = FindContainer(parentContainerName);
            if(parentContainer != nullptr){
                container->parentContainer = parentContainer;
                parentContainer->AddChild(container, relativePos);
                // std::cout << "Container: " << container->ContainerName << " added to Parent Container: " << parentContainerName << "\n\0";
            } else{
                // std::cout << "Couldnt add Contianer: " << container->ContainerName << "as Parent Container: " << parentContainerName << "not found..\n\0";
            }
            return container;
        }

        static void AddChildrenToContainer(string parentContainerName, vector<UIContainer*> containers, bool relativePos = false){
            for(UIContainer* container : containers)
                AddChildToContainer(parentContainerName, container, relativePos);
        }

        static void ProcessChanges(){
            RootUI.RegenerateChildren();
        }

        static void RenderUIToBuffer(){
            ProcessChanges();
            UIRenderer.RenderUIContainer(RootUI, UIRenderer.GetUITextureBuffer());
        }

        static void INIT_Krisp(int _w, int _h){
            RootUI = UIContainer("RootUI", 0, 0, _w, _h, SDL_Color {255,255,255,255});
            // KrispUI::SetSelectedContainer(&RootUI);
            KrispUI_DATA::K_WINDOW_WIDTH = _w;
            KrispUI_DATA::K_WINDOW_HEIHGT = _h;
        }

        void ProcessInputs(SDL_Event &event){
            int mouseState = SDL_GetMouseState(&KrispUI_DATA::K_MOUSE_X, &KrispUI_DATA::K_MOUSE_Y);
            SDL_Point mousePos = {KrispUI_DATA::K_MOUSE_X, KrispUI_DATA::K_MOUSE_Y};

            if(!KrispInput::IsCapturingText) {
                const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
                KrispInput::ProcessKeys(keyboardState);
            }

            //Update KRISP_DATA
            {
                using namespace KrispUI::KrispUI_DATA;
                K_MOUSE_X = mousePos.x;
                K_MOUSE_Y = mousePos.y;

            }

            UIContainer* targetUI = nullptr;

            using namespace KrispUI_DATA;


            if((SDL_BUTTON(1) | SDL_BUTTON(2) | SDL_BUTTON(3)) & mouseState)
                SetSelectedContainer(GetTargetContainer());
            else {
                ClearSelectedContainer();
                SetTargetContainer(FindContainer(mousePos));

            }

            if(!GetTargetContainer()) return;

            if (SDL_BUTTON(1) & mouseState) {
                if (!LeftClick) {
                    SetClickedContainer(GetTargetContainer());
                    LeftClick = 1;
                    //Action on down
                    if (dynamic_cast<UIButton*>(GetSelectedContainer())) {
                        if (dynamic_cast<UIButton*>(GetSelectedContainer())->functionData.OnLeftDown)
                            dynamic_cast<UIButton*>(GetSelectedContainer())->functionData.OnLeftDown();
                    }
                }
                //Action on hold down
                if (dynamic_cast<UIButton*>(GetSelectedContainer())) {
                    if (dynamic_cast<UIButton*>(GetSelectedContainer())->functionData.OnLeftHold)
                        dynamic_cast<UIButton*>(GetSelectedContainer())->functionData.OnLeftHold();
                }
            } else if (LeftClick) {
                LeftClick = 0;
                //Action on up
                if (dynamic_cast<UIButton*>(GetSelectedContainer())) {
                    if (dynamic_cast<UIButton*>(GetSelectedContainer())->functionData.OnLeftClick)
                        dynamic_cast<UIButton*>(GetSelectedContainer())->functionData.OnLeftClick();
                }
            }

            if (SDL_BUTTON(2) & mouseState) {
                if (!MiddleClick) {
                    SetClickedContainer(GetTargetContainer());
                    MiddleClick = 1;
                    //Action on down
                    if (dynamic_cast<UIButton*>(GetSelectedContainer())) {
                        if (dynamic_cast<UIButton*>(GetSelectedContainer())->functionData.OnMiddleDown)
                            dynamic_cast<UIButton*>(GetSelectedContainer())->functionData.OnMiddleDown();
                    }
                }
                //Action on hold down
                if (dynamic_cast<UIButton*>(GetSelectedContainer())) {
                    if (dynamic_cast<UIButton*>(GetSelectedContainer())->functionData.OnMiddleHold)
                        dynamic_cast<UIButton*>(GetSelectedContainer())->functionData.OnMiddleHold();
                }
            } else if (MiddleClick) {
                MiddleClick = 0;
                //Action on up
                if (dynamic_cast<UIButton*>(GetSelectedContainer())) {
                    if (dynamic_cast<UIButton*>(GetSelectedContainer())->functionData.OnMiddleClick)
                        dynamic_cast<UIButton*>(GetSelectedContainer())->functionData.OnMiddleClick();
                }
            }

            if (SDL_BUTTON(3) & mouseState) {
                if (!RightClick) {
                    SetClickedContainer(GetTargetContainer());
                    RightClick = 1;
                    //Action on down
                    if (dynamic_cast<UIButton*>(GetSelectedContainer())) {
                        if (dynamic_cast<UIButton*>(GetSelectedContainer())->functionData.OnRightDown)
                            dynamic_cast<UIButton*>(GetSelectedContainer())->functionData.OnRightDown();
                    }
                }
                //Action on hold down
                if (dynamic_cast<UIButton*>(GetSelectedContainer())) {
                    if (dynamic_cast<UIButton*>(GetSelectedContainer())->functionData.OnRightHold)
                        dynamic_cast<UIButton*>(GetSelectedContainer())->functionData.OnRightHold();
                }
            } else if (RightClick) {
                RightClick = 0;
                //Action on up
                if (dynamic_cast<UIButton*>(GetSelectedContainer())) {
                    if (dynamic_cast<UIButton*>(GetSelectedContainer())->functionData.OnRightClick)
                        dynamic_cast<UIButton*>(GetSelectedContainer())->functionData.OnRightClick();
                }
            }

            if(GetClickedContainer()) {
                //works
                if(dynamic_cast<UITextBox*>(GetLastClickedContainer())) {
                    UITextBox* textbox = dynamic_cast<UITextBox*>(GetLastClickedContainer());
                    if(textbox->isCapturingText) {
                        textbox->isCapturingText = false;
                        KrispInput::IsCapturingText = false;
                        SDL_StopTextInput();
                    }
                }

                //the worksn't bit :/
                if(dynamic_cast<UITextBox*>(GetClickedContainer())) {
                    UITextBox* textbox = dynamic_cast<UITextBox*>(GetClickedContainer());
                    if(!textbox->isCapturingText) {
                        textbox->isCapturingText = true;
                        KrispInput::IsCapturingText = true;
                        SDL_StartTextInput();
                    } else if(event.type == SDL_TEXTINPUT ){
                        textbox->AppendText(event.text.text);
                    } else if (textbox->isCapturingText && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE && !textbox->DisplayText.empty()) {
                        textbox->PopCharacter();
                    } else if (textbox->isCapturingText && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN && !textbox->DisplayText.empty()) {
                        textbox->AppendText("\n");
                    }
                }
            }

            //happens
            // if(SDL_IsTextInputActive() == SDL_TRUE)
            //     cout << "ON" << endl;
            // else
            //     cout << "OFF" << endl;

            //happensn't
            // if(event.type == SDL_TEXTINPUT)
            //     cout << "Recieved SDL_TextInput event: " << event.text.text << endl;
        }
    }
#endif