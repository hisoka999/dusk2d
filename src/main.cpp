#include <chrono>
#include <engine/core/SceneManager.h>
#include <engine/core/gamewindow.h>
#include <engine/core/input.h>
#include <iostream>
#include "config.h"
#include "engine/core/renderer.h"
#include "engine/graphics/TextureManager.h"
#include "engine/graphics/text.h"
#include "engine/graphics/texture.h"
#include "engine/utils/exceptions.h"
#include "engine/utils/localisation.h"
#include "engine/utils/logger.h"
#include "engine/utils/os.h"
#include "engine/utils/string.h"
#include "game/services/ItemRecipeService.h"
#include "game/services/ItemService.h"
#include "scenes/MainScene.h"
#include "scenes/WorldScene.h"

#include <magic_enum.hpp>

#ifdef GAME_DEBUG
template<size_t SIZE>
void writeEnumArray(std::array<std::string_view, SIZE> names, std::ofstream &stream)
{
    for (auto name: names)
    {
        stream << "msgid \"" << name << "\"\n";
        stream << "msgstr \"\"\n\n";
    }
}

void generateEnumPot(std::string fileName)
{
    std::ofstream os(fileName, std::ios::trunc | std::ios::out);

    //    writeEnumArray(magic_enum::enum_names<world::BuildingType>(), os);

    os.flush();
    os.close();
}

void writeKeyMapPot(core::KeyMap &keyMap, std::string fileName)
{
    std::ofstream os(fileName, std::ios::trunc | std::ios::out);

    std::vector<std::string> keyList;
    for (auto &[key, value]: keyMap)
    {
        if (std::find(keyList.begin(), keyList.end(), key) == keyList.end())
        {
            keyList.emplace_back(key);
        }
    }
    for (auto &key: keyList)
    {
        os << "msgid \"" << key << "\"\n";
        os << "msgstr \"\"\n\n";
    }

    os.flush();
    os.close();
}

#endif

