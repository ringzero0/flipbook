#include "Gui.hpp"


Gui::Gui(sf::RenderWindow& window) : window(window) {
    ImGui::SFML::Init(window);
    ImGuiIO& io = ImGui::GetIO();
    ImGui::GetIO().IniFilename = nullptr;


    // ImFont* font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/arial.ttf", 22.0f);
    //
    //
    // io.FontDefault = font;
    //
    //
    // ImGui::SFML::UpdateFontTexture();

    ImFont* font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/arial.ttf", 22.0f);
    if (font) {
        io.FontDefault = font;
        ImGui::SFML::UpdateFontTexture();
    } else {

        std::cerr << "Warning: could not load arial.ttf; using default font\n";
    }


    editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
    editor.SetShowWhitespaces(false);
    editor.SetTabSize(4);
    editor.SetReadOnly(false);
    // editor.SetText("-- code starts here");
    // setPixel(x,y,{r,g,b})
    // setPixel(0,0,{r=255,g=0,b=0}) << red at 0,0 place
    // getPixel(x,y) returns a color table/dictionary
    // color = getPixel(0,0) << color.r would be red if set
    // print(color.r,color.g,color.b)
    // width = getWidth()
    //  returns canvas width pixels
    // print(width)
    // height = getHeight() returns a height pixels
    // print(height)
    // copyRegion(x,y,x1,y1,dx2,dy2)  x and y are coordinates of top left of the rectangle,
    //  x1,y1 bottom, dx2,dy2 coords to be copied to
    // draw pixels at 0,0 1,1, 0,1, 1,0 then try
    //     copy(0,0,1,1,3,3)
    //     moveRegion(x,y,x1,y1,dx2,dy2) same as copy instead of copy it moves
    lua = new LuaEmbed(window);


}


Gui::~Gui()
{
	ImGui::SFML::Shutdown();
    delete lua;
}






void Gui::startUp() {
    if (!showStartupWindow) return;

    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(250, 200), ImGuiCond_Once);
    ImGui::Begin("Canvas Size", &showStartupWindow, ImGuiWindowFlags_NoResize);

    static char widthBuf[8] = "";
    static char heightBuf[8] = "";
    static std::string errorMsg;

    ImGui::Text("Width (px):");
    ImGui::SameLine();
    ImGui::InputText("##width", widthBuf, sizeof(widthBuf));

    ImGui::Text("Height (px):");
    ImGui::SameLine();
    ImGui::InputText("##height", heightBuf, sizeof(heightBuf));

    if (!errorMsg.empty()) {
        ImGui::TextColored(ImVec4(1,0,0,1), "%s", errorMsg.c_str());
    }

    if (ImGui::Button("OK", ImVec2(80, 30))) {
        try {
            int w = std::stoi(widthBuf);
            int h = std::stoi(heightBuf);

            if (w > 100 || h > 100) {
                errorMsg = "Width or Height must be <= 100px";
            } else {
                currentState.width = w;
                currentState.height = h;

                currentState.nCanvas->clear();
                if (currentState.canvasPosition != sf::Vector2f{3,3}) {
                    currentState.nCanvas->emplace_back(&window, w, h, currentState.canvasPosition);
                } else {
                    currentState.nCanvas->emplace_back(&window, w, h);
                }

                currentState.currentFrame = 0;
                // currentState.canvas = &(*currentState.nCanvas)[0];
                currentState.canvas = &(*currentState.nCanvas).front();

                showStartupWindow = false;
                errorMsg.clear();


                // animationPanelSetupRefresh();
                init();
                resize(window);
            }
        }
        catch (...) {
            errorMsg = "Invalid input!";
        }
    }

    ImGui::End();
}

