/** 
 * @file pulse_simulate.c
 * @brief 
 * @author yu
 * @date 2016-12-01
 * 
 * $Id$
 */

#include "unifw.h"
#include "base/ring.h"
#include "base/b2v.h"
#include "pulse_simulate.h"

/*- 系统函数头文件 */
#include <termios.h> 
#include <unistd.h>
#include <fcntl.h> 

extern const mod_dscrp_t ver_pulse_simulate;

static int32_t rcu02_port = -1;
static int32_t ate_port   = -1;
static int32_t uart_fd = -1;
static int32_t socket_fd = -1;
struct sockaddr_in rcu02_addr = {0};
struct sockaddr_in ate_addr   = {0};

static ring_t  ate_msg_ring = {0};
static uint8_t ate_msg_buffer[MAX_UDP_LEN*2] = {0};  /*-< 每次最多可收2*512 */

/**
 *@brief ecc_ring_init 初始化接收ECI消息的环形缓冲
 *
 *@param 无
 *
 *@return 无
 */

void ate_msg_ring_init(void)
{
    ring_init(&ate_msg_ring, ate_msg_buffer, (MAX_UDP_LEN*2));
} 


/**
 *@brief 初始化网络地址数据结构
 *
 *@param 
 *
 *@return 
 */

void udp_socket_init(struct sockaddr_in *addr, uint16_t port)
{
    memset(addr, 0, sizeof(struct sockaddr_in));
    addr->sin_family = AF_INET;	
 	addr->sin_addr.s_addr = htonl(INADDR_ANY); 
 	addr->sin_port = htons(port);
}

/**
 *@brief 绑定套接字和网络数据结构
 *
 *@param 
 *
 *@return 
 */

int32_t udp_bind(int32_t socketfd, struct sockaddr_in *addr)
{
    int32_t ret = -1;
    ret = bind(socketfd, (struct sockaddr*)addr, sizeof(struct sockaddr_in));
    return ret;
}

/**
 *@brief UDP接收函数
 *
 *@param 
 *
 *@return 
 */
int32_t udp_recvfrom(int32_t socketfd, uint8_t* buff, struct sockaddr_in *remote_addr)
{
    int32_t len = -1;
    int32_t remote_addr_len = -1;

    remote_addr_len = sizeof(struct sockaddr);

    /*- 阻塞模式 */
    //len = recvfrom(socketfd, buff, MAX_UDP_LEN, 0, (struct sockaddr*)remote_addr, &remote_addr_len);
    /*- 非阻塞模式 */
    len = recvfrom(socketfd, buff, MAX_UDP_LEN, MSG_DONTWAIT, (struct sockaddr*)remote_addr, &remote_addr_len);

    return len;
}

/**
 *@brief UDP发送函数
 *
 *@param 
 *
 *@return 
 */
int32_t udp_send(int32_t socketfd, uint8_t* buff, uint16_t length, struct sockaddr_in *remote_addr)
{
    int32_t len = -1;
    int32_t remote_addr_len = -1;

    remote_addr_len = sizeof(struct sockaddr_in);

    len = sendto(socketfd, buff, length, 0, (struct sockaddr*)remote_addr, remote_addr_len);

    return len;
}

/**
 *@brief 获取配置端口
 *
 *@param 
 *
 *@return 
 */

void cfg_port_get(int32_t argument, char* argv[], int32_t* cfg_port1, int32_t* cfg_port2)
{
    int8_t perr = -1;
    
    if(argument > 2)
	{
		*cfg_port1 = cb_atoi(argv[1], &perr);
		*cfg_port2 = cb_atoi(argv[2], &perr);
	} 
	else
	{
		*cfg_port1 = RCU02_PORT;
		*cfg_port2 = ATE_PORT;
	}
}


/**
 *@brief 打开并初始化一个串口
 *
 *@param 
 *
 *@return 
 */

