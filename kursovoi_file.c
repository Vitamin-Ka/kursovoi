#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

// �������� ���������
typedef struct {
	char manufacturer[20];
	float diagonal;
	int price;
	char country[20];
	int angle;
} display_t;

//���������� �������
int init(display_t* sir);
int in_file(display_t* a, int size); 
display_t* from_file(int* size); 
int put(display_t a, int index); 
int put_find(display_t* s, int size);
int edit(display_t* array, int size);
int compare_manuf(display_t* a, display_t* b); 
int compare_angle(display_t* a, display_t* b); 
int sort_mass(display_t* items, int size); 
display_t* delete_one(display_t* items, int* size); 
int is_number(const char* str);


//������� �������
int main() {
	setlocale(LC_ALL, "RUS");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	display_t* s = NULL;
	int size = 0;
	int how_many_new = 0;

	printf("��� ������������ ��������� ����-����, ��� � �����������:");
	int b = -1;
	while (b != 0) {
		puts("");
		printf("1. �������� ������\n"); 
		printf("2. ����� ������ �� ���������\n"); 
		printf("3. �������� � ����\n"); 
		printf("4. ������� �� �����\n"); 
		printf("5. ����������� ������ �� ���������\n"); 
		printf("6. �������� ���� �� �������\n"); 
		printf("7. ���������� ��� ������\n"); 
		printf("8. ������� ������\n");
		printf("0. ����� �� ���������\n"); 
		printf("������� ����� ������ ��������: ");
		scanf("%d", &b);
		puts("");

		if (b > 8 || b < 0) {
			printf("������ �������� �� ����������!\n");
			continue;
		}

		switch (b) {
		case (1): {
			printf("������� ������� �� ������ ��������? -- ");
			scanf("%d", &how_many_new);
			getchar();
			display_t* new_arr = (display_t*)realloc(s, (size + how_many_new) * sizeof(display_t));
			s = new_arr;

			for (int i = size; i < size + how_many_new; i++) {
				init(&s[i]);
			}
			size += how_many_new;
			puts("");
			printf("�� �������� ������!\n");
			continue;
		}
		case (2): {
			put_find(s, size);
			continue;
			}
		case (3): {
			in_file(s, size);
			continue;
		}
		case (4): {
			s = from_file(&size);
			printf("�� ������� ������ �� �����!\n");
			continue;
		}
		case (5): {
			sort_mass(s, size);
			continue;
		}
		case (6): {
			edit(s, size);
			continue;
		}
		case (7): {
			for (int i = 0; i < size; i++) {
				put(s[i], i + 1);
			}
			continue;
		}
		case (8): {
			s = delete_one(s, &size);
			continue;
		}
		case (0): {
			printf("�� ����� �� ���������.\n");
			break;
		}
		}
	}
	return 1;
}


