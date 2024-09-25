//
// Created by tronxi on 25/9/24.
//

#ifndef GAME_H
#define GAME_H
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <SDL_video.h>

struct Vector2 {
    float x;
    float y;
};

class Game {
public:
    Game();

    bool init();

    void run();

    void shutdown() const;

private:
    void processInput();

    void update();

    void render() const;

    bool isRunning;
    Vector2 ballPosition;
    Vector2 ballVelocity;
    Vector2 paddlePosition;
    int paddleSize;
    int thickness;
    Uint32 ticksCount;
    int paddleDir;
    int fails;

    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
};


#endif //GAME_H
