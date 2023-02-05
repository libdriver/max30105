/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_max30105.c
 * @brief     driver max30105 source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-01-01
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/01/01  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_max30105.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Maxim Integrated MAX30105"        /**< chip name */
#define MANUFACTURER_NAME         "Maxim Integrated"                 /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        1.7f                               /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        2.0f                               /**< chip max supply voltage */
#define MAX_CURRENT               50.0f                              /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                             /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                              /**< chip max operating temperature */
#define DRIVER_VERSION            1000                               /**< driver version */

/**
 * @brief iic address definition
 */
#define MAX30105_ADDRESS        0xAE        /**< iic address */

/**
 * @brief chip register definition
 */
#define MAX30105_REG_INTERRUPT_STATUS_1          0x00        /**< interrupt status 1 register */
#define MAX30105_REG_INTERRUPT_STATUS_2          0x01        /**< interrupt status 2 register */
#define MAX30105_REG_INTERRUPT_ENABLE_1          0x02        /**< interrupt enable 1 register */
#define MAX30105_REG_INTERRUPT_ENABLE_2          0x03        /**< interrupt enable 2 register */
#define MAX30105_REG_FIFO_WRITE_POINTER          0x04        /**< fifo write pointer register */
#define MAX30105_REG_OVERFLOW_COUNTER            0x05        /**< overflow counter register */
#define MAX30105_REG_FIFO_READ_POINTER           0x06        /**< fifo read pointer register */
#define MAX30105_REG_FIFO_DATA_REGISTER          0x07        /**< fifo data register */
#define MAX30105_REG_FIFO_CONFIG                 0x08        /**< fifo config register */
#define MAX30105_REG_MODE_CONFIG                 0x09        /**< mode config register */
#define MAX30105_REG_SPO2_CONFIG                 0x0A        /**< spo2 config register */
#define MAX30105_REG_LED_1_PA                    0x0C        /**< led 1 pa register */
#define MAX30105_REG_LED_2_PA                    0x0D        /**< led 2 pa register */
#define MAX30105_REG_LED_3_PA                    0x0E        /**< led 3 pa register */
#define MAX30105_REG_PILOT_PA                    0x10        /**< proximity mode led pulse amplitude register */
#define MAX30105_REG_MULTI_LED_MODE_CONTROL_1    0x11        /**< multi led mode control 1 register */
#define MAX30105_REG_MULTI_LED_MODE_CONTROL_2    0x12        /**< multi led mode control 2 register */
#define MAX30105_REG_DIE_TEMP_INTEGER            0x1F        /**< die temperature integer register */
#define MAX30105_REG_DIE_TEMP_FRACTION           0x20        /**< die temperature fraction register */
#define MAX30105_REG_DIE_TEMP_CONFIG             0x21        /**< die temperature config register */
#define MAX30105_REG_PROX_INT_THRESH             0x30        /**< proximity interrupt threshold */
#define MAX30105_REG_REVISION_ID                 0xFE        /**< revision id register */
#define MAX30105_REG_PART_ID                     0xFF        /**< part id register */

/**
 * @brief     initialize the chip
 * @param[in] *handle points to a max30105 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 id is invalid
 *            - 5 reset failed
 *            - 6 reset fifo failed
 * @note      none
 */
uint8_t max30105_init(max30105_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    uint8_t part_id;
    
    if (handle == NULL)                                                                                     /* check handle */
    {
        return 2;                                                                                           /* return error */
    }
    if (handle->debug_print == NULL)                                                                        /* check debug_print */
    {
        return 3;                                                                                           /* return error */
    }
    if (handle->iic_init == NULL)                                                                           /* check iic_init */
    {
        handle->debug_print("max30105: iic_init is null.\n");                                               /* iic_init is null */
        
        return 3;                                                                                           /* return error */
    }
    if (handle->iic_deinit == NULL)                                                                         /* check iic_deinit */
    {
        handle->debug_print("max30105: iic_deinit is null.\n");                                             /* iic_deinit is null */
        
        return 3;                                                                                           /* return error */
    }
    if (handle->iic_read == NULL)                                                                           /* check iic_read */
    {
        handle->debug_print("max30105: iic_read is null.\n");                                               /* iic_read is null */
        
        return 3;                                                                                           /* return error */
    }
    if (handle->iic_write == NULL)                                                                          /* check iic_write */
    {
        handle->debug_print("max30105: iic_write is null.\n");                                              /* iic_write is null */
        
        return 3;                                                                                           /* return error */
    }
    if (handle->receive_callback == NULL)                                                                   /* check receive_callback */
    {
        handle->debug_print("max30105: receive_callback is null.\n");                                       /* receive_callback is null */
        
        return 3;                                                                                           /* return error */
    }
    if (handle->delay_ms == NULL)                                                                           /* check delay_ms */
    {
        handle->debug_print("max30105: delay_ms is null.\n");                                               /* delay_ms is null */
        
        return 3;                                                                                           /* return error */
    }

    if (handle->iic_init() != 0)                                                                            /* init iic */
    {
        handle->debug_print("max30105: iic init failed.\n");                                                /* iic init failed */
        
        return 1;                                                                                           /* return error */
    }
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_PART_ID, (uint8_t *)&part_id, 1);                 /* read part id */
    if (res != 0)                                                                                           /* check result */
    {
        handle->debug_print("max30105: read part id failed.\n");                                            /* read part id failed */
        (void)handle->iic_deinit();                                                                         /* iic deinit */
        
        return 4;                                                                                           /* return error */
    }
    if (part_id != 0x15)                                                                                    /* check part id */
    {
        handle->debug_print("max30105: id is invalid.\n");                                                  /* id is invalid */
        (void)handle->iic_deinit();                                                                         /* iic deinit */
        
        return 4;                                                                                           /* return error */
    }
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_MODE_CONFIG, (uint8_t *)&prev, 1);                /* read mode config */
    if (res != 0)                                                                                           /* check result */
    {
        handle->debug_print("max30105: read mode config failed.\n");                                        /* read mode config failed */
        (void)handle->iic_deinit();                                                                         /* iic deinit */
        
        return 5;                                                                                           /* return error */
    }
    prev &= ~(1 << 6);                                                                                      /* clear config */
    prev |= 1 << 6;                                                                                         /* set 1 */
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_MODE_CONFIG, (uint8_t *)&prev, 1);               /* write mode config */
    if (res != 0)                                                                                           /* check result */
    {
        handle->debug_print("max30105: write mode config failed.\n");                                       /* write mode config failed */
        (void)handle->iic_deinit();                                                                         /* iic deinit */
        
        return 5;                                                                                           /* return error */
    }
    handle->delay_ms(10);                                                                                   /* delay 10 ms */
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_MODE_CONFIG, (uint8_t *)&prev, 1);                /* read mode config */
    if (res != 0)                                                                                           /* check result */
    {
        handle->debug_print("max30105: read mode config failed.\n");                                        /* read mode config failed */
        (void)handle->iic_deinit();                                                                         /* iic deinit */
        
        return 5;                                                                                           /* return error */
    }
    if ((prev & (1 << 6)) != 0)                                                                             /* check result */
    {
        handle->debug_print("max30105: reset failed.\n");                                                   /* reset failed */
        (void)handle->iic_deinit();                                                                         /* iic deinit */
        
        return 5;                                                                                           /* return error */
    }
    prev = 0;                                                                                               /* set zero */
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_FIFO_READ_POINTER, (uint8_t *)&prev, 1);         /* write fifo read pointer */
    if (res != 0)                                                                                           /* check result */
    {
        handle->debug_print("max30105: write fifo read pointer failed.\n");                                 /* write fifo read pointer failed */
        (void)handle->iic_deinit();                                                                         /* iic deinit */
        
        return 6;                                                                                           /* return error */
    }
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_FIFO_WRITE_POINTER, (uint8_t *)&prev, 1);        /* write fifo write pointer */
    if (res != 0)                                                                                           /* check result */
    {
        handle->debug_print("max30105: write fifo write pointer failed.\n");                                /* write fifo write pointer failed */
        (void)handle->iic_deinit();                                                                         /* iic deinit */
        
        return 6;                                                                                           /* return error */
    }
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_OVERFLOW_COUNTER, (uint8_t *)&prev, 1);          /* write overflow counter */
    if (res != 0)                                                                                           /* check result */
    {
        handle->debug_print("max30105: write overflow counter failed.\n");                                  /* write overflow counter failed */
        (void)handle->iic_deinit();                                                                         /* iic deinit */
        
        return 6;                                                                                           /* return error */
    }
    handle->inited = 1;                                                                                     /* flag finish initialization */
    
    return 0;                                                                                               /* success return 0 */
}

