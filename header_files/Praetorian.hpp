#pragma once
#include "Zombie.hpp"

class Praetorian:public Zombie{
    virtual void spawn(double x, double y, int seed) override;
};