// void Gui::menuBar() {
//     if (ImGui::BeginMainMenuBar()) {
//         if (ImGui::BeginMenu("File")) {
//             if (ImGui::MenuItem("New")) { showStartupWindow = true; }
//             if (ImGui::MenuItem("Open")) {
//                 FileUtils o(*currentState.nCanvas);
//                 std::string outpath = o.open();
//                 pxrt open;
//                 pxrt::pxrtFormat pxrtFile = open.readpxrt(outpath.c_str());
//
//                 if (pxrtFile.frames != 0) {
//                     currentState.nCanvas->clear();
//                     for (size_t i = 0; i < pxrtFile.frames; i++) {
//                         if (currentState.canvasPosition != sf::Vector2f{3,3})
//                             currentState.nCanvas->emplace_back(&window, pxrtFile.canvasData[i].width, pxrtFile.canvasData[i].height, currentState.canvasPosition);
//                         else
//                             currentState.nCanvas->emplace_back(&window, pxrtFile.canvasData[i].width, pxrtFile.canvasData[i].height);
//
//                         currentState.nCanvas->back().frameTime = pxrtFile.canvasData[i].frameTime;
//                         currentState.nCanvas->back().canvasImage.resize({ pxrtFile.canvasData[i].width , pxrtFile.canvasData[i].height }, pxrtFile.canvasData[i].pixels.data());
//                     }
//
//                     currentState.width = pxrtFile.canvasData[0].width;
//                     currentState.height = pxrtFile.canvasData[0].height;
//                     currentState.currentFrame = 0;
//                     currentState.canvas = &(*currentState.nCanvas)[0];
//
//                     // animationPanelSetupRefresh();
//                     resize(window);
//                 }
//             }
//             if (ImGui::MenuItem("Save")) {
//                 FileUtils s(*currentState.nCanvas);
//                 s.save();
//             }
//             if (ImGui::MenuItem("Export")) {
//                 FileUtils e(*currentState.nCanvas);
//                 e.convertToGif();
//                 currentState.canvas->exportCanvasImage();
//             }
//             if (ImGui::MenuItem("Exit")) { window.close(); }
//             ImGui::EndMenu();
//         }
//
//         if (ImGui::BeginMenu("Edit")) {
//             if (ImGui::MenuItem("Undo")) { /* implement */ }
//             if (ImGui::MenuItem("Redo")) { /* implement */ }
//             if (ImGui::MenuItem("Cut")) { /* implement */ }
//             if (ImGui::MenuItem("Copy")) { /* implement */ }
//             if (ImGui::MenuItem("Paste")) { /* implement */ }
//             ImGui::EndMenu();
//         }
//
//         if (ImGui::BeginMenu("View")) {
//             if (ImGui::MenuItem("Zoom In")) { /* implement */ }
//             if (ImGui::MenuItem("Zoom Out")) { /* implement */ }
//             if (ImGui::MenuItem("Toggle Grid")) { currentState.grid = !currentState.grid; }
//             ImGui::EndMenu();
//         }
//
//         if (ImGui::BeginMenu("Help")) {
//             if (ImGui::MenuItem("About")) { /* implement */ }
//             if (ImGui::MenuItem("Documentation")) { /* implement */ }
//             ImGui::EndMenu();
//         }
//
//         ImGui::EndMainMenuBar();
//     }
// }


void Gui::init() {


    startUp();
	menuBar();
	drawLeftPanel();
    drawColorShadesPanel();
	drawAnimationPanel();


}
//
// void Gui::codeWindow() {
//
//
//
//     static char buffer[1024 * 16] = ""; // 16 KB buffer for text
//
//         ImGui::Begin("Textarea Example");
//
//         ImGui::Text("Enter text below:");
//
//         ImGui::InputTextMultiline(
//             "##textarea",     // label (can hide with ##)
//             buffer,           // buffer to store text
//             IM_ARRAYSIZE(buffer), // size of buffer
//             ImVec2(-FLT_MIN, 200), // size (width=-automatic, height=200px)
//             ImGuiInputTextFlags_AllowTabInput // optional flags
//         );
//
//         ImGui::End();
//
// }


void Gui::codeWindow() {
    if (!currentState.codeEditor) {
        // currentState.editorFocused = false;
        return;
    }


    if (ImGui::Begin("Lua Code Editor", &currentState.codeEditor))
    {


        bool editorFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
        currentState.editorFocused = editorFocused;

        if (ImGui::Button("Run"))
        {

            std::string code = editor.GetText();



            // Execute Lua code
            if (lua->luaDoString(code.c_str()) != LUA_OK)
            {
                std::cerr << "Lua error: " << lua->luaToString(-1) << std::endl;
                lua->luaPopStack(1);
            }


        }




        ImVec2 editorSize = ImVec2(-FLT_MIN, -FLT_MIN);

        editor.Render("CodeEditor", editorSize);
    }



    ImGui::End();
}



