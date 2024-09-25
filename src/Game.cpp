//
// Created by tronxi on 25/9/24.
//

#include "Game.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>


Game::Game() : isRunning(true),
               ballPosition(),
               ballVelocity(),
               paddlePosition(),
               paddleSize(50),
               thickness(10),
               ticksCount(0),
               paddleDir(0),
               fails(0),
               window(nullptr),
               renderer(nullptr),
               font() {
    ballPosition.x = 320;
    ballPosition.y = 240;

    ballVelocity.x = -200;
    ballVelocity.y = -235;

    paddlePosition.x = 10;
    paddlePosition.y = 240;
}


bool Game::init() {
    const int sdl_result = SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    if (sdl_result != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("Basic Game", 300, 1000, 640, 480, 0);
    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        return false;
    }
    font = TTF_OpenFont("resources/OpenSans-Regular.ttf", 18);
    SDL_Surface *iconSurface = IMG_Load("resources/appicon.jpg");

    SDL_SetWindowIcon(window, iconSurface);

    SDL_FreeSurface(iconSurface);
    return true;
}

void Game::run() {
    while (isRunning) {
        processInput();
        update();
        render();
    }
}

void Game::processInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                return;
        }
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE]) {
        isRunning = false;
    }
    if (state[SDL_SCANCODE_A]) {
        ballPosition.x--;
    }
    if (state[SDL_SCANCODE_D]) {
        ballPosition.x++;
    }
    paddleDir = 0;
    if (state[SDL_SCANCODE_W]) {
        paddleDir = -1;
    }
    if (state[SDL_SCANCODE_S]) {
        paddleDir = 1;
    }
}

void Game::update() {
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksCount + 16)) {
    }
    
    float deltaTime = (SDL_GetTicks() - ticksCount) / 1000.0f;
    ticksCount = SDL_GetTicks();


    if (paddleDir != 0) {
        paddlePosition.y += paddleDir * 300.0f * deltaTime;

        if (paddlePosition.y < (paddleSize / 2.0f + thickness)) {
            paddlePosition.y = paddleSize / 2.0f + thickness;
        } else if (paddlePosition.y > (480.0f - paddleSize / 2.0f - thickness)) {
            paddlePosition.y = 480.0f - paddleSize / 2.0f - thickness;
        }
    }
    ballPosition.x += ballVelocity.x * deltaTime;
    ballPosition.y += ballVelocity.y * deltaTime;

    if (ballPosition.y < thickness) {
        ballVelocity.y *= -1;
    } else if (ballPosition.y > 480.0f) {
        ballVelocity.y *= -1;
    }

    float diffY = paddlePosition.y - ballPosition.y;
    diffY = diffY > 0.0f ? diffY : -diffY;


    float diffX = ballPosition.x - paddlePosition.x;
    diffX = diffX > 0.0f ? diffX : -diffX;
    if (diffY < paddleSize / 2 && diffX < thickness && ballVelocity.x < 0.0f) {
        ballVelocity.x *= -1;
    }

    if (ballPosition.x > 640) {
        ballVelocity.x *= -1;
    }

    if (ballPosition.x < 0) {
        ballPosition.x = 400;
        fails++;
    }
}


void Game::render() const {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect wall = {0, 0, 640, thickness};
    SDL_RenderFillRect(renderer, &wall);
    wall.y = 480 - thickness;
    SDL_RenderFillRect(renderer, &wall);
    const SDL_Rect ball = {static_cast<int>(ballPosition.x), static_cast<int>(ballPosition.y), thickness, thickness};
    SDL_RenderFillRect(renderer, &ball);

    const SDL_Rect paddle = {
        static_cast<int>(paddlePosition.x),
        static_cast<int>(paddlePosition.y - paddleSize / 2),
        thickness,
        paddleSize
    };
    SDL_RenderFillRect(renderer, &paddle);

    const SDL_Color white = {255, 255, 255};
    SDL_Surface *text = TTF_RenderText_Solid(font, std::to_string(fails).c_str(), white);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, text);

    const SDL_Rect textRect = {50, 50, 20, 20};
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);


    SDL_RenderPresent(renderer);
}


void Game::shutdown() const {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}

