#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "function.h"


#define MAX_CACHE_SIZE 16  // 最大缓存大小
#define MESSAGE_SIZE 1024   // 消息的大小



// 创建新消息
Message *create_msg(int id, const char *sender, const char *receiver, const char *content) {
    Message *new_msg = (Message *)malloc(sizeof(Message));
    new_msg->id = id;
    new_msg->time_sent = time(NULL);
    strncpy(new_msg->sender, sender, sizeof(new_msg->sender) - 1);
    strncpy(new_msg->receiver, receiver, sizeof(new_msg->receiver) - 1);
    strncpy(new_msg->content, content, sizeof(new_msg->content) - 1);
    new_msg->delivered = 0; // 默认未送达
    return new_msg;
}

// 初始化缓存
MessageCache *init_cache() {
    MessageCache *cache = (MessageCache *)malloc(sizeof(MessageCache));
    cache->head = NULL;
    cache->tail = NULL;
    cache->size = 0; // 初始大小为0
    return cache;
}

void remove_node(MessageCache *cache, Node *node) {
//    printf("remove_node function in\n");

    // 更新前驱节点的next指针
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        cache->head = node->next; // 如果是头节点，更新头指针
    }

    // 更新后继节点的prev指针
    if (node->next) {
        node->next->prev = node->prev;
    } else {
        cache->tail = node->prev; // 如果是尾节点，更新尾指针
    }

//    // 打印要删除的节点及其前后指针
//    printf("Node to remove: %p, prev: %p, next: %p\n", node, node->prev, node->next);
//    printf("Cache head: %p, Cache tail: %p\n", cache->head, cache->tail);
//
//    // 释放消息内容，如果有
//    if (node->msg) {
//        printf("Freeing message at address: %p\n", node->msg);
//        free(node->msg);
//        node->msg = NULL;
//    }
//
//    // 释放节点
//    printf("Freeing node at address: %p\n", node);
//    free(node);
    // 减少缓存的大小
    cache->size--;
//    printf("Node removed. Cache size: %d\n", cache->size); // 打印链表当前大小
}
//void remove_node(MessageCache *cache, Node *node) {
//    printf("remove_node function in\n");
//
//    // 更新前驱节点的next指针
//    if (node->prev) {
//        node->prev->next = node->next;
//    } else {
//        cache->head = node->next; // 如果是头节点，更新头指针
//    }
//
//    // 更新后继节点的prev指针
//    if (node->next) {
//        node->next->prev = node->prev;
//    } else {
//        cache->tail = node->prev; // 如果是尾节点，更新尾指针
//    }
//
//    // 打印要删除的节点及其前后指针
//    printf("Node to remove: %p, prev: %p, next: %p\n", node, node->prev, node->next);
//    printf("Cache head: %p, Cache tail: %p\n", cache->head, cache->tail);
//
//    // 释放消息内容，如果有
//    if (node->msg) {
//        printf("Freeing message at address: %p\n", node->msg);
//        free(node->msg);
//        node->msg = NULL;
//    }
//
//    // 释放节点
//    printf("Freeing node at address: %p\n", node);
//    free(node);
//
//    // 减少缓存的大小
//    cache->size--;
//    printf("Node removed. Cache size: %d\n", cache->size); // 打印链表当前大小
//}


// 将消息移动到链表头
void move_to_head(MessageCache *cache, Node *node) {
//    printf("remove_node in\n");
    remove_node(cache, node);
//    printf("remove_node out\n");
    // 将节点添加到头部
    node->next = cache->head;
    node->prev = NULL;
    if (cache->head) {
        cache->head->prev = node;
    }
    cache->head = node;
    if (!cache->tail) {
        cache->tail = node; // 如果这是新添加的节点，则更新尾指针
    }
    cache->size++;
}