void Gui::menuBar() {

    static bool showGifScaleDialog = false;
    static bool showPngScaleDialog = false;
    static int gifScale = 1;
    static int pngScale = 1;

    if (showGifScaleDialog)
    {
        ImGui::Begin("Export GIF", &showGifScaleDialog, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Text("Scale Canvas:");
        ImGui::InputInt("Scale factor", &gifScale);
        if (gifScale < 1) gifScale = 1;

        if (ImGui::Button("Export"))
        {
            FileUtils e(*currentState.nCanvas);
            e.convertToGif(gifScale);
            showGifScaleDialog = false;
            gifScale = 1;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            showGifScaleDialog = false;
            gifScale = 1;
        }

        ImGui::End();
    }

    if (showPngScaleDialog)
    {
        ImGui::Begin("Export PNG", &showPngScaleDialog, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Text("Scale Canvas:");
        ImGui::InputInt("Scale factor", &pngScale);
        if (pngScale < 1) pngScale = 1;

        if (ImGui::Button("Export"))
        {
            FileUtils e(*currentState.nCanvas);
            e.convertToPng(pngScale);
            showPngScaleDialog = false;
            pngScale = 1;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            showPngScaleDialog = false;
            pngScale = 1;
        }

        ImGui::End();
    }




    if (ImGui::BeginMainMenuBar()) {


        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New", "Ctrl+N")) {
                showStartupWindow = true;
            }

            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                FileUtils s(*currentState.nCanvas);

                s.save();
            }

            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                FileUtils o(*currentState.nCanvas);
                std::string outpath = o.open();
                pxrt open;
                pxrt::pxrtFormat pxrtFile = open.readpxrt(outpath.c_str());

                if (pxrtFile.frames != 0) {
                    currentState.nCanvas->clear();
                    for (size_t i = 0; i < pxrtFile.frames; i++) {
                        if (currentState.canvasPosition != sf::Vector2f{3,3})
                            currentState.nCanvas->emplace_back(&window, pxrtFile.canvasData[i].width, pxrtFile.canvasData[i].height, currentState.canvasPosition);
                        else
                            currentState.nCanvas->emplace_back(&window, pxrtFile.canvasData[i].width, pxrtFile.canvasData[i].height);

                        currentState.nCanvas->back().frameTime = pxrtFile.canvasData[i].frameTime;
                        currentState.nCanvas->back().canvasImage.resize({ pxrtFile.canvasData[i].width , pxrtFile.canvasData[i].height }, pxrtFile.canvasData[i].pixels.data());
                    }

                    currentState.width = pxrtFile.canvasData[0].width;
                    currentState.height = pxrtFile.canvasData[0].height;
                    currentState.currentFrame = 0;
                    // currentState.canvas = &(*currentState.nCanvas)[0];
                    currentState.canvas = &(*currentState.nCanvas).front();

                    resize(window);
                }
            }

            if (ImGui::BeginMenu("Export")) {
                if (ImGui::MenuItem("Export as GIF", "Ctrl+Shift+G")) {
                    showGifScaleDialog = true;


                }
                if (ImGui::MenuItem("Export as PNG", "Ctrl+Shift+P")) {
                    showPngScaleDialog = true;

                }
                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Exit", "Alt+F4")) { window.close(); }
            ImGui::EndMenu();
        }


        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) { /* implement */ }
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) { /* implement */ }
            if (ImGui::MenuItem("Cut", "Ctrl+X")) { /* implement */ }
            if (ImGui::MenuItem("Copy", "Ctrl+C")) { /* implement */ }
            if (ImGui::MenuItem("Paste", "Ctrl+V")) { /* implement */ }


            ImGui::Separator();

            if (ImGui::MenuItem(currentState.onion ? "Toggle Onion OFF" : "Toggle Onion ON", "Ctrl+Shift+O")) {
                currentState.onion = !currentState.onion;
            }
            if (ImGui::MenuItem("Copy Onion", "Ctrl+Alt+C")) {
                if (!currentState.canvas->isOnionCopied && currentState.currentFrame != 0) {
                    size_t prevFrame = (currentState.currentFrame + currentState.nCanvas->size() - 1) % currentState.nCanvas->size();
                    // currentState.canvas->copyOnionToCanvas((*currentState.nCanvas)[prevFrame].canvasImage);
                    auto it = currentState.nCanvas->begin();
                    std::advance(it, prevFrame);
                    currentState.canvas->copyOnionToCanvas(it->canvasImage);
                }
            }

            ImGui::EndMenu();
        }


        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Reset Canvas")) {
                resize(window);
            }

            if (ImGui::MenuItem("Zoom In", "Ctrl+Scroll up")) { /* implement */ }
            if (ImGui::MenuItem("Zoom Out", "Ctrl+Scroll down")) { /* implement */ }
            if (ImGui::MenuItem("Toggle Grid")) { currentState.grid = !currentState.grid; }
            ImGui::EndMenu();
        }


        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About")) { /* implement */ }
            if (ImGui::MenuItem("Documentation")) { /* implement */ }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void Gui::drawColorShadesPanel() {
    float panelY = window.getSize().y * 0.84f; // bottom panel Y
    float panelHeight = window.getSize().y * 0.16f;
    float leftPanelWidth = 150;   // match left panel
    float palettePanelWidth = 400; // adjust as needed

    ImGui::SetNextWindowPos(ImVec2(leftPanelWidth, panelY));
    ImGui::SetNextWindowSize(ImVec2(palettePanelWidth, panelHeight));
    ImGui::Begin("Color Palette", nullptr,
                 ImGuiWindowFlags_NoDecoration |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_HorizontalScrollbar);

    // Define main colors
    sf::Color colors[] = {
        sf::Color::Black, sf::Color::White, sf::Color::Red, sf::Color::Green,
        sf::Color::Blue, sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan,
        sf::Color(128, 0, 0), sf::Color(0, 128, 0), sf::Color(0, 0, 128),
        sf::Color(128, 128, 0), sf::Color(128, 0, 128), sf::Color(0, 128, 128),
        sf::Color(192,192,192), sf::Color(128,128,128)
    };

    ImVec2 btnSize(40, 40);
    int colorsPerRow = (int)(palettePanelWidth / (btnSize.x + 5)); // dynamically fit buttons

    for (size_t i = 0; i < sizeof(colors)/sizeof(colors[0]); ++i) {
        ImVec4 col(
            colors[i].r / 255.0f,
            colors[i].g / 255.0f,
            colors[i].b / 255.0f,
            1.0f
        );

        // Unique ID for each button
        std::string btnID = "##color" + std::to_string(i);

        ImGui::PushID(btnID.c_str());
        ImGui::PushStyleColor(ImGuiCol_Button, col);
        if (ImGui::Button("", btnSize)) {
            currentState.selectedColor = colors[i];
        }
        ImGui::PopStyleColor();
        ImGui::PopID();

        if ((i + 1) % colorsPerRow != 0)
            ImGui::SameLine();
    }

    ImGui::End();
}






