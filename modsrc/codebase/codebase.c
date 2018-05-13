#include "unifw.h"
#include "base/codebase.h"



/*- 牛顿迭代法开方近似根查表 */
const uint32_t s_msb_num[16] = {
	1<<0u, 1<<2u, 1<<4u, 1<<6u, 1<<8u, 1<<10u, 1<<12u, 1<<14u,
	1<<16u, 1<<18u, 1<<20u, 1<<22u, 1<<24u, 1<<26u, 1<<28u, 1<<30u,
}; 


/**
 * @brief cb_vLshift32  数据左移处理函数
 *
 * @param var 原始数据，范围：自然范围
 * @param r   左移位bit数，范围：自然范围
 *
 * @return  var左移r位的结果值
 */
uint32_t cb_vLshift32(uint32_t var, uint8_t r)
{
	/*- 变量初始化 */
	uint32_t ret = 0u;

	/*- 判断移位宽度是否在有效范围内 */
	if (r<=31)
	{
		/*- 指向数据左移运算 */
		ret = var<<r;
	}else{
		/*- 超出有效移位范围, 值为0 */
		ret = 0u;
	}

	/*- 返回计算结果 */
	return (ret);
}


/**
 * @brief cb_vRshift32  数据右移处理函数
 *
 * @param var 原始数据，范围：自然范围
 * @param r   右移位bit数，范围：自然范围
 *
 * @return  var右移r位的结果值
 */
uint32_t cb_vRshift32(uint32_t var, uint8_t r)
{
	/*- 变量初始化 */
	uint32_t ret = 0u;

	/*- 判断移位宽度是否在有效范围内 */
	if (r<=31)
	{
		/*- 指向数据左右移运算 */
		ret = var>>r;
	}else{
		/*- 超出有效移位范围, 值为0 */
		ret = 0u;
	}

	/*- 返回计算结果 */
	return (ret);
}


/**
 * @brief  vstb_bsearch 二分搜索
 *
 * @detail 搜索一个已排序数组，返回搜索值在数组中最相近的最小整数的索引位置
 *
 * @param value  搜索值，范围：自然范围
 * @param tbl    搜索已排序数组，范围：指针非空
 * @param blen    数组长度，范围：自然范围
 *
 * @return 搜索值小于数组最小数 - 0;
 *         搜索值大于数组最大数 - len-1;
 *         搜索值存在于数组中   - 返回索引 
 *         搜索值不存在于数组中 - 返回最相近最小整数的索引lo;
 *     
 */
static uint16_t vstb_bsearch(uint32_t value, uint32_t const tbl[], uint16_t blen)
{
    /*- 变量初始化 */
	uint16_t tmp = 0u, lo = 0u, ret = 0u, len = blen;	
    uint8_t flag = 0u;

    /*- 搜索值是否小于数组的最小值 */
    if (value <= tbl[0])
    {
        /*- 返回0  */
        ret = 0u;
    }
    /*-  搜索值是否大于数组中的最大值 */
    else if (value >= tbl[len-1u])
    {
        /*- 返回len-1  */
        ret = len-1u;
    }
    else
    {
        /*- 搜索起始索引不超过数组长度时进行二分搜索  */
        do
        {
            tmp = (lo+len)/2u;      /*-< 计算数组中轴元素的索引 */
            /*- value值是否等于中轴元素 */
            if (value == tbl[tmp]) 
            {
                /*- 返回中轴元素索引 */
                ret = tmp;
                flag = 1u;
                break;	        
            }
            /*- value值是否小于中轴元素 */
            else if (value < tbl[tmp]) 
            {
                len = tmp;	      /*-< 搜索范围取数组前半 */
            }
            else
            {
                lo = tmp;         /*-< 搜索范围取数组后半 */
            }	
        }
        while ((lo+1u) < len);
        /*- @alias 记录查找到的索引值 */
        if (0u == flag)    
        {
            ret = lo;
        }
        else
        {
            ; /*-< do nothing */
        }
    }
 
    /*- 返回结果 */
	return (ret);
}

/** 
 * @brief cb_sqrt 快速平方根
 *
 * @detail 使用牛顿迭代法来进行开方运算
 *          首先使用位数来得到一个近似根，
 *          然后使用X[n+1] = (X[n]+a/X[n])/2的方式来完成
 *          计算上限为<56755*56755(否则会除0溢出),已经经过验证
 *          本算法得到的结果和实际结果误差为1
 * 
 * @param src 被开方数，范围：自然范围
 *
 * @return 开方结果
 */

