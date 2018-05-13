#include "unifw.h"
#include "base/codebase.h"



/*- ţ�ٵ������������Ƹ���� */
const uint32_t s_msb_num[16] = {
	1<<0u, 1<<2u, 1<<4u, 1<<6u, 1<<8u, 1<<10u, 1<<12u, 1<<14u,
	1<<16u, 1<<18u, 1<<20u, 1<<22u, 1<<24u, 1<<26u, 1<<28u, 1<<30u,
}; 


/**
 * @brief cb_vLshift32  �������ƴ�����
 *
 * @param var ԭʼ���ݣ���Χ����Ȼ��Χ
 * @param r   ����λbit������Χ����Ȼ��Χ
 *
 * @return  var����rλ�Ľ��ֵ
 */
uint32_t cb_vLshift32(uint32_t var, uint8_t r)
{
	/*- ������ʼ�� */
	uint32_t ret = 0u;

	/*- �ж���λ����Ƿ�����Ч��Χ�� */
	if (r<=31)
	{
		/*- ָ�������������� */
		ret = var<<r;
	}else{
		/*- ������Ч��λ��Χ, ֵΪ0 */
		ret = 0u;
	}

	/*- ���ؼ����� */
	return (ret);
}


/**
 * @brief cb_vRshift32  �������ƴ�����
 *
 * @param var ԭʼ���ݣ���Χ����Ȼ��Χ
 * @param r   ����λbit������Χ����Ȼ��Χ
 *
 * @return  var����rλ�Ľ��ֵ
 */
uint32_t cb_vRshift32(uint32_t var, uint8_t r)
{
	/*- ������ʼ�� */
	uint32_t ret = 0u;

	/*- �ж���λ����Ƿ�����Ч��Χ�� */
	if (r<=31)
	{
		/*- ָ���������������� */
		ret = var>>r;
	}else{
		/*- ������Ч��λ��Χ, ֵΪ0 */
		ret = 0u;
	}

	/*- ���ؼ����� */
	return (ret);
}


/**
 * @brief  vstb_bsearch ��������
 *
 * @detail ����һ�����������飬��������ֵ�����������������С����������λ��
 *
 * @param value  ����ֵ����Χ����Ȼ��Χ
 * @param tbl    �������������飬��Χ��ָ��ǿ�
 * @param blen    ���鳤�ȣ���Χ����Ȼ��Χ
 *
 * @return ����ֵС��������С�� - 0;
 *         ����ֵ������������� - len-1;
 *         ����ֵ������������   - �������� 
 *         ����ֵ�������������� - �����������С����������lo;
 *     
 */
static uint16_t vstb_bsearch(uint32_t value, uint32_t const tbl[], uint16_t blen)
{
    /*- ������ʼ�� */
	uint16_t tmp = 0u, lo = 0u, ret = 0u, len = blen;	
    uint8_t flag = 0u;

    /*- ����ֵ�Ƿ�С���������Сֵ */
    if (value <= tbl[0])
    {
        /*- ����0  */
        ret = 0u;
    }
    /*-  ����ֵ�Ƿ���������е����ֵ */
    else if (value >= tbl[len-1u])
    {
        /*- ����len-1  */
        ret = len-1u;
    }
    else
    {
        /*- ������ʼ�������������鳤��ʱ���ж�������  */
        do
        {
            tmp = (lo+len)/2u;      /*-< ������������Ԫ�ص����� */
            /*- valueֵ�Ƿ��������Ԫ�� */
            if (value == tbl[tmp]) 
            {
                /*- ��������Ԫ������ */
                ret = tmp;
                flag = 1u;
                break;	        
            }
            /*- valueֵ�Ƿ�С������Ԫ�� */
            else if (value < tbl[tmp]) 
            {
                len = tmp;	      /*-< ������Χȡ����ǰ�� */
            }
            else
            {
                lo = tmp;         /*-< ������Χȡ������ */
            }	
        }
        while ((lo+1u) < len);
        /*- @alias ��¼���ҵ�������ֵ */
        if (0u == flag)    
        {
            ret = lo;
        }
        else
        {
            ; /*-< do nothing */
        }
    }
 
    /*- ���ؽ�� */
	return (ret);
}

/** 
 * @brief cb_sqrt ����ƽ����
 *
 * @detail ʹ��ţ�ٵ����������п�������
 *          ����ʹ��λ�����õ�һ�����Ƹ���
 *          Ȼ��ʹ��X[n+1] = (X[n]+a/X[n])/2�ķ�ʽ�����
 *          ��������Ϊ<56755*56755(������0���),�Ѿ�������֤
 *          ���㷨�õ��Ľ����ʵ�ʽ�����Ϊ1
 * 
 * @param src ������������Χ����Ȼ��Χ
 *
 * @return �������
 */

