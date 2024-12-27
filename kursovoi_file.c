#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

// ОСНОВНАЯ СТРУКТУРА
typedef struct {
	char manufacturer[20];
	float diagonal;
	int price;
	char country[20];
	int angle;
} display_t;

//ОБЪЯВЛЕНИЯ ФУНКЦИЙ
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


//ГЛАВНАЯ ФУНКЦИЯ
int main() {
	setlocale(LC_ALL, "RUS");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	display_t* s = NULL;
	int size = 0;
	int how_many_new = 0;

	printf("Вас приветствует программа Чудо-файл, вот её возможности:");
	int b = -1;
	while (b != 0) {
		puts("");
		printf("1. Добавить записи\n"); 
		printf("2. Найти записи по значениям\n"); 
		printf("3. Записать в файл\n"); 
		printf("4. Считать из файла\n"); 
		printf("5. Упорядочить записи по значениям\n"); 
		printf("6. Изменить одну из записей\n"); 
		printf("7. Напечатать все записи\n"); 
		printf("8. Удалить запись\n");
		printf("0. Выйти из программы\n"); 
		printf("Введите номер вашего действия: ");
		scanf("%d", &b);
		puts("");

		if (b > 8 || b < 0) {
			printf("Такого действия не существует!\n");
			continue;
		}

		switch (b) {
		case (1): {
			printf("Сколько записей вы хотите добавить? -- ");
			scanf("%d", &how_many_new);
			getchar();
			display_t* new_arr = (display_t*)realloc(s, (size + how_many_new) * sizeof(display_t));
			s = new_arr;

			for (int i = size; i < size + how_many_new; i++) {
				init(&s[i]);
			}
			size += how_many_new;
			puts("");
			printf("Вы добавили записи!\n");
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
			printf("Вы считали данные из файла!\n");
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
			printf("Вы вышли из программы.\n");
			break;
		}
		}
	}
	return 1;
}


