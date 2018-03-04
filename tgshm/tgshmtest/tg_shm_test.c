
/* 
 * 共享内存读写速度测试 
 */  
#include <unistd.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <sys/ipc.h>  
#include <sys/mman.h>  
#include <sys/types.h>  
#include <sys/wait.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <sys/ipc.h>  
#include <sys/mman.h>  
#include <sys/types.h>  
#include <sys/wait.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <errno.h>  
#include <unistd.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <sys/ipc.h>  
#include <sys/shm.h>  
#include <sys/types.h>  
#include <sys/wait.h>  

  
#define FILE_MODE (S_IRUSR | S_IWUSR)  
#define SVSHM_MODE (SHM_R | SHM_W | SHM_R>>3 | SHM_R>>6)  

void * GetShmMem(int size)  
{  
    void * ptr = NULL;  
	int shmid;    
	struct shmid_ds buff;  
	const char shmfile[] = "./tmp.shm_v";  
	shmid = shmget(ftok(shmfile, 0), size, SVSHM_MODE | IPC_CREAT);  
	if (shmid < 0)  
	{  
		printf("shmget failed, errormsg=%s errno=%d/n", strerror(errno), errno);  
		return NULL;  
	}  
	  
	ptr = (int *) shmat(shmid, NULL, 0);  
	if ((void *) -1 == ptr)  
	{  
		printf("shmat failed, errormsg=%s errno=%d/n", strerror(errno), errno);  
		return NULL;  
	}  
	  
	shmctl(shmid, IPC_STAT, &buff);  
  
    return ptr;  
}  
  
int realmain(int size, int loop)  
{  
    int * array_int = NULL;  
  
    /* get shmmem*/  
    array_int = (int *)GetShmMem(size);  
    if (NULL == array_int)  
    {  
        printf("GetShmMem failed/n");  
        return -1;  
    }  
  
    /* loop */  
    int array_num = size/sizeof(int);  
    while (0 != loop)  
    {  
        /* write */  
        for (int i = 0; i < array_num; i++)  
        {  
            array_int[i] = i;  
        }  
  
        /* read */  
        for (int i = 0; i < array_num; i++)  
        {  
            if (array_int[i] != i)  
            {  
                printf("ShmMem is invalid i=%d v=%d/n", i, array_int[i]);  
                return -1;  
            }  
        }  
      
        loop--;  
    }  
    printf("timecost=%lld/n", modulecall::call_timecost());  
  
    return 0;  
}  
  
int main()  
{  
    const int size = 2048;  
    int loop = 100000;  
	printf("shmget  ");  
	realmain(size, loop);  
    return 0;  
}  