uint16_t cb_sqrt(uint32_t src)
{
    /*- 变量初始化 */
    uint16_t y=0u, z=0u, ret=0u;

    /*- 按操作半位数取初始近似根 */
    z = vstb_bsearch(src, s_msb_num, sizeof(s_msb_num)/sizeof(s_msb_num[0]));
	y = (uint16_t)cb_vLshift32((uint32_t)1,(uint32_t)z);

    /*- 如果被开方数小于溢出可开方范围 */
    if (z < SQRT_TBL_MAX_BITS)
    {
        /*- @alias 迭代log2(z)次计算开方值 */
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

    /*- 得到开方结果 */
    ret = y;

    /*- 返回结果 */
    return (ret);
}

/**
 * @brief       cb_ptr_set 在指定指针地址赋值指定宽度的MSB值
 *
 * @detail      根据指定宽度，将数据封装为一组字节串，并从指针地址开始依次赋值到目标地址
 *
 * @param src   目的地址，范围：指针非空
 * @param width 数据宽度，范围：{1, 2, 4, -1, -2, -4}
 * @param data  MSB待封装数据，范围：自然范围
 *
 * @return  无
 */
void cb_ptr_set(uint8_t src[], int16_t width, int32_t data)
{
	/*- 指针src为空则直接宕机 */
	UNIFW_ASSERT(NULL != src);

    /*- 根据MSB的宽度把值赋给指定地址*/
    switch (width)
    {
        /*- 宽度为1*/
        case (1):       
            /*- 赋值*/        
            src[0] = (uint8_t)data & 0xFFu;                  
            break;
        /*- 宽度为2*/            
        case (2):
            /*- 赋值*/
            src[0] = ((uint32_t)data>>8U) & 0xFFu;  
            src[1] = ((uint32_t)data>>0U)&0xFFu;       
            break;
        /*- 宽度为4*/
        case (4):       
            /*- 赋值*/                    
            src[0] = ((uint32_t)data>>24U)&0xFFu;   
            src[1] = ((uint32_t)data>>16U)&0xFFu;
            src[2] = ((uint32_t)data>> 8U)&0xFFu;
            src[3] = ((uint32_t)data>> 0U)&0xFFu;      
            break;
        /*- 宽度为-1*/
        case (-1):
            /*- 赋值*/                    
            src[0] = (uint8_t)data & 0xFFu;
            break;
        /*- 宽度为-2*/
        case (-2):
            /*- 赋值*/                    
            src[0] = ((uint32_t)data>>8U)&0xFFu;
            src[1] = ((uint32_t)data>>0U)&0xFFu;
            break;
        /*- 宽度为-4*/
        case (-4):
            /*- 赋值*/                    
            src[0] = ((uint32_t)data>>24U)&0xFFu;
            src[1] = ((uint32_t)data>>16U)&0xFFu;
            src[2] = ((uint32_t)data>> 8U)&0xFFu;
            src[3] = ((uint32_t)data>> 0U)&0xFFu;
            break;
        /*- 宽度为其他值*/
        default:
             /*- 宽度值无效*/ 
            ;                              
            break;     
    }

}

/**
 * @brief cb_ptr_set_index 在指定地址赋值指定宽度的MSB值
 *
 * @detail      以偏移量表示指定开始指针地址，
 *              根据指定宽度，将数据封装为一组字节串，并从指针地址开始依次赋值到目标地址
 *
 * @param src   目的地址，范围：指针非空
 * @param width 数据宽度，范围：自然范围
 * @param data  MSB值，范围：自然范围
 * @param index 数据偏移，范围：自然范围
 *
 * @return 无
 */
void cb_ptr_set_index(uint8_t src[], int16_t width, int32_t data, uint16_t index)
{
	/*- 指针src为空则直接宕机 */
	UNIFW_ASSERT(NULL != src);

    /*- 在指定偏移量的地址设置指定宽度的数据 */
    cb_ptr_set(&src[index], width, data);

}


/**
 * @brief cb_ptr_get    从指针地址出获取指定宽度的MSB值
 * @detail      将一组字节串根据指定宽度重组为整数值
 *
 * @param src   数据源地址，范围：指针非空
 * @param width 数据宽度，范围：{1，2，4，-1，-2，-4}
 *
 * @return 返回重组后的整数值
 */
int32_t cb_ptr_get(uint8_t src[], int16_t width)
{
    /*- 变量初始化 */
    uint32_t val = 0u;

	/*- 指针src为空则直接宕机 */
	UNIFW_ASSERT(NULL != src);

    /*- 根据指定的宽度把值赋给指定地址*/
    switch (width)
    {
        /*- 宽度为1*/
        case (1):
            /*- 赋值*/                    
            val = (uint32_t)src[0];
            break;
        /*- 宽度为2*/
        case (2):
            /*- 赋值*/                    
            val = ((uint32_t)src[0]<<8u) + (uint32_t)src[1];
            break;
        /*- 宽度为4*/
        case (4):
            /*- 赋值*/                    
            val = ((uint32_t)src[0]<<24u) + ((uint32_t)src[1]<<16u) + ((uint32_t)src[2]<<8u) + (uint32_t)src[3];
            break;
        /*- 宽度为-1*/
        case (-1):
            /*- 赋值*/                    
            val = (uint32_t)src[0];
            break;
        /*- 宽度为-2*/
        case (-2):
            /*- 赋值*/                    
            val = ((uint32_t)src[0]<<8u) + (uint32_t)src[1];
            break;
        /*- 宽度为-4*/
        case (-4):
            /*- 赋值*/                    
            val = ((uint32_t)src[0]<<24u) + ((uint32_t)src[1]<<16u) + ((uint32_t)src[2]<<8u) + (uint32_t)src[3];
            break;
        /*- 宽度为其他值*/
        default: 
            /*- 宽度值无效*/                                
            ;
            break;      
    }

    /*- 返回重组的整数值 */
	return ((int32_t)(val)); 

}

/**
 * @brief cb_ptr_get_index 从内存地址获取指定宽度的数据
 * @detail      根据偏移量从指针开始地址获取一组字节串
 *              根据指定宽度重组为整数值
 *
 * @param src   数据源地址，范围：指针非空
 * @param width 数据宽度，范围：自然范围
 * @param index 偏移地址，范围：自然范围
 *
 * @return      返回重组的整数值
 */
int32_t cb_ptr_get_index(uint8_t src[], int16_t width, uint16_t index)
{
    /*- 变量初始化 */
    int32_t data = 0;

	/*- 指针src为空则直接宕机 */
	UNIFW_ASSERT(NULL != src);

    /*- 从指定偏移量的地址读取指定宽度的数据 */
    data = cb_ptr_get(&src[index], width);
    
    /*- 返回重组的整数值 */
    return (data);
}

/**
 * @brief      usr_memcpy 内存区拷贝
 * @detail     将指定内存地址的指定大小内存拷贝到目的地址
 * @param to   目的内存地址，范围：指针非空
 * @param from 源内存地址，范围：指针非空
 * @param size 内存区域大小，范围：自然范围
 * @return  无
 */
void usr_memcpy(void *to, const void *from, uint32_t size)
{
    /*- 变量初始化 */
	uint32_t i =0u;
	uint8_t  *p_to, *p_from = NULL;
    uint32_t v32_to = (uint32_t)to, v32_from = (uint32_t)from;
    uint32_t *p32_to;
    uint32_t *p32_from;
    uint16_t *p16_to;
    uint16_t *p16_from;
    uint32_t tail, alignCNT;

	/*- 指针to和from为空则直接宕机 */
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
            /*- 内存数据拷贝 */
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
            /*- 内存数据拷贝 */
            p_to[i] = p_from[i];
        }
    }
    else
    {
        /*- i=0;i小于内存区大小时;对i加1 */
        for (i=0u; i<size; i++) 
        {
            /*- 内存数据拷贝 */
            p_to[i] = p_from[i];
        }
    }
}

