#pragma once

typedef struct {
    float y;
    float alpha;
} plant_t;

void plant_init(plant_t* p, float alpha);
float plant_tick(plant_t* p, float u);