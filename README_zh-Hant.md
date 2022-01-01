<div align=center>
<img src="/doc/image/logo.png"/>
</div>

## LibDriver MAX30105

[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md)

MAX30105為集成粒子檢測模塊。包括內部LED、光電檢測器、光元件，以及帶環境光抑制的低噪聲電子電路。 MAX30105提供完備的系統方案，簡化消防報警燈煙霧檢測應用的設計過程。得益於極小的尺寸，MAX30105也可用作移動和可穿戴設備的煙霧檢測傳感器。 MAX30105採用1.8V單電源工作，獨立的5.0V電源用於內部LED，器件通過標準I2C兼容接口通信。可通過軟件關斷模塊，待機電流為零，允許電源軌在全部時間保持供電。MAX30105可用於菸霧報警器，樓宇自動化煙霧檢測器，移動設備煙霧檢測器和移動設備煙霧檢測器等。

LibDriver MAX30105是LibDriver推出的MAX30105的全功能驅動，該驅動提供FIFO讀取，ID讀取等功能。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example fifo](#example-fifo)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver MAX30105的源文件。

/interface目錄包含了LibDriver MAX30105與平台無關的IIC總線模板。

/test目錄包含了LibDriver MAX30105驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver MAX30105編程範例。

/doc目錄包含了LibDriver MAX30105離線文檔。

/datasheet目錄包含了MAX30105數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

### 安裝

參考/interface目錄下與平台無關的IIC總線模板，完成指定平台的IIC總線驅動。

將/src目錄，/interface目錄和/example目錄加入工程。

### 使用

#### example fifo

```C
volatile uint8_t res;
volatile uint32_t timeout;
volatile uint32_t cnt, times;

uint8_t max30105_receive_callback(uint8_t type)
{
    switch (type)
    {
        case MAX30105_INTERRUPT_STATUS_FIFO_FULL :
        {
            volatile uint8_t res;
            volatile uint8_t len;
            
            /* read data */
            len = 32;
            res = max30105_fifo_read((uint32_t *)gs_raw_red, (uint32_t *)gs_raw_ir, (uint32_t *)gs_raw_green, (uint8_t *)&len);
            if (res)
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
            break;
        }
    }
    
    return 0;
}

...
    
/* get times */
times = 3;
cnt = times;
                
/* set gpio */
g_gpio_irq = max30105_interface_fifo_irq_handler;
res = gpio_interrupt_init();
if (res)
{
    g_gpio_irq = NULL;

    return 1;
}

/* fifo init */
res = max30105_fifo_init(max30105_receive_callback);
if (res)
{
    gpio_interrupt_deinit();
    g_gpio_irq = NULL;

    return 1;
}

...
    
/* read data */
gs_flag = 0;
timeout = 5000;
while (timeout)
{
    if (gs_flag)
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
    max30105_fifo_deinit();
    gpio_interrupt_deinit();
    g_gpio_irq = NULL;

    return 1;
}

...
    
max30105_fifo_deinit();
gpio_interrupt_deinit();
g_gpio_irq = NULL;

return 0;
```

### 文檔

在線文檔: https://www.libdriver.com/docs/max30105/index.html

離線文檔: /doc/html/index.html

### 貢獻

請聯繫lishifenging@outlook.com

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com