#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h> 
#include <semaphore.h> 

#include "shm_r.h"

int main(int argc, char **argv)
{
    sem_t *p_shm_sem;
    u8 *pshm = NULL;
    T_SHM_R *pshm_r = NULL;
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

	pshm_r = (T_SHM_R *)pshm;
    memset(pshm_r, 0, sizeof(T_SHM_R));

	p_shm_sem = sem_open(TEST_SHM_SEM, O_CREAT, 0666, 0);
	if(p_shm_sem == SEM_FAILED)
	{
        perror("sem_open error");
        shmdt(pshm_r);
        shmctl(shm_id, IPC_RMID, NULL);
        return -1;
	}

	while(1)
	{
        sem_wait(p_shm_sem);
        printf("%s %d %s\n\r",__FUNCTION__,__LINE__,pshm_r->block);
	}

	if(shmdt(pshm_r))
	{
        perror("shmdt error");
        sem_close(p_shm_sem);
        sem_unlink(p_shm_sem);
        return -1;
	}
	
	if(shmctl(shm_id, IPC_RMID, NULL) == -1)
	{
        perror("shmctl error");
        sem_close(p_shm_sem);
        sem_unlink(p_shm_sem);
        return -1;
	}
    return 0;
}
