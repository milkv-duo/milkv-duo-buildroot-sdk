#include <stdint.h>
#include <stdio.h>
#include "pwm.h"
#include "hal_pwm.h"

const uint64_t count_unit = 100000000;  // 100M count per second
const uint64_t NSEC_COUNT = 1000000000;  // ns

//设定 PWM 输出
void pwm_init(pwm_configuration_t *cfg)
{
    uint32_t reg_base = cvi_pwm[cfg -> pwm_id].reg_base;
    uint64_t duty_clk, period_clk;

    cvi_pwm_set_polarity_high_ch(reg_base, (cfg->channel & PWM_MAX_CH));

    duty_clk = (cfg->pulse * count_unit) / NSEC_COUNT;
    cvi_pwm_set_high_period_ch(reg_base, (cfg->channel & PWM_MAX_CH), duty_clk);

    period_clk = (cfg->period * count_unit) / NSEC_COUNT;
    cvi_pwm_set_period_ch(reg_base, (cfg->channel & PWM_MAX_CH), period_clk);

    cvi_pwm_output_en_ch(reg_base, cfg->channel & PWM_MAX_CH);
}

//设定 PWM 输出参数
void pwm_set_output_cfg(pwm_configuration_t *cfg)
{
    uint64_t duty_clk, period_clk;

    duty_clk = (cfg->pulse * count_unit) / NSEC_COUNT;
    cvi_pwm_set_high_period_ch(cvi_pwm[cfg -> pwm_id].reg_base, (cfg->channel & PWM_MAX_CH), duty_clk);

    period_clk = (cfg->period * count_unit) / NSEC_COUNT;
    cvi_pwm_set_period_ch(cvi_pwm[cfg -> pwm_id].reg_base, (cfg->channel & PWM_MAX_CH), period_clk);

    if(PWM_PWMSTART(cvi_pwm[cfg -> pwm_id].reg_base)  &= CVI_PWM_START_CH_EN(cfg->channel & PWM_MAX_CH))
        cvi_pwm_update_ch(cvi_pwm[cfg -> pwm_id].reg_base, (cfg->channel & PWM_MAX_CH));
}

//启动 PWM 连续输出模式
void pwm_continuous_start(pwm_configuration_t *cfg)
{
    cvi_pwm_set_pwm_mode_ch(cvi_pwm[cfg -> pwm_id].reg_base, cfg->channel & PWM_MAX_CH, 0);

    cvi_pwm_start_en_ch(cvi_pwm[cfg -> pwm_id].reg_base, cfg->channel & PWM_MAX_CH);
}

//启动 PWM 固定脉冲个数输出模式
void pwm_pulse_cnt_start(pwm_configuration_t *cfg, uint32_t cnt)
{
    cvi_pwm_start_dis_ch(cvi_pwm[cfg -> pwm_id].reg_base, cfg->channel & PWM_MAX_CH);
    
    cvi_pwm_set_pwm_mode_ch(cvi_pwm[cfg -> pwm_id].reg_base, cfg->channel & PWM_MAX_CH, 1);
    cvi_pwm_pcount_ch(cvi_pwm[cfg -> pwm_id].reg_base, cfg->channel & PWM_MAX_CH, cnt);

    cvi_pwm_start_en_ch(cvi_pwm[cfg -> pwm_id].reg_base, cfg->channel & PWM_MAX_CH);
}

//查看固定脉冲个数输出模式下 PWM 是否输出完成
bool pwm_pulse_cnt_done(pwm_configuration_t *cfg)
{
    return (PWM_PWMDONE(cvi_pwm[cfg -> pwm_id].reg_base) &= CVI_PWM_DONE_CH_EN(cfg->channel & PWM_MAX_CH)) ? true : false;
}

//停止 PWM 输出
void pwm_stop(pwm_configuration_t *cfg)
{
    cvi_pwm_start_dis_ch(cvi_pwm[cfg -> pwm_id].reg_base, cfg->channel & PWM_MAX_CH);
}