// ДОБАВЛЕНИЕ ОДНОЙ ЗАПИСИ В МАССИВ
int init(display_t* sir) {
	puts("");
	while (1) {
		printf("Введите производителя: ");
		fgets(sir->manufacturer, sizeof(sir->manufacturer), stdin);
		sir->manufacturer[strcspn(sir->manufacturer, "\n")] = 0;
		if (strlen(sir->manufacturer) > 0) break;
		printf("Ошибка: производитель не может быть пустым. Пожалуйста, введите снова.\n");
	}

	while (1) {
		char input[20];
		printf("Введите диагональ: ");
		fgets(input, sizeof(input), stdin);
		input[strcspn(input, "\n")] = 0;

		if (is_number(input)) {
			sir->diagonal = atof(input);
			break;
		}
		else {
			printf("Ошибка: введите корректное значение для диагонали.\n");
		}
	}

	while (1) {
		char input[20];
		printf("Введите цену: ");
		fgets(input, sizeof(input), stdin);
		input[strcspn(input, "\n")] = 0;

		if (is_number(input)) {
			sir->price = atoi(input);
			break;
		}
		else {
			printf("Ошибка: введите корректное значение для цены.\n");
		}
	}

	while (1) {
		printf("Введите страну производства: ");
		fgets(sir->country, sizeof(sir->country), stdin);
		sir->country[strcspn(sir->country, "\n")] = 0;
		if (strlen(sir->country) > 0) break;
		printf("Ошибка: страна не может быть пустой. Пожалуйста, введите снова.\n");
	}

	while (1) {
		char input[20];
		printf("Введите угол обзора: ");
		fgets(input, sizeof(input), stdin);
		input[strcspn(input, "\n")] = 0;

		if (is_number(input)) {
			sir->angle = atoi(input);
			break;
		}
		else {
			printf("Ошибка: введите корректное значение для угла.\n");
		}
	}
	return 1;
}
// ЗАПИСЬ МАССИВА В ФАЙЛ
int in_file(display_t* a, int size) {

	FILE* str = fopen("base.txt", "w");

	for (int i = 0; i < size; i++) {
		fprintf(str, "%s,%.1f,%d,%s,%d\n", a[i].manufacturer, a[i].diagonal, a[i].price, a[i].country, a[i].angle);
	}
	fclose(str);

	printf("Вы записали данные в файл!");
	puts("");
	return 1;
}
// ЧТЕНИЕ МАССИВА ИЗ ФАЙЛА
display_t* from_file(int* size) {
	FILE* file = fopen("base.txt", "r");
	if (!file) {
		perror("Ошибка открытия файла");
		return NULL;
	}

	display_t* items = NULL;

	*size = 0;

	while (1) {
		display_t* temp = realloc(items, (*size + 1) * sizeof(display_t));
		if (temp == NULL) {
			perror("Ошибка выделения памяти");
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
// ПЕЧАТЬ ОДНОЙ ЗАПИСИ
int put(display_t a, int index) {
	printf("(%d). Производитель: %s; Диагональ: %.1f; Цена: %d; Страна: %s; Угол: %d\n", index, a.manufacturer, a.diagonal, a.price, a.country, a.angle);
	return 1;
}
// НАЙТИ И НАПЕЧАТАТЬ
int put_find(display_t* s, int size) {

	float diagonal;
	char manuf[20];
	int choice, flag = 0;

	printf("1. По производителю\n");
	printf("2. По диагонали экрана\n");
	printf("3. По производителю и диагонали экрана\n");
	printf("По каким параметрам вы хотите искать? -- ");
	scanf("%d", &choice);


	switch (choice) {
	case(1): {
		printf("Введите искомого производителя: ");
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
		printf("Введите искомую длину: ");
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
		printf("Введите искомого производителя: ");
		scanf("%s", manuf);
		printf("Введите искомую длину: ");
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
		printf("Неверный вариант поиска!");
		return;
	}
	}

	if (!flag) {
		printf("Элементы не найдены.\n");
	}
	return 1;
}
// ИЗМЕНИТЬ ОДНУ ЗАПИСЬ
int edit(display_t* array, int size) {
	int index;
	printf("Введите номер элемента для изменения: ");
	scanf("%d", &index);
	getchar();
	puts("");

	if (index < 1 || index > size) {
		printf("Такого элемента нет!\n");
		return;
	}

	index--;
	printf("Текущие данные элемента %d:\n", index + 1);
	put(array[index], index + 1);
	puts("");
	puts("**Обновление записи**");
	init(&array[index]);

	printf("Запись успешно обновлена.\n");
	return 1;
}
// СОРТИРОВКА
int compare_manuf(display_t* a, display_t* b) {
	return strcmp(a->manufacturer, b->manufacturer);
}
int compare_angle(display_t* a, display_t* b) {
	return (a->angle - b->angle);
}
int sort_mass(display_t* items, int size) {
	int choise = -1;

	printf("1. По производителю\n");
	printf("2. По углу обзора\n");
	printf("Какой параметр вы хотите отсортировать? — ");
	scanf("%d", &choise);

	if (choise == 1) {
		qsort(items, size, sizeof(display_t), (int (*)(const void*, const void*)) compare_manuf);
		printf("Элементы отсортированы по производителю.\n");
	}
	else if (choise == 2) {
		qsort(items, size, sizeof(display_t), (int (*)(const void*, const void*)) compare_angle);
		printf("Элементы отсортированы по углу.\n");
	}
	else {
		printf("Некорректный выбор.\n");
	}
	return 1;
}
// УДАЛЕНИЕ
display_t* delete_one(display_t* items, int* size) {
	int index;

	printf("Введите индекс элемента для удаления (1 до %d): ", *size);
	scanf("%d", &index);

	if (index < 1 || index > *size) {
		printf("Некорректный индекс.\n");
		return items;
	}

	for (int i = index - 1; i < *size - 1; i++) {
		items[i] = items[i + 1];
	}

	(*size)--;

	if (*size > 0) {
		display_t* temp = realloc(items, (*size) * sizeof(display_t));
		if (temp != NULL) {
			puts("Вы удалили элемент!");
			return temp;
		}
		else {
			printf("Ошибка выделения памяти.\n");
			return items;
		}
	}
	else {
		free(items);
		puts("Массива больше нет!");
		return NULL;
	}
	puts("");
}
// ПРОВЕРКА ВВОДА
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