// void Gui::drawLeftPanel() {
//
// 	ImGui::SetNextWindowPos(ImVec2(0, 25));
// 	ImGui::SetNextWindowSize(ImVec2(70, ImGui::GetIO().DisplaySize.y - 25));
//
// 	ImGui::Begin("Tools", nullptr,
// 		ImGuiWindowFlags_NoDecoration |
// 		ImGuiWindowFlags_NoResize |
// 		ImGuiWindowFlags_NoMove
// 	);
//
//
// 	const char* toolNames[] = { "Pencil", "Select", "Fill", "Move", "Eraser", "Palette" };
// 	const int toolCount = 6;
//
// 	for (int i = 0; i < toolCount; ++i) {
// 		if (i < 5) {
// 			primaryModes mode = static_cast<primaryModes>(i);
// 			bool selected = (currentState.pmode == mode);
//
// 			if (selected)
// 				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.6f, 1.0f, 1.0f));
//
// 			if (ImGui::Button(toolNames[i], ImVec2(60, 50)))
// 				currentState.pmode = mode;
//
// 			if (selected)
// 				ImGui::PopStyleColor();
// 		}
// 		else {
//
// 			if (ImGui::Button(toolNames[i], ImVec2(60, 50)))
// 				showColorPicker = !showColorPicker;
// 		}
// 	}
//
// 	ImGui::End();
//
// 	if (showColorPicker) {
// 		drawColorSelector();
// 	}
// }

