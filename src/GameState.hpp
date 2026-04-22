#pragma once

enum class GameState {
    MENU,           // Main menu
    PLAYING,        // Game is active
    GAME_OVER,      // Game over screen
    ABOUT,          // About screen
    COUNTDOWN       // Countdown before game starts
};