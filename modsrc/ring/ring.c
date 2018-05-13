#include "unifw.h"
#include "base/ring.h"
#include "base/codebase.h"

/**
 * @brief ring_init 	���λ����ʼ������
 *
 * @param ring   		���ζ��л���������ݽṹ��ָ�룬��Χ��ָ��ǿ�
 * @param buffer 		���л�����ָ�룬��Χ��ָ��ǿ�
 * @param max_length	���л�������С����Χ����Ȼ��Χ
 *
 * @return ��
 */
void ring_init(ring_t *ring, uint8_t buffer[], uint32_t max_length)
{
	/*- ָ��ring��bufferΪ����ֱ��崻� */
	UNIFW_ASSERT(NULL != ring);
	UNIFW_ASSERT(NULL != buffer);
	
	/*- ��ʼ�����ζ��л���������ݽṹ�� */
	ring->data  = buffer;
	ring->max   = max_length;
	ring->free  = ring->max;
	ring->read  = 0u;
	ring->write = 0u;
	ring->used  = 0u;

	/*- ���û����� */
	usr_memset(ring->data, 0, ring->max);
}

/**
 * @brief ring_push ���λ���PUSH����
 *
 * @param ring   	���ζ��л���������ݽṹ��ָ�룬��Χ��ָ��ǿ�
 * @param data   	push����������ʼ��ַ����Χ��ָ��ǿ�
 * @param length 	���ݳ��ȣ���Χ����Ȼ��Χ
 *
 * @return 		 	ʵ��push��������ݳ��ȣ���������㳤�ȵĴ洢�������᷵��0
 */
