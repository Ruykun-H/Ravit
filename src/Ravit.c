#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct box_size {
	int height;
	int width;
	char fnum;
};

struct num_data {
	int num;
	char empty;
};

struct box_size size;
int f_qt;
char** file_list;
int error_code;


int fopen_list(FILE* fp);

int fopen_main(FILE* fp);

int fopen_function(char** cmd_box, int y, int x, int fnum);


void stack_push(int* stack, int value) {
	if (stack[0] >= size.width - 1)
		printf("Stack full!\n");
	else
		stack[++stack[0]] = value;
}

int stack_top(int* stack) {
	if (stack[0] == 0)
		printf("Stack empty!\n");
	else
		return stack[stack[0]];
	return 0;
}

int stack_pop(int* stack) {
	if (stack[0] == 0)
		printf("Stack empty!\n");
	else
		return stack[stack[0]--];
	return 0;
}

int rand_dir() {
	srand((unsigned int)time(NULL));
	int num = rand() % 4;
	switch (num) {
	case 0:
		return 1;
	case 1:
		return 2;
	case 2:
		return 4;
	case 3:
		return 8;
	default:
		return 0;
	}
}


int main() {
	printf("Ravit - ver 0.01\n\n");
	FILE* fp = NULL;
	FILE* fp2 = NULL;

	if (0 == fopen_s(&fp, "list.rvt", "r")) {

		if (fopen_list(fp) == -1)
			goto ERROR;
		for (int i = 0; i < f_qt; i++) {
			printf("F%03d - %s\n", i, file_list[i]);
		}
		fclose(fp);
		
		if (0 == fopen_s(&fp2, file_list[0], "r")) {
			if (fopen_main(fp2) == -1)
				goto ERROR;

			char** cmd_box;
			cmd_box = (char**)malloc(sizeof(char*) * size.height);
			for (int i = 0; i < size.height; i++) {
				cmd_box[i] = (char*)malloc(sizeof(char) * size.width);
			}

			char* arr;
			arr = (char*)malloc(sizeof(char) * size.width + 1);

			for (int i = 0; i < size.height; i++) {
				for (int j = 0; j < size.width; j++) {
					arr[j] = '.';
				}
				fgets(arr, size.width + 1, fp2);

				for (int j = 0; j < size.width; j++) {
					if (arr[j] == ' ' || arr[j] == '\n' || arr[j] == '\0')
						cmd_box[i][j] = '.';
					else
						cmd_box[i][j] = arr[j];
				}
				if (strchr(arr, '\n') == NULL) {
					fgets(arr, size.width + 1, fp2);
				}
			}

			fclose(fp2);
			free(arr);

			for (int i = 0; i < size.height; i++) {
				for (int j = 0; j < size.width; j++) {
					printf("%c", cmd_box[i][j]);
				}
				printf("\n");
			}

			char** pointer_box;
			pointer_box = (char**)malloc(sizeof(char*) * size.height);
			for (int i = 0; i < size.height; i++) {
				pointer_box[i] = (char*)calloc(size.width, sizeof(char));
			}

			char** pointer_box_temp;
			pointer_box_temp = (char**)malloc(sizeof(char*) * (size.height + 1));
			for (int i = 0; i < size.height + 1; i++) {
				pointer_box_temp[i] = (char*)calloc(size.width + 1, sizeof(char));
			}

			struct num_data** data_box;
			data_box = (struct num_data**)malloc(sizeof(struct num_data*) * size.height);
			for (int i = 0; i < size.height; i++) {
				data_box[i] = (struct num_data*)calloc(size.width, sizeof(struct num_data));
			}

			int* stack;
			stack = (int*)malloc(sizeof(int) * size.width);
			stack[0] = 0;

			//main code
			int input = 0;
			int task = 0;
			int step = 0;
			while (1) {
				task++;
				step = 0;
				while (1) {
					int j = 0;

					printf("\nTask %d\n", task);
					printf("Input : ");
					scanf_s("%d", &input);
					if (input > 0 && input < size.width && cmd_box[0][input] == 'a') {
						pointer_box[0][input] = 2;
						j++;
					}
					else if (input == 0) {
						for (int i = 1; i < size.height; i++) {
							if (cmd_box[i][0] == 'A') {
								pointer_box[i][0] = 1;
								j++;
							}
						}
					}
					else if (input < 0)
						goto EXIT;
					if (j)
						break;
					else
						printf("Invalid Input!\n");
				}
				while (1) {
					step++;
					for (int i = 0; i < size.height; i++) {
						for (int j = 0; j < size.width; j++) {
							if (pointer_box[i][j] != 0) {
								if ((pointer_box[i][j] & 1) && (j + 1 < size.width)) {
									pointer_box_temp[i][j + 1] += 1;
								}
								if ((pointer_box[i][j] & 2) && (i + 1 < size.height)) {
									pointer_box_temp[i + 1][j] += 2;
								}
								if ((pointer_box[i][j] & 4) && (j - 1 > 0)) {
									pointer_box_temp[i][j - 1] += 4;
								}
								if ((pointer_box[i][j] & 8) && (i - 1 > 0)) {
									pointer_box_temp[i - 1][j] += 8;
								}
							}
						}
					}
					for (int i = 0; i < size.height; i++) {
						for (int j = 0; j < size.width; j++) {
							pointer_box[i][j] = pointer_box_temp[i][j];
							pointer_box_temp[i][j] = 0;
							if (pointer_box[i][j]) {
								if (cmd_box[i][j] == 'f' && data_box[i][j].empty) {
									if (data_box[i][j].num >= 0 && data_box[i][j].num < f_qt) {
										if (file_list[data_box[i][j].num][0]) {
											if (fopen_function(cmd_box, i, j, data_box[i][j].num) == -1)
												goto ERROR;
										}
									}
								}
							}
						}
					}
					for (int i = 0; i < size.height; i++) {
						for (int j = 0; j < size.width; j++) {
							if (pointer_box[i][j]) {
								if (cmd_box[i][j] == 'X') {
									pointer_box[i][j] = 0;
								}
								else if (cmd_box[i][j] == 'D' && data_box[i][j].empty == 0) {
									pointer_box[i][j] = 0;
								}
								else if (cmd_box[i][j] == '>') {
									pointer_box[i][j] = 1;
								}
								else if (cmd_box[i][j] == 'v') {
									pointer_box[i][j] = 2;
								}
								else if (cmd_box[i][j] == '<') {
									pointer_box[i][j] = 4;
								}
								else if (cmd_box[i][j] == '^') {
									pointer_box[i][j] = 8;
								}
								else if (cmd_box[i][j] == 'V') {
									pointer_box[i][j] = rand_dir();
								}
								else if (cmd_box[i][j] == 'A') {
									if (!(pointer_box[i][j] & 1)) {
										pointer_box[i][j] += 1;
									}
								}
								else if (cmd_box[i][j] == 'a') {
									if (!(pointer_box[i][j] & 2)) {
										pointer_box[i][j] += 2;
									}
								}
								else if (cmd_box[i][j] == 'B') {
									if (!(pointer_box[i][j] & 4)) {
										pointer_box[i][j] += 4;
									}
								}
								else if (cmd_box[i][j] == 'b') {
									if (!(pointer_box[i][j] & 8)) {
										pointer_box[i][j] += 8;
									}
								}
								else if (cmd_box[i][j] == 'd') {
									cmd_box[i][j] = '_';
								}
								else if (cmd_box[i][j] >= '0' && cmd_box[i][j] <= '9') {
									data_box[i][j].num = cmd_box[i][j] - 48;
									data_box[i][j].empty = 1;
								}
								else if (cmd_box[i][j] == 'E') {
									data_box[i][j].num = stack_pop(stack);
									data_box[i][j].empty = 1;
								}
								else if (cmd_box[i][j] == 'e') {
									data_box[i][j].num = stack_top(stack);
									data_box[i][j].empty = 1;
								}
								else if (cmd_box[i][j] == 'O') {
									data_box[i][j].num = 0;
									data_box[i][j].empty = 0;
								}
							}
						}
					}
					/*
					for (int i = 0; i < size.height; i++) {
						for (int j = 0; j < size.width; j++) {
							printf("%d", pointer_box[i][j]);
						}
						printf("\n");
					}
					printf("\n");
					*/
					char grav_dir = 0;
					char grav_op = 0;
					for (int i = 0; i < size.height; i++) {
						for (int j = 0; j < size.width; j++) {
							if (pointer_box[i][j] != 0) {
								switch (cmd_box[i][j]) {
								case 'G':
								case 'I':
								case 'K':
								case 'M':
								case 'T':
								case 'W':
									if (grav_dir == 0 || grav_dir == 1)
										grav_dir = 1;
									else
										grav_dir = -1;
									break;
								case 'g':
								case 'i':
								case 'k':
								case 'm':
								case 't':
								case 'w':
									if (grav_dir == 0 || grav_dir == 2)
										grav_dir = 2;
									else
										grav_dir = -1;
									break;
								case 'H':
								case 'J':
								case 'L':
								case 'N':
								case 'U':
								case 'Y':
									if (grav_dir == 0 || grav_dir == 4)
										grav_dir = 4;
									else
										grav_dir = -1;
									break;
								case 'h':
								case 'j':
								case 'l':
								case 'n':
								case 'u':
								case 'y':
									if (grav_dir == 0 || grav_dir == 8)
										grav_dir = 8;
									else
										grav_dir = -1;
									break;
								}
								switch (cmd_box[i][j]) {
								case 'I':
								case 'i':
								case 'J':
								case 'j':
									if (grav_op == 0 || grav_op == 1)
										grav_op = 1;
									else
										grav_op = -1;
									break;
								case 'K':
								case 'k':
								case 'L':
								case 'l':
									if (grav_op == 0 || grav_op == 2)
										grav_op = 2;
									else
										grav_op = -1;
									break;
								case 'M':
								case 'm':
								case 'N':
								case 'n':
									if (grav_op == 0 || grav_op == 3)
										grav_op = 3;
									else
										grav_op = -1;
									break;
								case 'T':
								case 't':
								case 'U':
								case 'u':
									if (grav_op == 0 || grav_op == 4)
										grav_op = 4;
									else
										grav_op = -1;
									break;
								case 'W':
								case 'w':
								case 'Y':
								case 'y':
									if (grav_op == 0 || grav_op == 5)
										grav_op = 5;
									else
										grav_op = -1;
									break;
								}
							}
						}
					}
					switch (grav_dir) {
					case 1:
						for (int i = 1; i < size.height; i++) {
							for (int j = (size.width - 2); j > 0; j--) {
								if (data_box[i][j].empty) {
									for (int k = j; k < size.width; k++) {
										if (k != size.width - 1) {
											if (data_box[i][k + 1].empty) {
												switch (grav_op) {
												case 1:
													data_box[i][k + 1].num += data_box[i][j].num;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
													k = size.width;
													break;
												case 2:
													data_box[i][k + 1].num -= data_box[i][j].num;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
													k = size.width;
													break;
												case 3:
													data_box[i][k + 1].num *= data_box[i][j].num;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
													k = size.width;
													break;
												case 4:
													data_box[i][k + 1].num /= data_box[i][j].num;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
													k = size.width;
													break;
												case 5:
													data_box[i][k + 1].num %= data_box[i][j].num;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
													k = size.width;
													break;
												default:
													if (k != j) {
														data_box[i][k].num = data_box[i][j].num;
														data_box[i][k].empty = 1;
														data_box[i][j].num = 0;
														data_box[i][j].empty = 0;
													}
													k = size.width;
												}
											}
											else if (cmd_box[i][k + 1] == 'x' || cmd_box[i][k + 1] == 'd') {
												if (k != j) {
													data_box[i][k].num = data_box[i][j].num;
													data_box[i][k].empty = 1;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
												}
												k = size.width;
											}
											else if (cmd_box[i][k + 1] == 'S' && data_box[i][j].num < 0
												|| cmd_box[i][k + 1] == 's' && data_box[i][j].num >= 0
												|| cmd_box[i][k + 1] == 'Z' && data_box[i][j].num == 0
												|| cmd_box[i][k + 1] == 'z' && data_box[i][j].num != 0) {
												if (k != j) {
													data_box[i][k].num = data_box[i][j].num;
													data_box[i][k].empty = 1;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
												}
												k = size.width;
											}
										}
										else {
											if (k != j) {
												data_box[i][k].num = data_box[i][j].num;
												data_box[i][k].empty = 1;
												data_box[i][j].num = 0;
												data_box[i][j].empty = 0;
											}
										}
									}
								}
							}
						}
						break;
					case 2:
						for (int i = (size.height - 2); i > 0; i--) {
							for (int j = 1; j < size.width; j++) {
								if (data_box[i][j].empty) {
									for (int k = i; k < size.height; k++) {
										if (k != size.height - 1) {
											if (data_box[k + 1][j].empty) {
												switch (grav_op) {
												case 1:
													data_box[k + 1][j].num += data_box[i][j].num;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
													k = size.height;
													break;
												case 2:
													data_box[k + 1][j].num -= data_box[i][j].num;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
													k = size.height;
													break;
												case 3:
													data_box[k + 1][j].num *= data_box[i][j].num;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
													k = size.height;
													break;
												case 4:
													data_box[k + 1][j].num /= data_box[i][j].num;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
													k = size.height;
													break;
												case 5:
													data_box[k + 1][j].num %= data_box[i][j].num;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
													k = size.height;
													break;
												default:
													if (k != i) {
														data_box[k][j].num = data_box[i][j].num;
														data_box[k][j].empty = 1;
														data_box[i][j].num = 0;
														data_box[i][j].empty = 0;
													}
													k = size.height;
												}
											}
											else if (cmd_box[k + 1][j] == 'x' || cmd_box[k + 1][j] == 'd') {
												if (k != i) {
													data_box[k][j].num = data_box[i][j].num;
													data_box[k][j].empty = 1;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
												}
												k = size.height;
											}
											else if (cmd_box[k + 1][j] == 'S' && data_box[i][j].num < 0
												|| cmd_box[k + 1][j] == 's' && data_box[i][j].num >= 0
												|| cmd_box[k + 1][j] == 'Z' && data_box[i][j].num == 0
												|| cmd_box[k + 1][j] == 'z' && data_box[i][j].num != 0) {
												if (k != j) {
													data_box[i][k].num = data_box[i][j].num;
													data_box[i][k].empty = 1;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
												}
												k = size.width;
											}
										}
										else {
											if (k != i) {
												data_box[k][j].num = data_box[i][j].num;
												data_box[k][j].empty = 1;
												data_box[i][j].num = 0;
												data_box[i][j].empty = 0;
											}
										}
									}
								}
							}
						}
						break;
					case 4:
						for (int i = 1; i < size.height; i++) {
							for (int j = 2; j < size.width; j++) {
								if (data_box[i][j].empty) {
									for (int k = j; k > 0; k--) {
										if (k != 1) {
											if (data_box[i][k - 1].empty) {
												switch (grav_op) {
												case 1:
													data_box[i][k - 1].num += data_box[i][j].num;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
													k = 0;
													break;
												case 2:
													data_box[i][k - 1].num -= data_box[i][j].num;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
													k = 0;
													break;
												case 3:
													data_box[i][k - 1].num *= data_box[i][j].num;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
													k = 0;
													break;
												case 4:
													data_box[i][k - 1].num /= data_box[i][j].num;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
													k = 0;
													break;
												case 5:
													data_box[i][k - 1].num %= data_box[i][j].num;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
													k = 0;
													break;
												default:
													if (k != j) {
														data_box[i][k].num = data_box[i][j].num;
														data_box[i][k].empty = 1;
														data_box[i][j].num = 0;
														data_box[i][j].empty = 0;
													}
													k = 0;
												}
											}
											else if (cmd_box[i][k - 1] == 'x' || cmd_box[i][k - 1] == 'd') {
												if (k != j) {
													data_box[i][k].num = data_box[i][j].num;
													data_box[i][k].empty = 1;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
												}
												k = 0;
											}
											else if (cmd_box[i][k - 1] == 'S' && data_box[i][j].num < 0
												|| cmd_box[i][k - 1] == 's' && data_box[i][j].num >= 0
												|| cmd_box[i][k - 1] == 'Z' && data_box[i][j].num == 0
												|| cmd_box[i][k - 1] == 'z' && data_box[i][j].num != 0) {
												if (k != j) {
													data_box[i][k].num = data_box[i][j].num;
													data_box[i][k].empty = 1;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
												}
												k = size.width;
											}
										}
										else {
											if (k != j) {
												data_box[i][k].num = data_box[i][j].num;
												data_box[i][k].empty = 1;
												data_box[i][j].num = 0;
												data_box[i][j].empty = 0;
											}
											k = 0;
										}
									}
								}
							}
						}
						break;
					case 8:
						for (int i = 2; i < size.height; i++) {
							for (int j = 1; j < size.width; j++) {
								if (data_box[i][j].empty) {
									for (int k = i; k > 0; k--) {
										if (k != 1) {
											if (data_box[k - 1][j].empty) {
												switch (grav_op) {
												case 1:
													data_box[k - 1][j].num += data_box[i][j].num;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
													k = 0;
													break;
												case 2:
													data_box[k - 1][j].num -= data_box[i][j].num;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
													k = 0;
													break;
												case 3:
													data_box[k - 1][j].num *= data_box[i][j].num;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
													k = 0;
													break;
												case 4:
													data_box[k - 1][j].num /= data_box[i][j].num;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
													k = 0;
													break;
												case 5:
													data_box[k - 1][j].num %= data_box[i][j].num;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
													k = 0;
													break;
												default:
													if (k != i) {
														data_box[k][j].num = data_box[i][j].num;
														data_box[k][j].empty = 1;
														data_box[i][j].num = 0;
														data_box[i][j].empty = 0;
													}
													k = 0;
												}
											}
											else if (cmd_box[k - 1][j] == 'x' || cmd_box[k - 1][j] == 'd') {
												if (k != i) {
													data_box[k][j].num = data_box[i][j].num;
													data_box[k][j].empty = 1;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
												}
												k = 0;
											}
											else if (cmd_box[k - 1][j] == 'S' && data_box[i][j].num < 0
												|| cmd_box[k - 1][j] == 's' && data_box[i][j].num >= 0
												|| cmd_box[k - 1][j] == 'Z' && data_box[i][j].num == 0
												|| cmd_box[k - 1][j] == 'z' && data_box[i][j].num != 0) {
												if (k != j) {
													data_box[i][k].num = data_box[i][j].num;
													data_box[i][k].empty = 1;
													data_box[i][j].num = 0;
													data_box[i][j].empty = 0;
												}
												k = size.width;
											}
										}
										else {
											if (k != i) {
												data_box[k][j].num = data_box[i][j].num;
												data_box[k][j].empty = 1;
												data_box[i][j].num = 0;
												data_box[i][j].empty = 0;
											}
											k = 0;
										}
									}
								}
							}
						}
						break;
					}
					//gravity end
					/*
					for (int i = 0; i < size.height; i++) {
						for (int j = 0; j < size.width; j++) {
							printf("%d", data_box[i][j].num);
						}
						printf("\n");
					}
					printf("\n");
					*/
					for (int i = 0; i < size.height; i++) {
						for (int j = 0; j < size.width; j++) {
							if (data_box[i][j].empty) {
								switch (cmd_box[i][j]) {
								case 'p':
									if (data_box[i][j].empty == 1) {
										stack_push(stack, data_box[i][j].num);
										data_box[i][j].empty++;
									}
									break;
								case 'P':
									stack_push(stack, data_box[i][j].num);
									data_box[i][j].num = 0;
									data_box[i][j].empty = 0;
									break;
								case 'o':
									data_box[i][j].num = 0;
									data_box[i][j].empty = 0;
									break;

								}
							}
						}
					}

					for (int i = 0; i < size.height; i++) {
						for (int j = 0; j < size.width; j++) {
							if (pointer_box[i][j]) {
								switch (cmd_box[i][j]) {
								case 'C':
									while (stack[0])
										stack_pop(stack);
									break;
								case 'c':
									stack_pop(stack);
									break;
								case 'Q':
									while (stack[0])
										printf("%d", stack_pop(stack));
									break;
								case 'q':
									printf("%d", stack_pop(stack));
									break;
								case 'R':
									while (stack[0])
										printf("%c", stack_pop(stack));
									break;
								case 'r':
									printf("%c", stack_pop(stack));
									break;
								}
							}
						}
					}

					int p_empty = 0;
					for (int i = 0; i < size.height; i++) {
						for (int j = 0; j < size.width; j++) {
							if (pointer_box[i][j] != 0)
								p_empty += 1;
						}
					}
					if (p_empty == 0) {
						break;
					}
					//step end
				}

				for (int i = 0; i < size.height; i++) {
					for (int j = 0; j < size.width; j++) {
						if (cmd_box[i][j] == '_')
							cmd_box[i][j] = 'd';
					}
				}
				printf("\nTotal steps : %d\n", step);
				//task end
			}
			//main code end
		EXIT:
			printf("End program...\n");

			for (int i = 0; i < size.height; i++) {
				free(cmd_box[i]);
			}
			free(cmd_box);

			for (int i = 0; i < size.height; i++) {
				free(pointer_box[i]);
			}
			free(pointer_box);

			for (int i = 0; i < size.height; i++) {
				free(pointer_box_temp[i]);
			}
			free(pointer_box_temp);

			for (int i = 0; i < size.height; i++) {
				free(data_box[i]);
			}
			free(data_box);

			free(stack);
		}
		else {
		error_code = 2;
		printf("\nFunction 000 not found");
		goto ERROR;
		}
	}
	else {
	error_code = 1;
	printf("\nlist.rvt not found");
	ERROR:
		printf("\nERROR!\n");
		printf("Error code %d\n", error_code);
		int any_number;
		printf("Enter any number to end program.\n");
		scanf_s("%d", &any_number);
	}
	return 0;
}


