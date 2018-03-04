#ifndef __TG_SHMR_H__
#define __TG_SHMR_H__



typedef struct tg_shm_reader
{
	int 			  	shm_id;
	u32  		  	 n_shm_count;
	T_SHM_BLOCK	*p_shm_buf;
	sem_t 			*p_shm_sem;
}T_SHM_READER;




#endif /*__TG_SHMR_H__*/
