#include "gt911.h"

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
    
    I2C_Set_SDA_Input();    // 确保SDA是输入模式
    I2C_SCL_LOW;
    
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
    I2C_Set_SDA_Output(); // 确保SDA是输出模式
    I2C_SCL_LOW;
    IIC_Delay_us(I2C_DELAY);
    I2C_SDA_LOW;
    IIC_Delay_us(I2C_DELAY);
    I2C_SCL_HIGH;
    IIC_Delay_us(I2C_DELAY);
}

static void I2C_Send_NAck()
{
    // 发送NACK就是在SDA=1时，SCL由0变成1
    I2C_Set_SDA_Output(); // 确保SDA是输出模式
    I2C_SCL_LOW;
    IIC_Delay_us(I2C_DELAY);
    I2C_SDA_HIGH;
    IIC_Delay_us(I2C_DELAY);
    I2C_SCL_HIGH;
    IIC_Delay_us(I2C_DELAY);
}

static void I2C_Write_Byte(uint8_t aByte)
{
    uint8_t i;
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
        I2C_Write_Byte((address)|0x01);
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
    GT911_RST_state(GPIO_PIN_RESET);
    delay_ms(10);
    GT911_RST_state(GPIO_PIN_SET);
    delay_ms(100);
}

void GT911_Init(void)
{
    GT911_Reset();
    // 这里可以添加其他初始化代码，例如配置触摸屏参数等
}

void GT911_Callback()
{
    // uint8_t touch_status;
    // uint8_t touch_data[20];
    // // GT911_ReadReg(0x814E, &touch_status, 1);
    // if (touch_status & 0x80)
    // {
    //     // GT911_ReadReg(0x8150, touch_data, 20);
    //     // 处理触摸数据
    //     for (int i = 0; i < touch_status & 0x0F; i++)
    //     {
    //         uint16_t x = (touch_data[i * 4 + 1] << 8) | touch_data[i * 4];
    //         uint16_t y = (touch_data[i * 4 + 3] << 8) | touch_data[i * 4 + 2];
    //         // 可以在这里进行触摸坐标的处理
    //     }
    //     // 清除标志位
    //     touch_status = 0;
    //     // GT911_WriteReg(0x814E, &touch_status, 1);
    // }
}