/**
 * @brief      usr_memsafecpy 内存区安全拷贝
 * @detail     将指定内存地址的指定大小内存拷贝到目的地址
 * @param to   目的内存地址，范围：指针非空
 * @param from 源内存地址，范围：指针非空
 * @param size 内存区域大小，范围：自然范围
 * @return  无
 */
void usr_memsafecpy(void *to, const void *from, uint32_t size)
{
    /*- 变量初始化 */
	uint32_t i =0u;
	uint8_t  *p_to, *p_from = NULL;
    uint32_t v32_to = (uint32_t)to, v32_from = (uint32_t)from;
    uint32_t *p32_to;
    uint32_t *p32_from;
    uint16_t *p16_to;
    uint16_t *p16_from;
    uint32_t tail, alignCNT;

	/*- 指针to和from为空则直接宕机 */
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
            /*- 内存数据拷贝 */
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
            /*- 内存数据拷贝 */
            p_to[i] = p_from[i];
            p_from[i] = 0;
        }
    }
    else
    {
        /*- i=0;i小于内存区大小时;对i加1 */
        for (i=0u; i<size; i++) 
        {
            /*- 内存数据拷贝 */
            p_to[i] = p_from[i];
            p_from[i] = 0;
        }
    }
}


/**
 * @brief      usr_memset 内存区初始化赋值
 *
 * @detail     将指定内存地址的指定大小内存按照uint8_t赋值
 *
 * @param to   内存地址，范围：指针非空
 * @param val  初始化赋值，范围：自然范围
 * @param size 内存区域大小，范围：自然范围
 *
 * @return     无
 */
