/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:   
 * @file name: soc_flash.h
 * @Description: soc flash include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-05-06 10:00:26
 * @LastEditTime: 2019-10-18 10:55:03
 */

#ifndef __SOC_FLASH_H__
#define __SOC_FLASH_H__
#include "tuya_cloud_types.h"
#include "uni_log.h"
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define APP_DP_DATA_OFFSET          0
#define APP_DP_DATA_OFFSET1         16
#define APP_DP_DATA_OFFSET2         32
/**
 * @brief soc flash save type enum
 */
typedef enum {
    SAVE_TYP1 = 0,
    SAVE_TYP2,
    SAVE_TYP3,
    SAVE_TYP4,
    SAVE_TYP_MAX,
}SOC_FLASH_SAVE_TYPE_E;

/**
 * @brief: soc data save
 * @param {IN SOC_FLASH_SAVE_TYPE_E eDataType -> save type(meaning data kind)}
 * @param {IN UINT_T uiAddr -> this type data address offset}
 * @param {IN UCHAR_T *pData -> save data}
 * @param {IN USHORT_T usLen -> save data len}
 * @retval: OPERATE_RET
 */
INT_T opSocFlashWrite(IN SOC_FLASH_SAVE_TYPE_E eDataType, IN UINT_T uiAddr, IN UCHAR_T *pData, IN USHORT_T usLen);
/**
 * @brief: soc flash save data read
 * @param {IN SOC_FLASH_SAVE_TYPE_E eDataType -> read data type(meaning data kind)}
 * @param {IN UINT_T uiAddr -> this type data address offset}
 * @param {IN USHORT_T ucLen -> read data len}
 * @param {OUT UCHAR_T *pData -> read data}
 * @retval: read data cnt
 */
INT_T uiSocFlashRead(IN SOC_FLASH_SAVE_TYPE_E eDataType, IN UINT_T uiAddr, IN USHORT_T usLen, OUT UCHAR_T *pData);


/**
 * @brief: soc flash special block delete
 * @param {none}
 * @return: OPERATE_RET
 * @retval: none
 */
INT_T opSocFlashEraseSpecial(IN SOC_FLASH_SAVE_TYPE_E DataType, IN UINT_T addr);
/**
 * @brief: soc flash erase all
 * @param {none}
 * @retval: OPERATE_RET
 */
INT_T opSocFlashErase(VOID);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __SOC_FLASH_H__ */
