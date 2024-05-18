#pragma once
#include "Zombie.hpp"

class Chaser:public Zombie{
    virtual void spawn(double x, double y, int seed) override;
};