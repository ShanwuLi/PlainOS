#include "early_uart.h"


//定义全局this指针,主要用于中断服务函数
static volatile struct _usart *volatile global_this = NULL;


//12:osc  10:ubrr
static const u16_t ubrr_tbl[12][10]={
/**-----------------------------*/
{
    25,12,6,3,2,1,1,0,0,0
},
{
    47,23,11,7,5,3,2,1,1,0
},
{
    51,25,12,8,6,3,2,1,1,0
},
/**-----------------------------*/
{
    95,47,23,15,11,7,5,3,2,1
},
{
    103,51,25,16,12,8,6,3,2,1
},
{
    191,95,47,31,23,15,11,7,5,3,
},
/**-----------------------------*/
{
    207,103,51,34,25,16,12,8,6,3
},
{
    287,143,71,47,35,23,17,11,8,5
},
{
    383,191,95,63,47,31,23,15,11,7
},
/**------------------------------*/
{
    416,207,103,68,51,34,25,16,12,8
},
{
    479,239,119,79,59,39,29,19,14,9
},
{
    520,259,129,86,64,42,32,21,15,10
}
/**-------------------------------*/
};


static void _transmit_data(struct _usart *mythis,const u8_t data)
{
    while (!((*mythis->ux_cofg.reg_ucsra) & (1<<5)) )
    ;
    (*mythis->ux_cofg.reg_udr) = data;
}


static void _send_str(struct _usart *mythis,s8_t *str)
{
    while(*str)
    {
        mythis->transmit_data(mythis,*(str++));
    }
}

void usart_init(usart * const usartn)
{
    global_this = usartn;
    switch(usartn->ux_cofg.number)
    {
        case 0:
          usartn->ux_cofg.reg_udr = (volatile u8_t *)0x2c;
          usartn->ux_cofg.reg_ucsra = (volatile u8_t *)0x2b;
          usartn->ux_cofg.reg_ucsrb = (volatile u8_t *)0x2a;
          usartn->ux_cofg.reg_ucsrc = (volatile u8_t *)0x95;
          usartn->ux_cofg.reg_ubrrl = (volatile u8_t *)0x29;
          usartn->ux_cofg.reg_ubrrh = (volatile u8_t *)0x90;
          break;

        case 1:
          usartn->ux_cofg.reg_udr = (volatile u8_t *)0x9c;
          usartn->ux_cofg.reg_ucsra = (volatile u8_t *)0x9b;
          usartn->ux_cofg.reg_ucsrb = (volatile u8_t *)0x9a;
          usartn->ux_cofg.reg_ucsrc = (volatile u8_t *)0x9d;
          usartn->ux_cofg.reg_ubrrl = (volatile u8_t *)0x99;
          usartn->ux_cofg.reg_ubrrh = (volatile u8_t *)0x98;
          break;

        default:
          usartn->ux_cofg.reg_udr = (volatile u8_t *)0x97;   //保留，操作这些寄存器不会有任何效果
          usartn->ux_cofg.reg_ucsra = (volatile u8_t *)0x97;
          usartn->ux_cofg.reg_ucsrb = (volatile u8_t *)0x97;
          usartn->ux_cofg.reg_ucsrc = (volatile u8_t *)0x97;
          usartn->ux_cofg.reg_ubrrl = (volatile u8_t *)0x97;
          usartn->ux_cofg.reg_ubrrh = (volatile u8_t *)0x97;
          break;
    }

    //设置波特率和数据格式
    (*usartn->ux_cofg.reg_ubrrh) = ubrr_tbl[usartn->ux_cofg.osc_hz][usartn->ux_cofg.ubrr]>>8;
    (*usartn->ux_cofg.reg_ubrrl) = ubrr_tbl[usartn->ux_cofg.osc_hz][usartn->ux_cofg.ubrr];

    (*usartn->ux_cofg.reg_ucsrb) = ((1<<4)|(1<<3));
    if(usartn->ux_cofg.wait_or_interrupt)
        (*usartn->ux_cofg.reg_ucsrb) |= (0x01<<7);
    (*usartn->ux_cofg.reg_ucsrc) = ((usartn->ux_cofg.crc_type<<4)|(usartn->ux_cofg.stop_bits_num<<3)|(usartn->ux_cofg.data_bits_num<<1));
    (*usartn->ux_cofg.reg_udr) = 0; //清除发送和接收寄存器


    //注册驱动
    usartn->transmit_data = _transmit_data;
    usartn->send_str = _send_str;
}
