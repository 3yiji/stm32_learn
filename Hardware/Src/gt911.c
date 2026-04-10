#include "gt911.h"
#include <stdint.h>
#include "stdio.h"
#include "string.h"
#include "stm32f1xx_hal_gpio.h"

// GT911驱动配置，默认使用第一份配置文件，如果屏幕异常，可尝试使用第二份
#if  1
const uint8_t CTP_CFG_GT911[] =  {
0x41,0x20,0x03,0xE0,0x01,0x05,0x3D,0x00,0x01,0x08,
0x1E,0x05,0x3C,0x3C,0x03,0x05,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x1A,0x1C,0x1E,0x14,0x8A,0x2A,0x0C,
0x2A,0x28,0xEB,0x04,0x00,0x00,0x01,0x61,0x03,0x2C,
0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x14,0x3C,0x94,0xC5,0x02,0x08,0x00,0x00,0x04,
0xB7,0x16,0x00,0x9F,0x1B,0x00,0x8B,0x22,0x00,0x7B,
0x2B,0x00,0x70,0x36,0x00,0x70,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x18,0x16,0x14,0x12,0x10,0x0E,0x0C,0x0A,
0x08,0x06,0x04,0x02,0xFF,0xFF,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x24,0x22,0x21,0x20,0x1F,0x1E,0x1D,0x1C,
0x18,0x16,0x13,0x12,0x10,0x0F,0x0A,0x08,0x06,0x04,
0x02,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x0A,0x00
};
#else
const uint8_t CTP_CFG_GT911[] =  {
  0x00,0x20,0x03,0xE0,0x01,0x05,0x0D,0x00,0x01,0x08,
  0x28,0x0F,0x50,0x32,0x03,0x05,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x8A,0x2A,0x0C,
  0x45,0x47,0x0C,0x08,0x00,0x00,0x00,0x02,0x02,0x2D,
  0x00,0x00,0x00,0x00,0x00,0x03,0x64,0x32,0x00,0x00,
  0x00,0x28,0x64,0x94,0xC5,0x02,0x07,0x00,0x00,0x04,
  0x9C,0x2C,0x00,0x8F,0x34,0x00,0x84,0x3F,0x00,0x7C,
  0x4C,0x00,0x77,0x5B,0x00,0x77,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x18,0x16,0x14,0x12,0x10,0x0E,0x0C,0x0A,
  0x08,0x06,0x04,0x02,0xFF,0xFF,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x16,0x18,0x1C,0x1D,0x1E,0x1F,0x20,0x21,
  0x22,0x24,0x13,0x12,0x10,0x0F,0x0A,0x08,0x06,0x04,
  0x02,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x24,0x01
};
#endif

/**************************************************************************
***                          设置SDA脚为输入                             ***
***************************************************************************/
static void I2C_Set_SDA_Input()
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    GPIO_InitStruct.Pin = I2C_SDA_Pin;    
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;         //浮空输入模式
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(I2C_SDA_Port, & GPIO_InitStruct);
}
/**************************************************************************
***                          设置SDA脚为输出                             ***
***************************************************************************/
static void I2C_Set_SDA_Output()
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    GPIO_InitStruct.Pin = I2C_SDA_Pin;     
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;     //推挽输出模式
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(I2C_SDA_Port, & GPIO_InitStruct);
}

static void I2C_Start()
{
    I2C_SCL_LOW;                    // 冗余操作，iic开始通信后没有停止时，SCL可能处于高电平，所以先将SCL拉低，确保通信开始时的状态正确
    IIC_Delay_us(I2C_DELAY);

    I2C_Set_SDA_Output(); // 确保SDA是输出模式
    I2C_SDA_HIGH;
    I2C_SCL_HIGH;
    IIC_Delay_us(I2C_DELAY);
    I2C_SDA_LOW;
    IIC_Delay_us(I2C_DELAY);
}

static void I2C_Stop()
{
    I2C_SCL_LOW;                    
    IIC_Delay_us(I2C_DELAY);

    I2C_Set_SDA_Output(); // 确保SDA是输出模式
    I2C_SDA_LOW;
    I2C_SCL_HIGH;
    IIC_Delay_us(I2C_DELAY);
    I2C_SDA_HIGH;
    IIC_Delay_us(I2C_DELAY);
}

