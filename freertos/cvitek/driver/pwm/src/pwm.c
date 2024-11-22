#include <stdint.h>
#include <stdio.h>
#include "pwm.h"

cvi_pwm_dev_typedef cvi_pwm[] =
{
    {
        .name = "pwm0",
        .reg_base = CVI_PWM0_BASE,
    },
    {
        .name = "pwm1",
        .reg_base = CVI_PWM1_BASE,
    },
    {
        .name = "pwm2",
        .reg_base = CVI_PWM2_BASE,
    },
    {
        .name = "pwm3",
        .reg_base = CVI_PWM3_BASE,
    },
};

cvi_pwm_regs_typedef cvi_pwm_reg = {
    .HLPERIOD0 = 0x0,
    .PERIOD0 = 0x4,
    .HLPERIOD1 = 0x8,
    .PERIOD1 = 0xc,
    .HLPERIOD2 = 0x10,
    .PERIOD2 = 0x14,
    .HLPERIOD3 = 0x18,
    .PERIOD3 = 0x1c,
    .CAP_FREQNUM = 0x20,
    .CAP_FREQDATA = 0x24,
    .POLARITY = 0x40,
    .PWMSTART = 0x44,
    .PWMDONE = 0x48,
    .PWMUPDATE = 0x4c,
    .PCOUNT0 = 0x50,
    .PCOUNT1 = 0x54,
    .PCOUNT2 = 0x58,
    .PCOUNT3 = 0x5c,
    .PULSECOUNT0 = 0x60,
    .PULSECOUNT1 = 0x64,
    .PULSECOUNT2 = 0x68,
    .PULSECOUNT3 = 0x6c,
    .SHIFTCOUNT0 = 0x80,
    .SHIFTCOUNT1 = 0x84,
    .SHIFTCOUNT2 = 0x88,
    .SHIFTCOUNT3 = 0x8c,
    .SHIFTSTART = 0x90,
    .CAP_FREQEN = 0x9c,
    .CAP_FREQDONE_NUM = 0xC0,
    .PWM_OE = 0xd0,
};

void cvi_pwm_set_high_period_ch(unsigned long reg_base, uint32_t ch, unsigned long long value)
{
    PWM_HLPERIODX(reg_base, ch) = value;
}

unsigned long long cvi_pwm_get_high_period_ch(unsigned long reg_base, uint32_t ch)
{
    return PWM_HLPERIODX(reg_base, ch);
}

void cvi_pwm_set_period_ch(unsigned long reg_base, uint32_t ch, unsigned long long value)
{
    PWM_PERIODX(reg_base, ch) = value;
}

unsigned long long cvi_pwm_get_period_ch(unsigned long reg_base, uint32_t ch)
{
    return PWM_PERIODX(reg_base, ch);
}

void cvi_pwm_set_polarity_high_ch(unsigned long reg_base, uint32_t ch)
{
    PWM_POLARITY(reg_base) |= CVI_PWM_POLARITY_CH_HIGH(ch);
}

void cvi_pwm_set_polarity_low_ch(unsigned long reg_base, uint32_t ch)
{
    PWM_POLARITY(reg_base) &= ~CVI_PWM_POLARITY_CH_HIGH(ch);
}

uint32_t cvi_pwm_get_polarity(unsigned long reg_base, uint32_t ch)
{
    return (PWM_POLARITY(reg_base) & CVI_PWM_POLARITY_CH_Msk(ch));
}

void cvi_pwm_set_pwm_mode_ch(unsigned long reg_base, uint32_t ch, uint8_t mode)
{
    if(mode)
        PWM_POLARITY(reg_base) |= CVI_PWM_MODE_CH_HIGH(ch);
    else
        PWM_POLARITY(reg_base) &= ~CVI_PWM_MODE_CH_HIGH(ch);
}

void cvi_pwm_start_en_ch(unsigned long reg_base, uint32_t ch)
{
    PWM_PWMSTART(reg_base) |= CVI_PWM_START_CH_EN(ch);
}

void cvi_pwm_start_dis_ch(unsigned long reg_base, uint32_t ch)
{
    PWM_PWMSTART(reg_base) &= ~CVI_PWM_START_CH_EN(ch);
}

void cvi_pwm_update_ch(unsigned long reg_base, uint32_t ch)
{
    PWM_PWMUPDATE(reg_base) |= CVI_PWM_UPDATE_CH_EN(ch);
    PWM_PWMUPDATE(reg_base) &= ~CVI_PWM_UPDATE_CH_EN(ch);
}

void cvi_pwm_pcount_ch(unsigned long reg_base, uint32_t ch, uint32_t value)
{
    switch (ch)
    {
        case PWM_CHANNEL_0:
            PWM_PCOUNT0(reg_base) = value;
            break;
        case PWM_CHANNEL_1:
            PWM_PCOUNT1(reg_base) = value;
            break;
        case PWM_CHANNEL_2:
            PWM_PCOUNT2(reg_base) = value;
            break;
        case PWM_CHANNEL_3:
            PWM_PCOUNT3(reg_base) = value;
            break;
        default:
            break;
    }
}

void cvi_pwm_output_en_ch(unsigned long reg_base, uint32_t ch)
{
    PWM_PWM_OE(reg_base) |= CVI_PWM_OUTPUT_CH_EN(ch);
}

void cvi_pwm_input_en_ch(unsigned long reg_base, uint32_t ch)
{
    PWM_PWM_OE(reg_base) &= ~CVI_PWM_OUTPUT_CH_EN(ch);
}

void cvi_cap_set_freqnum_ch(unsigned long reg_base, uint32_t ch, uint32_t value)
{
    CAP_FREQNUM(reg_base, ch) = value;
}

void cvi_cap_freq_en_ch(unsigned long reg_base, uint32_t ch)
{
    CAP_FREQEN(reg_base) |= CVI_CAP_FREQEN(ch);
}

void cvi_cap_freq_dis_ch(unsigned long reg_base, uint32_t ch)
{
    CAP_FREQEN(reg_base) &= ~CVI_CAP_FREQEN(ch);
}

uint32_t cvi_cap_get_freq_done_num_ch(unsigned long reg_base, uint32_t ch)
{
    return CAP_FREQDONE_NUM(reg_base, ch);
}

uint32_t cvi_cap_get_freq_data_ch(unsigned long reg_base, uint32_t ch)
{
    return CAP_FREQDATA(reg_base, ch);
}