int32_t uart_open(char * uart_dev)
{
    int32_t uartfd = -1;
	struct termios options = {0};

	uartfd=open(uart_dev,O_RDWR|O_NOCTTY|O_NDELAY);

	tcgetattr(uartfd,&options);
	options.c_cflag |=(CLOCAL|CREAD);
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;  
	options.c_cflag &= ~CRTSCTS;  
	options.c_cflag &= ~CSTOPB;  
	options.c_cflag |=PARENB;
	options.c_cflag &=~PARODD;
	options.c_iflag |=INPCK;
	options.c_iflag &=~ISTRIP;
	options.c_iflag &=~(IXON|IXOFF|IXANY);
	
	options.c_oflag &=~OPOST;
	options.c_lflag &=~(ICANON|ECHO|ECHOE|ISIG);
	options.c_iflag &= ~(INLCR|ICRNL|IGNCR);
	options.c_oflag &= ~(ONLCR|OCRNL);
	cfsetispeed(&options,B115200);
	cfsetospeed(&options,B115200); 

	tcflush(uartfd, TCIFLUSH);
	tcsetattr(uartfd,TCSANOW,&options);

	return uartfd;
}

/**
 *@brief 串口发送数据
 *
 *@param 
 *
 *@return 
 */

int32_t uart_write(int32_t uartfd, uint8_t *data,  uint16_t length)  
{  
    int32_t len = -1;  

    len = write(uartfd, data, length);

    return len;  
}  

/**
 *@brief 串口读取数据
 *
 *@param 
 *
 *@return 
 */

int32_t uart_read(int32_t uartfd, uint8_t *data, uint16_t length)  
{  
    int32_t len = -1;  

    fcntl(uartfd,F_SETFL,FNDELAY);  
    len = read(uartfd, data, length);

    return len;  
}

/**
 *@brief 接收并存储ATE报文
 *
 *@param 
 *
 *@return 
 */

int32_t ate_msg_recv(int32_t socketfd, struct sockaddr_in *remote_addr, ring_t *ring)
{
    int32_t len = -1;
    int32_t ret = 0u;
    uint8_t  ate_data[MAX_UDP_LEN] = {0};

    do
    {
        len = udp_recvfrom(socketfd, ate_data, remote_addr);

        if(len > 0)
        {
            ret += ring_push(ring, ate_data, len);
        }
        else
        {
            ;
        }

    }while(len > 0); /*-< 接收ATE报文，收完为止 */ 
    
    return ret ;
}

/**
 *@brief 获取车辆信息
 *
 *@param 
 *
 *@return 
 */

int16_t train_info_get(uint8_t *ate_data, train_info_t *train_info)
{
    int16_t offset = -1;
    int16_t ret = -1;
    uint32_t perr = 0;
    ssimp_hdr_t ssimp_hdr = {0};

    /*- 解析ATE报文 */
    offset = b2v_bytes_to_var(ate_data, (uint8_t *)&ssimp_hdr, ssimp_hdr_tbl, sizeof(ssimp_hdr_tbl)/sizeof(var_item_t), &perr);

    if((offset == SSIMP_HDR_LEN) && (ssimp_hdr.cmd == GEN_PULSE)) 
    {
        /*- 解析车辆信息 */
        offset = b2v_bytes_to_var(&ate_data[SSIMP_HDR_LEN], (uint8_t *)train_info, train_info_tbl, sizeof(train_info_tbl)/sizeof(var_item_t), &perr);
        
        if(offset == TRAIN_INFO_LEN)
        {
            ret = E_OK;
        }
        else
        {
            printp("\r\ntrain_info_parse err offset:%d",offset);
        }
    }
    else
    {
        printp("\r\nssimp_parse_err , offset:%d cmd:%02x \r\n", offset, ssimp_hdr.cmd);
    }
    /*- 返回解析后应用层报文的长度 */
    return ret;
}

/**
 *@brief 将从ATE获取的车辆信息转换为生成脉冲信息
 *
 *@param 
 *
 *@return 
 */

