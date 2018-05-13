/*
*********************************************************************************************************
*********************************************************************************************************
*                                    B2Vģ��ӿ�ͷ�ļ�
*
* b2vģ����ͨ��Э��ת��ģ�飬ͨ�����ñ����ʽ�Ա���Э���������ͽ������߱��Ľ�����ͨ���ԺͿ�������
* 
* b2vģ��֧��bit��ȡ��ֽڿ�ȵ����ñ���ʽ�����ֽ�ģʽ�£�Ҳ֧�ִ�С�˵�ת��
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
 *    B2Vģ�������붨��
**************************/
#define E_B2V_SIZE    0x11010001u
#define E_B2V_WIDTH   0x11010002u
#define E_B2V_ENDIAN  0x11010003u
#define E_B2V_VAR_NUM 0x11010004u

#define B2VM_B2V (0u)
#define B2VM_V2B (1u)

extern const mod_dscrp_t ver_b2v; /**< b2v���ģ��汾�ṹ�� */

#define _SIZE_OFFSET(t,m) (uint32_t)(&((t*)0)->m)								/**< ���type���ͽṹ���Ա����Ŀ���ַ�Ŀ��ƫ���� */
#define _SIZE_OFFSET_ARRAY(t,arr,index,m) (uint32_t)(&(((t*)0)->arr[index].m))	/**< ���type���ͽṹ���ڳ�Ա�ṹ������ĸ���Ա��Ŀ���ַ�Ŀ��ƫ���� */

#define B2V_USIZE_OFFSET(t, m) sizeof(((t*)0)->m), (uint32_t)(&((t*)0)->m)	/**< ����޷���t->m��size��offset */
#define B2V_ISIZE_OFFSET(t, m) -sizeof(((t*)0)->m), (uint32_t)(&((t*)0)->m) /**< ����з���t->m��size��offset */
#define B2V_ASIZE_OFFSET(t, arr, index, m) sizeof(((t*)0)->arr[0].m), (uint32_t)(&(((t*)0)->arr[index].m)) /**< ��ȡ�����޷��� array[index]->m��size��offset */

#define B2V_USIZE_OFFSET_NAME(t, m) sizeof(((t*)0)->m), (uint32_t)(&((t*)0)->m), #m, 0	/**< ����޷���t->m��size��offset */

#define UNIFW_BIG_ENDIAN    (0u) 	/**< �ֽ�����ģʽ  */
#define UNIFW_LITTLE_ENDIAN (1u) 	/**< �ֽ���С��ģʽ  */

#define B2V_RANGEP_RESERVED 0	/**< Ԥ�����ͣ���ʾ�����з��� */
#define B2V_RANGEP_ENUM 1	/**< ö������ */
#define B2V_RANGEP_MINMAX 2	/**< ��Χ���� [��Сֵ�����ֵ] */
#define B2V_RANGEP_NEMINMAX 3	/**< ��Χ��(��Сֵ�����ֵ) */
#define B2V_RANGEP_MIXED 4	/**< ��Χ������[��Сֵ�����ֵ]��Ҳ��ö��ֵ */
#define B2V_RANGEP_EQUAL 5	/**< ��Χ��ָ�������� */

//#define B2V_EN_NAME 0

/**
 * �������ؼ���Ϣ������
 * ����type���ͣ���width�е����ݴ洢����ؽṹ��
 * ��ƫ����, ʹ�����������ķ�ʽ���������ڲ���Ҫ�ı�����Ҳ��Ҫ������Ŀ���ҿ�Ȳ��ܳ���32bit/4�ֽ�
 */
