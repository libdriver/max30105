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
 * @file      driver_max30105_fifo_test.c
 * @brief     driver max30105 fifo test source file
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

#include "driver_max30105_fifo_test.h"

static max30105_handle_t gs_handle;        /**< max30105 handle */
static uint8_t gs_flag;                    /**< flag */
static uint32_t gs_raw_red[32];            /**< raw red buffer */
static uint32_t gs_raw_ir[32];             /**< raw ir buffer */
static uint32_t gs_raw_green[32];          /**< raw green buffer */

/**
 * @brief  fifo test irq handler
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t max30105_interface_fifo_test_irq_handler(void)
{
    /* run irq handler */
    if (max30105_irq_handler(&gs_handle))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     interface receive callback
 * @param[in] type is the irq type
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 * @note      none
 */
static uint8_t _max30105_interface_test_receive_callback(uint8_t type)
{
    switch (type)
    {
        case MAX30105_INTERRUPT_STATUS_FIFO_FULL :
        {
            volatile uint8_t res;
            volatile uint8_t len;
            
            /* read data */
            len = 32;
            res = max30105_read(&gs_handle, (uint32_t *)gs_raw_red, (uint32_t *)gs_raw_ir, (uint32_t *)gs_raw_green, (uint8_t *)&len);
            if (res)
            {
                max30105_interface_debug_print("max30105: read failed.\n");
               
                return 1;
            }
            max30105_interface_debug_print("max30105: irq fifo full with %d.\n", len);
            gs_flag = 1;
            
            break;
        }
        case MAX30105_INTERRUPT_STATUS_DATA_RDY :
        {
            max30105_interface_debug_print("max30105: irq data rdy.\n");
            
            break;
        }
        case MAX30105_INTERRUPT_STATUS_ALC_OVF :
        {
            max30105_interface_debug_print("max30105: irq alc ovf.\n");
            
            break;
        }
        case MAX30105_INTERRUPT_STATUS_PROX_INT :
        {
            max30105_interface_debug_print("max30105: irq proximity threshold.\n");
            
            break;
        }
        case MAX30105_INTERRUPT_STATUS_PWR_RDY :
        {
            max30105_interface_debug_print("max30105: irq pwr rdy.\n");
            
            break;
        }
        case MAX30105_INTERRUPT_STATUS_DIE_TEMP_RDY :
        {
            max30105_interface_debug_print("max30105: irq die temp rdy.\n");
            
            break;
        }
        default :
        {
            break;
        }
    }
    
    return 0;
}

