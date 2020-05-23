#include "app.hpp"
#include "aStar.hpp"

#include "utils.hpp"

#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <piksel/ext/rng.hpp>

#include <iostream>

// variables

const int rows = 10, cols = 10; // rows & cols count of our grid
int w, h;
const int gridSize = rows * cols;

glm::vec2 tileSize; // used to display our grid
std::vector<bool> grid(gridSize); // our obstacles grid
glm::vec2 mousePos;

glm::ivec2 A(0, 0); // start pos
glm::ivec2 B(1, 0); // end pos
AStar* aStar;

inline glm::ivec2 mousePosToCoords() { return glm::ivec2(glm::clamp<int>(mousePos.x / tileSize.x, 0, cols - 1), glm::clamp<int>(mousePos.y / tileSize.y, 0, rows - 1)); }

void randomiseGrid(std::vector<bool>& grid) {
    piksel::Rng& rng = piksel::Rng::getInstance();
    for(auto &&x : grid) {
        x = rng.random(0.0f, 1.0f) < 0.25f;
    }
    // ensure on collision at start & end points
    grid[coordsToId(A, cols)] = 0;
    grid[coordsToId(B, cols)] = 0;
}

void App::setup() {
    w = width;
    h = height;
    tileSize = glm::vec2( ((float) w) / cols, ((float) h) / rows);
    randomiseGrid(grid);
    aStar = new AStar(grid, cols, rows, A, B);

}

void App::draw(piksel::Graphics& g) {
    g.background(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    g.textSize(tileSize.x/8);

    g.noStroke();
    g.fill(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    // draw grid wall
    for(int id = 0; id < gridSize; id++) {
        const glm::ivec2 coords = idToCoords(id, cols); // get coordinates
        if (grid[id]) {
            g.rect(coords.x * tileSize.x, coords.y * tileSize.y, tileSize.x, tileSize.y);
        }
    }

    // draw openListCells
    g.noStroke();
    for(const Cell* cell : aStar->getOpenList()) {
        glm::ivec2 pos = cell->pos_;
        g.fill(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        g.rect(pos.x * tileSize.x, pos.y * tileSize.y, tileSize.x, tileSize.y);
        g.fill(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        g.text(std::to_string((int)cell->heuristic_), pos.x * tileSize.x + tileSize.x*0.1, pos.y * tileSize.y + tileSize.y*0.2);
        g.text(std::to_string((int)cell->cost_), pos.x * tileSize.x + tileSize.x*0.7, pos.y * tileSize.y + tileSize.y*0.2);
        g.text(std::to_string((int)cell->heuristic_ + (int)cell->cost_), pos.x * tileSize.x + tileSize.x/2, pos.y * tileSize.y + tileSize.y/2);
    }

    // draw A & B
    g.noStroke();
    g.fill(glm::vec4(0.0f, 0.6f, 0.8f, 1.0f));
    g.rect(A.x * tileSize.x, A.y * tileSize.y, tileSize.x, tileSize.y);
    g.rect(B.x * tileSize.x, B.y * tileSize.y, tileSize.x, tileSize.y);
    g.fill(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    g.text("A", A.x * tileSize.x + tileSize.x/2, A.y * tileSize.y + tileSize.y/2);
    g.text("B", B.x * tileSize.x + tileSize.x/2, B.y * tileSize.y + tileSize.y/2);

    // draw hover Case
    g.noStroke();
    g.fill(glm::vec4(1.0f, 0.0f, 0.0f, 0.2f));
    const glm::ivec2 selectedCoords = mousePosToCoords();
    g.rect(selectedCoords.x * tileSize.x, selectedCoords.y * tileSize.y, tileSize.x, tileSize.y);

    // draw grid lines
    g.stroke(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    g.strokeWeight(tileSize.x / 20);
    for(int r = 0; r< rows+1; r++) {
        g.line(0, r * tileSize.y, width, r * tileSize.y);
    }
    for(int c = 0; c < cols+1; c++) {
        g.line(c * tileSize.x, 0, c * tileSize.x, height);
    }
}


void App::mouseMoved(int x, int y) {

    // update mousePos
    mousePos.x = x;
    mousePos.y = y;
}

void App::mousePressed(int x, int y) {
}

void App::keyPressed(int key) {
    const glm::ivec2 mouseCoords = mousePosToCoords();
    const auto path = aStar->getPath();
    switch (key) {
    case 78: // N 
        aStar->next();
        break;
    case 67: // C
        aStar->computeFullPath();
        break;
    case 80: // P
        for(auto pos : path) {
            std::cout << " -> (" << pos.x << ", " << pos.y << ")";
        }
        std::cout << std::endl;
        break;
    case 81: // A (on azerty)
        std::cout << "A" << std::endl;
        if(!grid[coordsToId(mouseCoords, cols)] && mouseCoords != A && mouseCoords != B) {
            A = mouseCoords;
            aStar->setA(A);
            aStar->reset();
        }
        break;
    case 66: // B
        std::cout << "B" << std::endl;
        if(!grid[coordsToId(mouseCoords, cols)] && mouseCoords != A && mouseCoords != B) {
            B = mouseCoords;
            aStar->setB(B);
            aStar->reset();
        }
        break;
    
    default:
        std::cout << "keycode " << key << " not assigned yet" << std::endl;
        break;
    }
}