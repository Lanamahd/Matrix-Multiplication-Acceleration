/*
    NAME: Lana Mahmoud Ali Musaffer
    ID: 1210455
    Dr. Bashar Tahayna
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>

#define MAX 100

///////////////////////////////////////////////[PART 1]////////////////////////////////////////////////////////////////
#define NUM_OF_CHILDREN 4

int child_processes[NUM_OF_CHILDREN];

//EACH CHILD HAS ITS OWN PIPE & INITIALIZE TWO FILES DESCRIPTORS-->fd[0] for read, fd[1] for write.
int pipes[NUM_OF_CHILDREN][2];

int ID2[MAX][MAX], myOwnMatrix2[MAX][MAX], multTask2[MAX][MAX];

//A METHOD FOR DIVIDING THE TASK BETWEEN THE CHILD PROCESSES BY A SPECIFIED RANGE OF ROWS
void childProcess(int startRow, int endRow){
    char myId2[] = "1210455";
    char birth2[] = "2004";

    //FOR CALCULATING ID*BIRTH_YEAR, AND SAVE IT IN ARRAY--> or we can simply use:char matrix[] = "2425751820"; (by calculator)
    long long idCasting2 = atoll(myId2);
    long long birthCasting2 = atoll(birth2);
    long long res2 = idCasting2 * birthCasting2;
    char resultCasting2[11];
    snprintf(resultCasting2, sizeof(resultCasting2), "%lld", res2);

    // FILL THE ID MATRIX
    for (int i=0; i<MAX; i++){
        for (int j=0; j<MAX; j++){
            ID2[i][j] = myId2[(i*MAX+j) % strlen(myId2)] - '0';
        }
    }

    // MY OWN MATRIX MATRIX
    for (int i=0; i<MAX; i++){
        for (int j=0; j<MAX; j++){
            myOwnMatrix2[i][j] = resultCasting2[(i*MAX+j) % strlen(resultCasting2)] - '0';
        }
    }

    int rowsPerChild = MAX / NUM_OF_CHILDREN; //CALCULATE THE RANGE OF ROWS FOR EACH CHILD TO WORK ON

    //MATRIX MULTIPLICATION
    for (int i=startRow; i<endRow; i++){
        for (int j=0; j<MAX; j++){
            multTask2[i][j] = 0;
            for (int k=0; k<MAX; k++){
                multTask2[i][j] += ID2[i][k] * myOwnMatrix2[k][j];
            }
        }
    }
}

//A METHOD FOR ERROR-HANDLING & CLEANUP
void cleanup(int pipes[][2]){
    for (int i=0; i<NUM_OF_CHILDREN; i++){
        close(pipes[i][0]); //CLOSE THE READ END OF THE PIPE
        close(pipes[i][1]); //CLOSE THE WRITE END OF THE PIPE
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////[JOINABLE THREADS]/////////////////////////////////////////////////////////////////
#define NUM_OF_THREADS 4
pthread_barrier_t barrier;  //FOR SYNCHRONIZING
int ID3[MAX][MAX], myOwnMatrix3[MAX][MAX], multTask3[MAX][MAX];

//STRUCT FOR DATA THAT USED IN THE TASK, TO PASS THEM TO THE THREAD
struct ThreadData{
    int startRow2;
    int endRow2;
    char myId3[11];
    char birth3[5];
};

//STRUCT FOR THREAD POOL DATA
typedef struct{
    pthread_t *threads;
    int numThreads;
}ThreadPool;

//A METHOD THAT INITIALIZE A THREAD POOL WITH MULTITHREADS
ThreadPool *ThreadPoolInitialize(int numThreads){
    ThreadPool *pool = malloc(sizeof(ThreadPool));
    pool->threads = malloc(numThreads * sizeof(pthread_t));
    pool->numThreads = numThreads;
    pthread_barrier_init(&barrier, NULL, numThreads);  // Initialize the barrier
    return pool;
}

//A METHOD FOR DIVIDING THE TASK BETWEEN THE THREADS BY A SPECIFIED RANGE OF ROWS
void *ThreadFunc(void *param) {
    struct ThreadData *data = (struct ThreadData *)param;

    long long idCasting3 = atoll(data->myId3);
    long long birthCasting3 = atoll(data->birth3);
    long long res3 = idCasting3 * birthCasting3;
    char resultCasting3[11];
    snprintf(resultCasting3, sizeof(resultCasting3), "%lld", res3);

    int sum2 = 0;

    //FILL THE ID MATRIX
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            ID3[i][j] = data->myId3[(i*MAX+j) % 7] - '0';
        }
    }

    // Filling myOwnMatrix matrix
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            myOwnMatrix3[i][j] = resultCasting3[(i*MAX+j) % 10] - '0';
        }
    }

    pthread_barrier_wait(&barrier);  //TO WAIT FOR ALL THREADS TO REACH THIS POINT

    // Matrix multiplication
    for (int i = data->startRow2; i < data->endRow2; i++) {
        for (int j = 0; j < MAX; j++) {
            for (int k = 0; k < MAX; k++) {
                sum2 += ID3[i][k] * myOwnMatrix3[k][j];
            }

            pthread_barrier_wait(&barrier); //BARRIER BEFORE UPDATING THE SHARED MATRIX
            multTask3[i][j] += sum2;
            sum2 = 0;
        }
    }

    pthread_exit(NULL);
}

//A METHOD THAT CREATES & EXECUTES THREADS IN THE THREAD POOL
void ThreadPoolExecute(ThreadPool *pool, struct ThreadData *data) {
    for (int i = 0; i < pool->numThreads; i++) {
        pthread_create(&pool->threads[i], NULL, ThreadFunc, &data[i]);
    }

    //EXECUTION PAUSES HERE UNTIL THREAD IS DONE THE TASK
    for (int i = 0; i < pool->numThreads; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier); //DESTROY THE BARRIER AFTER ALL THREADS ARE COMPLETED

}

//A METHOD FOR CLEANUP AND FREEING THE MEMORY
void ThreadPoolDestroy(ThreadPool *pool) {
    free(pool->threads);
    free(pool);
}
///////////////////////////////////////////////[DETACHED THREADS]///////////////////////////////////////////////////////////
pthread_mutex_t mutex2;
int ID4[MAX][MAX], myOwnMatrix4[MAX][MAX], multTask4[MAX][MAX];

int globalCounter = 0;  //GLOBAL COUNTER FOR THE TIME

//STRUCT FOR DATA THAT USED IN THE TASK, TO PASS THEM TO THE THREAD
struct ThreadDetachedData {
    int startRow3;
    int endRow3;
    char myId4[11];
    char birth4[5];
};

//A METHOD FOR DIVIDING THE TASK BETWEEN THE THREADS BY A SPECIFIED RANGE OF ROWS
void *ThreadDetachedFunc(void *param2) {
    struct ThreadDetachedData *data2 = (struct ThreadDetachedData *)param2;

    long long idCasting4 = atoll(data2->myId4);
    long long birthCasting4 = atoll(data2->birth4);
    long long res4 = idCasting4 * birthCasting4;
    char resultCasting4[11];
    snprintf(resultCasting4, sizeof(resultCasting4), "%lld", res4);

    int sum3 = 0;

    //FILL ID4 MATRIX
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            ID4[i][j] = data2->myId4[(i * MAX + j) % 7] - '0';
        }
    }

    //FILL myOwnMatrix4 MATRIX
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            myOwnMatrix4[i][j] = resultCasting4[(i * MAX + j) % 10] - '0';
        }
    }
    //MATRIX MULTIPLICATION
    for (int i = data2->startRow3; i < data2->endRow3; i++) {
        for (int j = 0; j < MAX; j++) {
            for (int k = 0; k < MAX; k++) {
                sum3 += ID4[i][k] * myOwnMatrix4[k][j];
            }
            pthread_mutex_lock(&mutex2);
            multTask4[i][j] += sum3;
            pthread_mutex_unlock(&mutex2);
            sum3 = 0;
        }
    }

    //DECREMENT THE GLOBAL COUNTER
    pthread_mutex_lock(&mutex2);
    globalCounter--;
    pthread_mutex_unlock(&mutex2);

    pthread_exit(NULL);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////[MIXED THREADS]///////////////////////////////////////////////////////////
pthread_barrier_t barrier3;  //FOR SYNCHRONIZING
int ID5[MAX][MAX], myOwnMatrix5[MAX][MAX], multTask5[MAX][MAX];

//STRUCT FOR DATA THAT USED IN THE TASK, TO PASS THEM TO THE THREAD
struct ThreadMixedData {
    int startRow4;
    int endRow4;
    char myId5[11];
    char birth5[5];
};

//STRUCT FOR THREAD POOL DATA
typedef struct {
    pthread_t *threads3;
    int numThreads3;
} ThreadMixedPool;

//A METHOD THAT INITIALIZE A THREAD POOL WITH MULTITHREADS
ThreadMixedPool *ThreadMixedPoolInitialize(int numThreads3) {
    ThreadMixedPool *pool3 = malloc(sizeof(ThreadMixedPool));
    pool3->threads3 = malloc(numThreads3 * sizeof(pthread_t));
    pool3->numThreads3 = numThreads3;
    pthread_barrier_init(&barrier3, NULL, numThreads3);  // Initialize the barrier
    return pool3;
}

//A METHOD FOR CLEANUP AND FREEING THE MEMORY
void ThreadMixedPoolDestroy(ThreadMixedPool *pool3) {
    free(pool3->threads3);
    free(pool3);
}

//A METHOD FOR DIVIDING THE TASK BETWEEN THE THREADS BY A SPECIFIED RANGE OF ROWS
void *ThreadMixedFunc(void *param3) {
    struct ThreadMixedData *data3 = (struct ThreadMixedData *)param3;

    long long idCasting5 = atoll(data3->myId5);
    long long birthCasting5 = atoll(data3->birth5);
    long long res5 = idCasting5 * birthCasting5;
    char resultCasting5[11];
    snprintf(resultCasting5, sizeof(resultCasting5), "%lld", res5);

    int sum4 = 0;

    //FILL THE ID5 MATRIX
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            ID5[i][j] = data3->myId5[(i * MAX + j) % 7] - '0';
        }
    }

    //FILL myOwnMatrix5 MATRIX
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            myOwnMatrix5[i][j] = resultCasting5[(i * MAX + j) % 10] - '0';
        }
    }

    pthread_barrier_wait(&barrier3);  //TO WAIT FOR ALL THREADS TO REACH THIS POINT

    for (int i = data3->startRow4; i < data3->endRow4; i++) {
        for (int j = 0; j < MAX; j++) {
            for (int k = 0; k < MAX; k++) {
                sum4 += ID5[i][k] * myOwnMatrix5[k][j];
            }
            pthread_barrier_wait(&barrier3); //BARRIER BEFORE UPDATING THE SHARED MATRIX
            multTask5[i][j] += sum4;
            sum4 = 0;
        }
    }

    pthread_exit(NULL);
}

//A METHOD THAT CREATES & EXECUTES THREADS IN THE THREAD POOL
void ThreadMixedPoolExecute(ThreadMixedPool *pool3, struct ThreadMixedData *data3) {
    //DETACH HALF OF THE THREADS
    for (int i = 0; i < pool3->numThreads3 / 2; i++) {
        pthread_create(&pool3->threads3[i], NULL, ThreadMixedFunc, &data3[i]);
        pthread_detach(pool3->threads3[i]);
    }

    //JOIN THE OTHER HALF OF THE THREADS
    for (int i = pool3->numThreads3 / 2; i < pool3->numThreads3; i++)
        pthread_create(&pool3->threads3[i], NULL, ThreadMixedFunc, &data3[i]);

    for (int i = pool3->numThreads3 / 2; i < pool3->numThreads3; i++)
        pthread_join(pool3->threads3[i], NULL);

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]){

/////////////////////////////////////////////[NAIVE APPROACH]////////////////////////////////////////////////////////////////
    //START MEASURING THE TIME
    struct timeval nativeBegin, nativeEnd;
    gettimeofday(&nativeBegin, NULL);

    char myId[] = "1210455";
    char birth[] = "2004";

    //FOR CALCULATING ID*BIRTH_YEAR, AND SAVE IT IN ARRAY--> or we can simply use:char matrix[] = "2425751820"; (by calculator)
    long long idCasting = atoll(myId);
    long long birthCasting = atoll(birth);
    long long res = idCasting * birthCasting;
    char resultCasting[11]; //THE ANSWER IS 10 DIGITS
    snprintf(resultCasting, sizeof(resultCasting), "%lld", res);

    //INITIALIZE MY MATRICES, AND ESSENTAIL VARIABLES FOR THE LOOPS
    int ID[MAX][MAX], myOwnMatrix[MAX][MAX], multTask[MAX][MAX];
    int i, j, k, p = 0, l = 0;
    int sum = 0;

    //FILL THE ID MATRIX
    for (i = 0; i<MAX; i++) {
        for (j = 0; j<MAX; j++) {
            ID[i][j] = myId[l] - '0';  //CONVERT CHARACTER TO INTEGER
            l = (l + 1) % strlen(myId);    //MOVE TO THE NEXT DIGIT
        }
    }

    // MY OWN MATRIX MATRIX
    for (i = 0; i < MAX; i++) {
        for (j = 0; j < MAX; j++) {
            myOwnMatrix[i][j] = resultCasting[p] - '0';  //CONVERT CHARACTER TO INTEGER
            p = (p + 1) % strlen(resultCasting);    //MOVE TO THE NEXT DIGIT
        }
    }

    //MATRIX MULTIPLICATION
    for (i = 0; i<MAX; i++) {
        for (j = 0; j<MAX; j++) {
            for(k=0; k< MAX; k++){
                sum += ID[i][k] * myOwnMatrix[k][j];
            }
            multTask[i][j] = sum;
            sum = 0;
        }
    }

    //STOP MEASURING TIME AND CALCULATE THE ELAPSED TIME
    gettimeofday(&nativeEnd, NULL);
    long nativeSeconds = nativeEnd.tv_sec - nativeBegin.tv_sec;
    long nativeMicroseconds = nativeEnd.tv_usec - nativeBegin.tv_usec;
    double nativeTime = nativeSeconds + nativeMicroseconds*1e-6;

    //PRINT THE TIME
    printf("*********************\n");
    printf("TIME MEASURED FOR NAIVE APPROACH: %f seconds.\n", nativeTime);
    printf("*********************\n");

///////////////////////////////////////////////[CHILD PROCESSES]////////////////////////////////////////////////////////////////

    //START MEASURING THE TIME
    struct timeval processBegin, processEnd;
    gettimeofday(&processBegin, NULL);

    //CREATING THE PIPES
    for (int i=0; i<NUM_OF_CHILDREN; i++){
        if (pipe(pipes[i]) == -1){
            perror("An error ocurred with opening the pipe.\n");
            cleanup(pipes);
            exit(EXIT_FAILURE);
        }
    }

    //CREATING THE CHILD PROCESSES
    for (int i=0; i<NUM_OF_CHILDREN; i++){
        int id = fork();

        if (id == -1){
            perror("An error ocurred with fork.\n");
            cleanup(pipes);
            exit(EXIT_FAILURE);
        }

        //CHILD PROCESS
        else if (id == 0){
            close(pipes[i][0]); //COLSE THE READ END OF THE PIPE INT THE CHILD PROCESS

            int rowsPerChild = MAX / NUM_OF_CHILDREN;
            int startRow = i * rowsPerChild;
            int endRow;

            if(i == NUM_OF_CHILDREN - 1){
                endRow = MAX;
            }

            else{
                endRow = (i + 1) * rowsPerChild;
            }

            childProcess(startRow, endRow);

            //SENT THE RESULT MATRIX TO THE PARENT THROUGH THE PIPE
            if(write(pipes[i][1], multTask2, sizeof(multTask2)) == -1){
                perror("Error writing to pipe.\n");
                close(pipes[i][1]);
                exit(EXIT_FAILURE);
            }

            close(pipes[i][1]); //COLSE THE WRITE END OF THE PIPE INT THE CHILD PROCESS
            exit(EXIT_SUCCESS);
        }

        else{
            child_processes[i] = id;
        }
    }

    //PARENT PROCESS
    int finalResult[MAX][MAX] = {0}; //INITIALIZE THE FINAL MATRIX
    for (int i=0; i<NUM_OF_CHILDREN; i++){
        close(pipes[i][1]); //COLSE THE WRITE END OF THE PIPE INT THE PARENT PROCESS
    }

    //READ THE RESULT MATRIX FROM EACH CHILD THROUGH THE PIPE
    for (int i=0; i<NUM_OF_CHILDREN; i++){
        int partialResult[MAX][MAX];
        if (read(pipes[i][0], partialResult, sizeof(partialResult)) == -1){
            perror("Error reading from pipe.\n");
            cleanup(pipes);
            exit(EXIT_FAILURE);
        }
        close(pipes[i][0]);

        //ACCUMULATE THE PARTIAL RESULT INTO A SEPARATE MATRIX
        for (int row=0; row<MAX; row++){
            for (int col=0; col<MAX; col++){
                finalResult[row][col] += partialResult[row][col];
            }
        }
    }

    //WAIT FOR ALL CHILD PRECESSES TO COMPLETE
    for (int i=0; i<NUM_OF_CHILDREN; i++){
        int status;
        if (waitpid(child_processes[i], &status, 0) == -1){
            perror("Error waiting for child process.\n");
            cleanup(pipes);
            exit(EXIT_FAILURE);
        }
    }

    //PRINTING THE MATRIX
    printf("*********************\n");

    //STOP MEASURING TIME AND CALCULATE THE ELAPSED TIME
    gettimeofday(&processEnd, NULL);
    long processSeconds = processEnd.tv_sec - processBegin.tv_sec;
    long processMicroseconds = processEnd.tv_usec - processBegin.tv_usec;
    double processTime = processSeconds + processMicroseconds*1e-6;

    //PRINT THE TIME
    printf("*********************\n");
    printf("TIME MEASURED FOR PROCESS SOLUTION: %f seconds.\n", processTime);
    printf("*********************\n");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////[JOINABLE THREADS]////////////////////////////////////////////////////////////////

    //START MEASURING THE TIME
    struct timeval joinBegin, joinEnd;
    gettimeofday(&joinBegin, NULL);

    struct ThreadData threadData[NUM_OF_THREADS];

    //INITIALIZE THREAD DATA
    for (int i = 0; i < NUM_OF_THREADS; i++) {
        snprintf(threadData[i].myId3, sizeof(threadData[i].myId3), "%s", "1210455");
        snprintf(threadData[i].birth3, sizeof(threadData[i].birth3), "%s", "2004");
        threadData[i].startRow2 = i * (MAX / NUM_OF_THREADS);
        threadData[i].endRow2 = (i + 1) * (MAX / NUM_OF_THREADS);
    }

    ThreadPool *pool = ThreadPoolInitialize(NUM_OF_THREADS); //CREATE A POOL OF THREADS
    ThreadPoolExecute(pool, threadData); //TASK EXECUTION
    ThreadPoolDestroy(pool); //CLEANUP

    printf("*********************\n");

    //STOP MEASURING TIME AND CALCULATE THE ELAPSED TIME
    gettimeofday(&joinEnd, NULL);
    long joinSeconds = joinEnd.tv_sec - joinBegin.tv_sec;
    long joinMicroseconds = joinEnd.tv_usec - joinBegin.tv_usec;
    double joinTime = joinSeconds + joinMicroseconds*1e-6;

    //PRINT THE TIME
    printf("*********************\n");
    printf("TIME MEASURED FOR JOINABLE THREADS: %f seconds.\n", joinTime);
    printf("*********************\n");
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////[DETACHED THREADS]///////////////////////////////////////////////////////////
    pthread_mutex_init(&mutex2, NULL);

    struct ThreadDetachedData threadData2[NUM_OF_THREADS];

    //INITIALIZE THREAD DATA
    for (int i = 0; i < NUM_OF_THREADS; i++) {
        snprintf(threadData2[i].myId4, sizeof(threadData2[i].myId4), "%s", "1210455");
        snprintf(threadData2[i].birth4, sizeof(threadData2[i].birth4), "%s", "2004");
        threadData2[i].startRow3 = i * (MAX / NUM_OF_THREADS);
        threadData2[i].endRow3 = (i + 1) * (MAX / NUM_OF_THREADS);
    }

    globalCounter = NUM_OF_THREADS; //SET THE GLOBAL COUNTER

    clock_t start = clock();

    for (int i = 0; i < NUM_OF_THREADS; i++) {
        pthread_t tid;
        pthread_create(&tid, NULL, ThreadDetachedFunc, &threadData2[i]);
        pthread_detach(tid);
    }

    //WAIT UNTIL ALL THREADS FINISHED
    while (globalCounter > 0) {
        //sleep(2); //FOR AVOID BUSY-WAITING
    }

    clock_t end = clock();
    double globalTime = ((double) (end - start)) / CLOCKS_PER_SEC;

    pthread_mutex_destroy(&mutex2);

    printf("*********************\n");
    printf("*********************\n");
    printf("TIME MEASURED FOR DETACHED THREADS: %f seconds.\n", globalTime);
    printf("*********************\n");

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////[MIXED THREADS]///////////////////////////////////////////////////////////

    //START MEASURING THE TIME
    struct timeval mixedBegin, mixedEnd;
    gettimeofday(&mixedBegin, NULL);

    struct ThreadMixedData threadData3[NUM_OF_THREADS];

    //INITIALIZE THREAD DATA
    for (int i = 0; i < NUM_OF_THREADS; i++) {
        snprintf(threadData3[i].myId5, sizeof(threadData3[i].myId5), "%s", "1210455");
        snprintf(threadData3[i].birth5, sizeof(threadData3[i].birth5), "%s", "2004");
        threadData3[i].startRow4 = i * (MAX / NUM_OF_THREADS);
        threadData3[i].endRow4 = (i + 1) * (MAX / NUM_OF_THREADS);
    }

    ThreadMixedPool *pool3 = ThreadMixedPoolInitialize(NUM_OF_THREADS); //CREATE A POOL OF THREADS
    ThreadMixedPoolExecute(pool3, threadData3); //TASK EXECUTION
    ThreadMixedPoolDestroy(pool3); //CLEANUP

    printf("*********************\n");

    //STOP MEASURING TIME AND CALCULATE THE ELAPSED TIME
    gettimeofday(&mixedEnd, NULL);
    long mixedSeconds = mixedEnd.tv_sec - mixedBegin.tv_sec;
    long mixedMicroseconds = mixedEnd.tv_usec - mixedBegin.tv_usec;
    double mixedTime = mixedSeconds + mixedMicroseconds*1e-6;

    //PRINT THE TIME
    printf("*********************\n");
    printf("TIME MEASURED FOR MIXED THREADS: %f seconds.\n", mixedTime);
    printf("*********************\n");
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    return 0;
}
