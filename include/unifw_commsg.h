#ifndef  _UNIFW_COMMSG_H
#define  _UNIFW_COMMSG_H

#include "unifw.h"

#ifdef __cplusplus
extern "C"{
#endif

#define CAN_DATA_MAX_LEN  (8u)       /*-< CAN������󳤶�      */
#define UDP_DATA_MAX_LEN  (1600)    /*-< UDP������󳤶�      */
#define UART_DATA_MAX_LEN (600u)     /*-< UART������󳤶�    */
#define ACC_NBR (2u)                 /*-< ���ٶȼ�����            */
#define SPD_SENSOR_CHANNEL_NBR (2u)  /*-< �ٶ����崫�������� */

#define ETH_IPv4_MAC_STR_LEN (6*2 + (6-1) + 1)  /*-< MAC��ַ�ֽ��� */
#define ETH_IPv4_IP_STR_LEN  (3*4+4-1+1)
#define ETH_IPv4_IP_NBR      (4u)  /*-< IP��ַ�ֽ��� */

typedef struct _udpmsg
{
	uint8_t      ip_from[ETH_IPv4_IP_NBR]; /*-< ���ͷ���IP��ַ */
	uint16_t                    port_from; /*-< ���ͷ��Ķ˿� */
	uint32_t                    timestamp; /*-< ���ݽ��յı���ʱ�� */
	uint16_t                       length; /*-< ���ݳ��� */
	uint8_t        data[UDP_DATA_MAX_LEN]; /*-< �������� */
} udpmsg_t; /*-< UDP�������ݽṹ */

typedef struct _canmsg{
	uint32_t timestamp;  /*-< ʱ��� */
	uint32_t id;         /*-< CAN����ID */
	uint8_t  ch;         /*-< CANͨ��ͨ�� */
	uint8_t  length;     /*-< CAN���ݳ��� */
	uint8_t  IDE;        /*-< CAN��׼֡/��չ֡��־ */
	uint32_t id_ext;
	uint32_t id_std;
	uint8_t  data[CAN_DATA_MAX_LEN]; /*-< CAN���� */
} canmsg_t;

typedef struct _uartmsg
{
	uint32_t timestamp;  /*-< ���ݽ��յı���ʱ�� */
	uint16_t length;     /*-< ���ݳ���          */
	uint8_t  data[UART_DATA_MAX_LEN];  /*-< ��������          */
} uartmsg_t; /*-< ���ڽ������ݽṹ */

typedef struct _speedmsg
{
	uint16_t ts       ;
    uint8_t  dir      ; /*-< �ٶ�������λ���� */
    uint32_t count    ; /*-< �ۼƼ���       */
    uint8_t  adc_code ; /*-< ���߱�ʶ */
} speedmsg_t; /*-< ƽ̨�������ݽṹ */

typedef struct _accmsg
{
	uint16_t ts;
	uint8_t  acc_id;
	int32_t  ax;  /*-< ����     */
	int32_t  ay;  /*-< ����     */
	int32_t  az;  /*-< ����     */
} accmsg_t     ; /*-< ���ٶ���Ϣ */

/** ʵʱʱ�ӽṹ�� */
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
	uint8_t conflict_continuous_period; /*- ��ͻ������ */
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
	uint8_t safety_conflict_continuous_period; /*- ��ȫ�಻һ������������ */
	uint8_t hazard_conflict_continuous_period; /*- Σ�ղ಻һ������������ */
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