// void Gui::drawToolBar() {
// 	// Position right below the menubar
// 	ImGui::SetNextWindowPos(ImVec2(0, 25)); // assuming menubar height = 25
// 	ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 40)); // smaller height
//
// 	ImGui::Begin("Toolbar", nullptr,
// 		ImGuiWindowFlags_NoDecoration |
// 		ImGuiWindowFlags_NoResize |
// 		ImGuiWindowFlags_NoMove
// 	);
//
// 	const char* toolNames[] = { "Pencil", "Select", "Fill", "Eraser", "Colors" };
// 	const int toolCount = 5;
//
// 	for (int i = 0; i < toolCount; ++i) {
// 		if (i < 5) {
// 			primaryModes mode = static_cast<primaryModes>(i);
// 			bool selected = (currentState.pmode == mode);
//
// 			if (selected)
// 				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.6f, 1.0f, 1.0f));
//
// 			if (ImGui::Button(toolNames[i], ImVec2(40, 30))) // smaller button
// 				currentState.pmode = mode;
//
// 			if (selected)
// 				ImGui::PopStyleColor();
// 		}
// 		else {
// 			// Color palette button
// 			if (ImGui::Button(toolNames[i], ImVec2(40, 30)))
// 				showColorPicker = !showColorPicker;
// 		}
//
// 		if (i < toolCount - 1)
// 			ImGui::SameLine();
// 	}
//
// 	ImGui::End();
//
// 	if (showColorPicker) {
// 		drawColorSelector();
// 	}
// }




void Gui::drawColorSelector() {
	ImGui::SetNextWindowSize(ImVec2(250, 300), ImGuiCond_Once);
	ImGui::Begin("Color Selector", &showColorPicker);

	// Convert sf::Color to ImVec4
	ImVec4 colorVec(
		currentState.selectedColor.r / 255.0f,
		currentState.selectedColor.g / 255.0f,
		currentState.selectedColor.b / 255.0f,
		1.0f
	);


	if (ImGui::ColorPicker3("Pick Color", (float*)&colorVec)) {
		currentState.selectedColor = sf::Color(
			static_cast<uint8_t>(colorVec.x * 255),
			static_cast<uint8_t>(colorVec.y * 255),
			static_cast<uint8_t>(colorVec.z * 255)
		);
	}

	ImGui::End();
}


