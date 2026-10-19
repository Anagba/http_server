#include "rate_limiter.h"
#include <stdio.h>


void init_bucket(TokenBucket *bucket, double capacity, double refill_rate) 
{
    bucket->capacity = capacity;
    bucket->tokens = capacity; // Start full
    bucket->refill_rate = refill_rate;
    clock_gettime(CLOCK_MONOTONIC, &bucket->last_update);
    pthread_mutex_init(&bucket->lock, NULL);
}

int allow_request(TokenBucket *bucket) {
    pthread_mutex_lock(&bucket->lock);

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    // Calculate elapsed time in seconds
    double elapsed = (now.tv_sec - bucket->last_update.tv_sec) +
                     (now.tv_nsec - bucket->last_update.tv_nsec) / 1e9;

    // Refill tokens
    bucket->tokens += elapsed * bucket->refill_rate;
    if (bucket->tokens > bucket->capacity) {
        bucket->tokens = bucket->capacity;
    }

    bucket->last_update = now;
    printf("[RateLimiter] Debug: Elapsed = %.4fs | Tokens available = %.2f\n", elapsed, bucket->tokens);
    fflush(stdout);
    if (bucket->tokens >= 1.0) {
        bucket->tokens -= 1.0;
        pthread_mutex_unlock(&bucket->lock);
        return 1; // Allow request
    }

    pthread_mutex_unlock(&bucket->lock);
    return 0; // Rate limited (Block request)
}

void cleanup_bucket(TokenBucket *bucket) 
{
    pthread_mutex_destroy(&bucket->lock);
}
