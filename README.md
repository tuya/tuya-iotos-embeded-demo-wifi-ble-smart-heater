# Tuya IoTOS Embeded Demo WiFi & BLE  smart heater

[English](./README.md) | [中文](./README_zh.md) 



## Introduction 

This Demo is based on Tuya Smart Cloud Platform, Tuya Smart APP, IoTOS Embeded WiFi &Ble SDK, using Tuya WiFi/WiFi+BLE series modules quickly build a smart heater DEMO.


## Quick start 

### Development environment build 

+  Install the Linux development environment 
+  Download [Tuya IoTOS Embeded WiFi & BLE sdk](https://github.com/tuya/tuya-iotos-embeded-sdk-wifi-ble-bk7231n) 
+  Download the Demo to the apps directory of the SDK directory 

  ```bash
  $ cd apps
  $ git clone https://github.com/tuya/tuya-iotos-embeded-demo-wifi-ble-smart-heater.git
  ```

### Compile and burn

+ code modifications

  1. Fill in`tuya_device.h` with the PID of the smart product created in the Tuya IoT workbench.
     ```c
     #define PRODECT_ID     "xxxxxxxx"
     ```
     Replace `xxxxxxxx` with the PID. 
  
+ Compile and run the demo code

    Execute the following command in the SDK root directory to start compiling.
  
    ```bash
    sh build_app.sh apps/bk7231n_smart_heater_demo bk7231n_smart_heater_demo 1.0.0 
    ```
  
    Firmware burn-in license information please refer to：[Wi-Fi + BLE series module burn-in license](https://developer.tuya.com/cn/docs/iot/device-development/burn-and-authorization/burn-and-authorize-wifi-ble-modules/burn-and-authorize-wb-series-modules?id=Ka78f4pttsytd) 



### File description 

|     File name     |                           Function                           |
| :---------------: | :----------------------------------------------------------: |
|   tuya_device.c   |                    Project entry document                    |
| tuya_dp_process.c |                  DP data processing related                  |
|   tuya_thread.c   |               Logic controls thread dependency               |
|      b3950.c      |            B3950 temperature sensor drive related            |
|  gpio_control.c   |                   IO port control related                    |
|    interrupt.c    | External interrupts are associated with the use of trigger keys for acquisition |
|  led_color_set.c  |                      Driver LED Related                      |
|    soc_flash.c    |                   Flash operation related                    |
|     soc_i2c.c     |                         IIC drivers                          |
|      tc309.c      |             TC309 digital tube driver chip code              |
|      timer.c      |                   Timer related operations                   |
|     tm1650.c      |                TM1650 key driver chip related                |



### Application entry

Entry file: tuya_device.c

Important functions: device_init()

+ Call tuya_iot_wf_soc_dev_init_param() interface to initialize the SDK, configure the operating mode, the mating mode, and register various callback functions and store the firmware key and PID.
+ Calling the tuya_iot_reg_get_wf_nw_stat_cb() interface to register the device network status callback functions.
+ Call the application layer initialization function calorifier_init().

<br>

### dp point related

+ Send down dp point data stream: dev_obj_dp_cb() -> deal_dp_proc()
+ Report dp point interface: dev_report_dp_json_async()

| function name | OPERATE_RET dev_report_dp_json_async(IN CONST CHAR_T *dev_id,IN CONST TY_OBJ_DP_S *dp_data,IN CONST UINT_T cnt) |
| ------------- | ------------------------------------------------------------ |
| devid         | device id (if it is a gateway, MCU, SOC class device then devid = NULL; if it is a sub-device, then devid = sub-device_id) |
| dp_data       | dp structure array name                                      |
| cnt           | number of elements of the dp structure array                 |
| return        | OPRT_OK: Success Other: Failure                              |



## Related documentation 

+ [CBU Module Description ](https://developer.tuya.com/cn/docs/iot/cbu-module?id=Ka8uv3u1q1xxc) 
+ [Smart Heater Demo Detailed Instructions ](https://developer.tuya.com/cn/demo/smart-heater) 
+ [Tuya Demo Center](https://developer.tuya.com/demo)  



## Technical Support 

You can get support for Tuya in the following ways:

+ Developer Center：https://developer.tuya.com
+ Help Center https://support.tuya.com/help
+ Technical Support Work Order Center: [https://service.console.tuya.com](https://service.console.tuya.com/) 