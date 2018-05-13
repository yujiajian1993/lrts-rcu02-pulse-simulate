#include "unifw.h"
#include "base/ring.h"
#include "base/codebase.h"

/**
 * @brief ring_init 	环形缓冲初始化函数
 *
 * @param ring   		环形队列缓冲管理数据结构体指针，范围：指针非空
 * @param buffer 		队列缓冲区指针，范围：指针非空
 * @param max_length	队列缓冲区大小，范围：自然范围
 *
 * @return 无
 */
void ring_init(ring_t *ring, uint8_t buffer[], uint32_t max_length)
{
	/*- 指针ring和buffer为空则直接宕机 */
	UNIFW_ASSERT(NULL != ring);
	UNIFW_ASSERT(NULL != buffer);
	
	/*- 初始化环形队列缓冲管理数据结构体 */
	ring->data  = buffer;
	ring->max   = max_length;
	ring->free  = ring->max;
	ring->read  = 0u;
	ring->write = 0u;
	ring->used  = 0u;

	/*- 重置缓存区 */
	usr_memset(ring->data, 0, ring->max);
}

/**
 * @brief ring_push 环形缓冲PUSH函数
 *
 * @param ring   	环形队列缓冲管理数据结构体指针，范围：指针非空
 * @param data   	push进入数据起始地址，范围：指针非空
 * @param length 	数据长度，范围：自然范围
 *
 * @return 		 	实际push进入的数据长度，如果无满足长度的存储区，将会返回0
 */
