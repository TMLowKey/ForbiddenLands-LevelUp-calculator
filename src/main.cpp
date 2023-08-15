// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

#include <algorithm>
#include <string>
#include <vector>
#include <cctype>
#include <map>
#include <iostream>


// CONST
const int MAX_LEVEL = 10;
const int MIN_LEVEL = 0;
const char* attributeList[] = {
    "Strength", 
    "Agility", 
    "Wits", 
    "Empathy"
};

const char* skillList[] = {
    "Might", 
    "Endurance", 
    "Melee", 
    "Crafting", 
    "Stealth", 
    "Sleight of Hand", 
    "Move", 
    "Marksmanship", 
    "Scouting", 
    "Lore", 
    "Survival", 
    "Insight", 
    "Manipulation", 
    "Performance", 
    "Healing", 
    "Animal Handling"
};

const char* generalTalentsList[] = {
    "Ambidextrous",
    "Axe Fighter",
    "Berserker",
    "Bowyer",
    "Brawler",
    "Builder",
    "Chef",
    "Cold Blooded",
    "Defender",
    "Dragonslayer",
    "Executioner",
    "Fast Footwork",
    "Fast Shooter",
    "Fearless",
    "Firm Grip",
    "Fisher",
    "Hammer Fighter",
    "Herbalist",
    "Horseback Fighter",
    "Incorruptible",
    "Knife Fighter",
    "Lightning Fast",
    "Lockpicker",
    "Lucky",
    "Master of the Hunt",
    "Melee Charge",
    "Pack Rat",
    "Pain Resistant",
    "Pathfinder",
    "Poisoner",
    "Quartermaster",
    "Quickdraw",
    "Sailor",
    "Sharpshooter",
    "Sharp Tongue",
    "Shield Fighter",
    "Sixth Sense",
    "Smith",
    "Spear Fighter",
    "Steady Feet",
    "Sword Fighter",
    "Tailor",
    "Tanner",
    "Threatening",
    "Throwing Arm",
    "Wanderer"
};

const char* classTalentsList[] = {
    //"THE DRUID'S TALENTS",
    "Path of Healing",
    "Path of Shifting Shapes",
    "Path of Sight",
    //"THE FIGHTER'S TALENTS",
    "Path of The Blade",
    "Path of The Enemy",
    "Path of The Shield",
    //"THE HUNTER'S TALENTS",
    "Path of The Arrow",
    "Path of The Beast",
    "Path of The Forest",
    //"THE MINSTREL'S TALENTS",
    "Path of The Hymn",
    "Path of The Song",
    "Path of The Warcry",
    //"THE PEDDLER'S TALENTS",
    "Path of Gold",
    "Path of Lies",
    "Path of Many Things",
    //"THE RIDER'S TALENTS",
    "Path of The Companion",
    "Path of The Knight",
    "Path of The Plains",
    //"THE ROGUE'S TALENTS",
    "Path of The Face",
    "Path of The Killer",
    "Path of Poison",
    //"THE SORCERER'S TALENTS",
    "Path of Blood",
    "Path of Death",
    "Path of Signs",
    "Path of Stone"
};

std::map<std::string, int> attributeDict {
    { "Strength", 0 },
    { "Agility", 0 },
    { "Wits", 0 },
    { "Empathy", 0 },
};

std::map<std::string, int> skillDict {
    { "Might", 0 },
    { "Endurance", 0 },
    { "Melee", 0 },
    { "Crafting", 0 },
    { "Stealth", 0 },
    { "Sleight of Hand", 0 },
    { "Move", 0 },
    { "Marksmanship", 0 },
    { "Scouting", 0 },
    { "Lore", 0 },
    { "Survival", 0 },
    { "Insight", 0 },
    { "Manipulation", 0 },
    { "Performance", 0 },
    { "Healing", 0 },
    { "Animal Handling", 0 },
};

