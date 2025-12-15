#include <stdio.h>
#include "control.h"
#include "plant.h"

int main(void) {
    system_t sys;
    plant_t plant;
    
    sys_init(&sys);
    sys_config_pid(&sys, 2.0f, 0.1f, -10.0f, 10.0f);
    plant_init(&plant, 0.1f); 

    printf("TICK,MODE,SET,MEAS,OUT\n");

    for (int i = 0; i < 20; i++) {
        sys_wdg_reload(&sys);
        float y = plant_tick(&plant, sys_get_out(&sys));
        sys_update(&sys, y);
        printf("%d,%d,%.2f,%.2f,%.2f\n", i, sys.mode, sys.setpoint, sys.meas, sys.out);
    }

    sys_set_mode(&sys, MODE_OPEN);
    sys_set_open_out(&sys, 5.0f);
    
    for (int i = 20; i < 40; i++) {
        sys_wdg_reload(&sys);
        float y = plant_tick(&plant, sys_get_out(&sys));
        sys_update(&sys, y);
        printf("%d,%d,%.2f,%.2f,%.2f\n", i, sys.mode, sys.setpoint, sys.meas, sys.out);
    }

    sys_set_mode(&sys, MODE_CLOSED);
    sys_set_setpoint(&sys, 8.0f);

    for (int i = 40; i < 80; i++) {
        sys_wdg_reload(&sys);
        float y = plant_tick(&plant, sys_get_out(&sys));
        sys_update(&sys, y);
        printf("%d,%d,%.2f,%.2f,%.2f\n", i, sys.mode, sys.setpoint, sys.meas, sys.out);
    }

    for (int i = 80; i < 150; i++) {
        // brak sys_wdg_reload -> test SAFE
        float y = plant_tick(&plant, sys_get_out(&sys));
        sys_update(&sys, y);
        printf("%d,%d,%.2f,%.2f,%.2f\n", i, sys.mode, sys.setpoint, sys.meas, sys.out);
    }

    return 0;
}