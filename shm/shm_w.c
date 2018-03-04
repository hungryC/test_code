#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h> 
#include <semaphore.h> 
#include "shm_w.h"


int main(int argc, char **argv)
{
	sem_t *p_shm_sem;
	void *pshm = NULL;
	T_SHM_W *pshm_w = NULL;
	int loop = 100000;
	int shm_id;
	shm_id = shmget((key_t)TEST_SHM_KEY, TEST_SHM_MTU, 0666|IPC_CREAT);
	if(shm_id)
	{
		perror("shmget error");
		return -1;
	}

	pshm = shmat(shm_id, NULL, 0);
	if(pshm == (void *)-1)
	{
	    perror("shmar  error");
	    return -1;
	}

	pshm_w = (T_SHM_W *)pshm;
    memset(pshm_w, 0, TEST_SHM_MTU);
	p_shm_sem = sem_open(TEST_SHM_SEM, O_CREAT, 0666, 0);
	if(p_shm_sem == SEM_FAILED)
	{
        perror("sem_open error");
        shmdt(pshm_w);
        shmctl(shm_id, IPC_RMID, NULL);
        return -1;
	}
	
	while(loop --)
	{
	    memset(pshm_w->block, 0, TEST_SHM_MTU);
        sprintf(pshm_w->block, "test_%d", loop);
        sem_post(p_shm_sem);
        sleep(1);
	}

	if(shmdt(pshm_w))
	{
        perror("shmdt error");
        return -1;
	}
	
	return 0;
}
