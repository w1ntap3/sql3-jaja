CREATE TABLE IF NOT EXISTS categories (
  id INTEGER PRIMARY KEY NOT NULL,
  name TEXT NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS expenses (
  id INTEGER PRIMARY KEY NOT NULL,
  amount REAL NOT NULL,
  category_id INTEGER NOT NULL,
  date TEXT NOT NULL,
  description TEXT,
  FOREIGN KEY (category_id) REFERENCES categories(id)
);

insert into categories (id, name) values (1, 'fitness classes');
insert into categories (id, name) values (2, 'music lessons');
insert into categories (id, name) values (3, 'clothing');
insert into categories (id, name) values (4, 'electronics');
insert into categories (id, name) values (5, 'spa treatments');
insert into categories (id, name) values (6, 'gurt');
insert into categories (id, name) values (7, 'home decor');
insert into categories (id, name) values (8, 'beauty products');
insert into categories (id, name) values (9, 'hobbies');
insert into categories (id, name) values (10, 'twin');
insert into categories (id, name) values (11, 'cooking classes');
insert into categories (id, name) values (12, 'dz?');
insert into categories (id, name) values (13, 'movies');
insert into categories (id, name) values (14, 'basketball');
insert into categories (id, name) values (15, 'pet accessories');
insert into categories (id, name) values (16, 'video games');
insert into categories (id, name) values (17, 'sigma');
insert into categories (id, name) values (18, 'books');
insert into categories (id, name) values (19, 'restaurant meals');
insert into categories (id, name) values (20, 'travel');

insert into expenses (id, amount, category_id, date, description) values (1, 736.55, 3, '21-06-2023', null);
insert into expenses (id, amount, category_id, date, description) values (2, 33.95, 11, '20-10-2023', null);
insert into expenses (id, amount, category_id, date, description) values (3, 240.52, 2, '05-06-2025', 'Paid for marketing services');
insert into expenses (id, amount, category_id, date, description) values (4, 426.94, 1, '30-10-2023', null);
insert into expenses (id, amount, category_id, date, description) values (5, 491.45, 11, '30-08-2023', 'Invested in training programs');
insert into expenses (id, amount, category_id, date, description) values (6, 860.0, 9, '21-12-2023', 'Purchased office supplies');
insert into expenses (id, amount, category_id, date, description) values (7, 377.11, 19, '23-07-2024', 'Renovated office space');
insert into expenses (id, amount, category_id, date, description) values (8, 783.89, 13, '27-07-2023', 'Purchased office supplies');
insert into expenses (id, amount, category_id, date, description) values (9, 378.39, 6, '15-12-2023', null);
insert into expenses (id, amount, category_id, date, description) values (10, 622.3, 11, '01-10-2023', 'Renovated office space');
insert into expenses (id, amount, category_id, date, description) values (11, 369.69, 19, '19-05-2025', null);
insert into expenses (id, amount, category_id, date, description) values (12, 665.26, 2, '04-08-2023', null);
insert into expenses (id, amount, category_id, date, description) values (13, 596.91, 19, '11-10-2023', null);
insert into expenses (id, amount, category_id, date, description) values (14, 121.26, 10, '19-07-2024', null);
insert into expenses (id, amount, category_id, date, description) values (15, 387.82, 18, '26-11-2024', 'Purchased office supplies');
insert into expenses (id, amount, category_id, date, description) values (16, 772.01, 20, '10-02-2024', null);
insert into expenses (id, amount, category_id, date, description) values (17, 342.92, 2, '22-10-2024', null);
insert into expenses (id, amount, category_id, date, description) values (18, 91.14, 18, '26-07-2024', null);
insert into expenses (id, amount, category_id, date, description) values (19, 316.44, 13, '01-08-2023', null);
insert into expenses (id, amount, category_id, date, description) values (20, 97.52, 9, '12-07-2024', null);
insert into expenses (id, amount, category_id, date, description) values (21, 322.19, 8, '18-08-2023', null);
insert into expenses (id, amount, category_id, date, description) values (22, 122.55, 18, '17-07-2024', null);
insert into expenses (id, amount, category_id, date, description) values (23, 92.49, 6, '15-01-2025', null);
insert into expenses (id, amount, category_id, date, description) values (24, 382.76, 6, '26-09-2024', 'Renovated office space');
insert into expenses (id, amount, category_id, date, description) values (25, 397.58, 7, '01-07-2023', null);
insert into expenses (id, amount, category_id, date, description) values (26, 807.72, 5, '21-04-2025', 'Renovated office space');
insert into expenses (id, amount, category_id, date, description) values (27, 120.82, 20, '12-02-2024', null);
insert into expenses (id, amount, category_id, date, description) values (28, 692.21, 10, '10-12-2023', null);
insert into expenses (id, amount, category_id, date, description) values (29, 606.97, 8, '09-06-2023', null);
insert into expenses (id, amount, category_id, date, description) values (30, 509.57, 15, '27-01-2025', null);
insert into expenses (id, amount, category_id, date, description) values (31, 941.62, 14, '21-06-2023', null);
insert into expenses (id, amount, category_id, date, description) values (32, 403.81, 11, '23-02-2025', null);
insert into expenses (id, amount, category_id, date, description) values (33, 553.92, 14, '07-10-2023', 'Paid for marketing services');
insert into expenses (id, amount, category_id, date, description) values (34, 212.92, 11, '21-04-2024', 'Renovated office space');
insert into expenses (id, amount, category_id, date, description) values (35, 830.85, 15, '25-11-2023', 'Bought new equipment');
insert into expenses (id, amount, category_id, date, description) values (36, 125.3, 8, '17-09-2023', null);
insert into expenses (id, amount, category_id, date, description) values (37, 384.21, 19, '06-12-2024', null);
insert into expenses (id, amount, category_id, date, description) values (38, 782.86, 4, '16-03-2024', null);
insert into expenses (id, amount, category_id, date, description) values (39, 212.46, 14, '01-02-2024', null);
insert into expenses (id, amount, category_id, date, description) values (40, 911.46, 17, '08-05-2024', null);
insert into expenses (id, amount, category_id, date, description) values (41, 787.05, 10, '06-06-2023', 'Purchased office supplies');
insert into expenses (id, amount, category_id, date, description) values (42, 394.59, 2, '09-07-2024', null);
insert into expenses (id, amount, category_id, date, description) values (43, 331.05, 5, '07-09-2024', 'Invested in training programs');
insert into expenses (id, amount, category_id, date, description) values (44, 420.24, 7, '15-04-2025', 'Bought new equipment');
insert into expenses (id, amount, category_id, date, description) values (45, 486.81, 17, '04-12-2023', null);
insert into expenses (id, amount, category_id, date, description) values (46, 149.63, 3, '13-06-2024', null);
insert into expenses (id, amount, category_id, date, description) values (47, 817.65, 7, '29-06-2023', null);
insert into expenses (id, amount, category_id, date, description) values (48, 365.15, 1, '16-10-2024', 'Renovated office space');
insert into expenses (id, amount, category_id, date, description) values (49, 667.35, 20, '17-10-2024', 'Bought new equipment');
insert into expenses (id, amount, category_id, date, description) values (50, 474.52, 7, '17-08-2023', 'Bought new equipment');
insert into expenses (id, amount, category_id, date, description) values (51, 429.46, 7, '09-11-2024', null);
insert into expenses (id, amount, category_id, date, description) values (52, 331.26, 5, '20-12-2024', null);
insert into expenses (id, amount, category_id, date, description) values (53, 150.51, 6, '12-12-2024', null);
insert into expenses (id, amount, category_id, date, description) values (54, 65.13, 18, '22-04-2025', null);
insert into expenses (id, amount, category_id, date, description) values (55, 894.87, 10, '01-06-2025', null);
insert into expenses (id, amount, category_id, date, description) values (56, 794.14, 16, '16-12-2023', null);
insert into expenses (id, amount, category_id, date, description) values (57, 294.6, 7, '13-04-2024', null);
insert into expenses (id, amount, category_id, date, description) values (58, 523.75, 2, '02-08-2024', 'Paid for marketing services');
insert into expenses (id, amount, category_id, date, description) values (59, 536.56, 14, '23-06-2023', null);
insert into expenses (id, amount, category_id, date, description) values (60, 33.56, 13, '03-05-2024', null);
insert into expenses (id, amount, category_id, date, description) values (61, 684.05, 15, '12-09-2023', 'Paid for marketing services');
insert into expenses (id, amount, category_id, date, description) values (62, 337.12, 17, '28-06-2023', 'Renovated office space');
insert into expenses (id, amount, category_id, date, description) values (63, 460.43, 5, '14-08-2024', null);
insert into expenses (id, amount, category_id, date, description) values (64, 498.26, 20, '07-08-2023', 'Renovated office space');
insert into expenses (id, amount, category_id, date, description) values (65, 602.4, 4, '14-06-2024', null);
insert into expenses (id, amount, category_id, date, description) values (66, 366.88, 7, '21-08-2023', 'Paid for marketing services');
insert into expenses (id, amount, category_id, date, description) values (67, 725.07, 13, '21-08-2023', null);
insert into expenses (id, amount, category_id, date, description) values (68, 245.26, 13, '30-06-2024', 'Paid for marketing services');
insert into expenses (id, amount, category_id, date, description) values (69, 914.16, 1, '13-07-2023', null);
insert into expenses (id, amount, category_id, date, description) values (70, 913.38, 6, '18-10-2024', 'Invested in training programs');
insert into expenses (id, amount, category_id, date, description) values (71, 715.32, 2, '03-04-2025', null);
insert into expenses (id, amount, category_id, date, description) values (72, 785.87, 1, '15-08-2024', null);
insert into expenses (id, amount, category_id, date, description) values (73, 586.68, 1, '07-03-2024', null);
insert into expenses (id, amount, category_id, date, description) values (74, 593.08, 19, '14-01-2025', 'Paid for marketing services');
insert into expenses (id, amount, category_id, date, description) values (75, 682.72, 16, '22-12-2023', null);
insert into expenses (id, amount, category_id, date, description) values (76, 163.41, 9, '29-07-2024', null);
insert into expenses (id, amount, category_id, date, description) values (77, 667.37, 7, '08-02-2025', 'Purchased office supplies');
insert into expenses (id, amount, category_id, date, description) values (78, 49.34, 15, '08-08-2023', 'Invested in training programs');
insert into expenses (id, amount, category_id, date, description) values (79, 869.58, 18, '08-11-2023', null);
insert into expenses (id, amount, category_id, date, description) values (80, 63.7, 14, '14-03-2025', null);
insert into expenses (id, amount, category_id, date, description) values (81, 891.47, 16, '17-06-2023', null);
insert into expenses (id, amount, category_id, date, description) values (82, 369.14, 19, '23-09-2023', null);
insert into expenses (id, amount, category_id, date, description) values (83, 277.91, 12, '25-07-2024', 'Paid for marketing services');
insert into expenses (id, amount, category_id, date, description) values (84, 183.24, 9, '22-01-2024', null);
insert into expenses (id, amount, category_id, date, description) values (85, 554.29, 13, '27-05-2024', null);
insert into expenses (id, amount, category_id, date, description) values (86, 982.88, 5, '03-08-2023', null);
insert into expenses (id, amount, category_id, date, description) values (87, 877.81, 4, '13-09-2023', 'Invested in training programs');
insert into expenses (id, amount, category_id, date, description) values (88, 920.57, 10, '25-09-2024', null);
insert into expenses (id, amount, category_id, date, description) values (89, 792.89, 9, '18-11-2023', null);
insert into expenses (id, amount, category_id, date, description) values (90, 118.72, 18, '26-10-2023', 'Paid for marketing services');
insert into expenses (id, amount, category_id, date, description) values (91, 922.75, 4, '02-07-2024', null);
insert into expenses (id, amount, category_id, date, description) values (92, 698.21, 18, '03-11-2024', 'Paid for marketing services');
insert into expenses (id, amount, category_id, date, description) values (93, 61.89, 1, '13-05-2024', 'Paid for marketing services');
insert into expenses (id, amount, category_id, date, description) values (94, 767.81, 9, '09-03-2024', null);
insert into expenses (id, amount, category_id, date, description) values (95, 824.46, 9, '09-08-2023', null);
insert into expenses (id, amount, category_id, date, description) values (96, 949.76, 8, '07-01-2025', 'Purchased office supplies');
insert into expenses (id, amount, category_id, date, description) values (97, 420.8, 19, '15-09-2023', 'Bought new equipment');
insert into expenses (id, amount, category_id, date, description) values (98, 602.23, 4, '01-11-2023', null);
insert into expenses (id, amount, category_id, date, description) values (99, 472.27, 11, '25-04-2025', null);
insert into expenses (id, amount, category_id, date, description) values (100, 24.67, 10, '12-05-2024', 'Bought new equipment');
