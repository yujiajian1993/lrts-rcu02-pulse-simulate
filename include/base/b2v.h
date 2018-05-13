/*
*********************************************************************************************************
*********************************************************************************************************
*                                    B2V模块接口头文件
*
* b2v模块是通用协议转换模块，通过配置表的形式对报文协议进行组包和解包，提高报文解析的通用性和可配置性
* 
* b2v模块支持bit宽度、字节宽度的配置表形式，在字节模式下，也支持大、小端的转换
*
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  _B2V_H
#define  _B2V_H

#ifdef __cplusplus
extern "C"{
#endif

/**************************
 *    B2V模块错误代码定义
**************************/
#define E_B2V_SIZE    0x11010001u
#define E_B2V_WIDTH   0x11010002u
#define E_B2V_ENDIAN  0x11010003u
#define E_B2V_VAR_NUM 0x11010004u

#define B2VM_B2V (0u)
#define B2VM_V2B (1u)

extern const mod_dscrp_t ver_b2v; /**< b2v软件模块版本结构体 */

#define _SIZE_OFFSET(t,m) (uint32_t)(&((t*)0)->m)								/**< 获得type类型结构体成员的在目标地址的宽度偏移量 */
#define _SIZE_OFFSET_ARRAY(t,arr,index,m) (uint32_t)(&(((t*)0)->arr[index].m))	/**< 获得type类型结构体内成员结构体数组的各成员在目标地址的宽度偏移量 */

#define B2V_USIZE_OFFSET(t, m) sizeof(((t*)0)->m), (uint32_t)(&((t*)0)->m)	/**< 获得无符号t->m的size和offset */
#define B2V_ISIZE_OFFSET(t, m) -sizeof(((t*)0)->m), (uint32_t)(&((t*)0)->m) /**< 获得有符号t->m的size和offset */
#define B2V_ASIZE_OFFSET(t, arr, index, m) sizeof(((t*)0)->arr[0].m), (uint32_t)(&(((t*)0)->arr[index].m)) /**< 获取数组无符号 array[index]->m的size和offset */

#define B2V_USIZE_OFFSET_NAME(t, m) sizeof(((t*)0)->m), (uint32_t)(&((t*)0)->m), #m, 0	/**< 获得无符号t->m的size和offset */

#define UNIFW_BIG_ENDIAN    (0u) 	/**< 字节序大端模式  */
#define UNIFW_LITTLE_ENDIAN (1u) 	/**< 字节序小端模式  */

#define B2V_RANGEP_RESERVED 0	/**< 预留类型，表示不进行防护 */
#define B2V_RANGEP_ENUM 1	/**< 枚举类型 */
#define B2V_RANGEP_MINMAX 2	/**< 范围，从 [最小值，最大值] */
#define B2V_RANGEP_NEMINMAX 3	/**< 范围，(最小值，最大值) */
#define B2V_RANGEP_MIXED 4	/**< 范围，既有[最小值，最大值]，也有枚举值 */
#define B2V_RANGEP_EQUAL 5	/**< 范围，指定的类型 */

//#define B2V_EN_NAME 0

/**
 * 待解析关键信息描述表
 * 根据type类型，将width中的数据存储到相关结构体
 * 无偏移量, 使用增量解析的方式，即：对于不需要的变量，也需要描述条目，且宽度不能超过32bit/4字节
 */
typedef struct _var_item_t
{
	uint8_t  endian   ; /**< 数据流中的字节序: 0表示大端在前, 1表示小端在前  */
	uint8_t  width    ; /**< 在bit域中表示数据的bit位宽度, 在整字节版本数据中则表示字节数 */
	uint8_t  size     ; /**< 在目标结构体中的数据宽度 1/2/4分别8/16/32位(有符号无符号用统一的sizeof(type)) */
	uint32_t offset_d ; /**< 数据在目标地址的偏移量 */
	uint8_t  name[15]   ;	/**< 名字 */
	uint8_t  name_tail  ;	/**< 结尾 */
	uint16_t range_type	;	/**< 数据防护类型 */
	uint16_t enum_len	;	/**< 枚举表的数值 */
	int32_t *enum_table ;	/**< 枚举表 */
	int32_t  vmin		;	/**< 最小值 */
	int32_t  vmax		;	/**< 最大值 */
} var_item_t        	;

