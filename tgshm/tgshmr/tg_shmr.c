#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h> 
#include <semaphore.h> 
#include "tgshm.h"
#include "tg_shmr.h"

T_SHM_READER g_shm_reader;
sem_t g_shm_reader_sem;


void tg_shm_reader_init(void)
{
	int shm_id;
	int rv;
	
	shm_id = shmget ((key_t)TG_PACKET_SHM_KEY,TG_SHM_MAX_SIZE,0666 | IPC_CREAT);
	if (shm_id < 0)
	{
		perror("shmget error");
		return;
	}

	g_shm_reader.shm_id = shm_id;

	g_shm_reader.p_shm_buf = (T_SHM_BLOCK *)shmat ((key_t)shm_id,NULL,0);

	g_shm_reader.p_shm_sem = sem_open (TG_SHM_SEM, O_CREAT, 0666, 0);
	if (g_shm_reader.p_shm_sem == SEM_FAILED)
	{
		 perror("sem_open error");
 		 shmdt (g_shm_reader.p_shm_buf);
		 shmctl (g_shm_reader.shm_id, IPC_RMID, NULL) ;
	}

	rv = sem_init (&g_shm_reader_sem, 0, 1);
	if (rv < 0)
	{
		perror ("sem_init error");
		shmdt (g_shm_reader.p_shm_buf);
		shmctl(g_shm_reader.shm_id, IPC_RMID, NULL) ;
		
		sem_close (g_shm_reader.p_shm_sem);
	}	
}

void tg_shm_reader_deinit(void)
{
	shmdt (g_shm_reader.p_shm_buf);
	shmctl(g_shm_reader.shm_id, IPC_RMID, NULL) ;
	
	sem_close (g_shm_reader.p_shm_sem);

	sem_destroy(&g_shm_reader_sem);
}

inline int tg_shm_reader_index(void)
{
	return (g_shm_reader.n_shm_count%TG_SHM_BLOCK_NUM);
}

inline void tg_shm_reader_index_move(void)
{
	g_shm_reader.n_shm_count ++;
}

inline void tg_shm_reader_sem_wait(void)
{
	sem_wait(g_shm_reader.p_shm_sem);
}

int tg_shm_reader_next_packet(u8 **ppbuf)
{
	u8 *pbuf;
	T_SHM_HEADER *pheader;

	tg_shm_reader_sem_wait();

	sem_wait (&g_shm_reader_sem);
		
	pbuf =  (u8 *)(g_shm_reader.p_shm_buf + tg_shm_reader_index());
	
	tg_shm_reader_index_move();

	sem_post (&g_shm_reader_sem);

	pheader = (T_SHM_HEADER *)pbuf;
	*ppbuf = pheader->data;
	
	return pheader->len;
}




