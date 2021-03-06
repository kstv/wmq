#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

#include "service_dispatch.h"

#include "msg_queue.h"
#include "topic_fd_map.h"
#include "mq_sender.h"

static int queue_idx=0;


/**
 * hash负载均衡，选在消息队列
 */
static
int select_qeueue_default(server_t *master_server){
	int queue_no=0;
	return queue_no;
}


/**
 * 轮询消息队列
 * @return [description]
 */
static 
int select_queue_round(server_t *master_server){
	/** 采用Round Robin方式将其分配给一个消息队列处理 **/
	int i=queue_idx;
	do
	{
		i=(i+1)%master_server->queues;
	}while(i != queue_idx);
	queue_idx=(i+1)%master_server->queues;
	return queue_idx;
}



/**
 * 反序列化消息体，根据编码模式反序列化消息体
 * 
 * 暂时停用
 * 
 * @param pkg [description]
 * 
 */
static uint8_t* deserialize_message(socket_pkg_t *pkg){
	uint8_t* msg=NULL;
	assert(pkg != NULL);

	switch(pkg->code){
		case CODE_NATIVE:{
			msg=pkg->msg; //转化为结构体
			break;
		}
		case CODE_JSON:{
			break;
		}
		case CODE_GZIP:{
			break;
		}
		case CODE_PROTOBUF:{
			break;
		}
		default:break;
	}

	return msg;
}


/**
 * 处理消息包,根据处理操作
 * @param mq  [发送的消息队列通道]
 * @param pkg [消息包]
 */
static void dispatch_service(server_t *master_server,socket_pkg_t *pkg){
	

	switch(pkg->cmd){

		case MQ_OPEN:{
			break;
		}

		case MQ_CLOSE:{
			break;
		}

		case MQ_SUBTOPIC:{    // 注册topic
			add_topic(master_server->ht,pkg->topic,pkg->fd,&master_server->ht_lock);
			break;
		}
		
		case MQ_PUBMSG:{	//发送消息到消息队列

			struct hash_node *node=find_topic_fdlist(master_server->ht,pkg->topic,&master_server->ht_lock);
			if(node==NULL) return;
			
			int idx=select_queue_round(master_server);
			printf("producer------queues number :%d\n",idx);
			msg_queue_t *mq=master_server->mq[idx]; //选择负载最小的队列
			send_msg_mq(mq,pkg);
			
			break;
		}

		default:
			break;
	}
}


void handle_socket_pkg(server_t *master_server,socket_pkg_t *pkg)
{
	if(pkg==NULL) return; 
	if(pkg->msg == NULL){ 
		free(pkg);
		pkg=NULL;
		return;
	}
	dispatch_service(master_server,pkg);	
}

