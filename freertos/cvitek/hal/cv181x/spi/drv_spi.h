#ifndef __DRV_SPI_H__
#define __DRV_SPI_H__

#include <stdint.h>
#include <stdbool.h>
#include "mmio.h"

#define SPI0 0x0
#define SPI1 0x1
#define SPI2 0x2
#define SPI3 0x3

#define SPI0_BASE 0x04180000
#define SPI1_BASE 0x04190000
#define SPI2_BASE 0x041A0000
#define SPI3_BASE 0x041B0000

#define SPI_IRQ_MSAK 0x3e //expect the transmit FIFO empty interrupt
#define SPI_FREQUENCY 187500000

/* Transmit FiFO Threshold Level */
#define SPI_TXFTLR 0xf

#define SPI_CTRL0_DATA_FREAM_SHIFT 0  //frame size
#define SPI_CTRL0_FREAM_FORMAT_SHIFT 4 
#define SPI_CTRL0_CPHA_SHIFT 6   //phase
#define SPI_CTRL0_CPOL_SHIFT 7  //polarity
#define SPI_CTRL0_TRANS_MODE 8  //mode
#define SPI_CTRL0_LOOP_SHIFT 11
#define SPI_CTRL0_CTRL_FREAM_SHIFT 12

struct cv1800_spi {
    uint8_t spi_id;

    uint8_t fifo_len;
    uint8_t data_width;

    const void *send_buf;
    void *recv_buf;

    const void *send_end;
    void *recv_end;

    struct spi_regs *reg;
};

struct spi_message{
    const void *send_buf;
    void *recv_buf;
    uint16_t length;
    struct spi_message *next;
};

struct spi_cfg{
    uint8_t tmode;  // 传输模式
    uint8_t data_width;    // 数据帧大小
    uint32_t freq;  // 通信频率
} ;

struct spi_regs {
    uint32_t spi_ctrl0;         // 0x00
    uint32_t spi_ctrl1;         // 0x04
    uint32_t spi_ssienr;        // 0x08
    uint32_t spi_mwcr;          // 0x0c
    uint32_t spi_ser;           // 0x10
    uint32_t spi_baudr;         // 0x14
    uint32_t spi_txftlr;        // 0x18
    uint32_t spi_rxftlr;        // 0x1c
    uint32_t spi_txflr;         // 0x20
    uint32_t spi_rxflr;         // 0x24
    uint32_t spi_sr;            // 0x28
    uint32_t spi_imr;           // 0x2c
    uint32_t spi_isr;           // 0x30
    uint32_t spi_risr;          // 0x34
    uint32_t spi_txoicr;        // 0x38
    uint32_t spi_rxoicr;        // 0x3c
    uint32_t spi_rxuicr;        // 0x40
    uint32_t spi_msticr;        // 0x44
    uint32_t spi_icr;           // 0x48
    uint32_t spi_dmacr;         // 0x4c
    uint32_t spi_dmatdlr;       // 0x50
    uint32_t spi_dmardlr;       // 0x54
    uint32_t spi_idr;           // 0x58
    uint32_t spi_version;       // 0x5c
    uint32_t spi_dr;            // 0x60
    uint32_t spi_rx_sample_dly; // 0xf0
    uint32_t spi_cs_override;   // 0xf4
};

/* clear irq */
static inline void spi_clear_irq(struct spi_regs *reg, uint32_t mode)
{
    mmio_write_32((uintptr_t)&reg->spi_imr, mode);
}

static inline void spi_enable_cs(struct spi_regs *reg, uint32_t enable)
{
    mmio_write_32((uintptr_t)&reg->spi_ser, enable ? 0x1 : 0x0);
}

static inline void spi_set_frequency(struct spi_regs *reg, uint32_t target_frequency)
{
    uint16_t baudr_value;

    /* Calculate the BAUDR value based on the target frequency */
    baudr_value = SPI_FREQUENCY / target_frequency;

    /* Ensure BAUDR is an even number and within the valid range */
    if (baudr_value % 2 != 0) {
        baudr_value++;  // Increment to make it even
    }

    /* Validate the range of BAUDR */
    if (baudr_value < 2) {
        baudr_value = 2;  // Set to minimum valid value
    } else if (baudr_value > 65534) {
        baudr_value = 65534;  // Set to maximum valid value
    }

    mmio_write_32((uintptr_t)&reg->spi_baudr, baudr_value);
}


static inline void spi_enable(struct spi_regs *reg, uint32_t enable)
{
    mmio_write_32((uintptr_t)&reg->spi_ssienr, enable ? 0x1 : 0x0);
}

void hal_spi_init(uint8_t spi_id, struct spi_cfg *cfg);
void spixfer(uint8_t spi_id, struct spi_message *message);
void test_spi(void);

#endif