// ���������� ����� ������ � ������
int init(display_t* sir) {
	puts("");
	while (1) {
		printf("������� �������������: ");
		fgets(sir->manufacturer, sizeof(sir->manufacturer), stdin);
		sir->manufacturer[strcspn(sir->manufacturer, "\n")] = 0;
		if (strlen(sir->manufacturer) > 0) break;
		printf("������: ������������� �� ����� ���� ������. ����������, ������� �����.\n");
	}

	while (1) {
		char input[20];
		printf("������� ���������: ");
		fgets(input, sizeof(input), stdin);
		input[strcspn(input, "\n")] = 0;

		if (is_number(input)) {
			sir->diagonal = atof(input);
			break;
		}
		else {
			printf("������: ������� ���������� �������� ��� ���������.\n");
		}
	}

	while (1) {
		char input[20];
		printf("������� ����: ");
		fgets(input, sizeof(input), stdin);
		input[strcspn(input, "\n")] = 0;

		if (is_number(input)) {
			sir->price = atoi(input);
			break;
		}
		else {
			printf("������: ������� ���������� �������� ��� ����.\n");
		}
	}

	while (1) {
		printf("������� ������ ������������: ");
		fgets(sir->country, sizeof(sir->country), stdin);
		sir->country[strcspn(sir->country, "\n")] = 0;
		if (strlen(sir->country) > 0) break;
		printf("������: ������ �� ����� ���� ������. ����������, ������� �����.\n");
	}

	while (1) {
		char input[20];
		printf("������� ���� ������: ");
		fgets(input, sizeof(input), stdin);
		input[strcspn(input, "\n")] = 0;

		if (is_number(input)) {
			sir->angle = atoi(input);
			break;
		}
		else {
			printf("������: ������� ���������� �������� ��� ����.\n");
		}
	}
	return 1;
}
// ������ ������� � ����
int in_file(display_t* a, int size) {

	FILE* str = fopen("base.txt", "w");

	for (int i = 0; i < size; i++) {
		fprintf(str, "%s,%.1f,%d,%s,%d\n", a[i].manufacturer, a[i].diagonal, a[i].price, a[i].country, a[i].angle);
	}
	fclose(str);

	printf("�� �������� ������ � ����!");
	puts("");
	return 1;
}
// ������ ������� �� �����
display_t* from_file(int* size) {
	FILE* file = fopen("base.txt", "r");
	if (!file) {
		perror("������ �������� �����");
		return NULL;
	}

	display_t* items = NULL;

	*size = 0;

	while (1) {
		display_t* temp = realloc(items, (*size + 1) * sizeof(display_t));
		if (temp == NULL) {
			perror("������ ��������� ������");
			free(items);
			fclose(file);
			return NULL;
		}
		items = temp;

		if (fscanf(file, "%[^,],%f,%d,%[^,],%d\n",
			items[*size].manufacturer,
			&items[*size].diagonal,
			&items[*size].price,
			items[*size].country,
			&items[*size].angle) != 5) {
			break;
		}

		items[*size].manufacturer[19] = '\0'; 
		items[*size].country[19] = '\0';
		(*size)++;
	}

	fclose(file);
	return items;
}
// ������ ����� ������
int put(display_t a, int index) {
	printf("(%d). �������������: %s; ���������: %.1f; ����: %d; ������: %s; ����: %d\n", index, a.manufacturer, a.diagonal, a.price, a.country, a.angle);
	return 1;
}
// ����� � ����������
int put_find(display_t* s, int size) {

	float diagonal;
	char manuf[20];
	int choice, flag = 0;

	printf("1. �� �������������\n");
	printf("2. �� ��������� ������\n");
	printf("3. �� ������������� � ��������� ������\n");
	printf("�� ����� ���������� �� ������ ������? -- ");
	scanf("%d", &choice);


	switch (choice) {
	case(1): {
		printf("������� �������� �������������: ");
		scanf("%s", manuf);

		for (int i = 0; i < size; i++) {
			if (strcmp(s[i].manufacturer, manuf) == 0) {
				put(s[i], i + 1);
				flag = 1;
			}
		}
		break;
	}
	case(2): {
		printf("������� ������� �����: ");
		scanf("%f", &diagonal);

		for (int i = 0; i < size; i++) {
			if (s[i].diagonal == diagonal) {
				put(s[i], i + 1);
				flag = 1;
			}
		}
		break;
	}
	case(3): {
		printf("������� �������� �������������: ");
		scanf("%s", manuf);
		printf("������� ������� �����: ");
		scanf("%f", &diagonal);

		for (int i = 0; i < size; i++) {
			if (strcmp(s[i].manufacturer, manuf) == 0 && s[i].diagonal == diagonal) {
				put(s[i], i + 1);
				flag = 1;
			}
		}
		break;
	}
	default: {
		printf("�������� ������� ������!");
		return;
	}
	}

	if (!flag) {
		printf("�������� �� �������.\n");
	}
	return 1;
}
// �������� ���� ������
int edit(display_t* array, int size) {
	int index;
	printf("������� ����� �������� ��� ���������: ");
	scanf("%d", &index);
	getchar();
	puts("");

	if (index < 1 || index > size) {
		printf("������ �������� ���!\n");
		return;
	}

	index--;
	printf("������� ������ �������� %d:\n", index + 1);
	put(array[index], index + 1);
	puts("");
	puts("**���������� ������**");
	init(&array[index]);

	printf("������ ������� ���������.\n");
	return 1;
}
// ����������
int compare_manuf(display_t* a, display_t* b) {
	return strcmp(a->manufacturer, b->manufacturer);
}
int compare_angle(display_t* a, display_t* b) {
	return (a->angle - b->angle);
}
int sort_mass(display_t* items, int size) {
	int choise = -1;

	printf("1. �� �������������\n");
	printf("2. �� ���� ������\n");
	printf("����� �������� �� ������ �������������? � ");
	scanf("%d", &choise);

	if (choise == 1) {
		qsort(items, size, sizeof(display_t), (int (*)(const void*, const void*)) compare_manuf);
		printf("�������� ������������� �� �������������.\n");
	}
	else if (choise == 2) {
		qsort(items, size, sizeof(display_t), (int (*)(const void*, const void*)) compare_angle);
		printf("�������� ������������� �� ����.\n");
	}
	else {
		printf("������������ �����.\n");
	}
	return 1;
}
// ��������
display_t* delete_one(display_t* items, int* size) {
	int index;

	printf("������� ������ �������� ��� �������� (1 �� %d): ", *size);
	scanf("%d", &index);

	if (index < 1 || index > *size) {
		printf("������������ ������.\n");
		return items;
	}

	for (int i = index - 1; i < *size - 1; i++) {
		items[i] = items[i + 1];
	}

	(*size)--;

	if (*size > 0) {
		display_t* temp = realloc(items, (*size) * sizeof(display_t));
		if (temp != NULL) {
			puts("�� ������� �������!");
			return temp;
		}
		else {
			printf("������ ��������� ������.\n");
			return items;
		}
	}
	else {
		free(items);
		puts("������� ������ ���!");
		return NULL;
	}
	puts("");
}
// �������� �����
int is_number(const char* str) {
	int decimal_point_count = 0;

	while (*str) {
		if (isdigit(*str)) {
			str++;
			continue;
		}
		if (*str == '.' || *str == ',') {
			decimal_point_count++;
			if (decimal_point_count > 1) {
				return 0; 
			}
			str++;
			continue;
		}
		return 0;
	}
	return 1; 
}