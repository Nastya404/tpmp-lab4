-- tables
-- Table:  AUTOPARK_CONFIG 
CREATE TABLE IF NOT EXISTS  AUTOPARK_CONFIG  (
    config_key varchar(50) NOT NULL CONSTRAINT AUTOPARK_CONFIG_pk PRIMARY KEY,
    config_value varchar(200),
    description text
);

-- Table: CARS
CREATE TABLE IF NOT EXISTS CARS (
    number varchar(20) NOT NULL CONSTRAINT CARS_pk PRIMARY KEY,
    brand varchar(100),
    initial_mileage integer NOT NULL CHECK (initial_mileage >= 0),
    current_mileage integer NOT NULL CHECK (current_mileage >= initial_mileage),
    capacity integer CHECK (capacity > 0)
);

-- Table: DRIVERS
CREATE TABLE IF NOT EXISTS DRIVERS (
    surname varchar(100) NOT NULL,
    category varchar(10) NOT NULL CHECK (category IN ('A', 'B', 'C', 'D', 'E', 'CE', 'DE')),
    experience integer NOT NULL CHECK (experience >= 0),
    address varchar(200) NOT NULL,
    birth_year integer NOT NULL CHECK (birth_year <= 2008 AND birth_year >= 1956),
    personnel_number integer NOT NULL CONSTRAINT DRIVERS_pk PRIMARY KEY
);

-- Table: AUTOPARK_USERS
CREATE TABLE IF NOT EXISTS AUTOPARK_USERS (
    user_id integer NOT NULL CONSTRAINT AUTOPARK_USERS_pk PRIMARY KEY AUTOINCREMENT,
    login varchar(50) NOT NULL UNIQUE,
    password_hash varchar(255) NOT NULL,
    role varchar(20) NOT NULL CHECK (role IN ('ADMIN', 'DRIVER')),
    personnel_number integer,
    CONSTRAINT USERS_DRIVERS FOREIGN KEY (personnel_number)
    REFERENCES DRIVERS (personnel_number)
    ON DELETE SET NULL 
    ON UPDATE CASCADE
);

-- Table: ORDERS
CREATE TABLE IF NOT EXISTS ORDERS (
    order_id integer NOT NULL CONSTRAINT ORDERS_pk PRIMARY KEY AUTOINCREMENT,
    date date NOT NULL,
    driver_id integer NOT NULL,
    car_number varchar(20) NOT NULL,
    distance integer NOT NULL CHECK (distance > 0),
    cargo_mass decimal(10,2) NOT NULL CHECK (cargo_mass > 0),
    cost decimal(10,2) NOT NULL CHECK (cost > 0),
    CONSTRAINT ORDERS_DRIVERS FOREIGN KEY (driver_id)
    REFERENCES DRIVERS (personnel_number)
    ON DELETE RESTRICT 
    ON UPDATE CASCADE,
    CONSTRAINT ORDERS_CARS FOREIGN KEY (car_number)
    REFERENCES CARS (number)
    ON DELETE RESTRICT 
    ON UPDATE CASCADE
);

-- Point 4 TЗ:
-- запрет добавлять заказ, если cargo_mass превышает грузоподъемность машины
DROP TRIGGER IF EXISTS trg_orders_cargo_mass_ins;
CREATE TRIGGER trg_orders_cargo_mass_ins
BEFORE INSERT ON ORDERS
FOR EACH ROW
BEGIN
    SELECT RAISE(ABORT, 'cargo_mass exceeds car capacity')
    WHERE NEW.cargo_mass > (SELECT capacity FROM CARS WHERE number = NEW.car_number);
END;

DROP TRIGGER IF EXISTS trg_orders_cargo_mass_upd;
CREATE TRIGGER trg_orders_cargo_mass_upd
BEFORE UPDATE OF cargo_mass, car_number ON ORDERS
FOR EACH ROW
BEGIN
    SELECT RAISE(ABORT, 'cargo_mass exceeds car capacity')
    WHERE NEW.cargo_mass > (SELECT capacity FROM CARS WHERE number = NEW.car_number);
END;

-- Table: SALARY_RECORDS
CREATE TABLE IF NOT EXISTS SALARY_RECORDS (
    record_id integer NOT NULL CONSTRAINT SALARY_RECORDS_pk PRIMARY KEY AUTOINCREMENT,
    start_date date,
    end_date date CHECK (end_date >= start_date),
    total_salary decimal(10,2) NOT NULL CHECK (total_salary >= 0),
    calculated_at datetime NOT NULL,
    driver_id integer NOT NULL,
    CONSTRAINT SALARY_RECORDS_DRIVERS FOREIGN KEY (driver_id)
    REFERENCES DRIVERS (personnel_number)
    ON DELETE RESTRICT 
    ON UPDATE CASCADE,
    -- чтобы пересчитать период без дублей
    UNIQUE(driver_id, start_date, end_date)
);

-- End of file.

