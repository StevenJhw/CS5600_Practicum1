#ifndef MESSAGE_CACHE_H
#define MESSAGE_CACHE_H

#include <time.h>

// 消息结构体 less then 1024
typedef struct {
    int id;                   // 消息唯一标识符
    time_t time_sent;         // 发送时间
    char sender[100];         // 发送者
    char receiver[100];       // 接收者
    char content[800];        // 消息内容
    int delivered;            // 是否已送达标志
} Message;

// 链表节点结构体（用于 LRU 缓存）
typedef struct Node {
    Message *msg;             // 消息指针
    struct Node *prev;        // 前驱指针
    struct Node *next;        // 后继指针
} Node;

// LRU 缓存结构体
typedef struct {
    Node *head;               // 链表头
    Node *tail;               // 链表尾
    int size;                 // 当前缓存使用的消息数量
} MessageCache;

// 函数声明
Message *create_msg(int id, const char *sender, const char *receiver, const char *content);  // 创建新消息
MessageCache *init_cache();  // 初始化缓存
void remove_node(MessageCache *cache, Node *node);  // 删除链表中的节点
void move_to_head(MessageCache *cache, Node *node);  // 将消息移动到链表头
Message *cache_lookup(MessageCache *cache, int id);  // 缓存查找
void lru_add_message(MessageCache *cache, Message *msg);  // 添加消息到 LRU 缓存
void store_msg(MessageCache *cache, Message *msg);  // 存储消息到文件和缓存
Message *retrieve_msg(MessageCache *cache, int id);  // 从缓存或文件中检索消息

#endif // MESSAGE_CACHE_H