int16_t b2v_check_enum(int32_t *tab, uint16_t len, int32_t value);
int16_t b2v_check_v(const var_item_t *vitem, int32_t value);
int16_t b2v_check_bytes(uint8_t pstream[], const var_item_t * table, uint8_t var_num);
int16_t b2v_dump(void *data, const var_item_t * table, uint8_t var_num);

void     b2v_var_set(void *paddr, uint32_t value, uint8_t  size, uint32_t *perr);
uint32_t b2v_var_get(uint8_t *paddr, uint8_t  size , uint32_t *perr);

int16_t b2v_stream_to_var_byoffset(uint8_t *pstream, uint16_t offset, uint8_t * pdst, const var_item_t * table, uint8_t var_num, uint32_t *perr);
int16_t b2v_stream_to_var         (uint8_t *pstream, void * pdst, const var_item_t * table, uint8_t var_num, uint32_t *perr);

/**
 * @brief b2v_var_to_stream 将关键数据转换为协议信息
 * @detail 不能改变数据流数组中已有部分的数据值
 *
 * @param psrc 目标数组
 * @param pstream 数据结构体
 * @param table 数据解析表
 * @param var_num 条目数量,范围：[1,255]
 *
 * @return 转换是否成功
 *  -   0 转换成功
 *  - 非0 转换失败
 */
int16_t b2v_var_to_stream_byoffset(uint8_t * psrc, uint8_t * pstream, uint16_t offset, const var_item_t * table, uint8_t var_num, uint32_t *perr);
int16_t b2v_var_to_stream         (void * psrc, uint8_t * pstream, const var_item_t * table, uint8_t var_num, uint32_t *perr);



/** 
 * @brief b2v_stream_set_lsb 设置bit数据流中指定数据宽度的变量值(按照指定字小端节序)
 * @brief b2v_stream_set_msb 设置bit数据流中指定数据宽度的变量值(按照指定字大端节序)
 * @brief stream_set 设置bit数据流中指定数据宽度的变量值(按照指定字节序)
 * @param paddr 目标地址
 * @param roffset 偏移量
 * @param value 变量值
 * @param width bit位宽度
 * @param endian 字节序
 */


void b2v_stream_set_lsb(uint8_t *paddr, uint16_t roffset, uint32_t value, uint8_t width, uint32_t *perr); /**< 小端版  */
void b2v_stream_set_msb(uint8_t *paddr, uint16_t roffset, uint32_t value, uint8_t width, uint32_t *perr); /**< 大端版  */
void b2v_stream_set    (uint8_t *paddr, uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr);

/** 
 * @brief  b2v_stream_get_lsb 读取bit数据流中指定数据宽度的变量值(按照指定字小端节序)
 * @brief  b2v_stream_get_msb 读取bit数据流中指定数据宽度的变量值(按照指定字大端节序)
 * @brief  stream_get 读取bit数据流中指定数据宽度的变量值(按照指定字节序)
 * @param paddr 目标地址
 * @param roffset 偏移量
 * @param width bit位宽度
 * @param endian 字节序
 */
uint32_t b2v_stream_get_lsb(uint8_t *paddr, uint16_t roffset, uint8_t width, uint32_t *perr); /**< 小端版  */
uint32_t b2v_stream_get_msb(uint8_t *paddr, uint16_t roffset, uint8_t width, uint32_t *perr); /**< 大端版  */
uint32_t b2v_stream_get    (uint8_t *paddr, uint16_t roffset, uint8_t width, uint8_t endian, uint32_t *perr);