void usr_memset(void *to, uint8_t val, uint32_t size)
{
    /*- 变量初始化 */
    uint32_t i = 0u;
	uint8_t  *p_to = NULL;
    uint32_t vaddr = (uint32_t)to;
    uint32_t head, u32len, tail, val32;
    uint32_t *p_to32 = NULL;

	/*- 指针to为空则直接宕机 */
	UNIFW_ASSERT(NULL != to);

    p_to = to;
#if 0
    /*- i=0;i小于内存区大小时;对i加1 */
    for (i=0u; i<size; i++) 
    {
        /*- 将指定内存地址的指定大小区域用指定值初始化 */
        p_to[i] = val;
    }
#endif
    // 当数据量小时，直接赋值
    if (size < 10) 
    {
        for (i=0u; i<size; i++) 
        {
            /*- 将指定内存地址的指定大小区域用指定值初始化 */
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
                /*- 将指定内存地址的指定大小区域用指定值初始化 */
                p_to[i] = val;
            }
            p_to32   = (uint32_t *)(vaddr + 4 - head);
            u32len   = (size - head)>>2;
            tail     = head + (u32len<<2);
        }

        for (i=tail; i<size; i++)
        {
            /*- 将指定内存地址的指定大小区域用指定值初始化 */
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
 * @brief       min_u8 比较两个uint8_t的数
 *
 * @detail      计算两数中的较小值
 *
 * @param a     数1，范围：自然范围
 * @param b     数2，范围：自然范围
 *
 * @return      两数中的较小值
 */
uint8_t min_u8(uint8_t a, uint8_t b)
{
    /*- 变量初始化 */
    uint8_t ret = 0u;

    /*- @alias 比较两数 */
    if (a <= b)
	{
        ret = a;
	}
    else
	{
        ret = b;
	}

    /*- 返回比较结果 */
    return (ret);
}

/**
 * @brief       min_u16 比较两个uint16_t的数
 *
 * @detail      计算两数中的较小值
 *
 * @param a     数1，范围：自然范围
 * @param b     数2，范围：自然范围
 *
 * @return      两数中的较小值
 */
uint16_t min_u16(uint16_t a, uint16_t b)
{
    /*- 变量初始化 */
    uint16_t ret = 0u;
    
    /*- @alias 比较两数 */
    if (a <= b)
    {
		ret = a;
	}
    else
	{
        ret = b;
	}

    /*- 返回比较结果 */
    return (ret);
}


/**
 * @brief       min_u32 比较两个uint32_t的数
 *
 * @detail      计算两数中的较小值
 *
 * @param a     数1，范围：自然范围
 * @param b     数2，范围：自然范围
 *
 * @return      两数中的较小值
 */
uint32_t min_u32(uint32_t a, uint32_t b)
{
    /*- 变量初始化 */
    uint32_t ret = 0u;
    
    /*- @alias 比较两数 */
    if (a <= b)
	{
		ret = a;
	}
    else
	{
        ret = b;
	}

    /*- 返回比较结果 */
    return (ret);
}


/**
 * @brief       min_i8 比较两个int8_t的数
 *
 * @detail      计算两数中的较小值
 *
 * @param a     数1，范围：自然范围
 * @param b     数2，范围：自然范围
 *
 * @return      两数中的较小值
 */
int8_t min_i8(int8_t a, int8_t b)
{
    /*- 变量初始化 */
    int8_t ret = 0;

    /*- @alias 比较两数 */
    if (a <= b)
	{
        ret = a;
	}
    else
	{
        ret = b;
	}

    /*- 返回比较结果 */
    return (ret);
}


/**
 * @brief       min_i16 比较两个int16_t的数
 *
 * @detail      计算两数中的较小值
 *
 * @param a     数1，范围：自然范围
 * @param b     数2，范围：自然范围
 *
 * @return      两数中的较小值
 */
int16_t min_i16(int16_t a, int16_t b)
{
    /*- 变量初始化 */
    int16_t ret = 0;
    
    /*- @alias 比较两数 */
    if (a <= b)
	{
        ret = a;
	}
    else
	{
        ret = b;
	}

    /*- 返回比较结果 */
    return (ret);
}

/**
 * @brief       min_i32 比较两个int32_t的数
 *
 * @detail      计算两数中的较小值
 *
 * @param a     数1，范围：自然范围
 * @param b     数2，范围：自然范围
 *
 * @return      两数中的较小值
 */
int32_t min_i32(int32_t a, int32_t b)
{
    /*- 变量初始化 */
    int32_t ret = 0;
    
    /*- @alias 比较两数 */
    if (a <= b)
	{
        ret = a;
	}
    else
	{
        ret = b;
	}

    /*- 返回比较结果 */
    return (ret);
}

/**
 * @brief       max_u8 比较两个uint8_t的数
 *
 * @detail      计算两数中的较大值
 *
 * @param a     数1，范围：自然范围
 * @param b     数2，范围：自然范围
 *
 * @return      两数中的较大值
 */
uint8_t max_u8(uint8_t a, uint8_t b)
{
    /*- 变量初始化 */
    uint8_t ret = 0u;

    /*- @alias 比较两数 */
    if (a >= b)
	{
        ret = a;
	}
    else
	{
        ret = b;
	}

    /*- 返回比较结果 */
    return (ret);
}


/**
 * @brief       max_u16 比较两个uint16_t的数
 *
 * @detail      计算两数中的较大值
 *
 * @param a     数1，范围：自然范围
 * @param b     数2，范围：自然范围
 *
 * @return      两数中的较大值
 */
uint16_t max_u16(uint16_t a, uint16_t b)
{
    /*- 变量初始化 */
    uint16_t ret = 0u;
    
    /*- @alias 比较两数 */
    if (a >= b)
	{
        ret = a;
	}
    else
	{
        ret = b;
	}

    /*- 返回比较结果 */
    return (ret);
}


/**
 * @brief       max_u32 比较两个uint32_t的数
 *
 * @detail      计算两数中的较大值
 *
 * @param a     数1，范围：自然范围
 * @param b     数2，范围：自然范围
 *
 * @return      两数中的较大值
 */
uint32_t max_u32(uint32_t a, uint32_t b)
{
    /*- 变量初始化 */
    uint32_t ret = 0u;
    
    /*- @alias 比较两数 */
    if (a >= b)
	{
        ret = a;
	}
    else
	{
        ret = b;
	}

    /*- 返回比较结果 */
    return (ret);
}


/**
 * @brief       max_i8 比较两个int8_t的数
 *
 * @detail      计算两数中的较大值
 *
 * @param a     数1，范围：自然范围
 * @param b     数2，范围：自然范围
 *
 * @return      两数中的较大值
 */
int8_t max_i8(int8_t a, int8_t b)
{
    /*- 变量初始化 */
    int8_t ret = 0;

    /*- @alias 比较两数 */
    if (a >= b)
	{
        ret = a;
	}
    else
	{
        ret = b;
	}

    /*- 返回比较结果 */
    return (ret);
}

/**
 * @brief       max_i16 比较两个int16_t的数
 *
 * @detail      计算两数中的较大值
 *
 * @param a     数1，范围：自然范围
 * @param b     数2，范围：自然范围
 *
 * @return      两数中的较大值
 */
int16_t max_i16(int16_t a, int16_t b)
{
    /*- 变量初始化 */
    int16_t ret = 0;
    
    /*- @alias 比较两数 */
    if (a >= b)
	{
        ret = a;
	}
    else
	{
        ret = b;
	}

    /*- 返回比较结果 */
    return (ret);
}


/**
 * @brief       max_i32 比较两个int32_t的数
 *
 * @detail      计算两数中的较大值
 *
 * @param a     数1，范围：自然范围
 * @param b     数2，范围：自然范围
 *
 * @return      两数中的较大值
 */
int32_t max_i32(int32_t a, int32_t b)
{
    /*- 变量初始化 */
    int32_t ret = 0;
    
    /*- @alias 比较两数 */
    if (a >= b)
	{
        ret = a;
	}
    else
	{
        ret = b;
	}

    /*- 返回比较结果 */
    return (ret);
}


/**
 * @brief       abs_value_i8 计算绝对值
 *
 * @detail      计算一个int8_t数的绝对值
 *
 * @param a     入参，范围：自然范围
 *
 * @return      入参的绝对值
 */
uint8_t abs_value_i8(int8_t a)
{
    /*- 变量初始化 */
    uint8_t ret = 0u;

    /*- @alias 计算绝对值 */
    if (a >= 0)
	{
        ret = a;
	}
    else 
	{
        ret = 0 - a;
	}

    /*- 返回结果 */
    return (ret);
}

/**
 * @brief       abs_value_i16 计算绝对值
 *
 * @detail      计算一个int16_t数的绝对值
 *
 * @param a     入参，范围：自然范围
 *
 * @return      入参的绝对值
 */
uint16_t abs_value_i16(int16_t a)
{
    /*- 变量初始化 */
    uint16_t ret = 0u;

    /*- @alias 计算绝对值 */
    if (a >= 0)
	{
        ret = a;
	}
    else 
	{
        ret = 0 - a;
	}

    /*- 返回结果 */
    return (ret);
}


/**
 * @brief       abs_value_i32 计算绝对值
 *
 * @detail      计算一个int32_t数的绝对值
 *
 * @param a     入参，范围：自然范围
 *
 * @return      入参的绝对值
 */
uint32_t abs_value_i32(int32_t a)
{
    /*- 变量初始化 */
    uint32_t ret = 0u;

    /*- @alias 计算绝对值 */
    if (a >= 0)
	{
        ret = a;
	}
    else 
	{
        ret = 0 - a;
	}

    /*- 返回结果 */
    return (ret);
}


/**
 * @brief       abs_value_u8 计算绝对值
 *
 * @detail      计算一个uint8_t数的绝对值
 *
 * @param a     入参，范围：自然范围
 *
 * @return      入参的绝对值
 */
uint8_t abs_value_u8(uint8_t a)
{
    /*- 返回结果 */
    return (a);
}

/**
 * @brief       abs_value_u16 计算绝对值
 *
 * @detail      计算一个uint16_t数的绝对值
 *
 * @param a     入参，范围：自然范围
 *
 * @return      入参的绝对值
 */
uint16_t abs_value_u16(uint16_t a)
{
    /*- 返回结果 */
    return (a);
}

/**
 * @brief       abs_value_u32 计算绝对值
 *
 * @detail      计算一个uint32_t数的绝对值
 *
 * @param a     入参，范围：自然范围
 *
 * @return      入参的绝对值
 */
uint32_t abs_value_u32(uint32_t a)
{
    /*- 返回结果 */
    return (a);
}

/**
 * @brief       diff_value_u8 计算两数的差值
 *
 * @detail      两个uint8_t数的差值
 *
 * @param a     数a，范围：自然范围
 * @param b     数b，范围：自然范围
 *
 * @return      两数之差
 */
uint8_t diff_value_u8(uint8_t a, uint8_t b)
{
    /*- 返回结果 */
    return (max_u8(a,b) - min_u8(a,b));
}

/**
 * @brief       diff_value_u16 计算两数的差值
 *
 * @detail      两个uint16_t数的差值
 *
 * @param a     数a，范围：自然范围
 * @param b     数b，范围：自然范围
 *
 * @return      两数之差
 */
uint16_t diff_value_u16(uint16_t a, uint16_t b)
{
    /*- 返回结果 */
    return (max_u16(a,b) - min_u16(a,b));
}

/**
 * @brief       diff_value_u32 计算两数的差值
 *
 * @detail      两个uint32_t数的差值
 *
 * @param a     数a，范围：自然范围
 * @param b     数b，范围：自然范围
 *
 * @return      两数之差
 */
uint32_t diff_value_u32(uint32_t a, uint32_t b)
{
    /*- 返回结果 */
    return (max_u32(a,b) - min_u32(a,b));
}

/**
 * @brief       diff_value_i8 计算两数的差值
 *
 * @detail      两个int8_t数的差值
 *
 * @param a     数a，范围：自然范围
 * @param b     数b，范围：自然范围
 *
 * @return      两数之差
 */
int8_t diff_value_i8(int8_t a, int8_t b)
{
    /*- 返回结果 */
    return (max_i8(a,b) - min_i8(a,b));
}


/**
 * @brief       diff_value_i16 计算两数的差值
 *
 * @detail      两个int16_t数的差值
 *
 * @param a     数a，范围：自然范围
 * @param b     数b，范围：自然范围
 *
 * @return      两数之差
 */
int16_t diff_value_i16(int16_t a, int16_t b)
{
    /*- 返回结果 */
    return (max_i16(a,b) - min_i16(a,b));
}

/**
 * @brief       diff_value_i32 计算两数的差值
 *
 * @detail      两个int32_t数的差值
 *
 * @param a     数a，范围：自然范围
 * @param b     数b，范围：自然范围
 *
 * @return      两数之差
 */
int32_t diff_value_i32(int32_t a, int32_t b)
{
    /*- 返回结果 */
    return (max_i32(a,b) - min_i32(a,b));
}

/**
 * @brief       cb_atoi 字符串转数字
 *
 * @param str   待转换的字符串
 * @param perr  错误代码
 *
 * @return      转换结果
 */
int32_t cb_atoi(char * str, int8_t *perr)
{
    /*- 变量初始化 */
    int32_t value = 0;
    int32_t sign  = 1;
    int32_t radix = 0;
    uint8_t loop = 0;

    /*- 空指针检查 */
    if (str == NULL) 
    {
        *perr = E_NULL_PT;
    }
    else{
        /*- 判断首位是否为'-' */
        if (*str == '-')
        {
            /*- 符号位为负 */
            sign = -1;
            str++;
        }else{
            /*- 符号位为正 */
            sign = 1;
        }
        /*- 判断是否16进制 */
        if (*str == '0' && (*(str+1) == 'x' || *(str+1) == 'X'))
        {
            radix = 16; /*- 设置为16进制 */
            str += 2;
        }
        else if (*str == '0') /*-< 是否8进制 */
        {
            radix = 8; /*- 设置为8进制 */
            str++;
        }
        else
        {
            radix = 10; /*- 设置为10进制 */
        }
        
        /*- 设置解析结果为正常 */
        *perr = E_OK;
        /*- 遍历字符串 */
        while (*str && loop < 32)
        {
            /*- 解析个数+1 */
            loop++;
            if (radix == 16)  /*-< 为16进制 */
            {
                if(*str >= '0' && *str <= '9')
                {
                    value = value * radix + *str - '0';    //0到9的字符串
                }    
                else if(*str >= 'A' && *str <= 'F')
                {
                    value = value * radix + *str - 'A' + 10;   //大写十六进制的ABCDEF的字符串的情况
                }    
                else if(*str >= 'a' && *str <= 'f')
                {
                    value = value * radix + *str - 'a' + 10;  // 小写十六进制的abcdef的字符串的情况
                }    
                else
                {
                    *perr = E_FORMAT; /*-< 格式错误 */
                }
            }
            else if (radix == 8)  /*-< 为8进制 */
            {
                if(*str >= '0' && *str <= '7')
                {
                    value = value * radix + *str - '0';
                }else{
                    *perr = E_FORMAT; /*-< 格式错误 */
                }
            }
            else if (radix == 10)  /*-< 为10进制 */
            {
                if(*str >= '0' && *str <= '9')
                {
                    value = value * radix + *str - '0';
                }else{
                    *perr = E_FORMAT; /*-< 格式错误 */
                }
            }
            str++;
        }
    }

    /*- 解析成功 */
    if (*perr == E_OK)
    {
        /*- 未进入循环 */
        if (loop == 0){
            value = 0;
            if (radix == 8) {
                ;
            }
            else{
                *perr = E_FORMAT;
            }
        }else if (loop >= 32) /*-< 数据太长未解析结束 */
        {
            value = 0;
            *perr = E_FORMAT;
        }else{
            ;
        }
    }else{
        value = 0;
    }

    /*- 返回结果 */
    return (sign*value);
}
