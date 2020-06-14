

scd_t *InitScheduler();

int InitHandlers();

int InitSemaphores(char *app_path);

void StartWatching(char **argv, scd_t *scheduler, int semid, 
					char *who_m_i);
