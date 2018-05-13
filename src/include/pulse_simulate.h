#ifndef _PULSE_SIMULATE_H
#define _PULSE_SIMUKATE_H

#include "unifw.h"
#include "base/b2v.h"

#ifdef __cplusplus
extern "C"{
#endif

#define PI 3.1415926

#define UART4_DEVICE     "/dev/ttyO4"

#define _SIZE_OFFSET_ARR(t,arr,m)  (uint32_t)(&(((t*)0)->arr.m))

#define RCU02_PORT  (8703)
#define ATE_PORT    (8704)
#define MAX_UDP_LEN (512)

/*- CPU��FPGAָ�� */
#define R_DATA       (0X0A)  /*-< ������ */ 
#define W_PULSE_CTR  (0X05)  /*-< д����������� */ 
#define W_IO_CTR     (0X03)  /*-< дIO�������� */ 
#define R_FPGA_VER   (0X0C)  /*-< ��FPGA�汾�� */ 
#define R_PULSE_CNT  (0X09)  /*-< ��������� */
#define W_HIGH_V     (0X06)  /*-< ����ߵ�ƽ */ 
#define W_LOW_V      (0X00)  /*-< ����͵�ƽ */ 
#define W_LOSE_PULSE (0X50)  /*-< ��ʧ����ʹ�� */ 
#define W_USB_LED    (0X30)  /*-< дUSB_LED����ָ�� */

#define PUL_ENABLE   (0X01<<31)  /*-< ��������Ƶ���ֶ���ʹ��������� */ 
#define PUL_PHASE_DIFF_ENABLE (0X03<<30)  /*-< ������λ��ʹ�� */ 
#define FORWARD  (0X02<<28)  /*-< �������� */
#define BACKWARD (0X01<<28)  /*-< ���ַ��� */
#define LAST_OUTPUT (0X02<<30)  /*-< ������� */ 
#define FIX_OUTPUT  (0X01<<30)  /*-< �̶���� */ 


#define  GEN_PULSE   (0x30)  /*-< ATE��RCU02ָ�� */ 
#define  GET_PULSE   (0X31)  /*-< RCU02��ATEָ�� */

typedef struct
{
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
}ver_t;  /*-< �汾�Žṹ�� */  

typedef struct 
{
	uint8_t  src_type ; /**< Դ���� */
	uint8_t  src_id   ; /**< ԴID */
	uint8_t  dst_type ; /**< Ŀ������ */
	uint8_t  dst_id   ; /**< Ŀ��ID */
	uint8_t  serial   ; /**< ��� */
	uint8_t  pkg_num  ; /**< ������ */
	uint16_t pkg_len  ; /**< ������ */
    uint8_t  cmd   ; /**< ��������ID */
	uint8_t  param ; /**< ������� */
	uint16_t len   ; /**< ���� */
}ssimp_hdr_t ;  /*-< ����Э��ͷ�� */ 

typedef struct
{
    uint16_t diameter;        /*-< �־�ֵ */ 
    uint16_t pul_percycle;    /*-< ����ÿת������ */ 
    uint16_t ch1_speed;       /*-< ͨ��1�ٶ� */ 
    uint16_t ch2_speed;       /*-< ͨ��2�ٶ� */
    uint16_t ch3_speed;       /*-< ͨ��3�ٶ� */
    uint16_t ch4_speed;       /*-< ͨ��4�ٶ� */
    uint8_t  ch12_dir;        /*-< ͨ��1��2����0XAA��ת,0x55��ת */
    uint8_t  ch12_phase_diff; /*-< ͨ��1��2��λ��(1,179) */
    uint8_t  ch34_dir;        /*-< ͨ��3��4����0XAA��ת,0x55��ת */
    uint8_t  ch34_phase_diff; /*-< ͨ��3��4��λ��(1,179) */
}train_info_t;  /*-< ������Ϣ */ 

#define FPGA_ADDR    (0XFFFF) /*- FPGA��ַ */
#define DATA_LENGTH  (0X31)   /*-< д������������49 */

typedef struct
{
    uint32_t frame_start;   /*-< ֡��ʼ��־,�̶�Ϊ0xAAAAAA00 */ 
    uint8_t  cmd;           /*-< FPGA��дָ�� */ 
    uint16_t length;        /*-< ����������0x31 */ 
    uint16_t addr;          /*-< fpga��д��ַ0xffff*/ 
}fpga_rw_hdr_t;          /*-< ��дfpga��Ϣͷ */ 

typedef struct
{
    uint32_t ch1_duty_cycle;  /*-< ͨ��1ռ�ձ� */
    uint32_t ch2_duty_cycle;  /*-< ͨ��2ռ�ձ� */
    uint32_t ch3_duty_cycle;  /*-< ͨ��3ռ�ձ� */
    uint32_t ch4_duty_cycle;  /*-< ͨ��4ռ�ձ� */ 
    uint32_t ch1_fre;         /*-< ͨ��1Ƶ�ʣ�������*100��ʾ */
    uint32_t ch2_fre;         /*-< ͨ��2Ƶ�ʣ�������*100��ʾ */
    uint32_t ch3_fre;         /*-< ͨ��3Ƶ�ʣ�������*100��ʾ */
    uint32_t ch4_fre;         /*-< ͨ��4Ƶ�ʣ�������*100��ʾ */
    uint32_t ch12_phase_diff; /*-< ͨ��1,2��λ�� */ 
    uint32_t ch34_phase_diff; /*-< ͨ��3,4��λ�� */
    uint32_t ch12_pulse_cnt;  /*-< ͨ��1,2�̶����ɵ������������λʹ��ʱ��Ч */
    uint32_t ch34_pulse_cnt;  /*-< ͨ��3,4�̶����ɵ������������λʹ��ʱ��Ч */
    uint8_t  do_output;       /*-< 8·io��� */      
}pul_info_t;  /*-< дfpga����������Ϣ */ 

typedef struct
{
    uint16_t crc16;      /*-< ����У��ͣ�δʹ�� */ 
    uint16_t raw_di;     /*-< ���ص�DI�ɼ���Ϣ */
    uint16_t frame_end;  /*-< ֡������־���̶�0x00A2 */ 
}fpga_rw_tail_t;         /*-< дfpga��Ϣβ�� */

typedef struct
{
    fpga_rw_hdr_t   header;
    pul_info_t      pul_info;
    fpga_rw_tail_t  tail;
}fpga_rw_t;  /*-< ��дfpga���ݽṹ�� */ 

#define SSIMP_HDR_TBL_LEN (10)
#define SSIMP_HDR_LEN     (12)
extern const var_item_t ssimp_hdr_tbl[SSIMP_HDR_TBL_LEN];

#define TRAIN_INFO_TBL_LEN (10)
#define TRAIN_INFO_LEN (16)
extern const var_item_t train_info_tbl[TRAIN_INFO_TBL_LEN];

#define FPGA_RW_TBL_LEN (20)
#define FPGA_RW_LEN (64)
extern const var_item_t fpga_rw_tbl[FPGA_RW_TBL_LEN];

#ifdef __cplusplus
}
#endif

#endif  /*-< _PULSE_SIMUKATE_H */ 