/**
 * @brief     close the chip
 * @param[in] *handle points to a max30105 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 power down failed
 * @note      none
 */
uint8_t max30105_deinit(max30105_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_MODE_CONFIG, (uint8_t *)&prev, 1);         /* read mode config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: read mode config failed.\n");                                 /* read mode config failed */
       
        return 4;                                                                                    /* return error */
    }
    prev &= ~(1 << 7);                                                                               /* clear config */
    prev |= 1 << 7;                                                                                  /* set bool */
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_MODE_CONFIG, (uint8_t *)&prev, 1);        /* write mode config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: write mode config failed.\n");                                /* write mode config failed */
       
        return 4;                                                                                    /* return error */
    }
    if (handle->iic_deinit() != 0)                                                                   /* iic deinit */
    {
        handle->debug_print("max30105: iic deinit failed.\n");                                       /* iic deinit failed */
        
        return 1;                                                                                    /* return error */
    }
    
    handle->inited = 0;                                                                              /* flag close */
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief     irq handler
 * @param[in] *handle points to a max30105 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_irq_handler(max30105_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                                            /* check handle */
    {
        return 2;                                                                                                  /* return error */
    }
    if (handle->inited != 1)                                                                                       /* check handle initialization */
    {
        return 3;                                                                                                  /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_INTERRUPT_STATUS_1, (uint8_t *)&prev, 1);                /* read interrupt status1 */
    if (res != 0)                                                                                                  /* check result */
    {
        handle->debug_print("max30105: read interrupt status1 failed.\n");                                         /* read interrupt status1 failed */
       
        return 1;                                                                                                  /* return error */
    }
    if ((prev & (1 << MAX30105_INTERRUPT_STATUS_FIFO_FULL)) != 0)                                                  /* check fifo full */
    {
        if (handle->receive_callback != NULL)                                                                      /* if receive callback */
        {
            handle->receive_callback(MAX30105_INTERRUPT_STATUS_FIFO_FULL);                                         /* run callback */
        }
    }
    if ((prev & (1 << MAX30105_INTERRUPT_STATUS_DATA_RDY)) != 0)                                                   /* check data ready */
    {
        if (handle->receive_callback != NULL)                                                                      /* if receive callback */
        {
            handle->receive_callback(MAX30105_INTERRUPT_STATUS_DATA_RDY);                                          /* run callback */
        }
    }
    if ((prev & (1 << MAX30105_INTERRUPT_STATUS_ALC_OVF)) != 0)                                                    /* check alc ovf */
    {
        if (handle->receive_callback != NULL)                                                                      /* if receive callback */
        {
            handle->receive_callback(MAX30105_INTERRUPT_STATUS_ALC_OVF);                                           /* run callback */
        }
    }
    if ((prev & (1 << MAX30105_INTERRUPT_STATUS_PROX_INT)) != 0)                                                   /* check proxy int */
    {
        if (handle->receive_callback != NULL)                                                                      /* if receive callback */
        {
            handle->receive_callback(MAX30105_INTERRUPT_STATUS_PROX_INT);                                          /* run callback */
        }
    }
    if ((prev & (1 << MAX30105_INTERRUPT_STATUS_PWR_RDY)) != 0)                                                    /* check pwr ready */
    {
        if (handle->receive_callback != NULL)                                                                      /* if receive callback */
        {
            handle->receive_callback(MAX30105_INTERRUPT_STATUS_PWR_RDY);                                           /* run callback */
        }
    }
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_INTERRUPT_STATUS_2, (uint8_t *)&prev, 1);                /* read interrupt status2 */
    if (res != 0)                                                                                                  /* check result */
    {
        handle->debug_print("max30105: read interrupt status2 failed.\n");                                         /* read interrupt status2 failed */
       
        return 1;                                                                                                  /* return error */
    }
    if ((prev & (1 << MAX30105_INTERRUPT_STATUS_DIE_TEMP_RDY)) != 0)                                               /* check die temp ready */
    {
        uint8_t prev1;
        
        res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_DIE_TEMP_INTEGER, (uint8_t *)&prev, 1);              /* read die temp integer */
        if (res != 0)                                                                                              /* check result */
        {
            handle->debug_print("max30105: read die temp integer failed.\n");                                      /* read die temp integer failed */
           
            return 1;                                                                                              /* return error */
        }
        handle->raw = (uint16_t)prev << 4;                                                                         /* set integer part */
        res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_DIE_TEMP_FRACTION, (uint8_t *)&prev1, 1);            /* read die temp fraction */
        if (res != 0)                                                                                              /* check result */
        {
            handle->debug_print("max30105: read die temp fraction failed.\n");                                     /* read die temp fraction failed */
           
            return 1;                                                                                              /* return error */
        }
        handle->raw = handle->raw | prev1;                                                                         /* set fraction part */
        handle->temperature = (float)(prev) + (float)(prev1) * 0.0625f;                                            /* set the temperature */
        handle->finished_flag = 1;                                                                                 /* set flag */
        
        if (handle->receive_callback != NULL)                                                                      /* if receive callback */
        {
            handle->receive_callback(MAX30105_INTERRUPT_STATUS_DIE_TEMP_RDY);                                      /* run callback */
        }
    }
    
    return 0;                                                                                                      /* success return 0 */
}

/**
 * @brief         read the data
 * @param[in]     *handle points to a max30105 handle structure
 * @param[out]    *raw_red points to a red raw data buffer
 * @param[out]    *raw_ir points to an ir raw data buffer
 * @param[out]    *raw_green points to a green raw data buffer
 * @param[in,out] *len points to a length buffer
 * @return        status code
 *                - 0 success
 *                - 1 read failed
 *                - 2 handle is NULL
 *                - 3 handle is not initialized
 *                - 4 fifo overrun
 *                - 5 mode is invalid
 * @note          none
 */
