#pragma once

#include "Enemy.h"
#include <vector>

class Element
{
public:

    virtual void onApply(
        Enemy* target,
        std::vector<Enemy*>& enemies);

    virtual float damageModifier();
};

class FireElement : public Element
{
public:

    void onApply(
        Enemy* target,
        std::vector<Enemy*>& enemies) override;
};

class WaterElement : public Element
{
public:

    void onApply(
        Enemy* target,
        std::vector<Enemy*>& enemies) override;
};

class EarthElement : public Element
{
public:

    float damageModifier() override;
};