#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

// Optional: use these functions to add debug or error prints to your application
//#define DEBUG_LOG(msg,...)
#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter

    struct thread_data* thread_func_args = (struct thread_data *) thread_param;

    DEBUG_LOG("thread_func_args elements are %d, %d, %p", thread_func_args->wait_to_obtain_ms, thread_func_args->wait_to_release_ms, thread_func_args->mutex);

    DEBUG_LOG("Thread function entered. beginning sleep of %d usec.", thread_func_args->wait_to_obtain_ms);
    usleep(thread_func_args->wait_to_obtain_ms * 1000);
    DEBUG_LOG("Obtain sleep completed. Locking thread.");
    int rc = pthread_mutex_lock(thread_func_args->mutex);
    if (rc != 0)
    {
        ERROR_LOG("Failed to lock thread data.");
        return thread_func_args;
    }
    DEBUG_LOG("Thread locked. Beginning sleep.");
    usleep(thread_func_args->wait_to_release_ms*1000);

    DEBUG_LOG("Sleep completed. Unlocking thread.");
    rc = pthread_mutex_unlock(thread_func_args->mutex);
     if (rc != 0)
    {
        ERROR_LOG("Failed to unlock thread data.");
        return thread_param;
    }
    DEBUG_LOG("Thread unlocked. Marking thread complete.");
    
    thread_func_args->thread_complete_success = true;
    DEBUG_LOG("Thread completed, returning.");
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
    struct thread_data* thread_param = (struct thread_data*) malloc(sizeof(struct thread_data)); 
    if (thread_param == NULL) {
        ERROR_LOG("Failed to allocate memory.");
        return false;}

    thread_param->wait_to_obtain_ms = wait_to_obtain_ms;
    thread_param->wait_to_release_ms = wait_to_release_ms;
    thread_param->mutex = mutex;
    thread_param->thread_complete_success = false;

    DEBUG_LOG("thread_param elements initialized to %d, %d, %p", thread_param->wait_to_obtain_ms, thread_param->wait_to_release_ms, thread_param->mutex);
    int ret;
    ret = pthread_create(thread, NULL, threadfunc, thread_param);

    if (ret != 0) 
    {
        ERROR_LOG("Failed to create thread.");
        free(thread_param);
        return false;
    }
    return true;
}

