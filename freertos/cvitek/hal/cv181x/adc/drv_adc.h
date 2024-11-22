#ifndef __DRV_ADC_H__
#define __DRV_ADC_H__

#include <stdint.h>
#include <stdbool.h>

#define SARADC_BASE                         0x030F0000
#define SARADC_CH_MAX                       3

#define SARADC_CTRL_OFFSET                  0x04
#define SARADC_CTRL_START                   (1 << 0)
#define SARADC_CTRL_SEL_POS                 0x04

#define SARADC_STATUS_OFFSET                0x08
#define SARADC_STATUS_BUSY                  (1 << 0)

#define SARADC_CYC_SET_OFFSET               0x0C
#define SARADC_CYC_CLKDIV_DIV_POS           (12U)
#define SARADC_CYC_CLKDIV_DIV_MASK          (0xF << SARADC_CYC_CLKDIV_DIV_POS)
#define SARADC_CYC_CLKDIV_DIV_1             (0U<< SARADC_CYC_CLKDIV_DIV_POS)
#define SARADC_CYC_CLKDIV_DIV_2             (1U<< SARADC_CYC_CLKDIV_DIV_POS)
#define SARADC_CYC_CLKDIV_DIV_3             (2U<< SARADC_CYC_CLKDIV_DIV_POS)
#define SARADC_CYC_CLKDIV_DIV_4             (3U<< SARADC_CYC_CLKDIV_DIV_POS)
#define SARADC_CYC_CLKDIV_DIV_5             (4U<< SARADC_CYC_CLKDIV_DIV_POS)
#define SARADC_CYC_CLKDIV_DIV_6             (5U<< SARADC_CYC_CLKDIV_DIV_POS)
#define SARADC_CYC_CLKDIV_DIV_7             (6U<< SARADC_CYC_CLKDIV_DIV_POS)
#define SARADC_CYC_CLKDIV_DIV_8             (7U<< SARADC_CYC_CLKDIV_DIV_POS)
#define SARADC_CYC_CLKDIV_DIV_9             (8U<< SARADC_CYC_CLKDIV_DIV_POS)
#define SARADC_CYC_CLKDIV_DIV_10            (9U<< SARADC_CYC_CLKDIV_DIV_POS)
#define SARADC_CYC_CLKDIV_DIV_11            (10U<< SARADC_CYC_CLKDIV_DIV_POS)
#define SARADC_CYC_CLKDIV_DIV_12            (11U<< SARADC_CYC_CLKDIV_DIV_POS)
#define SARADC_CYC_CLKDIV_DIV_13            (12U<< SARADC_CYC_CLKDIV_DIV_POS)
#define SARADC_CYC_CLKDIV_DIV_14            (13U<< SARADC_CYC_CLKDIV_DIV_POS)
#define SARADC_CYC_CLKDIV_DIV_15            (14U<< SARADC_CYC_CLKDIV_DIV_POS)
#define SARADC_CYC_CLKDIV_DIV_16            (15U<< SARADC_CYC_CLKDIV_DIV_POS)

#define SARADC_RESULT_OFFSET                0x014
#define SARADC_RESULT(n)                    (SARADC_RESULT_OFFSET + (n) * 4)
#define SARADC_RESULT_MASK                  0x0FFF
#define SARADC_RESULT_VALID                 (1 << 15)

#define SARADC_INTR_EN_OFFSET                0x020  //bit0: interrupt enable, RW

#define SARADC_INTR_CLR_OFFSET               0x024  //bit0: interrupt clear, RWC 

#define SARADC_INTR_STA_OFFSET               0x028  //bit0: interrup masked status, RO,[0]:all channels measurenment in this time is finished

#define SARADC_INTR_RAW_OFFSET               0x02C  //bit 0: interrupt raw status, RO,[0]:all channels measurenment in this time is finished

#define ADC_DATA_QUEUE_LENGTH                10
#define ADC_INTR                             116

#define ADC_TIMEOUT_MS 1000  // 超时时间定义为1000毫秒

// ADC device structure
struct cvi_adc_dev
{
    const char *name;
    uint32_t base;
    uint32_t active_channel; 
};

int adc_sample(uint8_t adc_id, uint32_t channel, uint32_t *value);

#endif // __DRV_ADC_H__
