#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h> 
#include <semaphore.h> 
#include <string.h>
#include "tgshm.h"
#include "tg_shmw.h"

T_SHM_WRITER g_shm_writer;
sem_t g_shm_writer_sem;

void tg_shm_writer_init(void)
{
	int shm_id;
	int rv;

	g_shm_writer.n_shm_count = 0;

	shm_id = shmget ((key_t)TG_PACKET_SHM_KEY, TG_SHM_MAX_SIZE, 0666 | IPC_CREAT);
	if (shm_id<0)
	{
		perror("shmget error");
		return;
	}

	g_shm_writer.shm_id = shm_id;

	g_shm_writer.p_shm_buf = (T_SHM_BLOCK *)shmat (shm_id, NULL, 0);
	memset ((void*)g_shm_writer.p_shm_buf, 0, TG_SHM_MAX_SIZE); 


	g_shm_writer.p_shm_sem = sem_open(TG_SHM_SEM, O_CREAT, 0666, 0);
	if (g_shm_writer.p_shm_sem == SEM_FAILED)
	{	
		perror ("sem_open error");
		shmdt (g_shm_writer.p_shm_buf);
		shmctl (g_shm_writer.shm_id, IPC_RMID, NULL) ;
		return;
	}

	rv = sem_init(&g_shm_writer_sem, 0, 1);
	if (rv < 0)
	{
		perror ("sem_init error");
		shmdt (g_shm_writer.p_shm_buf);
		shmctl(g_shm_writer.shm_id, IPC_RMID, NULL) ;
		
		sem_close (g_shm_writer.p_shm_sem);
	}
	
}

void tg_shm_writer_deinit(void)
{
	shmdt (g_shm_writer.p_shm_buf);
	shmctl(g_shm_writer.shm_id, IPC_RMID, NULL) ;
	
	sem_close (g_shm_writer.p_shm_sem);

	sem_destroy(&g_shm_writer_sem);
}

inline int tg_shm_writer_index(void)
{
	return (g_shm_writer.n_shm_count%TG_SHM_BLOCK_NUM);
}

inline void tg_shm_writer_index_move(void)
{
	g_shm_writer.n_shm_count ++;
}

u8 *tg_shm_writer_get_free_block(void)
{
	u8 *pbuf;

	sem_wait (&g_shm_writer_sem);
	
	pbuf =  (u8 *)(g_shm_writer.p_shm_buf + tg_shm_writer_index());
	
	tg_shm_writer_index_move();

	sem_post (&g_shm_writer_sem);
	
	return pbuf;
}

inline void tg_shm_writer_sem_post(void)
{	
	sem_post(g_shm_writer.p_shm_sem);
}

int tg_shm_writer_send(const unsigned char *pbuf, int length)
{
	unsigned char *pblock;
	T_SHM_HEADER *phead;

	if  (NULL == pbuf)
	{
		return -1;
	}
    
    if (length > (TG_SHM_MTU - sizeof(int)))
    {
        return -1;
    }
	pblock = tg_shm_writer_get_free_block();
	phead = (T_SHM_HEADER *)pblock;

	phead->len = length;
	memcpy(phead->data, pbuf, length);

	tg_shm_writer_sem_post(); 

	return 0;
}