uint8_t max30105_read(max30105_handle_t *handle, uint32_t *raw_red, uint32_t *raw_ir, uint32_t *raw_green, uint8_t *len)
{
    uint8_t res;
    uint8_t prev;
    uint8_t mode;
    uint8_t k;
    uint8_t read_point;
    uint8_t write_point;
    uint8_t l;
    uint8_t bit;
    uint8_t i;
    uint8_t r;
    
    if (handle == NULL)                                                                                           /* check handle */
    {
        return 2;                                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                                      /* check handle initialization */
    {
        return 3;                                                                                                 /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_OVERFLOW_COUNTER, (uint8_t *)&prev, 1);                 /* read overflow counter */
    if (res != 0)                                                                                                 /* check result */
    {
        handle->debug_print("max30105: read overflow counter failed.\n");                                         /* read overflow counter failed */
       
        return 1;                                                                                                 /* return error */
    }
    r = 0;                                                                                                        /* set 0 */
    if (prev != 0)                                                                                                /* check overflow */
    {
        r = 4;                                                                                                    /* set 4 */
        
        handle->debug_print("max30105: fifo overrun.\n");                                                         /* fifo overrun*/
    }
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_FIFO_READ_POINTER, (uint8_t *)&read_point, 1);          /* read fifo read point */
    if (res != 0)                                                                                                 /* check result */
    {
        handle->debug_print("max30105: read fifo read point failed.\n");                                          /* read fifo read point failed */
       
        return 1;                                                                                                 /* return error */
    }
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_FIFO_WRITE_POINTER, (uint8_t *)&write_point, 1);        /* read fifo write point */
    if (res != 0)                                                                                                 /* check result */
    {
        handle->debug_print("max30105: read fifo write point failed.\n");                                         /* read fifo write point failed */
       
        return 1;                                                                                                 /* return error */
    }
    
    if (write_point > read_point)                                                                                 /* check point */
    {
        l = write_point - read_point;                                                                             /* get length */
    }
    else
    {
        l = 32 + write_point - read_point;                                                                        /* get length */
    }
    *len = ((*len) > l) ? l : (*len);                                                                             /* set read length */
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_MODE_CONFIG, (uint8_t *)&prev, 1);                      /* read mode config */
    if (res != 0)                                                                                                 /* check result */
    {
        handle->debug_print("max30105: read mode config failed.\n");                                              /* read mode config failed */
       
        return 1;                                                                                                 /* return error */
    }
    mode = (max30105_mode_t)(prev & 0x7);                                                                         /* get mode */
    if (mode == MAX30105_MODE_RED)                                                                                /* check red mode */
    {
        k = 3;                                                                                                    /* 3 */
    }
    else if (mode == MAX30105_MODE_RED_IR)                                                                        /* check red && ir mode*/
    {
        k = 6;                                                                                                    /* 6 */
    }
    else if (mode == MAX30105_MODE_GREEN_RED_IR)                                                                  /* check red && ir && green mode */
    {
        k = 9;                                                                                                    /* 9 */
    }
    else
    {
        handle->debug_print("max30105: mode is invalid.\n");                                                      /* mode is invalid */
       
        return 5;                                                                                                 /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_FIFO_DATA_REGISTER, handle->buf, (*len) * k);           /* read fifo read point */
    if (res != 0)                                                                                                 /* check result */
    {
        handle->debug_print("max30105: read fifo data register failed.\n");                                       /* read fifo data register failed */
       
        return 1;                                                                                                 /* return error */
    }
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_SPO2_CONFIG, (uint8_t *)&prev, 1);                      /* read spo2 config */
    if (res != 0)                                                                                                 /* check result */
    {
        handle->debug_print("max30105: read spo2 config failed.\n");                                              /* read spo2 config failed */
       
        return 1;                                                                                                 /* return error */
    }
    prev = prev & 0x3;                                                                                            /* get config */
    if (prev == 0)                                                                                                /* if 0 */
    {
        bit = 3;                                                                                                  /* 15 bits */
    }
    else if (prev == 1)                                                                                           /* if 1 */
    {
        bit = 2;                                                                                                  /* 16 bits */
    }
    else if (prev == 2)                                                                                           /* if 2 */
    {
        bit = 1;                                                                                                  /* 17 bits */
    }
    else                                                                                                          /* if 3 */
    {
        bit = 0;                                                                                                  /* 18 bits */
    }
    for (i = 0; i < (*len); i++)                                                                                  /* copy data */
    {
        if (mode == MAX30105_MODE_RED)                                                                            /* check red mode */
        {
            raw_red[i] = ((uint32_t)handle->buf[i * 3 + 0] << 16) |                                               /* get raw red data */
                         ((uint32_t)handle->buf[i * 3 + 1] << 8) |                                                /* get raw red data */
                         ((uint32_t)handle->buf[i * 3 + 2] << 0);                                                 /* get raw red data */
            raw_red[i] = raw_red[i] >> bit;                                                                       /* right shift bit */
        }
        else if (mode == MAX30105_MODE_RED_IR)                                                                    /* check red && ir mode*/
        {
            raw_red[i] = ((uint32_t)handle->buf[i * 6 + 0] << 16) |                                               /* get raw red data */
                         ((uint32_t)handle->buf[i * 6 + 1] << 8) |                                                /* get raw red data */
                         ((uint32_t)handle->buf[i * 6 + 2] << 0);                                                 /* get raw red data */
            raw_red[i] = raw_red[i] >> bit;                                                                       /* right shift bit */
            raw_ir[i] = ((uint32_t)handle->buf[i * 6 + 3] << 16) |                                                /* get raw ir data */
                        ((uint32_t)handle->buf[i * 6 + 4] << 8) |                                                 /* get raw ir data */
                        ((uint32_t)handle->buf[i * 6 + 5] << 0);                                                  /* get raw ir data */
            raw_ir[i] = raw_ir[i] >> bit;                                                                         /* right shift bit */
        }
        else
        {
            raw_red[i] = ((uint32_t)handle->buf[i * 9 + 0] << 16) |                                               /* get raw red data */
                         ((uint32_t)handle->buf[i * 9 + 1] << 8) |                                                /* get raw red data */
                         ((uint32_t)handle->buf[i * 9 + 2] << 0);                                                 /* get raw red data */
            raw_red[i] = raw_red[i] >> bit;                                                                       /* right shift bit */
            raw_ir[i] = ((uint32_t)handle->buf[i * 9 + 3] << 16) |                                                /* get raw ir data */
                        ((uint32_t)handle->buf[i * 9 + 4] << 8) |                                                 /* get raw ir data */
                        ((uint32_t)handle->buf[i * 9 + 5] << 0);                                                  /* get raw ir data */
            raw_ir[i] = raw_ir[i] >> bit;                                                                         /* right shift bit */
            raw_green[i] = ((uint32_t)handle->buf[i * 9 + 6] << 16) |                                             /* get raw green data */
                           ((uint32_t)handle->buf[i * 9 + 7] << 8) |                                              /* get raw green data */
                           ((uint32_t)handle->buf[i * 9 + 8] << 0);                                               /* get raw green data */
            raw_green[i] = raw_green[i] >> bit;                                                                   /* right shift bit */
        }
    }
    
    return r;                                                                                                     /* success return 0 */
}

/**
 * @brief      read the temperature
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *raw points to a raw data buffer
 * @param[out] *temp points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read temperature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_read_temperature(max30105_handle_t *handle, uint16_t *raw, float *temp)
{
    uint8_t res;
    uint8_t prev;
    uint16_t timeout;
    
    if (handle == NULL)                                                                                        /* check handle */
    {
        return 2;                                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                                   /* check handle initialization */
    {
        return 3;                                                                                              /* return error */
    }

    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_INTERRUPT_ENABLE_2, (uint8_t *)&prev, 1);            /* read interrupt enable2 */
    if (res != 0)                                                                                              /* check result */
    {
        handle->debug_print("max30105: read interrupt enable2 failed.\n");                                     /* read interrupt enable2 failed */
       
        return 1;                                                                                              /* return error */
    }
    if ((prev & (1 << 1)) == 0)                                                                                /* check config */
    {
        prev &= ~(1 << 1);                                                                                     /* clear interrupt */
        prev |= 1 << 1;                                                                                        /* set interrupt */
        res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_INTERRUPT_ENABLE_2, (uint8_t *)&prev, 1);       /* write interrupt enable2 */
        if (res != 0)                                                                                          /* check result */
        {
            handle->debug_print("max30105: write interrupt enable2 failed.\n");                                /* write interrupt enable2 failed */
           
            return 1;                                                                                          /* return error */
        }
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_DIE_TEMP_CONFIG, (uint8_t *)&prev, 1);               /* read die temp config */
    if (res != 0)                                                                                              /* check result */
    {
        handle->debug_print("max30105: read die temp config failed.\n");                                       /* read die temp config failed */
       
        return 1;                                                                                              /* return error */
    }
    prev &= ~(1 << 0);                                                                                         /* clear config */
    prev |= (1 << 0);                                                                                          /* set bool */
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_DIE_TEMP_CONFIG, (uint8_t *)&prev, 1);              /* write die temp config */
    if (res != 0)                                                                                              /* check result */
    {
        handle->debug_print("max30105: write die temp config failed.\n");                                      /* write die temp config failed */
       
        return 1;                                                                                              /* return error */
    }
    
    timeout = 5000;                                                                                            /* set 5000 ms */
    handle->finished_flag = 0;                                                                                 /* clear finished flag */
    while (timeout != 0)                                                                                       /* timeout */
    {
        handle->delay_ms(1);                                                                                   /* delay 1 ms */
        timeout--;                                                                                             /* timeout */
        if (handle->finished_flag != 0)                                                                        /* check finished flag */
        {
            break;                                                                                             /* break */
        }
    }
    if (timeout == 0)                                                                                          /* check timeout */
    {
        handle->debug_print("max30105: read timeout.\n");                                                      /* read timeout */
       
        return 1;                                                                                              /* return error */
    }
    *raw = handle->raw;                                                                                        /* get raw */
    *temp = handle->temperature;                                                                               /* get temperature */
    
    return 0;                                                                                                  /* success return 0 */
}