uint16_t cb_sqrt(uint32_t src)
{
    /*- ������ʼ�� */
    uint16_t y=0u, z=0u, ret=0u;

    /*- ��������λ��ȡ��ʼ���Ƹ� */
    z = vstb_bsearch(src, s_msb_num, sizeof(s_msb_num)/sizeof(s_msb_num[0]));
	y = (uint16_t)cb_vLshift32((uint32_t)1,(uint32_t)z);

    /*- �����������С������ɿ�����Χ */
    if (z < SQRT_TBL_MAX_BITS)
    {
        /*- @alias ����log2(z)�μ��㿪��ֵ */
        do
        {
            y = (uint16_t)(((uint32_t)y+(uint32_t)(src/y))/2u);
            z = z/2u;
        }
        while(z != 0u);
    }
    else
    {
        ; /*-< nothing */
    }

    /*- �õ�������� */
    ret = y;

    /*- ���ؽ�� */
    return (ret);
}

/**
 * @brief       cb_ptr_set ��ָ��ָ���ַ��ֵָ����ȵ�MSBֵ
 *
 * @detail      ����ָ����ȣ������ݷ�װΪһ���ֽڴ�������ָ���ַ��ʼ���θ�ֵ��Ŀ���ַ
 *
 * @param src   Ŀ�ĵ�ַ����Χ��ָ��ǿ�
 * @param width ���ݿ�ȣ���Χ��{1, 2, 4, -1, -2, -4}
 * @param data  MSB����װ���ݣ���Χ����Ȼ��Χ
 *
 * @return  ��
 */
void cb_ptr_set(uint8_t src[], int16_t width, int32_t data)
{
	/*- ָ��srcΪ����ֱ��崻� */
	UNIFW_ASSERT(NULL != src);

    /*- ����MSB�Ŀ�Ȱ�ֵ����ָ����ַ*/
    switch (width)
    {
        /*- ���Ϊ1*/
        case (1):       
            /*- ��ֵ*/        
            src[0] = (uint8_t)data & 0xFFu;                  
            break;
        /*- ���Ϊ2*/            
        case (2):
            /*- ��ֵ*/
            src[0] = ((uint32_t)data>>8U) & 0xFFu;  
            src[1] = ((uint32_t)data>>0U)&0xFFu;       
            break;
        /*- ���Ϊ4*/
        case (4):       
            /*- ��ֵ*/                    
            src[0] = ((uint32_t)data>>24U)&0xFFu;   
            src[1] = ((uint32_t)data>>16U)&0xFFu;
            src[2] = ((uint32_t)data>> 8U)&0xFFu;
            src[3] = ((uint32_t)data>> 0U)&0xFFu;      
            break;
        /*- ���Ϊ-1*/
        case (-1):
            /*- ��ֵ*/                    
            src[0] = (uint8_t)data & 0xFFu;
            break;
        /*- ���Ϊ-2*/
        case (-2):
            /*- ��ֵ*/                    
            src[0] = ((uint32_t)data>>8U)&0xFFu;
            src[1] = ((uint32_t)data>>0U)&0xFFu;
            break;
        /*- ���Ϊ-4*/
        case (-4):
            /*- ��ֵ*/                    
            src[0] = ((uint32_t)data>>24U)&0xFFu;
            src[1] = ((uint32_t)data>>16U)&0xFFu;
            src[2] = ((uint32_t)data>> 8U)&0xFFu;
            src[3] = ((uint32_t)data>> 0U)&0xFFu;
            break;
        /*- ���Ϊ����ֵ*/
        default:
             /*- ���ֵ��Ч*/ 
            ;                              
            break;     
    }

}

/**
 * @brief cb_ptr_set_index ��ָ����ַ��ֵָ����ȵ�MSBֵ
 *
 * @detail      ��ƫ������ʾָ����ʼָ���ַ��
 *              ����ָ����ȣ������ݷ�װΪһ���ֽڴ�������ָ���ַ��ʼ���θ�ֵ��Ŀ���ַ
 *
 * @param src   Ŀ�ĵ�ַ����Χ��ָ��ǿ�
 * @param width ���ݿ�ȣ���Χ����Ȼ��Χ
 * @param data  MSBֵ����Χ����Ȼ��Χ
 * @param index ����ƫ�ƣ���Χ����Ȼ��Χ
 *
 * @return ��
 */
