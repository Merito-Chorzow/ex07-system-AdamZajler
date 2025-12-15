#include "control.h"

static float clamp(float v, float min, float max) {
    if (v < min) return min;
    if (v > max) return max;
    return v;
}

void sys_init(system_t* sys) {
    sys->mode = MODE_INIT;
    sys->setpoint = 0.0f;
    sys->out = 0.0f;
    sys->meas = 0.0f;
    
    sys->pid.kp = 1.0f;
    sys->pid.ki = 0.0f;
    sys->pid.integ = 0.0f;
    sys->pid.lim_min = -100.0f;
    sys->pid.lim_max = 100.0f;
    
    sys->wd_limit = 50; 
    sys->wd_counter = 0;
    sys->ticks = 0;
    sys->u_sat_count = 0;

    sys->mode = MODE_IDLE;
}

void sys_config_pid(system_t* sys, float kp, float ki, float min, float max) {
    sys->pid.kp = kp;
    sys->pid.ki = ki;
    sys->pid.lim_min = min;
    sys->pid.lim_max = max;
    sys->pid.integ = 0.0f;
}

void sys_set_mode(system_t* sys, sys_mode_t new_mode) {
    if (sys->mode == MODE_FAULT && new_mode != MODE_IDLE && new_mode != MODE_INIT) {
        return;
    }
    
    if (new_mode == MODE_CLOSED && sys->mode != MODE_CLOSED) {
        sys->pid.integ = sys->out; 
    }
    
    sys->mode = new_mode;
    
    if (new_mode == MODE_IDLE || new_mode == MODE_SAFE || new_mode == MODE_FAULT) {
        sys->out = 0.0f;
    }
}

void sys_set_setpoint(system_t* sys, float sp) {
    sys->setpoint = sp;
}

void sys_set_open_out(system_t* sys, float val) {
    if (sys->mode == MODE_OPEN) {
        sys->out = clamp(val, sys->pid.lim_min, sys->pid.lim_max);
    }
}

void sys_wdg_reload(system_t* sys) {
    sys->wd_counter = 0;
}

void sys_update(system_t* sys, float measurement) {
    sys->ticks++;
    sys->meas = measurement;
    
    sys->wd_counter++;
    if (sys->wd_counter >= sys->wd_limit) {
        sys_set_mode(sys, MODE_SAFE);
    }

    switch (sys->mode) {
        case MODE_INIT:
            sys->out = 0.0f;
            sys->mode = MODE_IDLE;
            break;

        case MODE_IDLE:
        case MODE_SAFE:
        case MODE_FAULT:
            sys->out = 0.0f;
            break;

        case MODE_OPEN:
            break; 

        case MODE_CLOSED: {
            float err = sys->setpoint - sys->meas;
            float p_term = sys->pid.kp * err;
            
            sys->pid.integ += sys->pid.ki * err;
            sys->pid.integ = clamp(sys->pid.integ, sys->pid.lim_min, sys->pid.lim_max);
            
            float u = p_term + sys->pid.integ;
            
            if (u > sys->pid.lim_max) {
                u = sys->pid.lim_max;
                sys->u_sat_count++;
            } else if (u < sys->pid.lim_min) {
                u = sys->pid.lim_min;
                sys->u_sat_count++;
            }
            
            sys->out = u;
            break;
        }
    }
}

float sys_get_out(const system_t* sys) {
    return sys->out;
}