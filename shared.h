#ifndef SHARED_H
#define SHARED_H

#include <semaphore.h>
#include <mutex>

// Define shared data structures and global variables here

extern sem_t* empty;
extern sem_t* full;
extern std::mutex buffer_mutex;
extern int* buffer;
extern int in;

void producer();  // Declaration for the producer function
void consumer();  // Declaration for the consumer function

#endif
