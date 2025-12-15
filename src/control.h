#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    MODE_INIT,
    MODE_IDLE,
    MODE_OPEN,
    MODE_CLOSED,
    MODE_SAFE,
    MODE_FAULT
} sys_mode_t;

typedef struct {
    float kp;
    float ki;
    float integ;
    float lim_min;
    float lim_max;
} pid_t;

typedef struct {
    sys_mode_t mode;
    float setpoint;
    float out;
    float meas; 
    
    pid_t pid;
    
    int wd_counter;
    int wd_limit;
    
    unsigned ticks;
    unsigned u_sat_count;
} system_t;

void sys_init(system_t* sys);
void sys_config_pid(system_t* sys, float kp, float ki, float min, float max);
void sys_set_mode(system_t* sys, sys_mode_t new_mode);
void sys_set_setpoint(system_t* sys, float sp);
void sys_set_open_out(system_t* sys, float val);
void sys_wdg_reload(system_t* sys);
void sys_update(system_t* sys, float measurement);
float sys_get_out(const system_t* sys);