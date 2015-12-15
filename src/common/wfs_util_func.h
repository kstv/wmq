#ifndef _WFS_UTIL_FUNC_H
#define _WFS_UTIL_FUNC_H


/*************************
 	求一个buff的ip校验和
 @param	
		buff   待求ip校验和的buff
 @param
		len 	buff的长度
 @return 
 		返回ip校验和
**************************/
extern unsigned short checksum(unsigned char *buf,int len);


/***************************
 	设置文件描述符为非阻塞
 @param	
		fd   文件描述符
 @param
		len 	buff的长度
 @return  int
 		返回标志 
****************************/
extern void setnonblock(int fd);


/********************************
	判断两个unsigned char buff值是否相等
	
@param 	
		{unsigned char} buf1  需要比较的buff1	
@param
		{unsigned char} buf2  需要比较的buff2
@return 
		{int}   如果相等 返回0 如果不相等返回 -1 

********************************/
extern int is_equal(unsigned char *buf1,unsigned char *buf2);


extern void get_nowt(char *time_str);

#endif