uint16_t ring_push(ring_t *ring, uint8_t data[], uint16_t length)
{
	/*- ��ʼ������ */
	uint16_t ret = 0u;
	uint32_t write = 0u;
	uint16_t ex_length = length + 2u;
	uint32_t iH, iL, iW, iWL1;

	/*- ָ��ring��dataΪ����ֱ��崻� */
	UNIFW_ASSERT(NULL != ring);
	UNIFW_ASSERT(NULL != data);	

	/*- ����ʣ��ռ��Ƿ���ڴ�д�����ݳ��� */
	if (ring->free >= ex_length)
	{
		write = ring->write; 		 	/*-< д����ʼָ��λ�� */
		iH    = write;               	/*-< ���ݳ��ȸ�λ */
		iL    = (write+1u)%ring->max; 	/*-< ���ݳ��ȵ�λ */
		iW    = (write+2u)%ring->max; 	/*-< ʵ�����ݴ洢��ʼλ�� */
		/*- ��¼���ݳ�����Ϣ */
		ring->data[iH] = (uint8_t)((length>>8)&(uint16_t)0xff);
		ring->data[iL] = (uint8_t)((length>>0)&(uint16_t)0xff);

		/*- �ж��������ݳ����Ƿ񳬳������� */
		if ((iW+length) >= ring->max)
		{
			/*- ������Χ, �ִδ洢 */
			iWL1 = ring->max - iW; 										
			usr_memcpy(&(ring->data[iW]), data, iWL1); 					
			usr_memcpy(&(ring->data[0]), &data[iWL1], length - iWL1); 	
		}
		else
		{
			/*- δ������ַ��Χ */
			usr_memcpy(&(ring->data[iW]), data, (uint32_t)length);
		}

		/*- ���»��λ���״̬ */
		ring->free = ring->free - ex_length; 		/*-< ����ʵ��ʹ�ó��ȸ��¿��д洢�� */
		ring->used = ring->used + ex_length; 		/*-< ����ʵ��ʹ�ó��ȸ�����ʹ�ô洢�� */
		ring->write = (iW + length)%(ring->max); 	/*-< ����ָ��λ�ú��� */

		ret = length; 	/*-< ʵ�����ݳ��� */
	}
	else
	{
		;				/*-< do nothing */
	}

	/*- ���ؽ�� */
	return (ret);
}

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
uint16_t ring_pop(ring_t *ring, uint8_t data[], uint16_t max_length)
{
	/*- ��ʼ������ */  
	uint16_t ret = 0u;
	uint32_t read = 0u;
	uint16_t ex_length, length;
	uint32_t iH, iL, iR, iRL1;

	/*- ָ��ring��dataΪ����ֱ��崻� */
	UNIFW_ASSERT(NULL != ring);
	UNIFW_ASSERT(NULL != data);	

	/*- �ж��Ƿ��������Ԫ�� */
	if (ring->used > 0u)
	{
		read = ring->read;  			/*-< ����������ʼλ�� */
		iH   = read;					/*-< ���ݳ��ȸ�λ */
		iL   = (read+1u)%(ring->max);	/*-< ���ݳ��ȵ�λ */
		iR   = (read+2u)%(ring->max);	/*-< ʵ�����ݶ�ȡ��ʼλ�� */

		/*- ���������ݵĳ��� */
		length = ((uint16_t)((uint16_t)ring->data[iH]<<8) & (uint16_t)0xFF00) + (uint16_t)ring->data[iL]; 	
		ex_length = length + 2u;

		/*- ���ζ�ȡ���ݳ���С��Ŀ��洢�����鳤�� */
		if (length <= max_length)
		{
			/*- �ж϶�ȡ�����Ƿ�Խ�����ݴ洢�� */
			if ((iR + length) > ring->max)
			{
				/*- ������Χ, ��ȡ */
				iRL1 = ring->max - iR; 										
				usr_memsafecpy(data, &(ring->data[iR]), iRL1); 					
				usr_memsafecpy(&data[iRL1], &(ring->data[0]),  length - iRL1); 	
			}
			else
			{
				/*- δ������ַ��Χ */
				usr_memsafecpy(data, &(ring->data[iR]), (uint32_t)length);
			}
			ret = length; 			/*-< ʵ�ʳ��� */
		}
		else
		{
			/*- @alias ����������ݵ�ֵ */
			if ((iR + length) > ring->max)
			{
				/*- ������Χ, ��ȡ */
				iRL1 = ring->max - iR; 										
				usr_memset(&(ring->data[iR]), 0, iRL1); 					
				usr_memset(&(ring->data[0]), 0, length - iRL1); 	
			}
			else
			{
				/*- δ������ַ��Χ */
				usr_memset(&(ring->data[iR]), 0, (uint32_t)length);
			}
			ret = 0; /*-< ��ȡ���ݳ�������Ϊ���������ʶ0xFFFF */
		}

		/*- �������ֵ����λ��Ϊ0 */
		ring->data[iH] = 0u;
		ring->data[iL] = 0u;

		/*- ���»��λ���״̬ */
		ring->free = ring->free + ex_length;
		ring->used = ring->used - ex_length;	
		ring->read = (iR + length)%(ring->max);
	}
	else
	{
		;						/*-< do nothing */
	}

	/*- ����pop�������ݳ��� */
	return (ret);
}

/**
 * @brief ring_push_new ���λ���PUSH����
 *
 * @param ring   	���ζ��л���������ݽṹ��ָ�룬��Χ��ָ��ǿ�
 * @param data   	push����������ʼ��ַ����Χ��ָ��ǿ�
 * @param length 	���ݳ��ȣ���Χ����Ȼ��Χ
 *
 * @return 		 	ʵ��push��������ݳ��ȣ���������㳤�ȵĴ洢�������᷵��0
 */