/** 以下为整字节版本, 对于完全整字节对齐的协议可提高效率 */
/** 整字节版通用函数 */
/** 
 * @brief b2v_bytes_set_lsb 设置bit数据流中指定数据宽度的变量值(按照指定字小端节序)
 * @brief b2v_bytes_set_msb 设置bit数据流中指定数据宽度的变量值(按照指定字大端节序)
 * @brief b2v_bytes_set 设置bit数据流中指定数据宽度的变量值(按照指定字节序)
 * @brief b2v_bytes_get_lsb 读取bit数据流中指定数据宽度的变量值(按照指定字小端节序)
 * @brief b2v_bytes_get_msb 读取bit数据流中指定数据宽度的变量值(按照指定字大端节序)
 * @brief b2v_bytes_get 读取bit数据流中指定数据宽度的变量值(按照指定字节序)
 * @param paddr 目标地址
 * @param roffset 偏移量
 * @param value 变量值
 * @param width 字节数
 * @param endian 字节序
 */
void b2v_bytes_set_lsb (uint8_t *paddr, uint16_t roffset, uint32_t value, uint8_t width, uint32_t *perr); /**< 小端版  */
void b2v_bytes_set_msb (uint8_t *paddr, uint16_t roffset, uint32_t value, uint8_t width, uint32_t *perr); /**< 大端版  */
void b2v_bytes_set     (uint8_t *paddr, uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr);
uint32_t b2v_bytes_get_lsb(uint8_t *paddr, uint16_t roffset, uint8_t width, uint32_t *perr); /**< 小端版  */
uint32_t b2v_bytes_get_msb(uint8_t *paddr, uint16_t roffset, uint8_t width, uint32_t *perr); /**< 大端版  */
uint32_t b2v_bytes_get    (uint8_t *paddr, uint16_t roffset, uint8_t width, uint8_t endian, uint32_t *perr);

/** 整字节版接口函数  */
/**
 * @brief b2v_bytes_to_var 将得到的协议信息解析到关键数据结构体中
 * @brief b2v_var_to_bytes 将关键数据转换为字节数
 * @param pstream  变量地址
 * @param pdst  结构体地址
 * @param table  数据描述表
 * @param psrc 目标数组
 * @param pstream 数据结构体
 * @param table 数据解析表
 * @param var_num 条目数量,范围：[1,255]
 */
int16_t b2v_bytes_to_var(uint8_t *pstream, void * pdst, const var_item_t * table, uint8_t var_num, uint32_t *perr);
int16_t b2v_var_to_bytes(void *psrc, uint8_t * pstream, const var_item_t * table, uint8_t var_num, uint32_t *perr);

int16_t b2v_bytes_to_var_byoffset(uint8_t *pstream, uint16_t offset, uint8_t * pdst, const var_item_t * table, uint8_t var_num, uint32_t *perr);
int16_t b2v_var_to_bytes_byoffset(uint8_t *psrc, uint8_t * pstream, uint16_t offset, const var_item_t * table, uint8_t var_num, uint32_t *perr);


int16_t b2v_table_width(const var_item_t * table, uint8_t var_num, uint32_t *perr); /** 获取配置表元素中字段数据的总长度 */

uint32_t b2v_var_get_adapt(uint8_t paddr[], uint32_t value, uint8_t size, uint32_t *perr);
/** 提供IO映射表和变量之间的转换，此时需要注意，IO完全是从低位到高位，此时不需要配置第一个ENDIAN选项  */
uint32_t b2v_iom_get_adapt(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr);

uint32_t b2v_iom_set(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr);

int16_t b2v_iom_to_var(uint8_t *pstream, void * pdst, const var_item_t * table, uint8_t var_num, uint32_t *perr);

int16_t b2v_var_to_iom(void * psrc, uint8_t *pstream, const var_item_t * table, uint8_t var_num, uint32_t *perr);

#ifdef __cplusplus
}
#endif

#endif   /*_B2V_H */


