-- tables
-- Table:  AUTOPARK_CONFIG 
CREATE TABLE  AUTOPARK_CONFIG  (
    config_key varchar(50) NOT NULL CONSTRAINT  AUTOPARK_CONFIG _pk PRIMARY KEY,
    config_value varchar(200),
    description text
);

-- Table: CARS
CREATE TABLE CARS (
    number varchar(20) NOT NULL CONSTRAINT CARS_pk PRIMARY KEY,
    brand varchar(100),
    initial_mileage integer CHECK (initial_mileage >= 0),
    current_mileage integer CHECK (current_mileage >= initial_mileage),
    capacity integer CHECK (capacity > 0)
);

-- Table: DRIVERS
CREATE TABLE DRIVERS (
    surname varchar(100),
    category varchar(10) CHECK (category IN ('A', 'B', 'C', 'D', 'E', 'CE', 'DE')),
    experience integer CHECK (experience >= 0),
    address varchar(200),
    birth_year integer CHECK (birth_year <= 2008 AND birth_year >= 1956),
    personnel_number integer NOT NULL CONSTRAINT DRIVERS_pk PRIMARY KEY
);

-- Table: AUTOPARK_USERS
CREATE TABLE AUTOPARK_USERS (
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
CREATE TABLE ORDERS (
    order_id integer NOT NULL CONSTRAINT ORDERS_pk PRIMARY KEY AUTOINCREMENT,
    date date,
    driver_id integer NOT NULL,
    car_number varchar(20) NOT NULL,
    distance integer CHECK (distance > 0),
    cargo_mass decimal(10,2) CHECK (cargo_mass > 0),
    cost decimal(10,2) CHECK (cost > 0),
    CONSTRAINT ORDERS_DRIVERS FOREIGN KEY (driver_id)
    REFERENCES DRIVERS (personnel_number)
    ON DELETE RESTRICT 
    ON UPDATE CASCADE,
    CONSTRAINT ORDERS_CARS FOREIGN KEY (car_number)
    REFERENCES CARS (number)
    ON DELETE RESTRICT 
    ON UPDATE CASCADE
);

-- Table: SALARY_RECORDS
CREATE TABLE SALARY_RECORDS (
    record_id integer NOT NULL CONSTRAINT SALARY_RECORDS_pk PRIMARY KEY AUTOINCREMENT,
    start_date date,
    end_date date CHECK (end_date >= start_date),
    total_salary decimal(10,2) NOT NULL CHECK (total_salary >= 0),
    calculated_at datetime NOT NULL,
    driver_id integer NOT NULL,
    CONSTRAINT SALARY_RECORDS_DRIVERS FOREIGN KEY (driver_id)
    REFERENCES DRIVERS (personnel_number)
    ON DELETE RESTRICT 
    ON UPDATE CASCADE
);

-- End of file.

