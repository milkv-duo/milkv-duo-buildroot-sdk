#ifndef __PWM_H__
#define __PWM_H__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define     __IM     volatile const       /*! Defines 'read only' structure member permissions */
#define     __OM     volatile             /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile             /*! Defines 'read / write' structure member permissions */

#define CVI_PWM0_BASE               0x03060000
#define CVI_PWM1_BASE               0x03061000
#define CVI_PWM2_BASE               0x03062000
#define CVI_PWM3_BASE               0x03063000
#define PWM_MAX_CH                  3

typedef enum {
    PWM_CHANNEL_0    = 0U,
    PWM_CHANNEL_1,
    PWM_CHANNEL_2,
    PWM_CHANNEL_3,
    PWM_CHANNEL_4,
    PWM_CHANNEL_5,
    PWM_CHANNEL_6,
    PWM_CHANNEL_7,
    PWM_CHANNEL_8,
    PWM_CHANNEL_9,
    PWM_CHANNEL_10,
    PWM_CHANNEL_11,
    PWM_CHANNEL_12,
    PWM_CHANNEL_13,
    PWM_CHANNEL_14,
    PWM_CHANNEL_15,
    PWM_CHANNEL_NUM
} cvi_pwm_channel_t;


typedef struct{
    uint32_t HLPERIOD0;
    uint32_t PERIOD0;
    uint32_t HLPERIOD1;
    uint32_t PERIOD1;
    uint32_t HLPERIOD2;
    uint32_t PERIOD2;
    uint32_t HLPERIOD3;
    uint32_t PERIOD3;
    uint32_t CAP_FREQNUM;
    uint32_t CAP_FREQDATA;
    uint32_t POLARITY;
    uint32_t PWMSTART;
    uint32_t PWMDONE;
    uint32_t PWMUPDATE;
    uint32_t PCOUNT0;
    uint32_t PCOUNT1;
    uint32_t PCOUNT2;
    uint32_t PCOUNT3;
    uint32_t PULSECOUNT0;
    uint32_t PULSECOUNT1;
    uint32_t PULSECOUNT2;
    uint32_t PULSECOUNT3;
    uint32_t SHIFTCOUNT0;
    uint32_t SHIFTCOUNT1;
    uint32_t SHIFTCOUNT2;
    uint32_t SHIFTCOUNT3;
    uint32_t SHIFTSTART;
    uint32_t CAP_FREQEN;
    uint32_t CAP_FREQDONE_NUM;
    uint32_t PWM_OE;
} cvi_pwm_regs_typedef;

typedef struct cvi_pwm_dev
{
    const char *name;
    uint32_t reg_base;
} cvi_pwm_dev_typedef;

extern cvi_pwm_regs_typedef cvi_pwm_reg;
extern cvi_pwm_dev_typedef cvi_pwm[];

#define PWM_HLPERIOD0(reg_base)     *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.HLPERIOD0))
#define PWM_PERIOD0(reg_base)       *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.PERIOD0))
#define PWM_HLPERIOD1(reg_base)     *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.HLPERIOD1))
#define PWM_PERIOD1(reg_base)       *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.PERIOD1))
#define PWM_HLPERIOD2(reg_base)     *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.HLPERIOD2))
#define PWM_PERIOD2(reg_base)       *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.PERIOD2))
#define PWM_HLPERIOD3(reg_base)     *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.HLPERIOD3))
#define PWM_PERIOD3(reg_base)       *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.PERIOD3))
#define PWM_HLPERIODX(reg_base, _ch_)     *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.HLPERIOD0 + (_ch_ << 3)))
#define PWM_PERIODX(reg_base, _ch_)       *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.PERIOD0 * (1 + (_ch_ << 1))))

#define CAP_FREQNUM(reg_base, _ch_)     *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.CAP_FREQNUM + _ch_ * 8))
#define CAP_FREQDATA(reg_base, _ch_)       *((__IM uint32_t *)(reg_base + cvi_pwm_reg.CAP_FREQDATA + _ch_ * 8))

#define PWM_POLARITY(reg_base)      *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.POLARITY))
#define PWM_PWMSTART(reg_base)      *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.PWMSTART))
#define PWM_PWMDONE(reg_base)       *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.PWMDONE))
#define PWM_PWMUPDATE(reg_base)     *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.PWMUPDATE))

#define PWM_PCOUNT0(reg_base)       *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.PCOUNT0))
#define PWM_PCOUNT1(reg_base)       *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.PCOUNT1))
#define PWM_PCOUNT2(reg_base)       *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.PCOUNT2))
#define PWM_PCOUNT3(reg_base)       *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.PCOUNT3))

#define PWM_PULSECOUNT0(reg_base)       *((__IM uint32_t *)(reg_base + cvi_pwm_reg.PULSECOUNT0))
#define PWM_PULSECOUNT1(reg_base)       *((__IM uint32_t *)(reg_base + cvi_pwm_reg.PULSECOUNT1))
#define PWM_PULSECOUNT2(reg_base)       *((__IM uint32_t *)(reg_base + cvi_pwm_reg.PULSECOUNT2))
#define PWM_PULSECOUNT3(reg_base)       *((__IM uint32_t *)(reg_base + cvi_pwm_reg.PULSECOUNT3))

#define PWM_SHIFTCOUNT0(reg_base)       *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.SHIFTCOUNT0))
#define PWM_SHIFTCOUNT1(reg_base)       *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.SHIFTCOUNT1))
#define PWM_SHIFTCOUNT2(reg_base)       *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.SHIFTCOUNT2))
#define PWM_SHIFTCOUNT3(reg_base)       *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.SHIFTCOUNT3))
#define PWM_SHIFTSTART(reg_base)        *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.SHIFTSTART))