uint16_t ring_push_new(ring_t *ring, uint8_t data[], uint16_t length)
{
	/*- ��ʼ������ */
	uint16_t ret = 0u;
	uint32_t write = 0u;
	uint32_t iW, iWL1;

	/*- ָ��ring��dataΪ����ֱ��崻� */
	UNIFW_ASSERT(NULL != ring);
	UNIFW_ASSERT(NULL != data);	

	/*- ����ʣ��ռ��Ƿ���ڴ�д�����ݳ��� */
	if (ring->free >= length)
	{
		write = ring->write; 		 	/*-< д����ʼָ��λ�� */
		iW    = write%ring->max; 	/*-< ʵ�����ݴ洢��ʼλ�� */

		/*- �ж��������ݳ����Ƿ񳬳������� */
		if ((iW+length) >= ring->max)
		{
			/*- ������Χ, �ִδ洢 */
			iWL1 = ring->max - iW; 										
			usr_memcpy(&(ring->data[iW]), data, iWL1); 					
			usr_memcpy(&(ring->data[0]), &data[iWL1], length - iWL1); 	
		}
		else
		{
			/*- δ������ַ��Χ */
			usr_memcpy(&(ring->data[iW]), data, (uint32_t)length);
		}

		/*- ���»��λ���״̬ */
		ring->free = ring->free - length; 		/*-< ����ʵ��ʹ�ó��ȸ��¿��д洢�� */
		ring->used = ring->used + length; 		/*-< ����ʵ��ʹ�ó��ȸ�����ʹ�ô洢�� */
		ring->write = (iW + length)%(ring->max); 	/*-< ����ָ��λ�ú��� */

		ret = length; 	/*-< ʵ�����ݳ��� */
	}
	else
	{
		;				/*-< do nothing */
	}
	
	/*- ���ؽ�� */
	return (ret);
}



/**
 * @brief ring_pop_new 	���λ���POP����
 *
 * @detail 				��������������ݳ��ȴ���Ŀ��洢�����ȣ��Ὣ����Խ����������ʵ�ʳ����Ա�ʾ���ݳ����洢����С
 *
 * @param ring   		���ζ��л���������ݽṹ��ָ�룬��Χ��ָ��ǿ�
 * @param data   		pop����Ŀ��洢��ʼ��ַ����Χ��ָ��ǿ�
 * @param length 		���ݳ��ȣ���Χ����Ȼ��Χ
 *
 * @return 				pop�������ݳ���
 */
uint16_t ring_pop_new(ring_t *ring, uint8_t data[], uint16_t length)
{
	/*- ��ʼ������ */  
	uint16_t ret = 0u;
	uint32_t read = 0u;
	uint16_t tmp_len = 0u;
	uint32_t iR, iRL1;

	/*- ָ��ring��dataΪ����ֱ��崻� */
	UNIFW_ASSERT(NULL != ring);
	UNIFW_ASSERT(NULL != data);	

	/*- �ж��Ƿ��������Ԫ�� */
	if (ring->used > 0u)
	{
		read = ring->read;  			/*-< ����������ʼλ�� */
		iR   = read%(ring->max);	/*-< ʵ�����ݶ�ȡ��ʼλ�� */

		/*- ���ζ�ȡ���ݳ���С��Ŀ��洢�����鳤�� */
		if (length <= ring->used)
		{
			/*- �ж϶�ȡ�����Ƿ�Խ�����ݴ洢�� */
			if ((iR + length) > ring->max)
			{
				/*- ������Χ, ��ȡ */
				iRL1 = ring->max - iR; 										
				usr_memsafecpy(data, &(ring->data[iR]), iRL1);
				usr_memsafecpy(&data[iRL1], &(ring->data[0]),  length - iRL1);
			}
			else
			{
				/*- δ������ַ��Χ */
				usr_memsafecpy(data, &(ring->data[iR]), (uint32_t)length);
			}
			tmp_len = length; 			/*-< ʵ�ʳ��� */
		}
		else
		{
			/*- ��ֹ��ȡ */
			tmp_len = 0u; 
		}

		/*- ���»��λ���״̬ */
		ring->free = ring->free + tmp_len; 		
		ring->used = ring->used - tmp_len; 		
		ring->read = (iR + tmp_len)%(ring->max); 

	}
	else
	{
		;						/*-< do nothing */
	}
	ret = tmp_len;
	/*- ����pop�������ݳ��� */
	return (ret);
}

