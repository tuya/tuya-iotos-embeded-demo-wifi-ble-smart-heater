/*
 * @file name: tc309.c
 * @Descripttion: tc309 drive
 * @Author: xjw
 * @email: fudi.xu@tuya.com
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-04-26 16:35:33
 * @LastEditors: xjw
 * @LastEditTime: 2021-03-22 15:21:28
 */
#include "tc309.h"
#include "uni_log.h"
#include "soc_i2c.h"
/*============================ PROTOTYPES ====================================*/

/*============================ IMPLEMENTATION ================================*/
static UCHAR_T senset[20] = {0x04,0x15,0x25,0x36,0x47,0x58,0x68,0x79,0x8a,0x9b,0xac,0xbc,0xcd,0xde,0xef,0xff};   //sensitivity set


STATIC UCHAR_T gTc309InitFlag = FALSE;
/**
 * @brief: TC309 IIC delay proc
 * @param {none} 
 * @retval: none
 */
STATIC VOID vTc309Delay(USHORT_T usTime)
{       
    vSocI2CDelay(usTime);
}

/**
 * @brief: TC309 IIC start transport
 * @param {none}
 * @retval: OPERATE_LIGHT
 */
STATIC int vTc309Start(VOID)
{
    vSocI2CSclSet();
    vSocI2CSdaSet();

    vTc309Delay(2);

    if(!ucSocI2CSdaInputRead()) {    /* confirm SDA status is ready */
        return -1;
    }

    vSocI2CSdaReset();
    vTc309Delay(2);

    if(ucSocI2CSdaOutputRead()) {    /* confirm SDA status is set correct */
        return -1;
    }

    return 0;
}

/**
 * @brief: TC309 IIC stop transport
 * @param {none}
 * @retval: none
 */
STATIC VOID vTc309Stop(VOID)
{
    vSocI2CSdaReset();
    vSocI2CSclReset();

    vSocI2CSclSet();
    vTc309Delay(2);

    vSocI2CSdaSet();
    vTc309Delay(2);
}

/**
 * @brief: TC309 IIC wait ACK
 * @param {SDA_IO -> SDA pin}
 * @param {SCL_IO -> SCL pin}
 * @retval: OPERATE_LIGHT
 */
STATIC int vTc309WaitAck(VOID)
{
    vSocI2CSclReset();
    vTc309Delay(1);

    vSocI2CSclSet();
    vTc309Delay(1);
    if(ucSocI2CSdaOutputRead()) {     /* receive ACK NG */
        return -1;
    }
    
    vTc309Delay(5);
    return 0;
}

/**
 * @brief: TC309 IIC send byte
 * @param {UCHAR_T ucSendByte -> send one byte}
 * @retval: none
 */
STATIC VOID vTc309SendOneByte(UCHAR_T ucSendByte)
{
    UCHAR_T i = 0;

    for(i = 0; i < 8; i++)
    {
        vSocI2CSclReset();
        vTc309Delay(2);

        if(ucSendByte & 0x80) {
            vSocI2CSdaSet();
        } else {
            vSocI2CSdaReset();
        }

        vSocI2CSclSet();
        vTc309Delay(2);

        ucSendByte <<= 1;
   }
}

/**
 * @brief: TC309 write data proc
 * @param {*pBuffer -> write data buf}
 * @param {NumByteToWrite -> write data len}
 * @retval: OPERATE_LIGHT 
 */
STATIC int vTc309WritePage(UCHAR_T *pBuffer, USHORT_T usNumByteToWrite)
{
    int opRet = 0;

    if( NULL == pBuffer ) {
        PR_ERR("TC309 write data is invalid!");
        return -1;
    }
 
    opRet = vTc309Start();     /* start transport */
    if( 0 != opRet ) {
        PR_ERR("IIC is busy!");
        return -1;
    }
    
    while(usNumByteToWrite --) {
        
        vTc309SendOneByte(*pBuffer++);
        vTc309WaitAck();
    }

    vTc309Stop();
    vTc309Delay(1000);

    return 0;
}

unsigned char i2c_read_byte()
{
    vSocI2CSclReset();
    unsigned char i,k;
    for(i=0;i<8;i++)
    {
    vTc309Delay(2);
    vSocI2CSclSet();
    // i2c_delay();
    k=(k<<1)|vSocI2CSdaget();
    vSocI2CSclReset();
    }
    return k;
}


void i2c_read_direct(UCHAR_T dev_addr,UCHAR_T * dest_buf,UCHAR_T len)
{
    char i;
    vTc309Start();//启动
    vTc309SendOneByte((dev_addr<<1)+1);
    vTc309WaitAck();
    dest_buf[0]=i2c_read_byte();
    for(i=1;i<len;i++)
    {
    i2c_sendack();
    dest_buf[i]=i2c_read_byte();
    }
    i2c_sendnack();
    vTc309Stop();//停止
}

void i2c_sendack() 
{
    vSocI2CSdaReset();
    vTc309Delay(1);
    vSocI2CSclSet();
    vTc309Delay(2);
    vSocI2CSclReset();
    vSocI2CSdaSet();

}
void i2c_sendnack() 
{
    vSocI2CSdaSet();
    vTc309Delay(1);
    vSocI2CSclSet();
    vTc309Delay(2);

    vSocI2CSclReset();

}

int UserTc309Init()
{
    int opRet = -1;

    if(gTc309InitFlag != FALSE) {
        PR_NOTICE("Tc309 already init!");
        return 0;
    }
    I2C_PIN_T tI2CConfig = {
        .ucSdaPin = 15,
        .ucSclPin = 17,
    };
    opRet = opSocI2CInit(&tI2CConfig);          /* SDA&SCL GPIO INIT */
    if(opRet != 0) {
        PR_ERR("Tc309 I2C init error!");
        return -1;
    }

    gTc309InitFlag = TRUE;

    return 0;
}



int UserTc309Set_Sensitivity(UCHAR_T level)
{
        int opRet = 0;
        UCHAR_T IIC_Sendbuf[5] = {0};
        IIC_Sendbuf[0] = 0x80 ;        
        IIC_Sendbuf[1] = 0x00;
        IIC_Sendbuf[2] = senset[level];
        IIC_Sendbuf[3] = senset[level];
        opRet = vTc309WritePage(IIC_Sendbuf, 4);
        IIC_Sendbuf[0] = 0x80 ;        
        IIC_Sendbuf[1] = 0x08;
        opRet = vTc309WritePage(IIC_Sendbuf, 2);
    return 0;
}
