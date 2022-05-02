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
 * @file      driver_max30105_register_test.c
 * @brief     driver max30105 register test source file
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

#include "driver_max30105_register_test.h"
#include <stdlib.h>

static max30105_handle_t gs_handle;        /**< max30105 handle */

/**
 * @brief  register test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t max30105_register_test(void)
{
    uint8_t res;
    uint8_t pointer;
    uint8_t pointer_check;
    uint8_t threshold;
    uint8_t threshold_check;
    uint8_t value;
    uint8_t value_check;
    uint32_t adc;
    uint32_t adc_check;
    uint8_t revision_id;
    uint8_t part_id;
    max30105_info_t info;
    max30105_bool_t enable;
    max30105_adc_resolution_t resolution;
    max30105_sample_averaging_t sample;
    max30105_mode_t mode;
    max30105_particle_sensing_adc_range_t range;
    max30105_particle_sensing_sample_rate_t rate;
    max30105_led_t led;
    
    /* link interface function */
    DRIVER_MAX30105_LINK_INIT(&gs_handle, max30105_handle_t);
    DRIVER_MAX30105_LINK_IIC_INIT(&gs_handle, max30105_interface_iic_init);
    DRIVER_MAX30105_LINK_IIC_DEINIT(&gs_handle, max30105_interface_iic_deinit);
    DRIVER_MAX30105_LINK_IIC_READ(&gs_handle, max30105_interface_iic_read);
    DRIVER_MAX30105_LINK_IIC_WRITE(&gs_handle, max30105_interface_iic_write);
    DRIVER_MAX30105_LINK_DELAY_MS(&gs_handle, max30105_interface_delay_ms);
    DRIVER_MAX30105_LINK_DEBUG_PRINT(&gs_handle, max30105_interface_debug_print);
    DRIVER_MAX30105_LINK_RECEIVE_CALLBACK(&gs_handle, max30105_interface_receive_callback);
    
    /* get information */
    res = max30105_info(&info);
    if (res != 0)
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
    
    /* start register test */
    max30105_interface_debug_print("max30105: start register test.\n");
    
    /* init the max30105 */
    res = max30105_init(&gs_handle);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: init failed.\n");
       
        return 1;
    }
    
    /* max30105_set_interrupt/max30105_get_interrupt test */
    max30105_interface_debug_print("max30105: max30105_set_interrupt/max30105_get_interrupt test.\n");
    
    /* enable fifo full */
    res = max30105_set_interrupt(&gs_handle, MAX30105_INTERRUPT_FIFO_FULL_EN, MAX30105_BOOL_TRUE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: enable fifo full.\n");
    res = max30105_get_interrupt(&gs_handle, MAX30105_INTERRUPT_FIFO_FULL_EN, &enable);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check interrupt %s.\n", enable == MAX30105_BOOL_TRUE ? "ok" : "error");
    
    /* disable fifo full */
    res = max30105_set_interrupt(&gs_handle, MAX30105_INTERRUPT_FIFO_FULL_EN, MAX30105_BOOL_FALSE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: disable fifo full.\n");
    res = max30105_get_interrupt(&gs_handle, MAX30105_INTERRUPT_FIFO_FULL_EN, &enable);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check interrupt %s.\n", enable == MAX30105_BOOL_FALSE ? "ok" : "error");
    
    /* enable data ready */
    res = max30105_set_interrupt(&gs_handle, MAX30105_INTERRUPT_DATA_RDY_EN, MAX30105_BOOL_TRUE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: enable data ready.\n");
    res = max30105_get_interrupt(&gs_handle, MAX30105_INTERRUPT_DATA_RDY_EN, &enable);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check interrupt %s.\n", enable == MAX30105_BOOL_TRUE ? "ok" : "error");
    
    /* disable data ready */
    res = max30105_set_interrupt(&gs_handle, MAX30105_INTERRUPT_DATA_RDY_EN, MAX30105_BOOL_FALSE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: disable data ready.\n");
    res = max30105_get_interrupt(&gs_handle, MAX30105_INTERRUPT_DATA_RDY_EN, &enable);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check interrupt %s.\n", enable == MAX30105_BOOL_FALSE ? "ok" : "error");
    
    /* enable alc ovf */
    res = max30105_set_interrupt(&gs_handle, MAX30105_INTERRUPT_ALC_OVF_EN, MAX30105_BOOL_TRUE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: enable alc ovf.\n");
    res = max30105_get_interrupt(&gs_handle, MAX30105_INTERRUPT_ALC_OVF_EN, &enable);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check interrupt %s.\n", enable == MAX30105_BOOL_TRUE ? "ok" : "error");
    
    /* disable alc ovf */
    res = max30105_set_interrupt(&gs_handle, MAX30105_INTERRUPT_ALC_OVF_EN, MAX30105_BOOL_FALSE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: disable alc ovf.\n");
    res = max30105_get_interrupt(&gs_handle, MAX30105_INTERRUPT_ALC_OVF_EN, &enable);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check interrupt %s.\n", enable == MAX30105_BOOL_FALSE ? "ok" : "error");
    
    /* enable proximity threshold interrupt */
    res = max30105_set_interrupt(&gs_handle, MAX30105_INTERRUPT_PROX_INT_EN, MAX30105_BOOL_TRUE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: enable proximity threshold interrupt.\n");
    res = max30105_get_interrupt(&gs_handle, MAX30105_INTERRUPT_PROX_INT_EN, &enable);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check interrupt %s.\n", enable == MAX30105_BOOL_TRUE ? "ok" : "error");
    
    /* disable proximity threshold interrupt */
    res = max30105_set_interrupt(&gs_handle, MAX30105_INTERRUPT_PROX_INT_EN, MAX30105_BOOL_FALSE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: disable proximity threshold interrupt.\n");
    res = max30105_get_interrupt(&gs_handle, MAX30105_INTERRUPT_PROX_INT_EN, &enable);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check interrupt %s.\n", enable == MAX30105_BOOL_FALSE ? "ok" : "error");
    
    /* enable die temp ready */
    res = max30105_set_interrupt(&gs_handle, MAX30105_INTERRUPT_DIE_TEMP_RDY_EN, MAX30105_BOOL_TRUE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: enable die temp ready.\n");
    res = max30105_get_interrupt(&gs_handle, MAX30105_INTERRUPT_DIE_TEMP_RDY_EN, &enable);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check interrupt %s.\n", enable == MAX30105_BOOL_TRUE ? "ok" : "error");
    
    /* disable die temp ready */
    res = max30105_set_interrupt(&gs_handle, MAX30105_INTERRUPT_DIE_TEMP_RDY_EN, MAX30105_BOOL_FALSE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: disable die temp ready.\n");
    res = max30105_get_interrupt(&gs_handle, MAX30105_INTERRUPT_DIE_TEMP_RDY_EN, &enable);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get interrupt failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check interrupt %s.\n", enable == MAX30105_BOOL_FALSE ? "ok" : "error");
    
    /* max30105_set_fifo_write_pointer/max30105_get_fifo_write_pointer test */
    max30105_interface_debug_print("max30105: max30105_set_fifo_write_pointer/max30105_get_fifo_write_pointer test.\n");
    
    pointer = rand() % 0x1F;
    res = max30105_set_fifo_write_pointer(&gs_handle, pointer);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set fifo write pointer failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set fifo write pointer %d.\n", pointer);
    res = max30105_get_fifo_write_pointer(&gs_handle, (uint8_t *)&pointer_check);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get fifo write pointer failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* max30105_set_fifo_overflow_counter/max30105_get_fifo_overflow_counter test */
    max30105_interface_debug_print("max30105: max30105_set_fifo_overflow_counter/max30105_get_fifo_overflow_counter test.\n");
    
    pointer = rand() % 0x1F;
    res = max30105_set_fifo_overflow_counter(&gs_handle, pointer);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set fifo overflow counter failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set fifo overflow counter %d.\n", pointer);
    res = max30105_get_fifo_overflow_counter(&gs_handle, (uint8_t *)&pointer_check);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get fifo overflow counter failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* max30105_set_fifo_read_pointer/max30105_get_fifo_read_pointer test */
    max30105_interface_debug_print("max30105: max30105_set_fifo_read_pointer/max30105_get_fifo_read_pointer test.\n");
    
    pointer = rand() % 0x1F;
    res = max30105_set_fifo_read_pointer(&gs_handle, pointer);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set fifo read pointer failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set fifo read pointer %d.\n", pointer);
    res = max30105_get_fifo_read_pointer(&gs_handle, (uint8_t *)&pointer_check);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get fifo read pointer failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* max30105_set_fifo_data/max30105_get_fifo_data test */
    max30105_interface_debug_print("max30105: max30105_set_fifo_data/max30105_get_fifo_data test.\n");
    
    pointer = rand() % 0x1F;
    res = max30105_set_fifo_data(&gs_handle, pointer);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set fifo data failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set fifo data %d.\n", pointer);
    res = max30105_get_fifo_data(&gs_handle, (uint8_t *)&pointer_check);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get fifo data failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* max30105_set_fifo_sample_averaging/max30105_get_fifo_sample_averaging test */
    max30105_interface_debug_print("max30105: max30105_set_fifo_sample_averaging/max30105_get_fifo_sample_averaging test.\n");
    
    /* set sample averaging 1 */
    res = max30105_set_fifo_sample_averaging(&gs_handle, MAX30105_SAMPLE_AVERAGING_1);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set fifo sample averaging failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set sample averaging 1.\n");
    res = max30105_get_fifo_sample_averaging(&gs_handle, &sample);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get fifo sample averaging failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check sample %s.\n", sample == MAX30105_SAMPLE_AVERAGING_1 ? "ok" : "error");
    
    /* set sample averaging 2 */
    res = max30105_set_fifo_sample_averaging(&gs_handle, MAX30105_SAMPLE_AVERAGING_2);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set fifo sample averaging failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set sample averaging 2.\n");
    res = max30105_get_fifo_sample_averaging(&gs_handle, &sample);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get fifo sample averaging failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check sample %s.\n", sample == MAX30105_SAMPLE_AVERAGING_2 ? "ok" : "error");
    
    /* set sample averaging 4 */
    res = max30105_set_fifo_sample_averaging(&gs_handle, MAX30105_SAMPLE_AVERAGING_4);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set fifo sample averaging failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set sample averaging 4.\n");
    res = max30105_get_fifo_sample_averaging(&gs_handle, &sample);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get fifo sample averaging failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check sample %s.\n", sample == MAX30105_SAMPLE_AVERAGING_4 ? "ok" : "error");
    
    /* set sample averaging 8 */
    res = max30105_set_fifo_sample_averaging(&gs_handle, MAX30105_SAMPLE_AVERAGING_8);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set fifo sample averaging failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set sample averaging 8.\n");
    res = max30105_get_fifo_sample_averaging(&gs_handle, &sample);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get fifo sample averaging failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check sample %s.\n", sample == MAX30105_SAMPLE_AVERAGING_8 ? "ok" : "error");
    
    /* set sample averaging 16 */
    res = max30105_set_fifo_sample_averaging(&gs_handle, MAX30105_SAMPLE_AVERAGING_16);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set fifo sample averaging failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set sample averaging 16.\n");
    res = max30105_get_fifo_sample_averaging(&gs_handle, &sample);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get fifo sample averaging failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check sample %s.\n", sample == MAX30105_SAMPLE_AVERAGING_16 ? "ok" : "error");
    
    /* set sample averaging 32 */
    res = max30105_set_fifo_sample_averaging(&gs_handle, MAX30105_SAMPLE_AVERAGING_32);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set fifo sample averaging failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set sample averaging 32.\n");
    res = max30105_get_fifo_sample_averaging(&gs_handle, &sample);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get fifo sample averaging failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check sample %s.\n", sample == MAX30105_SAMPLE_AVERAGING_32 ? "ok" : "error");
    
    /* max30105_set_fifo_roll/max30105_get_fifo_roll test */
    max30105_interface_debug_print("max30105: max30105_set_fifo_roll/max30105_get_fifo_roll test.\n");
    
    /* enable fifo roll */
    res = max30105_set_fifo_roll(&gs_handle, MAX30105_BOOL_TRUE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set fifo roll failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: enable fifo roll.\n");
    res = max30105_get_fifo_roll(&gs_handle, &enable);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get fifo roll failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check roll %s.\n", enable == MAX30105_BOOL_TRUE ? "ok" : "error");
    
    /* disable fifo roll */
    res = max30105_set_fifo_roll(&gs_handle, MAX30105_BOOL_FALSE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set fifo roll failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: disable fifo roll.\n");
    res = max30105_get_fifo_roll(&gs_handle, &enable);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get fifo roll failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check roll %s.\n", enable == MAX30105_BOOL_FALSE ? "ok" : "error");
    
    /* max30105_set_fifo_almost_full/max30105_get_fifo_almost_full test */
    max30105_interface_debug_print("max30105: max30105_set_fifo_almost_full/max30105_get_fifo_almost_full test.\n");
    
    value = rand() % 0xF;
    res = max30105_set_fifo_almost_full(&gs_handle, value);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set fifo almost full failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set fifo almost full %d.\n", value);
    res = max30105_get_fifo_almost_full(&gs_handle, (uint8_t *)&value_check);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get fifo almost full failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check fifo almost full %s.\n", value_check == value ? "ok" : "error");
    
    /* max30105_set_shutdown/max30105_get_shutdown test */
    max30105_interface_debug_print("max30105: max30105_set_shutdown/max30105_get_shutdown test.\n");
    
    /* enable shutdown */
    res = max30105_set_shutdown(&gs_handle, MAX30105_BOOL_TRUE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set shutdown failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: enable shutdown.\n");
    res = max30105_get_shutdown(&gs_handle, &enable);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get shutdown failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check shutdown %s.\n", enable == MAX30105_BOOL_TRUE ? "ok" : "error");
    
    /* disable shutdown */
    res = max30105_set_shutdown(&gs_handle, MAX30105_BOOL_FALSE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set shutdown failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: disable shutdown.\n");
    res = max30105_get_shutdown(&gs_handle, &enable);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get shutdown failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check shutdown %s.\n", enable == MAX30105_BOOL_FALSE ? "ok" : "error");
    
    /* max30105_set_mode/max30105_get_mode test */
    max30105_interface_debug_print("max30105: max30105_set_mode/max30105_get_mode test.\n");
    
    /* red mode */
    res = max30105_set_mode(&gs_handle, MAX30105_MODE_RED);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set mode failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set red mode.\n");
    res = max30105_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get mode failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check mode %s.\n", mode == MAX30105_MODE_RED ? "ok" : "error");
    
    /* red ir mode */
    res = max30105_set_mode(&gs_handle, MAX30105_MODE_RED_IR);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set mode failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set red ir mode.\n");
    res = max30105_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get mode failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check mode %s.\n", mode == MAX30105_MODE_RED_IR ? "ok" : "error");
    
    /* red ir green */
    res = max30105_set_mode(&gs_handle, MAX30105_MODE_GREEN_RED_IR);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set mode failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set red ir green mode.\n");
    res = max30105_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get mode failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check mode %s.\n", mode == MAX30105_MODE_GREEN_RED_IR ? "ok" : "error");
    
    /* max30105_set_particle_sensing_adc_range/max30105_get_particle_sensing_adc_range test */
    max30105_interface_debug_print("max30105: max30105_set_particle_sensing_adc_range/max30105_get_particle_sensing_adc_range test.\n");
    
    /* 2048 */
    res = max30105_set_particle_sensing_adc_range(&gs_handle, MAX30105_PARTICLE_SENSING_ADC_RANGE_2048);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set particle sensing adc range failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set particle sensing adc range 2048.\n");
    res = max30105_get_particle_sensing_adc_range(&gs_handle, &range);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get particle sensing adc range failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check particle sensing adc range %s.\n", range == MAX30105_PARTICLE_SENSING_ADC_RANGE_2048 ? "ok" : "error");
    
    /* 4096 */
    res = max30105_set_particle_sensing_adc_range(&gs_handle, MAX30105_PARTICLE_SENSING_ADC_RANGE_4096);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set particle sensing adc range failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set particle sensing adc range 4096.\n");
    res = max30105_get_particle_sensing_adc_range(&gs_handle, &range);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get particle sensing adc range failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check particle sensing adc range %s.\n", range == MAX30105_PARTICLE_SENSING_ADC_RANGE_4096 ? "ok" : "error");
    
    /* 8192 */
    res = max30105_set_particle_sensing_adc_range(&gs_handle, MAX30105_PARTICLE_SENSING_ADC_RANGE_8192);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set particle sensing adc range failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set particle sensing adc range 8192.\n");
    res = max30105_get_particle_sensing_adc_range(&gs_handle, &range);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get particle sensing adc range failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check particle sensing adc range %s.\n", range == MAX30105_PARTICLE_SENSING_ADC_RANGE_8192 ? "ok" : "error");
    
    /* 16384 */
    res = max30105_set_particle_sensing_adc_range(&gs_handle, MAX30105_PARTICLE_SENSING_ADC_RANGE_16384);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set particle sensing adc range failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set particle sensing adc range 16384.\n");
    res = max30105_get_particle_sensing_adc_range(&gs_handle, &range);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get particle sensing adc range failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check particle sensing adc range %s.\n", range == MAX30105_PARTICLE_SENSING_ADC_RANGE_16384 ? "ok" : "error");
    
    /* max30105_set_particle_sensing_sample_rate/max30105_get_particle_sensing_sample_rate test */
    max30105_interface_debug_print("max30105: max30105_set_particle_sensing_sample_rate/max30105_get_particle_sensing_sample_rate test.\n");
    
    /* 50Hz */
    res = max30105_set_particle_sensing_sample_rate(&gs_handle, MAX30105_PARTICLE_SENSING_SAMPLE_RATE_50_HZ);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set particle sensing sample rate failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set particle sensing sample rate 50Hz.\n");
    res = max30105_get_particle_sensing_sample_rate(&gs_handle, &rate);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get particle sensing sample rate failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check particle sensing sample rate %s.\n", rate == MAX30105_PARTICLE_SENSING_SAMPLE_RATE_50_HZ ? "ok" : "error");
    
    /* 100Hz */
    res = max30105_set_particle_sensing_sample_rate(&gs_handle, MAX30105_PARTICLE_SENSING_SAMPLE_RATE_100_HZ);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set particle sensing sample rate failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set particle sensing sample rate 100Hz.\n");
    res = max30105_get_particle_sensing_sample_rate(&gs_handle, &rate);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get particle sensing sample rate failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check particle sensing sample rate %s.\n", rate == MAX30105_PARTICLE_SENSING_SAMPLE_RATE_100_HZ ? "ok" : "error");
    
    /* 200Hz */
    res = max30105_set_particle_sensing_sample_rate(&gs_handle, MAX30105_PARTICLE_SENSING_SAMPLE_RATE_200_HZ);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set particle sensing sample rate failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set particle sensing sample rate 200Hz.\n");
    res = max30105_get_particle_sensing_sample_rate(&gs_handle, &rate);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get particle sensing sample rate failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check particle sensing sample rate %s.\n", rate == MAX30105_PARTICLE_SENSING_SAMPLE_RATE_200_HZ ? "ok" : "error");
    
    /* 400Hz */
    res = max30105_set_particle_sensing_sample_rate(&gs_handle, MAX30105_PARTICLE_SENSING_SAMPLE_RATE_400_HZ);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set particle sensing sample rate failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set particle sensing sample rate 400Hz.\n");
    res = max30105_get_particle_sensing_sample_rate(&gs_handle, &rate);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get particle sensing sample rate failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check particle sensing sample rate %s.\n", rate == MAX30105_PARTICLE_SENSING_SAMPLE_RATE_400_HZ ? "ok" : "error");
    
    /* 800Hz */
    res = max30105_set_particle_sensing_sample_rate(&gs_handle, MAX30105_PARTICLE_SENSING_SAMPLE_RATE_800_HZ);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set particle sensing sample rate failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set particle sensing sample rate 800Hz.\n");
    res = max30105_get_particle_sensing_sample_rate(&gs_handle, &rate);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get particle sensing sample rate failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check particle sensing sample rate %s.\n", rate == MAX30105_PARTICLE_SENSING_SAMPLE_RATE_800_HZ ? "ok" : "error");
    
    /* 1000Hz */
    res = max30105_set_particle_sensing_sample_rate(&gs_handle, MAX30105_PARTICLE_SENSING_SAMPLE_RATE_1000_HZ);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set particle sensing sample rate failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set particle sensing sample rate 1000Hz.\n");
    res = max30105_get_particle_sensing_sample_rate(&gs_handle, &rate);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get particle sensing sample rate failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check particle sensing sample rate %s.\n", rate == MAX30105_PARTICLE_SENSING_SAMPLE_RATE_1000_HZ ? "ok" : "error");
    
    /* 1600Hz */
    res = max30105_set_particle_sensing_sample_rate(&gs_handle, MAX30105_PARTICLE_SENSING_SAMPLE_RATE_1600_HZ);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set particle sensing sample rate failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set particle sensing sample rate 1600Hz.\n");
    res = max30105_get_particle_sensing_sample_rate(&gs_handle, &rate);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get particle sensing sample rate failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check particle sensing sample rate %s.\n", rate == MAX30105_PARTICLE_SENSING_SAMPLE_RATE_1600_HZ ? "ok" : "error");
    
    /* 3200Hz */
    res = max30105_set_particle_sensing_sample_rate(&gs_handle, MAX30105_PARTICLE_SENSING_SAMPLE_RATE_3200_HZ);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set particle sensing sample rate failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set particle sensing sample rate 3200Hz.\n");
    res = max30105_get_particle_sensing_sample_rate(&gs_handle, &rate);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get particle sensing sample rate failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check particle sensing sample rate %s.\n", rate == MAX30105_PARTICLE_SENSING_SAMPLE_RATE_3200_HZ ? "ok" : "error");
    
    /* max30105_set_adc_resolution/max30105_get_adc_resolution test */
    max30105_interface_debug_print("max30105: max30105_set_adc_resolution/max30105_get_adc_resolution test.\n");
    
    /* 15 bits */
    res = max30105_set_adc_resolution(&gs_handle, MAX30105_ADC_RESOLUTION_15_BIT);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set adc resolution failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set adc resolution 15 bits.\n");
    res = max30105_get_adc_resolution(&gs_handle, &resolution);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get adc resolution failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check adc resolution %s.\n", resolution == MAX30105_ADC_RESOLUTION_15_BIT ? "ok" : "error");
    
    /* 16 bits */
    res = max30105_set_adc_resolution(&gs_handle, MAX30105_ADC_RESOLUTION_16_BIT);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set adc resolution failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set adc resolution 16 bits.\n");
    res = max30105_get_adc_resolution(&gs_handle, &resolution);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get adc resolution failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check adc resolution %s.\n", resolution == MAX30105_ADC_RESOLUTION_16_BIT ? "ok" : "error");
    
    /* 17 bits */
    res = max30105_set_adc_resolution(&gs_handle, MAX30105_ADC_RESOLUTION_17_BIT);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set adc resolution failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set adc resolution 17 bits.\n");
    res = max30105_get_adc_resolution(&gs_handle, &resolution);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get adc resolution failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check adc resolution %s.\n", resolution == MAX30105_ADC_RESOLUTION_17_BIT ? "ok" : "error");
    
    /* 18 bits */
    res = max30105_set_adc_resolution(&gs_handle, MAX30105_ADC_RESOLUTION_18_BIT);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set adc resolution failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set adc resolution 18 bits.\n");
    res = max30105_get_adc_resolution(&gs_handle, &resolution);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get adc resolution failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check adc resolution %s.\n", resolution == MAX30105_ADC_RESOLUTION_18_BIT ? "ok" : "error");
    
    /* max30105_set_led_red_pulse_amplitude/max30105_get_led_red_pulse_amplitude test */
    max30105_interface_debug_print("max30105: max30105_set_led_red_pulse_amplitude/max30105_get_led_red_pulse_amplitude test.\n");
    
    value = rand() % 256;
    res = max30105_set_led_red_pulse_amplitude(&gs_handle, value);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set led red pulse amplitude failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set led red pulse amplitude %d.\n", value);
    res = max30105_get_led_red_pulse_amplitude(&gs_handle, (uint8_t *)&value_check);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get led red pulse amplitude failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check led red pulse amplitude %s.\n", value_check == value ? "ok" : "error");
    
    /* max30105_set_led_ir_pulse_amplitude/max30105_get_led_ir_pulse_amplitude test */
    max30105_interface_debug_print("max30105: max30105_set_led_ir_pulse_amplitude/max30105_get_led_ir_pulse_amplitude test.\n");
    
    value = rand() % 256;
    res = max30105_set_led_ir_pulse_amplitude(&gs_handle, value);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set led ir pulse amplitude failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set led ir pulse amplitude %d.\n", value);
    res = max30105_get_led_ir_pulse_amplitude(&gs_handle, (uint8_t *)&value_check);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get led ir pulse amplitude failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check led ir pulse amplitude %s.\n", value_check == value ? "ok" : "error");
    
    /* max30105_set_led_green_pulse_amplitude/max30105_get_led_green_pulse_amplitude test */
    max30105_interface_debug_print("max30105: max30105_set_led_green_pulse_amplitude/max30105_get_led_green_pulse_amplitude test.\n");
    
    value = rand() % 256;
    res = max30105_set_led_green_pulse_amplitude(&gs_handle, value);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set led green pulse amplitude failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set led green pulse amplitude %d.\n", value);
    res = max30105_get_led_green_pulse_amplitude(&gs_handle, (uint8_t *)&value_check);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get led green pulse amplitude failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check led green pulse amplitude %s.\n", value_check == value ? "ok" : "error");
    
    /* max30105_set_led_proximity_pulse_amplitude/max30105_get_led_proximity_pulse_amplitude test */
    max30105_interface_debug_print("max30105: max30105_set_led_proximity_pulse_amplitude/max30105_get_led_proximity_pulse_amplitude test.\n");
    
    value = rand() % 256;
    res = max30105_set_led_proximity_pulse_amplitude(&gs_handle, value);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set led proximity pulse amplitude failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set led proximity pulse amplitude %d.\n", value);
    res = max30105_get_led_proximity_pulse_amplitude(&gs_handle, (uint8_t *)&value_check);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get led proximity pulse amplitude failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check led proximity pulse amplitude %s.\n", value_check == value ? "ok" : "error");
    
    /* max30105_set_slot/max30105_get_slot test */
    max30105_interface_debug_print("max30105: max30105_set_slot/max30105_get_slot test.\n");
    
    /* slot1 led none */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_1, MAX30105_LED_NONE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot1 led none.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_1, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot1 %s.\n", led == MAX30105_LED_NONE ? "ok" : "error");
    
    /* slot1 red led1 pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_1, MAX30105_LED_RED_LED1_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot1 red led1 pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_1, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot1 %s.\n", led == MAX30105_LED_RED_LED1_PA ? "ok" : "error");
    
    /* slot1 ir led2 pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_1, MAX30105_LED_IR_LED2_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot1 ir led2 pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_1, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot1 %s.\n", led == MAX30105_LED_IR_LED2_PA ? "ok" : "error");
    
    /* slot1 green led3 pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_1, MAX30105_LED_GREEN_LED3_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot1 green led3 pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_1, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot1 %s.\n", led == MAX30105_LED_GREEN_LED3_PA ? "ok" : "error");
    
    /* slot1 red pilot pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_1, MAX30105_LED_RED_PILOT_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot1 red pilot pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_1, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot1 %s.\n", led == MAX30105_LED_RED_PILOT_PA ? "ok" : "error");
    
    /* slot1 ir pilot pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_1, MAX30105_LED_IR_PILOT_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot1 ir pilot pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_1, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot1 %s.\n", led == MAX30105_LED_IR_PILOT_PA ? "ok" : "error");
    
    /* slot1 green pilot pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_1, MAX30105_LED_GREEN_PILOT_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot1 green pilot pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_1, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot1 %s.\n", led == MAX30105_LED_GREEN_PILOT_PA ? "ok" : "error");
    
    /* slot2 led none */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_2, MAX30105_LED_NONE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot2 led none.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_2, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot2 %s.\n", led == MAX30105_LED_NONE ? "ok" : "error");
    
    /* slot2 red led1 pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_2, MAX30105_LED_RED_LED1_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot2 red led1 pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_2, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot2 %s.\n", led == MAX30105_LED_RED_LED1_PA ? "ok" : "error");
    
    /* slot2 ir led2 pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_2, MAX30105_LED_IR_LED2_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot2 ir led2 pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_2, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot2 %s.\n", led == MAX30105_LED_IR_LED2_PA ? "ok" : "error");
    
    /* slot2 green led3 pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_2, MAX30105_LED_GREEN_LED3_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot2 green led3 pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_2, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot2 %s.\n", led == MAX30105_LED_GREEN_LED3_PA ? "ok" : "error");
    
    /* slot2 red pilot pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_2, MAX30105_LED_RED_PILOT_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot2 red pilot pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_2, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot2 %s.\n", led == MAX30105_LED_RED_PILOT_PA ? "ok" : "error");
    
    /* slot2 ir pilot pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_2, MAX30105_LED_IR_PILOT_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot2 ir pilot pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_2, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot2 %s.\n", led == MAX30105_LED_IR_PILOT_PA ? "ok" : "error");
    
    /* slot2 green pilot pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_2, MAX30105_LED_GREEN_PILOT_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot2 green pilot pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_2, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot2 %s.\n", led == MAX30105_LED_GREEN_PILOT_PA ? "ok" : "error");
    
    /* slot3 led none */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_3, MAX30105_LED_NONE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot3 led none.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_3, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot3 %s.\n", led == MAX30105_LED_NONE ? "ok" : "error");
    
    /* slot3 red led1 pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_3, MAX30105_LED_RED_LED1_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot3 red led1 pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_3, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot3 %s.\n", led == MAX30105_LED_RED_LED1_PA ? "ok" : "error");
    
    /* slot3 ir led2 pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_3, MAX30105_LED_IR_LED2_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot3 ir led2 pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_3, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot3 %s.\n", led == MAX30105_LED_IR_LED2_PA ? "ok" : "error");
    
    /* slot3 green led3 pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_3, MAX30105_LED_GREEN_LED3_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot3 green led3 pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_3, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot3 %s.\n", led == MAX30105_LED_GREEN_LED3_PA ? "ok" : "error");
    
    /* slot3 red pilot pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_3, MAX30105_LED_RED_PILOT_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot3 red pilot pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_3, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot3 %s.\n", led == MAX30105_LED_RED_PILOT_PA ? "ok" : "error");
    
    /* slot3 ir pilot pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_3, MAX30105_LED_IR_PILOT_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot3 ir pilot pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_3, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot3 %s.\n", led == MAX30105_LED_IR_PILOT_PA ? "ok" : "error");
    
    /* slot3 green pilot pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_3, MAX30105_LED_GREEN_PILOT_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot3 green pilot pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_3, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot3 %s.\n", led == MAX30105_LED_GREEN_PILOT_PA ? "ok" : "error");
    
    /* slot4 led none */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_4, MAX30105_LED_NONE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot4 led none.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_4, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot4 %s.\n", led == MAX30105_LED_NONE ? "ok" : "error");
    
    /* slot4 red led1 pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_4, MAX30105_LED_RED_LED1_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot4 red led1 pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_4, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot4 %s.\n", led == MAX30105_LED_RED_LED1_PA ? "ok" : "error");
    
    /* slot4 ir led2 pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_4, MAX30105_LED_IR_LED2_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot4 ir led2 pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_4, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot4 %s.\n", led == MAX30105_LED_IR_LED2_PA ? "ok" : "error");
    
    /* slot4 green led3 pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_4, MAX30105_LED_GREEN_LED3_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot4 green led3 pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_4, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot4 %s.\n", led == MAX30105_LED_GREEN_LED3_PA ? "ok" : "error");
    
    /* slot4 red pilot pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_4, MAX30105_LED_RED_PILOT_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot4 red pilot pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_4, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot4 %s.\n", led == MAX30105_LED_RED_PILOT_PA ? "ok" : "error");
    
    /* slot4 ir pilot pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_4, MAX30105_LED_IR_PILOT_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot4 ir pilot pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_4, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot4 %s.\n", led == MAX30105_LED_IR_PILOT_PA ? "ok" : "error");
    
    /* slot4 green pilot pa */
    res = max30105_set_slot(&gs_handle, MAX30105_SLOT_4, MAX30105_LED_GREEN_PILOT_PA);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set slot4 green pilot pa.\n");
    res = max30105_get_slot(&gs_handle, MAX30105_SLOT_4, &led);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get slot failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check slot4 %s.\n", led == MAX30105_LED_GREEN_PILOT_PA ? "ok" : "error");
    
    /* max30105_set_die_temperature/max30105_get_die_temperature test */
    max30105_interface_debug_print("max30105: max30105_set_die_temperature/max30105_get_die_temperature test.\n");
    
    /* disable */
    res = max30105_set_die_temperature(&gs_handle, MAX30105_BOOL_FALSE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set die temperature failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: disable die temperature.\n");
    res = max30105_get_die_temperature(&gs_handle, &enable);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get die temperature failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check die temperature %s.\n", enable == MAX30105_BOOL_FALSE ? "ok" : "error");
    
    /* enable */
    res = max30105_set_die_temperature(&gs_handle, MAX30105_BOOL_TRUE);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set die temperature failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: enable die temperature.\n");
    res = max30105_get_die_temperature(&gs_handle, &enable);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get die temperature failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check die temperature %s.\n", res == 0 ? "ok" : "error");
    
    /* max30105_set_proximity_interrupt_threshold/max30105_get_proximity_interrupt_threshold test */
    max30105_interface_debug_print("max30105: max30105_set_proximity_interrupt_threshold/max30105_get_proximity_interrupt_threshold test.\n");
    
    threshold = rand() % 256;
    res = max30105_set_proximity_interrupt_threshold(&gs_handle, threshold);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: set proximity interrupt threshold failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: set proximity interrupt threshold %d.\n", threshold);
    res = max30105_get_proximity_interrupt_threshold(&gs_handle, (uint8_t *)&threshold_check);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get proximity interrupt threshold failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    
    /* max30105_proximity_threshold_convert_to_register/max30105_proximity_threshold_convert_to_data test */
    max30105_interface_debug_print("max30105: max30105_proximity_threshold_convert_to_register/max30105_proximity_threshold_convert_to_data test.\n");
    
    adc = rand() % 256 * 1023;
    res = max30105_proximity_threshold_convert_to_register(&gs_handle, adc, (uint8_t *)&value);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: proximity threshold convert to register failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: adc is %d.\n", adc);
    res = max30105_proximity_threshold_convert_to_data(&gs_handle, value, (uint32_t *)&adc_check);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: proximity threshold convert to data failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check adc is %d.\n", adc_check);
    
    /* max30105_get_id test */
    max30105_interface_debug_print("max30105: max30105_get_id test.\n");

    /* get id */
    res = max30105_get_id(&gs_handle, (uint8_t *)&revision_id, (uint8_t *)&part_id);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get id failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: revision id is 0x%02X part id is 0x%02X.\n", revision_id, part_id);
    
    /* max30105_get_interrupt_status test */
    max30105_interface_debug_print("max30105: max30105_get_interrupt_status test.\n");
    
    /* get fifo full */
    res = max30105_get_interrupt_status(&gs_handle, MAX30105_INTERRUPT_STATUS_FIFO_FULL, &enable);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get interrupt status failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: interrupt status fifo full is %d.\n", enable);
    
    /* get data ready */
    res = max30105_get_interrupt_status(&gs_handle, MAX30105_INTERRUPT_STATUS_DATA_RDY, &enable);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get interrupt status failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: interrupt status data ready is %d.\n", enable);
    
    /* get alc ovf */
    res = max30105_get_interrupt_status(&gs_handle, MAX30105_INTERRUPT_STATUS_ALC_OVF, &enable);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get interrupt status failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: interrupt status alc ovf is %d.\n", enable);
    
    /* get proximity threshold */
    res = max30105_get_interrupt_status(&gs_handle, MAX30105_INTERRUPT_STATUS_PROX_INT, &enable);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get interrupt status failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: interrupt status proximity threshold is %d.\n", enable);
    
    /* get pwr ready */
    res = max30105_get_interrupt_status(&gs_handle, MAX30105_INTERRUPT_STATUS_PWR_RDY, &enable);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get interrupt status failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: interrupt status pwr ready is %d.\n", enable);
    
    /* get die temp ready */
    res = max30105_get_interrupt_status(&gs_handle, MAX30105_INTERRUPT_STATUS_DIE_TEMP_RDY, &enable);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: get interrupt status failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: interrupt status die temp ready is %d.\n", enable);
    
    /* max30105_reset test */
    max30105_interface_debug_print("max30105: max30105_reset test.\n");
    
    /* reset */
    res = max30105_reset(&gs_handle);
    if (res != 0)
    {
        max30105_interface_debug_print("max30105: reset failed.\n");
        (void)max30105_deinit(&gs_handle);
       
        return 1;
    }
    max30105_interface_debug_print("max30105: check reset %s.\n", res == 0 ? "ok" : "error");
    max30105_interface_delay_ms(100);
    
    /* finish register test */
    max30105_interface_debug_print("max30105: finish register test.\n");
    (void)max30105_deinit(&gs_handle);
    
    return 0;
}
