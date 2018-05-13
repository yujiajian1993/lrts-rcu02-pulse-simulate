/**
 * @file b2v.c
 * @brief 
 * @author hu.renqiang, zhuo.kaikuo
 * @version 
 * @date 2016-10-1, 2017-7
 */

#include "unifw.h"
#include "unifw_errno.h"

#include "base/codebase.h"
#include "base/b2v.h"

uint32_t b2v_stream_get_adapt(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr);
uint32_t b2v_bytes_get_adapt(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr);

/**
 * @brief b2v_check_enum 检查枚举数值
 *
 * @param tab 范围：指针非空
 * @param len 范围：uint16
 * @param value 范围：任意值
 *
 * @return 
 * 	- 0 表示成功
 * 	- -E_INVALID_RANGE 表示失败
 */
int16_t b2v_check_enum(int32_t *tab, uint16_t len, int32_t value)
{
	int16_t ret=0;
	int16_t i;
	/*- 初始化；搜索表未结束；遍历搜索表 */
	for (i=0; i<len; i++)
	{
		/*- 找到匹配值 */
		if (tab[i] == value)
		{
			break;
		}
	}
	/*- 偏移量等于选项值 */
	if (i == len)
	{
		ret = -E_INVALID_RANGE;
	}
	return ret;
}

/**
 * @brief b2v_check_v 检查比特域中的数值合法性
 *
 * @param vitem 范围：指针非空
 * @param value 范围：任意值
 *
 * @return 
 * 	- 0 表示合法
 * 	- -E_TYPE_FAULT 表示防护类型非法
 * 	- -E_INVALID_RANGE 表示非法
 */
int16_t b2v_check_v(const var_item_t *vitem, int32_t value)
{
	int16_t ret=0;
	/*- 断言判断 */
	UNIFW_ASSERT(NULL!=vitem);
	/*- 防护类型 */
	switch(vitem->range_type)
	{
		case B2V_RANGEP_RESERVED:	/*-< 预留不检查 */
			break;
		case B2V_RANGEP_ENUM:	/*-< 枚举 */
			UNIFW_ASSERT(NULL!=vitem->enum_table);
			ret = b2v_check_enum(vitem->enum_table, vitem->enum_len, value);
			break;
		case B2V_RANGEP_MINMAX:	/*-< [min, max] */
			if (value < vitem->vmin || value > vitem->vmax)
			{
				ret = -E_INVALID_RANGE;
			}
			break;
		case B2V_RANGEP_NEMINMAX:	/*-< (min, max) */
			if (value <= vitem->vmin || value >= vitem->vmax)
			{
				ret = -E_INVALID_RANGE;
			}
			break;
		case B2V_RANGEP_MIXED:	/*-< 混合型 */
			/*- 在[min, max]范围内 */
			if (value >= vitem->vmin && value <= vitem->vmax)
			{
				;
			}else{
				/*- 按枚举类型计算返回值 */
				ret = b2v_check_enum(vitem->enum_table, vitem->enum_len, value);
			}
			break;
		case B2V_RANGEP_EQUAL:	/*-< 等于型 */
			if (value != vitem->vmin)
			{
				ret = -E_INVALID_RANGE;
			}
			break;
		default:	/*-< 增加非法分支 */
			ret = -E_TYPE_FAULT;
	}
	return ret;
}

/**
 * @brief b2v_check_bytes 检查字节内容
 *
 * @param pstream[] 原始的bit流
 * @param table 描述表
 * @param var_num 描述表数量
 *
 * @return 
 * 	- 0表示无错误
 * 	- -E_INVALID_RANGE表示非法的范围
 */
int16_t b2v_check_bytes(uint8_t pstream[], const var_item_t * table, uint8_t var_num)
{
	/*- 定义所需中间变量 */
	int16_t i, ret=0;
	uint16_t offset=0;
	uint32_t data, err;
	UNIFW_ASSERT(NULL != table);
	/*- 初始化; 未到达表尾 ; 遍历 */
	for (i=0; i<var_num; i++)
	{
		/*- 获取相关变量，不进行任何存储 */
		data = b2v_bytes_get(pstream, offset, table[i].width, table[i].endian, &err); 

        /*- 数组偏移增加width宽度 */
        offset += table[i].width;

		/*- 若检查结果非法 */
		if (b2v_check_v(&table[i], data) <0)
		{
			/*- 退出遍历，并返回错误代码 */
			ret = -E_INVALID_RANGE;
			break;
		}
	}
	return ret;
}

/**
 * @brief b2v_dump 输出b2v的结构体内容
 *
 * @param data 结构体的基地址
 * @param table 描述表
 * @param var_num 描述表的条目数量
 *
 * @return 
 */
int16_t b2v_dump(void *src, const var_item_t * table, uint8_t var_num)
{
	/*- 定义所需中间变量 */
	int16_t i;
	uint32_t perr;
	uint32_t ret=0, tmp;
	uint8_t *data = (uint8_t*)src;
	printp("\nb2v dump:");
	/*- 初始化; 未到达表尾 ; 遍历 */
	for (i=0; i<var_num; i++)
	{
		/*- 变量可打印 */
		if (0 !=table[i].name[0])
		{
			tmp = b2v_var_get(&data[table[i].offset_d], table[i].size, &perr);
			printp("%16s: %5d	", table[i].name, ret);
			/*- 每两格一列 */
			switch (ret%2)
			{
				case 1:
					printp("\n");
					break;
			}
			ret ++;
		}
	}
	return ret;
}

/** 
 * @brief b2v_var_set 设置指定类型的数据到指定目标地址
 *
 * @detail            将数据按照其类型的数据宽度， 封装到指定目标地址
 *
 * @param paddr       目标地址，范围：指针非空
 * @param value       变量值，范围：自然范围
 * @param size        指定的数据宽度，范围：{1,2,4}
 * @param *perr       错误代码返回值，范围：指针非空
 *
 * @return            无
 */
void b2v_var_set(void *paddr, uint32_t value, uint8_t size, uint32_t *perr)
{
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);

    /*- 入参是否有空指针 */
    if (NULL == paddr)
    {
        /*- 设置错误代码: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }
    else
    { 
		/*- 设置错误代码: E_OK */  
	    *perr = (uint32_t)E_OK;

		/*- 根据数据的宽度把值赋给指定地址, 如果宽度无效则设置错误代码 */
        switch (size)
        {
			/*- 宽度为1 */
            case (1u):
				/*- 按照单字节赋值 */
                *((uint8_t *)paddr) = (uint8_t)value;
                break;
			/*- 宽度为2 */
            case (2u):
			    /*- 按照2字节赋值 */
                *((uint16_t *)paddr) = (uint16_t)value;
                break;
			/*- 宽度为4 */
            case (4u):
			    /*- 按照4字节赋值 */
                *((uint32_t *)paddr) = (uint32_t)value;
                break;
			/*- 宽度为其他值 */
            default:
			    /*- 设置错误代码: E_B2V_SIZE  */
                *perr = (uint32_t)E_B2V_SIZE;
                break;
        }
    }
}