/**
 * @brief ring_is_empty 	���λ���Ϊ���жϺ���
 *
 * @param ring   			���ζ��л���������ݽṹ��ָ�룬��Χ��ָ��ǿ�
 *
 * @return 					���зǿ��򷵻�0�����򷵻�1
 */
uint8_t ring_is_empty(ring_t ring[])
{
	/*- ��ʼ������ */	
	uint8_t ret = 0u;

	/*- �ж�ָ��ring�Ƿ�Ϊ�ǿ� */
	if (NULL != ring) 
	{
		/*- �жϻ��λ���ring�Ƿ�δ��ʹ�� */		
		if(ring->used > 0u)
		{
			ret = 0u;		/*-< ����0 */
		}
		else
		{
			ret = 1u;		/*-< ����1 */
		}
	}
	else
	{
		;					/*-< do nothing */
	}	

	/*- �������ս�� */	
	return ret;		
}

/** 
 * @brief rings_reset 	��λ����
 * 
 * @param r 			ring�ṹ���������Χ��ָ��ǿ�
 *
 * @return 				��
 */
void rings_reset(ring_t *r)
{
    /*- ���ζ���ָ���Ƿ�ǿ� */
    if (NULL != r)
    {
        /*- ��ջ��ζ������� */
	    r->write = 0u;
	    r->read  = 0u;
	    r->used  = 0u;
		r->free  = r->max;
    }
    else
    {
        ;				/*-< do nothing */
    }
}

/** 
 * @brief ring_vote_encode 	���λ��������ɱ������
 * 
 * @param ring 			ring�ṹ���������Χ��ָ��ǿ�
 * @param to            Ŀ�����ݴ洢���飬��Χ��ָ��ǿ�
 * @param max_length    ��󳤶ȣ���Χ����Ȼ��Χ
 *
 * @return 				���ݳ���
 */
int32_t ring_vote_encode(ring_t *ring, uint8_t to[], uint32_t max_length)
{
	/*- ������ʼ�� */
	int32_t ret = -1;
	uint32_t i = 0u;

	/*- ��ָ���쳣���� */
	UNIFW_ASSERT(ring != NULL);
	UNIFW_ASSERT(to != NULL);

	/*- ���Ŀ��洢�����ܹ��洢�㹻������ */
	if ((ring->used + 22) < max_length)
	{
		/*- ring�������ݸ�ֵ */
		cb_ptr_set_index(to, 4u, ring->read ,  0);
		cb_ptr_set_index(to, 4u, ring->write,  4);
		cb_ptr_set_index(to, 4u, ring->free ,  8);
		cb_ptr_set_index(to, 4u, ring->used , 12);
		cb_ptr_set_index(to, 4u, ring->max  , 16);
		cb_ptr_set_index(to, 2u, ring->unit , 20);

		/*- @alias ring�������ݸ�ֵ */
		for (i=0u; i<ring->used; i++)
		{
			to[22+i] = ring->data[(ring->read+i)%ring->max];
		}

		/*- �������ݳ��� */
		ret = ring->used + 22;
	}else{
		/*- ���ش������ */
		ret = -1;
	}

	/*- ���ؽ�� */
	return ret;
}

/** 
 * @brief ring_sync_encode 	���λ���������ͬ������
 * 
 * @param ring 			ring�ṹ���������Χ��ָ��ǿ�
 * @param to            Ŀ�����ݴ洢���飬��Χ��ָ��ǿ�
 * @param max_length    ��󳤶ȣ���Χ����Ȼ��Χ
 *
 * @return 				���ݳ���
 */
int32_t ring_sync_encode(ring_t *ring, uint8_t to[], uint32_t max_length)
{
	/*- ���λ���������ͬ������ */
	return ring_vote_encode(ring, to, max_length);
}

/** 
 * @brief ring_sync_decode 	ͬ���������ɻ��λ�����
 * 
 * @param ring 			ring�ṹ���������Χ��ָ��ǿ�
 * @param from          ͬ�����ݣ���Χ��ָ��ǿ�
 * @param length        ���ݳ��ȣ���Χ����Ȼ��Χ
 *
 * @return 				0: ͬ���ɹ�   1: ͬ��ʧ��
 */
