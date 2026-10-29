#pragma once

#include <array>
#include <random>

enum class Direction { Up, Down, Left, Right };

// 4x4 oyun tahtası ve 2048 kuralları.
// Ekrana çizim veya klavye ile ilgili hiçbir şey bilmez, bu sayede test edilebilir.
class Board {
public:
    static constexpr int SIZE = 4;
    static constexpr int WIN_TILE = 2048;
    using Row = std::array<int, SIZE>;
    using Grid = std::array<Row, SIZE>;

    explicit Board(unsigned seed = std::random_device{}());

    // Tahtayı temizler ve iki başlangıç taşı koyar.
    void reset();

    // Taşları verilen yöne kaydırır. Tahta değiştiyse yeni bir taş ekler ve true döner.
    bool move(Direction dir);

    // Yapılabilecek bir hamle kaldı mı?
    bool canMove() const;

    // Tahtada 2048 (veya daha büyük) bir taş var mı?
    bool hasWon() const;

    int score() const { return score_; }
    const Grid& grid() const { return grid_; }

    // Testlerde belirli bir tahta durumu kurmak için.
    void setGrid(const Grid& grid) { grid_ = grid; }

    // Bir satırı sola kaydırıp birleştirir, kazanılan puanı döner.
    // Her taş bir hamlede en fazla bir kez birleşir: [2,2,2,2] -> [4,4,0,0]
    static int slideRow(Row& row);

private:
    int& cell(Direction dir, int line, int index);
    void spawnTile();

    Grid grid_{};
    int score_ = 0;
    std::mt19937 rng_;
};
