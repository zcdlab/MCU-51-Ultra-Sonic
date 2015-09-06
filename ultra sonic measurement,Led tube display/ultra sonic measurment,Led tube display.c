#include<reg52.h> 
#include<intrins.h>
#define uint unsigned int// 基于做而论道http://hi.baidu.com/do_sermon/item/0f5c1bdef9affa11d90e44d0
#define uchar unsigned char
unsigned char code WE0[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x80};
unsigned char temp[3];
sbit RX=P3^4; //ECHO
sbit TX=P3^5; //TRIG
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;
bit flag=0;
uint time=0;
uint s=0;
uint tem=0;
void delay(int xms)//delay for x ms
{
int i,j;
for(i=xms;i>0;i--)
for(j=110;j>0;j--);
}
void start() //>=10us high pulse to init TRIG	
{
TX=1;
_nop_();
_nop_();
_nop_();
_nop_();
_nop_();
_nop_();
_nop_();
_nop_();
_nop_();
_nop_();
_nop_();
_nop_();
_nop_();
_nop_();
_nop_();
_nop_();
_nop_();
_nop_();
_nop_();
_nop_();
_nop_();
_nop_();
_nop_();
TX=0;
}
uchar count()
{
time=TH0*256+TL0;
TH0=0;
TL0=0; //timer
s=(time*47)/256;//cm
return s;
}
void dispros()//dispros
{
temp[0]=s%1000/100;
temp[1]=s%1000%100/10;
temp[2]=s%100%100%10; 
}
void display() //display
{
uchar i;
if(s>=500||s<=2||flag==1) //blind area -->flow out	
{
flag=0;
s=0;
TH0=0;
TL0=0; //timer
for(i=0;i<3;i++) //display
{
P0=0x40;
switch(i)
{
case 0:LSA=0; LSB=0; LSC=0; break;
case 1:LSA=1; LSB=0; LSC=0; break;
case 2:LSA=0; LSB=1; LSC=0; break;
}
delay(2);
}
}
else //display normal
{
LSA=0;LSB=0;LSC=0;
P0= WE0[temp[0]]; delay(3);
LSA=1;LSB=0; LSC=0;
P0= WE0[temp[1]]; delay(2);
LSA=0;LSB=1;LSC=0;
P0= WE0[temp[2]]; delay(1);
}
}
void main()
{
TMOD=0X01; //timer in 0 type
TH0=0;
TL0=0;
EA=1; 
ET0=1;//timer interrupt
while(1)
{
start();//init
while(!RX); //wait till RX=0
TR0=1; //counter start
while(RX); //count&wait till RX=1
TR0=0; //counter stop
count(); 
dispros(); //dispros data
display(); //display
delay(90);
}
}
void t0() interrupt 0
{
TH0=0;
TL0=0; //timer
flag=1; //flag to flow
}