int32_t ring_sync_decode(ring_t *ring, uint8_t from[], uint32_t length)
{
	/*- ������ʼ�� */
	uint32_t omax = 0u;
	uint32_t oused = 0u;
	uint16_t ounit= 0u;
	uint32_t i = 0u;
	int32_t  ret = -1;

	/*- ��ָ�봦�� */
	UNIFW_ASSERT(ring != NULL);
	UNIFW_ASSERT(from != NULL);

	/*- ��ȡͬ�������еĻ������ؼ����� */
	omax  = cb_ptr_get_index(from, 4, 16);
	ounit = cb_ptr_get_index(from, 2, 20);
	oused  = cb_ptr_get_index(from, 4u,12);

	/*- �ж��Ƿ��ܹ�����ͬ�� */
	if (omax == ring->max && omax > 0 && ounit == ring->unit && (oused+22) == length)
	{
		/*- ͬ����������ǰ״̬ */
		ring->read  = cb_ptr_get_index(from, 4,  0);
		ring->write = cb_ptr_get_index(from, 4u, 4);
		ring->free  = cb_ptr_get_index(from, 4u, 8);
		ring->used  = cb_ptr_get_index(from, 4u,12);

		/*- @alias ͬ������Ԫ�� */
		for (i=0; i<ring->used; i++)
		{
			ring->data[(ring->read+i)%ring->max] = from[22+i];
		}
		
		/*- ͬ���ɹ� */
		ret = 0;
	}else{
		/*- ͬ��ʧ�� */
		ret = -1;
	}

	return ret;
}


/** 
 * @brief rings_init 	��ʼ������
 * 
 * @param r 			ring�ṹ����� ����Χ��ָ��ǿ�
 * @param c 			��������Ԫ�ظ������㣩����Χ����Ȼ��Χ
 * @param unit 			Ԫ�ؿ�ȣ���Χ����Ȼ��Χ
 * @param data 			����ָ�룬��Χ��ָ��ǿ�
 *	
 * @return 				��
 */
void rings_init(ring_t *r, uint16_t c, uint16_t unit, void *data)
{
    /*- ���ζ���ָ��ring��ָ��data�Ƿ�ǿ� */	
    if ((NULL != r) && (NULL != data))
    {
	    /*- ��������Ϊc */
	    r->max = c;
	    /*- ����Ԫ�ؿ��Ϊunit */
	    r->unit = unit;
	    /*- ��������ָ��Ϊdata */
	    r->data = (uint8_t *)data;
	    /*- ��λr */
	    rings_reset(r);
    }
    else
    {
        ;				/*-< do nothing */
    }	
}

/** 
 * @brief rings_rset 	���»��λ����ض�Ԫ��
 * 
 * @param r 			ring�ṹ���������Χ��ָ��ǿ�
 * @param o 			ָ��ƫ��������Χ����Ȼ��Χ
 * @param e 			ѹ���Ԫ��ָ�룬��Χ��ָ��ǿ�
 *
 * @return 				��
 */
void rings_rset(ring_t *r, uint16_t o, void *e)
{	
	/*- �жϻ��ζ��нṹ��ָ��r��ѹ��Ԫ��ָ��e�Ƿ�ǿ� */
    if ((NULL != r) && (NULL != e))
    {
		/*- ʵ��ƫ���� */	
		uint16_t ro = o % (uint16_t)r->max;

		/*- ��e����ֵ���Ƶ�r->dataͷ�� */
		usr_memcpy(&r->data[ro * r->unit], e, (uint32_t)r->unit);
    }
    else
    {
        ;				/*-< do nothing */
    }
}

/**
 * @brief rings_rget 	��ȡ���λ����ض�Ԫ�ص�ָ��
 *
 * @param r 			ring�ṹ���������Χ��ָ��ǿ�
 * @param o				ָ��ƫ��������Χ����Ȼ��Χ
 *
 * @return 				����ָ��
 */