void pulse_info_gen(train_info_t *train_info, pul_info_t *pul_info)
{
    uint32_t circle  = 0;
    uint32_t ch1_fre = 0;
    uint32_t ch2_fre = 0;
    uint32_t ch3_fre = 0;
    uint32_t ch4_fre = 0;

    /*- 计算车轮周长 */
    circle = PI*(train_info->diameter);
    /*- 计算产生脉冲的频率 */
    if(circle != 0)
    {
        /*- 为了保证后面数据精度，将频率*10倍，后续转化为周期取到数后也需*10*/
        ch1_fre = ((10*train_info->ch1_speed)*(train_info->pul_percycle)*10/circle); /*-< 速度为cm/s,*10转换为mm/s */ 
        ch2_fre = ((10*train_info->ch2_speed)*(train_info->pul_percycle)*10/circle);
        ch3_fre = ((10*train_info->ch3_speed)*(train_info->pul_percycle)*10/circle);
        ch4_fre = ((10*train_info->ch4_speed)*(train_info->pul_percycle)*10/circle);

        printp("\r\nch1_fre:%f,ch2_fre:%f,ch3_fre:%f,ch4_fre:%f",(((double)ch1_fre)/10),(((double)ch2_fre)/10),(((double)ch3_fre)/10),(((double)ch4_fre)/10));

    }
    else
    {
        printp("\r\ntrain_circle:%d",circle);
    }

    if(ch1_fre != 0)
    {
        /*- 计算通道1脉冲的占空比 */
        pul_info->ch1_duty_cycle = (50*1000000)*10/ch1_fre;  /*-< 将频率转换为周期，1000000表示单位为us,50代表占空比为50% */
        /*- 计算通道1脉冲频率 */
        pul_info->ch1_fre = (100*1000000)*10/ch1_fre;  /*-< 用周期(us)*100表示 */
        pul_info->ch1_fre = pul_info->ch1_fre|PUL_ENABLE; /*-< 使能脉冲输出 */ 
    }
    else
    {
        pul_info->ch1_duty_cycle = 0;
        pul_info->ch1_fre = 0;
        printp("\r\nch1_duty_cycle:%d,pul_info->ch1_fre:%d",pul_info->ch1_duty_cycle,pul_info->ch1_fre);
    }

    if(ch2_fre != 0)
    {
        /*- 计算通道2脉冲的占空比 */
        pul_info->ch2_duty_cycle = (50*1000000)*10/ch2_fre;  /*-< 将频率转换为周期，1000000表示单位为us,50代表占空比为50% */
        /*- 计算通道2脉冲频率 */
        pul_info->ch2_fre = (100*1000000)*10/ch2_fre;  /*-< 用周期(us)*100表示 */ 
        pul_info->ch2_fre = pul_info->ch2_fre|PUL_ENABLE; /*-< 使能脉冲输出 */
    }
    else
    {
        pul_info->ch2_duty_cycle = 0;
        pul_info->ch2_fre = 0;
        printp("\r\nch2_duty_cycle:%d,pul_info->ch2_fre:%d",pul_info->ch2_duty_cycle,pul_info->ch2_fre);
    }

    if(ch3_fre != 0)
    {
        /*- 计算通道3脉冲的占空比 */
        pul_info->ch3_duty_cycle = (50*1000000)*10/ch3_fre;  /*-< 将频率转换为周期，1000000表示单位为us,50代表占空比为50% */
        /*- 计算通道3脉冲频率 */
        pul_info->ch3_fre = (100*1000000)*10/ch3_fre;  /*-< 用周期(us)*100表示 */
        pul_info->ch3_fre = pul_info->ch3_fre|PUL_ENABLE; /*-< 使能脉冲输出 */
    }
    else
    {
        pul_info->ch3_duty_cycle = 0;
        pul_info->ch3_fre = 0;
        printp("\r\nch3_duty_cycle:%d,pul_info->ch3_fre:%d",pul_info->ch3_duty_cycle,pul_info->ch3_fre);
    }

    if(ch4_fre != 0)
    {
        /*- 计算通道4脉冲的占空比 */
        pul_info->ch4_duty_cycle = (50*1000000)*10/ch4_fre;  /*-< 将频率转换为周期，1000000表示单位为us,50代表占空比为50% */
        /*- 计算通道4脉冲频率 */
        pul_info->ch4_fre = (100*1000000)*10/ch4_fre;  /*-< 用周期(us)*100表示 */
        pul_info->ch4_fre = pul_info->ch4_fre|PUL_ENABLE; /*-< 使能脉冲输出 */ 
    }
    else
    {
        pul_info->ch4_duty_cycle = 0;
        pul_info->ch4_fre = 0;
        printp("\r\nch4_duty_cycle:%d,pul_info->ch4_fre:%d",pul_info->ch4_duty_cycle,pul_info->ch4_fre);
    }

    if((train_info->ch12_phase_diff > 0) && (train_info->ch12_phase_diff < 180))
    {
        /*- 计算通道1,2的相位差 */
        if(ch1_fre != 0)
        {
            pul_info->ch12_phase_diff = (100*1000000*train_info->ch12_phase_diff)*10/(ch1_fre*180);
        }
        else if(ch2_fre != 0)
        {
            pul_info->ch12_phase_diff = (100*1000000*train_info->ch12_phase_diff)*10/(ch2_fre*180);
        }
        else
        {
            pul_info->ch12_phase_diff = 0;
        }

        if(train_info->ch12_dir == 0XAA)
        {
            pul_info->ch12_phase_diff = pul_info->ch12_phase_diff|PUL_PHASE_DIFF_ENABLE|FORWARD;
        }
        else if(train_info->ch12_dir == 0X55)
        {
            pul_info->ch12_phase_diff = pul_info->ch12_phase_diff|PUL_PHASE_DIFF_ENABLE|BACKWARD;
        }
        else
        {
            printp("\r\ntrain_info->ch12_dir:%02x",train_info->ch12_dir);
        }

    }
    else
    {
        printp("\r\nphase12_diff:%d",pul_info->ch12_phase_diff);
    }

    if((train_info->ch34_phase_diff > 0) && (train_info->ch34_phase_diff < 180))
    {
        /*- 计算通道3,4的相位差 */
        if(ch3_fre != 0)
        {
            pul_info->ch34_phase_diff = (100*1000000*train_info->ch34_phase_diff)*10/(ch3_fre*180);
        }
        else if(ch4_fre != 0)
        {
            pul_info->ch34_phase_diff = (100*1000000*train_info->ch34_phase_diff)*10/(ch4_fre*180);
        }
        else
        {
            pul_info->ch34_phase_diff = 0;
        }
        

        if(train_info->ch34_dir == 0XAA)
        {
            pul_info->ch34_phase_diff = pul_info->ch34_phase_diff|PUL_PHASE_DIFF_ENABLE|FORWARD;
        }
        else if(train_info->ch34_dir == 0X55)
        {
            pul_info->ch34_phase_diff = pul_info->ch34_phase_diff|PUL_PHASE_DIFF_ENABLE|BACKWARD;
        }
        else
        {
            printp("\r\ntrain_info->ch34_dir:%02x",train_info->ch34_dir);
        }

    }
    else
    {
        printp("\r\nphase34_diff:%d",pul_info->ch34_phase_diff);
    }

    /*- 计算通道1,2的脉冲输出个数 */
    if(ch1_fre != 0)
    {
        
        pul_info->ch12_pulse_cnt = ((100*1000000)*10/ch1_fre)|LAST_OUTPUT;  /*-< 脉冲持续输出 */
    }
    else if(ch2_fre != 0)
    {
        pul_info->ch12_pulse_cnt = ((100*1000000)*10/ch2_fre)|LAST_OUTPUT;  /*-< 脉冲持续输出 */
    }
    else
    {
        pul_info->ch12_pulse_cnt = 0|LAST_OUTPUT;
    }

    /*- 计算通道3,4的脉冲输出个数 */
    if(ch3_fre != 0)
    {
        pul_info->ch34_pulse_cnt = ((100*1000000)*10/ch3_fre)|LAST_OUTPUT; /*-< 脉冲持续输出 */  
    }
    else if(ch4_fre != 0)
    {
        pul_info->ch34_pulse_cnt = ((100*1000000)*10/ch4_fre)|LAST_OUTPUT; /*-< 脉冲持续输出 */
    }
    else
    {
        pul_info->ch34_pulse_cnt = 0|LAST_OUTPUT; /*-< 脉冲持续输出 */
    }

    /*- 灯输出控制 */
    pul_info->do_output = 0X8F;
}

