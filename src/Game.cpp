#include "Game.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

#include "Terminal.hpp"

namespace {

constexpr int CELL_WIDTH = 6;

struct TileColor {
    int value;
    int background;  // 256 renk paletinden
    int foreground;
};

// Klasik 2048 renklerine yakın bir palet
constexpr TileColor COLORS[] = {
    {2, 230, 238},    {4, 229, 238},    {8, 215, 255},    {16, 209, 255},
    {32, 203, 255},   {64, 196, 255},   {128, 228, 238},  {256, 227, 238},
    {512, 226, 238},  {1024, 220, 238}, {2048, 214, 255},
};

std::string colorize(int value, const std::string& text) {
    int bg = 129, fg = 255;  // 2048'den büyük taşlar mor
    for (const TileColor& c : COLORS) {
        if (c.value == value) {
            bg = c.background;
            fg = c.foreground;
            break;
        }
    }
    std::ostringstream out;
    out << "\033[1;48;5;" << bg << ";38;5;" << fg << "m" << text << "\033[0m";
    return out.str();
}

std::string centered(const std::string& text, int width) {
    int total = width - static_cast<int>(text.size());
    if (total <= 0) return text;
    int left = total / 2;
    return std::string(left, ' ') + text + std::string(total - left, ' ');
}

std::string horizontalLine(const char* left, const char* middle, const char* right) {
    std::string line = "  ";
    line += left;
    for (int c = 0; c < Board::SIZE; c++) {
        for (int i = 0; i < CELL_WIDTH; i++) line += "─";
        line += (c + 1 < Board::SIZE) ? middle : right;
    }
    return line;
}

}  // namespace

Game::Game(std::string highScoreFile) : highScoreFile_(std::move(highScoreFile)) {
    loadHighScore();
}

void Game::run() {
    terminal::init();

    bool running = true;
    while (running) {
        render();
        message_.clear();

        switch (terminal::readKey()) {
            case terminal::Key::Up:      tryMove(Direction::Up); break;
            case terminal::Key::Down:    tryMove(Direction::Down); break;
            case terminal::Key::Left:    tryMove(Direction::Left); break;
            case terminal::Key::Right:   tryMove(Direction::Right); break;
            case terminal::Key::Undo:    undo(); break;
            case terminal::Key::Restart: restart(); break;
            case terminal::Key::Quit:    running = false; break;
            case terminal::Key::Other:   break;
        }
    }

    saveHighScore();
    terminal::clear();
    terminal::restore();
    std::cout << "Oyun bitti. Skorun: " << board_.score()
              << "  |  Rekor: " << highScore_ << "\n";
}

void Game::tryMove(Direction dir) {
    if (!board_.canMove()) return;

    Board snapshot = board_;
    if (!board_.move(dir)) return;  // tahta değişmediyse hamle sayılmaz

    previous_ = snapshot;
    canUndo_ = true;

    if (board_.score() > highScore_) {
        highScore_ = board_.score();
        saveHighScore();
    }

    if (!winShown_ && board_.hasWon()) {
        winShown_ = true;
        message_ = "Tebrikler, 2048'e ulaştın! İstersen devam edebilirsin.";
    }
}

void Game::undo() {
    if (!canUndo_) {
        message_ = "Geri alınacak hamle yok.";
        return;
    }
    board_ = previous_;
    canUndo_ = false;
    message_ = "Son hamle geri alındı.";
}

void Game::restart() {
    board_.reset();
    canUndo_ = false;
    winShown_ = false;
    message_ = "Yeni oyun başladı.";
}

void Game::render() const {
    std::ostringstream out;

    out << "\n  \033[1;38;5;214m2048\033[0m"
        << "    Skor: \033[1m" << board_.score() << "\033[0m"
        << "    Rekor: \033[1m" << highScore_ << "\033[0m\n\n";

    out << horizontalLine("┌", "┬", "┐") << "\n";
    for (int r = 0; r < Board::SIZE; r++) {
        out << "  │";
        for (int c = 0; c < Board::SIZE; c++) {
            int value = board_.grid()[r][c];
            if (value == 0)
                out << centered("·", CELL_WIDTH + 1);  // "·" UTF-8'de 2 bayt
            else
                out << colorize(value, centered(std::to_string(value), CELL_WIDTH));
            out << "│";
        }
        out << "\n";
        out << (r + 1 < Board::SIZE ? horizontalLine("├", "┼", "┤")
                                    : horizontalLine("└", "┴", "┘"))
            << "\n";
    }

    out << "\n";
    if (!board_.canMove())
        out << "  \033[1;31mHamle kalmadı, oyun bitti!\033[0m  R: yeniden başla  Q: çıkış\n";
    else if (!message_.empty())
        out << "  " << message_ << "\n";
    else
        out << "\n";

    out << "\n  Ok tuşları / WASD: hareket   U: geri al   R: yeniden başla   Q: çıkış\n";

    terminal::clear();
    std::cout << out.str() << std::flush;
}

void Game::loadHighScore() {
    std::ifstream file(highScoreFile_);
    if (!(file >> highScore_)) highScore_ = 0;
}

void Game::saveHighScore() const {
    std::ofstream file(highScoreFile_);
    if (file) file << highScore_ << "\n";
}