void cb_ptr_set_index(uint8_t src[], int16_t width, int32_t data, uint16_t index)
{
	/*- ָ��srcΪ����ֱ��崻� */
	UNIFW_ASSERT(NULL != src);

    /*- ��ָ��ƫ�����ĵ�ַ����ָ����ȵ����� */
    cb_ptr_set(&src[index], width, data);

}


/**
 * @brief cb_ptr_get    ��ָ���ַ����ȡָ����ȵ�MSBֵ
 * @detail      ��һ���ֽڴ�����ָ���������Ϊ����ֵ
 *
 * @param src   ����Դ��ַ����Χ��ָ��ǿ�
 * @param width ���ݿ�ȣ���Χ��{1��2��4��-1��-2��-4}
 *
 * @return ��������������ֵ
 */
int32_t cb_ptr_get(uint8_t src[], int16_t width)
{
    /*- ������ʼ�� */
    uint32_t val = 0u;

	/*- ָ��srcΪ����ֱ��崻� */
	UNIFW_ASSERT(NULL != src);

    /*- ����ָ���Ŀ�Ȱ�ֵ����ָ����ַ*/
    switch (width)
    {
        /*- ���Ϊ1*/
        case (1):
            /*- ��ֵ*/                    
            val = (uint32_t)src[0];
            break;
        /*- ���Ϊ2*/
        case (2):
            /*- ��ֵ*/                    
            val = ((uint32_t)src[0]<<8u) + (uint32_t)src[1];
            break;
        /*- ���Ϊ4*/
        case (4):
            /*- ��ֵ*/                    
            val = ((uint32_t)src[0]<<24u) + ((uint32_t)src[1]<<16u) + ((uint32_t)src[2]<<8u) + (uint32_t)src[3];
            break;
        /*- ���Ϊ-1*/
        case (-1):
            /*- ��ֵ*/                    
            val = (uint32_t)src[0];
            break;
        /*- ���Ϊ-2*/
        case (-2):
            /*- ��ֵ*/                    
            val = ((uint32_t)src[0]<<8u) + (uint32_t)src[1];
            break;
        /*- ���Ϊ-4*/
        case (-4):
            /*- ��ֵ*/                    
            val = ((uint32_t)src[0]<<24u) + ((uint32_t)src[1]<<16u) + ((uint32_t)src[2]<<8u) + (uint32_t)src[3];
            break;
        /*- ���Ϊ����ֵ*/
        default: 
            /*- ���ֵ��Ч*/                                
            ;
            break;      
    }

    /*- �������������ֵ */
	return ((int32_t)(val)); 

}

/**
 * @brief cb_ptr_get_index ���ڴ��ַ��ȡָ����ȵ�����
 * @detail      ����ƫ������ָ�뿪ʼ��ַ��ȡһ���ֽڴ�
 *              ����ָ���������Ϊ����ֵ
 *
 * @param src   ����Դ��ַ����Χ��ָ��ǿ�
 * @param width ���ݿ�ȣ���Χ����Ȼ��Χ
 * @param index ƫ�Ƶ�ַ����Χ����Ȼ��Χ
 *
 * @return      �������������ֵ
 */
int32_t cb_ptr_get_index(uint8_t src[], int16_t width, uint16_t index)
{
    /*- ������ʼ�� */
    int32_t data = 0;

	/*- ָ��srcΪ����ֱ��崻� */
	UNIFW_ASSERT(NULL != src);

    /*- ��ָ��ƫ�����ĵ�ַ��ȡָ����ȵ����� */
    data = cb_ptr_get(&src[index], width);
    
    /*- �������������ֵ */
    return (data);
}

/**
 * @brief      usr_memcpy �ڴ�������
 * @detail     ��ָ���ڴ��ַ��ָ����С�ڴ濽����Ŀ�ĵ�ַ
 * @param to   Ŀ���ڴ��ַ����Χ��ָ��ǿ�
 * @param from Դ�ڴ��ַ����Χ��ָ��ǿ�
 * @param size �ڴ������С����Χ����Ȼ��Χ
 * @return  ��
 */
