#ifndef __TOP_FD_MAP_H_
#define __TOP_FD_MAP_H_

#include <pthread.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdint.h>

#include "hashtable.h"
#include "lists.h"




struct list_entry{  //entry / elem
	char *topic;
	int  fd;
	TGAP_LIST_ENTRY(list_entry) field;  		//指向下一个节点或者上一个节点
};

// TGAP_LIST_HEAD(FD_LIST,list_entry) fd_list; //声明一个头结点
// TGAP_LIST_HEAD_INIT(&fd_list);	 		   //初始化头结点
struct hash_node{ 
	//head 
	TGAP_RWLIST_HEAD(FD_LIST,list_entry) fd_list_head; //声明一个头结点
	
};




/**
 * 创建一个hashtable
 * @return [description]
 */
extern HashTable *create_fdtopic_hashtable();

/**
 * 添加topic和fd
 * @param ht    [hashtable]
 * @param topic [主题]
 * @param fd    [文件描述符]
 * 
 * @param ht_lock  [hashtable 的互斥锁]
 */
extern void add_topic(HashTable *ht,
			uint8_t *topic,
			int fd,
			pthread_rwlock_t *ht_lock);

/**
 * 根据topic找到对应的fd list
 * @param  ht    [description]
 * @param  topic [description]
 * @param rb_root_lock  [hashtable 的互斥锁]
 * @return       [description]
 * 
 */
extern struct hash_node *find_topic_fdlist(HashTable *ht,
				char *topic,
				pthread_rwlock_t *rb_root_lock);

/**
 * 删除topic中对应的fd
 * @param ht    [description]
 * @param topic [description]
 * @param fd    [description]
 */
extern void delete_fd(HashTable *ht,char *topic,int fd);



extern void destroy_fdtopic_hashtable(HashTable *ht);

#endif