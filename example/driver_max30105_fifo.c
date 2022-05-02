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
 * @file      driver_max30105_fifo.c
 * @brief     driver max30105 fifo source file
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

#include "driver_max30105_fifo.h"

static max30105_handle_t gs_handle;        /**< max30105 handle */

/**
 * @brief  fifo example irq handler
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t max30105_fifo_irq_handler(void)
{
    /* run irq handler */
    if (max30105_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     fifo example init
 * @param[in] *fifo_receive_callback points to a fifo receive callback
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t max30105_fifo_init(void (*fifo_receive_callback)(uint8_t type))
{
    uint8_t res;
    uint8_t threshold;
    max30105_bool_t enable;
    
    /* link interface function */
    DRIVER_MAX30105_LINK_INIT(&gs_handle, max30105_handle_t);
    DRIVER_MAX30105_LINK_IIC_INIT(&gs_handle, max30105_interface_iic_init);
    DRIVER_MAX30105_LINK_IIC_DEINIT(&gs_handle, max30105_interface_iic_deinit);
    DRIVER_MAX30105_LINK_IIC_READ(&gs_handle, max30105_interface_iic_read);
    DRIVER_MAX30105_LINK_IIC_WRITE(&gs_handle, max30105_interface_iic_write);
    DRIVER_MAX30105_LINK_DELAY_MS(&gs_handle, max30105_interface_delay_ms);
    DRIVER_MAX30105_LINK_DEBUG_PRINT(&gs_handle, max30105_interface_debug_print);
    DRIVER_MAX30105_LINK_RECEIVE_CALLBACK(&gs_handle, fifo_receive_callback);
    
    /* init the max30105 */
    res = max30105_init(&gs_handle);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: init failed.\n");
       
        return 1;
    }
    
    /* enable shutdown */
    res = max30105_set_shutdown(&gs_handle, MAX30105_BOOL_TRUE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set shutdown failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set fifo sample averaging */
    res = max30105_set_fifo_sample_averaging(&gs_handle, MAX30105_FIFO_DEFAULT_SAMPLE_AVERAGING);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set fifo sample averaging failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set fifo roll */
    res = max30105_set_fifo_roll(&gs_handle, MAX30105_FIFO_DEFAULT_FIFO_ROLL);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set fifo roll failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set fifo almost full */
    res = max30105_set_fifo_almost_full(&gs_handle, MAX30105_FIFO_DEFAULT_FIFO_ALMOST_FULL);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set fifo almost full failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set mode */
    res = max30105_set_mode(&gs_handle, MAX30105_FIFO_DEFAULT_MODE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set mode failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set particle sensing adc range */
    res = max30105_set_particle_sensing_adc_range(&gs_handle, MAX30105_FIFO_DEFAULT_PARTICLE_SENSING_ADC_RANGE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set particle sensing adc range failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set particle sensing sample rate */
    res = max30105_set_particle_sensing_sample_rate(&gs_handle, MAX30105_FIFO_DEFAULT_PARTICLE_SENSING_SAMPLE_RATE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set particle sensing sample rate failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set adc resolution */
    res = max30105_set_adc_resolution(&gs_handle, MAX30105_FIFO_DEFAULT_ADC_RESOLUTION);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set adc resolution failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set led red pulse amplitude */
    res = max30105_set_led_red_pulse_amplitude(&gs_handle, MAX30105_FIFO_DEFAULT_LED_RED_PULSE_AMPLITUDE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set led red pulse amplitude failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set led ir pulse amplitude */
    res = max30105_set_led_ir_pulse_amplitude(&gs_handle, MAX30105_FIFO_DEFAULT_LED_IR_PULSE_AMPLITUDE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set led ir pulse amplitude failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set led green pulse amplitude */
    res = max30105_set_led_green_pulse_amplitude(&gs_handle, MAX30105_FIFO_DEFAULT_LED_GREEN_PULSE_AMPLITUDE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set led green pulse amplitude failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set led proximity pulse amplitude */
    res = max30105_set_led_proximity_pulse_amplitude(&gs_handle, MAX30105_FIFO_DEFAULT_LED_PROXIMITY_PULSE_AMPLITUDE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set led proximity pulse amplitude failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* convert the adc */
    res = max30105_proximity_threshold_convert_to_register(&gs_handle, MAX30105_FIFO_DEFAULT_PROXIMITY_THRESHOLD, (uint8_t *)&threshold);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: proximity threshold convert to register failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set proximity interrupt threshold */
    res = max30105_set_proximity_interrupt_threshold(&gs_handle, threshold);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set proximity interrupt threshold failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set slot */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_1, MAX30105_FIFO_DEFAULT_SLOT1);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set slot */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_2, MAX30105_FIFO_DEFAULT_SLOT2);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set slot */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_3, MAX30105_FIFO_DEFAULT_SLOT3);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set slot */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_4, MAX30105_FIFO_DEFAULT_SLOT4);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set die temperature */
    res = max30105_set_die_temperature(&gs_handle, MAX30105_FIFO_DEFAULT_DIE_TEMPERATURE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set die temperature failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set interrupt */
    res = max30105_set_interrupt(&gs_handle, MAX30105_INTERRUPT_FIFO_FULL_EN, MAX30105_BOOL_TRUE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set interrupt */
    res = max30105_set_interrupt(&gs_handle, MAX30105_INTERRUPT_DATA_RDY_EN, MAX30105_FIFO_DEFAULT_INTERRUPT_DATA_RDY_EN);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set interrupt */
    res = max30105_set_interrupt(&gs_handle, MAX30105_INTERRUPT_ALC_OVF_EN, MAX30105_FIFO_DEFAULT_INTERRUPT_ALC_OVF_EN);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set interrupt */
    res = max30105_set_interrupt(&gs_handle, MAX30105_INTERRUPT_PROX_INT_EN, MAX30105_BOOL_TRUE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set interrupt */
    res = max30105_set_interrupt(&gs_handle, MAX30105_INTERRUPT_DIE_TEMP_RDY_EN, MAX30105_FIFO_DEFAULT_INTERRUPT_DIE_TEMP_RDY_EN);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable shutdown */
    res = max30105_set_shutdown(&gs_handle, MAX30105_BOOL_FALSE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set shutdown failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* get status */
    res = max30105_get_interrupt_status(&gs_handle, MAX30105_INTERRUPT_STATUS_FIFO_FULL, &enable);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get interrupt status failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    return 0;
}

/**
 * @brief  fifo example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t max30105_fifo_deinit(void)
{
    uint8_t res;
    
    res = max30105_deinit(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief         read the data
 * @param[out]    *raw_red points to a red raw data buffer
 * @param[out]    *raw_ir points to a ir raw data buffer
 * @param[out]    *raw_green points to a green raw data buffer
 * @param[in,out] *len points to a length buffer
 * @return        status code
 *                - 0 success
 *                - 1 read failed
 * @note          none
 */
uint8_t max30105_fifo_read(uint32_t *raw_red, uint32_t *raw_ir, uint32_t *raw_green, uint8_t *len)
{
    uint8_t res;
    
    res = max30105_read(&gs_handle, raw_red, raw_ir, raw_green, len);
    if (res != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