void usr_memcpy(void *to, const void *from, uint32_t size)
{
    /*- ������ʼ�� */
	uint32_t i =0u;
	uint8_t  *p_to, *p_from = NULL;
    uint32_t v32_to = (uint32_t)to, v32_from = (uint32_t)from;
    uint32_t *p32_to;
    uint32_t *p32_from;
    uint16_t *p16_to;
    uint16_t *p16_from;
    uint32_t tail, alignCNT;

	/*- ָ��to��fromΪ����ֱ��崻� */
	UNIFW_ASSERT(NULL != to);
	UNIFW_ASSERT(NULL != from);

    p_to = to;
    p_from = (uint8_t *)from;

    if ((v32_to&0x3u) == 0 && (v32_from&0x3u) == 0)
    {
        alignCNT = size>>2;
        tail     = alignCNT<<2;

        p32_to   = (uint32_t *)to  ;
        p32_from = (uint32_t *)from;
        for (i=0; i<alignCNT; i++)
        {
            p32_to[i] = p32_from[i];
        }

        for (i=tail; i<size; i++) 
        {
            /*- �ڴ����ݿ��� */
            p_to[i] = p_from[i];
        }
    }
    else if ((v32_to&0x1u) == 0 && (v32_from&0x1u) == 0)
    {
        alignCNT = size>>1;
        tail     = alignCNT<<1;

        p16_to   = (uint16_t *)to  ;
        p16_from = (uint16_t *)from;
        for (i=0; i<alignCNT; i++)
        {
            p16_to[i] = p16_from[i];
        }

        for (i=tail; i<size; i++) 
        {
            /*- �ڴ����ݿ��� */
            p_to[i] = p_from[i];
        }
    }
    else
    {
        /*- i=0;iС���ڴ�����Сʱ;��i��1 */
        for (i=0u; i<size; i++) 
        {
            /*- �ڴ����ݿ��� */
            p_to[i] = p_from[i];
        }
    }
}

/**
 * @brief      usr_memsafecpy �ڴ�����ȫ����
 * @detail     ��ָ���ڴ��ַ��ָ����С�ڴ濽����Ŀ�ĵ�ַ
 * @param to   Ŀ���ڴ��ַ����Χ��ָ��ǿ�
 * @param from Դ�ڴ��ַ����Χ��ָ��ǿ�
 * @param size �ڴ������С����Χ����Ȼ��Χ
 * @return  ��
 */
void usr_memsafecpy(void *to, const void *from, uint32_t size)
{
    /*- ������ʼ�� */
	uint32_t i =0u;
	uint8_t  *p_to, *p_from = NULL;
    uint32_t v32_to = (uint32_t)to, v32_from = (uint32_t)from;
    uint32_t *p32_to;
    uint32_t *p32_from;
    uint16_t *p16_to;
    uint16_t *p16_from;
    uint32_t tail, alignCNT;

	/*- ָ��to��fromΪ����ֱ��崻� */
	UNIFW_ASSERT(NULL != to);
	UNIFW_ASSERT(NULL != from);

    p_to = to;
    p_from = (uint8_t *)from;

    if ((v32_to&0x3u) == 0 && (v32_from&0x3u) == 0)
    {
        alignCNT = size>>2;
        tail     = alignCNT<<2;

        p32_to   = (uint32_t *)to  ;
        p32_from = (uint32_t *)from;
        for (i=0; i<alignCNT; i++)
        {
            p32_to[i]   = p32_from[i];
            p32_from[i] = 0;
        }

        for (i=tail; i<size; i++) 
        {
            /*- �ڴ����ݿ��� */
            p_to[i]   = p_from[i];
            p_from[i] = 0;
        }
    }
    else if ((v32_to&0x1u) == 0 && (v32_from&0x1u) == 0)
    {
        alignCNT = size>>1;
        tail     = alignCNT<<1;

        p16_to   = (uint16_t *)to  ;
        p16_from = (uint16_t *)from;
        for (i=0; i<alignCNT; i++)
        {
            p16_to[i] = p16_from[i];
            p16_from[i] = 0;
        }

        for (i=tail; i<size; i++) 
        {
            /*- �ڴ����ݿ��� */
            p_to[i] = p_from[i];
            p_from[i] = 0;
        }
    }
    else
    {
        /*- i=0;iС���ڴ�����Сʱ;��i��1 */
        for (i=0u; i<size; i++) 
        {
            /*- �ڴ����ݿ��� */
            p_to[i] = p_from[i];
            p_from[i] = 0;
        }
    }
}


/**
 * @brief      usr_memset �ڴ�����ʼ����ֵ
 *
 * @detail     ��ָ���ڴ��ַ��ָ����С�ڴ水��uint8_t��ֵ
 *
 * @param to   �ڴ��ַ����Χ��ָ��ǿ�
 * @param val  ��ʼ����ֵ����Χ����Ȼ��Χ
 * @param size �ڴ������С����Χ����Ȼ��Χ
 *
 * @return     ��
 */