core::KeyMap initKeyMap(utils::IniBase *settings)
{

    core::KeyMap keyMap;
    keyMap.emplace("MOVE_LEFT", settings->getValueI("keys", "MOVE_LEFT_1", SDLK_a));
    keyMap.emplace("MOVE_LEFT", settings->getValueI("keys", "MOVE_LEFT_2", SDLK_LEFT));

    keyMap.emplace("MOVE_RIGHT", settings->getValueI("keys", "MOVE_RIGHT_1", SDLK_d));
    keyMap.emplace("MOVE_RIGHT", settings->getValueI("keys", "MOVE_RIGHT_2", SDLK_RIGHT));

    keyMap.emplace("MOVE_UP", settings->getValueI("keys", "MOVE_UP_1", SDLK_w));
    keyMap.emplace("MOVE_UP", settings->getValueI("keys", "MOVE_UP_2", SDLK_UP));

    keyMap.emplace("MOVE_DOWN", settings->getValueI("keys", "MOVE_DOWN_1", SDLK_s));
    keyMap.emplace("MOVE_DOWN", settings->getValueI("keys", "MOVE_DOWN_2", SDLK_DOWN));

    keyMap.emplace("MENU", settings->getValueI("keys", "MENU_1", SDLK_ESCAPE));
    keyMap.emplace("MENU", settings->getValueI("keys", "MENU_2", 0));
    keyMap.emplace("CONSOLE", settings->getValueI("keys", "CONSOLE_1", SDLK_c));
    keyMap.emplace("CONSOLE", settings->getValueI("keys", "CONSOLE_2", 0));
    keyMap.emplace("RESEARCH", settings->getValueI("keys", "RESEARCH_1", SDLK_r));
    keyMap.emplace("RESEARCH", settings->getValueI("keys", "RESEARCH_2", 0));
    keyMap.emplace("INPUT", settings->getValueI("keys", "INPUT_1", SDLK_RETURN));
    keyMap.emplace("INPUT", settings->getValueI("keys", "INPUT_2", SDLK_e));

    return keyMap;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    if (argc == 2)
    {
        std::string arg = std::string(argv[1]);
        if (arg == "--version")
        {
            std::cout << GAME_VERSION_MAJOR << "." << GAME_VERSION_MINOR << std::endl;
            return 0;
        }
    }

    try
    {
#ifdef GAME_DEBUG
        std::string fileName = "locale/enum.pot";
        generateEnumPot(fileName);
#endif

        std::filesystem::path loggingFolder = std::filesystem::path(utils::os::get_pref_dir("", "Dusk2D")) / "logs";
        g_appLogger.init(loggingFolder, utils::LogLevel::trace);
        g_sglLogger.init(loggingFolder, utils::LogLevel::trace);

        core::GameWindow win(utils::string_format("Dusk2D %d.%d", GAME_VERSION_MAJOR, GAME_VERSION_MINOR), 1280, 720,
                             "Dusk2D");
        win.setWindowIcon("logo.png");

        auto keyMap = initKeyMap(win.getSettings().get());
        core::Input input(keyMap);

#ifdef GAME_DEBUG
        writeKeyMapPot(keyMap, "locale/keymap.pot");
#endif

        auto lang = win.getSettings()->getValue("Base", "Lang");
        // if (!lang.empty())
        // {
        //     Language lng = magic_enum::enum_cast<Language>(lang).value();
        //     Localisation::Instance().loadLanguage(lng, "Dusk2D");
        //     Localisation::Instance().loadLanguage(lng, "enum");
        //     Localisation::Instance().loadLanguage(lng, "keymap");
        // }
        // else
        // {
        //     Localisation::Instance().detectLanguage("Dusk2D");
        //     Localisation::Instance().detectLanguage("enum");
        //     Localisation::Instance().detectLanguage("keymap");
        // }
        setlocale(LC_ALL, Localisation::Instance().getLocale().name().c_str());

        services::ItemRecipeService::Instance().loadData("data/item_recipes.json");
        services::ItemService::Instance().loadData("data/items.json");

        core::Renderer ren;
        graphics::TextureManager::Instance().setRenderer(&ren);
        graphics::TextureManager::Instance().loadTheme("data/theme.json", true);

        auto &sceneManager = core::SceneManager::Instance();

        ren.open(&win, win.getSettings()->getValueB("Base", "VSync"));
        graphics::Rect viewPort = ren.getViewPort();
        core::Camera mainCamera(viewPort);
        ren.setMainCamera(&mainCamera);

        auto mainScene = std::make_shared<scenes::MainScene>(&ren, &sceneManager, win.getSettings(), &input);
        auto worldScene = std::make_shared<scenes::WorldScene>(&ren);
        mainScene->setGameWindow(&win);
        sceneManager.addScene("main", mainScene);
        sceneManager.addScene("world", worldScene);
        sceneManager.setCurrentScene("main");

        unsigned int lastTime = ren.getTickCount();
        unsigned int lastUpdateTime = ren.getTickCount();

        unsigned int frames = 0;

        unsigned int fps = 0;
        graphics::Text text;
        text.openFont(utils::os::combine("fonts", "arial.ttf"), 22);
        SDL_Color color = {200, 200, 0, 0};

        bool run = true;
        lastUpdateTime = 0;
        while (run && mainScene->isRunning())
        {
            ren.setDrawColor(0, 0, 0, 255);
            ren.clear();
            bool saveScreenshot = false;

            while (input.poll())
            {
                if (input.isQuit())
                    run = false;

                else if (input.isKeyDown(SDLK_p))
                {
                    saveScreenshot = true;
                }

                sceneManager.handleEvents(&input);
            }
            sceneManager.render();
            frames++;
            if ((ren.getTickCount() - lastTime) >= 1000)
            {
                lastTime = ren.getTickCount();
                fps = frames;
                frames = 0;
            }
            lastUpdateTime += ren.getTimeDelta();
            if (lastUpdateTime >= 40)
            {

                sceneManager.fixedUpdate(40);
                lastUpdateTime -= 40;
            }
            sceneManager.update();

            int textPosX = mainCamera.getWidth() - 120;
            int textPosY = mainCamera.getHeight() - 50;

            text.render(&ren, "FPS: " + std::to_string(fps), color, textPosX, textPosY);
            text.render(&ren, "FT: " + std::to_string(ren.getTimeDelta()) + "ms", color, textPosX, textPosY + 20);
            ren.renderPresent();
            if (saveScreenshot)
            {

                SDL_Surface *surf = SDL_CreateRGBSurface(0, win.getWidth(), win.getHeight(), 32, 0x00ff0000, 0x0000ff00,
                                                         0x000000ff, 0xff000000);
                SDL_RenderReadPixels(ren.getRenderer(), NULL, SDL_PIXELFORMAT_ARGB8888, surf->pixels, surf->pitch);

                if (surf != 0)
                {
                    std::string fileName = utils::os::get_pref_dir("", "dusk2d") + "screenshot_" +
                                           std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) +
                                           ".png";
                    std::cout << "screenshot: " << fileName << std::endl;
                    IMG_SavePNG(surf, fileName.c_str());
                    SDL_FreeSurface(surf);
                }
                else
                {
                    throw SDLException("SDL_GetWindowSurface");
                }
            }

            // win.delay(delay);

            ren.calcDelta();
        }
        sceneManager.freeScenes();
        graphics::TextureManager::Instance().clear();
    }
    catch (SDLException &e)
    {
        std::cerr << "SDL Exception: " << e.what() << std::endl;
        return 1;
    }
    // catch (std::exception &e)
    // {
    //     std::cerr << "unkown standard exception: " << e.what() << std::endl;
    //     return 1;
    // }
    return 0;
}