uint16_t ring_push(ring_t *ring, uint8_t data[], uint16_t length)
{
	/*- 初始化参数 */
	uint16_t ret = 0u;
	uint32_t write = 0u;
	uint16_t ex_length = length + 2u;
	uint32_t iH, iL, iW, iWL1;

	/*- 指针ring和data为空则直接宕机 */
	UNIFW_ASSERT(NULL != ring);
	UNIFW_ASSERT(NULL != data);	

	/*- 缓冲剩余空间是否大于待写入数据长度 */
	if (ring->free >= ex_length)
	{
		write = ring->write; 		 	/*-< 写入起始指针位置 */
		iH    = write;               	/*-< 数据长度高位 */
		iL    = (write+1u)%ring->max; 	/*-< 数据长度低位 */
		iW    = (write+2u)%ring->max; 	/*-< 实际数据存储起始位置 */
		/*- 记录数据长度信息 */
		ring->data[iH] = (uint8_t)((length>>8)&(uint16_t)0xff);
		ring->data[iL] = (uint8_t)((length>>0)&(uint16_t)0xff);

		/*- 判断新入数据长度是否超出缓冲区 */
		if ((iW+length) >= ring->max)
		{
			/*- 超出范围, 分次存储 */
			iWL1 = ring->max - iW; 										
			usr_memcpy(&(ring->data[iW]), data, iWL1); 					
			usr_memcpy(&(ring->data[0]), &data[iWL1], length - iWL1); 	
		}
		else
		{
			/*- 未超出地址范围 */
			usr_memcpy(&(ring->data[iW]), data, (uint32_t)length);
		}

		/*- 更新环形缓冲状态 */
		ring->free = ring->free - ex_length; 		/*-< 根据实际使用长度更新空闲存储区 */
		ring->used = ring->used + ex_length; 		/*-< 根据实际使用长度更新已使用存储区 */
		ring->write = (iW + length)%(ring->max); 	/*-< 读出指针位置后移 */

		ret = length; 	/*-< 实际数据长度 */
	}
	else
	{
		;				/*-< do nothing */
	}

	/*- 返回结果 */
	return (ret);
}

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
uint16_t ring_pop(ring_t *ring, uint8_t data[], uint16_t max_length)
{
	/*- 初始化参数 */  
	uint16_t ret = 0u;
	uint32_t read = 0u;
	uint16_t ex_length, length;
	uint32_t iH, iL, iR, iRL1;

	/*- 指针ring和data为空则直接宕机 */
	UNIFW_ASSERT(NULL != ring);
	UNIFW_ASSERT(NULL != data);	

	/*- 判断是否存在数据元素 */
	if (ring->used > 0u)
	{
		read = ring->read;  			/*-< 读出数据起始位置 */
		iH   = read;					/*-< 数据长度高位 */
		iL   = (read+1u)%(ring->max);	/*-< 数据长度低位 */
		iR   = (read+2u)%(ring->max);	/*-< 实际数据读取起始位置 */

		/*- 待读出数据的长度 */
		length = ((uint16_t)((uint16_t)ring->data[iH]<<8) & (uint16_t)0xFF00) + (uint16_t)ring->data[iL]; 	
		ex_length = length + 2u;

		/*- 本次读取数据长度小于目标存储区数组长度 */
		if (length <= max_length)
		{
			/*- 判断读取数据是否越过数据存储区 */
			if ((iR + length) > ring->max)
			{
				/*- 超出范围, 读取 */
				iRL1 = ring->max - iR; 										
				usr_memsafecpy(data, &(ring->data[iR]), iRL1); 					
				usr_memsafecpy(&data[iRL1], &(ring->data[0]),  length - iRL1); 	
			}
			else
			{
				/*- 未超出地址范围 */
				usr_memsafecpy(data, &(ring->data[iR]), (uint32_t)length);
			}
			ret = length; 			/*-< 实际长度 */
		}
		else
		{
			/*- @alias 清除本次数据的值 */
			if ((iR + length) > ring->max)
			{
				/*- 超出范围, 读取 */
				iRL1 = ring->max - iR; 										
				usr_memset(&(ring->data[iR]), 0, iRL1); 					
				usr_memset(&(ring->data[0]), 0, length - iRL1); 	
			}
			else
			{
				/*- 未超出地址范围 */
				usr_memset(&(ring->data[iR]), 0, (uint32_t)length);
			}
			ret = 0; /*-< 读取数据长度设置为长度溢出标识0xFFFF */
		}

		/*- 清除长度值所在位置为0 */
		ring->data[iH] = 0u;
		ring->data[iL] = 0u;

		/*- 更新环形缓冲状态 */
		ring->free = ring->free + ex_length;
		ring->used = ring->used - ex_length;	
		ring->read = (iR + length)%(ring->max);
	}
	else
	{
		;						/*-< do nothing */
	}

	/*- 返回pop出的数据长度 */
	return (ret);
}

/**
 * @brief ring_push_new 环形缓冲PUSH函数
 *
 * @param ring   	环形队列缓冲管理数据结构体指针，范围：指针非空
 * @param data   	push进入数据起始地址，范围：指针非空
 * @param length 	数据长度，范围：自然范围
 *
 * @return 		 	实际push进入的数据长度，如果无满足长度的存储区，将会返回0
 */
uint16_t ring_push_new(ring_t *ring, uint8_t data[], uint16_t length)
{
	/*- 初始化参数 */
	uint16_t ret = 0u;
	uint32_t write = 0u;
	uint32_t iW, iWL1;

	/*- 指针ring和data为空则直接宕机 */
	UNIFW_ASSERT(NULL != ring);
	UNIFW_ASSERT(NULL != data);	

	/*- 缓冲剩余空间是否大于待写入数据长度 */
	if (ring->free >= length)
	{
		write = ring->write; 		 	/*-< 写入起始指针位置 */
		iW    = write%ring->max; 	/*-< 实际数据存储起始位置 */

		/*- 判断新入数据长度是否超出缓冲区 */
		if ((iW+length) >= ring->max)
		{
			/*- 超出范围, 分次存储 */
			iWL1 = ring->max - iW; 										
			usr_memcpy(&(ring->data[iW]), data, iWL1); 					
			usr_memcpy(&(ring->data[0]), &data[iWL1], length - iWL1); 	
		}
		else
		{
			/*- 未超出地址范围 */
			usr_memcpy(&(ring->data[iW]), data, (uint32_t)length);
		}

		/*- 更新环形缓冲状态 */
		ring->free = ring->free - length; 		/*-< 根据实际使用长度更新空闲存储区 */
		ring->used = ring->used + length; 		/*-< 根据实际使用长度更新已使用存储区 */
		ring->write = (iW + length)%(ring->max); 	/*-< 读出指针位置后移 */

		ret = length; 	/*-< 实际数据长度 */
	}
	else
	{
		;				/*-< do nothing */
	}
	
	/*- 返回结果 */
	return (ret);
}



