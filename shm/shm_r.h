#ifndef __SHM_R_H__ 
#define __SHM_R_H__

#define TEST_SHM_MTU 1500
#define TEST_SHM_KEY (5689)
#define TEST_SHM_SEM "/test_shm_sem"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;




typedef struct{
	u8 block[TEST_SHM_MTU];
	
}T_SHM_R;

#endif // __SHM_R_H__