#pragma once

#include <glm/glm.hpp>

// usful functions
inline int coordsToId(const int& x, const int& y, const int& cols) { return y * cols + x; }
inline int coordsToId(const glm::ivec2& p, const int& cols) { return coordsToId(p.x, p.y, cols); }
inline glm::ivec2 idToCoords(const int& id, const int& cols) { return glm::ivec2(id%cols, id/cols); }

inline bool isInBound(const glm::ivec2& pos, const glm::ivec2& min, const glm::ivec2& max) { return glm::all(glm::greaterThanEqual(pos, min)) && glm::all(glm::lessThan(pos, max)); }
inline bool isInBound(const glm::ivec2& pos, const int& rows, const int& cols) { return isInBound(pos, glm::ivec2(0,0), glm::ivec2(rows,cols)); }

// compute the manhattan distance between two glm vec2
inline float manhattanDist(const glm::vec2& a, const glm::vec2& b) { return glm::dot(glm::abs(a - b), glm::vec2(1, 1)); }