/**
 * @brief      get the interrupt status
 * @param[in]  *handle points to a max30105 handle structure
 * @param[in]  status is the interrupt status
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_interrupt_status(max30105_handle_t *handle, max30105_interrupt_status_t status, max30105_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                                        /* check handle */
    {
        return 2;                                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                                   /* check handle initialization */
    {
        return 3;                                                                                              /* return error */
    }

    if (status == MAX30105_INTERRUPT_STATUS_DIE_TEMP_RDY)                                                      /* if die temp ready status */
    {
        res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_INTERRUPT_STATUS_2, (uint8_t *)&prev, 1);        /* read interrupt status2 */
        if (res != 0)                                                                                          /* check result */
        {
            handle->debug_print("max30105: read interrupt status2 failed.\n");                                 /* read interrupt status2 failed */
           
            return 1;                                                                                          /* return error */
        }
        *enable = (max30105_bool_t)((prev >> status) & 0x01);                                                  /* get bool */
    
        return 0;                                                                                              /* success return 0 */
    }
    else
    {
        res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_INTERRUPT_STATUS_1, (uint8_t *)&prev, 1);        /* read interrupt status1 */
        if (res != 0)                                                                                          /* check result */
        {
            handle->debug_print("max30105: read interrupt status1 failed.\n");                                 /* read interrupt status1 failed */
           
            return 1;                                                                                          /* return error */
        }
        *enable = (max30105_bool_t)((prev >> status) & 0x01);                                                  /* get bool */
    
        return 0;                                                                                              /* success return 0 */
    }
}

/**
 * @brief     set the interrupt bool
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] type is the interrupt type
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_interrupt(max30105_handle_t *handle, max30105_interrupt_t type, max30105_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                                        /* check handle */
    {
        return 2;                                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                                   /* check handle initialization */
    {
        return 3;                                                                                              /* return error */
    }

    if (type == MAX30105_INTERRUPT_DIE_TEMP_RDY_EN)                                                            /* if internal temperature enable */
    {
        res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_INTERRUPT_ENABLE_2, (uint8_t *)&prev, 1);        /* read interrupt enable2 */
        if (res != 0)                                                                                          /* check result */
        {
            handle->debug_print("max30105: read interrupt enable2 failed.\n");                                 /* read interrupt enable2 failed */
           
            return 1;                                                                                          /* return error */
        }
        prev &= ~(1 << type);                                                                                  /* clear interrupt */
        prev |= enable << type;                                                                                /* set interrupt */
        res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_INTERRUPT_ENABLE_2, (uint8_t *)&prev, 1);       /* write interrupt enable2 */
        if (res != 0)                                                                                          /* check result */
        {
            handle->debug_print("max30105: write interrupt enable2 failed.\n");                                /* write interrupt enable2 failed */
           
            return 1;                                                                                          /* return error */
        }
        
        return 0;                                                                                              /* success return 0 */
    }
    else
    {
        res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_INTERRUPT_ENABLE_1, (uint8_t *)&prev, 1);        /* read interrupt enable1 */
        if (res != 0)                                                                                          /* check result */
        {
            handle->debug_print("max30105: read interrupt enable1 failed.\n");                                 /* read interrupt enable1 failed */
           
            return 1;                                                                                          /* return error */
        }
        prev &= ~(1 << type);                                                                                  /* clear interrupt */
        prev |= enable << type;                                                                                /* set interrupt */
        res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_INTERRUPT_ENABLE_1, (uint8_t *)&prev, 1);       /* write interrupt enable1 */
        if (res != 0)                                                                                          /* check result */
        {
            handle->debug_print("max30105: write interrupt enable1 failed.\n");                                /* write interrupt enable1 failed */
           
            return 1;                                                                                          /* return error */
        }
    
        return 0;                                                                                              /* success return 0 */
    }
}

/**
 * @brief      get the interrupt bool
 * @param[in]  *handle points to a max30105 handle structure
 * @param[in]  type is the interrupt type
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_interrupt(max30105_handle_t *handle, max30105_interrupt_t type, max30105_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                                        /* check handle */
    {
        return 2;                                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                                   /* check handle initialization */
    {
        return 3;                                                                                              /* return error */
    }

    if (type == MAX30105_INTERRUPT_DIE_TEMP_RDY_EN)                                                            /* if internal temperature enable */
    {
        res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_INTERRUPT_ENABLE_2, (uint8_t *)&prev, 1);        /* read interrupt enable2 */
        if (res != 0)                                                                                          /* check result */
        {
            handle->debug_print("max30105: read interrupt enable2 failed.\n");                                 /* read interrupt enable2 failed */
           
            return 1;                                                                                          /* return error */
        }
        *enable = (max30105_bool_t)((prev >> type) & 0x01);                                                    /* get bool */
    
        return 0;                                                                                              /* success return 0 */
    }
    else
    {
        res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_INTERRUPT_ENABLE_1, (uint8_t *)&prev, 1);        /* read interrupt enable1 */
        if (res != 0)                                                                                          /* check result */
        {
            handle->debug_print("max30105: read interrupt enable1 failed.\n");                                 /* read interrupt enable1 failed */
           
            return 1;                                                                                          /* return error */
        }
        *enable = (max30105_bool_t)((prev >> type) & 0x01);                                                    /* get bool */
    
        return 0;                                                                                              /* success return 0 */
    }
}

/**
 * @brief     set the fifo write pointer
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] pointer is the written pointer
 * @return    status code
 *            - 0 success
 *            - 1 set fifo write pointer failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 pointer can't be over 0x1F
 * @note      pointer <= 0x1F
 */
uint8_t max30105_set_fifo_write_pointer(max30105_handle_t *handle, uint8_t pointer)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                                     /* check handle */
    {
        return 2;                                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                                /* check handle initialization */
    {
        return 3;                                                                                           /* return error */
    }
    if (pointer > 0x1F)                                                                                     /* check pointer */
    {
        handle->debug_print("max30105: pointer can't be over 0x1F.\n");                                     /* pointer can't be over 0x1F */
       
        return 4;                                                                                           /* return error */
    }

    prev = pointer & 0x1F;                                                                                  /* set pointer */
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_FIFO_WRITE_POINTER, (uint8_t *)&prev, 1);        /* write fifo write pointer */
    if (res != 0)                                                                                           /* check result */
    {
        handle->debug_print("max30105: write fifo write pointer failed.\n");                                /* write fifo write pointer failed */
       
        return 1;                                                                                           /* return error */
    }
    
    return 0;                                                                                               /* success return 0 */
}

/**
 * @brief      get the fifo write pointer
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *pointer points to a pointer buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo write pointer failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_fifo_write_pointer(max30105_handle_t *handle, uint8_t *pointer)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                                    /* check handle */
    {
        return 2;                                                                                          /* return error */
    }
    if (handle->inited != 1)                                                                               /* check handle initialization */
    {
        return 3;                                                                                          /* return error */
    }

    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_FIFO_WRITE_POINTER, (uint8_t *)&prev, 1);        /* read fifo write pointer */
    if (res != 0)                                                                                          /* check result */
    {
        handle->debug_print("max30105: read fifo write pointer failed.\n");                                /* read fifo write pointer failed */
       
        return 1;                                                                                          /* return error */
    }
    *pointer = prev & 0x1F;                                                                                /* get pointer */
    
    return 0;                                                                                              /* success return 0 */
}

/**
 * @brief     set the fifo overflow counter
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] counter is the overflow counter
 * @return    status code
 *            - 0 success
 *            - 1 set fifo overflow counter failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 counter can't be over 0x1F
 * @note      counter <= 0x1F
 */
uint8_t max30105_set_fifo_overflow_counter(max30105_handle_t *handle, uint8_t counter)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                                   /* check handle */
    {
        return 2;                                                                                         /* return error */
    }
    if (handle->inited != 1)                                                                              /* check handle initialization */
    {
        return 3;                                                                                         /* return error */
    }
    if (counter > 0x1F)                                                                                   /* check counter */
    {
        handle->debug_print("max30105: counter can't be over 0x1F.\n");                                   /* counter can't be over 0x1F */
       
        return 4;                                                                                         /* return error */
    }

    prev = counter & 0x1F;                                                                                /* set counter */
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_OVERFLOW_COUNTER, (uint8_t *)&prev, 1);        /* set fifo overflow counter */
    if (res != 0)                                                                                         /* check result */
    {
        handle->debug_print("max30105: set fifo overflow counter failed.\n");                             /* set fifo overflow counter failed */
       
        return 1;                                                                                         /* return error */
    }
    
    return 0;                                                                                             /* success return 0 */
}