/** 
 * @brief b2v_var_get  从指定地址获取指定大小的变量值
 *
 * @detail             将指定地址根据指定变量类型的宽度解析数据的值
 *
 * @param paddr        目标地址，范围：指针非空
 * @param size         指定的数据宽度，范围：{1,2,4}
 * @param *perr        错误代码指针，范围：指针非空
 *
 * @return             解析变量值
 */
uint32_t b2v_var_get(uint8_t paddr[], uint8_t size, uint32_t *perr)
{
    /*- 变量初始化 */
    uint32_t value = 0u;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- 入参是否有空指针 */
    if (NULL == paddr)
    {
        /*- 设置错误代码: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }
    else
    {   
		/*- 设置错误代码: E_OK */  
	    *perr = (uint32_t)E_OK;

        /*- 将指定地址的数据解析成指定宽度的数据 */
        switch (size)
        {
			/*- 宽度为1 */
            case (1u):
			    /*- 按照单字节取值 */
                value = (uint32_t)(*((uint8_t *)paddr));
                break;
			/*- 宽度为2 */
            case (2u):
			    /*- 按照2字节取值 */
                value = (uint32_t)(*((uint16_t *)paddr));
                break;
			/*- 宽度为4 */
            case (4u):
			    /*- 按照4字节取值 */
                value = (uint32_t)(*((uint32_t *)paddr));
                break;
			/*- 宽度为其他值 */
            default:
			    /*- 设置错误代码: E_B2V_SIZE  */
                *perr = (uint32_t)E_B2V_SIZE;
                break;
        }
    }

    /*- 返回解析的变量值 */
    return (value);
}

/**
 *@brief b2v_var_get_adapt  从指定地址获取指定大小的变量值
 *
 *@param paddr  目标地址，范围：指针非空
 *@param value  变量值，范围：自然范围
 *@param size   指定的数据宽度，范围：{1,2,4}
 *@param perr   错误代码返回值，范围：指针非空
 *
 *@return 解析变量值
 */
uint32_t b2v_var_get_adapt(uint8_t paddr[], uint32_t value, uint8_t size, uint32_t *perr)
{
    /*- 从指定地址获取指定大小的变量值 */
	return b2v_var_get(paddr, size, perr);
}

/** 
 * @brief b2v_stream_set_msb 设置bit数据流中指定数据宽度的变量值(字节序大端版)
 *
 * @detail 从大端开始根据变量值得数据宽度，将数据设置到数据流的指定地址
 *
 * @param paddr     目标地址，范围：指针非空
 * @param roffset   偏移量，范围：自然范围
 * @param value     变量值，范围：自然范围
 * @param width     bit位宽度，范围：自然范围
 * @param *perr     错误代码指针，范围：指针非空
 *
 * @return          无
 */ 
