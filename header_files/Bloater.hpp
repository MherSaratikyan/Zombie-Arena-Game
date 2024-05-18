#pragma once
#include "Zombie.hpp"

class Bloater:public Zombie{
    virtual void spawn(double x, double y, int seed) override;
};