void usr_memset(void *to, uint8_t val, uint32_t size)
{
    /*- ������ʼ�� */
    uint32_t i = 0u;
	uint8_t  *p_to = NULL;
    uint32_t vaddr = (uint32_t)to;
    uint32_t head, u32len, tail, val32;
    uint32_t *p_to32 = NULL;

	/*- ָ��toΪ����ֱ��崻� */
	UNIFW_ASSERT(NULL != to);

    p_to = to;
#if 0
    /*- i=0;iС���ڴ�����Сʱ;��i��1 */
    for (i=0u; i<size; i++) 
    {
        /*- ��ָ���ڴ��ַ��ָ����С������ָ��ֵ��ʼ�� */
        p_to[i] = val;
    }
#endif
    // ��������Сʱ��ֱ�Ӹ�ֵ
    if (size < 10) 
    {
        for (i=0u; i<size; i++) 
        {
            /*- ��ָ���ڴ��ַ��ָ����С������ָ��ֵ��ʼ�� */
            p_to[i] = val;
        }
    }else
    {
        head = vaddr&0x3;

        if (head == 0)
        {
            p_to32 = (uint32_t *)vaddr;
            u32len = size>>2;
            tail = u32len<<2;
        }else{
            for (i=0u; i<(4 - head); i++) 
            {
                /*- ��ָ���ڴ��ַ��ָ����С������ָ��ֵ��ʼ�� */
                p_to[i] = val;
            }
            p_to32   = (uint32_t *)(vaddr + 4 - head);
            u32len   = (size - head)>>2;
            tail     = head + (u32len<<2);
        }

        for (i=tail; i<size; i++)
        {
            /*- ��ָ���ڴ��ַ��ָ����С������ָ��ֵ��ʼ�� */
            p_to[i] = val;
        }

        val32 = (val<<24) + (val<<16) + (val<<8) + val;

        for(i=0; i<u32len; i++)
        {
            p_to32[i] = val32;
        }
    }
}

/**
 * @brief       min_u8 �Ƚ�����uint8_t����
 *
 * @detail      ���������еĽ�Сֵ
 *
 * @param a     ��1����Χ����Ȼ��Χ
 * @param b     ��2����Χ����Ȼ��Χ
 *
 * @return      �����еĽ�Сֵ
 */
uint8_t min_u8(uint8_t a, uint8_t b)
{
    /*- ������ʼ�� */
    uint8_t ret = 0u;

    /*- @alias �Ƚ����� */
    if (a <= b)
	{
        ret = a;
	}
    else
	{
        ret = b;
	}

    /*- ���رȽϽ�� */
    return (ret);
}

/**
 * @brief       min_u16 �Ƚ�����uint16_t����
 *
 * @detail      ���������еĽ�Сֵ
 *
 * @param a     ��1����Χ����Ȼ��Χ
 * @param b     ��2����Χ����Ȼ��Χ
 *
 * @return      �����еĽ�Сֵ
 */
uint16_t min_u16(uint16_t a, uint16_t b)
{
    /*- ������ʼ�� */
    uint16_t ret = 0u;
    
    /*- @alias �Ƚ����� */
    if (a <= b)
    {
		ret = a;
	}
    else
	{
        ret = b;
	}

    /*- ���رȽϽ�� */
    return (ret);
}


/**
 * @brief       min_u32 �Ƚ�����uint32_t����
 *
 * @detail      ���������еĽ�Сֵ
 *
 * @param a     ��1����Χ����Ȼ��Χ
 * @param b     ��2����Χ����Ȼ��Χ
 *
 * @return      �����еĽ�Сֵ
 */
uint32_t min_u32(uint32_t a, uint32_t b)
{
    /*- ������ʼ�� */
    uint32_t ret = 0u;
    
    /*- @alias �Ƚ����� */
    if (a <= b)
	{
		ret = a;
	}
    else
	{
        ret = b;
	}

    /*- ���رȽϽ�� */
    return (ret);
}


/**
 * @brief       min_i8 �Ƚ�����int8_t����
 *
 * @detail      ���������еĽ�Сֵ
 *
 * @param a     ��1����Χ����Ȼ��Χ
 * @param b     ��2����Χ����Ȼ��Χ
 *
 * @return      �����еĽ�Сֵ
 */
int8_t min_i8(int8_t a, int8_t b)
{
    /*- ������ʼ�� */
    int8_t ret = 0;

    /*- @alias �Ƚ����� */
    if (a <= b)
	{
        ret = a;
	}
    else
	{
        ret = b;
	}

    /*- ���رȽϽ�� */
    return (ret);
}


/**
 * @brief       min_i16 �Ƚ�����int16_t����
 *
 * @detail      ���������еĽ�Сֵ
 *
 * @param a     ��1����Χ����Ȼ��Χ
 * @param b     ��2����Χ����Ȼ��Χ
 *
 * @return      �����еĽ�Сֵ
 */
