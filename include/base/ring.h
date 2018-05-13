#ifndef __RING_H
#define __RING_H

#include "unifw.h"

#ifdef __cplusplus
extern "C"{
#endif

extern const mod_dscrp_t ver_ring;

/**
 * 可变长环形缓冲: 包含基本的push和pop操作, 每次push存储的数据可以变长度 
 * 每次push数据存储时，将写指针的开始2个字节作为数据长度存储, 再存储实际数据
 * pop时 ：从read指针处读取2字节的长度值length，再读取后续length字节的数据
 * push时：从write指针处写入2字节的长度值length，再写入实际push的数据
 */

/** 环形缓冲管理结构体 */
typedef struct ring_t_ {
	uint32_t read;  /*-< 读地址 */
	uint32_t write; /*-< 写地址 */
	uint32_t free;  /*-< 空闲字节 */
	uint32_t used;  /*-< 已使用字节 */
	uint32_t max;   /*-< 最大缓冲区字节数 */
	uint16_t unit;	/*-< 每个元素的默认大小 */
	uint8_t *data;  /*-< 缓冲区指针 */
} ring_t;



/**
 * @brief ring_init 环形缓冲初始化函数
 *
 * @param ring   环形队列缓冲管理数据结构体指针
 * @param buffer 队列缓冲区指针
 * @param max_length   队列缓冲区大小
 */
void ring_init(ring_t *ring, uint8_t buffer[], uint32_t max_length);

/**
 * @brief ring_push 环形缓冲PUSH函数
 *
 * @param ring   环形队列缓冲管理数据结构体指针
 * @param data   push进入数据起始地址
 * @param length 数据长度
 */
uint16_t ring_push(ring_t *ring, uint8_t data[], uint16_t length);
uint16_t ring_push_new(ring_t *ring, uint8_t data[], uint16_t length);


/**
 * @brief ring_pop 		环形缓冲POP函数
 *
 * @detail 				如果缓冲区内数据长度大于目标存储区长度，会将数据越过，并返回实际长度以表示数据超出存储区大小
 *
 * @param ring   		环形队列缓冲管理数据结构体指针，范围：指针非空
 * @param data   		pop数据目标存储起始地址，范围：指针非空
 * @param max_length 	目标存储区最大长度，范围：自然范围
 *
 * @return 				pop出的数据长度
 *                      0: 表示ring已空
 *                      0xFFFF: ring中的数据超过了max-length，跳过了该数据
 *                      其它: 表示pop的数据长度
 */
uint16_t ring_pop(ring_t *ring, uint8_t data[], uint16_t max_length);
uint16_t ring_pop_new(ring_t *ring, uint8_t data[], uint16_t length);

/**
 * @brief ring_is_empty 环形缓冲为空判断函数
 *
 * @param ring   环形队列缓冲管理数据结构体指针
 */
uint8_t ring_is_empty(ring_t *ring);


int32_t ring_vote_encode(ring_t *ring, uint8_t to[], uint32_t max_length); /*- 生成ring表决数据 */
int32_t ring_sync_encode(ring_t *ring, uint8_t to[], uint32_t max_length); /*- 生成ring同步数据 */
int32_t ring_sync_decode(ring_t *ring, uint8_t from[], uint32_t length); /*- 从同步数据生成ring */


/** 
 * @brief rings_init 初始化函数
 * 
 * @param r rings结构体变量 
 * @param c 容量（以元素个数计算）
 * @param unit 元素宽度
 * @param data 数组指针
 */
void     rings_init    (ring_t *r, uint16_t c, uint16_t unit, void *  data); 

/** 
 * @brief rings_reset 复位函数
 * 
 * @param r rings结构体变量
 */
void     rings_reset   (ring_t *r); 

/** 
 * @brief rings_rset 原始设置指针
 * 
 * @param r 结构体指针
 * @param o 指针偏移量
 * @param e 压入的元素指针
 */
void     rings_rset(ring_t *r, uint16_t o, void * e);

/**
 * @brief rings_rget 获取原始的指针
 *
 * @param r 结构体指针
 * @param o 指针偏移量
 *
 * @return 返回原始指针
 */
void*    rings_rget(ring_t *r, uint16_t o);

/** 
 * @brief rings_rpopanull 	丢弃一段数据（不返回）
 * 
 * @param r 				ring结构体
 * @param len 				待丢弃长度
 *
 * @return 					无
 */
void rings_rpopanull(ring_t *r, uint16_t len);

/** 
 * @brief rings_rpopa 		弹出可变长数据长度（不检查越界）
 * 
 * @param r 				ring结构体
 * @param dst 				目的地址指针
 * @param len 				读取长度
 *
 * @return 					无
 */
void rings_rpopa(ring_t *r, uint8_t *FAR dst, uint16_t len);

/** 
 * @brief rings_rpusha 		压入可变长度数组数据（不检查越界）
 * 
 * @param r 				ring结构体
 * @param src 				源数据指针
 * @param len 				待压入长度
 *
 * @return 					无
 */
void rings_rpusha(ring_t *r, uint8_t *FAR src, uint16_t len);

/** 定义环形缓冲的折回功能 */
#define RINGS_WRAP(val) do{if((val)>=r->max){(val)-=r->max;}}while(0)

/**
 * @brief rings_inc_w 增加write指针
 * @param r 结构体
 */
#define rings_inc_w(r) do{(r)->write++; if ((r)->write>=(r)->max){(r)->write = ((r)->write%(r)->max);}}while(0)

/**
 * @brief rings_inc_r 增加read指针
 * @param r 结构体
 */
#define rings_inc_r(r) do{(r)->read++; if ((r)->read>=(r)->max){(r)->read = ((r)->read%(r)->max);}}while(0)

/**
 * @brief rings_rpushp 不判断长度和容量时压入指针
 * @param r 结构体
 * @param e 元素
 */
#define rings_rpushp(r, e) do{rings_rset(r, (r)->write, e); rings_inc_w(r);}while(0)

/** 
 * @brief rings_pushp 压入指针
 * 
 * @param r rings结构体
 * @param e 数据指针
 * 
 * @return 
 *  - 0 表示正常压入
 *  - -E_FULL表示r已满
 */
int16_t  rings_pushp   (ring_t *r, void * e);


/** 
 * @brief rings_pop 弹出指针
 * 
 * @param r rings结构体
 * 
 * @return 
 * 	- 0 正常弹出
 * 	- -E_EMPTY 
 */
int16_t  rings_pop     (ring_t *r);  

/** 
 * @brief rings_getp 获取头部指针
 * 
 * @param r rings结构体
 * @param offset 偏移量
 * 
 * @return 偏移处的数据
 */
void * rings_getp    (ring_t *r, uint16_t offset);  

/**
 * @brief  rings_wgetp 获取写入的指针
 *
 * @param r
 * @param offset
 *
 * @return 
 */
void * rings_wgetp    (ring_t *r, uint16_t offset);  

/** 
 * @brief rings_pusha 压入数组数据（检查越界等）
 * 
 * @param r rings结构体
 * @param src 源地址
 * @param len 待压入数据长度
 * 
 * @return 
 * 	- -E_FULL 表示环形缓冲已满
 */
int16_t  rings_pusha   (ring_t *r, void *src, uint16_t len);

/** 
 * @brief rings_popa 弹出数组
 * 
 * @param r rings结构体
 * @param dst 目标地址
 * @param len 待弹出的长度
 * 
 * @return 
 * - -E_OVER_FLOW 表示无足够长度，溢出
 * - >0 表示实际弹出的长度
 */
int16_t  rings_popa    (ring_t *r, void *dst, uint16_t len);

#define rings_rset_i(r, o, u, type) do{type tmpu=u; rings_rset(r, o, &tmpu);}while(0)

/**
 * @brief rings_rset_u16 设置一个uint16数据
 *
 * @param r 结构体
 * @param o 偏移量
 * @param u 数值
 */
#define rings_rset_u16(r, o, u) rings_rset_i(r, o, u, uint16_t)

#define rings_push_i(r, u, type) do{type tmpu=u; rings_pushp(r, &tmpu);}while(0)

#define rings_push_u16(r, e) rings_push_i(r, e, uint16_t)
#define rings_push_i16(r, e) rings_push_i(r, e, int16_t)

#define rings_get_i(r, type) *((type*)rings_getp(r, 0))

#define rings_get_u16(r) rings_get_i(r, uint16_t)

#ifdef __cplusplus
}
#endif

#endif // __RING_H