/**
 * @brief ring_pop_new 	环形缓冲POP函数
 *
 * @detail 				如果缓冲区内数据长度大于目标存储区长度，会将数据越过，并返回实际长度以表示数据超出存储区大小
 *
 * @param ring   		环形队列缓冲管理数据结构体指针，范围：指针非空
 * @param data   		pop数据目标存储起始地址，范围：指针非空
 * @param length 		数据长度，范围：自然范围
 *
 * @return 				pop出的数据长度
 */
uint16_t ring_pop_new(ring_t *ring, uint8_t data[], uint16_t length)
{
	/*- 初始化参数 */  
	uint16_t ret = 0u;
	uint32_t read = 0u;
	uint16_t tmp_len = 0u;
	uint32_t iR, iRL1;

	/*- 指针ring和data为空则直接宕机 */
	UNIFW_ASSERT(NULL != ring);
	UNIFW_ASSERT(NULL != data);	

	/*- 判断是否存在数据元素 */
	if (ring->used > 0u)
	{
		read = ring->read;  			/*-< 读出数据起始位置 */
		iR   = read%(ring->max);	/*-< 实际数据读取起始位置 */

		/*- 本次读取数据长度小于目标存储区数组长度 */
		if (length <= ring->used)
		{
			/*- 判断读取数据是否越过数据存储区 */
			if ((iR + length) > ring->max)
			{
				/*- 超出范围, 读取 */
				iRL1 = ring->max - iR; 										
				usr_memsafecpy(data, &(ring->data[iR]), iRL1);
				usr_memsafecpy(&data[iRL1], &(ring->data[0]),  length - iRL1);
			}
			else
			{
				/*- 未超出地址范围 */
				usr_memsafecpy(data, &(ring->data[iR]), (uint32_t)length);
			}
			tmp_len = length; 			/*-< 实际长度 */
		}
		else
		{
			/*- 禁止读取 */
			tmp_len = 0u; 
		}

		/*- 更新环形缓冲状态 */
		ring->free = ring->free + tmp_len; 		
		ring->used = ring->used - tmp_len; 		
		ring->read = (iR + tmp_len)%(ring->max); 

	}
	else
	{
		;						/*-< do nothing */
	}
	ret = tmp_len;
	/*- 返回pop出的数据长度 */
	return (ret);
}

/**
 * @brief ring_is_empty 	环形缓冲为空判断函数
 *
 * @param ring   			环形队列缓冲管理数据结构体指针，范围：指针非空
 *
 * @return 					队列非空则返回0，否则返回1
 */
uint8_t ring_is_empty(ring_t ring[])
{
	/*- 初始化参数 */	
	uint8_t ret = 0u;

	/*- 判断指针ring是否为非空 */
	if (NULL != ring) 
	{
		/*- 判断环形缓冲ring是否未被使用 */		
		if(ring->used > 0u)
		{
			ret = 0u;		/*-< 返回0 */
		}
		else
		{
			ret = 1u;		/*-< 返回1 */
		}
	}
	else
	{
		;					/*-< do nothing */
	}	

	/*- 返回最终结果 */	
	return ret;		
}

/** 
 * @brief rings_reset 	复位函数
 * 
 * @param r 			ring结构体变量，范围：指针非空
 *
 * @return 				无
 */
