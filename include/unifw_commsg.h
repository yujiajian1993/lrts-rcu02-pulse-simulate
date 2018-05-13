#ifndef  _UNIFW_COMMSG_H
#define  _UNIFW_COMMSG_H

#include "unifw.h"

#ifdef __cplusplus
extern "C"{
#endif

#define CAN_DATA_MAX_LEN  (8u)       /*-< CAN数据最大长度      */
#define UDP_DATA_MAX_LEN  (1600)    /*-< UDP数据最大长度      */
#define UART_DATA_MAX_LEN (600u)     /*-< UART数据最大长度    */
#define ACC_NBR (2u)                 /*-< 加速度计数量            */
#define SPD_SENSOR_CHANNEL_NBR (2u)  /*-< 速度脉冲传感器数量 */

#define ETH_IPv4_MAC_STR_LEN (6*2 + (6-1) + 1)  /*-< MAC地址字节数 */
#define ETH_IPv4_IP_STR_LEN  (3*4+4-1+1)
#define ETH_IPv4_IP_NBR      (4u)  /*-< IP地址字节数 */

typedef struct _udpmsg
{
	uint8_t      ip_from[ETH_IPv4_IP_NBR]; /*-< 发送方的IP地址 */
	uint16_t                    port_from; /*-< 发送方的端口 */
	uint32_t                    timestamp; /*-< 数据接收的本地时间 */
	uint16_t                       length; /*-< 数据长度 */
	uint8_t        data[UDP_DATA_MAX_LEN]; /*-< 数据内容 */
} udpmsg_t; /*-< UDP报文数据结构 */

typedef struct _canmsg{
	uint32_t timestamp;  /*-< 时间戳 */
	uint32_t id;         /*-< CAN报文ID */
	uint8_t  ch;         /*-< CAN通信通道 */
	uint8_t  length;     /*-< CAN数据长度 */
	uint8_t  IDE;        /*-< CAN标准帧/扩展帧标志 */
	uint32_t id_ext;
	uint32_t id_std;
	uint8_t  data[CAN_DATA_MAX_LEN]; /*-< CAN数据 */
} canmsg_t;

typedef struct _uartmsg
{
	uint32_t timestamp;  /*-< 数据接收的本地时间 */
	uint16_t length;     /*-< 数据长度          */
	uint8_t  data[UART_DATA_MAX_LEN];  /*-< 数据内容          */
} uartmsg_t; /*-< 串口接收数据结构 */

typedef struct _speedmsg
{
	uint16_t ts       ;
    uint8_t  dir      ; /*-< 速度脉冲相位方向 */
    uint32_t count    ; /*-< 累计计数       */
    uint8_t  adc_code ; /*-< 断线标识 */
} speedmsg_t; /*-< 平台测速数据结构 */

typedef struct _accmsg
{
	uint16_t ts;
	uint8_t  acc_id;
	int32_t  ax;  /*-< 方向     */
	int32_t  ay;  /*-< 方向     */
	int32_t  az;  /*-< 方向     */
} accmsg_t     ; /*-< 加速度信息 */

/** 实时时钟结构体 */
typedef struct _rt_time{
    uint16_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  second;
    uint16_t ms;
} rt_time_t;

#define ADC_OK (0u)

#define GP_DIR_ERR      (0u)
#define GP_DIR_FORWARD  (1u)
#define GP_DIR_BACKWARD (2u)

#define SPD_VOTE_INITED   (0u)
#define SPD_VOTE_OK       (1u)
#define SPD_VOTE_ERR      (2u)
#define SPD_VOTE_WARNING  (3u)
#define SPD_ADC_ERR       (4u)
#define SPD_BOARD_ERR	  (5u)

typedef struct _speed_status
{
	uint8_t status;
}speed_status_t;

#define VDI_VOTE_INITED (0u)
#define VDI_VOTE_0      (1u)
#define VDI_VOTE_1      (2u)
#define VDI_VOTE_ERR    (3u)
#define VDI_BOARD_ERR   (4u)

typedef struct _vdi_status
{
	uint8_t status;
	uint8_t conflict_continuous_period; /*- 冲突周期数 */
	uint8_t latest_valid_status;
}vdi_status_t;

#define VDOF_VOTE_INITED			(0u)
#define VDOF_VOTE_OK				(1u)
#define VDOF_VOTE_SAFETY_CONFILICT	(2u)
#define VDOF_VOTE_HAZARD_CONFILICT	(3u)
#define VDOF_BOARD_ERR          	(4u)

typedef struct _vdof_status
{
	uint8_t status;
	uint8_t safety_conflict_continuous_period; /*- 安全侧不一致连续周期数 */
	uint8_t hazard_conflict_continuous_period; /*- 危险侧不一致连续周期数 */
}vdof_status_t;

#define NDI_INITED    (0u)
#define NDI_0         (1u)
#define NDI_1         (2u)
#define NDI_BOARD_ERR (3u)
typedef struct _ndi_status
{
	uint8_t status;
}ndi_status_t;

#ifdef __cplusplus
}
#endif

#endif   /*_UNIFW_COMMSG_H*/

