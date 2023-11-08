#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "shared.h"

const int BUFFER_SIZE = 2; //Size of ftable
int* buffer; //Pointer to buffer
int in = 0; //Index of buffer

sem_t* empty;
sem_t* full;
std::mutex buffer_mutex;

void producer() {
    int item;

    for (int i = 0; i < 2; i++) {  // Produce 2 items
        item = rand() % 100;  // Generate a random item
        sem_wait(empty);  // Wait if the buffer is full
        buffer_mutex.lock();  // Lock the buffer
        buffer[in] = item;  // Add the item to the buffer
        std::cout << "Produced: " << item << std::endl;
        in = (in + 1) % BUFFER_SIZE;
        buffer_mutex.unlock();  // Unlock the buffer
        sem_post(full);  // Signal that the buffer is no longer empty
    }
}
int main() {
    // Create and initialize shared memory segment
    int shm_fd = shm_open("/consumerproducer", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(shm_fd, BUFFER_SIZE * sizeof(int)); //Sets size of shared memory to accomdate buffer
    buffer = static_cast<int*>(mmap(nullptr, BUFFER_SIZE * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));  //Maps shared memory into address space 


    empty = sem_open("/empty", O_CREAT, S_IRUSR | S_IWUSR, BUFFER_SIZE); //Semaphore to control empty slots in buffer
    full = sem_open("/full", O_CREAT, S_IRUSR | S_IWUSR, 0); //Semaphore to control full slots in buffer

    std::thread producer_thread(producer);

    producer_thread.join();
    //Closes semaphores and unlinks them from shared memory
    
    sem_close(empty); //Close and unlink the empty semaphore
    sem_close(full); //Close and unlink the full semaphore
    sem_unlink("/empty");
    sem_unlink("/full");

    munmap(buffer, BUFFER_SIZE * sizeof(int)); //Unmaps shared memory //Unmap the shared memory from the process
    shm_unlink("/consumerproducer"); //Unlink the shared memory


    return 0;
}