int16_t min_i16(int16_t a, int16_t b)
{
    /*- ������ʼ�� */
    int16_t ret = 0;
    
    /*- @alias �Ƚ����� */
    if (a <= b)
	{
        ret = a;
	}
    else
	{
        ret = b;
	}

    /*- ���رȽϽ�� */
    return (ret);
}

/**
 * @brief       min_i32 �Ƚ�����int32_t����
 *
 * @detail      ���������еĽ�Сֵ
 *
 * @param a     ��1����Χ����Ȼ��Χ
 * @param b     ��2����Χ����Ȼ��Χ
 *
 * @return      �����еĽ�Сֵ
 */
int32_t min_i32(int32_t a, int32_t b)
{
    /*- ������ʼ�� */
    int32_t ret = 0;
    
    /*- @alias �Ƚ����� */
    if (a <= b)
	{
        ret = a;
	}
    else
	{
        ret = b;
	}

    /*- ���رȽϽ�� */
    return (ret);
}

/**
 * @brief       max_u8 �Ƚ�����uint8_t����
 *
 * @detail      ���������еĽϴ�ֵ
 *
 * @param a     ��1����Χ����Ȼ��Χ
 * @param b     ��2����Χ����Ȼ��Χ
 *
 * @return      �����еĽϴ�ֵ
 */
uint8_t max_u8(uint8_t a, uint8_t b)
{
    /*- ������ʼ�� */
    uint8_t ret = 0u;

    /*- @alias �Ƚ����� */
    if (a >= b)
	{
        ret = a;
	}
    else
	{
        ret = b;
	}

    /*- ���رȽϽ�� */
    return (ret);
}


/**
 * @brief       max_u16 �Ƚ�����uint16_t����
 *
 * @detail      ���������еĽϴ�ֵ
 *
 * @param a     ��1����Χ����Ȼ��Χ
 * @param b     ��2����Χ����Ȼ��Χ
 *
 * @return      �����еĽϴ�ֵ
 */
uint16_t max_u16(uint16_t a, uint16_t b)
{
    /*- ������ʼ�� */
    uint16_t ret = 0u;
    
    /*- @alias �Ƚ����� */
    if (a >= b)
	{
        ret = a;
	}
    else
	{
        ret = b;
	}

    /*- ���رȽϽ�� */
    return (ret);
}


/**
 * @brief       max_u32 �Ƚ�����uint32_t����
 *
 * @detail      ���������еĽϴ�ֵ
 *
 * @param a     ��1����Χ����Ȼ��Χ
 * @param b     ��2����Χ����Ȼ��Χ
 *
 * @return      �����еĽϴ�ֵ
 */
uint32_t max_u32(uint32_t a, uint32_t b)
{
    /*- ������ʼ�� */
    uint32_t ret = 0u;
    
    /*- @alias �Ƚ����� */
    if (a >= b)
	{
        ret = a;
	}
    else
	{
        ret = b;
	}

    /*- ���رȽϽ�� */
    return (ret);
}


/**
 * @brief       max_i8 �Ƚ�����int8_t����
 *
 * @detail      ���������еĽϴ�ֵ
 *
 * @param a     ��1����Χ����Ȼ��Χ
 * @param b     ��2����Χ����Ȼ��Χ
 *
 * @return      �����еĽϴ�ֵ
 */
int8_t max_i8(int8_t a, int8_t b)
{
    /*- ������ʼ�� */
    int8_t ret = 0;

    /*- @alias �Ƚ����� */
    if (a >= b)
	{
        ret = a;
	}
    else
	{
        ret = b;
	}

    /*- ���رȽϽ�� */
    return (ret);
}

/**
 * @brief       max_i16 �Ƚ�����int16_t����
 *
 * @detail      ���������еĽϴ�ֵ
 *
 * @param a     ��1����Χ����Ȼ��Χ
 * @param b     ��2����Χ����Ȼ��Χ
 *
 * @return      �����еĽϴ�ֵ
 */
int16_t max_i16(int16_t a, int16_t b)
{
    /*- ������ʼ�� */
    int16_t ret = 0;
    
    /*- @alias �Ƚ����� */
    if (a >= b)
	{
        ret = a;
	}
    else
	{
        ret = b;
	}

    /*- ���رȽϽ�� */
    return (ret);
}


/**
 * @brief       max_i32 �Ƚ�����int32_t����
 *
 * @detail      ���������еĽϴ�ֵ
 *
 * @param a     ��1����Χ����Ȼ��Χ
 * @param b     ��2����Χ����Ȼ��Χ
 *
 * @return      �����еĽϴ�ֵ
 */
