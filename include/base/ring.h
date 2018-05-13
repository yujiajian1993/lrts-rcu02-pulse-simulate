#ifndef __RING_H
#define __RING_H

#include "unifw.h"

#ifdef __cplusplus
extern "C"{
#endif

extern const mod_dscrp_t ver_ring;

/**
 * �ɱ䳤���λ���: ����������push��pop����, ÿ��push�洢�����ݿ��Ա䳤�� 
 * ÿ��push���ݴ洢ʱ����дָ��Ŀ�ʼ2���ֽ���Ϊ���ݳ��ȴ洢, �ٴ洢ʵ������
 * popʱ ����readָ�봦��ȡ2�ֽڵĳ���ֵlength���ٶ�ȡ����length�ֽڵ�����
 * pushʱ����writeָ�봦д��2�ֽڵĳ���ֵlength����д��ʵ��push������
 */

/** ���λ������ṹ�� */
typedef struct ring_t_ {
	uint32_t read;  /*-< ����ַ */
	uint32_t write; /*-< д��ַ */
	uint32_t free;  /*-< �����ֽ� */
	uint32_t used;  /*-< ��ʹ���ֽ� */
	uint32_t max;   /*-< ��󻺳����ֽ��� */
	uint16_t unit;	/*-< ÿ��Ԫ�ص�Ĭ�ϴ�С */
	uint8_t *data;  /*-< ������ָ�� */
} ring_t;



/**
 * @brief ring_init ���λ����ʼ������
 *
 * @param ring   ���ζ��л���������ݽṹ��ָ��
 * @param buffer ���л�����ָ��
 * @param max_length   ���л�������С
 */
void ring_init(ring_t *ring, uint8_t buffer[], uint32_t max_length);

/**
 * @brief ring_push ���λ���PUSH����
 *
 * @param ring   ���ζ��л���������ݽṹ��ָ��
 * @param data   push����������ʼ��ַ
 * @param length ���ݳ���
 */
uint16_t ring_push(ring_t *ring, uint8_t data[], uint16_t length);
uint16_t ring_push_new(ring_t *ring, uint8_t data[], uint16_t length);


/**
 * @brief ring_pop 		���λ���POP����
 *
 * @detail 				��������������ݳ��ȴ���Ŀ��洢�����ȣ��Ὣ����Խ����������ʵ�ʳ����Ա�ʾ���ݳ����洢����С
 *
 * @param ring   		���ζ��л���������ݽṹ��ָ�룬��Χ��ָ��ǿ�
 * @param data   		pop����Ŀ��洢��ʼ��ַ����Χ��ָ��ǿ�
 * @param max_length 	Ŀ��洢����󳤶ȣ���Χ����Ȼ��Χ
 *
 * @return 				pop�������ݳ���
 *                      0: ��ʾring�ѿ�
 *                      0xFFFF: ring�е����ݳ�����max-length�������˸�����
 *                      ����: ��ʾpop�����ݳ���
 */
uint16_t ring_pop(ring_t *ring, uint8_t data[], uint16_t max_length);
uint16_t ring_pop_new(ring_t *ring, uint8_t data[], uint16_t length);

/**
 * @brief ring_is_empty ���λ���Ϊ���жϺ���
 *
 * @param ring   ���ζ��л���������ݽṹ��ָ��
 */
uint8_t ring_is_empty(ring_t *ring);


int32_t ring_vote_encode(ring_t *ring, uint8_t to[], uint32_t max_length); /*- ����ring������� */
int32_t ring_sync_encode(ring_t *ring, uint8_t to[], uint32_t max_length); /*- ����ringͬ������ */
int32_t ring_sync_decode(ring_t *ring, uint8_t from[], uint32_t length); /*- ��ͬ����������ring */


/** 
 * @brief rings_init ��ʼ������
 * 
 * @param r rings�ṹ����� 
 * @param c ��������Ԫ�ظ������㣩
 * @param unit Ԫ�ؿ��
 * @param data ����ָ��
 */
void     rings_init    (ring_t *r, uint16_t c, uint16_t unit, void *  data); 

/** 
 * @brief rings_reset ��λ����
 * 
 * @param r rings�ṹ�����
 */
void     rings_reset   (ring_t *r); 

/** 
 * @brief rings_rset ԭʼ����ָ��
 * 
 * @param r �ṹ��ָ��
 * @param o ָ��ƫ����
 * @param e ѹ���Ԫ��ָ��
 */
void     rings_rset(ring_t *r, uint16_t o, void * e);

/**
 * @brief rings_rget ��ȡԭʼ��ָ��
 *
 * @param r �ṹ��ָ��
 * @param o ָ��ƫ����
 *
 * @return ����ԭʼָ��
 */
void*    rings_rget(ring_t *r, uint16_t o);

/** 
 * @brief rings_rpopanull 	����һ�����ݣ������أ�
 * 
 * @param r 				ring�ṹ��
 * @param len 				����������
 *
 * @return 					��
 */
void rings_rpopanull(ring_t *r, uint16_t len);

/** 
 * @brief rings_rpopa 		�����ɱ䳤���ݳ��ȣ������Խ�磩
 * 
 * @param r 				ring�ṹ��
 * @param dst 				Ŀ�ĵ�ַָ��
 * @param len 				��ȡ����
 *
 * @return 					��
 */
void rings_rpopa(ring_t *r, uint8_t *FAR dst, uint16_t len);

/** 
 * @brief rings_rpusha 		ѹ��ɱ䳤���������ݣ������Խ�磩
 * 
 * @param r 				ring�ṹ��
 * @param src 				Դ����ָ��
 * @param len 				��ѹ�볤��
 *
 * @return 					��
 */
void rings_rpusha(ring_t *r, uint8_t *FAR src, uint16_t len);

/** ���廷�λ�����ۻع��� */
#define RINGS_WRAP(val) do{if((val)>=r->max){(val)-=r->max;}}while(0)

/**
 * @brief rings_inc_w ����writeָ��
 * @param r �ṹ��
 */
#define rings_inc_w(r) do{(r)->write++; if ((r)->write>=(r)->max){(r)->write = ((r)->write%(r)->max);}}while(0)

/**
 * @brief rings_inc_r ����readָ��
 * @param r �ṹ��
 */
#define rings_inc_r(r) do{(r)->read++; if ((r)->read>=(r)->max){(r)->read = ((r)->read%(r)->max);}}while(0)

/**
 * @brief rings_rpushp ���жϳ��Ⱥ�����ʱѹ��ָ��
 * @param r �ṹ��
 * @param e Ԫ��
 */
#define rings_rpushp(r, e) do{rings_rset(r, (r)->write, e); rings_inc_w(r);}while(0)

/** 
 * @brief rings_pushp ѹ��ָ��
 * 
 * @param r rings�ṹ��
 * @param e ����ָ��
 * 
 * @return 
 *  - 0 ��ʾ����ѹ��
 *  - -E_FULL��ʾr����
 */
int16_t  rings_pushp   (ring_t *r, void * e);


/** 
 * @brief rings_pop ����ָ��
 * 
 * @param r rings�ṹ��
 * 
 * @return 
 * 	- 0 ��������
 * 	- -E_EMPTY 
 */
int16_t  rings_pop     (ring_t *r);  

/** 
 * @brief rings_getp ��ȡͷ��ָ��
 * 
 * @param r rings�ṹ��
 * @param offset ƫ����
 * 
 * @return ƫ�ƴ�������
 */
void * rings_getp    (ring_t *r, uint16_t offset);  

/**
 * @brief  rings_wgetp ��ȡд���ָ��
 *
 * @param r
 * @param offset
 *
 * @return 
 */
void * rings_wgetp    (ring_t *r, uint16_t offset);  

/** 
 * @brief rings_pusha ѹ���������ݣ����Խ��ȣ�
 * 
 * @param r rings�ṹ��
 * @param src Դ��ַ
 * @param len ��ѹ�����ݳ���
 * 
 * @return 
 * 	- -E_FULL ��ʾ���λ�������
 */
int16_t  rings_pusha   (ring_t *r, void *src, uint16_t len);

/** 
 * @brief rings_popa ��������
 * 
 * @param r rings�ṹ��
 * @param dst Ŀ���ַ
 * @param len �������ĳ���
 * 
 * @return 
 * - -E_OVER_FLOW ��ʾ���㹻���ȣ����
 * - >0 ��ʾʵ�ʵ����ĳ���
 */
int16_t  rings_popa    (ring_t *r, void *dst, uint16_t len);

#define rings_rset_i(r, o, u, type) do{type tmpu=u; rings_rset(r, o, &tmpu);}while(0)

/**
 * @brief rings_rset_u16 ����һ��uint16����
 *
 * @param r �ṹ��
 * @param o ƫ����
 * @param u ��ֵ
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
