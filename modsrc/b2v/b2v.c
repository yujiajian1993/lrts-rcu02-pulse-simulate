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
 * @brief b2v_check_enum ���ö����ֵ
 *
 * @param tab ��Χ��ָ��ǿ�
 * @param len ��Χ��uint16
 * @param value ��Χ������ֵ
 *
 * @return 
 * 	- 0 ��ʾ�ɹ�
 * 	- -E_INVALID_RANGE ��ʾʧ��
 */
int16_t b2v_check_enum(int32_t *tab, uint16_t len, int32_t value)
{
	int16_t ret=0;
	int16_t i;
	/*- ��ʼ����������δ���������������� */
	for (i=0; i<len; i++)
	{
		/*- �ҵ�ƥ��ֵ */
		if (tab[i] == value)
		{
			break;
		}
	}
	/*- ƫ��������ѡ��ֵ */
	if (i == len)
	{
		ret = -E_INVALID_RANGE;
	}
	return ret;
}

/**
 * @brief b2v_check_v ���������е���ֵ�Ϸ���
 *
 * @param vitem ��Χ��ָ��ǿ�
 * @param value ��Χ������ֵ
 *
 * @return 
 * 	- 0 ��ʾ�Ϸ�
 * 	- -E_TYPE_FAULT ��ʾ�������ͷǷ�
 * 	- -E_INVALID_RANGE ��ʾ�Ƿ�
 */
int16_t b2v_check_v(const var_item_t *vitem, int32_t value)
{
	int16_t ret=0;
	/*- �����ж� */
	UNIFW_ASSERT(NULL!=vitem);
	/*- �������� */
	switch(vitem->range_type)
	{
		case B2V_RANGEP_RESERVED:	/*-< Ԥ������� */
			break;
		case B2V_RANGEP_ENUM:	/*-< ö�� */
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
		case B2V_RANGEP_MIXED:	/*-< ����� */
			/*- ��[min, max]��Χ�� */
			if (value >= vitem->vmin && value <= vitem->vmax)
			{
				;
			}else{
				/*- ��ö�����ͼ��㷵��ֵ */
				ret = b2v_check_enum(vitem->enum_table, vitem->enum_len, value);
			}
			break;
		case B2V_RANGEP_EQUAL:	/*-< ������ */
			if (value != vitem->vmin)
			{
				ret = -E_INVALID_RANGE;
			}
			break;
		default:	/*-< ���ӷǷ���֧ */
			ret = -E_TYPE_FAULT;
	}
	return ret;
}

/**
 * @brief b2v_check_bytes ����ֽ�����
 *
 * @param pstream[] ԭʼ��bit��
 * @param table ������
 * @param var_num ����������
 *
 * @return 
 * 	- 0��ʾ�޴���
 * 	- -E_INVALID_RANGE��ʾ�Ƿ��ķ�Χ
 */
int16_t b2v_check_bytes(uint8_t pstream[], const var_item_t * table, uint8_t var_num)
{
	/*- ���������м���� */
	int16_t i, ret=0;
	uint16_t offset=0;
	uint32_t data, err;
	UNIFW_ASSERT(NULL != table);
	/*- ��ʼ��; δ�����β ; ���� */
	for (i=0; i<var_num; i++)
	{
		/*- ��ȡ��ر������������κδ洢 */
		data = b2v_bytes_get(pstream, offset, table[i].width, table[i].endian, &err); 

        /*- ����ƫ������width��� */
        offset += table[i].width;

		/*- ��������Ƿ� */
		if (b2v_check_v(&table[i], data) <0)
		{
			/*- �˳������������ش������ */
			ret = -E_INVALID_RANGE;
			break;
		}
	}
	return ret;
}

/**
 * @brief b2v_dump ���b2v�Ľṹ������
 *
 * @param data �ṹ��Ļ���ַ
 * @param table ������
 * @param var_num ���������Ŀ����
 *
 * @return 
 */