/**
 * @brief      get the fifo overflow counter
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *counter points to a counter buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo overflow counter failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_fifo_overflow_counter(max30105_handle_t *handle, uint8_t *counter)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }

    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_OVERFLOW_COUNTER, (uint8_t *)&prev, 1);        /* get fifo overflow counter */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("max30105: get fifo overflow counter failed.\n");                            /* get fifo overflow counter failed */
       
        return 1;                                                                                        /* return error */
    }
    *counter = prev & 0x1F;                                                                              /* get pointer */
    
    return 0;                                                                                            /* success return 0 */
}

/**
 * @brief     set the fifo read pointer
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] pointer is the read pointer
 * @return    status code
 *            - 0 success
 *            - 1 set fifo read pointer failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 pointer can't be over 0x1F
 * @note      pointer <= 0x1F
 */
uint8_t max30105_set_fifo_read_pointer(max30105_handle_t *handle, uint8_t pointer)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                                    /* check handle */
    {
        return 2;                                                                                          /* return error */
    }
    if (handle->inited != 1)                                                                               /* check handle initialization */
    {
        return 3;                                                                                          /* return error */
    }
    if (pointer > 0x1F)                                                                                    /* check pointer */
    {
        handle->debug_print("max30105: pointer can't be over 0x1F.\n");                                    /* pointer can't be over 0x1F */
       
        return 4;                                                                                          /* return error */
    }

    prev = pointer & 0x1F;                                                                                 /* set pointer */
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_FIFO_READ_POINTER, (uint8_t *)&prev, 1);        /* write fifo read pointer */
    if (res != 0)                                                                                          /* check result */
    {
        handle->debug_print("max30105: write fifo read pointer failed.\n");                                /* write fifo read pointer failed */
       
        return 1;                                                                                          /* return error */
    }
    
    return 0;                                                                                              /* success return 0 */
}

/**
 * @brief      get the fifo read pointer
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *pointer points to a pointer buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo read pointer failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_fifo_read_pointer(max30105_handle_t *handle, uint8_t *pointer)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                                   /* check handle */
    {
        return 2;                                                                                         /* return error */
    }
    if (handle->inited != 1)                                                                              /* check handle initialization */
    {
        return 3;                                                                                         /* return error */
    }

    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_FIFO_READ_POINTER, (uint8_t *)&prev, 1);        /* read fifo read pointer */
    if (res != 0)                                                                                         /* check result */
    {
        handle->debug_print("max30105: read fifo read pointer failed.\n");                                /* read fifo read pointer failed */
       
        return 1;                                                                                         /* return error */
    }
    *pointer = prev & 0x1F;                                                                               /* get pointer */
    
    return 0;                                                                                             /* success return 0 */
}

/**
 * @brief     set the fifo data
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] data is the fifo data
 * @return    status code
 *            - 0 success
 *            - 1 set fifo data failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_fifo_data(max30105_handle_t *handle, uint8_t data)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                     /* check handle */
    {
        return 2;                                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                                /* check handle initialization */
    {
        return 3;                                                                                           /* return error */
    }
    
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_FIFO_DATA_REGISTER, (uint8_t *)&data, 1);        /* write fifo data register */
    if (res != 0)                                                                                           /* check result */
    {
        handle->debug_print("max30105: write fifo data register failed.\n");                                /* write fifo data register failed */
       
        return 1;                                                                                           /* return error */
    }
    
    return 0;                                                                                               /* success return 0 */
}

/**
 * @brief      get the fifo data
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *data points to a fifo data buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo data failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_fifo_data(max30105_handle_t *handle, uint8_t *data)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                   /* check handle */
    {
        return 2;                                                                                         /* return error */
    }
    if (handle->inited != 1)                                                                              /* check handle initialization */
    {
        return 3;                                                                                         /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_FIFO_DATA_REGISTER, (uint8_t *)data, 1);        /* read fifo data register */
    if (res != 0)                                                                                         /* check result */
    {
        handle->debug_print("max30105: read fifo data register failed.\n");                               /* read fifo data register failed */
       
        return 1;                                                                                         /* return error */
    }
    
    return 0;                                                                                             /* success return 0 */
}

/**
 * @brief     set the fifo sample averaging
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] sample is the fifo sample averaging
 * @return    status code
 *            - 0 success
 *            - 1 set fifo sample averaging failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_fifo_sample_averaging(max30105_handle_t *handle, max30105_sample_averaging_t sample)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_FIFO_CONFIG, (uint8_t *)&prev, 1);         /* read fifo config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: read fifo config failed.\n");                                 /* read fifo config failed */
       
        return 1;                                                                                    /* return error */
    }
    prev &= ~(0x7 << 5);                                                                             /* clear config */
    prev |= sample << 5;                                                                             /* set sample */
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_FIFO_CONFIG, (uint8_t *)&prev, 1);        /* write fifo config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: write fifo config failed.\n");                                /* write fifo config failed */
       
        return 1;                                                                                    /* return error */
    }
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief      get the fifo sample averaging
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *sample points to a fifo sample averaging buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo sample averaging failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_fifo_sample_averaging(max30105_handle_t *handle, max30105_sample_averaging_t *sample)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_FIFO_CONFIG, (uint8_t *)&prev, 1);         /* read fifo config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: read fifo config failed.\n");                                 /* read fifo config failed */
       
        return 1;                                                                                    /* return error */
    }
    *sample = (max30105_sample_averaging_t)(0x7 & (prev >> 5));                                      /* get sample averaging */
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief     enable or disable the fifo roll 
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set fifo roll failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_fifo_roll(max30105_handle_t *handle, max30105_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_FIFO_CONFIG, (uint8_t *)&prev, 1);         /* read fifo config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: read fifo config failed.\n");                                 /* read fifo config failed */
       
        return 1;                                                                                    /* return error */
    }
    prev &= ~(0x1 << 4);                                                                             /* clear config */
    prev |= enable << 4;                                                                             /* set enable */
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_FIFO_CONFIG, (uint8_t *)&prev, 1);        /* write fifo config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: write fifo config failed.\n");                                /* write fifo config failed */
       
        return 1;                                                                                    /* return error */
    }
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief      get the fifo roll status
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo roll failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_fifo_roll(max30105_handle_t *handle, max30105_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_FIFO_CONFIG, (uint8_t *)&prev, 1);         /* read fifo config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: read fifo config failed.\n");                                 /* read fifo config failed */
       
        return 1;                                                                                    /* return error */
    }
    *enable = (max30105_bool_t)(0x1 & (prev >> 4));                                                  /* get bool */
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief     set the fifo almost full value
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] value is the fifo almost full value
 * @return    status code
 *            - 0 success
 *            - 1 set fifo almost full failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 value can't be over 0xF
 * @note      none
 */
uint8_t max30105_set_fifo_almost_full(max30105_handle_t *handle, uint8_t value)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    if (value > 0xF)                                                                                 /* check value */
    {
        handle->debug_print("max30105: value can't be over 0xF.\n");                                 /* value can't be over 0xF */
       
        return 4;                                                                                    /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_FIFO_CONFIG, (uint8_t *)&prev, 1);         /* read fifo config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: read fifo config failed.\n");                                 /* read fifo config failed */
       
        return 1;                                                                                    /* return error */
    }
    prev &= ~(0xF << 0);                                                                             /* clear config */
    prev |= value << 0;                                                                              /* set value */
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_FIFO_CONFIG, (uint8_t *)&prev, 1);        /* write fifo config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: write fifo config failed.\n");                                /* write fifo config failed */
       
        return 1;                                                                                    /* return error */
    }
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief      get the fifo almost full value
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *value points to a fifo almost full value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo almost full failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_get_fifo_almost_full(max30105_handle_t *handle, uint8_t *value)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                             /* check handle */
    {
        return 2;                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                        /* check handle initialization */
    {
        return 3;                                                                                   /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_FIFO_CONFIG, (uint8_t *)&prev, 1);        /* read fifo config */
    if (res != 0)                                                                                   /* check result */
    {
        handle->debug_print("max30105: read fifo config failed.\n");                                /* read fifo config failed */
       
        return 1;                                                                                   /* return error */
    }
    *value = prev & 0xF;                                                                            /* get value */
    
    return 0;                                                                                       /* success return 0 */
}