void *rings_rget(ring_t r[], uint16_t o)
{
	/*- ��ʼ������ */
	void *ret = NULL;

	/*- �жϻ��ζ��нṹ��ָ��r�Ƿ�Ϊ�ǿ� */
	if (NULL != r)
	{
		/*- ʵ��ƫ���� */
		uint16_t ro = o % (uint16_t)r->max;
		/*- ����Ԫ��ָ�� */
		ret =  (void*)(&r->data[ro * r->unit]);
	}
	else
	{
		;				/*-< do nothing */
	}
	
	/*- ����ָ�� */
	return (void *)ret;
}

/** 
 * @brief rings_pushp 	ѹ��ָ��
 * 
 * @param r 			ring�ṹ�壬��Χ��ָ��ǿ�
 * @param e 			����ָ�룬��Χ��ָ��ǿ�
 * 
 * @return 				������ѹ���򷵻�0����r�����򷵻�-E_FULL
 */
int16_t rings_pushp(ring_t *r, void *e)
{
	/*- ��ʼ������ */
    int16_t ret = -E_FULL;

	/*- �ж�ring�ṹ��ָ��r������ָ��e�Ƿ�Ϊ�ǿ� �� �ж���ʹ�ó����Ƿ�����󳤶ȷ�Χ�� */
	if ((NULL != r) && (NULL != e) && (r->used < r->max))
	{ 
		/*- ���»��λ��� */
		rings_rset(r, (uint16_t)r->write, e);
		rings_inc_w(r);
		r->used ++;

        ret = 0;			/*-< ѹ��ɹ���ret��0 */
	}
	else
	{
		ret = -E_FULL;		/*-< ָ��Ϊ�ջ�r������ret��Ϊ-E_FULL */
	}

	/*- ���ؽ�� */
	return ret; 
} 

/**
 * @brief rings_getp 	��ȡ���λ����ض�Ԫ�ص�ָ�루pop�ã�
 *
 * @param r				ring�ṹ�壬��Χ��ָ��ǿ�
 * @param offset		ָ��ƫ��������Χ����Ȼ��Χ
 *
 * @return 				���λ����ض�Ԫ�ص�ָ��
 */
void * rings_getp(ring_t r[], uint16_t offset)
{
	/*- ��ʼ������ */
	void *ret = NULL;

	/*- �жϻ��ζ��нṹ��ָ��r�Ƿ�Ϊ�ǿ� */
    if (NULL != r)
    {
		/*- �����ض�Ԫ�ص�ָ�� */		
		ret = rings_rget(r, r->read+offset);
    }
	else
	{
		;				/*-< do nothing */
	}
	/*- �����ض�Ԫ�ص�ָ�� */
	return (void*)ret;
}

/** 
 * @brief *rings_wgetp 	��ȡ���λ����ض�Ԫ�ص�ָ�루push�ã�
 * 
 * @param r 			ring�ṹ�壬��Χ��ָ��ǿ�
 * @param offset 		ָ��ƫ��������Χ����Ȼ��Χ
 * 
 * @return 				���λ����ض�Ԫ�ص�ָ��
 */
void * rings_wgetp(ring_t r[], uint16_t offset)
{
	/*- ��ʼ������ */	
	void *ret = NULL;
	/*- �жϻ��ζ��нṹ��ָ��r�Ƿ�Ϊ�ǿ� */
    if (NULL != r)
    {
		/*- ��ȡָ�� */	
		ret = rings_rget(r, ((uint16_t)r->write + offset) - 1u);
    }
	else
	{
		;				/*-< do nothing */
	}
	/*- �����ض�Ԫ�ص�ָ�� */
    return (void*)ret;
}

/** 
 * @brief rings_rpopanull 	����һ�����ݣ������أ�
 * 
 * @param r 				ring�ṹ�壬��Χ��ָ��ǿ�
 * @param len 				���������ȣ���Χ����Ȼ��Χ
 *
 * @return 					��
 */