static uint8_t I2C_Wait_Ack()
{
    uint16_t wait_time=0;
    uint8_t ack_nack = 1;
    I2C_SCL_LOW;
    IIC_Delay_us(I2C_DELAY);
    I2C_Set_SDA_Input();    // 确保SDA是输入模式
    
    
    //等待SDA脚被从机拉低
    while(I2C_SDA_STATUS)
    {
        wait_time++;
        //如果等待时间过长，则退出等待
        if (wait_time>=2000)
        {
            ack_nack = 0;
            break;
        }
    }
    // SCL由0变为1，读入ACK状态
    // 如果此时SDA=0，则是ACK
    // 如果此时SDA=1，则是NACK
    I2C_SCL_HIGH;
    IIC_Delay_us(I2C_DELAY);
    
    return ack_nack;
}

static void I2C_Send_Ack()
{
    // 发送ACK就是在SDA=0时，SCL由0变成1
    I2C_SCL_LOW;
    IIC_Delay_us(I2C_DELAY);
    I2C_Set_SDA_Output(); // 确保SDA是输出模式

    I2C_SDA_LOW;
    IIC_Delay_us(I2C_DELAY);
    I2C_SCL_HIGH;
    IIC_Delay_us(I2C_DELAY);
}

static void I2C_Send_NAck()
{
    // 发送NACK就是在SDA=1时，SCL由0变成1
    I2C_SCL_LOW;
    IIC_Delay_us(I2C_DELAY);
    I2C_Set_SDA_Output(); // 确保SDA是输出模式

    I2C_SDA_HIGH;
    IIC_Delay_us(I2C_DELAY);
    I2C_SCL_HIGH;
    IIC_Delay_us(I2C_DELAY);
}

static void I2C_Write_Byte(uint8_t aByte)
{
    uint8_t i;
    I2C_SCL_LOW;
    IIC_Delay_us(I2C_DELAY);
    I2C_Set_SDA_Output(); // 确保SDA是输出模式

    for (i=0;i<8;i++)
    {
        //先将SCL拉低；
        I2C_SCL_LOW;
        IIC_Delay_us(I2C_DELAY);
        //然后在SDA输出数据
        if(aByte&0x80)
        {
            I2C_SDA_HIGH;
        }
        else
        {
            I2C_SDA_LOW;
        }
        IIC_Delay_us(I2C_DELAY);
        //最后将SCL拉高，在SCL上升沿写入数据
        I2C_SCL_HIGH;
        IIC_Delay_us(I2C_DELAY);
        
        aByte = aByte<<1;//数据位移
    }
}

static uint8_t I2C_Read_Byte()
{
    uint8_t aByte = 0;
    I2C_SCL_LOW;
    IIC_Delay_us(I2C_DELAY);    //延时等待SDA数据稳定
    I2C_Set_SDA_Input();    // 确保SDA是输出模式

    for (uint8_t i=0;i<8;i++)
    {
        I2C_SCL_LOW;
        IIC_Delay_us(I2C_DELAY);    //延时等待SDA数据稳定
        I2C_SCL_HIGH;    // SCL=1，锁定SDA数据
        IIC_Delay_us(I2C_DELAY);
        aByte |= (I2C_SDA_STATUS << (7 - i)); // SCL=1，锁定SDA数据，并将数据位移到正确位置
    }
    return aByte;
}

void I2C_Write_nBytes(uint8_t i2c_addr,uint8_t *data,uint8_t len)
{
    //启动I2C
    I2C_Start();
    //写I2C从机地址，写操作
    I2C_Write_Byte((i2c_addr<<1)|0x00);
    //如果从机响应ACC则继续，如果从机未响应ACK则停止
    if (!I2C_Wait_Ack())
        goto err;
    
    //开始写n个字节数据
    for (uint8_t j=0;j<len;j++)
    {
        I2C_Write_Byte(data[j]);
        // 每写一个字节数据后，都要等待从机回应ACK
        if (!I2C_Wait_Ack())
            goto err;
    }
    
    //停止I2C
    err:
    I2C_Stop();
}