/**
 * @brief     set the shutdown
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set shutdown failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_shutdown(max30105_handle_t *handle, max30105_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_MODE_CONFIG, (uint8_t *)&prev, 1);         /* read mode config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: read mode config failed.\n");                                 /* read mode config failed */
       
        return 1;                                                                                    /* return error */
    }
    prev &= ~(1 << 7);                                                                               /* clear config */
    prev |= enable << 7;                                                                             /* set bool */
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_MODE_CONFIG, (uint8_t *)&prev, 1);        /* write mode config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: write mode config failed.\n");                                /* write mode config failed */
       
        return 1;                                                                                    /* return error */
    }
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief      get the shutdown
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get shutdown failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_shutdown(max30105_handle_t *handle, max30105_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_MODE_CONFIG, (uint8_t *)&prev, 1);         /* read mode config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: read mode config failed.\n");                                 /* read mode config failed */
       
        return 1;                                                                                    /* return error */
    }
    *enable = (max30105_bool_t)((prev >> 7) & 0x01);                                                 /* get bool */
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief     reset the chip
 * @param[in] *handle points to a max30105 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_reset(max30105_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_MODE_CONFIG, (uint8_t *)&prev, 1);         /* read mode config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: read mode config failed.\n");                                 /* read mode config failed */
       
        return 1;                                                                                    /* return error */
    }
    prev &= ~(1 << 6);                                                                               /* clear config */
    prev |= 1 << 6;                                                                                  /* set 1 */
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_MODE_CONFIG, (uint8_t *)&prev, 1);        /* write mode config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: write mode config failed.\n");                                /* write mode config failed */
       
        return 1;                                                                                    /* return error */
    }
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief     set the mode
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] mode is the chip mode
 * @return    status code
 *            - 0 success
 *            - 1 set mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_mode(max30105_handle_t *handle, max30105_mode_t mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_MODE_CONFIG, (uint8_t *)&prev, 1);         /* read mode config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: read mode config failed.\n");                                 /* read mode config failed */
       
        return 1;                                                                                    /* return error */
    }
    prev &= ~(7 << 0);                                                                               /* clear config */
    prev |= mode << 0;                                                                               /* set mode */
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_MODE_CONFIG, (uint8_t *)&prev, 1);        /* write mode config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: write mode config failed.\n");                                /* write mode config failed */
       
        return 1;                                                                                    /* return error */
    }
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief      get the mode
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *mode points to a chip mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_mode(max30105_handle_t *handle, max30105_mode_t *mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_MODE_CONFIG, (uint8_t *)&prev, 1);         /* read mode config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: read mode config failed.\n");                                 /* read mode config failed */
       
        return 1;                                                                                    /* return error */
    }
    *mode = (max30105_mode_t)(prev & 0x7);                                                           /* get mode */
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief     set the particle sensing adc range
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] range is the particle sensing adc range
 * @return    status code
 *            - 0 success
 *            - 1 set particle sensing adc range failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_particle_sensing_adc_range(max30105_handle_t *handle, max30105_particle_sensing_adc_range_t range)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_SPO2_CONFIG, (uint8_t *)&prev, 1);         /* read spo2 config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: read spo2 config failed.\n");                                 /* read spo2 config failed */
       
        return 1;                                                                                    /* return error */
    }
    prev &= ~(3 << 5);                                                                               /* clear config */
    prev |= range << 5;                                                                              /* set range */
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_SPO2_CONFIG, (uint8_t *)&prev, 1);        /* write spo2 config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: write spo2 config failed.\n");                                /* write spo2 config failed */
       
        return 1;                                                                                    /* return error */
    }
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief      get the particle sensing adc range
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *range points to a particle sensing adc range buffer
 * @return     status code
 *             - 0 success
 *             - 1 get particle sensing adc range failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_particle_sensing_adc_range(max30105_handle_t *handle, max30105_particle_sensing_adc_range_t *range)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_SPO2_CONFIG, (uint8_t *)&prev, 1);         /* read spo2 config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: read spo2 config failed.\n");                                 /* read spo2 config failed */
       
        return 1;                                                                                    /* return error */
    }
    *range = (max30105_particle_sensing_adc_range_t)((prev >> 5) & 0x3);                             /* get range */
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief     set the particle sensing sample rate
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] rate is the particle sensing sample rate
 * @return    status code
 *            - 0 success
 *            - 1 set particle sensing sample rate failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_particle_sensing_sample_rate(max30105_handle_t *handle, max30105_particle_sensing_sample_rate_t rate)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_SPO2_CONFIG, (uint8_t *)&prev, 1);         /* read spo2 config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: read spo2 config failed.\n");                                 /* read spo2 config failed */
       
        return 1;                                                                                    /* return error */
    }
    prev &= ~(7 << 2);                                                                               /* clear config */
    prev |= rate << 2;                                                                               /* set sample rate */
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_SPO2_CONFIG, (uint8_t *)&prev, 1);        /* write spo2 config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: write spo2 config failed.\n");                                /* write spo2 config failed */
       
        return 1;                                                                                    /* return error */
    }
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief      get the particle sensing sample rate
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *rate points to a particle sensing sample rate buffer
 * @return     status code
 *             - 0 success
 *             - 1 get particle sensing sample rate failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_particle_sensing_sample_rate(max30105_handle_t *handle, max30105_particle_sensing_sample_rate_t *rate)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                             /* check handle */
    {
        return 2;                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                        /* check handle initialization */
    {
        return 3;                                                                                   /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_SPO2_CONFIG, (uint8_t *)&prev, 1);        /* read spo2 config */
    if (res != 0)                                                                                   /* check result */
    {
        handle->debug_print("max30105: read spo2 config failed.\n");                                /* read spo2 config failed */
       
        return 1;                                                                                   /* return error */
    }
    *rate = (max30105_particle_sensing_sample_rate_t)((prev >> 2) & 0x7);                           /* get sample rate */
    
    return 0;                                                                                       /* success return 0 */
}

/**
 * @brief     set the adc resolution
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] resolution is the adc resolution
 * @return    status code
 *            - 0 success
 *            - 1 set adc resolution failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_adc_resolution(max30105_handle_t *handle, max30105_adc_resolution_t resolution)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_SPO2_CONFIG, (uint8_t *)&prev, 1);         /* read spo2 config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: read spo2 config failed.\n");                                 /* read spo2 config failed */
       
        return 1;                                                                                    /* return error */
    }
    prev &= ~(3 << 0);                                                                               /* clear config */
    prev |= resolution << 0;                                                                         /* set adc resolution */
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_SPO2_CONFIG, (uint8_t *)&prev, 1);        /* write spo2 config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: write spo2 config failed.\n");                                /* write spo2 config failed */
       
        return 1;                                                                                    /* return error */
    }
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief      get the adc resolution
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *resolution points to an adc resolution buffer
 * @return     status code
 *             - 0 success
 *             - 1 get adc resolution failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_adc_resolution(max30105_handle_t *handle, max30105_adc_resolution_t *resolution)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_SPO2_CONFIG, (uint8_t *)&prev, 1);         /* read spo2 config */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("max30105: read spo2 config failed.\n");                                 /* read spo2 config failed */
       
        return 1;                                                                                    /* return error */
    }
    *resolution = (max30105_adc_resolution_t)((prev >> 0) & 0x3);                                    /* set adc resolution */
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief     set the red led pulse amplitude
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] amp is the red led pulse amplitude
 * @return    status code
 *            - 0 success
 *            - 1 set led red pulse amplitude failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_led_red_pulse_amplitude(max30105_handle_t *handle, uint8_t amp)
{
    uint8_t res;
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_LED_1_PA, (uint8_t *)&amp, 1);        /* write led 1 pa*/
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("max30105: write led 1 pa failed.\n");                               /* write led 1 pa failed */
       
        return 1;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

/**
 * @brief      get the red led pulse amplitude
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *amp points to a red led pulse amplitude buffer
 * @return     status code
 *             - 0 success
 *             - 1 get led red pulse amplitude failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_led_red_pulse_amplitude(max30105_handle_t *handle, uint8_t *amp)
{
    uint8_t res;
    
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_LED_1_PA, (uint8_t *)amp, 1);        /* read led 1 pa */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("max30105: read led 1 pa failed.\n");                              /* read led 1 pa failed */
       
        return 1;                                                                              /* return error */
    }
    
    return 0;                                                                                  /* success return 0 */
}