int fopen_list(FILE* fp) {
	char buffer[50];

	fgets(buffer, sizeof(buffer), fp);
	printf("%s", buffer);

	if (!strncmp("File Quantity :", buffer, 15)) {
		char f_qt_a[5];
		for (int i = 0; i < 5; i++) {
			if (buffer[i + 15] != '\n')
				f_qt_a[i] = buffer[i + 15];
			else {
				f_qt_a[i] = '\0';
				break;
			}
		}
		f_qt = atoi(f_qt_a);
		if (f_qt <= 0) {
			printf("\nFile Quantity not defined");
			error_code = 4;
			goto ERROR;
		}

		file_list = (char**)malloc(sizeof(char*) * f_qt);
		for (int i = 0; i < f_qt; i++) {
			file_list[i] = (char*)malloc(sizeof(char) * 50);
			file_list[i][0] = '\0';
		}
		for (int i = 0; i < f_qt; i++) {
			FILE* fp2 = NULL;
			fgets(buffer, sizeof(buffer), fp);
			char* pt_temp = strchr(buffer, '\n');
			if (pt_temp != NULL)
				*pt_temp = '\0';
			if (0 == fopen_s(&fp2, buffer, "r")) {
				char buffer2[5];
				fgets(buffer2, sizeof(buffer2), fp2);
				if (!strncmp("F", buffer2, 1)) {
					char fnum_a[4];
					char fnum;
					for (int j = 0; j < 3; j++) {
						fnum_a[j] = buffer2[j + 1];
					}
					fnum_a[3] = '\0';
					fnum = atoi(fnum_a);
					if (fnum < f_qt) {
						if (file_list[fnum][0] == '\0') {
							for (int j = 0; j < 50 - 1; j++) {
								if (buffer[j] != '\n')
									file_list[fnum][j] = buffer[j];
								else {
									file_list[fnum][j] = '\0';
									break;
								}
							}
							file_list[fnum][50 - 1] = '\0';
						}
						else
							printf("Function number overlapped! - %s\n", buffer);
					}
				}
				else {
					printf("\n%s - Can not read", buffer);
					error_code = 6;
					goto ERROR;
				}
			}
			else {
				error_code = 5;
				printf("\n%s - File not found!", buffer);
				goto ERROR;
			}
		}
		return 0;
	}
	else {
		error_code = 4;
		printf("\nFile Quantity not defined");
	ERROR:
		return -1;
	}
	return 0;
}

