#ifndef  _UNIFW_ERRNO_H
#define  _UNIFW_ERRNO_H

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @file unifw_errno.h
 * @brief define error no
 * @author 003
 * @version 
 * @date 2016-05-25
 */

/* define global err value*/
#define E_OK 			0	/**<  ÎÞ´íÎó */
#define E_ZERO_DIVIDER  1	/**<  ³ýÁã´íÎó */
#define E_OVER_FLOW 	2	/**<  Òç³ö´íÎó */
#define E_INVALID_RANGE 3	/**<  ³¬ÏÞÖÆ */
#define E_MIN 			4	/**<  ×îÐ¡Öµ */
#define E_MAX 			5	/**<  ×î´óÖµ */
#define E_NULL_PT 		6	/**<  ´íÎóÖ¸Õë */
#define E_NOT_FOUND 	7	/**<  ÎÞ·¨ÕÒµ½ */
#define E_NOT_END 		8	/**<  Î´µ½Î²²¿ */
#define E_TYPE_FAULT	9	/**<  ÀàÐÍ´íÎó */
#define E_FULL	        10	/**<  »º³åÂú */
#define E_NOT_FULL	    11	/**<  »º³åÎ´Âú */
#define E_CRC           12  /**<  CRCÖµ´íÎó */
#define E_LOST_CONFIG   13  /**<  ¶ªÊ§ÅäÖÃ */
#define E_INVALID_ADDR  14  /**<  µØÖ·´íÎó */
#define E_EMPTY         15  /**<  »º³åÎª¿Õ */
#define E_RESERVED      18  /**<  »º³åÎª¿Õ */
#define E_BUSY          20  /**<  Éè±¸·±Ã¦ */
#define E_PARAM         21  /**<  ²ÎÊý´íÎó */
#define E_TIMEOUT       22  /**<  ³¬Ê± */
#define E_FPGA_BRIDGE   23  /**<  FPGAÇÅ³õÊ¼»¯Ê§°Ü */
#define E_CPCI_INIT     24  /**<  CPCI³õÊ¼»¯Ê§°Ü */
#define E_9030_INIT     25  /**<  9030³õÊ¼»¯Ê§°Ü */
#define E_C2C_WRITE     26  /**<  CPU·¢ËÍÐ´ÈëÊ§°Ü */
#define E_FORMAT        40  /**<  ¸ñÊ½´íÎó */
#define E_DEFAULT      127  /**<  Ä¬ÈÏµÄ´íÎó´úÂë */

#define E_NET_SOCK_ERR_NONE   20000u

#ifdef __cplusplus
}
#endif

#endif   /*_UNIFW_ERRNO_H*/

