// Oyun kurallarını kontrol eden basit testler.
// Çalıştırma: ctest  veya  ./test_board

#include <iostream>
#include <string>

#include "Board.hpp"

static int failures = 0;

static void check(bool condition, const std::string& name) {
    if (condition) {
        std::cout << "[GECTI] " << name << "\n";
    } else {
        std::cout << "[KALDI] " << name << "\n";
        failures++;
    }
}

static bool slides(Board::Row input, Board::Row expected, int expectedPoints) {
    int points = Board::slideRow(input);
    return input == expected && points == expectedPoints;
}

int main() {
    check(slides({2, 0, 2, 0}, {4, 0, 0, 0}, 4), "aradaki bosluga ragmen birlesir");
    check(slides({2, 2, 2, 2}, {4, 4, 0, 0}, 8), "dort ayni tas iki cift olur");
    check(slides({2, 2, 4, 0}, {4, 4, 0, 0}, 4), "yeni olusan tas ayni hamlede tekrar birlesmez");
    check(slides({4, 4, 4, 0}, {8, 4, 0, 0}, 8), "birlesme kayma yonunden baslar");
    check(slides({2, 4, 8, 16}, {2, 4, 8, 16}, 0), "farkli taslar yerinde kalir");
    check(slides({0, 0, 0, 2}, {2, 0, 0, 0}, 0), "tek tas en basa kayar");

    {
        Board board(42);
        board.setGrid({{{2, 4, 2, 4}, {4, 2, 4, 2}, {2, 4, 2, 4}, {4, 2, 4, 2}}});
        check(!board.canMove(), "dolu ve birlesmeyen tahtada hamle yok");
        check(!board.move(Direction::Left), "degismeyen hamle false doner");
    }

    {
        Board board(42);
        board.setGrid({{{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {2, 0, 0, 2}}});
        bool moved = board.move(Direction::Right);
        check(moved && board.grid()[3][3] == 4 && board.score() == 4,
              "saga hamlede birlesip skor artar");
    }

    {
        Board board(42);
        board.setGrid({{{2, 0, 0, 0}, {2, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}}});
        board.move(Direction::Up);
        check(board.grid()[0][0] == 4 && board.grid()[1][0] == 8, "yukari hamle sutunu birlestirir");
    }

    {
        Board board(7);
        board.setGrid({{{1024, 1024, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}});
        check(!board.hasWon(), "2048 yokken kazanilmamis");
        board.move(Direction::Left);
        check(board.hasWon(), "2048 olusunca kazanilir");
    }

    {
        Board board(1);
        int tiles = 0;
        for (const auto& row : board.grid())
            for (int v : row)
                if (v != 0) tiles++;
        check(tiles == 2, "oyun iki tasla baslar");
    }

    std::cout << (failures == 0 ? "\nTum testler gecti.\n" : "\nBazi testler kaldi!\n");
    return failures == 0 ? 0 : 1;
}
