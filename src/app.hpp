#pragma once
#include <piksel/baseapp.hpp>

class App : public piksel::BaseApp {
public:
    App() : BaseApp(1024, 1024) {}
    void setup();
    void draw(piksel::Graphics& g);
    void mouseMoved(int x, int y);
    void mousePressed(int button);
    void keyPressed(int key);
};