/**
 * @brief     set the ir led pulse amplitude
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] amp is the ir led pulse amplitude
 * @return    status code
 *            - 0 success
 *            - 1 set led ir pulse amplitude failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_led_ir_pulse_amplitude(max30105_handle_t *handle, uint8_t amp)
{
    uint8_t res;
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_LED_2_PA, (uint8_t *)&amp, 1);        /* write led 2 pa */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("max30105: write led 2 pa failed.\n");                               /* write led 2 pa failed */
       
        return 1;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

/**
 * @brief      get the ir led pulse amplitude
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *amp points to an ir led pulse amplitude buffer
 * @return     status code
 *             - 0 success
 *             - 1 get led ir pulse amplitude failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_led_ir_pulse_amplitude(max30105_handle_t *handle, uint8_t *amp)
{
    uint8_t res;
    
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_LED_2_PA, (uint8_t *)amp, 1);        /* read led 2 pa */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("max30105: read led 2 pa failed.\n");                              /* read led 2 pa failed */
       
        return 1;                                                                              /* return error */
    }
    
    return 0;                                                                                  /* success return 0 */
}

/**
 * @brief     set the green led pulse amplitude
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] amp is the green led pulse amplitude
 * @return    status code
 *            - 0 success
 *            - 1 set led green pulse amplitude failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_led_green_pulse_amplitude(max30105_handle_t *handle, uint8_t amp)
{
    uint8_t res;
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_LED_3_PA, (uint8_t *)&amp, 1);        /* write led 3 pa */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("max30105: write led 3 pa failed.\n");                               /* write led 3 pa failed */
       
        return 1;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

/**
 * @brief      get the green led pulse amplitude
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *amp points to a green led pulse amplitude buffer
 * @return     status code
 *             - 0 success
 *             - 1 get led green pulse amplitude failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_led_green_pulse_amplitude(max30105_handle_t *handle, uint8_t *amp)
{
    uint8_t res;
    
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_LED_3_PA, (uint8_t *)amp, 1);        /* read led 3 pa */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("max30105: read led 3 pa failed.\n");                              /* read led 3 pa failed */
       
        return 1;                                                                              /* return error */
    }
    
    return 0;                                                                                  /* success return 0 */
}

/**
 * @brief     set the proximity led pulse amplitude
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] amp is the proximity led pulse amplitude
 * @return    status code
 *            - 0 success
 *            - 1 set led proximity pulse amplitude failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_led_proximity_pulse_amplitude(max30105_handle_t *handle, uint8_t amp)
{
    uint8_t res;
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_PILOT_PA, (uint8_t *)&amp, 1);        /* write led proximity pa */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("max30105: write led proximity pa failed.\n");                       /* write led proximity pa failed */
       
        return 1;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

/**
 * @brief      get the proximity led pulse amplitude
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *amp points to a proximity led pulse amplitude buffer
 * @return     status code
 *             - 0 success
 *             - 1 get led proximity pulse amplitude failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_led_proximity_pulse_amplitude(max30105_handle_t *handle, uint8_t *amp)
{
    uint8_t res;
    
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_PILOT_PA, (uint8_t *)amp, 1);        /* read led proximity pa */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("max30105: read led proximity pa failed.\n");                      /* read led proximity pa failed */
       
        return 1;                                                                              /* return error */
    }
    
    return 0;                                                                                  /* success return 0 */
}

/**
 * @brief     set the led slot
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] slot is the slot number
 * @param[in] led is the led mode
 * @return    status code
 *            - 0 success
 *            - 1 set slot failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_slot(max30105_handle_t *handle, max30105_slot_t slot, max30105_led_t led)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                                              /* check handle */
    {
        return 2;                                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                                         /* check handle initialization */
    {
        return 3;                                                                                                    /* return error */
    }
    
    if (slot == MAX30105_SLOT_1)                                                                                     /* slot 1 */
    {
        res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_MULTI_LED_MODE_CONTROL_1, (uint8_t *)&prev, 1);        /* read led slot */
        if (res != 0)                                                                                                /* check result */
        {
            handle->debug_print("max30105: read led slot failed.\n");                                                /* read led slot failed */
           
            return 1;                                                                                                /* return error */
        }
        prev &= ~(0x7 << 0);                                                                                         /* clear config */
        prev |= led << 0;                                                                                            /* set led */
        res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_MULTI_LED_MODE_CONTROL_1, (uint8_t *)&prev, 1);       /* write led slot */
        if (res != 0)                                                                                                /* check result */
        {
            handle->debug_print("max30105: write led slot failed.\n");                                               /* write led slot failed */
           
            return 1;                                                                                                /* return error */
        }
        
        return 0;                                                                                                    /* success return 0 */
    }
    else if (slot == MAX30105_SLOT_2)                                                                                /* slot 2 */
    {
        res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_MULTI_LED_MODE_CONTROL_1, (uint8_t *)&prev, 1);        /* read led slot */
        if (res != 0)                                                                                                /* check result */
        {
            handle->debug_print("max30105: read led slot failed.\n");                                                /* read led slot failed */
           
            return 1;                                                                                                /* return error */
        }
        prev &= ~(0x7 << 4);                                                                                         /* clear config */
        prev |= led << 4;                                                                                            /* set led */
        res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_MULTI_LED_MODE_CONTROL_1, (uint8_t *)&prev, 1);       /* write led slot */
        if (res != 0)                                                                                                /* check result */
        {
            handle->debug_print("max30105: write led slot failed.\n");                                               /* write led slot failed */
           
            return 1;                                                                                                /* return error */
        }
        
        return 0;                                                                                                    /* success return 0 */
    }
    else if (slot == MAX30105_SLOT_3)                                                                                /* slot 3 */
    {
        res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_MULTI_LED_MODE_CONTROL_2, (uint8_t *)&prev, 1);        /* read led slot */
        if (res != 0)                                                                                                /* check result */
        {
            handle->debug_print("max30105: read led slot failed.\n");                                                /* read led slot failed */
           
            return 1;                                                                                                /* return error */
        }
        prev &= ~(0x7 << 0);                                                                                         /* clear config */
        prev |= led << 0;                                                                                            /* set led */
        res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_MULTI_LED_MODE_CONTROL_2, (uint8_t *)&prev, 1);       /* write led slot */
        if (res != 0)                                                                                                /* check result */
        {
            handle->debug_print("max30105: write led slot failed.\n");                                               /* write led slot failed */
           
            return 1;                                                                                                /* return error */
        }
        
        return 0;                                                                                                    /* success return 0 */
    }
    else if (slot == MAX30105_SLOT_4)                                                                                /* slot 4 */
    {
        res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_MULTI_LED_MODE_CONTROL_2, (uint8_t *)&prev, 1);        /* read led slot */
        if (res != 0)                                                                                                /* check result */
        {
            handle->debug_print("max30105: read led slot failed.\n");                                                /* read led slot failed */
           
            return 1;                                                                                                /* return error */
        }
        prev &= ~(0x7 << 4);                                                                                         /* clear config */
        prev |= led << 4;                                                                                            /* set led */
        res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_MULTI_LED_MODE_CONTROL_2, (uint8_t *)&prev, 1);       /* write led slot */
        if (res != 0)                                                                                                /* check result */
        {
            handle->debug_print("max30105: write led slot failed.\n");                                               /* write led slot failed */
           
            return 1;                                                                                                /* return error */
        }
        
        return 0;                                                                                                    /* success return 0 */
    }
    else                                                                                                             /* unknown */
    {
        handle->debug_print("max30105: slot is invalid.\n");                                                         /* slot is invalid */
       
        return 1;                                                                                                    /* return error */
    }
}