// 缓存查找
Message *cache_lookup(MessageCache *cache, int id) {

//    printf("cache_lookup in \n" );
    Node *current = cache->head;
//    printf("Node in \n" );
    while (current) {
//        printf("while in \n" );
        if (current->msg->id == id) {
//            printf("current->msg in \n" );
            move_to_head(cache, current); // 将使用过的消息移动到头部
//            printf("move_to_head out\n" );
            return current->msg; // 找到并返回缓存中的消息
        }
//        printf("current->msg->id is null\n" );
        current = current->next;
    }
//    printf("while done \n" );
    return NULL; // 未找到
}
//Message *cache_lookup(MessageCache *cache, int id) {
//    printf("cache_lookup in\n");
//
//    Node *current = cache->head;
//    if (!current) {
//        printf("Cache is empty.\n");
//        return NULL; // 如果链表为空，直接返回 NULL
//    }
//
//    printf("Node in\n");
//
//    while (current) {
//        printf("while in, current node address: %p\n", current);
//
//        // 确保 current->msg 不为 NULL
//        if (current->msg == NULL) {
//            printf("Warning: current->msg is NULL at node address %p\n", current);
//            current = current->next;  // 跳过当前节点，继续下一个节点
//            continue;  // 继续循环
//        } else if (current->msg->id == id) {
//            printf("Found node with id: %d at node address %p\n", id, current);
//
//            move_to_head(cache, current); // 将使用过的消息移动到头部
//            printf("move_to_head out\n");
//            return current->msg; // 找到并返回缓存中的消息
//        }
//
//        current = current->next;
//    }
//
//    printf("while done, message with id %d not found.\n", id);
//    return NULL; // 未找到
//}


// 添加消息到 LRU 缓存
void lru_add_message(MessageCache *cache, Message *msg) {
    if (cache->size >= MAX_CACHE_SIZE) {
        // 缓存已满，移除尾部节点（最久未使用的）
        remove_node(cache, cache->tail);
    }
    // 创建新节点并添加到头部
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->msg = msg;
    new_node->prev = NULL;
    new_node->next = cache->head;
    if (cache->head) {
        cache->head->prev = new_node; // 更新头部旧节点的前驱
    }
    cache->head = new_node; // 将新节点添加为头部
    if (!cache->tail) {
        cache->tail = new_node; // 如果这是第一个节点，则更新尾指针
    }
    cache->size++;
}

// store in the file and cache
void store_msg(MessageCache *cache, Message *msg) {
    // 保存消息到文件
    char filename[50];
    sprintf(filename, "message_%d.txt", msg->id);
    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        fprintf(file, "ID: %d\n", msg->id);
        fprintf(file, "Time Sent: %s", ctime(&msg->time_sent));
        fprintf(file, "Sender: %s\n", msg->sender);
        fprintf(file, "Receiver: %s\n", msg->receiver);
        fprintf(file, "Content: %s\n", msg->content);
        fclose(file);
    }

    // 将消息添加到缓存中，应用 LRU 策略
    lru_add_message(cache, msg);
}




Message *retrieve_msg(MessageCache *cache, int id) {
//    printf("Retrieve IN \n" );
    // 先在缓存中查找消息
    Message *cached_msg = cache_lookup(cache, id);
//    printf("cached_msg check \n" );
    if (cached_msg) {
//        printf("cached_msg IN \n" );
        // 缓存命中，直接返回消息
        return cached_msg;
    }
//    printf("file IN \n" );
    // 缓存不命中，从文件加载消息
    char filename[50];
    sprintf(filename, "message_%d.txt", id);
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        Message *msg = (Message *)malloc(sizeof(Message));
        fscanf(file, "ID: %d\n", &msg->id);
        char time_buf[100];
        fgets(time_buf, sizeof(time_buf), file);
        msg->time_sent = time(NULL);
        fscanf(file, "Sender: %s\n", msg->sender);
        fscanf(file, "Receiver: %s\n", msg->receiver);
        fscanf(file, "Content: %s\n", msg->content);
        msg->delivered = 1;
        fclose(file);
//        printf("Open File\n");
        // 将新加载的消息添加到缓存中
        lru_add_message(cache, msg);
        return msg;
    }
//    printf("NULL");

    return NULL;
}
