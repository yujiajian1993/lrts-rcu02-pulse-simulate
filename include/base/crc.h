#ifndef __CRC_H
#define __CRC_H

#ifdef __cplusplus
extern "C"{
#endif

/** @brief cb_crc16_ccitt CCITT CRC16算法计算接口函数, 默认初值为0
 *  @detail 多项式: X16+X12+X5+1，对应的数字是0x1021, 最高位1不显示，不需要颠倒bit位的做法
 *  Name   : "XMODEM"    Alias  : "ZMODEM"    Alias  : "CRC-16/ACORN"
 *  Width  : 16    Poly   : 1021    Init   : 0000    RefIn  : False    RefOut : False    XorOut : 0000    
 *  Check  : 31C3
 *  @param  data 数据地址
 *  @param  size 数据长度
 */
uint16_t cb_crc16_ccitt(const uint8_t * data, uint32_t size);


/** @brief cb_crc32_ieee8023 IEEE 802.3 CRC32算法计算接口函数，默认初值为和异或值为0xFFFFFFFF
 *  @detail 多项式: X32+X26+X23+X22+X16+X12+X11+X10+X8+X7+X5+X4+X2+X1+1，对应数字是0x04C11DB7, 最高位1不显示
 *   数据和最后结果均需要反bit位.对应数字是0xEDB88320(即0x04C11DB7的镜像)
 *   Name   : "CRC-32"    Alias  : "CRC-32/ADCCP"    Alias  : "PKZIP"
 *   Width  : 32    Poly   : 04C11DB7    
 *   Init   : FFFFFFFF    RefIn  : True    RefOut : True    XorOut : FFFFFFFF    
 *   Check  : CBF43926
 *  @param data 数据地址
 *  @param size 数据长度
 */
uint32_t cb_crc32_ieee8023(const uint8_t * data, uint32_t size);

/** @brief cb_crc32_Castagnoli 真Castagnoli CRC32计算方式
 *  @detail 多项式: 1EDC6F41, 最高位1不显示采用0x82F63B78 即 0x1EDC6F41的镜像 
 *  @ 预期做法
 *   Name   : "CRC-32C"    Alias  : "CRC-32/ISCSI"    Alias  : "CRC-32/CASTAGNOLI"
 *   Width  : 32    Poly   : 1EDC6F41    
 *   Init   : FFFFFFFF    RefIn  : True    RefOut : True    XorOut : FFFFFFFF    
 *   Check  : E3069283
 *   使用真正的 Castagnoli配置表及其计算方式
 *  @param data 数据地址
 *  @param size 数据长度
 */
uint32_t cb_crc32_Castagnoli(const uint8_t * data, uint32_t size);


#ifdef __cplusplus
}
#endif

#endif //__CRC_H
