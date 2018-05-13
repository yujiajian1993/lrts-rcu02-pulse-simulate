#ifndef __CRC_H
#define __CRC_H

#ifdef __cplusplus
extern "C"{
#endif

/** @brief cb_crc16_ccitt CCITT CRC16�㷨����ӿں���, Ĭ�ϳ�ֵΪ0
 *  @detail ����ʽ: X16+X12+X5+1����Ӧ��������0x1021, ���λ1����ʾ������Ҫ�ߵ�bitλ������
 *  Name   : "XMODEM"    Alias  : "ZMODEM"    Alias  : "CRC-16/ACORN"
 *  Width  : 16    Poly   : 1021    Init   : 0000    RefIn  : False    RefOut : False    XorOut : 0000    
 *  Check  : 31C3
 *  @param  data ���ݵ�ַ
 *  @param  size ���ݳ���
 */
uint16_t cb_crc16_ccitt(const uint8_t * data, uint32_t size);


/** @brief cb_crc32_ieee8023 IEEE 802.3 CRC32�㷨����ӿں�����Ĭ�ϳ�ֵΪ�����ֵΪ0xFFFFFFFF
 *  @detail ����ʽ: X32+X26+X23+X22+X16+X12+X11+X10+X8+X7+X5+X4+X2+X1+1����Ӧ������0x04C11DB7, ���λ1����ʾ
 *   ���ݺ����������Ҫ��bitλ.��Ӧ������0xEDB88320(��0x04C11DB7�ľ���)
 *   Name   : "CRC-32"    Alias  : "CRC-32/ADCCP"    Alias  : "PKZIP"
 *   Width  : 32    Poly   : 04C11DB7    
 *   Init   : FFFFFFFF    RefIn  : True    RefOut : True    XorOut : FFFFFFFF    
 *   Check  : CBF43926
 *  @param data ���ݵ�ַ
 *  @param size ���ݳ���
 */
uint32_t cb_crc32_ieee8023(const uint8_t * data, uint32_t size);

/** @brief cb_crc32_Castagnoli ��Castagnoli CRC32���㷽ʽ
 *  @detail ����ʽ: 1EDC6F41, ���λ1����ʾ����0x82F63B78 �� 0x1EDC6F41�ľ��� 
 *  @ Ԥ������
 *   Name   : "CRC-32C"    Alias  : "CRC-32/ISCSI"    Alias  : "CRC-32/CASTAGNOLI"
 *   Width  : 32    Poly   : 1EDC6F41    
 *   Init   : FFFFFFFF    RefIn  : True    RefOut : True    XorOut : FFFFFFFF    
 *   Check  : E3069283
 *   ʹ�������� Castagnoli���ñ�����㷽ʽ
 *  @param data ���ݵ�ַ
 *  @param size ���ݳ���
 */
uint32_t cb_crc32_Castagnoli(const uint8_t * data, uint32_t size);


#ifdef __cplusplus
}
#endif

#endif //__CRC_H