void I2C_Read_nBytes(uint8_t i2c_addr,uint8_t *buf,uint8_t len)
{
    uint8_t j;
    
    //启动I2C
    I2C_Start();
    //写I2C从机地址，读操作
    I2C_Write_Byte((i2c_addr<<1)|0x01);
    //如果从机响应ACC则继续，如果从机未响应ACK则停止
    if (!I2C_Wait_Ack())
        goto err;
    //开始读n个字节数据
    for (j=0;j<len;j++)
    {
        buf[j]=I2C_Read_Byte();
        // 每读一个字节数据后，都要发送ACK给从机
        I2C_Send_Ack();
    }
    
    //停止I2C
    err:
    I2C_Stop();
}

uint8_t I2C_Scan_Addr()
{
    uint8_t address;
    for(address=0; address<126; address++)
    {
        I2C_Start();
        //写I2C从机地址，读操作
        I2C_Write_Byte((address<<1)|0x01);
        if (I2C_Wait_Ack())
        {
            I2C_Stop();
            return address; // 找到设备，返回地址
        }
        // I2C_Stop();
    }
    return address; // 没有设备响应，返回128
}

void GT911_Reset(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);               // 先停止这个中断，避免在复位过程中被触发
    GPIO_InitStruct.Pin = GT911_INT_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;         // 推挽输出模式
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GT911_INT_port, &GPIO_InitStruct);

    GT911_RST_state(GPIO_PIN_RESET);                        // 将INT引脚拉低，确保GT911进入复位状态
    GT911_INT_state(GPIO_PIN_RESET);                        // 将RST引脚拉低，开始复位
    delay_ms(20);
    GT911_RST_state(GPIO_PIN_RESET);                        
    GT911_INT_state(GPIO_PIN_SET); 
    delay_ms(20);                                        // 等待20ms，确保GT911完成复位
    GT911_RST_state(GPIO_PIN_SET);                        
    GT911_INT_state(GPIO_PIN_SET); 
    delay_ms(20); 
    GT911_RST_state(GPIO_PIN_SET);                        
    GT911_INT_state(GPIO_PIN_RESET);
    delay_ms(200);

    GPIO_InitStruct.Pin = GT911_INT_pin;                    // 重新配置回中断模式
    // GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GT911_INT_port, &GPIO_InitStruct);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);                // 启用中断
    delay_ms(100);
}

uint8_t GT911_Calc_Checksum(uint8_t *config_buf)
{
    uint32_t sum = 0;  // 用32位避免溢出

    // 累加 184 个字节
    for (int i = 0; i < 184; i++)
    {
        sum += config_buf[i];
    }

    // 8位补码：0x100 - sum
    uint8_t checksum = (0x100 - (sum & 0xFF)) & 0xFF;

    return checksum;
}

void GT911_Init(void)
{
    GT911_Reset();
    // 读取ID
    uint8_t temp[200];
    uint8_t result = GT911_ReadReg(GT_PID_REG,temp,14);      // 读取GT911的产品ID寄存器，正常情况下应该返回"911"字符串
    temp[4]=0;
    printf("GTP_ID:%5s\n",temp);
    if(strcmp((char*)temp,"911")==0)//ID==911
	{
        temp[0]=0X02;			
		GT911_WriteReg(GT_CTRL_REG,temp,1);//软复位GT911

        // 写入配置文件，计算并写入校验和
        // uint8_t checksum = GT911_Calc_Checksum((uint8_t*)CTP_CFG_GT911);
        // uint8_t frech = 0x01; // 刷新寄存器的值
        // temp[0] = GT911_WriteReg(GT_CFGS_REG, CTP_CFG_GT911, 184); // 先将配置数据写入GT911
        // temp[1] = GT911_WriteReg(GT_CHECK_REG, &checksum, 1);
        // temp[2] = GT911_WriteReg(GT_FRESH_REG, CTP_CFG_GT911+185, 1);

		GT911_ReadReg(GT_CFGS_REG,temp,186);//读取GT_CFGS_REG寄存器
        printf("CFGS:");
        for (int i = 0; i < 186; i++)
        {
            if ((i % 10) == 0)
            {
                printf("\n");
            }
            printf("%02X ", temp[i]);
        }
        printf("\n");
		delay_ms(10);
		temp[0]=0X00;	 
		GT911_WriteReg(GT_CTRL_REG,temp,1);//结束复位   
	} 
}