int fopen_main(FILE* fp) {
	//struct box_size size;
	char buffer[20];

	fgets(buffer, sizeof(buffer), fp);
	printf("\n%s", buffer);

	if (!strncmp("F", buffer, 1)) {
		char* bp1 = strchr(buffer, '*');
		char* bp2 = strchr(buffer, '\n');
		if (bp1 == NULL || bp2 == NULL)
			goto ERROR;

		char t_f[4] = { 0 };
		for (int i = 1; i < 4; i++) {
			t_f[i] = buffer[i];
		}
		size.fnum = atoi(t_f);

		char t_h[20] = { 0 };
		for (int i = 0; i < (bp1 - &buffer[0] - 4); i++) {
			t_h[i] = buffer[4 + i];
		}
		size.height = atoi(t_h);

		char t_w[20] = { 0 };
		for (int i = 0; i < (bp2 - &buffer[0] - 4); i++) {
			t_w[i] = buffer[bp1 - &buffer[0] + i + 1];
		}
		size.width = atoi(t_w);
	}
	else {
	ERROR:
		printf("\nFunction 000 can not read");
		error_code = 3;
		return -1;
	}
	return 0;
}

int fopen_function(char** cmd_box, int y, int x, int fnum) {
	FILE* fp = NULL;

	if (0 == fopen_s(&fp, file_list[fnum], "r")) {
		char buffer[20];
		fgets(buffer, sizeof(buffer), fp);

		if (!strncmp("F", buffer, 1)) {
			int height, width;
			char* bp1 = strchr(buffer, '*');
			char* bp2 = strchr(buffer, '\n');
			if (bp1 == NULL || bp2 == NULL)
				goto ERROR;

			char t_h[20] = { 0 };
			for (int i = 0; i < (bp1 - &buffer[0] - 4); i++) {
				t_h[i] = buffer[4 + i];
			}
			height = atoi(t_h);

			char t_w[20] = { 0 };
			for (int i = 0; i < (bp2 - &buffer[0] - 4); i++) {
				t_w[i] = buffer[bp1 - &buffer[0] + i + 1];
			}
			width = atoi(t_w);

			char* arr;
			arr = (char*)malloc(sizeof(char) * size.width + 1);
			fgets(arr, width + 1, fp);
			if (strchr(arr, '\n') == NULL) {
				fgets(arr, size.width + 1, fp);
			}
			for (int i = 1; i < height && (y + i) < size.height; i++) {
				for (int j = 0; j < size.width; j++) {
					arr[j] = ' ';
				}
				fgets(arr, width + 1, fp);
				for (int j = 1; j < width && (x + j) < size.width; j++) {
					if (!(arr[j] == ' ' || arr[j] == '\n' || arr[j] == '\0'))
						cmd_box[y + i][x + j] = arr[j];
				}
				if (strchr(arr, '\n') == NULL) {
					fgets(arr, size.width + 1, fp);
				}
			}
			free(arr);
		}
	}
	else {
	ERROR:
		printf("\nFunction %03d not found", fnum);
		error_code = 7;
		if (fp != NULL)
			fclose(fp);
		return -1;
	}
	if (fp != NULL)
		fclose(fp);
	return 0;
}