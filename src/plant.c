#include "plant.h"

void plant_init(plant_t* p, float alpha) {
    p->y = 0.0f;
    p->alpha = alpha;
}

float plant_tick(plant_t* p, float u) {
    p->y = p->y + p->alpha * (u - p->y);
    return p->y;
}