typedef struct _var_item_t
{
	uint8_t  endian   ; /**< �������е��ֽ���: 0��ʾ�����ǰ, 1��ʾС����ǰ  */
	uint8_t  width    ; /**< ��bit���б�ʾ���ݵ�bitλ���, �����ֽڰ汾���������ʾ�ֽ��� */
	uint8_t  size     ; /**< ��Ŀ��ṹ���е����ݿ�� 1/2/4�ֱ�8/16/32λ(�з����޷�����ͳһ��sizeof(type)) */
	uint32_t offset_d ; /**< ������Ŀ���ַ��ƫ���� */
	uint8_t  name[15]   ;	/**< ���� */
	uint8_t  name_tail  ;	/**< ��β */
	uint16_t range_type	;	/**< ���ݷ������� */
	uint16_t enum_len	;	/**< ö�ٱ����ֵ */
	int32_t *enum_table ;	/**< ö�ٱ� */
	int32_t  vmin		;	/**< ��Сֵ */
	int32_t  vmax		;	/**< ���ֵ */
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
 * @brief b2v_var_to_stream ���ؼ�����ת��ΪЭ����Ϣ
 * @detail ���ܸı����������������в��ֵ�����ֵ
 *
 * @param psrc Ŀ������
 * @param pstream ���ݽṹ��
 * @param table ���ݽ�����
 * @param var_num ��Ŀ����,��Χ��[1,255]
 *
 * @return ת���Ƿ�ɹ�
 *  -   0 ת���ɹ�
 *  - ��0 ת��ʧ��
 */
int16_t b2v_var_to_stream_byoffset(uint8_t * psrc, uint8_t * pstream, uint16_t offset, const var_item_t * table, uint8_t var_num, uint32_t *perr);
int16_t b2v_var_to_stream         (void * psrc, uint8_t * pstream, const var_item_t * table, uint8_t var_num, uint32_t *perr);



/** 
 * @brief b2v_stream_set_lsb ����bit��������ָ�����ݿ�ȵı���ֵ(����ָ����С�˽���)
 * @brief b2v_stream_set_msb ����bit��������ָ�����ݿ�ȵı���ֵ(����ָ���ִ�˽���)
 * @brief stream_set ����bit��������ָ�����ݿ�ȵı���ֵ(����ָ���ֽ���)
 * @param paddr Ŀ���ַ
 * @param roffset ƫ����
 * @param value ����ֵ
 * @param width bitλ���
 * @param endian �ֽ���
 */


void b2v_stream_set_lsb(uint8_t *paddr, uint16_t roffset, uint32_t value, uint8_t width, uint32_t *perr); /**< С�˰�  */
void b2v_stream_set_msb(uint8_t *paddr, uint16_t roffset, uint32_t value, uint8_t width, uint32_t *perr); /**< ��˰�  */
void b2v_stream_set    (uint8_t *paddr, uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr);

/** 
 * @brief  b2v_stream_get_lsb ��ȡbit��������ָ�����ݿ�ȵı���ֵ(����ָ����С�˽���)
 * @brief  b2v_stream_get_msb ��ȡbit��������ָ�����ݿ�ȵı���ֵ(����ָ���ִ�˽���)
 * @brief  stream_get ��ȡbit��������ָ�����ݿ�ȵı���ֵ(����ָ���ֽ���)
 * @param paddr Ŀ���ַ
 * @param roffset ƫ����
 * @param width bitλ���
 * @param endian �ֽ���
 */
uint32_t b2v_stream_get_lsb(uint8_t *paddr, uint16_t roffset, uint8_t width, uint32_t *perr); /**< С�˰�  */
uint32_t b2v_stream_get_msb(uint8_t *paddr, uint16_t roffset, uint8_t width, uint32_t *perr); /**< ��˰�  */
uint32_t b2v_stream_get    (uint8_t *paddr, uint16_t roffset, uint8_t width, uint8_t endian, uint32_t *perr);


/** ����Ϊ���ֽڰ汾, ������ȫ���ֽڶ����Э������Ч�� */
/** ���ֽڰ�ͨ�ú��� */
/** 
 * @brief b2v_bytes_set_lsb ����bit��������ָ�����ݿ�ȵı���ֵ(����ָ����С�˽���)
 * @brief b2v_bytes_set_msb ����bit��������ָ�����ݿ�ȵı���ֵ(����ָ���ִ�˽���)
 * @brief b2v_bytes_set ����bit��������ָ�����ݿ�ȵı���ֵ(����ָ���ֽ���)
 * @brief b2v_bytes_get_lsb ��ȡbit��������ָ�����ݿ�ȵı���ֵ(����ָ����С�˽���)
 * @brief b2v_bytes_get_msb ��ȡbit��������ָ�����ݿ�ȵı���ֵ(����ָ���ִ�˽���)
 * @brief b2v_bytes_get ��ȡbit��������ָ�����ݿ�ȵı���ֵ(����ָ���ֽ���)
 * @param paddr Ŀ���ַ
 * @param roffset ƫ����
 * @param value ����ֵ
 * @param width �ֽ���
 * @param endian �ֽ���
 */
void b2v_bytes_set_lsb (uint8_t *paddr, uint16_t roffset, uint32_t value, uint8_t width, uint32_t *perr); /**< С�˰�  */
void b2v_bytes_set_msb (uint8_t *paddr, uint16_t roffset, uint32_t value, uint8_t width, uint32_t *perr); /**< ��˰�  */
void b2v_bytes_set     (uint8_t *paddr, uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr);
uint32_t b2v_bytes_get_lsb(uint8_t *paddr, uint16_t roffset, uint8_t width, uint32_t *perr); /**< С�˰�  */
uint32_t b2v_bytes_get_msb(uint8_t *paddr, uint16_t roffset, uint8_t width, uint32_t *perr); /**< ��˰�  */
uint32_t b2v_bytes_get    (uint8_t *paddr, uint16_t roffset, uint8_t width, uint8_t endian, uint32_t *perr);

/** ���ֽڰ�ӿں���  */
/**
 * @brief b2v_bytes_to_var ���õ���Э����Ϣ�������ؼ����ݽṹ����
 * @brief b2v_var_to_bytes ���ؼ�����ת��Ϊ�ֽ���
 * @param pstream  ������ַ
 * @param pdst  �ṹ���ַ
 * @param table  ����������
 * @param psrc Ŀ������
 * @param pstream ���ݽṹ��
 * @param table ���ݽ�����
 * @param var_num ��Ŀ����,��Χ��[1,255]
 */
int16_t b2v_bytes_to_var(uint8_t *pstream, void * pdst, const var_item_t * table, uint8_t var_num, uint32_t *perr);
int16_t b2v_var_to_bytes(void *psrc, uint8_t * pstream, const var_item_t * table, uint8_t var_num, uint32_t *perr);

int16_t b2v_bytes_to_var_byoffset(uint8_t *pstream, uint16_t offset, uint8_t * pdst, const var_item_t * table, uint8_t var_num, uint32_t *perr);
int16_t b2v_var_to_bytes_byoffset(uint8_t *psrc, uint8_t * pstream, uint16_t offset, const var_item_t * table, uint8_t var_num, uint32_t *perr);


int16_t b2v_table_width(const var_item_t * table, uint8_t var_num, uint32_t *perr); /** ��ȡ���ñ�Ԫ�����ֶ����ݵ��ܳ��� */

uint32_t b2v_var_get_adapt(uint8_t paddr[], uint32_t value, uint8_t size, uint32_t *perr);
/** �ṩIOӳ���ͱ���֮���ת������ʱ��Ҫע�⣬IO��ȫ�Ǵӵ�λ����λ����ʱ����Ҫ���õ�һ��ENDIANѡ��  */
uint32_t b2v_iom_get_adapt(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr);

uint32_t b2v_iom_set(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr);

int16_t b2v_iom_to_var(uint8_t *pstream, void * pdst, const var_item_t * table, uint8_t var_num, uint32_t *perr);

int16_t b2v_var_to_iom(void * psrc, uint8_t *pstream, const var_item_t * table, uint8_t var_num, uint32_t *perr);

#ifdef __cplusplus
}
#endif

#endif   /*_B2V_H */


