#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2

pthread_t philosophers[NUM_PHILOSOPHERS];
pthread_mutex_t forks[NUM_PHILOSOPHERS];
int state[NUM_PHILOSOPHERS];

void grab_forks(int philosopher_num) {
    pthread_mutex_lock(&forks[philosopher_num]);
    pthread_mutex_lock(&forks[(philosopher_num + 1) % NUM_PHILOSOPHERS]);
    state[philosopher_num] = EATING;
}

void put_away_forks(int philosopher_num) {
    pthread_mutex_unlock(&forks[philosopher_num]);
    pthread_mutex_unlock(&forks[(philosopher_num + 1) % NUM_PHILOSOPHERS]);
    state[philosopher_num] = THINKING;
}

void test(int philosopher_num) {
    if (state[philosopher_num] == HUNGRY &&
        state[(philosopher_num + 1) % NUM_PHILOSOPHERS] != EATING &&
        state[(philosopher_num + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS] != EATING) {
        grab_forks(philosopher_num);
    }
}

void* philosopher(void* num) {
    int philosopher_num = *((int*)num);

    while (1) {
        // Think
        sleep(1);

        state[philosopher_num] = HUNGRY;
        printf("Philosopher %d is hungry.\n", philosopher_num);

        test(philosopher_num);

        pthread_mutex_lock(&forks[philosopher_num]);
        state[philosopher_num] = EATING;

        printf("Philosopher %d is eating.\n", philosopher_num);
        sleep(2); // Simulate eating

        put_away_forks(philosopher_num);

        printf("Philosopher %d has finished eating.\n", philosopher_num);
        pthread_mutex_unlock(&forks[philosopher_num]);

        // Think again
        sleep(1);
    }

    return NULL;
}

int main() {
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_create(&philosophers[i], NULL, philosopher, &i);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    return 0;
}
