#include "aStar.hpp"

#include <queue>
#include <cmath>
#include <algorithm>
#include <iostream>

void AStar::reset() {
    openList_ = IterablePriorityQueue<Cell*, std::vector<Cell*>, CellCompare>();
    closedPosList_.clear();
    path_.clear();
    done_ = false;
    openList_.push(new Cell(A_ , 0.0f, estimateHeuristicCost(A_, B_), nullptr));
}

bool AStar::next() {
    std::cout << "next" << std::endl;
    if(!done_) { // si on à déjà fini on faire rien de plus
        Cell* current = nullptr;
        if (!openList_.empty()) {
            current = openList_.top();
            std::cout << "(" << current->pos_.x << ", " << current->pos_.y << ")" << std::endl;
            // si on est toujours pas arrivé à notre destination on continue sinon on s'arrete ici
            if (current->pos_ != B_) {

                openList_.pop();
                closedPosList_.push_back(current->pos_);

                for (int i = 0; i < (moveDiagonally_ ? 8 : 4); i++) {
                    glm::ivec2 newPos = current->pos_ + directions[i];
                    // on check si notre nouelle position à tester existe sur la grille et si elle n'est pas un obstacte (qu'il n'y a pas collision)
                    if( !isInBound(newPos, rows_, cols_) || isCollisionAt(newPos) ) {
                        // si c'est l'un des deux cas ou les deux cela ne sert à rien de continer avec celle-ci, on passe à la position suivante à tester
                        continue;
                    }

                    // si notre position à déjà été exploré par le passé, on passe à la suivante
                    if( std::find(closedPosList_.begin(), closedPosList_.end(), newPos) != closedPosList_.end()) {
                        continue;
                    }

                    // on cherche si notre position est dans notre openList
                    Cell* neighbor = nullptr;
                    for (Cell* cell : openList_) {
                        if (cell->pos_ == newPos) {
                            neighbor = cell;
                            break;
                        }
                    }

                    // on calcul le nouveau coût associé à la position actuelle (distance à partir du départ) de la cellule en sommant le coût de la cellule actuelle plus la distance à celle-ci
                    float newCost = current->cost_+ (i < 4 ? 1.0f : sqrtf(2.0f)); 

                    if (neighbor == nullptr) { // si la position n'est pas dans l'openList on l'ajoute

                        openList_.push( new Cell(newPos, newCost, estimateHeuristicCost((glm::vec2)newPos, (glm::vec2)B_), current) );

                    // si la position y est déjà et que le score calculé par le chemin actuel est inférieur alors on change de parent
                    } else if (newCost < neighbor->cost_) {  
                        neighbor->cameFrom_ = current;
                        neighbor->cost_ = newCost;
                    }
                }
            }
        }else {
            std::cout << "chemin impossible" << std::endl;
        }

        if (current != nullptr && current->pos_ == B_) {
            calcPath(current);
            done_ = true;
        }
    }
    return done_;
}

std::vector<glm::ivec2> AStar::calcPath(Cell* current) {
    while(current != nullptr) {
        path_.push_back(current->pos_);
        current = current->cameFrom_;
    }
    std::reverse(path_.begin(), path_.end());
    return path_;
}

void AStar::computeFullPath() {
    while(!next()) {}
}


float AStar::estimateHeuristicCost(const int& x1, const int& y1, const int& x2, const int& y2) {
    return estimateHeuristicCost(glm::vec2(x1, y1), glm::vec2(x2, y2));
}
float AStar::estimateHeuristicCost(const glm::vec2& a, const glm::vec2& b) {
    if (moveDiagonally_) {
        return weigth_ * glm::distance(a, b); // euclidian distance
    } else {
        return weigth_ * manhattanDist(a, b); // manhattan distance 
    }
}