// void Gui::drawAnimationPanel() {
//
//     ImGui::SetNextWindowPos(ImVec2(0, window.getSize().y * 0.84f));
//     ImGui::SetNextWindowSize(ImVec2(window.getSize().x, window.getSize().y * 0.16f));
//     ImGui::Begin("Animation Panel", nullptr, ImGuiWindowFlags_NoDecoration);
//
//
//     if (ImGui::Button(animationState.isPlaying ? "Pause" : "Play", ImVec2(90, 30)))
//         animationState.isPlaying = !animationState.isPlaying;
//     ImGui::SameLine();
//
//     if (ImGui::Button(currentState.onion ? "Onion ON" : "Onion OFF", ImVec2(130, 30)))
//         currentState.onion = !currentState.onion;
//     ImGui::SameLine();
//
//     if (ImGui::Button("Copy Onion", ImVec2(130, 30))) {
//         if (!currentState.canvas->isOnionCopied && currentState.currentFrame != 0) {
//             size_t prevFrame = (currentState.currentFrame + currentState.nCanvas->size() - 1) % currentState.nCanvas->size();
//             currentState.canvas->copyOnionToCanvas((*currentState.nCanvas)[prevFrame].canvasImage);
//         }
//     }
//     ImGui::SameLine();
//
//     if (ImGui::Button(currentState.grid ? "Grid ON" : "Grid OFF", ImVec2(130, 30)))
//         currentState.grid = !currentState.grid;
//
//     ImGui::Spacing();
//
//
//     if (currentState.canvas) {
//         int frameDuration = currentState.canvas->frameTime;
//     	ImGui::PushItemWidth(250);
//         if (ImGui::InputInt("Frame Duration (ms)", &frameDuration)) {
//             currentState.canvas->frameTime = frameDuration;
//         }
//     }
//
//     ImGui::Spacing();
//
//
//     ImGui::Text("Frames:");
//     ImGui::BeginChild("FramesChild", ImVec2(0, 50), false, ImGuiWindowFlags_HorizontalScrollbar);
//
//     for (size_t i = 0; i < currentState.nCanvas->size(); ++i) {
//         char buf[16];
//         sprintf(buf, "%zu", i + 1);
//
//         if (i > 0) ImGui::SameLine();
//
//         if (ImGui::Button(buf, ImVec2(40, 40))) {
//             currentState.currentFrame = i;
//             currentState.canvas = &(*currentState.nCanvas)[i];
//         }
//
//         // Highlight current frame
//         if (currentState.currentFrame == i) {
//             ImGui::GetWindowDrawList()->AddRect(
//                 ImGui::GetItemRectMin(), ImGui::GetItemRectMax(),
//                 IM_COL32(100, 149, 237, 255), 0.0f, 0, 3.0f
//             );
//         }
//     }
//
//     ImGui::SameLine();
//     if (ImGui::Button("+", ImVec2(40, 40))) {
//         size_t idx = currentState.nCanvas->size();
//         if (currentState.canvasPosition == sf::Vector2f({3, 3})) {
//             currentState.nCanvas->emplace_back(&window, currentState.width, currentState.height, currentState.canvas->getCanvasPosition());
//         } else {
//             currentState.nCanvas->emplace_back(&window, currentState.width, currentState.height, currentState.canvasPosition);
//         }
//         currentState.currentFrame = currentState.nCanvas->size() - 1;
//         currentState.canvas = &(*currentState.nCanvas)[currentState.currentFrame];
//     }
//
//     ImGui::EndChild();
//
//     ImGui::End();
// }

void Gui::drawLeftPanel() {
    ImVec2 buttonSize(130, 30); // keep buttons same size
    float panelWidth = 150;     // slightly wider than buttons
    float panelHeight = window.getSize().y * 0.16f; // same as animation panel
    float panelY = window.getSize().y * 0.84f;      // same Y as animation panel

    ImGui::SetNextWindowPos(ImVec2(0, panelY));
    ImGui::SetNextWindowSize(ImVec2(panelWidth, panelHeight));

    ImGui::Begin("LeftPanel", nullptr,
                 ImGuiWindowFlags_NoDecoration |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove);

    const char* toolNames[] = { "Pencil", "Fill", "Eraser" };
    const int toolCount = 3;

    for (int i = 0; i < toolCount; ++i) {
        primaryModes mode = static_cast<primaryModes>(i);
        bool selected = (currentState.pmode == mode);

        if (selected)
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.6f, 1.0f, 1.0f));

        if (ImGui::Button(toolNames[i], buttonSize))
            currentState.pmode = mode;

        if (selected)
            ImGui::PopStyleColor();
    }

    ImGui::Spacing();

    if (ImGui::Button("Colors", buttonSize))
        showColorPicker = !showColorPicker;
    if (ImGui::Button("Code", buttonSize))
        currentState.codeEditor = !currentState.codeEditor;

    ImGui::End();

    if (showColorPicker)
        drawColorSelector();
    if (currentState.codeEditor)
        codeWindow();
}