std::map<std::string, int> generalTalentsDict {
    { "Ambidextrous", 0 },
    { "Axe Fighter", 0 },
    { "Berserker", 0 },
    { "Bowyer", 0 },
    { "Brawler", 0 },
    { "Builder", 0 },
    { "Chef", 0 },
    { "Cold Blooded", 0 },
    { "Defender", 0 },
    { "Dragonslayer", 0 },
    { "Executioner", 0 },
    { "Fast Footwork", 0 },
    { "Fast Shooter", 0 },
    { "Fearless", 0 },
    { "Firm Grip", 0 },
    { "Fisher", 0 },
    { "Hammer Fighter", 0 },
    { "Herbalist", 0 },
    { "Horseback Fighter", 0 },
    { "Incorruptible", 0 },
    { "Knife Fighter", 0 },
    { "Lightning Fast", 0 },
    { "Lockpicker", 0 },
    { "Lucky", 0 },
    { "Master of the Hunt", 0 },
    { "Melee Charge", 0 },
    { "Pack Rat", 0 },
    { "Pain Resistant", 0 },
    { "Pathfinder", 0 },
    { "Poisoner", 0 },
    { "Quartermaster", 0 },
    { "Quickdraw", 0 },
    { "Sailor", 0 },
    { "Sharpshooter", 0 },
    { "Sharp Tongue", 0 },
    { "Shield Fighter", 0 },
    { "Sixth Sense", 0 },
    { "Smith", 0 },
    { "Spear Fighter", 0 },
    { "Steady Feet", 0 },
    { "Sword Fighter", 0 },
    { "Tailor", 0 },
    { "Tanner", 0 },
    { "Threatening", 0 },
    { "Throwing Arm", 0 },
    { "Wanderer", 0 },
};

std::map<std::string, int> classTalentsDict {
    //"THE DRUID'S TALENTS",
    { "Path of Healing", 0 },
    { "Path of Shifting Shapes", 0 },
    { "Path of Sight", 0 },
        //"THE FIGHTER'S TALENTS",
    { "Path of The Blade", 0 },
    { "Path of The Enemy", 0 },
    { "Path of The Shield", 0 },
        //"THE HUNTER'S TALENTS",
    { "Path of The Arrow", 0 },
    { "Path of The Beast", 0 },
    { "Path of The Forest", 0 },
        //"THE MINSTREL'S TALENTS",
    { "Path of The Hymn", 0 },
    { "Path of The Song", 0 },
    { "Path of The Warcry", 0 },
        //"THE PEDDLER'S TALENTS",
    { "Path of Gold", 0 },
    { "Path of Lies", 0 },
    { "Path of Many Things", 0 },
        //"THE RIDER'S TALENTS",
    { "Path of The Companion", 0 },
    { "Path of The Knight", 0 },
    { "Path of The Plains", 0 },
        //"THE ROGUE'S TALENTS",
    { "Path of The Face", 0 },
    { "Path of The Killer", 0 },
    { "Path of Poison", 0 },
        //"THE SORCERER'S TALENTS",
    { "Path of Blood", 0 },
    { "Path of Death", 0 },
    { "Path of Signs", 0 },
    { "Path of Stone", 0 },
};

// CUSTOM FUNCTIONS
void AddTalentSlider(const char* talentName, int& talentValue)
{
    ImGui::TableNextRow();

    ImGui::TableSetColumnIndex(0);
    ImGui::Text(talentName);

    std::string supertalentName = "##" + std::string(talentName);

    ImGui::TableSetColumnIndex(1);
    ImGui::SliderInt(supertalentName.c_str(), &talentValue, 0, 3);
}

void AddClass(const char* class_name, const char* class_talents, const char* talentName1, const char* talentName2, const char* talentName3, int& talentValue1, int& talentValue2, int& talentValue3)
{
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    if (ImGui::CollapsingHeader(class_name, ImGuiTreeNodeFlags_None))
    {
        ImGui::BeginTable(class_talents, 2);

        AddTalentSlider(talentName1, talentValue1);

        AddTalentSlider(talentName2, talentValue2);

        AddTalentSlider(talentName3, talentValue3);

        ImGui::EndTable();
    }

}

