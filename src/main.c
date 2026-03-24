#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db_core.h"
#include "crud.h"
#include "reports.h"
#include "models.h"

void print_menu() {
    printf("\n=== ИС АВТОПАРК ===\n");
    printf("1. Добавить водителя\n");
    printf("2. Добавить автомобиль\n");
    printf("3. Создать новый заказ\n");
    printf("4. Обновить пробег авто\n");
    printf("5. ОТЧЕТ: Машина с макс. пробегом\n");
    printf("6. ОТЧЕТ: Статистика по водителю\n");
    printf("7. ОТЧЕТ: Сводный отчет по всем\n");
    printf("0. Выход\n");
    printf("Выберите действие: ");
}

int main() {
    sqlite3 *db;
    
    // 1. Подключение и инициализация из SQL-файла
    if (db_connect("data/autopark.db", &db) != 0) return 1;
	db_init_from_file(db, "data/autopark_create.sql");

    int choice;
    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) break;

        if (choice == 0) break;

        switch (choice) {
            case 1: {
                Driver d;
                printf("Табельный номер: "); scanf("%d", &d.personnel_number);
                printf("Фамилия: "); scanf("%s", d.surname);
                printf("Категория (A/B/C...): "); scanf("%s", d.category);
                printf("Стаж: "); scanf("%d", &d.experience);
                printf("Адрес: "); scanf("%s", d.address);
                printf("Год рождения: "); scanf("%d", &d.birth_year);
                add_driver(db, d);
                break;
            }
            case 2: {
                Car c;
                printf("Гос. номер: "); scanf("%s", c.number);
                printf("Марка: "); scanf("%s", c.brand);
                printf("Начальный пробег: "); scanf("%d", &c.initial_mileage);
                c.current_mileage = c.initial_mileage;
                printf("Грузоподъемность (кг): "); scanf("%d", &c.capacity);
                add_car(db, c);
                break;
            }
            case 3: {
                Order o;
                printf("Дата (YYYY-MM-DD): "); scanf("%s", o.date);
                printf("ID водителя: "); scanf("%d", &o.driver_id);
                printf("Номер авто: "); scanf("%s", o.car_number);
                printf("Дистанция (км): "); scanf("%d", &o.distance);
                printf("Вес груза: "); scanf("%lf", &o.cargo_mass);
                printf("Стоимость: "); scanf("%lf", &o.cost);
                add_order(db, o);
                break;
            }
            case 4: {
                char num[21]; int mil;
                printf("Номер авто: "); scanf("%s", num);
                printf("Новый пробег: "); scanf("%d", &mil);
                update_mileage(db, num, mil);
                break;
            }
            case 5: get_max_mileage_car(db); break;
            case 6: {
                int id;
                printf("Введите ID водителя: "); scanf("%d", &id);
                get_driver_statistics(db, id);
                break;
            }
            case 7: get_all_drivers_statistics(db); break;
            default: printf("Неверный выбор!\n");
        }
    }

    db_close(db);
    return 0;
}
