#include "drv_adc.h"
#include "mmio.h"
#include "FreeRTOS.h"
#include "task.h"

static struct cvi_adc_dev adc_dev_config[] =
{
    {
        .name = "adc1",
        .base = SARADC_BASE,
        .active_channel = 0,
    },
};

// Configure SARADC control register
void cvi_set_saradc_ctrl(uint32_t reg_base, uint32_t value)
{
    value |= mmio_read_32(reg_base + SARADC_CTRL_OFFSET);
    mmio_write_32(reg_base + SARADC_CTRL_OFFSET, value);
}

// Reset SARADC control register
void cvi_reset_saradc_ctrl(uint32_t reg_base, uint32_t value)
{
    value = mmio_read_32(reg_base + SARADC_CTRL_OFFSET) & ~value;
    mmio_write_32(reg_base + SARADC_CTRL_OFFSET, value);
}

// Get SARADC status
uint32_t cvi_get_saradc_status(uint32_t reg_base)
{
    return mmio_read_32(reg_base + SARADC_STATUS_OFFSET);
}

// Set SARADC clock cycle
void cvi_set_cyc(uint32_t reg_base)
{
    uint32_t value = mmio_read_32(reg_base + SARADC_CYC_SET_OFFSET);
    value &= ~SARADC_CYC_CLKDIV_DIV_16;
    mmio_write_32(reg_base + SARADC_CYC_SET_OFFSET, value);
    value |= SARADC_CYC_CLKDIV_DIV_16;
    mmio_write_32(reg_base + SARADC_CYC_SET_OFFSET, value);
}

// Enable or disable ADC
int adc_enabled(struct cvi_adc_dev *adc_dev, uint32_t channel, uint8_t enabled)
{
    if (channel > SARADC_CH_MAX)
        return 1;

    if (enabled)
    {
        cvi_set_saradc_ctrl(adc_dev->base, channel << (SARADC_CTRL_SEL_POS+1));
        cvi_set_cyc(adc_dev->base);
        cvi_set_saradc_ctrl(adc_dev->base, SARADC_CTRL_START);
    }
    else
    {
        cvi_reset_saradc_ctrl(adc_dev->base, channel << (SARADC_CTRL_SEL_POS+1));
    }
    return 0;
}

// Perform an ADC conversion
int adc_convert(struct cvi_adc_dev *adc_dev, uint32_t channel, uint32_t *value)
{
    uint32_t result;
    uint32_t start_time = xTaskGetTickCount();

    if (channel > SARADC_CH_MAX)
        return -1;  // 错误码定义为 -1

    while (cvi_get_saradc_status(adc_dev->base) & SARADC_STATUS_BUSY)
    {
        if ((xTaskGetTickCount() - start_time) > pdMS_TO_TICKS(ADC_TIMEOUT_MS))
            return -2;  // 超时错误码定义为 -2

        vTaskDelay(pdMS_TO_TICKS(10));
    }

    result = mmio_read_32(adc_dev->base + SARADC_RESULT(channel - 1));
    if (result & SARADC_RESULT_VALID)
    {
        *value = result & SARADC_RESULT_MASK;
    }
    else
    {
        return -3;  // 无效结果错误码定义为 -3
    }
    return 0;
}

int adc_sample(uint8_t adc_id, uint32_t channel, uint32_t *value)
{
    if (adc_id >= sizeof(adc_dev_config) / sizeof(adc_dev_config[0]))
        return -1;  // ADC设备ID越界

    struct cvi_adc_dev *adc_dev = &adc_dev_config[adc_id];
    adc_dev->active_channel = channel;

    int ret = adc_enabled(adc_dev, adc_dev->active_channel, 1);
    if (ret != 0) {
        return ret;  // 启用ADC失败
    }

    ret = adc_convert(adc_dev, adc_dev->active_channel, value);
    adc_enabled(adc_dev, adc_dev->active_channel, 0);  // 确保停用ADC，忽略返回值

    return ret;  // 返回转换结果
}