void b2v_stream_set_msb(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint32_t *perr)
{
    /*- 变量初始化 */
    uint16_t i        = 0u  ;
    uint16_t index    = 0u  ;
    uint16_t ref      = 0u  ;
    uint32_t data1    = 0u  ;
    uint8_t  datas[5] = {0u};
    uint16_t len      = 0u  ;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- 入参是否有空指针 */
    if (NULL == paddr)
    {
        /*- 设置错误代码: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }else if((0u == width) || (width > 32u)) /*-< width宽度是否超出有效范围 */
	{
		/*- 设置错误代码: E_B2V_WIDTH */
		*perr = (uint32_t)E_B2V_WIDTH;
	}
    else
    {
        /*- 将数据左对齐*/
        data1 = cb_vLshift32(value, (32u - width));
        index = roffset / 8u;
        ref   = roffset % 8u;
        datas[0] = (uint8_t)cb_vRshift32(cb_vRshift32(data1, (uint8_t)ref), (uint8_t)24u);
        datas[1] = (uint8_t)cb_vRshift32(cb_vRshift32(data1, (uint8_t)ref), (uint8_t)16u);
        datas[2] = (uint8_t)cb_vRshift32(cb_vRshift32(data1, (uint8_t)ref), (uint8_t)8u );
        datas[3] = (uint8_t)cb_vRshift32(cb_vRshift32(data1, (uint8_t)ref), (uint8_t)0u );
        datas[4] = (uint8_t)cb_vRshift32(cb_vLshift32(data1, (uint8_t)(32u-ref)), (uint8_t)24u);

        /*- 设置数据长度向上取整 */
        len = (ref + width + 7u) / 8u;

        /*- @alias 清除指定宽度的原数据中的数据*/
        if ((ref+width) <= 8u)
        {
            data1 = paddr[index];
            paddr[index] = (uint8_t)((data1&cb_vLshift32(0xffu, (8u-ref))) + (data1&cb_vRshift32(0xffu, (width + ref))));
        }
        else
        {
            data1 = paddr[index];
            paddr[index] = data1&cb_vLshift32(0xffu, (8u-ref));
            for (i = 1u; i < (len-1U); i++)
            {
                ((uint8_t *)paddr)[index+i] = 0u;
            }
            if (((ref+width)%8u) == 0u)
            {
                ((uint8_t *)paddr)[(index+len)-1u] = 0u;
            }
            else
            {
                data1 = paddr[(index+len)-1u];
                paddr[(index+len)-1u] = (uint8_t)(data1&cb_vRshift32(0xffu, ((width + ref)%8u)));
            }
            
        }
        /*- @alias 循环将当前数据压入目标地址 */
        for (i = 0u; i < len; i++)
        {
            ((uint8_t *)paddr)[index+i] |= datas[i];
        }

        /*- 设置错误代码: E_OK */
		*perr = (uint32_t)E_OK;
    }
}

/**
 * @brief b2v_stream_set_lsb 设置指定数据宽度的变量值到bit流中(字节序小端版)
 *
 * @detail 从小端开始根据变量值得数据宽度，将数据设置到数据流的指定地址
 *
 * @param paddr        目标地址，范围：指针非空
 * @param roffset      偏移量，范围：自然范围
 * @param value        变量值，范围：自然范围
 * @param width        bit位宽度，范围：[1,32]
 * @param *perr        错误代码指针，范围：指针非空
 *
 * @return             无
 */
void b2v_stream_set_lsb(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint32_t *perr)
{
    /*- 变量初始化 */
    uint32_t value_msb = 0u;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- 入参是否有空指针 */
    if (NULL == paddr)
    {
        /*- 设置错误代码: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }else if((0u == width) || (width > 32u)) /*-< width宽度是否超出有效范围 */
	{
		/*- 设置错误代码: E_B2V_WIDTH */
		*perr = (uint32_t)E_B2V_WIDTH;
	}
    else
    {
        /*- 根据字节流的指定宽度，转换为大端模式 */
        switch (width)
        {
			/*- 宽度为8 */
            case (8u):
                /*- 按8bit取值 */
                value_msb = value; 
                break;
			/*- 宽度为16 */
            case (16u):
                /*- 按16bit取值 */
                value_msb = (cb_vLshift32(value, 8u)&0x00ff00u) + (cb_vRshift32(value, 8u)&0xffu); 
                break;
			/*- 宽度为32 */
            case (32u):
                /*- 按32bit取值 */
                value_msb = (cb_vLshift32(value, 24u)&0xff000000u) + (cb_vLshift32(value, 8u)&0x00ff0000u) + (cb_vRshift32(value, 8u)&0xff00u) + (cb_vRshift32(value, 24u)&0xffu);  
                break;
			/*- 宽度为其他值 */
            default:
			    /*- 非8bit对齐, 直接认为是大端 */
                value_msb = value;
                break;
        }

        /*- 设置bit数据流中指定数据宽度的变量值 */
        b2v_stream_set_msb(paddr, roffset, value_msb, width, perr);
    }
}

/** 
 * @brief b2v_stream_set 设置bit数据流中指定数据宽度的变量值
 *
 * @detail 按照指定字节序， 根据变量值得数据宽度，将数据设置到数据流的指定地址
 *
 * @param paddr        目标地址，范围：指针非空
 * @param roffset      偏移量，范围：自然范围
 * @param value        变量值，范围：自然范围
 * @param width        bit位宽度，范围：自然范围
 * @param endian       字节序，范围：{0,1}
 * @param *perr        错误代码指针，范围：指针非空
 *
 * @return             无
 */
void b2v_stream_set(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr)
{
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);

    /*- 入参是否有空指针 */
    if (NULL == paddr)
    {
        /*- 设置错误代码: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
	}else
    {
		*perr = (uint32_t)E_OK; /*-< 设置错误代码: E_OK */
    
        /*- 是否为大端模式 */
        if (UNIFW_BIG_ENDIAN == endian)
        {
            /*- 按大端模式赋值 */
            b2v_stream_set_msb(paddr, roffset, value, width, perr);
        }
        else if (UNIFW_LITTLE_ENDIAN == endian) /*-< 是否为小端模式 */
        {
            /*- 按小端模式赋值 */
            b2v_stream_set_lsb(paddr, roffset, value, width, perr);
        }   
        else
        {
            /*- 设置错误代码: E_B2V_ENDIAN */
            *perr = (uint32_t)E_B2V_ENDIAN;
        }
    }
}
/** 
 * @brief  b2v_stream_get_lsb 读取bit流中指定数据宽度的变量值(字节序小端版)
 *
 * @detail 从小端开始，根据指定变量类型的宽度从bit流中读取变量值
 *
 * @param paddr        目标地址，范围：指针非空
 * @param roffset      偏移量，范围：自然范围
 * @param width        bit位宽度，范围：[1,32]
 * @param *perr        错误代码指针，范围：指针非空
 *
 * @return 解析数据值
 */
uint32_t b2v_stream_get_lsb(uint8_t paddr[], uint16_t roffset, uint8_t width, uint32_t *perr)
{
   /*- 变量初始化 */
    uint16_t index = 0u;
    uint8_t offset = 0u;
    uint32_t data  = 0u;

    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- 入参是否有空指针 */
    if (paddr == NULL)
    {
        /*- 设置错误代码: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }
	else if((0u == width) || (width > 32u)) /*-< width超出范围 */
	{
		/*- 设置错误代码: E_B2V_WIDTH */
		*perr = (uint32_t)E_B2V_WIDTH;
	}
    else
    {
        /*- 获取待解析数据的起始字节和起始位 */
        index  = (uint16_t)(roffset / 8u);
        offset = (uint8_t)(roffset % 8u);
        data   = 0u;

        /*- 解析32bit数据 */
        data  = (uint32_t)cb_vLshift32(paddr[index]   , 24u + offset);
        data += (uint32_t)cb_vLshift32(paddr[index+1u], 16u + offset);
        data += (uint32_t)cb_vLshift32(paddr[index+2u], 8u + offset );
        data += (uint32_t)cb_vLshift32(paddr[index+3u], offset      );
        data += (uint32_t)cb_vRshift32(paddr[index+4u], 8u - offset );
        /*- 截取低地址位的有效width长度数据 */
        data = cb_vRshift32(data, (32u - width));

		/*- 设置错误代码: E_OK */
		*perr = (uint32_t)E_OK;

        /*- 根据字节流的指定宽度，读取解析数据*/
        switch (width)
        {
			/*- 宽度为8 */
            case (8u):
                /*- 按8bit取值 */
                data = data; 
                break;
			/*- 宽度为16 */
            case (16u):
                /*- 按16bit取值 */
                data = (cb_vLshift32(data, 8u)&0x00ff00u) + (cb_vRshift32(data, 8u)&0xffu); 
                break;
			/*- 宽度为32 */
            case (32u):
                /*- 按32bit取值 */
                data = (cb_vLshift32(data, 24u)&0xff000000u) + (cb_vLshift32(data, 8u)&0x00ff0000u) + (cb_vRshift32(data, 8u)&0xff00u) + (cb_vRshift32(data, 24u)&0xffu);  
                break;
			/*- 宽度为其他值 */
            default:
			    /*- 按照大端截取低地址位的有效width长度数据 */
                data = data;
                break;
        }
    }

    /*- 返回解析结果 */
    return (data);
}

/** 
 * @brief  b2v_stream_get_msb 读取bit数据流中指定数据宽度的变量值(字节序大端版)
 *
 * @detail 从大端开始，根据指定变量类型的宽度从bit流中读取变量值
 *
 * @param paddr        目标地址，范围：指针非空
 * @param roffset      偏移量，范围：自然范围
 * @param width        bit位宽度，范围：[1,32]
 * @param *perr        错误代码指针，范围：指针非空
 *
 * @return 解析数据值
 */
uint32_t b2v_stream_get_msb(uint8_t paddr[], uint16_t roffset, uint8_t width, uint32_t *perr)
{
    /*- 变量初始化 */
    uint16_t index = 0u;
    uint8_t offset = 0u;
    uint32_t data  = 0u;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- 入参是否有空指针 */
    if (NULL == paddr)
    {
        /*- 设置错误代码: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }else if((0u == width) || (width > 32u)) /*-< 入参width宽度错误 */
	{
		/*- 设置错误代码: E_B2V_WIDTH */
		*perr = (uint32_t)E_B2V_WIDTH;
	}
    else
    {
        /*- 获取待解析数据的起始字节和起始位 */
        index  = (uint16_t)(roffset / 8u);
        offset = (uint8_t)(roffset % 8u);
        data   = 0u;

        /*- 解析32bit数据 */
        data  = (uint32_t)cb_vLshift32(paddr[index]   , 24u + offset);
        data += (uint32_t)cb_vLshift32(paddr[index+1u], 16u + offset);
        data += (uint32_t)cb_vLshift32(paddr[index+2u], 8u + offset );
        data += (uint32_t)cb_vLshift32(paddr[index+3u], offset      );
        data += (uint32_t)cb_vRshift32(paddr[index+4u], 8u - offset );

        /*- 截取低地址位的有效width长度数据 */
        data = cb_vRshift32(data, 32u - width);

		/*- 设置故障代码: E_OK */
		*perr = (uint32_t)E_OK;
    }

    /*- 返回解析结果 */
    return (data);
}

/** 
 * @brief  b2v_stream_get 读取bit流中指定数据宽度的变量值
 *
 * @detail 按照指定字节序 ，根据指定变量类型的宽度从bit流中读取变量值
 *
 * @param paddr        目标地址，范围：指针非空
 * @param roffset      偏移量，范围：自然范围
 * @param width        bit位宽度，范围：[1,32]
 * @param endian       字节序，范围：{0,1}
 * @param *perr        错误代码指针，范围：指针非空
 *
 * @return 失败返回0， 成功返回解析变量值
 */
uint32_t b2v_stream_get(uint8_t paddr[], uint16_t roffset, uint8_t width, uint8_t endian, uint32_t *perr)
{
    /*- 变量初始化 */
    uint32_t ret = 0u;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- 入参是否有空指针 */
    if (NULL == paddr)
    {
        /*- 设置错误代码: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
	}else
    {
		*perr = (uint32_t)E_OK; /*-< 设置错误代码: E_OK */
		
        if (UNIFW_BIG_ENDIAN == endian) /*-< 是否大端模式 */
        {
            /*- 调用大端模式取值函数 */
            ret = b2v_stream_get_msb(paddr, roffset, width, perr);
        }
        else if (UNIFW_LITTLE_ENDIAN == endian) /*-< 是否小端模式 */
        {
            /*- 调用小端模式取值函数 */
            ret = b2v_stream_get_lsb(paddr, roffset, width, perr);
        }
        else
        {
			/*- 设置错误代码：E_B2V_ENDIAN */
            *perr = (uint32_t)E_B2V_ENDIAN;
        }
    }

    /*- 返回解析结果 */
    return (ret);
}

/* 备用 */
typedef uint32_t (*b2vf_stream_read_t)(uint8_t paddr[], uint16_t roffset, uint8_t width, uint8_t endian, uint32_t *perr);

typedef uint32_t (*b2vf_stream_write_t)(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr);

typedef uint32_t (*b2vf_var_write_t)(void *paddr, uint32_t value, uint8_t size, uint32_t *perr);

typedef uint32_t (*b2vf_var_read_t)(void *paddr, uint8_t size, uint32_t *perr);

int16_t b2v_conv_byoffset(
		uint8_t pstream[], uint16_t offset, uint8_t pvar[], 
		const var_item_t table[], uint8_t var_num, uint32_t *perr,
		uint16_t mode, 
		uint32_t (*stream_fun)(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr), 
		uint32_t (*var_fun)(void *paddr, uint32_t value, uint8_t size, uint32_t *perr)
		);

/**
 * @brief b2v_conv_byoffset 将得到的协议报文解析到关键数据结构体中
 *
 * @detail 遍历数值描述表，是一个模板函数
 *
 * @param pstream       变量地址，范围：指针非空
 * @param offset        偏移量，范围：自然范围
 * @param pvar          结构体地址, 全局变量应使用NULL, 并在配置中输入地址，范围：自然范围
 * @param table         数据描述表，范围：指针非空
 * @param var_num       变量数目,范围：[1,255]
 * @param *perr         错误代码指针，范围：指针非空
 * @param mode 			模式，范围：{B2VM_B2V, B2VM_V2B}
 * @param stream_fun    用于操作bit域的函数，按照写的方式传递参数，范围：自然范围
 * @param var_fun       用于操作变量的函数，按照写的方式传递参数，范围：自然范围
 *
 * @return 解析结果      当前解析成功的位置
 */
int16_t b2v_conv_byoffset(
		uint8_t pstream[], uint16_t offset, uint8_t pvar[], 
		const var_item_t table[], uint8_t var_num, uint32_t *perr,
		uint16_t mode, 
		uint32_t (*stream_fun)(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr), 
		uint32_t (*var_fun)(void *paddr, uint32_t value, uint8_t size, uint32_t *perr)
		)
{
    /*- 变量初始化 */
    int16_t   ret         = 0;
    uint8_t   i           = 0u;
    uint16_t  bits_offset = 0u;
    uint8_t * pdst_var    = NULL;
    uint32_t  value       = 0u;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- 入参是否有空指针，其中pdst允许使用空指针（全局变量） */
    if((NULL == pstream) || (NULL == table))
    {
        /*- 设置错误代码: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }else if  (0u == var_num) /*-< 配置表大小是否为0 */
	{
		/*- 设置错误代码: E_B2V_VAR_NUM */
		*perr = (uint32_t)E_B2V_VAR_NUM;
	}
    else
    {
		*perr = (uint32_t)E_OK; /*-< 设置错误代码: E_OK */
        bits_offset = offset; /*-< 初始解析位置 */
		/*- i=0; 遍历配置表未结束; i++ */
        for(i=0u; i<var_num; i++)
        {
            /*- 获取目的地址 */
            pdst_var = pvar + table[i].offset_d; 
            /*- @alias 从数据流得到变量值   */
			if (B2VM_B2V == mode)
			{
				value = stream_fun(pstream, bits_offset, 0, table[i].width, table[i].endian, perr);
			}else{
				value = var_fun(pdst_var, 0, table[i].size, perr);
			}

			/*- 解析变量值无错误 */
			if (E_OK == *perr)
			{
				/*- @alias 设置变量值到存储地址 */
				if (B2VM_B2V == mode)
				{
					var_fun(pdst_var, value, table[i].size, perr);
				}else{
					stream_fun(pstream, bits_offset, value, table[i].width, table[i].endian, perr);
				}

				/*- 设置变量值无错误 */
				if (E_OK == *perr)
                {
                    /*- 下一个变量解析偏移量 */
                    bits_offset = bits_offset + table[i].width;
                }else{
                    ; /*-< do_nothing */
                }
            }else{
                ; /*-< do_nothing */
            }

            /*- 遍历过程错误 */
            if (E_OK != *perr)
            {
                break; /*-< 退出遍历 */
            }
        }

        ret =  (int16_t)bits_offset; /*-< 得到解析成功后的地址偏移量 */
    }

    /*- 返回执行结果 */
    return (ret);
}

/**
 *@brief b2v_stream_get_adapt   读取bit流中指定数据宽度的变量值
 *
 *@param paddr      目标地址，范围：指针非空
 *@param roffset    偏移量，范围：自然范围
 *@param value      变量值，范围：自然范围
 *@param width      bit位宽度，范围：[1,32]
 *@param endian     字节序，范围：{0,1}
 *@param perr       错误代码指针，范围：指针非空
 *
 *@return   失败返回0， 成功返回解析变量值
 */
uint32_t b2v_stream_get_adapt(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr)
{
    /*- 读取bit流中指定数据宽度的变量值 */
	return (b2v_stream_get(paddr, roffset, width, endian, perr));
}


/**
 * @brief b2v_stream_to_var_byoffset 将得到的协议报文解析到关键数据结构体中
 *
 * @detail 将协议报文bit流按照bit偏移量和数据描述表将报文解析成关键数据结构体
 *
 * @param pstream       变量地址，范围：指针非空
 * @param offset        偏移量，范围：自然范围
 * @param pdst          结构体地址，范围：指针非空
 * @param table         数据描述表，范围：指针非空
 * @param var_num       变量数目,范围：[1,255]
 * @param *perr         错误代码指针，范围：指针非空
 *
 * @return 解析结果      当前解析成功的位置
 */
int16_t b2v_stream_to_var_byoffset(uint8_t *pstream, uint16_t offset, uint8_t * pdst, const var_item_t * table, uint8_t var_num, uint32_t *perr)
{
    /*- 将得到的协议报文解析到关键数据结构体中 */
	return b2v_conv_byoffset(
			pstream, offset, pdst, table, var_num, perr,
			B2VM_B2V, b2v_stream_get_adapt, b2v_var_set
			);
}


/**
 * @brief b2v_stream_to_var 将得到的协议信息解析到关键数据结构体中，默认偏移量为0
 *
 * @param pstream      变量地址，范围：指针非空
 * @param offset       偏移量，范围：自然范围
 * @param pdst         结构体地址，范围：指针非空
 * @param table        数据描述表，范围：指针非空
 * @param var_num      变量数目,范围：[1,255]
 * @param *perr        错误代码指针，范围：指针非空
 *
 * @return             当前解析成功的位置
 */
int16_t b2v_stream_to_var(uint8_t pstream[], void *pdst, const var_item_t * table, uint8_t var_num, uint32_t *perr)
{
	/*- 变量初始化 */
	int16_t ret = 0;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
	/*- 入参是否有空指针 */
	if ((NULL == pstream) || (NULL == pdst) || (NULL == table))
	{
        /*- 设置错误代码: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }else if(0u == var_num) /*-< 配置表大小为空 */
	{
		/*- 设置错误代码: E_B2V_VAR_NUM */
		*perr = (uint32_t)E_B2V_VAR_NUM;
	}
    else
	{
		/*- 调用下层执行函数 */
		ret = b2v_stream_to_var_byoffset(pstream, 0u, pdst, table, var_num, perr);
	}

	/*- 返回结果 */
    return (ret);
}

/**
 * @brief b2v_var_to_stream_byoffset 将数据结构体变量转换为协议信息数组
 *
 * @detail 将数据结构体按照数据描述表解析成协议报文bit流
 *
 * @param psrc         源数据结构体，范围：指针非空
 * @param pstream      目标数据流数组，范围：指针非空
 * @param offset       偏移量，范围：自然范围
 * @param table        数据解析表，范围：指针非空
 * @param var_num      条目数量, 范围：[1,255]
 * @param *perr        错误代码指针，范围：指针非空
 *
 * @return             当前解析成功的位置
 */
int16_t b2v_var_to_stream_byoffset(uint8_t psrc[], uint8_t pstream[], uint16_t offset, const var_item_t table[], uint8_t var_num, uint32_t *perr)
{
    /*- 将得到的协议报文解析到关键数据结构体中 */    
	return b2v_conv_byoffset(
			pstream, offset, psrc, table, var_num, perr,
			B2VM_V2B, b2v_stream_set, b2v_var_get_adapt		
			);
}

/**
 * @brief b2v_var_to_stream 将数据结构体变量转换为协议信息数组，默认偏移量为0
 *
 * @param psrc         源数据结构体，范围：指针非空
 * @param pstream      目标数据流数组，范围：指针非空
 * @param table        数据解析表，范围：指针非空
 * @param var_num      条目数量,范围：[1,255]
 * @param *perr        错误代码指针，范围：指针非空
 *
 * @return             当前解析成功的位置
 */
int16_t b2v_var_to_stream(void *psrc, uint8_t pstream[], const var_item_t * table, uint8_t var_num, uint32_t *perr)
{
	/*- 变量初始化 */
	int16_t ret = 0;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
	/*- 入参是否有空指针 */
	if ((NULL == psrc) || (NULL == pstream) || (NULL == table))
	{
        /*- 设置错误代码: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }else if(0u == var_num) /*-< 解析配置表条目数量为0 */
	{
		/*- 设置错误代码: E_B2V_VAR_NUM */
		*perr = (uint32_t)E_B2V_VAR_NUM;
	}
    else
	{
		/*- 调用下层执行函数执行函数 */
        ret = b2v_var_to_stream_byoffset(psrc, pstream, 0u, table, var_num, perr);
	}

	/*- 返回结果 */
	return (ret);
}


/** 整字节版本: 可提高效率 */
/**
 * @brief b2v_bytes_set_lsb 设置bit数据流中指定数据宽度的变量值(小端版)
 *
 * @detail 以字节为偏移量单位， 从小端开始，根据变量值得数据宽度，将数据设置到数据流的指定地址
 *
 * @param paddr        目标地址，范围：指针非空
 * @param roffset      偏移量，范围：自然范围
 * @param value        变量值，范围：自然范围
 * @param width        字节数，范围：[1,4]
 * @param *perr        错误代码指针，范围：指针非空
 *
 * @return  无
 */ 
void b2v_bytes_set_lsb(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint32_t *perr)
{
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- 检查入参是否有空指针 */
    if(paddr == NULL) 
    {
        /*- 设置错误代码: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }
    else
    {
		/*- 设置错误代码: E_OK */
		*perr = (uint32_t)E_OK;

        /*- 根据数据宽度设置变量值到指定地址*/
        switch (width)
        {
			/*- 宽度为1 */
            case (1u):
			    /*- 按照单字节赋值 */
                paddr[roffset] = (uint8_t)value; 
                break;
			/*- 宽度为2 */
            case (2u):
			    /*- 按照2字节赋值 */
                paddr[roffset+0u] = (uint8_t)(cb_vRshift32(value, 0u)&0xffu);
                paddr[roffset+1u] = (uint8_t)(cb_vRshift32(value, 8u)&0xffu);
                break;
			/*- 宽度为4 */
            case (4u):
			    /*- 按照4字节赋值 */
                paddr[roffset+0u] = (uint8_t)(cb_vRshift32(value, 0u )&0xffu);
                paddr[roffset+1u] = (uint8_t)(cb_vRshift32(value, 8u )&0xffu);
                paddr[roffset+2u] = (uint8_t)(cb_vRshift32(value, 16u)&0xffu);
                paddr[roffset+3u] = (uint8_t)(cb_vRshift32(value, 24u)&0xffu);
                break;
			/*- 宽度为其他值 */
            default:
				/*- 设置错误代码: E_B2V_WIDTH */
				*perr = (uint32_t)E_B2V_WIDTH;
                break;
        }
    }
}

/** 
 * @brief b2v_bytes_set_msb 设置bit数据流中指定数据宽度的变量值(大端版)
 *
 * @detail 以字节为偏移量单位， 从大端开始，根据变量值得数据宽度，将数据设置到数据流的指定地址
 *
 * @param paddr        目标地址，范围：指针非空
 * @param roffset      偏移量，范围：自然范围
 * @param value        变量值，范围：自然范围
 * @param width        字节数，范围：[1,4]
 * @param *perr        错误代码指针，范围：指针非空
 * 
 * @return 无
 */
void b2v_bytes_set_msb(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint32_t *perr)
{
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- 检查入参是否有空指针 */
    if(paddr == NULL)
    {
        /*- 设置错误代码: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }
    else
    {
		/*- 设置错误代码: E_OK */
		*perr = (uint32_t)E_OK;

        /*- 根据数据宽度设置变量值到指定地址*/
        switch (width)
        {
			/*- 宽度为1 */
            case (1u):
			    /*- 按照单字节赋值 */
                paddr[roffset] = (uint8_t)value;
                break;
			/*- 宽度为2 */
            case (2u):
			    /*- 按照2字节赋值(大端) */
                paddr[roffset+0u] = (uint8_t)(cb_vRshift32(value, 8u)&0xffu);
                paddr[roffset+1u] = (uint8_t)(cb_vRshift32(value, 0u)&0xffu);
                break;
			/*- 宽度为4 */
            case (4u):
			    /*- 按照4字节赋值(大端) */
                paddr[roffset+0u] = (uint8_t)(cb_vRshift32(value, 24u)&0xffu);
                paddr[roffset+1u] = (uint8_t)(cb_vRshift32(value, 16u)&0xffu);
                paddr[roffset+2u] = (uint8_t)(cb_vRshift32(value, 8u )&0xffu);
                paddr[roffset+3u] = (uint8_t)(cb_vRshift32(value, 0u )&0xffu);
                break;
			/*- 宽度为其他值 */
            default:
			    /*- 设置错误代码: E_B2V_WIDTH */
				*perr = (uint32_t)E_B2V_WIDTH;
                break;
        }
    }

}

/** 
 * @brief b2v_bytes_set 设置bit数据流中指定数据宽度的变量值(按照指定字节序)
 *
 * @param paddr        目标地址，范围：指针非空
 * @param roffset      偏移量，范围：自然范围
 * @param value        变量值，范围：自然范围
 * @param width        字节数，范围：[1,4]
 * @param endian       字节序，范围：{0,1}
 * @param *perr        错误代码指针，范围：指针非空
 *
 * @return 无
 */
void b2v_bytes_set(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr)
{
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
	/*- 是否大端模式 */
    if (UNIFW_BIG_ENDIAN == endian)
    {
        /*- 调用大端模式赋值 */
        b2v_bytes_set_msb(paddr, roffset, value, width, perr);
    }
    else if (UNIFW_LITTLE_ENDIAN == endian) /*-< 是否小端模式 */
    {
        /*- 调用小端模式设置 */
        b2v_bytes_set_lsb(paddr, roffset, value, width, perr);
    }
    else
    {
		/*- 设置错误代码: E_B2V_ENDIAN */
        *perr = (uint32_t)E_B2V_ENDIAN;
    }
}

/** 
 * @brief b2v_bytes_get_lsb 读取bit数据流中指定数据宽度的变量值(小端版)
 *
 * @detail 以字节为偏移量单位， 从小端开始，从数据流的指定地址，根据变量类型宽度读取变量值
 *
 * @param paddr        目标地址，范围：指针非空
 * @param roffset      偏移量，范围：自然范围
 * @param width        字节数，范围：{0,1}
 * @param *perr        错误代码指针，范围：指针非空
 *
 * @return 小端解析得到的变量值
 */ 
uint32_t b2v_bytes_get_lsb(uint8_t paddr[], uint16_t roffset, uint8_t width, uint32_t *perr)
{
    /*- 变量初始化 */
    uint32_t value = 0u;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- 入参是否有空指针 */
    if (paddr==NULL)
    {
        /*- 设置错误代码: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }
    else
    {
		/*- 设置错误代码: E_OK */
		*perr = (uint32_t)E_OK;

        /*- 根据宽度偏移量从地址读取变量值 */
        switch (width)
        {
			/*- 宽度为1 */
            case (1u):
			    /*- 按照单字节取值(小端) */
                value = (uint32_t)paddr[roffset];
                break;
			/*- 宽度为2 */
            case (2u):
			    /*- 按照2字节取值(小端) */
                value = (uint32_t)(((uint32_t)cb_vLshift32(paddr[roffset+1u],8u))&0xff00u) + (uint32_t)paddr[roffset];
                break;
			/*- 宽度为4 */
            case (4u):
			    /*- 按照4字节取值(小端) */
                value = (uint32_t)(((uint32_t)cb_vLshift32(paddr[roffset+3u], 24u))&0xff000000u) + (uint32_t)(((uint32_t)cb_vLshift32(paddr[roffset+2U], 16U))&0xff0000u) + 
                        (uint32_t)(((uint32_t)cb_vLshift32(paddr[roffset+1u], 8u))&0xff00u) + (uint32_t)paddr[roffset];
                break;
			/*- 宽度为其他值 */
            default:
			    /*- 设置故障代码: E_B2V_WIDTH */
				*perr = (uint32_t)E_B2V_WIDTH;
                break;
        }
    }

    /*- 返回解析结果 */ 
    return (value);
}

/**
 * @brief  b2v_bytes_get_msb 读取bit数据流中指定数据宽度的变量值(大端版)
 *
 * @detail 以字节为偏移量单位， 从大端开始，从数据流的指定地址，根据变量类型宽度读取变量值
 *
 * @param paddr        目标地址，范围：指针非空
 * @param roffset      偏移量，范围：自然范围
 * @param width        字节数，范围：[1,4]
 * @param *perr        错误代码指针，范围：指针非空
 *
 * @return 大端解析得到的变量值
 */
uint32_t b2v_bytes_get_msb(uint8_t paddr[], uint16_t roffset, uint8_t width, uint32_t *perr)
{
    /*- 变量初始化 */
    uint32_t value = 0u;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- 入参是否有空指针 */
    if (NULL == paddr)
    {
        /*- 设置错误代码: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }
    else
    {
		/*- 设置错误代码: E_OK */
		*perr = (uint32_t)E_OK;

        /*- 根据宽度偏移量从地址读取变量值 */
        switch (width)
         {
			/*- 宽度为1 */
            case (1u):
			    /*- 按照单字节取值(大端) */
                value = paddr[roffset];
                break;
			/*- 宽度为2 */
            case (2u):
			    /*- 按照2字节取值(大端) */
                value = (uint32_t)(((uint32_t)cb_vLshift32(paddr[roffset], 8u))&0xff00u) + (uint32_t)paddr[roffset+1u];
                break;
			/*- 宽度为4 */
            case (4u):
			    /*- 按照4字节取值(大端) */
                value = (uint32_t)(((uint32_t)cb_vLshift32(paddr[roffset], 24u))&0xff000000u) + (uint32_t)(((uint32_t)cb_vLshift32(paddr[roffset+1u], 16u))&0xff0000u) + 
                        (uint32_t)(((uint32_t)cb_vLshift32(paddr[roffset+2u], 8u))&0xff00u) + (uint32_t)paddr[roffset+3u];
                break;
			/*- 宽度为其他值 */
            default:
				/*- 设置错误代码: E_B2V_WIDTH */
				*perr = (uint32_t)E_B2V_WIDTH;
                break;
         }
    }

    /*- 返回解析结果 */
    return (value);
}

/** 
 * @brief  b2v_bytes_get 读取bit数据流中指定数据宽度的变量值(按照指定字节序) *
 * @param paddr        目标地址，范围：指针非空
 * @param roffset      偏移量，范围：自然范围
 * @param width        字节数，范围：[1,4]
 * @param endian       字节序，范围：{0,1}
 * @param *perr        错误代码指针，范围：指针非空
 *
 * @return 返回解析变量值
 */
uint32_t b2v_bytes_get(uint8_t paddr[], uint16_t roffset, uint8_t width, uint8_t endian, uint32_t *perr) {
    /*- 变量初始化 */
    uint32_t ret = 0u;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    if (UNIFW_BIG_ENDIAN == endian) /*-< 是否大端模式 */
    {
        /*- 调用大端模式取值函数 */
        ret = b2v_bytes_get_msb(paddr, roffset, width, perr);
    }
    else if (UNIFW_LITTLE_ENDIAN == endian) /*-< 是否大端模式 */
    {
        /*- 调用小端模式取值函数 */
        ret = b2v_bytes_get_lsb(paddr, roffset, width, perr);
    }
    else
    {
		/*- 设置错误代码: E_B2V_ENDIAN */
        *perr = (uint32_t)E_B2V_ENDIAN;
    }
    
    /*- 返回取值执行结果 */
    return (ret);
}

/**
 *@brief b2v_bytes_get_adapt   读取字节流中指定数据宽度的变量值
 *
 *@param paddr      目标地址，范围：指针非空
 *@param roffset    偏移量，范围：自然范围
 *@param value      变量值，范围：自然范围
 *@param width      bit位宽度，范围：[1,32]
 *@param endian     字节序，范围：{0,1}
 *@param perr       错误代码指针，范围：指针非空
 *
 *@return   失败返回0， 成功返回解析变量值
 */
uint32_t b2v_bytes_get_adapt(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr)
{
    /*- 读取字节流中指定数据宽度的变量值 */
	return b2v_bytes_get(paddr, roffset, width, endian, perr);
}

/**
 * @brief b2v_bytes_to_var_byoffset 将得到的协议信息解析到关键数据结构体中
 *
 * @detail 以字节为单位，将协议报文数据流根据数据描述表解析到关键数据结构体中
 *
 * @param pstream          变量地址，范围：指针非空
 * @param offset           偏移量，范围：自然范围
 * @param pdst             结构体地址，范围：指针非空
 * @param table            数据描述表，范围：指针非空
 * @param var_num          变量数目,范围：[1,255]
 * @param *perr            错误代码指针，范围：指针非空
 *
 * @return                 执行结束后最新的字节偏移量
 */
int16_t b2v_bytes_to_var_byoffset(uint8_t pstream[], uint16_t offset, uint8_t pdst[], const var_item_t table[], uint8_t var_num, uint32_t *perr)
{
    /*- 将得到的协议报文解析到关键数据结构体中 */    
	return b2v_conv_byoffset(
			pstream, offset, pdst, table, var_num, perr,
			B2VM_B2V, b2v_bytes_get_adapt, b2v_var_set
			);
}

/**
 * @brief b2v_bytes_to_var 将得到的协议信息解析到关键数据结构体中，默认偏移量为0
 *
 * @param pstream   变量地址，范围：指针非空
 * @param pdst      结构体地址，范围：指针非空
 * @param table     数据描述表，范围：指针非空
 * @param var_num   变量数目,范围：[1,255]
 * @param *perr     错误代码指针，范围：指针非空
 *
 * @return  执行结束后最新的字节偏移量
 */
int16_t b2v_bytes_to_var(uint8_t pstream[], void *pdst, const var_item_t * table, uint8_t var_num, uint32_t *perr)
{
	int16_t ret = 0;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);

	ret = b2v_check_bytes(pstream, table, var_num);
	if (ret < 0)
	{
		*perr = (uint32_t)E_INVALID_RANGE;
		/*- 未进行任何转换 */
		ret = 0;
	}else{
		/*- 直接调用下一层函数, 默认偏移量为0 */
		ret = (b2v_bytes_to_var_byoffset(pstream, 0u, pdst, table , var_num, perr));
	}
    
    return ret;
}

/**
 * @brief b2v_var_to_bytes_byoffset 将变量结构体转换为数据流，数据流可指定偏移量
 *
 * @detail 偏移量以字节为单位，根据数据解析表将结构体转换为数据流
 *
 * @param psrc          目标数组，范围：指针非空
 * @param pstream       数据结构体，范围：指针非空
 * @param offset        偏移量，范围：自然范围
 * @param table         数据解析表，范围：指针非空
 * @param var_num       条目数量,范围：[1,255]
 * @param *perr         错误代码指针，范围：指针非空
 *
 * @return  执行结束后最新的字节偏移量
 */
int16_t b2v_var_to_bytes_byoffset(uint8_t psrc[], uint8_t pstream[], uint16_t offset, const var_item_t table[], uint8_t var_num, uint32_t *perr)
{
    /*- 将得到的协议报文解析到关键数据结构体中 */    
	return b2v_conv_byoffset(
			pstream, offset, psrc, table, var_num, perr,
			B2VM_V2B, b2v_bytes_set, b2v_var_get_adapt
			);
}


/**
 * @brief b2v_var_to_bytes 将关键数据转换为字节数，默认偏移量为0
 *
 * @param psrc          目标数组，范围：指针非空
 * @param pstream       数据结构体，范围：指针非空
 * @param table         数据解析表，范围：指针非空
 * @param var_num       条目数量,范围：[1,255]
 * @param *perr         错误代码指针，范围：指针非空
 *
 * @return 执行结束后最新的字节偏移量
 */
int16_t b2v_var_to_bytes(void *psrc, uint8_t pstream[], const var_item_t * table, uint8_t var_num, uint32_t *perr)
{
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
	/*- 直接调用下一层函数, 默认偏移量为0 */
    return (b2v_var_to_bytes_byoffset(psrc, pstream, 0u, table, var_num, perr));
}

/**
 * @brief b2v_table_width 计算待解析（或打包）数据对应的数据描述表长度
 * 
 * @param table         数据描述表，范围：指针非空
 * @param var_num       条目数量，范围：[1,255]
 * @param *perr         错误代码指针，范围：指针非空
 * 
 * @return 返回指定数据描述表的数据配置累计长度
 */
int16_t b2v_table_width(const var_item_t table[], uint8_t var_num, uint32_t *perr)
{
	/*- 变量初始化 */
    int16_t width = 0;
    uint16_t i = 0u;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- 检查入参是否有空指针 */
    if (NULL == table)
    {
        /*- 设置错误代码: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }else if (0u == var_num) /*-< 配置表条目数量是否为0 */
	{
		/*- 设置错误代码: E_B2V_VAR_NUM */
		*perr = (uint32_t)E_B2V_VAR_NUM;
	}
    else
    {
		/*- 设置错误代码: E_OK */
		*perr = (uint32_t)E_OK;

		/*- @alias 遍历配置表, 统计数据累计宽度值 */
        for(i=0u; i<var_num; i++)
        {
			width += (int16_t)table[i].width;
        }
    }
    
	/*- 返回累计宽度值 */
    return (width);
}

/**
 * @brief b2v_iom_get_adapt iom获取函数
 *
 * @param paddr 源基地址，范围：指针非空
 * @param roffset 初始偏移量，范围：自然范围
 * @param value	  变量值，范围：自然范围，本函数内无效
 * @param width	  宽度，范围：自然范围
 * @param endian  字节序，范围：{0,1}，本函数内无效
 * @param perr    错误代码，范围：指针非空
 *
 * @return 实际获取数值
 */
uint32_t b2v_iom_get_adapt(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr)
{
	/*- 变量初始化 */
	uint32_t ret=0;
	uint8_t raw=0;

	/*- 获取待解析数据的起始字节和起始位 */
	uint16_t index  = (uint16_t)(roffset / 8u);
	uint8_t offset = (uint8_t)(roffset % 8u);

	/*- 变量断言判断 */
    UNIFW_ASSERT(NULL != perr);
    UNIFW_ASSERT(NULL != paddr);
    UNIFW_ASSERT(1u == width);

    /*- 设置错误代码为OK  */
	*perr = E_OK;

	/*- 获取原始数值 */
	raw = paddr[index];
	/*- 码位有效 */
	if ((raw>>offset)&0x00000001)
	{
        /*- 返回1 */
		ret = 1;
	}
    else
    {
        ;    /*-< do_nothing  */ 
    }

    /*- 返回实际获取数值  */
	return ret;
}

/**
 * @brief b2v_iom_set iom的设置函数
 *
 * @param paddr 源基地址，范围：指针非空
 * @param roffset 初始偏移量，范围：自然范围
 * @param value   变量值，范围：自然范围
 * @param width   宽度，范围：自然范围
 * @param endian  字节序，范围：{0,1}，本函数内无效
 * @param perr    错误代码，范围：指针非空
 *
 * @return 固定返回0
 */
uint32_t b2v_iom_set(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr)
{
	/*- 获取待解析数据的起始字节和起始位 */
	uint16_t index  = (uint16_t)(roffset / 8u);
	uint8_t offset = (uint8_t)(roffset % 8u);

	/*- 变量断言判断 */
    UNIFW_ASSERT(NULL != perr);
    UNIFW_ASSERT(NULL != paddr);
    UNIFW_ASSERT(1u == width);

    /*- 设置错误代码为OK  */
	*perr = E_OK;

	/*- 如果变量值有效 */
	if (0u != value)
	{
		/*- 设置iom  */
        paddr[index] |= (1u<<offset);
	}
    else
    {
        ;    /*-< do_nothing  */ 
    }

    /*- 固定返回0  */
	return 0;
}

/**
 * @brief b2v_iom_to_var 从io表格转换成结构体变量
 *
 * @param pstream 	io码位表，范围：指针非空
 * @param pdst		目标结构体变量，范围：指针非空
 * @param table		b2v描述表格，范围：指针非空
 * @param var_num	b2v描述表格条目数，范围：自然范围
 * @param perr		错误代码，范围：指针非空
 *
 * @return 解析结果      当前解析成功的位置
 */
int16_t b2v_iom_to_var(uint8_t *pstream, void * pdst, const var_item_t * table, uint8_t var_num, uint32_t *perr)
{
    /*- 将得到的协议报文解析到关键数据结构体中 */    
	return b2v_conv_byoffset(
			pstream, 0u, pdst, table, var_num, perr,
			B2VM_B2V, b2v_iom_get_adapt, b2v_var_set
			);
}

/**
 * @brief b2v_var_to_iom   从结构体变量转换成io表格
 *
 * @param psrc		源结构体变量，范围：指针非空
 * @param pstream   io码位表，范围：指针非空
 * @param table     b2v描述表格，范围：指针非空
 * @param var_num   b2v描述表格条目数，范围：自然范围
 * @param perr      错误代码，范围：指针非空
 *
 * @return 解析结果      当前解析成功的位置
 */
int16_t b2v_var_to_iom(void * psrc, uint8_t *pstream, const var_item_t * table, uint8_t var_num, uint32_t *perr)
{
    /*- 将得到的协议报文解析到关键数据结构体中 */    
	return b2v_conv_byoffset(
			pstream, 0u, psrc, table, var_num, perr,
			B2VM_V2B, b2v_iom_set, b2v_var_get_adapt
			);
}
