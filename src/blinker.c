#include "esp_common.h"

#include "blinker.h"

int toogle_mode(int mode)
{
    if (mode == PARALELL_MODE)
    {
        return SERIAL_MODE;
    }

    if (mode == SERIAL_MODE)
    {
        return PARALELL_MODE;
    }

    return PARALELL_MODE;
}

int toogle_step_delay(int step_delay)
{

    if (step_delay == 300)
    {
        return 500;
    }

    if (step_delay == 500)
    {
        return 1000;
    }

    if (step_delay == 1000)
    {
        return 300;
    }

    return 1000;
}

void blinker(void *params)
{

    struct BlinkerParams *p = (struct BlinkerParams *)params;

    while (1)
    {

        int32_t duty = count_duty(p);

        pwm_set_duty(duty, p->channelId);
        pwm_start();

        _delay(p);
        _increment_msc(p);
    }
}

void total_blinker(void *params)
{
    struct TotalBlinkerParams *p = (struct TotalBlinkerParams *)params;

    while (1)
    {
        for (int i = 0; i < p->count; i++)
        {
            struct BlinkerParams *bp = p->params + i;
            u32_t duty = count_duty(bp);
            pwm_set_duty(duty, bp->channelId);
        }

        pwm_start();

        for (int i = 0; i < p->count; i++)
        {
            BlinkerParams *bp = p->params + i;
            bp->msc += p->delay;
        }

        vTaskDelay(p->delay / portTICK_RATE_MS);
    }
}

void _increment_msc(struct BlinkerParams *p)
{
    if (p->mode == PARALELL_MODE)
    {
        p->msc += p->delay;
        return;
    }

    if (p->mode == SERIAL_MODE)
    {
        p->msc += p->step_delay;
        return;
    }

    p->msc += p->delay;
}

void _delay(struct BlinkerParams *p)
{
    if (p->mode == PARALELL_MODE)
    {
        vTaskDelay(p->delay / portTICK_RATE_MS);
        return;
    }

    if (p->mode == SERIAL_MODE)
    {
        vTaskDelay(p->step_delay / portTICK_RATE_MS);
        return;
    }

    vTaskDelay(p->delay / portTICK_RATE_MS);
}

int count_duty(BlinkerParams *p)
{
    if (p->mode == PARALELL_MODE)
    {
        return count_duty_paralell(p->lowValue, p->highValue, p->period, p->delay, p->msc);
    }

    if (p->mode == SERIAL_MODE)
    {
        return count_duty_serial(p->lowValue, p->highValue, p->step_mod, p->step_count, p->step_delay, p->msc);
    }

    return count_duty_paralell(p->lowValue, p->highValue, p->period, p->delay, p->msc);
}

int count_duty_serial(int32_t low, int32_t high, int32_t step_mod, int32_t step_count, int32_t step_delay, int32_t msc)
{

    int32_t step = msc / step_delay;

    step = step % step_count;

    if (step == step_mod)
    {
        return high;
    }

    return low;
}

int count_duty_paralell(int32_t low, int32_t high, int32_t period, int32_t delay, int32_t msc)
{

    msc = msc % period;

    msc += delay - msc % delay;

    int32_t half_period = period / 2;

    if (msc < half_period)
    {
        return low + (high - low) * msc / half_period;
    }
    else
    {
        return high - (high - low) * (msc - half_period) / half_period;
    }
}
