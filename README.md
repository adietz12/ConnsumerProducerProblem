# ConsumerProducerProblem
OS project 1 by Anthony Dietz

This project demonstrates a solution to the classic producer-consumer problem using shared memory and semaphores in C++. The project is split into two source files: `producer.cpp` and `consumer.cpp`, which work together to produce and consume items through shared memory.

## Files

- `producer.cpp`: This file contains the producer program, which generates items and places them into a shared buffer or table.
- `consumer.cpp`: This file contains the consumer program, which retrieves and consumes items from the shared buffer or table.
- `shared.h`: This header file defines common variables and synchronization mechanisms shared between the producer and consumer.

## Compilation

To compile the programs, you can use the `g++` compiler on a Linux system. I ran each file in a separate terminal and that seemed to work.

```bash
g++ -o producer producer.cpp -lpthread -lrt
g++ -o consumer consumer.cpp -lpthread -lrt
./producer
./consumer
```

##Example
![image](https://github.com/adietz12/ConnsumerProducerProblem/assets/150217383/4739f3f1-bcb6-4441-859c-9a24f880dc23)
![image](https://github.com/adietz12/ConnsumerProducerProblem/assets/150217383/cb09dc0f-0d01-49b5-a9d2-e6f1cf3edab6)
These two images show both programs running concurrently in separate terminals.
