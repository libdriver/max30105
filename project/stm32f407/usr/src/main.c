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
 * @file      main.c
 * @brief     main source file
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
#include "driver_max30105_register_test.h"
#include "driver_max30105_fifo_test.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "gpio.h"
#include "uart.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
static uint8_t gs_flag;                    /**< flag */
static uint32_t gs_raw_red[32];            /**< raw red buffer */
static uint32_t gs_raw_ir[32];             /**< raw ir buffer */
static uint32_t gs_raw_green[32];          /**< raw green buffer */
uint8_t g_buf[256];                        /**< uart buffer */
uint16_t g_len;                            /**< uart buffer length */
uint8_t (*g_gpio_irq)(void) = NULL;        /**< irq function address */

/**
 * @brief exti 0 irq
 * @note  none
 */
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/**
 * @brief     gpio exti callback
 * @param[in] pin is the gpio pin
 * @note      none
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_0)
    {
        if (g_gpio_irq)
        {
            g_gpio_irq();
        }
    }
}

/**
 * @brief     interface receive callback
 * @param[in] type is the irq type
 * @note      none
 */
void max30105_receive_callback(uint8_t type)
{
    switch (type)
    {
        case MAX30105_INTERRUPT_STATUS_FIFO_FULL :
        {
            uint8_t res;
            uint8_t len;
            
            /* read data */
            len = 32;
            res = max30105_fifo_read((uint32_t *)gs_raw_red, (uint32_t *)gs_raw_ir, (uint32_t *)gs_raw_green, (uint8_t *)&len);
            if (res != 0)
            {
                max30105_interface_debug_print("max30105: read failed.\n");
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
            max30105_interface_debug_print("max30105: irq prox int.\n");
            
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
            max30105_interface_debug_print("max30105: unknown code.\n");
            
            break;
        }
    }
}

/**
 * @brief     max30105 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */
uint8_t max30105(uint8_t argc, char **argv)
{
    if (argc == 1)
    {
        goto help;
    }
    else if (argc == 2)
    {
        if (strcmp("-i", argv[1]) == 0)
        {
            max30105_info_t info;
            
            /* print max30105 info */
            max30105_info(&info);
            max30105_interface_debug_print("max30105: chip is %s.\n", info.chip_name);
            max30105_interface_debug_print("max30105: manufacturer is %s.\n", info.manufacturer_name);
            max30105_interface_debug_print("max30105: interface is %s.\n", info.interface);
            max30105_interface_debug_print("max30105: driver version is %d.%d.\n", info.driver_version/1000, (info.driver_version%1000)/100);
            max30105_interface_debug_print("max30105: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
            max30105_interface_debug_print("max30105: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
            max30105_interface_debug_print("max30105: max current is %0.2fmA.\n", info.max_current_ma);
            max30105_interface_debug_print("max30105: max temperature is %0.1fC.\n", info.temperature_max);
            max30105_interface_debug_print("max30105: min temperature is %0.1fC.\n", info.temperature_min);
            
            return 0;
        }
        else if (strcmp("-p", argv[1]) == 0)
        {
            /* print pin connection */
            max30105_interface_debug_print("max30105: SCL connected to GPIOB PIN8.\n");
            max30105_interface_debug_print("max30105: SDA connected to GPIOB PIN9.\n");
            max30105_interface_debug_print("max30105: INT connected to GPIOB PIN0.\n");
            
            return 0;
        }
        else if (strcmp("-h", argv[1]) == 0)
        {
            /* show max30105 help */
            
            help:
            
            max30105_interface_debug_print("max30105 -i\n\tshow max30105 chip and driver information.\n");
            max30105_interface_debug_print("max30105 -h\n\tshow max30105 help.\n");
            max30105_interface_debug_print("max30105 -p\n\tshow max30105 pin connections of the current board.\n");
            max30105_interface_debug_print("max30105 -t reg\n\trun max30105 register test.\n");
            max30105_interface_debug_print("max30105 -t fifo <times>\n\trun max30105 fifo test.times means test times.\n");
            max30105_interface_debug_print("max30105 -c fifo <times>\n\trun max30105 fifo function.times means read times.\n");
            
            return 0;
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 3)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
            /* reg test */
            if (strcmp("reg", argv[2]) == 0)
            {
                uint8_t res;
                
                res = max30105_register_test();
                if (res != 0)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 4)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
            /* reg test */
            if (strcmp("fifo", argv[2]) == 0)
            {
                uint8_t res;
                
                /* set gpio */
                g_gpio_irq = max30105_fifo_test_irq_handler;
                res = gpio_interrupt_init();
                if (res != 0)
                {
                    g_gpio_irq = NULL;
                    
                    return 1;
                }
                
                res = max30105_fifo_test(atoi(argv[3]));
                if (res != 0)
                {
                    (void)gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 1;
                }
                else
                {
                    (void)gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        else if (strcmp("-c", argv[1]) == 0)
        {
            /* reg test */
            if (strcmp("fifo", argv[2]) == 0)
            {
                uint8_t res;
                uint32_t timeout;
                uint32_t cnt, times;
                
                /* get times */
                times = atoi(argv[3]);
                cnt = times;
                
                /* set gpio */
                g_gpio_irq = max30105_fifo_irq_handler;
                res = gpio_interrupt_init();
                if (res != 0)
                {
                    g_gpio_irq = NULL;
                    
                    return 1;
                }
                
                /* fifo init */
                res = max30105_fifo_init(max30105_receive_callback);
                if (res != 0)
                {
                    (void)gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 1;
                }
                
                /* read data */
                gs_flag = 0;
                timeout = 5000;
                while (timeout != 0)
                {
                    if (gs_flag != 0)
                    {
                        max30105_interface_debug_print("max30105: %d/%d.\n", cnt - times + 1, cnt);
                        
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
                    (void)max30105_fifo_deinit();
                    (void)gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                   
                    return 1;
                }
                
                (void)max30105_fifo_deinit();
                (void)gpio_interrupt_deinit();
                g_gpio_irq = NULL;
                
                return 0;
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    /* param is invalid */
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart1 init */
    uart1_init(115200);
    
    /* shell init && register max30105 fuction */
    shell_init();
    shell_register("max30105", max30105);
    uart1_print("max30105: welcome to libdriver max30105.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart1_read(g_buf, 256);
        if (g_len)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart1_print("max30105: run failed.\n");
            }
            else if (res == 2)
            {
                uart1_print("max30105: unknow command.\n");
            }
            else if (res == 3)
            {
                uart1_print("max30105: length is too long.\n");
            }
            else if (res == 4)
            {
                uart1_print("max30105: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart1_print("max30105: param is invalid.\n");
            }
            else
            {
                uart1_print("max30105: unknow status code.\n");
            }
            uart1_flush();
        }
        delay_ms(100);
    }
}
