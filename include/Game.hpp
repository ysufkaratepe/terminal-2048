#pragma once

#include <string>

#include "Board.hpp"

// Oyun döngüsü: tuşları okur, tahtayı günceller, ekranı çizer, rekoru saklar.
class Game {
public:
    explicit Game(std::string highScoreFile = "2048_rekor.txt");
    void run();

private:
    void tryMove(Direction dir);
    void undo();
    void restart();
    void render() const;

    void loadHighScore();
    void saveHighScore() const;

    Board board_;
    Board previous_;        // tek adım geri almak için önceki durum
    bool canUndo_ = false;
    bool winShown_ = false; // "Kazandın" mesajı sadece bir kez gösterilsin
    int highScore_ = 0;
    std::string message_;
    std::string highScoreFile_;
};
