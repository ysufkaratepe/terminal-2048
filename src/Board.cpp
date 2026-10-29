#include "Board.hpp"

#include <vector>

Board::Board(unsigned seed) : rng_(seed) {
    reset();
}

void Board::reset() {
    grid_ = Grid{};
    score_ = 0;
    spawnTile();
    spawnTile();
}

int Board::slideRow(Row& row) {
    Row result{};
    int points = 0;
    int pos = 0;
    bool canMerge = false;  // son yerleştirilen taş birleşebilir mi?

    for (int value : row) {
        if (value == 0) continue;

        if (canMerge && result[pos - 1] == value) {
            result[pos - 1] *= 2;
            points += result[pos - 1];
            canMerge = false;  // aynı taş ikinci kez birleşmesin
        } else {
            result[pos++] = value;
            canMerge = true;
        }
    }

    row = result;
    return points;
}

// Her yön için satırı "sola kaydırma" problemine çeviren indeks eşlemesi.
// line: kaçıncı satır/sütun, index: kayma yönünde baştan kaçıncı hücre.
int& Board::cell(Direction dir, int line, int index) {
    switch (dir) {
        case Direction::Left:  return grid_[line][index];
        case Direction::Right: return grid_[line][SIZE - 1 - index];
        case Direction::Up:    return grid_[index][line];
        case Direction::Down:  return grid_[SIZE - 1 - index][line];
    }
    return grid_[line][index];  // buraya ulaşılmaz
}

bool Board::move(Direction dir) {
    bool changed = false;

    for (int line = 0; line < SIZE; line++) {
        Row row;
        for (int i = 0; i < SIZE; i++) row[i] = cell(dir, line, i);

        Row before = row;
        score_ += slideRow(row);

        if (row != before) {
            changed = true;
            for (int i = 0; i < SIZE; i++) cell(dir, line, i) = row[i];
        }
    }

    if (changed) spawnTile();
    return changed;
}

bool Board::canMove() const {
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            if (grid_[r][c] == 0) return true;
            if (c + 1 < SIZE && grid_[r][c] == grid_[r][c + 1]) return true;
            if (r + 1 < SIZE && grid_[r][c] == grid_[r + 1][c]) return true;
        }
    }
    return false;
}

bool Board::hasWon() const {
    for (const Row& row : grid_)
        for (int value : row)
            if (value >= WIN_TILE) return true;
    return false;
}

void Board::spawnTile() {
    std::vector<std::pair<int, int>> empty;
    for (int r = 0; r < SIZE; r++)
        for (int c = 0; c < SIZE; c++)
            if (grid_[r][c] == 0) empty.emplace_back(r, c);

    if (empty.empty()) return;

    std::uniform_int_distribution<std::size_t> pick(0, empty.size() - 1);
    auto [r, c] = empty[pick(rng_)];

    // %90 ihtimalle 2, %10 ihtimalle 4
    std::uniform_int_distribution<int> chance(1, 10);
    grid_[r][c] = (chance(rng_) == 1) ? 4 : 2;
}