int32_t max_i32(int32_t a, int32_t b)
{
    /*- ������ʼ�� */
    int32_t ret = 0;
    
    /*- @alias �Ƚ����� */
    if (a >= b)
	{
        ret = a;
	}
    else
	{
        ret = b;
	}

    /*- ���رȽϽ�� */
    return (ret);
}


/**
 * @brief       abs_value_i8 �������ֵ
 *
 * @detail      ����һ��int8_t���ľ���ֵ
 *
 * @param a     ��Σ���Χ����Ȼ��Χ
 *
 * @return      ��εľ���ֵ
 */
uint8_t abs_value_i8(int8_t a)
{
    /*- ������ʼ�� */
    uint8_t ret = 0u;

    /*- @alias �������ֵ */
    if (a >= 0)
	{
        ret = a;
	}
    else 
	{
        ret = 0 - a;
	}

    /*- ���ؽ�� */
    return (ret);
}

/**
 * @brief       abs_value_i16 �������ֵ
 *
 * @detail      ����һ��int16_t���ľ���ֵ
 *
 * @param a     ��Σ���Χ����Ȼ��Χ
 *
 * @return      ��εľ���ֵ
 */
uint16_t abs_value_i16(int16_t a)
{
    /*- ������ʼ�� */
    uint16_t ret = 0u;

    /*- @alias �������ֵ */
    if (a >= 0)
	{
        ret = a;
	}
    else 
	{
        ret = 0 - a;
	}

    /*- ���ؽ�� */
    return (ret);
}


/**
 * @brief       abs_value_i32 �������ֵ
 *
 * @detail      ����һ��int32_t���ľ���ֵ
 *
 * @param a     ��Σ���Χ����Ȼ��Χ
 *
 * @return      ��εľ���ֵ
 */
uint32_t abs_value_i32(int32_t a)
{
    /*- ������ʼ�� */
    uint32_t ret = 0u;

    /*- @alias �������ֵ */
    if (a >= 0)
	{
        ret = a;
	}
    else 
	{
        ret = 0 - a;
	}

    /*- ���ؽ�� */
    return (ret);
}


/**
 * @brief       abs_value_u8 �������ֵ
 *
 * @detail      ����һ��uint8_t���ľ���ֵ
 *
 * @param a     ��Σ���Χ����Ȼ��Χ
 *
 * @return      ��εľ���ֵ
 */
uint8_t abs_value_u8(uint8_t a)
{
    /*- ���ؽ�� */
    return (a);
}

/**
 * @brief       abs_value_u16 �������ֵ
 *
 * @detail      ����һ��uint16_t���ľ���ֵ
 *
 * @param a     ��Σ���Χ����Ȼ��Χ
 *
 * @return      ��εľ���ֵ
 */
uint16_t abs_value_u16(uint16_t a)
{
    /*- ���ؽ�� */
    return (a);
}

/**
 * @brief       abs_value_u32 �������ֵ
 *
 * @detail      ����һ��uint32_t���ľ���ֵ
 *
 * @param a     ��Σ���Χ����Ȼ��Χ
 *
 * @return      ��εľ���ֵ
 */
uint32_t abs_value_u32(uint32_t a)
{
    /*- ���ؽ�� */
    return (a);
}

/**
 * @brief       diff_value_u8 ���������Ĳ�ֵ
 *
 * @detail      ����uint8_t���Ĳ�ֵ
 *
 * @param a     ��a����Χ����Ȼ��Χ
 * @param b     ��b����Χ����Ȼ��Χ
 *
 * @return      ����֮��
 */
uint8_t diff_value_u8(uint8_t a, uint8_t b)
{
    /*- ���ؽ�� */
    return (max_u8(a,b) - min_u8(a,b));
}

/**
 * @brief       diff_value_u16 ���������Ĳ�ֵ
 *
 * @detail      ����uint16_t���Ĳ�ֵ
 *
 * @param a     ��a����Χ����Ȼ��Χ
 * @param b     ��b����Χ����Ȼ��Χ
 *
 * @return      ����֮��
 */
uint16_t diff_value_u16(uint16_t a, uint16_t b)
{
    /*- ���ؽ�� */
    return (max_u16(a,b) - min_u16(a,b));
}

/**
 * @brief       diff_value_u32 ���������Ĳ�ֵ
 *
 * @detail      ����uint32_t���Ĳ�ֵ
 *
 * @param a     ��a����Χ����Ȼ��Χ
 * @param b     ��b����Χ����Ȼ��Χ
 *
 * @return      ����֮��
 */
uint32_t diff_value_u32(uint32_t a, uint32_t b)
{
    /*- ���ؽ�� */
    return (max_u32(a,b) - min_u32(a,b));
}

