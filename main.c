#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "function.h"


int main() {
    srand(time(NULL));
    MessageCache *cache = init_cache();
    int cache_hits = 0; // 统计缓存命中次数

    // Step 1: 存储160条消息
    for (int i = 0; i < 160; i++) {
        char sender[20];
        char receiver[20];
        char content[100];

        sprintf(sender, "Sender_%d", i);
        sprintf(receiver, "Receiver_%d", i);
        sprintf(content, "Message content for ID %d", i);

        Message *msg = create_msg(i, sender, receiver, content);
        store_msg(cache, msg);
//        printf("Stored message ID %d\n", i);
    }

    // Step 2: 执行1000次 retrieve_msg 调用，并统计缓存命中次数
    for (int i = 0; i < 1000; i++) {  // 假设你要执行1000次
//        printf("I :%d\n", i);  // 打印最终缓存命中次数
        int random_id = rand() % 160;  // 随机选择消息ID，范围在[0, 159]
//        printf("ID: %d\n", random_id);
        Message *cached_msg = cache_lookup(cache, random_id);
        if (cached_msg != NULL) {
            cache_hits++;  // 如果在缓存中找到消息，增加命中次数
//            printf("Cache hits\n");
//            continue;
        }
//        printf("retrieve_msg \n");
        retrieve_msg(cache, random_id);
    }

    printf("Cache hits: %d out of 1000\n", cache_hits);  // 打印最终缓存命中次数

    return 0;
}