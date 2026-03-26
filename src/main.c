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

void print_menu(User *u) {
    int is_admin = (strcmp(u->role, "ADMIN") == 0);
    
    printf("\n=== ИС АВТОПАРК (Лаб 4) ===\n");
    printf("Пользователь: %s | Роль: %s\n", u->login, u->role);
    printf("---------------------------\n");
    
    if (is_admin) printf("1.  [УПРАВЛЕНИЕ] Добавить водителя\n");
    if (is_admin) printf("2.  [УПРАВЛЕНИЕ] Добавить автомобиль\n");
    printf("3.  [*] Создать заказ\n");
    printf("4.  [*] Рассчитать зарплаты ВСЕМ\n");
    if (is_admin) printf("5.  [УПРАВЛЕНИЕ] Обновить пробег авто\n");
    printf("6.  [*] ОТЧЕТ: Машина с макс. пробегом\n");
    printf("7.  [*] ОТЧЕТ: Статистика по конкретной машине\n");
    printf("8.  [*] ОТЧЕТ: Заказы водителя за период\n");
    printf("9.  [*] ОТЧЕТ: Статистика по всем водителям\n");
    if (is_admin) printf("10. [УПРАВЛЕНИЕ] ОТЧЕТ: Самый неактивный водитель\n");
    if (is_admin) printf("11. [УПРАВЛЕНИЕ] РАСЧЕТ: Зарплата конкретному водителю\n");
    if (is_admin) printf("12. [УПРАВЛЕНИЕ] Просмотр таблицы начисленных зарплат\n");
    printf("0.  Выход\n");
    printf("Выберите действие: ");
}

int main() {
    sqlite3 *db;
    
    if (db_connect("data/autopark.db", &db) != 0) return 1;
    db_init_from_file(db, "data/autopark_create.sql");

    User current_user;
    int logged_in = 0;

    // Цикл авторизации
    while (!logged_in) {
        char login[51], password[256];
        printf("\n=== ВХОД В СИСТЕМУ ===\n");
        printf("Логин: "); scanf("%50s", login); clear_buffer();
        printf("Пароль: "); scanf("%255s", password); clear_buffer();

        if (authenticate_user(db, login, password, &current_user) == 0) {
            printf("\nУспешный вход! Добро пожаловать, %s.\n", current_user.login);
            logged_in = 1;
        } else {
            printf("Ошибка: Неверный логин или пароль. Попробуйте снова.\n");
        }
    }

    int is_admin = (strcmp(current_user.role, "ADMIN") == 0);
    int choice;

    while (1) {
        print_menu(&current_user);
        if (scanf("%d", &choice) != 1) {
            clear_buffer();
            continue;
        }
        if (choice == 0) break;

        switch (choice) {
            case 1: {
                if (!is_admin) { printf("Нет доступа!\n"); break; }
                Driver d;
                printf("Табельный номер: "); scanf("%d", &d.personnel_number); clear_buffer();
                printf("Фамилия: "); scanf("%100[^\n]", d.surname); clear_buffer();
                printf("Категория: "); scanf("%9[^\n]", d.category); clear_buffer();
                printf("Стаж: "); scanf("%d", &d.experience); clear_buffer();
                printf("Адрес: "); scanf("%200[^\n]", d.address); clear_buffer();
                printf("Год рождения: "); scanf("%d", &d.birth_year); clear_buffer();
                add_driver(db, d);
                break;
            }
            case 2: {
                if (!is_admin) { printf("Нет доступа!\n"); break; }
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
                if (is_admin) {
                    printf("ID водителя: "); scanf("%d", &o.driver_id); clear_buffer();
                } else {
                    o.driver_id = current_user.personnel_number;
                    printf("ID водителя: %d (автоматически)\n", o.driver_id);
                }
                printf("Номер авто: "); scanf("%20s", o.car_number); clear_buffer();
                printf("Дистанция (км): "); scanf("%d", &o.distance);
                printf("Вес груза (кг): "); scanf("%lf", &o.cargo_mass);
                printf("Стоимость: "); scanf("%lf", &o.cost); clear_buffer();
                process_new_order(db, o); 
                break;
            }
            case 4: {
                char start_date[11], end_date[11];
                printf("Начало (YYYY-MM-DD): "); scanf("%10s", start_date); clear_buffer();
                printf("Конец (YYYY-MM-DD): "); scanf("%10s", end_date); clear_buffer();
                calculate_salaries_to_db(db, start_date, end_date);
                break;
            }
            case 5: {
                if (!is_admin) { printf("Нет доступа!\n"); break; }
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
                if (is_admin) {
                    printf("ID водителя: "); scanf("%d", &id); clear_buffer();
                } else {
                    id = current_user.personnel_number;
                }
                printf("Начало (YYYY-MM-DD): "); scanf("%10s", s); clear_buffer();
                printf("Конец (YYYY-MM-DD): "); scanf("%10s", e); clear_buffer();
                get_driver_orders(db, id, s, e);
                break;
            }
            case 9:
                get_all_drivers_statistics(db);
                break;
            case 10:
                if (!is_admin) { printf("Нет доступа!\n"); break; }
                get_lazy_driver_info(db);
                break;
            case 11: {
                if (!is_admin) { printf("Нет доступа!\n"); break; }
                char surname[101], s[11], e[11];
                printf("Фамилия водителя: "); scanf("%100s", surname); clear_buffer();
                printf("Начало (YYYY-MM-DD): "); scanf("%10s", s); clear_buffer();
                printf("Конец (YYYY-MM-DD): "); scanf("%10s", e); clear_buffer();
                get_driver_salary_info(db, surname, s, e);
                break;
            }
            case 12: {
                if (!is_admin) { printf("Нет доступа!\n"); break; }
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
