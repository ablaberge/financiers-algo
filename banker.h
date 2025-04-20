/* declarations related to banker's algorithm */
void isSafe(int *available, int **alloc_matrix, int **need_matrix, int length, int width);
void findSafeSchedules(int *available, int **alloc_matrix, int **need_matrix, int length, int width, int *finish, int *schedule, int step);
