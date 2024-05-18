#pragma once
#include "Zombie.hpp"

class Crawler:public Zombie{
    virtual void spawn(double x, double y, int seed) override;
};