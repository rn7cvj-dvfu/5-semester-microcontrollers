#include "init.h"

void init_uart()
{
    UART_SetBaudrate(0, UART_BAUD_RATE);
}

void init_pwm()
{

    uint32 io_info[][3] = {
        {PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO4, 4},
        {PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO0, 0},
        {PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO2, 2},
        {PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12, 12},
    };

    u32 dutys[] = {0, 0, 0, 0};
    pwm_init(1000, dutys, 4, io_info);

    printf("PWM init\n");

    printf("Channel 0: GPIO5 | D1\n");
    printf("Channel 1: GPIO4 | D2\n");
    printf("Channel 2: GPIO0 | D3\n");
    printf("Channel 3: GPIO2 | D4\n");
}

uint32 user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;
    switch (size_map)
    {
    case FLASH_SIZE_4M_MAP_256_256:
        rf_cal_sec = 128 - 5;
        break;

    case FLASH_SIZE_8M_MAP_512_512:
        rf_cal_sec = 256 - 5;
        break;

    case FLASH_SIZE_16M_MAP_512_512:
    case FLASH_SIZE_16M_MAP_1024_1024:
        rf_cal_sec = 512 - 5;
        break;

    case FLASH_SIZE_32M_MAP_512_512:
    case FLASH_SIZE_32M_MAP_1024_1024:
        rf_cal_sec = 1024 - 5;
        break;

    default:
        rf_cal_sec = 0;
        break;
    }

    return rf_cal_sec;
}