void AddAttributeOrSkill(const char* element1, int& element1Value, const char* element2, int& element2Value, const char* element3, int& element3Value, const char* element4, int& element4Value)
{
    std::string m_element1 = element1;
    
    if (std::islower(m_element1[0]))
    {
        m_element1[0] = std::toupper(m_element1[0]);
    }

    std::string m_element2 = element2;

    if (std::islower(m_element2[0]))
    {
        m_element2[0] = std::toupper(m_element2[0]);
    }

    std::string m_element3 = element3;

    if (std::islower(m_element3[0]))
    {
        m_element3[0] = std::toupper(m_element3[0]);
    }

    std::string m_element4 = element4;

    if (std::islower(m_element4[0]))
    {
        m_element4[0] = std::toupper(m_element4[0]);
    }

    std::string superelement1 = "##" + std::string(element1);

    ImGui::TableSetColumnIndex(0);
    ImGui::Text(m_element1.c_str());
    ImGui::TableSetColumnIndex(1);

    ImGui::InputInt(superelement1.c_str(), &element1Value);
    element1Value = std::min(std::max(element1Value, MIN_LEVEL), MAX_LEVEL);

    std::string superelement2 = "##" + std::string(element2);

    ImGui::TableSetColumnIndex(2);
    ImGui::Text(m_element2.c_str());
    ImGui::TableSetColumnIndex(3);

    ImGui::InputInt(superelement2.c_str(), &element2Value);
    element2Value = std::min(std::max(element2Value, MIN_LEVEL), MAX_LEVEL);

    std::string superelement3 = "##" + std::string(element3);

    ImGui::TableSetColumnIndex(4);
    ImGui::Text(m_element3.c_str());
    ImGui::TableSetColumnIndex(5);

    ImGui::InputInt(superelement3.c_str(), &element3Value);
    element3Value = std::min(std::max(element3Value, MIN_LEVEL), MAX_LEVEL);

    std::string superelement4 = "##" + std::string(element4);

    ImGui::TableSetColumnIndex(6);
    ImGui::Text(m_element4.c_str());
    ImGui::TableSetColumnIndex(7);

    ImGui::InputInt(superelement4.c_str(), &element4Value);
    element4Value = std::min(std::max(element4Value, MIN_LEVEL), MAX_LEVEL);
}



