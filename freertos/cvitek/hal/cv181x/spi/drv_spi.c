#include "drv_spi.h"
#include <stdio.h>
#include <stddef.h>
static struct cv1800_spi dw_spi[4];
static struct spi_regs *get_spi_base(uint8_t spi_id)
{
    struct spi_regs *spi_base = NULL;

    switch (spi_id) {
    case SPI0:
        spi_base = (struct spi_regs *)SPI0_BASE;
        break;
    case SPI1:
        spi_base = (struct spi_regs *)SPI1_BASE;
        break;
    case SPI2:
        spi_base = (struct spi_regs *)SPI2_BASE;
        break;
    case SPI3:
        spi_base = (struct spi_regs *)SPI3_BASE;
        break;
    }
    return spi_base;
}

int hw_spi_send(struct cv1800_spi *dev) {
    uint32_t txflr = mmio_read_32((uintptr_t)&dev->reg->spi_txflr);
    uint32_t max = dev->fifo_len - txflr;
    uint16_t value;

    while (max)
    {
        if (dev->send_end - dev->send_buf)
        {
            if (dev->data_width == 8)
                value = *(uint8_t *)(dev->send_buf);
            else
                value = *(uint16_t *)(dev->send_buf);
        }
        else
        {
            return 0;
        }

        mmio_write_32((uintptr_t)&dev->reg->spi_dr, value);
        dev->send_buf += dev->data_width >> 3;
        max--;
    }

    return 0;
}

int hw_spi_recv(struct cv1800_spi *dev) {
    uint32_t rxflr = mmio_read_32((uintptr_t)&dev->reg->spi_rxflr);
    uint32_t tem;
    int ret = rxflr;

    while (rxflr)
    {
        tem = mmio_read_32((uintptr_t)&dev->reg->spi_dr);

        if (dev->recv_buf < dev->recv_end)
        {
            if (dev->data_width == 8)
                *(uint8_t *)(dev->recv_buf) = tem;
            else
                *(uint16_t *)(dev->recv_buf) = tem;
        }
        else
        {
            return 0;
        }

        rxflr--;
        dev->recv_buf += dev->data_width >> 3;
    }

    return (int)ret;
}

static int dw_spi_transfer_poll(struct cv1800_spi *spi_dev, struct spi_message *message) {
    int ret = 0;
    
    if (message->send_buf != NULL)
    {
        spi_dev->send_buf = message->send_buf;
        spi_dev->send_end = (void *)((uint8_t *)spi_dev->send_buf + message->length);
    }

    if (message->recv_buf != NULL)
    {
        spi_dev->recv_buf = message->recv_buf;
        spi_dev->recv_end = (void *)((uint8_t *)spi_dev->recv_buf + message->length);
    }

    if (message->send_buf)
    {
        while (spi_dev->send_buf != spi_dev->send_end)
        {
            hw_spi_send(spi_dev);
        }

        /* wait for complete */
        while (mmio_read_32((uintptr_t)&spi_dev->reg->spi_txflr)) {}

        ret = message->length;
    }

    if (message->recv_buf)
    {
        while (spi_dev->recv_buf != spi_dev->recv_end)
        {
            hw_spi_recv(spi_dev);
        }

        ret = message->length;
    }

    return ret;
}

void spixfer(uint8_t spi_id, struct spi_message *message){
    struct cv1800_spi *spi_dev = &dw_spi[spi_id];
    dw_spi_transfer_poll(spi_dev, message);
}


// SPI控制器整体初始化
void hal_spi_init(uint8_t spi_id, struct spi_cfg *cfg)
{
    struct cv1800_spi *dws = &dw_spi[spi_id];
    dws->reg = get_spi_base(spi_id);
    if (dws->reg == NULL) {
        return;
    }

    dws->spi_id = spi_id;
    dws->fifo_len = SPI_TXFTLR;
    dws->data_width = cfg->data_width; 

    spi_enable(dws->reg, 0);
    spi_clear_irq(dws->reg, SPI_IRQ_MSAK);

    spi_set_frequency(dws->reg, cfg->freq);

    uint32_t mode = 0;
    if (cfg->data_width != 8 && cfg->data_width != 16)
            return -1;

     // 设置数据帧大小
    mode |= ((cfg->data_width - 1) & 0x0F);  // 根据数据宽度设置低四位
    // 设置CPOL和CPHA，假设cfg->tmode的低位为CPOL，次低位为CPHA
    mode |= ((cfg->tmode & 0x01) << 7);  // CPOL位设置
    mode |= ((cfg->tmode & 0x02) << 5);  // CPHA位设置，将tmode的第二位左移6位
    mmio_write_32((uintptr_t)&dws->reg->spi_ctrl0, mode);

    spi_enable_cs(dws->reg, 0x1);
    spi_enable(dws->reg, 0x1);
    
    mode = mmio_read_32((uintptr_t)&dws->reg->spi_ctrl0);
    printf("mode: %x", mode);
    mode = mmio_read_32((uintptr_t)&dws->reg->spi_baudr);
    printf("spi_baudr: %x", mode);
}

#include <string.h>  // For memcmp

// 测试数据
static uint8_t test_data[] = {0xAA, 0xBB, 0xCC, 0xDD};
static uint8_t recv_data[sizeof(test_data)];

void test_spi(void) {
    uint8_t spi_id = 0;  // 假设使用第一个SPI设备
    struct spi_cfg cfg;

    // 设置SPI配置
    cfg.freq = 1000000;  // 设置频率为1MHz
    cfg.data_width = 8;  // 设置数据宽度为8位
    cfg.tmode = 0;       // 设置为标准模式 (CPOL = 0, CPHA = 0)

    // 初始化SPI
    hal_spi_init(spi_id, &cfg);
    
    // 准备消息结构体
    struct spi_message message;
    message.send_buf = test_data;
    message.recv_buf = recv_data;
    message.length = sizeof(test_data);
    message.next = NULL;

    // 执行SPI传输
    spixfer(spi_id, &message);

    // 检查发送和接收的数据是否一致
    if (memcmp(test_data, recv_data, sizeof(test_data)) == 0) {
        printf("SPI transfer test PASSED.\n");
    } else {
        printf("SPI transfer test FAILED.\n");
    }

    // 可选：打印接收到的数据
    printf("Received Data:\n");
    for (int i = 0; i < sizeof(recv_data); i++) {
        printf("0x%X ", recv_data[i]);
    }
    printf("\n");
}