void Gui::drawAnimationPanel() {
    float panelY = window.getSize().y * 0.84f;
    float panelHeight = window.getSize().y * 0.16f;
    float leftPanelWidth = 150;
    float palettePanelWidth = 400; // match palette panel width

    ImGui::SetNextWindowPos(ImVec2(leftPanelWidth + palettePanelWidth, panelY));
    ImGui::SetNextWindowSize(ImVec2(window.getSize().x - (leftPanelWidth + palettePanelWidth), panelHeight));
    ImGui::Begin("Animation Panel", nullptr, ImGuiWindowFlags_NoDecoration);


    if (ImGui::Button(animationState.isPlaying ? "Pause" : "Play", ImVec2(90, 30)))
        animationState.isPlaying = !animationState.isPlaying;
    ImGui::SameLine();

    if (ImGui::Button(currentState.onion ? "Onion ON" : "Onion OFF", ImVec2(130, 30)))
        currentState.onion = !currentState.onion;
    ImGui::SameLine();

    if (ImGui::Button("Copy Onion", ImVec2(130, 30))) {
        if (!currentState.canvas->isOnionCopied && currentState.currentFrame != 0) {
            size_t prevFrame = (currentState.currentFrame + currentState.nCanvas->size() - 1) % currentState.nCanvas->size();
            // currentState.canvas->copyOnionToCanvas((*currentState.nCanvas)[prevFrame].canvasImage);
            auto it = currentState.nCanvas->begin();
            std::advance(it, prevFrame);
            currentState.canvas->copyOnionToCanvas(it->canvasImage);
        }
    }
    ImGui::SameLine();

    if (ImGui::Button(currentState.grid ? "Grid ON" : "Grid OFF", ImVec2(130, 30)))
        currentState.grid = !currentState.grid;

    ImGui::SameLine();

    ImGui::Text("X: %d, Y: %d", static_cast<int>(currentState.mousePosOnCanvas.x), static_cast<int>(currentState.mousePosOnCanvas.y));


    ImGui::Spacing();


    if (currentState.canvas) {
        int frameDuration = currentState.canvas->frameTime;
        ImGui::PushItemWidth(150);
        if (ImGui::InputInt("Frame Duration (ms)", &frameDuration))
            currentState.canvas->frameTime = frameDuration;
        ImGui::PopItemWidth();
    }

    ImGui::Spacing();


    ImGui::Text("Frames:");
    ImGui::BeginChild("FramesChild", ImVec2(0, 50), false, ImGuiWindowFlags_HorizontalScrollbar);

    for (size_t i = 0; i < currentState.nCanvas->size(); ++i) {
        char buf[16];
        sprintf(buf, "%zu", i + 1);

        if (i > 0) ImGui::SameLine();

        if (ImGui::Button(buf, ImVec2(40, 40))) {
            currentState.currentFrame = i;
            // currentState.canvas = &(*currentState.nCanvas)[i];
            auto it = currentState.nCanvas->begin();
            std::advance(it, i);
            currentState.canvas = &(*it);

        }

        if (currentState.currentFrame == i) {
            ImGui::GetWindowDrawList()->AddRect(
                ImGui::GetItemRectMin(), ImGui::GetItemRectMax(),
                IM_COL32(100, 149, 237, 255), 0.0f, 0, 3.0f
            );
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("+", ImVec2(40, 40))) {
        size_t idx = currentState.nCanvas->size();
        if (currentState.canvasPosition == sf::Vector2f({3, 3})) {
            currentState.nCanvas->emplace_back(&window, currentState.width, currentState.height, currentState.canvas->getCanvasPosition());
        } else {
            currentState.nCanvas->emplace_back(&window, currentState.width, currentState.height, currentState.canvasPosition);
        }
        currentState.currentFrame = currentState.nCanvas->size() - 1;
        // currentState.canvas = &(*currentState.nCanvas)[currentState.currentFrame];
        auto it = currentState.nCanvas->begin();
        std::advance(it, currentState.currentFrame);
        currentState.canvas = &(*it);

    }

    ImGui::EndChild();
    ImGui::End();
}













// void Gui::menuBar() {
//
//
// 	if (ImGui::BeginMainMenuBar())
// 	{
// 		if (ImGui::BeginMenu("File"))
// 		{
// 			if (ImGui::MenuItem("Open")) { /* ... */ }
// 			if (ImGui::MenuItem("Save")) { /* ... */ }
// 			ImGui::EndMenu();
// 		}
//
// 		if (ImGui::BeginMenu("Edit"))
// 		{
// 			if (ImGui::MenuItem("Undo")) { /* ... */ }
// 			if (ImGui::MenuItem("Redo")) { /* ... */ }
// 			ImGui::EndMenu();
// 		}
//
// 		ImGui::EndMainMenuBar();
// 	}
//
//
// }






void Gui::processEvents(sf::Event &event)
{
	ImGui::SFML::ProcessEvent(window, event);
}

bool Gui::wantMouse()
{
	return ImGui::GetIO().WantCaptureMouse;
}

void Gui::update(sf::Time dt)
{
	ImGui::SFML::Update(window, dt);
}

void Gui::render(){
	ImGui::SFML::Render(window);
}