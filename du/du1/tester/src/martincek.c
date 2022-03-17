//
// Created by matom on 17.3.2022.
//

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t read_all = PTHREAD_COND_INITIALIZER;
pthread_cond_t sth_to_read = PTHREAD_COND_INITIALIZER;


const int BUFFER_SIZE = 500;
char *buf;
int val = 0;

void *citatel(void *data) {
    int rd;
    while(1)
    {
        pthread_mutex_lock(&mutex);
        while(val != 0)
        {
            pthread_cond_wait(&read_all, &mutex);
        }

        int rd = read(0,buf,BUFFER_SIZE);
        val = rd;

        pthread_cond_signal(&sth_to_read);
        pthread_mutex_unlock(&mutex);

        if(rd == 0) break;
    }
}

void *zapisovatel(void *data) {
    while(1)
    {
        pthread_mutex_lock(&mutex);
        while(val == 0)
        {
            pthread_cond_wait(&sth_to_read, &mutex);
        }
        write(1,buf,val);
        val = 0;
        pthread_cond_signal(&read_all);
        pthread_mutex_unlock(&mutex);
    }
}

int main (int argc, char* argv[]) {
    buf = (char* )malloc(BUFFER_SIZE * sizeof(char));

    pthread_t t1, t2;
    pthread_create(&t1, NULL, citatel,  NULL);
    pthread_create(&t2, NULL, zapisovatel,  NULL);

    pthread_join(t1, NULL);

    //printf("\nSkoncil\n");

    return 0;
}