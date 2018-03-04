#ifndef __TG_SHMW_H__
#define __TG_SHMW_H__


typedef struct tg_shm_writer
{
	int 			  	shm_id;
	u32  		  	n_shm_count;
	T_SHM_BLOCK	*p_shm_buf;
	sem_t 			*p_shm_sem;
}T_SHM_WRITER;











#endif /*__TG_SHMW_H__*/