/**
 *@brief 上电初始化
 *
 *@param 
 *
 *@return 
 */

void mcu_on_init(void)
{
    int32_t bind_ret = -1;

    /*- 设置ip和端口，初始化网络地址结构 */
    udp_socket_init(&rcu02_addr, rcu02_port);

    /*- rcu02作为服务器端,先初始化socket */
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    printp("\r\nsocket_fd:%d",socket_fd);

    /*- 绑定套接字和网络地址 */
    bind_ret = udp_bind(socket_fd, &rcu02_addr);
    printp("\r\nbind_ret:%d",bind_ret);

    uart_fd = uart_open(UART4_DEVICE);
    printp("\r\nuart_fd:%d",uart_fd);

    ate_msg_ring_init();
}

/**
 *@brief 根据fpga读写指令生成头尾
 *
 *@param 
 *
 *@return 
 */

 void fpga_rw_hdr_tail_gen(uint8_t rw_cmd, fpga_rw_hdr_t *rw_header, fpga_rw_tail_t *rw_tail)
 {
     rw_header->frame_start = 0xAAAAAA00;
     rw_header->cmd         = rw_cmd;
     rw_header->length      = 0x0031;
     rw_header->addr        = 0xFFFF;

     rw_tail->crc16         = 0XFFFF;
     rw_tail->raw_di        = 0X0000;
     rw_tail->frame_end     = 0X00A2;
 }

 /**
 *@brief 读取fpga版本号
 *
 *@param 
 *
 *@return 
 */

 void fpga_ver_get(ver_t *fpga_ver)
 {
    int16_t offset = -1;
    uint8_t w_data[MAX_UDP_LEN] = {0};
    uint8_t r_data[MAX_UDP_LEN] = {0};
    int32_t read_len = -1;
    uint32_t perr = 0;
    fpga_rw_t fpga_rw = {0};

    fpga_rw_hdr_tail_gen(R_FPGA_VER, &fpga_rw.header, &fpga_rw.tail); 

    memset(&fpga_rw.pul_info, 0, sizeof(pul_info_t)); 

    offset = b2v_var_to_bytes(&fpga_rw, w_data, fpga_rw_tbl, sizeof(fpga_rw_tbl)/sizeof(var_item_t), &perr);  

    if(offset == FPGA_RW_LEN)
    {
        uart_write(uart_fd, w_data, FPGA_RW_LEN);
        usleep(30000);
        read_len = uart_read(uart_fd, r_data, FPGA_RW_LEN);

        if(read_len == 4)
        {
            fpga_ver->major = r_data[1];
            fpga_ver->minor = r_data[2];
            fpga_ver->patch = r_data[3];
        }
        else
        {
            printp("\r\nfpga_ver_get:%d",read_len);
        }
    }
    else
    {
        printp("\r\noffset:%d",offset);
    }
    
 }

 /**
  *@brief 打印数组数据
  *
  *@param 
  *
  *@return 
  */

 void printf_array(char* str, uint8_t *data, int32_t len)
 {
    uint16_t i = 0;
    printp("\r\n%s len:%d :\r\n",str,len);
    for(i = 0 ; i < len ; i++)
    {
        printp("%02x ",data[i]);
        if((i + 1)%4  == 0) printp("  ");
        if((i + 1)%16 == 0) printp("\r\n");
    }
    printp("\r\n");
 }

 /**
  *@brief 接收并处理ate消息
  *
  *@param 
  *
  *@return 
  */

 void ate_msg_recv_handler(void)
 {
     int16_t ret = -1;
     int16_t offset = 0;
     uint8_t ate_data[MAX_UDP_LEN] = {0};
     int32_t recv_len = 0;
     uint16_t pop_len  = 0;
     uint32_t perr = 0;
     uint8_t w_data[MAX_UDP_LEN] = {0};
     train_info_t train_info = {0};
     fpga_rw_t fpga_rw = {0};

     recv_len = ate_msg_recv(socket_fd, &ate_addr, &ate_msg_ring);

     if(recv_len > 0)
     {
        while(!ring_is_empty(&ate_msg_ring))
        {
            pop_len = ring_pop(&ate_msg_ring, ate_data, MAX_UDP_LEN);

            printf_array("ate_to_rcu02", ate_data, pop_len);

            if(pop_len == (SSIMP_HDR_LEN + TRAIN_INFO_LEN))
            {
                ret = train_info_get(ate_data, &train_info);
                if(ret == E_OK)
                {
                    pulse_info_gen(&train_info, &fpga_rw.pul_info);

                    fpga_rw_hdr_tail_gen(W_PULSE_CTR, &fpga_rw.header, &fpga_rw.tail);

                    offset = b2v_var_to_bytes(&fpga_rw, w_data, fpga_rw_tbl, sizeof(fpga_rw_tbl)/sizeof(var_item_t), &perr); 
                    
                    if(offset == FPGA_RW_LEN)
                    {
                        printf_array("mcu_to_fpga", w_data, FPGA_RW_LEN);
                        uart_write(uart_fd, w_data, FPGA_RW_LEN);
                    }
                    else
                    {
                        printp("\r\noffset:%d",offset);
                    }
                }
                else
                {
                    printp("\r\ntrain_info_get:%d",ret);
                }
            }
            else
            {
                printp("\r\nring_pop:%d",pop_len);
            }
        }
     }
     else
     {
         ;
     }
 }

 /**
  *@brief 生成ssimp头部
  *
  *@param 
  *
  *@return 
  */
 
 void ssimp_header_gen(ssimp_hdr_t *ssimp_hdr)
 {
    ssimp_hdr->src_type = 0x00;
    ssimp_hdr->src_id   = 0x00;
    ssimp_hdr->dst_type = 0x00;
    ssimp_hdr->dst_id   = 0x00;
    ssimp_hdr->serial   = 0x01;
    ssimp_hdr->pkg_num  = 0x01;
    ssimp_hdr->pkg_len  = 0x14;
    ssimp_hdr->cmd      = 0x31;
    ssimp_hdr->param    = 0x00;
    ssimp_hdr->len      = 0x10;
 }

 /**
  *@brief 向ATE发送脉冲计数
  *
  *@param 
  *
  *@return 
  */

 void ate_msg_send_handler(void)
 {
     int16_t ret = -1;
     int16_t offset = -1;
     int32_t read_len = -1;
     int32_t send_len = -1;
     uint32_t perr = 0;
     uint8_t w_data[MAX_UDP_LEN] = {0};
     uint8_t pulse_data[MAX_UDP_LEN] = {0};
     uint8_t ate_data[MAX_UDP_LEN] = {0};
     fpga_rw_t fpga_rw = {0};
     ssimp_hdr_t ssimp_hdr = {0};

     fpga_rw_hdr_tail_gen(R_PULSE_CNT, &fpga_rw.header, &fpga_rw.tail);

     memset(&fpga_rw.pul_info, 0, sizeof(pul_info_t)); 

     offset = b2v_var_to_bytes(&fpga_rw, w_data, fpga_rw_tbl, sizeof(fpga_rw_tbl)/sizeof(var_item_t), &perr);  

     if(offset == FPGA_RW_LEN)
     {
        uart_write(uart_fd, w_data, FPGA_RW_LEN);
        usleep(30000);
        read_len = uart_read(uart_fd, pulse_data, FPGA_RW_LEN);

        printf_array("fpga_to_mcu", pulse_data, read_len);

        ssimp_header_gen(&ssimp_hdr);

        /*- 解析生成ssimp头 */
        offset = b2v_var_to_bytes(&ssimp_hdr, ate_data, ssimp_hdr_tbl, sizeof(ssimp_hdr_tbl)/sizeof(var_item_t), &perr);

        if((read_len == 16) && (offset == SSIMP_HDR_LEN))
        {
            usr_memcpy(&ate_data[SSIMP_HDR_LEN], pulse_data, 16);
            send_len = udp_send(socket_fd, ate_data, (SSIMP_HDR_LEN + 16), &ate_addr);
            printf_array("rcu02_to_ate", ate_data, send_len);
        }
        else
        {
            printp("\r\npulse_cnt_get:%d",read_len);
        }
    }
    else
    {
        printp("\r\noffset:%d",offset);
    }
    
 }

/**
 *@brief 主函数
 *
 *@param 
 *
 *@return 
 */

int main(int argc , char* argv[])
{
    ver_t fpga_ver = {0};

    /*- 获取配置端口 */
    cfg_port_get(argc, argv, &rcu02_port, &ate_port);

    /*- 上电初始化 */
    mcu_on_init();

    /*- 读取fpga版本 */
    fpga_ver_get(&fpga_ver);

    printp("\r\npulse_simulate_ver:%d.%d.%d;fpga_ver:%d.%d.%d",\
           ver_pulse_simulate.major,ver_pulse_simulate.minor,ver_pulse_simulate.patch,\
           fpga_ver.major,fpga_ver.minor,fpga_ver.patch);
    printp("\r\nrcu02_port:%d ,ate_port:%d",rcu02_port,ate_port);
    
    while(1)
    {
        ate_msg_recv_handler();
        usleep(500000);
        ate_msg_send_handler();
    }

    return 0;
}




