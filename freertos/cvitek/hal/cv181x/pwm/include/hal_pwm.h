#ifndef __DRV_PWM_H__
#define __DRV_PWM_H__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PWM_ID_0 0
#define PWM_ID_1 1
#define PWM_ID_2 2
#define PWM_ID_3 3

#define PWM_CHANNEL_0 0
#define PWM_CHANNEL_1 1
#define PWM_CHANNEL_2 2
#define PWM_CHANNEL_3 3

typedef struct{
    uint8_t  pwm_id;
    uint32_t channel;
    uint32_t period;
    uint32_t pulse;
} pwm_configuration_t;


#ifdef __cplusplus
}
#endif

void pwm_init(pwm_configuration_t *cfg);
void pwm_set_output_cfg(pwm_configuration_t *cfg);
void pwm_continuous_start(pwm_configuration_t *cfg);
void pwm_pulse_cnt_start(pwm_configuration_t *cfg, uint32_t cnt);
bool pwm_pulse_cnt_done(pwm_configuration_t *cfg);
void pwm_stop(pwm_configuration_t *cfg);

#endif