void rings_rpopanull(ring_t *r, uint16_t len)
{
	/*- �жϻ��ζ��нṹ��ָ��r�Ƿ�Ϊ�ǿ� */
    if (NULL != r)
    {
		/*- ����һ������ */
	    r->read += len; 
	    RINGS_WRAP(r->read); 
		if (r->used <= len)
		{
			r->used = 0;
		}
		else
		{
	    	r->used -= len;
		}
    }
	else
	{
		;					 /*-< do nothing */
	} 
}

/** 
 * @brief rings_rpopa 		�����ɱ䳤���ݳ��ȣ������Խ�磩
 * 
 * @param r 				ring�ṹ�壬��Χ��ָ��ǿ�
 * @param dst 				Ŀ�ĵ�ַָ�룬��Χ��ָ��ǿ�
 * @param len 				��ȡ���ȣ���Χ����Ȼ��Χ
 *
 * @return 					��
 */
void rings_rpopa(ring_t *r, uint8_t *FAR dst, uint16_t len)
{
	/*- �жϻ��ζ��нṹ��ָ��r�Ƿ�Ϊ�ǿ� */
    if ((NULL != r)&&(NULL != dst))
    {
		/*- pop��ָ�����ȵ����� */	
	    usr_memcpy(dst, &r->data[r->read], (uint32_t)len);
		/*- pop����½ṹ�� */
	    rings_rpopanull(r, len);
    }
	else
	{
		;					 /*-< do nothing */
	}
}

/** 
 * @brief rings_pop 		����һ��Ԫ��
 * 
 * @param r					ring�ṹ�壬��Χ��ָ��ǿ�
 * 
 * @return 					��������������0�����򷵻�-E_EMPTY	
 * 	
 */
int16_t rings_pop(ring_t *r)
{
	/*- ��ʼ������ */
    int16_t ret = -E_EMPTY;
	/*- �ж�ring�ṹ���Ƿ�Ϊ�ǿ� �� �ж���ʹ�ó����Ƿ�Ϊ0 */
	if ((NULL != r) && (0u != r->used))
    { 
		/*- ����Ԫ�� */
		r->read++; 
		RINGS_WRAP(r->read);  
		r->used--;

		/*- �����ɹ���ret��0 */
        ret = 0; 
	}
	else
	{
		/*- ָ��Ϊ�� �� ����Ϊ�ս�ret��Ϊ-E_EMPTY */
		ret = -E_EMPTY;
	}

	/*- ���ؽ�� */
	return ret; 
}

/** 
 * @brief rings_rpusha 		ѹ��ɱ䳤���������ݣ������Խ�磩
 * 
 * @param r 				ring�ṹ�壬��Χ��ָ��ǿ�
 * @param src 				Դ����ָ�룬��Χ��ָ��ǿ�
 * @param len 				��ѹ�볤�ȣ���Χ����Ȼ����
 *
 * @return 					��
 */
void rings_rpusha(ring_t *r, uint8_t *FAR src, uint16_t len)
{
	/*- �жϻ��ζ��нṹ��ָ��r�Ƿ�Ϊ�ǿ� */
    if (NULL != r)
    {
	    /*- ��src�����ݸ��Ƶ�r->data[r->write] */
	    usr_memcpy(&(r->data[r->write]), src, (uint32_t)len);
	    /*- ���»��λ���ṹ�� */
	    r->write += len; 
	    RINGS_WRAP(r->write); 
	    r->used += len;
    }
	else
	{
		;					 /*-< do nothing */
	} 
}

/** 
 * @brief rings_pusha 		ѹ���������ݣ����Խ��ȣ�
 * 
 * @param r 				ring�ṹ�壬��Χ��ָ��ǿ�
 * @param src 				Դ��ַ����Χ��ָ��ǿ�
 * @param len 				��ѹ�����ݳ��ȣ���Χ����Ȼ��Χ
 * 
 * @return 					��ѹ��ɹ�����ѹ������ݳ��ȣ������λ�����������-E_FULL
 */