/**
 * @brief     fifo test
 * @param[in] times is the test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t max30105_fifo_test(uint32_t times)
{
    volatile uint8_t res;
    volatile uint32_t i;
    volatile uint32_t timeout;
    volatile uint16_t raw;
    volatile uint8_t threshold;
    volatile float temp;
    max30105_bool_t enable;
    max30105_info_t info;
    
    /* link interface function */
    DRIVER_MAX30105_LINK_INIT(&gs_handle, max30105_handle_t);
    DRIVER_MAX30105_LINK_IIC_INIT(&gs_handle, max30105_interface_iic_init);
    DRIVER_MAX30105_LINK_IIC_DEINIT(&gs_handle, max30105_interface_iic_deinit);
    DRIVER_MAX30105_LINK_IIC_READ(&gs_handle, max30105_interface_iic_read);
    DRIVER_MAX30105_LINK_IIC_WRITE(&gs_handle, max30105_interface_iic_write);
    DRIVER_MAX30105_LINK_DELAY_MS(&gs_handle, max30105_interface_delay_ms);
    DRIVER_MAX30105_LINK_DEBUG_PRINT(&gs_handle, max30105_interface_debug_print);
    DRIVER_MAX30105_LINK_RECEIVE_CALLBACK(&gs_handle, _max30105_interface_test_receive_callback);
    
    /* get information */
    res = max30105_info(&info);
    if (res)
    {
        max30105_interface_debug_print("max30105: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip info */
        max30105_interface_debug_print("max30105: chip is %s.\n", info.chip_name);
        max30105_interface_debug_print("max30105: manufacturer is %s.\n", info.manufacturer_name);
        max30105_interface_debug_print("max30105: interface is %s.\n", info.interface);
        max30105_interface_debug_print("max30105: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        max30105_interface_debug_print("max30105: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        max30105_interface_debug_print("max30105: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        max30105_interface_debug_print("max30105: max current is %0.2fmA.\n", info.max_current_ma);
        max30105_interface_debug_print("max30105: max temperature is %0.1fC.\n", info.temperature_max);
        max30105_interface_debug_print("max30105: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start read test */
    max30105_interface_debug_print("max30105: start fifo test.\n");
    
    /* init the max30105 */
    res = max30105_init(&gs_handle);
    if (res)
    {
        max30105_interface_debug_print("max30105: init failed.\n");
       
        return 1;
    }

    /* enable shutdown */
    res = max30105_set_shutdown(&gs_handle, MAX30105_BOOL_TRUE);
    if (res)
    {
        max30105_interface_debug_print("max30105: set shutdown failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set fifo sample averaging */
    res = max30105_set_fifo_sample_averaging(&gs_handle, MAX30105_SAMPLE_AVERAGING_8);
    if (res)
    {
        max30105_interface_debug_print("max30105: set fifo sample averaging failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set fifo roll */
    res = max30105_set_fifo_roll(&gs_handle, MAX30105_BOOL_TRUE);
    if (res)
    {
        max30105_interface_debug_print("max30105: set fifo roll failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set fifo almost full */
    res = max30105_set_fifo_almost_full(&gs_handle, 0xF);
    if (res)
    {
        max30105_interface_debug_print("max30105: set fifo almost full failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set mode */
    res = max30105_set_mode(&gs_handle, MAX30105_MODE_GREEN_RED_IR);
    if (res)
    {
        max30105_interface_debug_print("max30105: set mode failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set particle sensing adc range */
    res = max30105_set_particle_sensing_adc_range(&gs_handle, MAX30105_PARTICLE_SENSING_ADC_RANGE_4096);
    if (res)
    {
        max30105_interface_debug_print("max30105: set particle sensing adc range failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set particle sensing sample rate */
    res = max30105_set_particle_sensing_sample_rate(&gs_handle, MAX30105_PARTICLE_SENSING_SAMPLE_RATE_100_HZ);
    if (res)
    {
        max30105_interface_debug_print("max30105: set particle sensing sample rate failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set adc resolution */
    res = max30105_set_adc_resolution(&gs_handle, MAX30105_ADC_RESOLUTION_18_BIT);
    if (res)
    {
        max30105_interface_debug_print("max30105: set adc resolution failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set led red pulse amplitude */
    res = max30105_set_led_red_pulse_amplitude(&gs_handle, 0x7F);
    if (res)
    {
        max30105_interface_debug_print("max30105: set led red pulse amplitude failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set led ir pulse amplitude */
    res = max30105_set_led_ir_pulse_amplitude(&gs_handle, 0x7F);
    if (res)
    {
        max30105_interface_debug_print("max30105: set led ir pulse amplitude failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set led green pulse amplitude */
    res = max30105_set_led_green_pulse_amplitude(&gs_handle, 0x7F);
    if (res)
    {
        max30105_interface_debug_print("max30105: set led green pulse amplitude failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set led proximity pulse amplitude */
    res = max30105_set_led_proximity_pulse_amplitude(&gs_handle, 0x7F);
    if (res)
    {
        max30105_interface_debug_print("max30105: set led proximity pulse amplitude failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* convert the adc */
    res = max30105_proximity_threshold_convert_to_register(&gs_handle, 1023, (uint8_t *)&threshold);
    if (res)
    {
        max30105_interface_debug_print("max30105: proximity threshold convert to register failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set proximity interrupt threshold */
    res = max30105_set_proximity_interrupt_threshold(&gs_handle, threshold);
    if (res)
    {
        max30105_interface_debug_print("max30105: set proximity interrupt threshold failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set slot */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_1, MAX30105_LED_RED_LED1_PA);
    if (res)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set slot */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_2, MAX30105_LED_IR_LED2_PA);
    if (res)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set slot */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_3, MAX30105_LED_GREEN_LED3_PA);
    if (res)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set slot */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_4, MAX30105_LED_NONE);
    if (res)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable die temperature */
    res = max30105_set_die_temperature(&gs_handle, MAX30105_BOOL_FALSE);
    if (res)
    {
        max30105_interface_debug_print("max30105: set die temperature failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set interrupt */
    res = max30105_set_interrupt(&gs_handle, MAX30105_INTERRUPT_FIFO_FULL_EN, MAX30105_BOOL_TRUE);
    if (res)
    {
        max30105_interface_debug_print("max30105: set interrupt failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set interrupt */
    res = max30105_set_interrupt(&gs_handle, MAX30105_INTERRUPT_DATA_RDY_EN, MAX30105_BOOL_FALSE);
    if (res)
    {
        max30105_interface_debug_print("max30105: set interrupt failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set interrupt */
    res = max30105_set_interrupt(&gs_handle, MAX30105_INTERRUPT_ALC_OVF_EN, MAX30105_BOOL_TRUE);
    if (res)
    {
        max30105_interface_debug_print("max30105: set interrupt failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set interrupt */
    res = max30105_set_interrupt(&gs_handle, MAX30105_INTERRUPT_PROX_INT_EN, MAX30105_BOOL_TRUE);
    if (res)
    {
        max30105_interface_debug_print("max30105: set interrupt failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set interrupt */
    res = max30105_set_interrupt(&gs_handle, MAX30105_INTERRUPT_DIE_TEMP_RDY_EN, MAX30105_BOOL_FALSE);
    if (res)
    {
        max30105_interface_debug_print("max30105: set interrupt failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable shutdown */
    res = max30105_set_shutdown(&gs_handle, MAX30105_BOOL_FALSE);
    if (res)
    {
        max30105_interface_debug_print("max30105: set shutdown failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }

    /* get status */
    res = max30105_get_interrupt_status(&gs_handle, MAX30105_INTERRUPT_STATUS_FIFO_FULL, &enable);
    if (res)
    {
        max30105_interface_debug_print("max30105: get interrupt status failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* get status */
    res = max30105_get_interrupt_status(&gs_handle, MAX30105_INTERRUPT_STATUS_DIE_TEMP_RDY, &enable);
    if (res)
    {
        max30105_interface_debug_print("max30105: get interrupt status failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* read temperature */
    res = max30105_read_temperature(&gs_handle, (uint16_t *)&raw, (float *)&temp);
    if (res)
    {
        max30105_interface_debug_print("max30105: read temperature failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: temperature is %0.4fC.\n", temp);
    
    /* read data */
    gs_flag = 0;
    timeout = 5000;
    while (timeout)
    {
        if (gs_flag)
        {
            /* clear config */
            gs_flag = 0;
            timeout = 5000;
            times--;
            if (times == 0)
            {
                break;
            }
        }
        max30105_interface_delay_ms(1);
        timeout--;
    }
    
    /* check timeout */
    if (timeout == 0)
    {
        max30105_interface_debug_print("max30105: read timeout failed.\n");
        max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* finish read test */
    max30105_interface_debug_print("max30105: finish fifo test.\n");
    max30105_deinit(&gs_handle);
    
    return 0;
}
