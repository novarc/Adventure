/*
 * PlatformInterface.cpp
 */

#include "stdinclude.hpp"
#include "GeometricPrimitives.hpp"
#include "Elements.hpp"
#include "PlatformInterface.hpp"

#include <chrono>
#include <random>

#include <SDL2/SDL.h>

class SystemImpl : public System
{
public:
    SystemImpl() : gameMain(nullptr), renderWindow(nullptr), renderContext(nullptr), isRunning(false),
                   rng(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())) { }

    ~SystemImpl() { }

    shared_ptr<Tex> loadTex(const string file) {
        // TODO
        return nullptr;
//        if( fileTexMap.find(file) == fileTexMap.end() ) {
//            // Load the texture
//            sf::Image image;
//            if( !image.loadFromFile(file) )
//                throw invalid_argument("Unable to open file: " + file);
//
//            sf::Texture* tex = new sf::Texture;
//            if( !tex->loadFromImage(image) )
//                throw runtime_error("Loading image from texture " + file + " failed.");
//
//            fileTexMap[file] = tex;
//        }
//
//        sf::Texture* tex = fileTexMap[file];
//
//        sf::Vector2u sz = tex->getSize();
//        xy size( static_cast<float>( sz.x ),
//                 static_cast<float>( sz.y ) );
//
//        return shared_ptr<Tex>( new TexImpl(tex, size) );
    }

    void drawImage(const Tex &tex, xy pos, bool horizontalFlip=false, float angle=0.0f) {
//        // TODO
//        sf::Sprite sprite( *(dynamic_cast<const TexImpl&>(tex).tex) );
//        sprite.setPosition( pos.x + (horizontalFlip ? tex.getSize().x : 0) ,
//                            windowProperties.size.y - tex.getSize().y - pos.y );
//        sprite.setRotation(angle);
//        if(horizontalFlip) {
//            sprite.setScale(-1, 1);
//        }
//        renderWindow->draw(sprite);
    }

    void drawText(string line, xy pos, Color color=Color(), float fontSize=15.0f) {
//        // TODO
//        sf::Text textToDraw(line, defaultFont, static_cast<unsigned int>(fontSize));
//        textToDraw.setColor(sf::Color(color.r, color.g, color.b, color.a));
//        textToDraw.setPosition( static_cast<float>( pos.x ), static_cast<float>( pos.y ) );
//        renderWindow->draw(textToDraw);
    }

    void drawBox(xy pos, xy size, Color fillColor, Color outlineColor, float outlineThickness) {
        cout<<"x";
        SDL_Rect rect = {pos.x, pos.y, size.x, size.y};

        SDL_SetRenderDrawColor(renderContext, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
        SDL_RenderDrawRect(renderContext, &rect);

        SDL_SetRenderDrawColor(renderContext, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
        SDL_Rect inner_rect = getInnerRect(rect);
        SDL_RenderFillRect(renderContext, &inner_rect);
//        sf::RectangleShape rectangle;
//        rectangle.setPosition ( sf::Vector2f( pos.x,  windowProperties.size.y - pos.y) );
//        rectangle.setSize     ( sf::Vector2f( size.x, -size.y ) );
//        rectangle.setFillColor( sf::Color( fillColor.r, fillColor.g, fillColor.b, fillColor.a) );
//        rectangle.setOutlineThickness( outlineThickness );
//        rectangle.setOutlineColor( sf::Color(outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a) );
//        renderWindow->draw(rectangle);
    }

    void setMouseCursorVisibility(bool visibility) {
//        TODO
//        renderWindow->setMouseCursorVisible( visibility );
    }

    virtual void getInput(InputCallbacks *callbacks) {
//        TODO
//    	if( sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) )
//    		callbacks->escKey();
//        if( sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) )
//            callbacks->upKey();
//        if( sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A) )
//            callbacks->leftKey();
//        if( sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) )
//            callbacks->rightKey();
//        if( sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S) )
//            callbacks->downKey();
    }

    unsigned int random() {
        return static_cast<unsigned int>( rng() );
    }

    void exit() {
        isRunning = false;
    }

private:
    friend int main(int argc, char *argv[]);
    friend System* Sys();

    static SystemImpl* singleton;

    map<const string, SDL_Texture*> fileTexMap;

    class TexImpl : public Tex
    {
    public:
        SDL_Texture* tex;

        TexImpl(SDL_Texture* tex, xy size) :
            tex(tex), size(size) {}

        const xy getSize() const {
            return size;
        }

    private:
        xy size;
    };

    unique_ptr<GameMain> gameMain;

    SDL_Window* renderWindow;
    SDL_Renderer* renderContext;
    bool isRunning;

    std::minstd_rand0 rng;

    void handleCmdlineArgs(int argc, char *argv[])
    {
        if( argc == 2 ) {
            if( !strcmp(argv[1], "-f") )
                windowProperties.fullscreen = true;
        }
        else if( argc == 4 && !strcmp(argv[1], "-r") ) {
            windowProperties.size = xy(atoi(argv[2]), atoi(argv[3]) );
        }
    }

    void createWindowAndContext()
    {
        if(windowProperties.fullscreen) {
            // TODO
            throw invalid_argument("Unimplemented");
//            sf::VideoMode best_mode = sf::VideoMode::getFullscreenModes()[0];
//            windowProperties.size.x = static_cast<float>( best_mode.width );
//            windowProperties.size.y = static_cast<float>( best_mode.height );
//            sf::RenderWindow *renderWindow = new sf::RenderWindow(
//            		best_mode, windowProperties.title, sf::Style::Fullscreen);
//            renderWindow->setVerticalSyncEnabled(true);
//            return renderWindow;
        }
        else {
            renderWindow = SDL_CreateWindow(windowProperties.title.c_str(),
                                160, 100, // TODO
                                static_cast<int>( windowProperties.size.x ),
                                static_cast<int>( windowProperties.size.y ),
                                SDL_WINDOW_SHOWN);
            if (renderWindow == nullptr){
                throw invalid_argument(string("SDL_CreateWindow Error: ") + SDL_GetError());
            }
        }

        SDL_Renderer* sdl_renderer = SDL_CreateRenderer(renderWindow, -1,
                                         SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (sdl_renderer == nullptr){
            SDL_DestroyWindow(renderWindow);
            throw invalid_argument(string("SDL_CreateRenderer Error: ") + SDL_GetError());
        }
    }

//    string getFPS(int currentTime)
//    {
//        static const int updateFrequency = 10;
//        static int lastUpdate = 0,
//                lastFrameTime = 0,
//                duration = 0, fps = 0;
//        duration = currentTime - lastFrameTime;
//        if(duration && !lastUpdate) {
//            fps = 1000 / duration;
//            lastUpdate = updateFrequency;
//        }
//        lastFrameTime = currentTime;
//        lastUpdate--;
//
//        stringstream s;
//        s<<fps;
//        return s.str();
//    }

    static SDL_Rect getInnerRect(const SDL_Rect &rect) {
        SDL_Rect innerRect;
        innerRect.x = rect.x + 1;
        innerRect.y = rect.y + 1;
        innerRect.w = rect.w - 2;
        innerRect.h = rect.h - 2;
        return innerRect;
    }

    int platformMain(int argc, char *argv[])
    {
        windowProperties = GameMain::defaultWindowProperties();

        handleCmdlineArgs(argc, argv);

        /*
         * Platform Setup
         */
        try {
            createWindowAndContext();
        }
        catch(exception &e) {
            cerr<<"Exception caught: "<<e.what()<<endl;
            SDL_Quit();
            return EXIT_FAILURE;
        }

        /*
         * Game Loop
         */
        try
        {
            gameMain = unique_ptr<GameMain>( GameMain::getSingleton() );
            isRunning = true;

            while(isRunning)
            {
                SDL_Event sdl_event;
                while( SDL_PollEvent(&sdl_event) )
                {
                    if( sdl_event.type == SDL_QUIT ) {
                        isRunning = false; break;
                    }
                    else if (sdl_event.type == SDL_KEYDOWN && sdl_event.key.keysym.sym == SDLK_ESCAPE) {
                        isRunning = false; break;
                    }
                }

                SDL_RenderClear(renderContext);

                gameMain->step();

                SDL_RenderPresent(renderContext);
            }

            SDL_DestroyRenderer(renderContext);
            SDL_DestroyWindow(renderWindow);
            SDL_Quit();
            return EXIT_SUCCESS;
        }
        catch(exception &e)
        {
            cerr<<"Exception caught: "<<e.what()<<endl;

            SDL_DestroyRenderer(renderContext);
            SDL_DestroyWindow(renderWindow);
            SDL_Quit();
            return EXIT_FAILURE;
        }
    }
};

SystemImpl* SystemImpl::singleton = nullptr;

System* Sys() {
    return SystemImpl::singleton;
}

int main(int argc, char *argv[]) {
    SystemImpl *sys = SystemImpl::singleton = new SystemImpl();
    int retCode = sys->platformMain(argc, argv);
    delete SystemImpl::singleton;
    return retCode;
}