/**
 * @brief      get the led slot
 * @param[in]  *handle points to a max30105 handle structure
 * @param[in]  slot is the slot number
 * @param[out] *led points to a led mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get slot failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_slot(max30105_handle_t *handle, max30105_slot_t slot, max30105_led_t *led)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                                              /* check handle */
    {
        return 2;                                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                                         /* check handle initialization */
    {
        return 3;                                                                                                    /* return error */
    }
    
    if (slot == MAX30105_SLOT_1)                                                                                     /* slot 1 */
    {
        res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_MULTI_LED_MODE_CONTROL_1, (uint8_t *)&prev, 1);        /* read led slot */
        if (res != 0)                                                                                                /* check result */
        {
            handle->debug_print("max30105: read led slot failed.\n");                                                /* read led slot failed */
           
            return 1;                                                                                                /* return error */
        }
        *led = (max30105_led_t)((prev >> 0) & 0x7);                                                                  /* get led */
        
        return 0;                                                                                                    /* success return 0 */
    }
    else if (slot == MAX30105_SLOT_2)                                                                                /* slot 2 */
    {
        res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_MULTI_LED_MODE_CONTROL_1, (uint8_t *)&prev, 1);        /* read led slot */
        if (res != 0)                                                                                                /* check result */
        {
            handle->debug_print("max30105: read led slot failed.\n");                                                /* read led slot failed */
           
            return 1;                                                                                                /* return error */
        }
        *led = (max30105_led_t)((prev >> 4) & 0x7);                                                                  /* get led */
        
        return 0;                                                                                                    /* success return 0 */
    }
    else if (slot == MAX30105_SLOT_3)                                                                                /* slot 3 */
    {
        res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_MULTI_LED_MODE_CONTROL_2, (uint8_t *)&prev, 1);        /* read led slot */
        if (res != 0)                                                                                                /* check result */
        {
            handle->debug_print("max30105: read led slot failed.\n");                                                /* read led slot failed */
           
            return 1;                                                                                                /* return error */
        }
        *led = (max30105_led_t)((prev >> 0) & 0x7);                                                                  /* get led */
        
        return 0;                                                                                                    /* success return 0 */
    }
    else if (slot == MAX30105_SLOT_4)                                                                                /* slot 4 */
    {
        res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_MULTI_LED_MODE_CONTROL_2, (uint8_t *)&prev, 1);        /* read led slot */
        if (res != 0)                                                                                                /* check result */
        {
            handle->debug_print("max30105: read led slot failed.\n");                                                /* read led slot failed */
           
            return 1;                                                                                                /* return error */
        }
        *led = (max30105_led_t)((prev >> 4) & 0x7);                                                                  /* get led */
        
        return 0;                                                                                                    /* success return 0 */
    }
    else                                                                                                             /* unknown */
    {
        handle->debug_print("max30105: slot is invalid.\n");                                                         /* slot is invalid */
       
        return 1;                                                                                                    /* return error */
    }
}

/**
 * @brief     enable or disable die temperature
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set die temperature failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_die_temperature(max30105_handle_t *handle, max30105_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_DIE_TEMP_CONFIG, (uint8_t *)&prev, 1);         /* read die temp config */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("max30105: read die temp config failed.\n");                                 /* read die temp config failed */
       
        return 1;                                                                                        /* return error */
    }
    prev &= ~(1 << 0);                                                                                   /* clear config */
    prev |= (enable << 0);                                                                               /* set bool */
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_DIE_TEMP_CONFIG, (uint8_t *)&prev, 1);        /* write die temp config */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("max30105: write die temp config failed.\n");                                /* write die temp config failed */
       
        return 1;                                                                                        /* return error */
    }
    
    return 0;                                                                                            /* success return 0 */
}

/**
 * @brief      get the die temperature status
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get die temperature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_die_temperature(max30105_handle_t *handle, max30105_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_DIE_TEMP_CONFIG, (uint8_t *)&prev, 1);         /* read die temp config */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("max30105: read die temp config failed.\n");                                 /* read die temp config failed */
       
        return 1;                                                                                        /* return error */
    }
    *enable = (max30105_bool_t)((prev >> 0) & 0x1);                                                      /* get bool */
    
    return 0;                                                                                            /* success return 0 */
}

/**
 * @brief     set the proximity interrupt threshold
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] threshold is the proximity interrupt threshold
 * @return    status code
 *            - 0 success
 *            - 1 set proximity interrupt threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_proximity_interrupt_threshold(max30105_handle_t *handle, uint8_t threshold)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                       /* check handle */
    {
        return 2;                                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                                  /* check handle initialization */
    {
        return 3;                                                                                             /* return error */
    }
    
    res = handle->iic_write(MAX30105_ADDRESS, MAX30105_REG_PROX_INT_THRESH, (uint8_t *)&threshold, 1);        /* write proximity interrupt threshold */
    if (res != 0)                                                                                             /* check result */
    {
        handle->debug_print("max30105: write proximity interrupt threshold failed.\n");                       /* write read proximity interrupt threshold failed */
       
        return 1;                                                                                             /* return error */
    }

    return 0;                                                                                                 /* success return 0 */
}

/**
 * @brief      get the proximity interrupt threshold
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *threshold points to a proximity interrupt threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get proximity interrupt threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_proximity_interrupt_threshold(max30105_handle_t *handle, uint8_t *threshold)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                     /* check handle */
    {
        return 2;                                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                                /* check handle initialization */
    {
        return 3;                                                                                           /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_PROX_INT_THRESH, (uint8_t *)threshold, 1);        /* read proximity interrupt threshold */
    if (res != 0)                                                                                           /* check result */
    {
        handle->debug_print("max30105: read proximity interrupt threshold failed.\n");                      /* read read proximity interrupt threshold failed */
       
        return 1;                                                                                           /* return error */
    }

    return 0;                                                                                               /* success return 0 */
}

/**
 * @brief      convert the proximity threshold to the register raw data
 * @param[in]  *handle points to a max30105 handle structure
 * @param[in]  adc is the adc range
 * @param[out] *reg points to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_proximity_threshold_convert_to_register(max30105_handle_t *handle, uint32_t adc, uint8_t *reg)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *reg = (uint8_t)(adc / 1023);   /* convert real data to register data */
    
    return 0;                       /* success return 0 */
}

/**
 * @brief      convert the register raw data to the proximity threshold
 * @param[in]  *handle points to a max30105 handle structure
 * @param[in]  reg is the register raw data
 * @param[out] *adc points to an adc range buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_proximity_threshold_convert_to_data(max30105_handle_t *handle, uint8_t reg, uint32_t *adc)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *adc = (uint32_t)(reg * 1023);  /* convert raw data to real data */
    
    return 0;                       /* success return 0 */
}

/**
 * @brief      get the chip id
 * @param[in]  *handle points to a max30105 handle structure
 * @param[out] *revision_id points to a revision id buffer
 * @param[out] *part_id points to a part id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get id failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_id(max30105_handle_t *handle, uint8_t *revision_id, uint8_t *part_id)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                   /* check handle */
    {
        return 2;                                                                                         /* return error */
    }
    if (handle->inited != 1)                                                                              /* check handle initialization */
    {
        return 3;                                                                                         /* return error */
    }
    
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_REVISION_ID, (uint8_t *)revision_id, 1);        /* read revision id */
    if (res != 0)                                                                                         /* check result */
    {
        handle->debug_print("max30105: read revision id failed.\n");                                      /* read revision id failed */
       
        return 1;                                                                                         /* return error */
    }
    res = handle->iic_read(MAX30105_ADDRESS, MAX30105_REG_PART_ID, (uint8_t *)part_id, 1);                /* read part id */
    if (res != 0)                                                                                         /* check result */
    {
        handle->debug_print("max30105: read part id failed.\n");                                          /* read part id failed */
       
        return 1;                                                                                         /* return error */
    }
    
    return 0;                                                                                             /* success return 0 */
}

/**
 * @brief     set the chip register
 * @param[in] *handle points to a max30105 handle structure
 * @param[in] reg is the iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max30105_set_reg(max30105_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                            /* check handle */
    {
        return 2;                                                  /* return error */
    }
    if (handle->inited != 1)                                       /* check handle initialization */
    {
        return 3;                                                  /* return error */
    }
    
    if (handle->iic_write(MAX30105_ADDRESS, reg, buf, len) != 0)   /* write data */
    {
        return 1;                                                  /* return error */
    }
    else
    {
        return 0;                                                  /* success return 0 */
    }
}

/**
 * @brief      get the chip register
 * @param[in]  *handle points to a max30105 handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max30105_get_reg(max30105_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                           /* check handle */
    {
        return 2;                                                 /* return error */
    }
    if (handle->inited != 1)                                      /* check handle initialization */
    {
        return 3;                                                 /* return error */
    }
    
    if (handle->iic_read(MAX30105_ADDRESS, reg, buf, len) != 0)   /* read data */
    {
        return 1;                                                 /* return error */
    }
    else
    {
        return 0;                                                 /* success return 0 */
    }
}

/**
 * @brief      get chip's information
 * @param[out] *info points to a max30105 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t max30105_info(max30105_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(max30105_info_t));                       /* initialize max30105 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