#define CAP_FREQEN(reg_base)        *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.CAP_FREQEN))
#define CAP_FREQDONE_NUM(reg_base, _ch_)  *((__IM uint32_t *)(reg_base + cvi_pwm_reg.CAP_FREQDONE_NUM + _ch_ * 4))

#define PWM_PWM_OE(reg_base)        *((__IOM uint32_t *)(reg_base + cvi_pwm_reg.PWM_OE))


/*! PWM Configure Register,     offset: 0x00 */
#define CVI_PWM_HIGH_PERIOD_Pos                         (0U)
#define CVI_PWM_HIGH_PERIOD_Msk                         (0xffffffff)

#define CVI_PWM_PERIOD_Pos                              (0U)
#define CVI_PWM_PERIOD_Msk                              (0xffffffff)

#define CVI_PWM_POLARITY_CH_Pos(_ch_)                     (_ch_)
#define CVI_PWM_POLARITY_CH_Msk(_ch_)                     (1U << CVI_PWM_POLARITY_CH_Pos(_ch_))
#define CVI_PWM_POLARITY_CH_HIGH(_ch_)                    CVI_PWM_POLARITY_CH_Msk(_ch_)

#define CVI_PWM_MODE_CH_Pos(_ch_)                     (_ch_)
#define CVI_PWM_MODE_CH_Msk(_ch_)                     (1U << (CVI_PWM_MODE_CH_Pos(_ch_) + 8))
#define CVI_PWM_MODE_CH_HIGH(_ch_)                    CVI_PWM_MODE_CH_Msk(_ch_)

#define CVI_PWM_START_CH_Pos(_ch_)                        (_ch_)
#define CVI_PWM_START_CH_Msk(_ch_)                        (1U << CVI_PWM_START_CH_Pos(_ch_))
#define CVI_PWM_START_CH_EN(_ch_)                         CVI_PWM_START_CH_Msk(_ch_)

#define CVI_PWM_DONE_CH_Pos(_ch_)                        (_ch_)
#define CVI_PWM_DONE_CH_Msk(_ch_)                        (1U << CVI_PWM_DONE_CH_Pos(_ch_))
#define CVI_PWM_DONE_CH_EN(_ch_)                         CVI_PWM_DONE_CH_Msk(_ch_)

#define CVI_PWM_UPDATE_CH_Pos(_ch_)                        (_ch_)
#define CVI_PWM_UPDATE_CH_Msk(_ch_)                        (1U << CVI_PWM_UPDATE_CH_Pos(_ch_))
#define CVI_PWM_UPDATE_CH_EN(_ch_)                         CVI_PWM_UPDATE_CH_Msk(_ch_)

#define CVI_PWM_OUTPUT_CH_Pos(_ch_)                       (_ch_)
#define CVI_PWM_OUTPUT_CH_Msk(_ch_)                       (1U << CVI_PWM_OUTPUT_CH_Pos(_ch_))
#define CVI_PWM_OUTPUT_CH_EN(_ch_)                        CVI_PWM_OUTPUT_CH_Msk(_ch_)

#define CVI_CAP_FREQNUM_CH_Pos                             (0U)
#define CVI_CAP_FREQNUM_CH_Msk                             (0xffffffff)

#define CVI_CAP_FREQEN_Pos(_ch_)                           (_ch_)
#define CVI_CAP_FREQEN_Msk(_ch_)                           (1U << CVI_CAP_FREQEN_Pos(_ch_))
#define CVI_CAP_FREQEN(_ch_)                                CVI_CAP_FREQEN_Msk(_ch_)

#define CVI_CAP_FREQDONE_NUM_Poa                               (0U)
#define CVI_CAP_FREQDONE_NUM_Msk                               (0xffffffff)

#define CVI_CAP_FREQDATA_pos                                 (0U)
#define CVI_CAP_FREQDATA_msk                                 (0xffffffff)

#ifdef __cplusplus
}
#endif

void cvi_pwm_set_high_period_ch(unsigned long reg_base, uint32_t ch, unsigned long long value);
unsigned long long cvi_pwm_get_high_period_ch(unsigned long reg_base, uint32_t ch);
void cvi_pwm_set_period_ch(unsigned long reg_base, uint32_t ch, unsigned long long value);
unsigned long long cvi_pwm_get_period_ch(unsigned long reg_base, uint32_t ch);
void cvi_pwm_set_polarity_high_ch(unsigned long reg_base, uint32_t ch);
void cvi_pwm_set_polarity_low_ch(unsigned long reg_base, uint32_t ch);
uint32_t cvi_pwm_get_polarity(unsigned long reg_base, uint32_t ch);
void cvi_pwm_set_pwm_mode_ch(unsigned long reg_base, uint32_t ch, uint8_t mode);
void cvi_pwm_start_en_ch(unsigned long reg_base, uint32_t ch);
void cvi_pwm_start_dis_ch(unsigned long reg_base, uint32_t ch);
void cvi_pwm_update_ch(unsigned long reg_base, uint32_t ch);
void cvi_pwm_pcount_ch(unsigned long reg_base, uint32_t ch, uint32_t value);
void cvi_pwm_output_en_ch(unsigned long reg_base, uint32_t ch);
void cvi_pwm_input_en_ch(unsigned long reg_base, uint32_t ch);
void cvi_cap_set_freqnum_ch(unsigned long reg_base, uint32_t ch, uint32_t value);
void cvi_cap_freq_en_ch(unsigned long reg_base, uint32_t ch);
void cvi_cap_freq_dis_ch(unsigned long reg_base, uint32_t ch);
uint32_t cvi_cap_get_freq_done_num_ch(unsigned long reg_base, uint32_t ch);
uint32_t cvi_cap_get_freq_data_ch(unsigned long reg_base, uint32_t ch);

#endif
