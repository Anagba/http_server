#ifndef RATE_LIMITER_H
#define RATE_LIMITER_H

#include <pthread.h>
#include <time.h>

typedef struct {
    double capacity;
    double tokens;
    double refill_rate; // Tokens per second
    struct timespec last_update;
    pthread_mutex_t lock;
} TokenBucket;

void init_bucket(TokenBucket *bucket, double capacity, double refill_rate);
int allow_request(TokenBucket *bucket);
void cleanup_bucket(TokenBucket *bucket);

#endif