uint8_t GT911_ReadReg(uint16_t reg_addr, uint8_t *buf, uint8_t len)
{
    uint8_t status = 0;             // 状态指示器，0成功，其他失败
    uint8_t addr_buf[2];
    addr_buf[0] = (reg_addr >> 8) & 0xFF; // 高字节
    addr_buf[1] = reg_addr & 0xFF;        // 低字节

    I2C_Start();        // 启动I2C
    I2C_Write_Byte((GT911_I2C_ADDR<<1)|0x00);     // 写入设备地址，写操作
    if (!I2C_Wait_Ack()){       // 等待响应
        status = 1;             // 写入设备地址失败
        goto err;          
    }

    for(uint8_t i=0; i<2; i++)
    {
        I2C_Write_Byte(addr_buf[i]);            // 写入需要读取的寄存器地址
        if (!I2C_Wait_Ack()){       // 等待响应
            status = 2;             // 写入寄存器地址失败
            goto err;          
        }
    }
    I2C_Stop();


    I2C_Start();        // 重新启动I2C
    I2C_Write_Byte((GT911_I2C_ADDR<<1)|0x01);     // 写设备地址，读操作
    if (!I2C_Wait_Ack()){       // 等待响应
        status = 3;             // 写入设备地址失败
        goto err;          
    }
    for(uint16_t i=0; i<len; i++)
    {
        buf[i] = I2C_Read_Byte();              // 读取数据
        if (i < len - 1) {
            I2C_Send_Ack();                    // 发送ACK
        } else {
            I2C_Send_NAck();                   // 最后一个字节发送NACK
        }
    }

err:
    I2C_Stop();         // 停止I2C

    return status;
}

uint8_t GT911_WriteReg(uint16_t reg_addr, uint8_t *buf, uint8_t len)
{
    uint8_t status = 0;             // 状态指示器，0成功，其他失败
    uint8_t addr_buf[2];
    addr_buf[0] = (reg_addr >> 8) & 0xFF; // 高字节
    addr_buf[1] = reg_addr & 0xFF;        // 低字节

    I2C_Start();        // 启动I2C
    I2C_Write_Byte((GT911_I2C_ADDR<<1)|0x00);     // 写入设备地址，写操作
    if (!I2C_Wait_Ack()){       // 等待响应
        status = 1;             // 写入设备地址失败
        goto err;          
    }

    for(uint8_t i=0; i<2; i++)
    {
        I2C_Write_Byte(addr_buf[i]);            // 写入需要读取的寄存器地址
        if (!I2C_Wait_Ack()){       // 等待响应
            status = 2;             // 写入寄存器地址失败
            goto err;          
        }
    }

    for(uint16_t i=0; i<len; i++)
    {
        I2C_Write_Byte(buf[i]);              // 写入数据
        if (!I2C_Wait_Ack()){       // 等待响应
            status = 3;             // 写入数据失败
            goto err;          
        }
    }

err:
    I2C_Stop();         // 停止I2C

    return status;
}


void GT911_Callback()
{
    uint8_t touch_status;
    uint8_t touch_data[20];
    uint8_t temp = GT911_ReadReg(0x814E, &touch_status, 1);
    printf("GT911_ReadReg(0x814E) result: %d, touch_status: 0x%02X\n", temp, touch_status);
    if (touch_status & 0x80)
    {
        // GT911_ReadReg(0x8150, touch_data, 20);
        // 处理触摸数据
        for (int i = 0; i < (touch_status & 0x0F); i++)
        {
            uint16_t x = (touch_data[i * 4 + 1] << 8) | touch_data[i * 4];
            uint16_t y = (touch_data[i * 4 + 3] << 8) | touch_data[i * 4 + 2];
            // 可以在这里进行触摸坐标的处理
            printf("Touch %d: x=%d, y=%d\n", i, x, y);
        }
        // 清除标志位
        touch_status = 0;
        GT911_WriteReg(0x814E, &touch_status, 1);
    }
}