int16_t  rings_pusha(ring_t *r, void *FAR src, uint16_t len)
{
	/*- ��ʼ������ */
	uint16_t len1 = 0u, len2 = 0u;
    int16_t ret = -E_FULL;

	/*- �ṹ��ָ��rΪ�ǿ������㹻�Ŀռ���ѹ������ */
	if ((NULL != r) && (NULL != src) && (len <= (r->max - r->used)))
	{
		/*- ʣ���ѹ�볤�� */
		len1 = (uint16_t)(r->max - r->write);
		/*- @alias ����ѹ�����ݳ���С��ʣ���ѹ�볤�� */
		if (len <= len1)
		{
			len2 = 0u;
			len1 = len;
		}
		else
		{
			/*- ��ѹ�����ݶ���ĳ��� */
			len2 = len - len1;
		}
		/*- �Ƚ���������Χ�ڵ����ݲ���ѹ�� */
		rings_rpusha(r, (uint8_t * FAR)src, len1);
		/*- ������ʣ������ */	
		if (len2 != 0u)
		{
			/*- ������Ϊlen2��ʣ�����ݲ���ѹ�뻺����� */
			rings_rpusha(r, &(((uint8_t * FAR)src)[len1]), len2);
		}
		else
		{
			;					 /*-< do nothing */
		}

		/*- ѹ��ɹ���ret��Ϊѹ������ݳ��� */
		ret = (int16_t)len;
	}
	else
	{
		/*- ��ָ��Ϊ�ջ��λ���������ret��Ϊ-E_FULL */
		ret = -E_FULL;
	}
	
	/*- ���ؽ�� */
    return ret;
}

/** 
 * @brief rings_popa 		�����ض����ȵ����ݣ����Խ�磩
 * 
 * @param r 				ring�ṹ�壬��Χ��ָ��ǿ�
 * @param dst 				Ŀ���ַ����Χ��ָ��ǿ�
 * @param len 				�������ĳ��ȣ���Χ����Ȼ��Χ
 * 
 * @return 					�������ɹ�����ʵ�ʵ����ĳ��ȣ������㹻�����򷵻������־-E_OVER_FLOW
 */
int16_t  rings_popa(ring_t *r, void *FAR dst, uint16_t len)
{
	/*- ��ʼ������ */
	uint16_t len1 = 0u, len2 = 0u;
    int16_t ret = -E_OVER_FLOW;

	/*- �жϻ��ζ��нṹ��ָ��r�Ƿ�Ϊ�ǿ� */
    if ((NULL != r) && (NULL != dst))
    {
		/*- �ж϶�ȡ�����Ƿ񲻴��ڿɶ����� */
		if (len <= r->used)
		{
			/*- ���㻷�λ���ʣ�೤�� */
			len1 = (uint16_t)(r->max - r->read);

			/*- �ж������Ƿ�Խ�� */
			if (len <= len1)
			{
				/*- δԽ�罫len1��Ϊʣ�೤�� */
				len2 = 0u;
				len1 = len;
			}
			else
			{
				/*- Խ��ʱ��len2��Ϊ�����ĳ��� */
				len2 = len - len1;
			}
			/*- ��ȡδԽ������� */
			rings_rpopa(r, (uint8_t * FAR)dst, len1);
			/*- �ж��Ƿ����Խ������ */
			if (len2 != 0u)
			{
				/*- ��ȡԽ������� */
				rings_rpopa(r, &(((uint8_t * FAR)dst)[len1]), len2);
			}
			else
			{
				;					 /*-< do nothing */
			}
			/*- ��¼���ݶ�ȡ���� */			
			ret = (int16_t)len;
		}
		else
		{
			/*- ��¼���������Ϣ */
			ret = -E_OVER_FLOW;
		}
    }
	else
	{
		;	 /*-< do nothing */
	}
	
	/*- ���ؽ�� */
	return ret;
}




