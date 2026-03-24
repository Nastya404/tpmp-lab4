#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db_core.h"
#include "crud.h"
#include "reports.h"
#include "business_logic.h"
#include "models.h"

void clear_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void print_menu() {
    printf("\n=== ИС АВТОПАРК (Лаб 4) ===\n");
    printf("1.  Добавить водителя\n");
    printf("2.  Добавить автомобиль\n");
    printf("3.  Создать заказ (Проверка веса: П.4 ТЗ)\n");
    printf("4.  Рассчитать зарплаты ВСЕМ (П.5 ТЗ)\n");
    printf("5.  Обновить пробег авто\n");
    printf("6.  ОТЧЕТ: Машина с макс. пробегом (П.2.5 ТЗ)\n");
    printf("7.  ОТЧЕТ: Статистика по конкретной машине (П.2.2 ТЗ)\n");
    printf("8.  ОТЧЕТ: Заказы водителя за период (П.2.1 ТЗ)\n");
    printf("9.  ОТЧЕТ: Статистика по всем водителям (П.2.3 ТЗ)\n");
    printf("10. ОТЧЕТ: Самый неактивный водитель (П.2.4 ТЗ)\n");
    printf("11. РАСЧЕТ: Зарплата водителю за период (П.6 ТЗ)\n");
    printf("12. Просмотр таблицы начисленных зарплат\n");
    printf("0.  Выход\n");
    printf("Выберите действие: ");
}

int main() {
    sqlite3 *db;
    
    if (db_connect("data/autopark.db", &db) != 0) return 1;
    db_init_from_file(db, "data/autopark_create.sql");

    int choice;
    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            clear_buffer();
            continue;
        }
        if (choice == 0) break;

        switch (choice) {
            case 1: {
                Driver d;
                printf("Табельный номер: "); scanf("%d", &d.personnel_number);
                clear_buffer();
                printf("Фамилия: "); scanf("%100[^\n]", d.surname); clear_buffer();
                printf("Категория: "); scanf("%9[^\n]", d.category); clear_buffer();
                printf("Стаж: "); scanf("%d", &d.experience); clear_buffer();
                printf("Адрес: "); scanf("%200[^\n]", d.address); clear_buffer();
                printf("Год рождения: "); scanf("%d", &d.birth_year); clear_buffer();
                add_driver(db, d);
                break;
            }
            case 2: {
                Car c;
                printf("Гос. номер: "); scanf("%20s", c.number); clear_buffer();
                printf("Марка: "); scanf("%100[^\n]", c.brand); clear_buffer();
                printf("Начальный пробег: "); scanf("%d", &c.initial_mileage);
                c.current_mileage = c.initial_mileage;
                printf("Грузоподъемность (кг): "); scanf("%d", &c.capacity); clear_buffer();
                add_car(db, c);
                break;
            }
            case 3: {
                Order o;
                printf("Дата (YYYY-MM-DD): "); scanf("%10s", o.date); clear_buffer();
                printf("ID водителя: "); scanf("%d", &o.driver_id); clear_buffer();
                printf("Номер авто: "); scanf("%20s", o.car_number); clear_buffer();
                printf("Дистанция (км): "); scanf("%d", &o.distance);
                printf("Вес груза (кг): "); scanf("%lf", &o.cargo_mass);
                printf("Стоимость: "); scanf("%lf", &o.cost); clear_buffer();
                process_new_order(db, o); 
                break;
            }
            case 4: {
                char period[20];
                printf("Введите период для расчета (напр. 2026-03): ");
                scanf("%19s", period); clear_buffer();
                calculate_salaries_to_db(db, period);
                break;
            }
            case 5: {
                char num[21]; int mil;
                printf("Номер авто: "); scanf("%20s", num); clear_buffer();
                printf("Новый пробег: "); scanf("%d", &mil); clear_buffer();
                update_mileage(db, num, mil);
                break;
            }
            case 6: 
                get_max_mileage_car(db); 
                break;
            case 7: {
                char num[21];
                printf("Введите номер авто: "); scanf("%20s", num); clear_buffer();
                get_car_statistics(db, num);
                break;
            }
            case 8: {
                int id; char s[11], e[11];
                printf("ID водителя: "); scanf("%d", &id); clear_buffer();
                printf("Начало (YYYY-MM-DD): "); scanf("%10s", s); clear_buffer();
                printf("Конец (YYYY-MM-DD): "); scanf("%10s", e); clear_buffer();
                get_driver_orders(db, id, s, e);
                break;
            }
            case 9:
                get_all_drivers_statistics(db);
                break;
            case 10:
                get_lazy_driver_info(db);
                break;
            case 11: {
                char surname[101], s[11], e[11];
                printf("Фамилия водителя: "); scanf("%100s", surname); clear_buffer();
                printf("Начало: "); scanf("%10s", s); clear_buffer();
                printf("Конец: "); scanf("%10s", e); clear_buffer();
                get_driver_orders(db, -1, s, e); 
                break;
            }
            case 12: {
                char s[11], e[11];
                printf("Начало периода: "); scanf("%10s", s); clear_buffer();
                printf("Конец периода: "); scanf("%10s", e); clear_buffer();
                get_salary_records(db, s, e);
                break;
            }
            default: 
                printf("Неверный выбор!\n");
                clear_buffer();
        }
    }

    db_close(db);
    return 0;
}