int16_t b2v_dump(void *src, const var_item_t * table, uint8_t var_num)
{
	/*- ���������м���� */
	int16_t i;
	uint32_t perr;
	uint32_t ret=0, tmp;
	uint8_t *data = (uint8_t*)src;
	printp("\nb2v dump:");
	/*- ��ʼ��; δ�����β ; ���� */
	for (i=0; i<var_num; i++)
	{
		/*- �����ɴ�ӡ */
		if (0 !=table[i].name[0])
		{
			tmp = b2v_var_get(&data[table[i].offset_d], table[i].size, &perr);
			printp("%16s: %5d	", table[i].name, ret);
			/*- ÿ����һ�� */
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
 * @brief b2v_var_set ����ָ�����͵����ݵ�ָ��Ŀ���ַ
 *
 * @detail            �����ݰ��������͵����ݿ�ȣ� ��װ��ָ��Ŀ���ַ
 *
 * @param paddr       Ŀ���ַ����Χ��ָ��ǿ�
 * @param value       ����ֵ����Χ����Ȼ��Χ
 * @param size        ָ�������ݿ�ȣ���Χ��{1,2,4}
 * @param *perr       ������뷵��ֵ����Χ��ָ��ǿ�
 *
 * @return            ��
 */
void b2v_var_set(void *paddr, uint32_t value, uint8_t size, uint32_t *perr)
{
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);

    /*- ����Ƿ��п�ָ�� */
    if (NULL == paddr)
    {
        /*- ���ô������: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }
    else
    { 
		/*- ���ô������: E_OK */  
	    *perr = (uint32_t)E_OK;

		/*- �������ݵĿ�Ȱ�ֵ����ָ����ַ, ��������Ч�����ô������ */
        switch (size)
        {
			/*- ���Ϊ1 */
            case (1u):
				/*- ���յ��ֽڸ�ֵ */
                *((uint8_t *)paddr) = (uint8_t)value;
                break;
			/*- ���Ϊ2 */
            case (2u):
			    /*- ����2�ֽڸ�ֵ */
                *((uint16_t *)paddr) = (uint16_t)value;
                break;
			/*- ���Ϊ4 */
            case (4u):
			    /*- ����4�ֽڸ�ֵ */
                *((uint32_t *)paddr) = (uint32_t)value;
                break;
			/*- ���Ϊ����ֵ */
            default:
			    /*- ���ô������: E_B2V_SIZE  */
                *perr = (uint32_t)E_B2V_SIZE;
                break;
        }
    }
}

/** 
 * @brief b2v_var_get  ��ָ����ַ��ȡָ����С�ı���ֵ
 *
 * @detail             ��ָ����ַ����ָ���������͵Ŀ�Ƚ������ݵ�ֵ
 *
 * @param paddr        Ŀ���ַ����Χ��ָ��ǿ�
 * @param size         ָ�������ݿ�ȣ���Χ��{1,2,4}
 * @param *perr        �������ָ�룬��Χ��ָ��ǿ�
 *
 * @return             ��������ֵ
 */
uint32_t b2v_var_get(uint8_t paddr[], uint8_t size, uint32_t *perr)
{
    /*- ������ʼ�� */
    uint32_t value = 0u;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- ����Ƿ��п�ָ�� */
    if (NULL == paddr)
    {
        /*- ���ô������: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }
    else
    {   
		/*- ���ô������: E_OK */  
	    *perr = (uint32_t)E_OK;

        /*- ��ָ����ַ�����ݽ�����ָ����ȵ����� */
        switch (size)
        {
			/*- ���Ϊ1 */
            case (1u):
			    /*- ���յ��ֽ�ȡֵ */
                value = (uint32_t)(*((uint8_t *)paddr));
                break;
			/*- ���Ϊ2 */
            case (2u):
			    /*- ����2�ֽ�ȡֵ */
                value = (uint32_t)(*((uint16_t *)paddr));
                break;
			/*- ���Ϊ4 */
            case (4u):
			    /*- ����4�ֽ�ȡֵ */
                value = (uint32_t)(*((uint32_t *)paddr));
                break;
			/*- ���Ϊ����ֵ */
            default:
			    /*- ���ô������: E_B2V_SIZE  */
                *perr = (uint32_t)E_B2V_SIZE;
                break;
        }
    }

    /*- ���ؽ����ı���ֵ */
    return (value);
}

/**
 *@brief b2v_var_get_adapt  ��ָ����ַ��ȡָ����С�ı���ֵ
 *
 *@param paddr  Ŀ���ַ����Χ��ָ��ǿ�
 *@param value  ����ֵ����Χ����Ȼ��Χ
 *@param size   ָ�������ݿ�ȣ���Χ��{1,2,4}
 *@param perr   ������뷵��ֵ����Χ��ָ��ǿ�
 *
 *@return ��������ֵ
 */
uint32_t b2v_var_get_adapt(uint8_t paddr[], uint32_t value, uint8_t size, uint32_t *perr)
{
    /*- ��ָ����ַ��ȡָ����С�ı���ֵ */
	return b2v_var_get(paddr, size, perr);
}

/** 
 * @brief b2v_stream_set_msb ����bit��������ָ�����ݿ�ȵı���ֵ(�ֽ����˰�)
 *
 * @detail �Ӵ�˿�ʼ���ݱ���ֵ�����ݿ�ȣ����������õ���������ָ����ַ
 *
 * @param paddr     Ŀ���ַ����Χ��ָ��ǿ�
 * @param roffset   ƫ��������Χ����Ȼ��Χ
 * @param value     ����ֵ����Χ����Ȼ��Χ
 * @param width     bitλ��ȣ���Χ����Ȼ��Χ
 * @param *perr     �������ָ�룬��Χ��ָ��ǿ�
 *
 * @return          ��
 */ 
void b2v_stream_set_msb(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint32_t *perr)
{
    /*- ������ʼ�� */
    uint16_t i        = 0u  ;
    uint16_t index    = 0u  ;
    uint16_t ref      = 0u  ;
    uint32_t data1    = 0u  ;
    uint8_t  datas[5] = {0u};
    uint16_t len      = 0u  ;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- ����Ƿ��п�ָ�� */
    if (NULL == paddr)
    {
        /*- ���ô������: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }else if((0u == width) || (width > 32u)) /*-< width����Ƿ񳬳���Ч��Χ */
	{
		/*- ���ô������: E_B2V_WIDTH */
		*perr = (uint32_t)E_B2V_WIDTH;
	}
    else
    {
        /*- �����������*/
        data1 = cb_vLshift32(value, (32u - width));
        index = roffset / 8u;
        ref   = roffset % 8u;
        datas[0] = (uint8_t)cb_vRshift32(cb_vRshift32(data1, (uint8_t)ref), (uint8_t)24u);
        datas[1] = (uint8_t)cb_vRshift32(cb_vRshift32(data1, (uint8_t)ref), (uint8_t)16u);
        datas[2] = (uint8_t)cb_vRshift32(cb_vRshift32(data1, (uint8_t)ref), (uint8_t)8u );
        datas[3] = (uint8_t)cb_vRshift32(cb_vRshift32(data1, (uint8_t)ref), (uint8_t)0u );
        datas[4] = (uint8_t)cb_vRshift32(cb_vLshift32(data1, (uint8_t)(32u-ref)), (uint8_t)24u);

        /*- �������ݳ�������ȡ�� */
        len = (ref + width + 7u) / 8u;

        /*- @alias ���ָ����ȵ�ԭ�����е�����*/
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
        /*- @alias ѭ������ǰ����ѹ��Ŀ���ַ */
        for (i = 0u; i < len; i++)
        {
            ((uint8_t *)paddr)[index+i] |= datas[i];
        }

        /*- ���ô������: E_OK */
		*perr = (uint32_t)E_OK;
    }
}

/**
 * @brief b2v_stream_set_lsb ����ָ�����ݿ�ȵı���ֵ��bit����(�ֽ���С�˰�)
 *
 * @detail ��С�˿�ʼ���ݱ���ֵ�����ݿ�ȣ����������õ���������ָ����ַ
 *
 * @param paddr        Ŀ���ַ����Χ��ָ��ǿ�
 * @param roffset      ƫ��������Χ����Ȼ��Χ
 * @param value        ����ֵ����Χ����Ȼ��Χ
 * @param width        bitλ��ȣ���Χ��[1,32]
 * @param *perr        �������ָ�룬��Χ��ָ��ǿ�
 *
 * @return             ��
 */
void b2v_stream_set_lsb(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint32_t *perr)
{
    /*- ������ʼ�� */
    uint32_t value_msb = 0u;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- ����Ƿ��п�ָ�� */
    if (NULL == paddr)
    {
        /*- ���ô������: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }else if((0u == width) || (width > 32u)) /*-< width����Ƿ񳬳���Ч��Χ */
	{
		/*- ���ô������: E_B2V_WIDTH */
		*perr = (uint32_t)E_B2V_WIDTH;
	}
    else
    {
        /*- �����ֽ�����ָ����ȣ�ת��Ϊ���ģʽ */
        switch (width)
        {
			/*- ���Ϊ8 */
            case (8u):
                /*- ��8bitȡֵ */
                value_msb = value; 
                break;
			/*- ���Ϊ16 */
            case (16u):
                /*- ��16bitȡֵ */
                value_msb = (cb_vLshift32(value, 8u)&0x00ff00u) + (cb_vRshift32(value, 8u)&0xffu); 
                break;
			/*- ���Ϊ32 */
            case (32u):
                /*- ��32bitȡֵ */
                value_msb = (cb_vLshift32(value, 24u)&0xff000000u) + (cb_vLshift32(value, 8u)&0x00ff0000u) + (cb_vRshift32(value, 8u)&0xff00u) + (cb_vRshift32(value, 24u)&0xffu);  
                break;
			/*- ���Ϊ����ֵ */
            default:
			    /*- ��8bit����, ֱ����Ϊ�Ǵ�� */
                value_msb = value;
                break;
        }

        /*- ����bit��������ָ�����ݿ�ȵı���ֵ */
        b2v_stream_set_msb(paddr, roffset, value_msb, width, perr);
    }
}

/** 
 * @brief b2v_stream_set ����bit��������ָ�����ݿ�ȵı���ֵ
 *
 * @detail ����ָ���ֽ��� ���ݱ���ֵ�����ݿ�ȣ����������õ���������ָ����ַ
 *
 * @param paddr        Ŀ���ַ����Χ��ָ��ǿ�
 * @param roffset      ƫ��������Χ����Ȼ��Χ
 * @param value        ����ֵ����Χ����Ȼ��Χ
 * @param width        bitλ��ȣ���Χ����Ȼ��Χ
 * @param endian       �ֽ��򣬷�Χ��{0,1}
 * @param *perr        �������ָ�룬��Χ��ָ��ǿ�
 *
 * @return             ��
 */
void b2v_stream_set(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr)
{
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);

    /*- ����Ƿ��п�ָ�� */
    if (NULL == paddr)
    {
        /*- ���ô������: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
	}else
    {
		*perr = (uint32_t)E_OK; /*-< ���ô������: E_OK */
    
        /*- �Ƿ�Ϊ���ģʽ */
        if (UNIFW_BIG_ENDIAN == endian)
        {
            /*- �����ģʽ��ֵ */
            b2v_stream_set_msb(paddr, roffset, value, width, perr);
        }
        else if (UNIFW_LITTLE_ENDIAN == endian) /*-< �Ƿ�ΪС��ģʽ */
        {
            /*- ��С��ģʽ��ֵ */
            b2v_stream_set_lsb(paddr, roffset, value, width, perr);
        }   
        else
        {
            /*- ���ô������: E_B2V_ENDIAN */
            *perr = (uint32_t)E_B2V_ENDIAN;
        }
    }
}
/** 
 * @brief  b2v_stream_get_lsb ��ȡbit����ָ�����ݿ�ȵı���ֵ(�ֽ���С�˰�)
 *
 * @detail ��С�˿�ʼ������ָ���������͵Ŀ�ȴ�bit���ж�ȡ����ֵ
 *
 * @param paddr        Ŀ���ַ����Χ��ָ��ǿ�
 * @param roffset      ƫ��������Χ����Ȼ��Χ
 * @param width        bitλ��ȣ���Χ��[1,32]
 * @param *perr        �������ָ�룬��Χ��ָ��ǿ�
 *
 * @return ��������ֵ
 */
uint32_t b2v_stream_get_lsb(uint8_t paddr[], uint16_t roffset, uint8_t width, uint32_t *perr)
{
   /*- ������ʼ�� */
    uint16_t index = 0u;
    uint8_t offset = 0u;
    uint32_t data  = 0u;

    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- ����Ƿ��п�ָ�� */
    if (paddr == NULL)
    {
        /*- ���ô������: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }
	else if((0u == width) || (width > 32u)) /*-< width������Χ */
	{
		/*- ���ô������: E_B2V_WIDTH */
		*perr = (uint32_t)E_B2V_WIDTH;
	}
    else
    {
        /*- ��ȡ���������ݵ���ʼ�ֽں���ʼλ */
        index  = (uint16_t)(roffset / 8u);
        offset = (uint8_t)(roffset % 8u);
        data   = 0u;

        /*- ����32bit���� */
        data  = (uint32_t)cb_vLshift32(paddr[index]   , 24u + offset);
        data += (uint32_t)cb_vLshift32(paddr[index+1u], 16u + offset);
        data += (uint32_t)cb_vLshift32(paddr[index+2u], 8u + offset );
        data += (uint32_t)cb_vLshift32(paddr[index+3u], offset      );
        data += (uint32_t)cb_vRshift32(paddr[index+4u], 8u - offset );
        /*- ��ȡ�͵�ַλ����Чwidth�������� */
        data = cb_vRshift32(data, (32u - width));

		/*- ���ô������: E_OK */
		*perr = (uint32_t)E_OK;

        /*- �����ֽ�����ָ����ȣ���ȡ��������*/
        switch (width)
        {
			/*- ���Ϊ8 */
            case (8u):
                /*- ��8bitȡֵ */
                data = data; 
                break;
			/*- ���Ϊ16 */
            case (16u):
                /*- ��16bitȡֵ */
                data = (cb_vLshift32(data, 8u)&0x00ff00u) + (cb_vRshift32(data, 8u)&0xffu); 
                break;
			/*- ���Ϊ32 */
            case (32u):
                /*- ��32bitȡֵ */
                data = (cb_vLshift32(data, 24u)&0xff000000u) + (cb_vLshift32(data, 8u)&0x00ff0000u) + (cb_vRshift32(data, 8u)&0xff00u) + (cb_vRshift32(data, 24u)&0xffu);  
                break;
			/*- ���Ϊ����ֵ */
            default:
			    /*- ���մ�˽�ȡ�͵�ַλ����Чwidth�������� */
                data = data;
                break;
        }
    }

    /*- ���ؽ������ */
    return (data);
}

/** 
 * @brief  b2v_stream_get_msb ��ȡbit��������ָ�����ݿ�ȵı���ֵ(�ֽ����˰�)
 *
 * @detail �Ӵ�˿�ʼ������ָ���������͵Ŀ�ȴ�bit���ж�ȡ����ֵ
 *
 * @param paddr        Ŀ���ַ����Χ��ָ��ǿ�
 * @param roffset      ƫ��������Χ����Ȼ��Χ
 * @param width        bitλ��ȣ���Χ��[1,32]
 * @param *perr        �������ָ�룬��Χ��ָ��ǿ�
 *
 * @return ��������ֵ
 */
uint32_t b2v_stream_get_msb(uint8_t paddr[], uint16_t roffset, uint8_t width, uint32_t *perr)
{
    /*- ������ʼ�� */
    uint16_t index = 0u;
    uint8_t offset = 0u;
    uint32_t data  = 0u;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- ����Ƿ��п�ָ�� */
    if (NULL == paddr)
    {
        /*- ���ô������: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }else if((0u == width) || (width > 32u)) /*-< ���width��ȴ��� */
	{
		/*- ���ô������: E_B2V_WIDTH */
		*perr = (uint32_t)E_B2V_WIDTH;
	}
    else
    {
        /*- ��ȡ���������ݵ���ʼ�ֽں���ʼλ */
        index  = (uint16_t)(roffset / 8u);
        offset = (uint8_t)(roffset % 8u);
        data   = 0u;

        /*- ����32bit���� */
        data  = (uint32_t)cb_vLshift32(paddr[index]   , 24u + offset);
        data += (uint32_t)cb_vLshift32(paddr[index+1u], 16u + offset);
        data += (uint32_t)cb_vLshift32(paddr[index+2u], 8u + offset );
        data += (uint32_t)cb_vLshift32(paddr[index+3u], offset      );
        data += (uint32_t)cb_vRshift32(paddr[index+4u], 8u - offset );

        /*- ��ȡ�͵�ַλ����Чwidth�������� */
        data = cb_vRshift32(data, 32u - width);

		/*- ���ù��ϴ���: E_OK */
		*perr = (uint32_t)E_OK;
    }

    /*- ���ؽ������ */
    return (data);
}

/** 
 * @brief  b2v_stream_get ��ȡbit����ָ�����ݿ�ȵı���ֵ
 *
 * @detail ����ָ���ֽ��� ������ָ���������͵Ŀ�ȴ�bit���ж�ȡ����ֵ
 *
 * @param paddr        Ŀ���ַ����Χ��ָ��ǿ�
 * @param roffset      ƫ��������Χ����Ȼ��Χ
 * @param width        bitλ��ȣ���Χ��[1,32]
 * @param endian       �ֽ��򣬷�Χ��{0,1}
 * @param *perr        �������ָ�룬��Χ��ָ��ǿ�
 *
 * @return ʧ�ܷ���0�� �ɹ����ؽ�������ֵ
 */
uint32_t b2v_stream_get(uint8_t paddr[], uint16_t roffset, uint8_t width, uint8_t endian, uint32_t *perr)
{
    /*- ������ʼ�� */
    uint32_t ret = 0u;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- ����Ƿ��п�ָ�� */
    if (NULL == paddr)
    {
        /*- ���ô������: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
	}else
    {
		*perr = (uint32_t)E_OK; /*-< ���ô������: E_OK */
		
        if (UNIFW_BIG_ENDIAN == endian) /*-< �Ƿ���ģʽ */
        {
            /*- ���ô��ģʽȡֵ���� */
            ret = b2v_stream_get_msb(paddr, roffset, width, perr);
        }
        else if (UNIFW_LITTLE_ENDIAN == endian) /*-< �Ƿ�С��ģʽ */
        {
            /*- ����С��ģʽȡֵ���� */
            ret = b2v_stream_get_lsb(paddr, roffset, width, perr);
        }
        else
        {
			/*- ���ô�����룺E_B2V_ENDIAN */
            *perr = (uint32_t)E_B2V_ENDIAN;
        }
    }

    /*- ���ؽ������ */
    return (ret);
}

/* ���� */
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
 * @brief b2v_conv_byoffset ���õ���Э�鱨�Ľ������ؼ����ݽṹ����
 *
 * @detail ������ֵ��������һ��ģ�庯��
 *
 * @param pstream       ������ַ����Χ��ָ��ǿ�
 * @param offset        ƫ��������Χ����Ȼ��Χ
 * @param pvar          �ṹ���ַ, ȫ�ֱ���Ӧʹ��NULL, ���������������ַ����Χ����Ȼ��Χ
 * @param table         ������������Χ��ָ��ǿ�
 * @param var_num       ������Ŀ,��Χ��[1,255]
 * @param *perr         �������ָ�룬��Χ��ָ��ǿ�
 * @param mode 			ģʽ����Χ��{B2VM_B2V, B2VM_V2B}
 * @param stream_fun    ���ڲ���bit��ĺ���������д�ķ�ʽ���ݲ�������Χ����Ȼ��Χ
 * @param var_fun       ���ڲ��������ĺ���������д�ķ�ʽ���ݲ�������Χ����Ȼ��Χ
 *
 * @return �������      ��ǰ�����ɹ���λ��
 */
int16_t b2v_conv_byoffset(
		uint8_t pstream[], uint16_t offset, uint8_t pvar[], 
		const var_item_t table[], uint8_t var_num, uint32_t *perr,
		uint16_t mode, 
		uint32_t (*stream_fun)(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr), 
		uint32_t (*var_fun)(void *paddr, uint32_t value, uint8_t size, uint32_t *perr)
		)
{
    /*- ������ʼ�� */
    int16_t   ret         = 0;
    uint8_t   i           = 0u;
    uint16_t  bits_offset = 0u;
    uint8_t * pdst_var    = NULL;
    uint32_t  value       = 0u;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- ����Ƿ��п�ָ�룬����pdst����ʹ�ÿ�ָ�루ȫ�ֱ����� */
    if((NULL == pstream) || (NULL == table))
    {
        /*- ���ô������: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }else if  (0u == var_num) /*-< ���ñ��С�Ƿ�Ϊ0 */
	{
		/*- ���ô������: E_B2V_VAR_NUM */
		*perr = (uint32_t)E_B2V_VAR_NUM;
	}
    else
    {
		*perr = (uint32_t)E_OK; /*-< ���ô������: E_OK */
        bits_offset = offset; /*-< ��ʼ����λ�� */
		/*- i=0; �������ñ�δ����; i++ */
        for(i=0u; i<var_num; i++)
        {
            /*- ��ȡĿ�ĵ�ַ */
            pdst_var = pvar + table[i].offset_d; 
            /*- @alias ���������õ�����ֵ   */
			if (B2VM_B2V == mode)
			{
				value = stream_fun(pstream, bits_offset, 0, table[i].width, table[i].endian, perr);
			}else{
				value = var_fun(pdst_var, 0, table[i].size, perr);
			}

			/*- ��������ֵ�޴��� */
			if (E_OK == *perr)
			{
				/*- @alias ���ñ���ֵ���洢��ַ */
				if (B2VM_B2V == mode)
				{
					var_fun(pdst_var, value, table[i].size, perr);
				}else{
					stream_fun(pstream, bits_offset, value, table[i].width, table[i].endian, perr);
				}

				/*- ���ñ���ֵ�޴��� */
				if (E_OK == *perr)
                {
                    /*- ��һ����������ƫ���� */
                    bits_offset = bits_offset + table[i].width;
                }else{
                    ; /*-< do_nothing */
                }
            }else{
                ; /*-< do_nothing */
            }

            /*- �������̴��� */
            if (E_OK != *perr)
            {
                break; /*-< �˳����� */
            }
        }

        ret =  (int16_t)bits_offset; /*-< �õ������ɹ���ĵ�ַƫ���� */
    }

    /*- ����ִ�н�� */
    return (ret);
}

/**
 *@brief b2v_stream_get_adapt   ��ȡbit����ָ�����ݿ�ȵı���ֵ
 *
 *@param paddr      Ŀ���ַ����Χ��ָ��ǿ�
 *@param roffset    ƫ��������Χ����Ȼ��Χ
 *@param value      ����ֵ����Χ����Ȼ��Χ
 *@param width      bitλ��ȣ���Χ��[1,32]
 *@param endian     �ֽ��򣬷�Χ��{0,1}
 *@param perr       �������ָ�룬��Χ��ָ��ǿ�
 *
 *@return   ʧ�ܷ���0�� �ɹ����ؽ�������ֵ
 */
uint32_t b2v_stream_get_adapt(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr)
{
    /*- ��ȡbit����ָ�����ݿ�ȵı���ֵ */
	return (b2v_stream_get(paddr, roffset, width, endian, perr));
}


/**
 * @brief b2v_stream_to_var_byoffset ���õ���Э�鱨�Ľ������ؼ����ݽṹ����
 *
 * @detail ��Э�鱨��bit������bitƫ�������������������Ľ����ɹؼ����ݽṹ��
 *
 * @param pstream       ������ַ����Χ��ָ��ǿ�
 * @param offset        ƫ��������Χ����Ȼ��Χ
 * @param pdst          �ṹ���ַ����Χ��ָ��ǿ�
 * @param table         ������������Χ��ָ��ǿ�
 * @param var_num       ������Ŀ,��Χ��[1,255]
 * @param *perr         �������ָ�룬��Χ��ָ��ǿ�
 *
 * @return �������      ��ǰ�����ɹ���λ��
 */
int16_t b2v_stream_to_var_byoffset(uint8_t *pstream, uint16_t offset, uint8_t * pdst, const var_item_t * table, uint8_t var_num, uint32_t *perr)
{
    /*- ���õ���Э�鱨�Ľ������ؼ����ݽṹ���� */
	return b2v_conv_byoffset(
			pstream, offset, pdst, table, var_num, perr,
			B2VM_B2V, b2v_stream_get_adapt, b2v_var_set
			);
}


/**
 * @brief b2v_stream_to_var ���õ���Э����Ϣ�������ؼ����ݽṹ���У�Ĭ��ƫ����Ϊ0
 *
 * @param pstream      ������ַ����Χ��ָ��ǿ�
 * @param offset       ƫ��������Χ����Ȼ��Χ
 * @param pdst         �ṹ���ַ����Χ��ָ��ǿ�
 * @param table        ������������Χ��ָ��ǿ�
 * @param var_num      ������Ŀ,��Χ��[1,255]
 * @param *perr        �������ָ�룬��Χ��ָ��ǿ�
 *
 * @return             ��ǰ�����ɹ���λ��
 */
int16_t b2v_stream_to_var(uint8_t pstream[], void *pdst, const var_item_t * table, uint8_t var_num, uint32_t *perr)
{
	/*- ������ʼ�� */
	int16_t ret = 0;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
	/*- ����Ƿ��п�ָ�� */
	if ((NULL == pstream) || (NULL == pdst) || (NULL == table))
	{
        /*- ���ô������: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }else if(0u == var_num) /*-< ���ñ��СΪ�� */
	{
		/*- ���ô������: E_B2V_VAR_NUM */
		*perr = (uint32_t)E_B2V_VAR_NUM;
	}
    else
	{
		/*- �����²�ִ�к��� */
		ret = b2v_stream_to_var_byoffset(pstream, 0u, pdst, table, var_num, perr);
	}

	/*- ���ؽ�� */
    return (ret);
}

/**
 * @brief b2v_var_to_stream_byoffset �����ݽṹ�����ת��ΪЭ����Ϣ����
 *
 * @detail �����ݽṹ�尴�����������������Э�鱨��bit��
 *
 * @param psrc         Դ���ݽṹ�壬��Χ��ָ��ǿ�
 * @param pstream      Ŀ�����������飬��Χ��ָ��ǿ�
 * @param offset       ƫ��������Χ����Ȼ��Χ
 * @param table        ���ݽ�������Χ��ָ��ǿ�
 * @param var_num      ��Ŀ����, ��Χ��[1,255]
 * @param *perr        �������ָ�룬��Χ��ָ��ǿ�
 *
 * @return             ��ǰ�����ɹ���λ��
 */
int16_t b2v_var_to_stream_byoffset(uint8_t psrc[], uint8_t pstream[], uint16_t offset, const var_item_t table[], uint8_t var_num, uint32_t *perr)
{
    /*- ���õ���Э�鱨�Ľ������ؼ����ݽṹ���� */    
	return b2v_conv_byoffset(
			pstream, offset, psrc, table, var_num, perr,
			B2VM_V2B, b2v_stream_set, b2v_var_get_adapt		
			);
}

/**
 * @brief b2v_var_to_stream �����ݽṹ�����ת��ΪЭ����Ϣ���飬Ĭ��ƫ����Ϊ0
 *
 * @param psrc         Դ���ݽṹ�壬��Χ��ָ��ǿ�
 * @param pstream      Ŀ�����������飬��Χ��ָ��ǿ�
 * @param table        ���ݽ�������Χ��ָ��ǿ�
 * @param var_num      ��Ŀ����,��Χ��[1,255]
 * @param *perr        �������ָ�룬��Χ��ָ��ǿ�
 *
 * @return             ��ǰ�����ɹ���λ��
 */
int16_t b2v_var_to_stream(void *psrc, uint8_t pstream[], const var_item_t * table, uint8_t var_num, uint32_t *perr)
{
	/*- ������ʼ�� */
	int16_t ret = 0;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
	/*- ����Ƿ��п�ָ�� */
	if ((NULL == psrc) || (NULL == pstream) || (NULL == table))
	{
        /*- ���ô������: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }else if(0u == var_num) /*-< �������ñ���Ŀ����Ϊ0 */
	{
		/*- ���ô������: E_B2V_VAR_NUM */
		*perr = (uint32_t)E_B2V_VAR_NUM;
	}
    else
	{
		/*- �����²�ִ�к���ִ�к��� */
        ret = b2v_var_to_stream_byoffset(psrc, pstream, 0u, table, var_num, perr);
	}

	/*- ���ؽ�� */
	return (ret);
}


/** ���ֽڰ汾: �����Ч�� */
/**
 * @brief b2v_bytes_set_lsb ����bit��������ָ�����ݿ�ȵı���ֵ(С�˰�)
 *
 * @detail ���ֽ�Ϊƫ������λ�� ��С�˿�ʼ�����ݱ���ֵ�����ݿ�ȣ����������õ���������ָ����ַ
 *
 * @param paddr        Ŀ���ַ����Χ��ָ��ǿ�
 * @param roffset      ƫ��������Χ����Ȼ��Χ
 * @param value        ����ֵ����Χ����Ȼ��Χ
 * @param width        �ֽ�������Χ��[1,4]
 * @param *perr        �������ָ�룬��Χ��ָ��ǿ�
 *
 * @return  ��
 */ 
void b2v_bytes_set_lsb(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint32_t *perr)
{
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- �������Ƿ��п�ָ�� */
    if(paddr == NULL) 
    {
        /*- ���ô������: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }
    else
    {
		/*- ���ô������: E_OK */
		*perr = (uint32_t)E_OK;

        /*- �������ݿ�����ñ���ֵ��ָ����ַ*/
        switch (width)
        {
			/*- ���Ϊ1 */
            case (1u):
			    /*- ���յ��ֽڸ�ֵ */
                paddr[roffset] = (uint8_t)value; 
                break;
			/*- ���Ϊ2 */
            case (2u):
			    /*- ����2�ֽڸ�ֵ */
                paddr[roffset+0u] = (uint8_t)(cb_vRshift32(value, 0u)&0xffu);
                paddr[roffset+1u] = (uint8_t)(cb_vRshift32(value, 8u)&0xffu);
                break;
			/*- ���Ϊ4 */
            case (4u):
			    /*- ����4�ֽڸ�ֵ */
                paddr[roffset+0u] = (uint8_t)(cb_vRshift32(value, 0u )&0xffu);
                paddr[roffset+1u] = (uint8_t)(cb_vRshift32(value, 8u )&0xffu);
                paddr[roffset+2u] = (uint8_t)(cb_vRshift32(value, 16u)&0xffu);
                paddr[roffset+3u] = (uint8_t)(cb_vRshift32(value, 24u)&0xffu);
                break;
			/*- ���Ϊ����ֵ */
            default:
				/*- ���ô������: E_B2V_WIDTH */
				*perr = (uint32_t)E_B2V_WIDTH;
                break;
        }
    }
}

/** 
 * @brief b2v_bytes_set_msb ����bit��������ָ�����ݿ�ȵı���ֵ(��˰�)
 *
 * @detail ���ֽ�Ϊƫ������λ�� �Ӵ�˿�ʼ�����ݱ���ֵ�����ݿ�ȣ����������õ���������ָ����ַ
 *
 * @param paddr        Ŀ���ַ����Χ��ָ��ǿ�
 * @param roffset      ƫ��������Χ����Ȼ��Χ
 * @param value        ����ֵ����Χ����Ȼ��Χ
 * @param width        �ֽ�������Χ��[1,4]
 * @param *perr        �������ָ�룬��Χ��ָ��ǿ�
 * 
 * @return ��
 */
void b2v_bytes_set_msb(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint32_t *perr)
{
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- �������Ƿ��п�ָ�� */
    if(paddr == NULL)
    {
        /*- ���ô������: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }
    else
    {
		/*- ���ô������: E_OK */
		*perr = (uint32_t)E_OK;

        /*- �������ݿ�����ñ���ֵ��ָ����ַ*/
        switch (width)
        {
			/*- ���Ϊ1 */
            case (1u):
			    /*- ���յ��ֽڸ�ֵ */
                paddr[roffset] = (uint8_t)value;
                break;
			/*- ���Ϊ2 */
            case (2u):
			    /*- ����2�ֽڸ�ֵ(���) */
                paddr[roffset+0u] = (uint8_t)(cb_vRshift32(value, 8u)&0xffu);
                paddr[roffset+1u] = (uint8_t)(cb_vRshift32(value, 0u)&0xffu);
                break;
			/*- ���Ϊ4 */
            case (4u):
			    /*- ����4�ֽڸ�ֵ(���) */
                paddr[roffset+0u] = (uint8_t)(cb_vRshift32(value, 24u)&0xffu);
                paddr[roffset+1u] = (uint8_t)(cb_vRshift32(value, 16u)&0xffu);
                paddr[roffset+2u] = (uint8_t)(cb_vRshift32(value, 8u )&0xffu);
                paddr[roffset+3u] = (uint8_t)(cb_vRshift32(value, 0u )&0xffu);
                break;
			/*- ���Ϊ����ֵ */
            default:
			    /*- ���ô������: E_B2V_WIDTH */
				*perr = (uint32_t)E_B2V_WIDTH;
                break;
        }
    }

}

/** 
 * @brief b2v_bytes_set ����bit��������ָ�����ݿ�ȵı���ֵ(����ָ���ֽ���)
 *
 * @param paddr        Ŀ���ַ����Χ��ָ��ǿ�
 * @param roffset      ƫ��������Χ����Ȼ��Χ
 * @param value        ����ֵ����Χ����Ȼ��Χ
 * @param width        �ֽ�������Χ��[1,4]
 * @param endian       �ֽ��򣬷�Χ��{0,1}
 * @param *perr        �������ָ�룬��Χ��ָ��ǿ�
 *
 * @return ��
 */
void b2v_bytes_set(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr)
{
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
	/*- �Ƿ���ģʽ */
    if (UNIFW_BIG_ENDIAN == endian)
    {
        /*- ���ô��ģʽ��ֵ */
        b2v_bytes_set_msb(paddr, roffset, value, width, perr);
    }
    else if (UNIFW_LITTLE_ENDIAN == endian) /*-< �Ƿ�С��ģʽ */
    {
        /*- ����С��ģʽ���� */
        b2v_bytes_set_lsb(paddr, roffset, value, width, perr);
    }
    else
    {
		/*- ���ô������: E_B2V_ENDIAN */
        *perr = (uint32_t)E_B2V_ENDIAN;
    }
}

/** 
 * @brief b2v_bytes_get_lsb ��ȡbit��������ָ�����ݿ�ȵı���ֵ(С�˰�)
 *
 * @detail ���ֽ�Ϊƫ������λ�� ��С�˿�ʼ������������ָ����ַ�����ݱ������Ϳ�ȶ�ȡ����ֵ
 *
 * @param paddr        Ŀ���ַ����Χ��ָ��ǿ�
 * @param roffset      ƫ��������Χ����Ȼ��Χ
 * @param width        �ֽ�������Χ��{0,1}
 * @param *perr        �������ָ�룬��Χ��ָ��ǿ�
 *
 * @return С�˽����õ��ı���ֵ
 */ 
uint32_t b2v_bytes_get_lsb(uint8_t paddr[], uint16_t roffset, uint8_t width, uint32_t *perr)
{
    /*- ������ʼ�� */
    uint32_t value = 0u;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- ����Ƿ��п�ָ�� */
    if (paddr==NULL)
    {
        /*- ���ô������: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }
    else
    {
		/*- ���ô������: E_OK */
		*perr = (uint32_t)E_OK;

        /*- ���ݿ��ƫ�����ӵ�ַ��ȡ����ֵ */
        switch (width)
        {
			/*- ���Ϊ1 */
            case (1u):
			    /*- ���յ��ֽ�ȡֵ(С��) */
                value = (uint32_t)paddr[roffset];
                break;
			/*- ���Ϊ2 */
            case (2u):
			    /*- ����2�ֽ�ȡֵ(С��) */
                value = (uint32_t)(((uint32_t)cb_vLshift32(paddr[roffset+1u],8u))&0xff00u) + (uint32_t)paddr[roffset];
                break;
			/*- ���Ϊ4 */
            case (4u):
			    /*- ����4�ֽ�ȡֵ(С��) */
                value = (uint32_t)(((uint32_t)cb_vLshift32(paddr[roffset+3u], 24u))&0xff000000u) + (uint32_t)(((uint32_t)cb_vLshift32(paddr[roffset+2U], 16U))&0xff0000u) + 
                        (uint32_t)(((uint32_t)cb_vLshift32(paddr[roffset+1u], 8u))&0xff00u) + (uint32_t)paddr[roffset];
                break;
			/*- ���Ϊ����ֵ */
            default:
			    /*- ���ù��ϴ���: E_B2V_WIDTH */
				*perr = (uint32_t)E_B2V_WIDTH;
                break;
        }
    }

    /*- ���ؽ������ */ 
    return (value);
}

/**
 * @brief  b2v_bytes_get_msb ��ȡbit��������ָ�����ݿ�ȵı���ֵ(��˰�)
 *
 * @detail ���ֽ�Ϊƫ������λ�� �Ӵ�˿�ʼ������������ָ����ַ�����ݱ������Ϳ�ȶ�ȡ����ֵ
 *
 * @param paddr        Ŀ���ַ����Χ��ָ��ǿ�
 * @param roffset      ƫ��������Χ����Ȼ��Χ
 * @param width        �ֽ�������Χ��[1,4]
 * @param *perr        �������ָ�룬��Χ��ָ��ǿ�
 *
 * @return ��˽����õ��ı���ֵ
 */
uint32_t b2v_bytes_get_msb(uint8_t paddr[], uint16_t roffset, uint8_t width, uint32_t *perr)
{
    /*- ������ʼ�� */
    uint32_t value = 0u;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- ����Ƿ��п�ָ�� */
    if (NULL == paddr)
    {
        /*- ���ô������: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }
    else
    {
		/*- ���ô������: E_OK */
		*perr = (uint32_t)E_OK;

        /*- ���ݿ��ƫ�����ӵ�ַ��ȡ����ֵ */
        switch (width)
         {
			/*- ���Ϊ1 */
            case (1u):
			    /*- ���յ��ֽ�ȡֵ(���) */
                value = paddr[roffset];
                break;
			/*- ���Ϊ2 */
            case (2u):
			    /*- ����2�ֽ�ȡֵ(���) */
                value = (uint32_t)(((uint32_t)cb_vLshift32(paddr[roffset], 8u))&0xff00u) + (uint32_t)paddr[roffset+1u];
                break;
			/*- ���Ϊ4 */
            case (4u):
			    /*- ����4�ֽ�ȡֵ(���) */
                value = (uint32_t)(((uint32_t)cb_vLshift32(paddr[roffset], 24u))&0xff000000u) + (uint32_t)(((uint32_t)cb_vLshift32(paddr[roffset+1u], 16u))&0xff0000u) + 
                        (uint32_t)(((uint32_t)cb_vLshift32(paddr[roffset+2u], 8u))&0xff00u) + (uint32_t)paddr[roffset+3u];
                break;
			/*- ���Ϊ����ֵ */
            default:
				/*- ���ô������: E_B2V_WIDTH */
				*perr = (uint32_t)E_B2V_WIDTH;
                break;
         }
    }

    /*- ���ؽ������ */
    return (value);
}

/** 
 * @brief  b2v_bytes_get ��ȡbit��������ָ�����ݿ�ȵı���ֵ(����ָ���ֽ���) *
 * @param paddr        Ŀ���ַ����Χ��ָ��ǿ�
 * @param roffset      ƫ��������Χ����Ȼ��Χ
 * @param width        �ֽ�������Χ��[1,4]
 * @param endian       �ֽ��򣬷�Χ��{0,1}
 * @param *perr        �������ָ�룬��Χ��ָ��ǿ�
 *
 * @return ���ؽ�������ֵ
 */
uint32_t b2v_bytes_get(uint8_t paddr[], uint16_t roffset, uint8_t width, uint8_t endian, uint32_t *perr) {
    /*- ������ʼ�� */
    uint32_t ret = 0u;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    if (UNIFW_BIG_ENDIAN == endian) /*-< �Ƿ���ģʽ */
    {
        /*- ���ô��ģʽȡֵ���� */
        ret = b2v_bytes_get_msb(paddr, roffset, width, perr);
    }
    else if (UNIFW_LITTLE_ENDIAN == endian) /*-< �Ƿ���ģʽ */
    {
        /*- ����С��ģʽȡֵ���� */
        ret = b2v_bytes_get_lsb(paddr, roffset, width, perr);
    }
    else
    {
		/*- ���ô������: E_B2V_ENDIAN */
        *perr = (uint32_t)E_B2V_ENDIAN;
    }
    
    /*- ����ȡִֵ�н�� */
    return (ret);
}

/**
 *@brief b2v_bytes_get_adapt   ��ȡ�ֽ�����ָ�����ݿ�ȵı���ֵ
 *
 *@param paddr      Ŀ���ַ����Χ��ָ��ǿ�
 *@param roffset    ƫ��������Χ����Ȼ��Χ
 *@param value      ����ֵ����Χ����Ȼ��Χ
 *@param width      bitλ��ȣ���Χ��[1,32]
 *@param endian     �ֽ��򣬷�Χ��{0,1}
 *@param perr       �������ָ�룬��Χ��ָ��ǿ�
 *
 *@return   ʧ�ܷ���0�� �ɹ����ؽ�������ֵ
 */
uint32_t b2v_bytes_get_adapt(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr)
{
    /*- ��ȡ�ֽ�����ָ�����ݿ�ȵı���ֵ */
	return b2v_bytes_get(paddr, roffset, width, endian, perr);
}

/**
 * @brief b2v_bytes_to_var_byoffset ���õ���Э����Ϣ�������ؼ����ݽṹ����
 *
 * @detail ���ֽ�Ϊ��λ����Э�鱨����������������������������ؼ����ݽṹ����
 *
 * @param pstream          ������ַ����Χ��ָ��ǿ�
 * @param offset           ƫ��������Χ����Ȼ��Χ
 * @param pdst             �ṹ���ַ����Χ��ָ��ǿ�
 * @param table            ������������Χ��ָ��ǿ�
 * @param var_num          ������Ŀ,��Χ��[1,255]
 * @param *perr            �������ָ�룬��Χ��ָ��ǿ�
 *
 * @return                 ִ�н��������µ��ֽ�ƫ����
 */
int16_t b2v_bytes_to_var_byoffset(uint8_t pstream[], uint16_t offset, uint8_t pdst[], const var_item_t table[], uint8_t var_num, uint32_t *perr)
{
    /*- ���õ���Э�鱨�Ľ������ؼ����ݽṹ���� */    
	return b2v_conv_byoffset(
			pstream, offset, pdst, table, var_num, perr,
			B2VM_B2V, b2v_bytes_get_adapt, b2v_var_set
			);
}

/**
 * @brief b2v_bytes_to_var ���õ���Э����Ϣ�������ؼ����ݽṹ���У�Ĭ��ƫ����Ϊ0
 *
 * @param pstream   ������ַ����Χ��ָ��ǿ�
 * @param pdst      �ṹ���ַ����Χ��ָ��ǿ�
 * @param table     ������������Χ��ָ��ǿ�
 * @param var_num   ������Ŀ,��Χ��[1,255]
 * @param *perr     �������ָ�룬��Χ��ָ��ǿ�
 *
 * @return  ִ�н��������µ��ֽ�ƫ����
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
		/*- δ�����κ�ת�� */
		ret = 0;
	}else{
		/*- ֱ�ӵ�����һ�㺯��, Ĭ��ƫ����Ϊ0 */
		ret = (b2v_bytes_to_var_byoffset(pstream, 0u, pdst, table , var_num, perr));
	}
    
    return ret;
}

/**
 * @brief b2v_var_to_bytes_byoffset �������ṹ��ת��Ϊ����������������ָ��ƫ����
 *
 * @detail ƫ�������ֽ�Ϊ��λ���������ݽ������ṹ��ת��Ϊ������
 *
 * @param psrc          Ŀ�����飬��Χ��ָ��ǿ�
 * @param pstream       ���ݽṹ�壬��Χ��ָ��ǿ�
 * @param offset        ƫ��������Χ����Ȼ��Χ
 * @param table         ���ݽ�������Χ��ָ��ǿ�
 * @param var_num       ��Ŀ����,��Χ��[1,255]
 * @param *perr         �������ָ�룬��Χ��ָ��ǿ�
 *
 * @return  ִ�н��������µ��ֽ�ƫ����
 */
int16_t b2v_var_to_bytes_byoffset(uint8_t psrc[], uint8_t pstream[], uint16_t offset, const var_item_t table[], uint8_t var_num, uint32_t *perr)
{
    /*- ���õ���Э�鱨�Ľ������ؼ����ݽṹ���� */    
	return b2v_conv_byoffset(
			pstream, offset, psrc, table, var_num, perr,
			B2VM_V2B, b2v_bytes_set, b2v_var_get_adapt
			);
}


/**
 * @brief b2v_var_to_bytes ���ؼ�����ת��Ϊ�ֽ�����Ĭ��ƫ����Ϊ0
 *
 * @param psrc          Ŀ�����飬��Χ��ָ��ǿ�
 * @param pstream       ���ݽṹ�壬��Χ��ָ��ǿ�
 * @param table         ���ݽ�������Χ��ָ��ǿ�
 * @param var_num       ��Ŀ����,��Χ��[1,255]
 * @param *perr         �������ָ�룬��Χ��ָ��ǿ�
 *
 * @return ִ�н��������µ��ֽ�ƫ����
 */
int16_t b2v_var_to_bytes(void *psrc, uint8_t pstream[], const var_item_t * table, uint8_t var_num, uint32_t *perr)
{
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
	/*- ֱ�ӵ�����һ�㺯��, Ĭ��ƫ����Ϊ0 */
    return (b2v_var_to_bytes_byoffset(psrc, pstream, 0u, table, var_num, perr));
}

/**
 * @brief b2v_table_width ��������������������ݶ�Ӧ��������������
 * 
 * @param table         ������������Χ��ָ��ǿ�
 * @param var_num       ��Ŀ��������Χ��[1,255]
 * @param *perr         �������ָ�룬��Χ��ָ��ǿ�
 * 
 * @return ����ָ����������������������ۼƳ���
 */
int16_t b2v_table_width(const var_item_t table[], uint8_t var_num, uint32_t *perr)
{
	/*- ������ʼ�� */
    int16_t width = 0;
    uint16_t i = 0u;
    /*- UNIFW_ASSERT */
    UNIFW_ASSERT(NULL != perr);
    
    /*- �������Ƿ��п�ָ�� */
    if (NULL == table)
    {
        /*- ���ô������: E_NULL_PT */
        *perr = (uint32_t)E_NULL_PT;
    }else if (0u == var_num) /*-< ���ñ���Ŀ�����Ƿ�Ϊ0 */
	{
		/*- ���ô������: E_B2V_VAR_NUM */
		*perr = (uint32_t)E_B2V_VAR_NUM;
	}
    else
    {
		/*- ���ô������: E_OK */
		*perr = (uint32_t)E_OK;

		/*- @alias �������ñ�, ͳ�������ۼƿ��ֵ */
        for(i=0u; i<var_num; i++)
        {
			width += (int16_t)table[i].width;
        }
    }
    
	/*- �����ۼƿ��ֵ */
    return (width);
}

/**
 * @brief b2v_iom_get_adapt iom��ȡ����
 *
 * @param paddr Դ����ַ����Χ��ָ��ǿ�
 * @param roffset ��ʼƫ��������Χ����Ȼ��Χ
 * @param value	  ����ֵ����Χ����Ȼ��Χ������������Ч
 * @param width	  ��ȣ���Χ����Ȼ��Χ
 * @param endian  �ֽ��򣬷�Χ��{0,1}������������Ч
 * @param perr    ������룬��Χ��ָ��ǿ�
 *
 * @return ʵ�ʻ�ȡ��ֵ
 */
uint32_t b2v_iom_get_adapt(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr)
{
	/*- ������ʼ�� */
	uint32_t ret=0;
	uint8_t raw=0;

	/*- ��ȡ���������ݵ���ʼ�ֽں���ʼλ */
	uint16_t index  = (uint16_t)(roffset / 8u);
	uint8_t offset = (uint8_t)(roffset % 8u);

	/*- ���������ж� */
    UNIFW_ASSERT(NULL != perr);
    UNIFW_ASSERT(NULL != paddr);
    UNIFW_ASSERT(1u == width);

    /*- ���ô������ΪOK  */
	*perr = E_OK;

	/*- ��ȡԭʼ��ֵ */
	raw = paddr[index];
	/*- ��λ��Ч */
	if ((raw>>offset)&0x00000001)
	{
        /*- ����1 */
		ret = 1;
	}
    else
    {
        ;    /*-< do_nothing  */ 
    }

    /*- ����ʵ�ʻ�ȡ��ֵ  */
	return ret;
}

/**
 * @brief b2v_iom_set iom�����ú���
 *
 * @param paddr Դ����ַ����Χ��ָ��ǿ�
 * @param roffset ��ʼƫ��������Χ����Ȼ��Χ
 * @param value   ����ֵ����Χ����Ȼ��Χ
 * @param width   ��ȣ���Χ����Ȼ��Χ
 * @param endian  �ֽ��򣬷�Χ��{0,1}������������Ч
 * @param perr    ������룬��Χ��ָ��ǿ�
 *
 * @return �̶�����0
 */
uint32_t b2v_iom_set(uint8_t paddr[], uint16_t roffset, uint32_t value, uint8_t width, uint8_t endian, uint32_t *perr)
{
	/*- ��ȡ���������ݵ���ʼ�ֽں���ʼλ */
	uint16_t index  = (uint16_t)(roffset / 8u);
	uint8_t offset = (uint8_t)(roffset % 8u);

	/*- ���������ж� */
    UNIFW_ASSERT(NULL != perr);
    UNIFW_ASSERT(NULL != paddr);
    UNIFW_ASSERT(1u == width);

    /*- ���ô������ΪOK  */
	*perr = E_OK;

	/*- �������ֵ��Ч */
	if (0u != value)
	{
		/*- ����iom  */
        paddr[index] |= (1u<<offset);
	}
    else
    {
        ;    /*-< do_nothing  */ 
    }

    /*- �̶�����0  */
	return 0;
}

/**
 * @brief b2v_iom_to_var ��io���ת���ɽṹ�����
 *
 * @param pstream 	io��λ����Χ��ָ��ǿ�
 * @param pdst		Ŀ��ṹ���������Χ��ָ��ǿ�
 * @param table		b2v������񣬷�Χ��ָ��ǿ�
 * @param var_num	b2v���������Ŀ������Χ����Ȼ��Χ
 * @param perr		������룬��Χ��ָ��ǿ�
 *
 * @return �������      ��ǰ�����ɹ���λ��
 */
int16_t b2v_iom_to_var(uint8_t *pstream, void * pdst, const var_item_t * table, uint8_t var_num, uint32_t *perr)
{
    /*- ���õ���Э�鱨�Ľ������ؼ����ݽṹ���� */    
	return b2v_conv_byoffset(
			pstream, 0u, pdst, table, var_num, perr,
			B2VM_B2V, b2v_iom_get_adapt, b2v_var_set
			);
}

/**
 * @brief b2v_var_to_iom   �ӽṹ�����ת����io���
 *
 * @param psrc		Դ�ṹ���������Χ��ָ��ǿ�
 * @param pstream   io��λ����Χ��ָ��ǿ�
 * @param table     b2v������񣬷�Χ��ָ��ǿ�
 * @param var_num   b2v���������Ŀ������Χ����Ȼ��Χ
 * @param perr      ������룬��Χ��ָ��ǿ�
 *
 * @return �������      ��ǰ�����ɹ���λ��
 */
int16_t b2v_var_to_iom(void * psrc, uint8_t *pstream, const var_item_t * table, uint8_t var_num, uint32_t *perr)
{
    /*- ���õ���Э�鱨�Ľ������ؼ����ݽṹ���� */    
	return b2v_conv_byoffset(
			pstream, 0u, psrc, table, var_num, perr,
			B2VM_V2B, b2v_iom_set, b2v_var_get_adapt
			);
}