// Main code
int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ShowWindows
        {
            // Get the position and size of the viewport
            ImVec2 window_pos = ImGui::GetMainViewport()->Pos;
            ImVec2 window_size = ImGui::GetMainViewport()->Size;

            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always); // This will set the window to the top-left corner of the screen
            ImGui::SetNextWindowSize(window_size, ImGuiCond_Always); // This will set the window's size to the size of the viewport

            // Create a window that doesn't move or resize
            ImGui::Begin("My Window", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
            
            // ATRIBUTE SECTION
                ImGui::BeginTable("Atributes and Skills", 8);

                ImGui::TableNextRow();

                static int strength = MIN_LEVEL;
                static int agility = MIN_LEVEL;
                static int wits = MIN_LEVEL;
                static int empathy = MIN_LEVEL;


                AddAttributeOrSkill("strength", attributeDict["Strength"], "agility", attributeDict["Agility"], "wits", attributeDict["Wits"], "empathy", attributeDict["Empathy"]);

                ImGui::TableNextRow();

                static int might = MIN_LEVEL;
                static int stealth = MIN_LEVEL;
                static int scouting = MIN_LEVEL;
                static int manipulation = MIN_LEVEL;

                AddAttributeOrSkill("might", skillDict["might"], "stealth", skillDict["stealth"], "scouting", skillDict["scouting"], "manipulation", skillDict["manipulation"]);
                ImGui::TableNextRow();

                static int endurance = MIN_LEVEL;
                static int sleightofhands = MIN_LEVEL;
                static int lore = MIN_LEVEL;
                static int performance = MIN_LEVEL;

                AddAttributeOrSkill("endurance", skillDict["endurance"], "sleightofhands", skillDict["sleightofhands"], "lore", skillDict["lore"], "performance", skillDict["performance"]);
                ImGui::TableNextRow();

                static int melee = MIN_LEVEL;
                static int move = MIN_LEVEL;
                static int survival = MIN_LEVEL;
                static int healing = MIN_LEVEL;

                AddAttributeOrSkill("melee", skillDict["melee"], "move", skillDict["move"], "survival", skillDict["survival"], "healing", skillDict["healing"]);

                ImGui::TableNextRow();

                static int crafting = MIN_LEVEL;
                static int markmanship = MIN_LEVEL;
                static int insight = MIN_LEVEL;
                static int animalhandling = MIN_LEVEL;

                AddAttributeOrSkill("crafting", skillDict["crafting"], "markmanship", skillDict["markmanship"], "insight", skillDict["insight"], "animalhandling", skillDict["animalhandling"]);

                ImGui::EndTable();
            

            // TALENT SECTION
            ImGui::BeginTable("Talents", 2);

            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("General");

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("Class");

            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);

            ImGui::BeginTable("General talents", 2);

            /*
            int talentsLevel[IM_ARRAYSIZE(generalTalentsList)]; // create a new array

            for (int i = 0; i < IM_ARRAYSIZE(generalTalentsList); i++)
            {
                talentsLevel[i] = MIN_LEVEL;
                AddTalentSlider(generalTalentsList[i], talentsLevel[i]);
            }
            */
            
            static int ambidextrous = MIN_LEVEL;
            AddTalentSlider("Ambidextrous", skillDict["ambidextrous"]);

            static int axefighter = MIN_LEVEL;
            AddTalentSlider("Axe Fighter", skillDict["axefighter"]);

            static int berserk = MIN_LEVEL;
            AddTalentSlider("Berserk", skillDict["berserk"]);

            static int bowyer = MIN_LEVEL;
            AddTalentSlider("Bowyer", skillDict["bowyer"]);

            static int builder = MIN_LEVEL;
            AddTalentSlider("Builder", skillDict["builder"]);

            static int chef = MIN_LEVEL;
            AddTalentSlider("Chef", skillDict["chef"]);

            static int coldblooded = MIN_LEVEL;
            AddTalentSlider("Cold Blooded", skillDict["coldblooded"]);

            static int defender = MIN_LEVEL;
            AddTalentSlider("Defender", skillDict["defender"]);

            static int dragonslayer = MIN_LEVEL;
            AddTalentSlider("Dragon Slayer", skillDict["dragonslayer"]);

            static int executioner = MIN_LEVEL;
            AddTalentSlider("Executioner", skillDict["executioner"]);

            static int fastfootwork = MIN_LEVEL;
            AddTalentSlider("Fast Footwork", skillDict["fastfootwork"]);

            static int fastshooter = MIN_LEVEL;
            AddTalentSlider("Fast Shooter", skillDict["fastshooter"]);

            static int firmgrip = MIN_LEVEL;
            AddTalentSlider("Firm Grip", skillDict["firmgrip"]);

            static int fisher = MIN_LEVEL;
            AddTalentSlider("Fisher", skillDict["fisher"]);

            static int hammerfighter = MIN_LEVEL;
            AddTalentSlider("Hammer Fighter", skillDict["hammerfighter"]);

            static int herbalist = MIN_LEVEL;
            AddTalentSlider("Herbalist", skillDict["herbalist"]);

            static int horsebackfighter = MIN_LEVEL;
            AddTalentSlider("Horseback Fighter", skillDict["horsebackfighter"]);

            static int incorruptible = MIN_LEVEL;
            AddTalentSlider("Incorruptible", skillDict["incorruptible"]);

            static int knifefighter = MIN_LEVEL;
            AddTalentSlider("Knife Fighter", skillDict["knifefighter"]);

            static int lockpicker = MIN_LEVEL;
            AddTalentSlider("Lockpicker", skillDict["lockpicker"]);

            static int lucky = MIN_LEVEL;
            AddTalentSlider("Lucky", skillDict["lucky"]);

            static int masterofthehunt = MIN_LEVEL;
            AddTalentSlider("Master of the Hunt", skillDict["masterofthehunt"]);

            static int meleecharge = MIN_LEVEL;
            AddTalentSlider("Melee Charge", skillDict["meleecharge"]);

            static int packrat = MIN_LEVEL;
            AddTalentSlider("Pack Rat", skillDict["packrat"]);

            static int painresistant = MIN_LEVEL;
            AddTalentSlider("Pain Resistant", skillDict["painresistant"]);

            static int pathfinder = MIN_LEVEL;
            AddTalentSlider("Pathfinder", skillDict["pathfinder"]);

            static int poisoner = MIN_LEVEL;
            AddTalentSlider("Poisoner", skillDict["poisoner"]);

            static int quartermaster = MIN_LEVEL;
            AddTalentSlider("Quartermaster", skillDict["quartermaster"]);

            static int quickdraw = MIN_LEVEL;
            AddTalentSlider("Quickdraw", skillDict["quickdraw"]);

            static int sailor = MIN_LEVEL;
            AddTalentSlider("Sailor", skillDict["sailor"]);

            static int sharpshooter = MIN_LEVEL;
            AddTalentSlider("Sharpshooter", skillDict["sharpshooter"]);

            static int sharptongue = MIN_LEVEL;
            AddTalentSlider("Sharp Tongue", skillDict["sharptongue"]);

            static int shieldfighter = MIN_LEVEL;
            AddTalentSlider("Shield Fighter", skillDict["shieldfighter"]);

            static int sixthsense = MIN_LEVEL;
            AddTalentSlider("Sixth Sense", skillDict["sixthsense"]);

            static int smith = MIN_LEVEL;
            AddTalentSlider("Smith", skillDict["smith"]);

            static int spearfighter = MIN_LEVEL;
            AddTalentSlider("Spear Fighter", skillDict["spearfighter"]);

            static int steadyfeet = MIN_LEVEL;
            AddTalentSlider("Steady Feet", skillDict["steadyfeet"]);

            static int swordfighter = MIN_LEVEL;
            AddTalentSlider("Sword Fighter", skillDict["swordfighter"]);

            static int tailor = MIN_LEVEL;
            AddTalentSlider("Tailor", skillDict["tailor"]);

            static int tanner = MIN_LEVEL;
            AddTalentSlider("Tanner", skillDict["tanner"]);

            static int threatening = MIN_LEVEL;
            AddTalentSlider("Threatening", skillDict["threatening"]);

            static int throwingarm = MIN_LEVEL;
            AddTalentSlider("Throwing Arm", skillDict["throwingarm"]);

            static int wanderer = MIN_LEVEL;
            AddTalentSlider("Wanderer", skillDict["wanderer"]);

            ImGui::EndTable();

            ImGui::TableSetColumnIndex(1);

            ImGui::BeginTable("Class talents ", 1);

            static int pathofhealing = MIN_LEVEL;
            static int pathofshiftingshapes = MIN_LEVEL;
            static int pathofsight = MIN_LEVEL;

            AddClass("Druid", "Druid's talents", "Path of the Healing", "Path of the Shifting Shapes", "Path of the Sight", classTalentsDict["pathofhealing"], classTalentsDict["pathofshiftingshapes"], classTalentsDict["pathofsight"]);

            static int pathofblade = MIN_LEVEL;
            static int pathofenemy = MIN_LEVEL;
            static int pathofshield = MIN_LEVEL;

            AddClass("Fighter", "Fighter's talents", "Path of the Blade", "Path of the Enemy", "Path of the Shield", classTalentsDict["pathofblade"], classTalentsDict["pathofenemy"], classTalentsDict["pathofshield"]);

            static int pathofarrow = MIN_LEVEL;
            static int pathofbeast = MIN_LEVEL;
            static int pathofforest = MIN_LEVEL;

            AddClass("Hunter", "Hunter's talents", "Path of the Arrow", "Path of the Beast", "Path of the Forest", classTalentsDict["pathofarrow"], classTalentsDict["pathofbeast"], classTalentsDict["pathofforest"]);

            static int pathofthehymn = MIN_LEVEL;
            static int pathofsong = MIN_LEVEL;
            static int pathofwarcry = MIN_LEVEL;

            AddClass("Ministrel", "Ministrel's talents", "Path of the Hymn", "Path of the Song", "Path of the Warcry", classTalentsDict["pathofthehymn"], classTalentsDict["pathofsong"], classTalentsDict["pathofwarcry"]);

            static int pathofthegold = MIN_LEVEL;
            static int pathoflies = MIN_LEVEL;
            static int pathofmanythings = MIN_LEVEL;

            AddClass("Peddler", "Peddler's talents", "Path of the Gold", "Path of the Lies", "Path of the Many Things", classTalentsDict["pathofthegold"], classTalentsDict["pathoflies"], classTalentsDict["pathofmanythings"]);

            static int pathofthecompanion = MIN_LEVEL;
            static int pathofknight = MIN_LEVEL;
            static int pathofplains = MIN_LEVEL;

            AddClass("Rider", "Rider's talents", "Path of the Companion", "Path of the Knight", "Path of the Plains", classTalentsDict["pathofthecompanion"], classTalentsDict["pathofknight"], classTalentsDict["pathofplains"]);

            static int pathoftheface = MIN_LEVEL;
            static int pathofkiller = MIN_LEVEL;
            static int pathofpoison = MIN_LEVEL;

            AddClass("Rogue", "Rogue's talents", "Path of the Face", "Path of the Killer", "Path of Poison", classTalentsDict["pathoftheface"], classTalentsDict["pathofkiller"], classTalentsDict["pathofpoison"]);

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            if (ImGui::CollapsingHeader("Sorcerer", ImGuiTreeNodeFlags_None))
            {
                ImGui::BeginTable("Sorcerer's talents", 2);

                static int pathofblood = MIN_LEVEL;
                static int pathofdeath = MIN_LEVEL;
                static int pathofsigns = MIN_LEVEL;
                static int pathofstone = MIN_LEVEL;

                AddTalentSlider("Path of Blood", classTalentsDict["pathofblood"]);

                AddTalentSlider("Path of Death", classTalentsDict["pathofdeath"]);

                AddTalentSlider("Path of Signs", classTalentsDict["pathofsigns"]);

                AddTalentSlider("Path of Stone", classTalentsDict["pathofstone"]);

                ImGui::EndTable();
            }

            ImGui::EndTable();

            ImGui::EndTable();

            ImGui::Text("What do you want to level up?");

            const char* items[] = { "Attribute", "Skill", "General Talent", "Class Talent"};
            static int item_current = 0;

            ImGui::Combo("##combo", &item_current, items, IM_ARRAYSIZE(items));

            static int choosenAttribute = 0;
            static int choosenSkill = 0;
            static int choosenGeneralTalent = 0;
            static int choosenClassTalent = 0;

            if (item_current == 0)
            {
                ImGui::Combo("##combo1", &choosenAttribute, attributeList, IM_ARRAYSIZE(attributeList));
            }
            if (item_current == 1)
            {
                ImGui::Combo("##combo1", &choosenSkill, skillList, IM_ARRAYSIZE(skillList));
            }
            if (item_current == 2)
            {
                ImGui::Combo("##combo2", &choosenGeneralTalent, generalTalentsList, IM_ARRAYSIZE(generalTalentsList));
            }
            if (item_current == 3)
            {
                ImGui::Combo("##combo3", &choosenClassTalent, classTalentsList, IM_ARRAYSIZE(classTalentsList));
            }

            static int clicked = 0;
            if (ImGui::Button("Button"))
                clicked++;

            int cost = 5;

            ImGui::Text("You gonna need:");
            if (clicked & 1)
            {

            }


            for (const auto& pair : attributeDict) {
                ImGui::Text(pair.first.c_str());
                ImGui::Text(std::to_string(pair.second).c_str());
            
            }

            //ImGui::Text("%d", attributeDict["agility"]);



            ImGui::End();
        }


        //ImGui::ShowDemoWindow(&show_demo_window);

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
