#include "Element.h"
#include <iostream>

void Element::onApply(
Enemy* target,
std::vector<Enemy*>& enemies)
{
}

float Element::damageModifier()
{
    return 1.0f;
}

void FireElement::onApply(
Enemy* target,
std::vector<Enemy*>& enemies)
{
    if (!target) return;
    target->applyBurn(10.0f, 3.0f);

    std::cout<<"Burn Applied\n";
}

void WaterElement::onApply(
Enemy* target,
std::vector<Enemy*>& enemies)
{
    if (!target) return;
    target->applySlow(0.4f, 2.0f);

    std::cout<<"Slow Applied\n";
}

float EarthElement::damageModifier()
{
    return 1.5f;
}