/**
 * @brief       diff_value_i8 ���������Ĳ�ֵ
 *
 * @detail      ����int8_t���Ĳ�ֵ
 *
 * @param a     ��a����Χ����Ȼ��Χ
 * @param b     ��b����Χ����Ȼ��Χ
 *
 * @return      ����֮��
 */
int8_t diff_value_i8(int8_t a, int8_t b)
{
    /*- ���ؽ�� */
    return (max_i8(a,b) - min_i8(a,b));
}


/**
 * @brief       diff_value_i16 ���������Ĳ�ֵ
 *
 * @detail      ����int16_t���Ĳ�ֵ
 *
 * @param a     ��a����Χ����Ȼ��Χ
 * @param b     ��b����Χ����Ȼ��Χ
 *
 * @return      ����֮��
 */
int16_t diff_value_i16(int16_t a, int16_t b)
{
    /*- ���ؽ�� */
    return (max_i16(a,b) - min_i16(a,b));
}

/**
 * @brief       diff_value_i32 ���������Ĳ�ֵ
 *
 * @detail      ����int32_t���Ĳ�ֵ
 *
 * @param a     ��a����Χ����Ȼ��Χ
 * @param b     ��b����Χ����Ȼ��Χ
 *
 * @return      ����֮��
 */
int32_t diff_value_i32(int32_t a, int32_t b)
{
    /*- ���ؽ�� */
    return (max_i32(a,b) - min_i32(a,b));
}

/**
 * @brief       cb_atoi �ַ���ת����
 *
 * @param str   ��ת�����ַ���
 * @param perr  �������
 *
 * @return      ת�����
 */
int32_t cb_atoi(char * str, int8_t *perr)
{
    /*- ������ʼ�� */
    int32_t value = 0;
    int32_t sign  = 1;
    int32_t radix = 0;
    uint8_t loop = 0;

    /*- ��ָ���� */
    if (str == NULL) 
    {
        *perr = E_NULL_PT;
    }
    else{
        /*- �ж���λ�Ƿ�Ϊ'-' */
        if (*str == '-')
        {
            /*- ����λΪ�� */
            sign = -1;
            str++;
        }else{
            /*- ����λΪ�� */
            sign = 1;
        }
        /*- �ж��Ƿ�16���� */
        if (*str == '0' && (*(str+1) == 'x' || *(str+1) == 'X'))
        {
            radix = 16; /*- ����Ϊ16���� */
            str += 2;
        }
        else if (*str == '0') /*-< �Ƿ�8���� */
        {
            radix = 8; /*- ����Ϊ8���� */
            str++;
        }
        else
        {
            radix = 10; /*- ����Ϊ10���� */
        }
        
        /*- ���ý������Ϊ���� */
        *perr = E_OK;
        /*- �����ַ��� */
        while (*str && loop < 32)
        {
            /*- ��������+1 */
            loop++;
            if (radix == 16)  /*-< Ϊ16���� */
            {
                if(*str >= '0' && *str <= '9')
                {
                    value = value * radix + *str - '0';    //0��9���ַ���
                }    
                else if(*str >= 'A' && *str <= 'F')
                {
                    value = value * radix + *str - 'A' + 10;   //��дʮ�����Ƶ�ABCDEF���ַ��������
                }    
                else if(*str >= 'a' && *str <= 'f')
                {
                    value = value * radix + *str - 'a' + 10;  // Сдʮ�����Ƶ�abcdef���ַ��������
                }    
                else
                {
                    *perr = E_FORMAT; /*-< ��ʽ���� */
                }
            }
            else if (radix == 8)  /*-< Ϊ8���� */
            {
                if(*str >= '0' && *str <= '7')
                {
                    value = value * radix + *str - '0';
                }else{
                    *perr = E_FORMAT; /*-< ��ʽ���� */
                }
            }
            else if (radix == 10)  /*-< Ϊ10���� */
            {
                if(*str >= '0' && *str <= '9')
                {
                    value = value * radix + *str - '0';
                }else{
                    *perr = E_FORMAT; /*-< ��ʽ���� */
                }
            }
            str++;
        }
    }

    /*- �����ɹ� */
    if (*perr == E_OK)
    {
        /*- δ����ѭ�� */
        if (loop == 0){
            value = 0;
            if (radix == 8) {
                ;
            }
            else{
                *perr = E_FORMAT;
            }
        }else if (loop >= 32) /*-< ����̫��δ�������� */
        {
            value = 0;
            *perr = E_FORMAT;
        }else{
            ;
        }
    }else{
        value = 0;
    }

    /*- ���ؽ�� */
    return (sign*value);
}
