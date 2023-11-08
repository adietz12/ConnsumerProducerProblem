#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "shared.h"

const int BUFFER_SIZE = 2;
int* buffer;
int in = 0;

sem_t* empty;
sem_t* full;
std::mutex buffer_mutex;

void consumer() {
    int item;

    for (int i = 0; i < 2; i++) {  // Consume 2 items
        sem_wait(full);  // Wait if the buffer is empty
        buffer_mutex.lock();  // Lock the buffer
        item = buffer[in];  // Retrieve an item from the buffer
        std::cout << "Consumed: " << item << std::endl;
        in = (in + 1) % BUFFER_SIZE;
        buffer_mutex.unlock();  // Unlock the buffer
        sem_post(empty);  // Signal that the buffer is no longer full
    }
}

int main() {
    // Create and initialize shared memory segment with the name consumerproducer
    int shm_fd = shm_open("/consumerproducer", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(shm_fd, BUFFER_SIZE * sizeof(int)); //Sets size of shared memory to accomdate buffer
    buffer = static_cast<int*>(mmap(nullptr, BUFFER_SIZE * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0)); //Maps shared memory into address space 

    empty = sem_open("/empty", O_CREAT, S_IRUSR | S_IWUSR, BUFFER_SIZE); //Semaphore to control empty slots in buffer
    full = sem_open("/full", O_CREAT, S_IRUSR | S_IWUSR, 0); //Semaphore to control full slots in buffer

    std::thread consumer_thread(consumer); //Creates thread that executes consumer function

    consumer_thread.join();  //Wait for thread to finish execution

    sem_close(empty); //Close and unlink the empty semaphore
    sem_close(full); //Close and unlink the full semaphore
    sem_unlink("/empty");
    sem_unlink("/full");

    munmap(buffer, BUFFER_SIZE * sizeof(int)); //Unmap the shared memory from the process
    shm_unlink("/consumerproducer"); //Unlink the shared memory

    return 0;
}
