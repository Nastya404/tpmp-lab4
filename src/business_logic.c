#include <stdio.h>
#include <string.h>
#include "business_logic.h"
#include "crud.h"
#include "models.h"   

int check_cargo_limit(const char* car_number, float mass) {
    Car car;
    
    if (get_car_by_number(car_number, &car) == STATUS_OK) { 
        if (mass > car.capacity) {
            return CARGO_LIMIT_EXCEEDED; 
        }
        return STATUS_OK; 
    }
    
    return NOT_FOUND;
}

int process_new_order(Order order) {
    int limit_check = check_cargo_limit(order.car_number, order.cargo_mass);

    if (limit_check == CARGO_LIMIT_EXCEEDED) {
        printf("Ошибка: Вес превышен!\n"); 
        return CARGO_LIMIT_EXCEEDED;
    } 
    else if (limit_check == NOT_FOUND) {
        printf("Ошибка: Машина с номером %s не найдена!\n", order.car_number);
        return NOT_FOUND;
    }

    int db_status = add_order(order); 
    
    if (db_status == STATUS_OK) {
        printf("Заказ добавлен успешно.\n"); 
        return STATUS_OK;
    } else {
        printf("Ошибка базы данных при сохранении заказа.\n");
        return DB_ERROR;
    }
}

int calculate_salaries_to_db(const char* start_date, const char* end_date) {
    int status = execute_salary_calculation_all(start_date, end_date); 
    
    if (status == STATUS_OK) {
        printf("Зарплаты за период %s - %s успешно рассчитаны и занесены в БД.\n", start_date, end_date);
    } else {
        printf("Ошибка при расчете зарплат в БД.\n");
    }
    return status;
}
