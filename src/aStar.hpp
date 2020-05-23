#pragma once

#include <vector>
#include <set>
#include <queue>
#include "utils.hpp"
#include "IterablePriorityQueue.hpp"

struct Cell {
    glm::ivec2 pos_;
    float cost_;
    float heuristic_;
    Cell* cameFrom_;
    Cell(const glm::ivec2& pos, const float& cost, const float& heuristic, Cell* cameFrom)
        : pos_(pos), cost_(cost), heuristic_(heuristic), cameFrom_(cameFrom) {
    }
};

struct CellCompare {
    bool operator()(const Cell* a, const Cell* b) {
        return a->cost_ + a->heuristic_ > b->cost_ + b->heuristic_;
    }
};

class AStar {

private:

    const std::vector<glm::ivec2> directions = {
        { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 },
        { -1, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 }
    };

    const std::vector<bool>& grid_;
    int cols_, rows_;
    glm::ivec2 A_, B_;
    bool moveDiagonally_;
    int weigth_;

    std::vector<glm::ivec2> path_;
    bool done_;
    IterablePriorityQueue<Cell*, std::vector<Cell*>, CellCompare> openList_;
    std::vector<Cell*> closedList_;

public:
    AStar(const std::vector<bool>& grid, const int& cols, const int& rows, glm::ivec2& A, glm::ivec2& B, const bool& moveDiagonally = false, const int& weigth = 1) : 
    grid_(grid), cols_(cols), rows_(rows), A_(A), B_(B), moveDiagonally_(moveDiagonally), done_(false), weigth_(weigth) {
        reset();
    }

    void reset();
    bool next();
    void loopNextComputePath();
    std::vector<glm::ivec2> computePath();
    std::vector<glm::ivec2> calcPath(Cell* current);

    inline auto getOpenList() { return openList_; };
    inline std::vector<Cell*> getClosedList() { return closedList_; };
    inline std::vector<glm::ivec2> getPath() { return path_; };
    inline void setA(const glm::ivec2& A) { A_ = A; };
    inline void setB(const glm::ivec2& B) { B_ = B; };
    inline bool isDone() { return done_; };
    inline bool isCollisionAt(const int& x, const int& y) { return grid_[coordsToId(x, y, cols_)]; };
    inline bool isCollisionAt(const glm::ivec2& pos) { return grid_[coordsToId(pos, cols_)]; };

private:
    float estimateHeuristicCost(const int& x1, const int& y1, const int& x2, const int& y2);
    float estimateHeuristicCost(const glm::vec2& a, const glm::vec2& b);
};