/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:   
 * @file name: soc_flash.c
 * @Description: soc flash proc
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-05-06 10:00:26
 * @LastEditTime: 2019-10-21 14:15:59
 */

#include "soc_flash.h"
#include "uf_file.h"
#include "tuya_cloud_error_code.h"

STATIC BOOL_T bSocFlashInitFlag = FALSE;



/**
 * @brief: wifi uf write(a+ mode)
 * @param {IN CHAR_T *pFilename -> file name}
 * @param {IN UCHAR_T *pData -> save data}
 * @param {IN USHORT_T usLen -> save data len}
 * @retval: OPERATE_LIGHT
 */
STATIC INT_T opSocFlashFileWrite(IN CHAR_T *pFilename, IN UCHAR_T *pData, IN USHORT_T usLen)
{
    int opRet = -1;
    uFILE * fp = NULL;
    UINT_T uiOffSet = 0;
    UINT_T uiWriteCnt = 0;
    
    fp = ufopen(pFilename, "a+");
    if(NULL == fp) {
        PR_ERR("uf file %s can't open and write data!", pFilename);
        return OPRT_COM_ERROR;
    }
    
    uiOffSet = ufseek(fp, 0, APP_DP_DATA_OFFSET);
    if(uiOffSet != 0) {
        PR_ERR("uf file %s Set file offset to 0 error!", pFilename);
        return OPRT_COM_ERROR;
    }

    uiWriteCnt = ufwrite(fp, pData, usLen);
    if(uiWriteCnt != usLen) {
        PR_ERR("uf file %s write data error!", pFilename);
        return OPRT_COM_ERROR;
    }

    opRet = ufclose(fp);
    if(opRet != OPRT_OK) {
        PR_ERR("uf file %s close error!", pFilename);
        return opRet;
    }

    return OPRT_OK;
}


/**
 * @brief: soc data save
 * @param {IN SOC_FLASH_SAVE_TYPE_E eDataType -> save type(meaning data kind)}
 * @param {IN UINT_T uiAddr -> this type data address offset}
 * @param {IN UCHAR_T *pData -> save data}
 * @param {IN USHORT_T usLen -> save data len}
 * @retval: OPERATE_LIGHT
 */
INT_T opSocFlashWrite(IN SOC_FLASH_SAVE_TYPE_E eDataType, IN UINT_T uiAddr, IN UCHAR_T *pData, IN USHORT_T usLen)
{
    int opRet = -1;
    CHAR_T cTemp[4] = {0};

    if(bSocFlashInitFlag != TRUE) {

        bSocFlashInitFlag = TRUE;
    }

    if(eDataType >= SAVE_TYP_MAX) {
        PR_ERR("Write soc flash type error!");
        return OPRT_INVALID_PARM;
    }

    /*set data type tag*/
    snprintf(cTemp, 4, "%d", eDataType);
    opRet = opSocFlashFileWrite(cTemp, pData, usLen);
    if(opRet != OPRT_OK) {
        return opRet;
    }
    
    return OPRT_OK;
}

/**
 * @brief: wifi uf read
 * @param {IN CHAR_T *pFilename -> read file name}
 * @param {IN USHORT_T usLen -> read data len}
 * @param {OUT UCHAR_T *pData -> read data}
 * @retval: read data cnt
 */
STATIC INT_T uiSocFlashFileRead(IN CHAR_T *pFilename, IN USHORT_T usLen, OUT UCHAR_T *pData)
{
    int opRet = -1;
    uFILE * fp = NULL;
    INT_T uiReadCnt = 0;

    fp = ufopen(pFilename, "r+");
    if(NULL == fp) {
        PR_ERR("uf file %s can't open and read data!", pFilename);
        return OPRT_COM_ERROR;
    }

    PR_DEBUG("uf open OK");
    uiReadCnt = ufread(fp, pData, usLen);
    PR_DEBUG("uf file %s read data %d!", pFilename, uiReadCnt);

    opRet = ufclose(fp);
    if(opRet != OPRT_OK) {
        PR_ERR("uf file %s close error!", pFilename);
        return opRet;
    }
    
    return uiReadCnt;
}

/**
 * @brief: soc flash save data read
 * @param {IN SOC_FLASH_SAVE_TYPE_E eDataType -> read data type(meaning data kind)}
 * @param {IN UINT_T uiAddr -> this type data address offset}
 * @param {IN USHORT_T ucLen -> read data len}
 * @param {OUT UCHAR_T *pData -> read data}
 * @retval: read data cnt
 */
INT_T uiSocFlashRead(IN SOC_FLASH_SAVE_TYPE_E eDataType, IN UINT_T uiAddr, IN USHORT_T usLen, OUT UCHAR_T *pData)
{
    int opRet = -1;
    INT_T uiReadCnt = 0;
    CHAR_T cTemp[4] = {0};
    
    if(bSocFlashInitFlag != TRUE) {
        opRet = uf_file_app_init("12345678901234567890123456789012", 32);
        if(opRet != OPRT_OK) {
            PR_ERR("uf file init error! can't write or read!");
            return opRet;
        }
        bSocFlashInitFlag = TRUE;
    }

    if(eDataType >= SAVE_TYP_MAX) {
        PR_ERR("Read soc flash type error!");
        return OPRT_INVALID_PARM;
    }
    
    snprintf(cTemp, 4, "%d", eDataType);
    PR_DEBUG("file name %s",cTemp);
    uiReadCnt = uiSocFlashFileRead(cTemp, usLen, pData);

    return uiReadCnt;
}


/**
 * @brief: soc flash special block delete
 * @param {none}
 * @retval: OPERATE_LIGHT
 */
INT_T opSocFlashEraseSpecial(IN SOC_FLASH_SAVE_TYPE_E DataType, IN UINT_T addr)
{
    int opRet = 0;
    CHAR_T cTemp[4] = {0};
    
    if(bSocFlashInitFlag != TRUE) {
        return OPRT_OK;     /* directly return */
    }
    snprintf(cTemp, 4, "%d", DataType);
    opRet = ufdelete(cTemp);
    if(opRet != OPRT_OK) {
        PR_ERR("Delete %s file error!", cTemp);
   }

    return OPRT_OK;

}

/**
 * @brief: soc flash erase all
 * @param {none}
 * @retval: OPERATE_LIGHT
 */
INT_T opSocFlashErase(VOID)
{
    int opRet = -1;
    CHAR_T cTemp[4] = {0};
    
    if(bSocFlashInitFlag != TRUE) {
        return OPRT_OK;     /* directly return */
    }
    snprintf(cTemp, 4, "%d", SAVE_TYP1);
    opRet = ufdelete(cTemp);
    if(opRet != OPRT_OK) {
        PR_ERR("Delete %s file error!", cTemp);
    }
    snprintf(cTemp, 4, "%d", SAVE_TYP2);
    opRet = ufdelete(cTemp);
    if(opRet != OPRT_OK) {
        PR_ERR("Delete %s file error!", cTemp);
    }
    
    snprintf(cTemp, 4, "%d", SAVE_TYP3);
    opRet = ufdelete(cTemp);
    if(opRet != OPRT_OK) {
        PR_ERR("Delete %s file error!", cTemp);
    }
    
    snprintf(cTemp, 4, "%d", SAVE_TYP4);
    opRet = ufdelete(cTemp);
    if(opRet != OPRT_OK) {
        PR_ERR("Delete %s file error!", cTemp);
    }

    opRet = ufdelete("oem_cfg");
    if(opRet != OPRT_OK) {
        PR_ERR("Delete oem_cfg file error!");
    }

    return OPRT_OK;

}


