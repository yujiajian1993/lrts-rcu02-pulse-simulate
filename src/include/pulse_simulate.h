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

/*- CPU对FPGA指令 */
#define R_DATA       (0X0A)  /*-< 读数据 */ 
#define W_PULSE_CTR  (0X05)  /*-< 写脉冲控制数据 */ 
#define W_IO_CTR     (0X03)  /*-< 写IO控制数据 */ 
#define R_FPGA_VER   (0X0C)  /*-< 读FPGA版本号 */ 
#define R_PULSE_CNT  (0X09)  /*-< 读脉冲计数 */
#define W_HIGH_V     (0X06)  /*-< 输出高电平 */ 
#define W_LOW_V      (0X00)  /*-< 输出低电平 */ 
#define W_LOSE_PULSE (0X50)  /*-< 丢失脉冲使能 */ 
#define W_USB_LED    (0X30)  /*-< 写USB_LED控制指令 */

#define PUL_ENABLE   (0X01<<31)  /*-< 生成脉冲频率字段中使能脉冲输出 */ 
#define PUL_PHASE_DIFF_ENABLE (0X03<<30)  /*-< 脉冲相位差使能 */ 
#define FORWARD  (0X02<<28)  /*-< 车轮正向 */
#define BACKWARD (0X01<<28)  /*-< 车轮反向 */
#define LAST_OUTPUT (0X02<<30)  /*-< 持续输出 */ 
#define FIX_OUTPUT  (0X01<<30)  /*-< 固定输出 */ 


#define  GEN_PULSE   (0x30)  /*-< ATE对RCU02指令 */ 
#define  GET_PULSE   (0X31)  /*-< RCU02对ATE指令 */

typedef struct
{
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
}ver_t;  /*-< 版本号结构体 */  

typedef struct 
{
	uint8_t  src_type ; /**< 源类型 */
	uint8_t  src_id   ; /**< 源ID */
	uint8_t  dst_type ; /**< 目的类型 */
	uint8_t  dst_id   ; /**< 目的ID */
	uint8_t  serial   ; /**< 序号 */
	uint8_t  pkg_num  ; /**< 包数量 */
	uint16_t pkg_len  ; /**< 包长度 */
    uint8_t  cmd   ; /**< 服务命令ID */
	uint8_t  param ; /**< 服务参数 */
	uint16_t len   ; /**< 长度 */
}ssimp_hdr_t ;  /*-< 仿真协议头部 */ 

typedef struct
{
    uint16_t diameter;        /*-< 轮径值 */ 
    uint16_t pul_percycle;    /*-< 车轮每转脉冲数 */ 
    uint16_t ch1_speed;       /*-< 通道1速度 */ 
    uint16_t ch2_speed;       /*-< 通道2速度 */
    uint16_t ch3_speed;       /*-< 通道3速度 */
    uint16_t ch4_speed;       /*-< 通道4速度 */
    uint8_t  ch12_dir;        /*-< 通道1，2方向0XAA正转,0x55反转 */
    uint8_t  ch12_phase_diff; /*-< 通道1，2相位差(1,179) */
    uint8_t  ch34_dir;        /*-< 通道3，4方向0XAA正转,0x55反转 */
    uint8_t  ch34_phase_diff; /*-< 通道3，4相位差(1,179) */
}train_info_t;  /*-< 车辆信息 */ 

#define FPGA_ADDR    (0XFFFF) /*- FPGA地址 */
#define DATA_LENGTH  (0X31)   /*-< 写入数据区长度49 */

typedef struct
{
    uint32_t frame_start;   /*-< 帧起始标志,固定为0xAAAAAA00 */ 
    uint8_t  cmd;           /*-< FPGA读写指令 */ 
    uint16_t length;        /*-< 数据区长度0x31 */ 
    uint16_t addr;          /*-< fpga读写地址0xffff*/ 
}fpga_rw_hdr_t;          /*-< 读写fpga消息头 */ 

typedef struct
{
    uint32_t ch1_duty_cycle;  /*-< 通道1占空比 */
    uint32_t ch2_duty_cycle;  /*-< 通道2占空比 */
    uint32_t ch3_duty_cycle;  /*-< 通道3占空比 */
    uint32_t ch4_duty_cycle;  /*-< 通道4占空比 */ 
    uint32_t ch1_fre;         /*-< 通道1频率，用周期*100表示 */
    uint32_t ch2_fre;         /*-< 通道2频率，用周期*100表示 */
    uint32_t ch3_fre;         /*-< 通道3频率，用周期*100表示 */
    uint32_t ch4_fre;         /*-< 通道4频率，用周期*100表示 */
    uint32_t ch12_phase_diff; /*-< 通道1,2相位差 */ 
    uint32_t ch34_phase_diff; /*-< 通道3,4相位差 */
    uint32_t ch12_pulse_cnt;  /*-< 通道1,2固定生成的脉冲个数，高位使能时有效 */
    uint32_t ch34_pulse_cnt;  /*-< 通道3,4固定生成的脉冲个数，高位使能时有效 */
    uint8_t  do_output;       /*-< 8路io输出 */      
}pul_info_t;  /*-< 写fpga生成脉冲信息 */ 

typedef struct
{
    uint16_t crc16;      /*-< 数据校验和，未使用 */ 
    uint16_t raw_di;     /*-< 读回的DI采集信息 */
    uint16_t frame_end;  /*-< 帧结束标志，固定0x00A2 */ 
}fpga_rw_tail_t;         /*-< 写fpga消息尾部 */

typedef struct
{
    fpga_rw_hdr_t   header;
    pul_info_t      pul_info;
    fpga_rw_tail_t  tail;
}fpga_rw_t;  /*-< 读写fpga数据结构体 */ 

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