void rings_reset(ring_t *r)
{
    /*- 环形队列指针是否非空 */
    if (NULL != r)
    {
        /*- 清空环形队列数据 */
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
 * @brief ring_vote_encode 	环形缓冲区生成表决数据
 * 
 * @param ring 			ring结构体变量，范围：指针非空
 * @param to            目标数据存储数组，范围：指针非空
 * @param max_length    最大长度，范围：自然范围
 *
 * @return 				数据长度
 */
int32_t ring_vote_encode(ring_t *ring, uint8_t to[], uint32_t max_length)
{
	/*- 变量初始化 */
	int32_t ret = -1;
	uint32_t i = 0u;

	/*- 空指针异常处理 */
	UNIFW_ASSERT(ring != NULL);
	UNIFW_ASSERT(to != NULL);

	/*- 如果目标存储数组能够存储足够的数据 */
	if ((ring->used + 22) < max_length)
	{
		/*- ring基本数据赋值 */
		cb_ptr_set_index(to, 4u, ring->read ,  0);
		cb_ptr_set_index(to, 4u, ring->write,  4);
		cb_ptr_set_index(to, 4u, ring->free ,  8);
		cb_ptr_set_index(to, 4u, ring->used , 12);
		cb_ptr_set_index(to, 4u, ring->max  , 16);
		cb_ptr_set_index(to, 2u, ring->unit , 20);

		/*- @alias ring缓冲数据赋值 */
		for (i=0u; i<ring->used; i++)
		{
			to[22+i] = ring->data[(ring->read+i)%ring->max];
		}

		/*- 更新数据长度 */
		ret = ring->used + 22;
	}else{
		/*- 返回错误代码 */
		ret = -1;
	}

	/*- 返回结果 */
	return ret;
}

/** 
 * @brief ring_sync_encode 	环形缓冲区生成同步数据
 * 
 * @param ring 			ring结构体变量，范围：指针非空
 * @param to            目标数据存储数组，范围：指针非空
 * @param max_length    最大长度，范围：自然范围
 *
 * @return 				数据长度
 */
int32_t ring_sync_encode(ring_t *ring, uint8_t to[], uint32_t max_length)
{
	/*- 环形缓冲区生成同步数据 */
	return ring_vote_encode(ring, to, max_length);
}

/** 
 * @brief ring_sync_decode 	同步数据生成环形缓冲区
 * 
 * @param ring 			ring结构体变量，范围：指针非空
 * @param from          同步数据，范围：指针非空
 * @param length        数据长度，范围：自然范围
 *
 * @return 				0: 同步成功   1: 同步失败
 */
int32_t ring_sync_decode(ring_t *ring, uint8_t from[], uint32_t length)
{
	/*- 变量初始化 */
	uint32_t omax = 0u;
	uint32_t oused = 0u;
	uint16_t ounit= 0u;
	uint32_t i = 0u;
	int32_t  ret = -1;

	/*- 空指针处理 */
	UNIFW_ASSERT(ring != NULL);
	UNIFW_ASSERT(from != NULL);

	/*- 获取同步数据中的缓冲区关键参数 */
	omax  = cb_ptr_get_index(from, 4, 16);
	ounit = cb_ptr_get_index(from, 2, 20);
	oused  = cb_ptr_get_index(from, 4u,12);

	/*- 判断是否能够进行同步 */
	if (omax == ring->max && omax > 0 && ounit == ring->unit && (oused+22) == length)
	{
		/*- 同步缓冲区当前状态 */
		ring->read  = cb_ptr_get_index(from, 4,  0);
		ring->write = cb_ptr_get_index(from, 4u, 4);
		ring->free  = cb_ptr_get_index(from, 4u, 8);
		ring->used  = cb_ptr_get_index(from, 4u,12);

		/*- @alias 同步数据元素 */
		for (i=0; i<ring->used; i++)
		{
			ring->data[(ring->read+i)%ring->max] = from[22+i];
		}
		
		/*- 同步成功 */
		ret = 0;
	}else{
		/*- 同步失败 */
		ret = -1;
	}

	return ret;
}


/** 
 * @brief rings_init 	初始化函数
 * 
 * @param r 			ring结构体变量 ，范围：指针非空
 * @param c 			容量（以元素个数计算），范围：自然范围
 * @param unit 			元素宽度，范围：自然范围
 * @param data 			数组指针，范围：指针非空
 *	
 * @return 				无
 */
void rings_init(ring_t *r, uint16_t c, uint16_t unit, void *data)
{
    /*- 环形队列指针ring和指针data是否非空 */	
    if ((NULL != r) && (NULL != data))
    {
	    /*- 设置容量为c */
	    r->max = c;
	    /*- 设置元素宽度为unit */
	    r->unit = unit;
	    /*- 设置数据指针为data */
	    r->data = (uint8_t *)data;
	    /*- 复位r */
	    rings_reset(r);
    }
    else
    {
        ;				/*-< do nothing */
    }	
}

/** 
 * @brief rings_rset 	更新环形缓冲特定元素
 * 
 * @param r 			ring结构体变量，范围：指针非空
 * @param o 			指针偏移量，范围：自然范围
 * @param e 			压入的元素指针，范围：指针非空
 *
 * @return 				无
 */
void rings_rset(ring_t *r, uint16_t o, void *e)
{	
	/*- 判断环形队列结构体指针r和压入元素指针e是否非空 */
    if ((NULL != r) && (NULL != e))
    {
		/*- 实际偏移量 */	
		uint16_t ro = o % (uint16_t)r->max;

		/*- 将e处数值复制到r->data头部 */
		usr_memcpy(&r->data[ro * r->unit], e, (uint32_t)r->unit);
    }
    else
    {
        ;				/*-< do nothing */
    }
}

/**
 * @brief rings_rget 	获取环形缓冲特定元素的指针
 *
 * @param r 			ring结构体变量，范围：指针非空
 * @param o				指针偏移量，范围：自然范围
 *
 * @return 				返回指针
 */
void *rings_rget(ring_t r[], uint16_t o)
{
	/*- 初始化参数 */
	void *ret = NULL;

	/*- 判断环形队列结构体指针r是否为非空 */
	if (NULL != r)
	{
		/*- 实际偏移量 */
		uint16_t ro = o % (uint16_t)r->max;
		/*- 返回元素指针 */
		ret =  (void*)(&r->data[ro * r->unit]);
	}
	else
	{
		;				/*-< do nothing */
	}
	
	/*- 返回指针 */
	return (void *)ret;
}

/** 
 * @brief rings_pushp 	压入指针
 * 
 * @param r 			ring结构体，范围：指针非空
 * @param e 			数据指针，范围：指针非空
 * 
 * @return 				若正常压入则返回0，若r已满则返回-E_FULL
 */
int16_t rings_pushp(ring_t *r, void *e)
{
	/*- 初始化参数 */
    int16_t ret = -E_FULL;

	/*- 判断ring结构体指针r和数据指针e是否为非空 并 判断已使用长度是否在最大长度范围内 */
	if ((NULL != r) && (NULL != e) && (r->used < r->max))
	{ 
		/*- 更新环形缓冲 */
		rings_rset(r, (uint16_t)r->write, e);
		rings_inc_w(r);
		r->used ++;

        ret = 0;			/*-< 压入成功将ret置0 */
	}
	else
	{
		ret = -E_FULL;		/*-< 指针为空或r已满将ret置为-E_FULL */
	}

	/*- 返回结果 */
	return ret; 
} 

/**
 * @brief rings_getp 	获取环形缓冲特定元素的指针（pop用）
 *
 * @param r				ring结构体，范围：指针非空
 * @param offset		指针偏移量，范围：自然范围
 *
 * @return 				环形缓冲特定元素的指针
 */
void * rings_getp(ring_t r[], uint16_t offset)
{
	/*- 初始化参数 */
	void *ret = NULL;

	/*- 判断环形队列结构体指针r是否为非空 */
    if (NULL != r)
    {
		/*- 计算特定元素的指针 */		
		ret = rings_rget(r, r->read+offset);
    }
	else
	{
		;				/*-< do nothing */
	}
	/*- 返回特定元素的指针 */
	return (void*)ret;
}

/** 
 * @brief *rings_wgetp 	获取环形缓冲特定元素的指针（push用）
 * 
 * @param r 			ring结构体，范围：指针非空
 * @param offset 		指针偏移量，范围：自然范围
 * 
 * @return 				环形缓冲特定元素的指针
 */
void * rings_wgetp(ring_t r[], uint16_t offset)
{
	/*- 初始化参数 */	
	void *ret = NULL;
	/*- 判断环形队列结构体指针r是否为非空 */
    if (NULL != r)
    {
		/*- 获取指针 */	
		ret = rings_rget(r, ((uint16_t)r->write + offset) - 1u);
    }
	else
	{
		;				/*-< do nothing */
	}
	/*- 返回特定元素的指针 */
    return (void*)ret;
}

/** 
 * @brief rings_rpopanull 	丢弃一段数据（不返回）
 * 
 * @param r 				ring结构体，范围：指针非空
 * @param len 				待丢弃长度，范围：自然范围
 *
 * @return 					无
 */
void rings_rpopanull(ring_t *r, uint16_t len)
{
	/*- 判断环形队列结构体指针r是否为非空 */
    if (NULL != r)
    {
		/*- 丢弃一段数据 */
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
 * @brief rings_rpopa 		弹出可变长数据长度（不检查越界）
 * 
 * @param r 				ring结构体，范围：指针非空
 * @param dst 				目的地址指针，范围：指针非空
 * @param len 				读取长度，范围：自然范围
 *
 * @return 					无
 */
void rings_rpopa(ring_t *r, uint8_t *FAR dst, uint16_t len)
{
	/*- 判断环形队列结构体指针r是否为非空 */
    if ((NULL != r)&&(NULL != dst))
    {
		/*- pop出指定长度的数据 */	
	    usr_memcpy(dst, &r->data[r->read], (uint32_t)len);
		/*- pop后更新结构体 */
	    rings_rpopanull(r, len);
    }
	else
	{
		;					 /*-< do nothing */
	}
}

/** 
 * @brief rings_pop 		丢弃一个元素
 * 
 * @param r					ring结构体，范围：指针非空
 * 
 * @return 					若正常弹出返回0，否则返回-E_EMPTY	
 * 	
 */
int16_t rings_pop(ring_t *r)
{
	/*- 初始化参数 */
    int16_t ret = -E_EMPTY;
	/*- 判断ring结构体是否为非空 并 判断已使用长度是否不为0 */
	if ((NULL != r) && (0u != r->used))
    { 
		/*- 丢弃元素 */
		r->read++; 
		RINGS_WRAP(r->read);  
		r->used--;

		/*- 丢弃成功将ret置0 */
        ret = 0; 
	}
	else
	{
		/*- 指针为空 或 队列为空将ret置为-E_EMPTY */
		ret = -E_EMPTY;
	}

	/*- 返回结果 */
	return ret; 
}

/** 
 * @brief rings_rpusha 		压入可变长度数组数据（不检查越界）
 * 
 * @param r 				ring结构体，范围：指针非空
 * @param src 				源数据指针，范围：指针非空
 * @param len 				待压入长度，范围：自然长度
 *
 * @return 					无
 */
void rings_rpusha(ring_t *r, uint8_t *FAR src, uint16_t len)
{
	/*- 判断环形队列结构体指针r是否为非空 */
    if (NULL != r)
    {
	    /*- 将src处内容复制到r->data[r->write] */
	    usr_memcpy(&(r->data[r->write]), src, (uint32_t)len);
	    /*- 更新环形缓冲结构体 */
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
 * @brief rings_pusha 		压入数组数据（检查越界等）
 * 
 * @param r 				ring结构体，范围：指针非空
 * @param src 				源地址，范围：指针非空
 * @param len 				待压入数据长度，范围：自然范围
 * 
 * @return 					若压入成功返回压入的数据长度，若环形缓冲已满返回-E_FULL
 */
int16_t  rings_pusha(ring_t *r, void *FAR src, uint16_t len)
{
	/*- 初始化参数 */
	uint16_t len1 = 0u, len2 = 0u;
    int16_t ret = -E_FULL;

	/*- 结构体指针r为非空且有足够的空间来压入数据 */
	if ((NULL != r) && (NULL != src) && (len <= (r->max - r->used)))
	{
		/*- 剩余可压入长度 */
		len1 = (uint16_t)(r->max - r->write);
		/*- @alias 若待压入数据长度小于剩余可压入长度 */
		if (len <= len1)
		{
			len2 = 0u;
			len1 = len;
		}
		else
		{
			/*- 待压入数据多出的长度 */
			len2 = len - len1;
		}
		/*- 先将长度允许范围内的数据部分压入 */
		rings_rpusha(r, (uint8_t * FAR)src, len1);
		/*- 若存在剩余数据 */	
		if (len2 != 0u)
		{
			/*- 将长度为len2的剩余数据部分压入缓冲队列 */
			rings_rpusha(r, &(((uint8_t * FAR)src)[len1]), len2);
		}
		else
		{
			;					 /*-< do nothing */
		}

		/*- 压入成功则ret置为压入的数据长度 */
		ret = (int16_t)len;
	}
	else
	{
		/*- 若指针为空或环形缓冲已满则将ret置为-E_FULL */
		ret = -E_FULL;
	}
	
	/*- 返回结果 */
    return ret;
}

/** 
 * @brief rings_popa 		弹出特定长度的数据（检查越界）
 * 
 * @param r 				ring结构体，范围：指针非空
 * @param dst 				目标地址，范围：指针非空
 * @param len 				待弹出的长度，范围：自然范围
 * 
 * @return 					若弹出成功返回实际弹出的长度，若无足够长度则返回溢出标志-E_OVER_FLOW
 */
int16_t  rings_popa(ring_t *r, void *FAR dst, uint16_t len)
{
	/*- 初始化参数 */
	uint16_t len1 = 0u, len2 = 0u;
    int16_t ret = -E_OVER_FLOW;

	/*- 判断环形队列结构体指针r是否为非空 */
    if ((NULL != r) && (NULL != dst))
    {
		/*- 判断读取长度是否不大于可读长度 */
		if (len <= r->used)
		{
			/*- 计算环形缓冲剩余长度 */
			len1 = (uint16_t)(r->max - r->read);

			/*- 判断数据是否越界 */
			if (len <= len1)
			{
				/*- 未越界将len1设为剩余长度 */
				len2 = 0u;
				len1 = len;
			}
			else
			{
				/*- 越界时将len2设为超出的长度 */
				len2 = len - len1;
			}
			/*- 读取未越界的数据 */
			rings_rpopa(r, (uint8_t * FAR)dst, len1);
			/*- 判断是否存在越界数据 */
			if (len2 != 0u)
			{
				/*- 读取越界的数据 */
				rings_rpopa(r, &(((uint8_t * FAR)dst)[len1]), len2);
			}
			else
			{
				;					 /*-< do nothing */
			}
			/*- 记录数据读取长度 */			
			ret = (int16_t)len;
		}
		else
		{
			/*- 记录数据溢出信息 */
			ret = -E_OVER_FLOW;
		}
    }
	else
	{
		;	 /*-< do nothing */
	}
	
	/*- 返回结果 */
	return ret;
}




