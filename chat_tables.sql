--sqlite3 chat-sk.db -init chat_tables.sql  
--komenda do inicjalizacji pustej bazy danych

DROP TABLE IF EXISTS messages;
DROP TABLE IF EXISTS users;


CREATE TABLE messages(
    message_id INTEGER PRIMARY KEY AUTOINCREMENT,
    sender_id TEXT,
    receiver_id TEXT,
    message TEXT,
    sending_date TEXT);


CREATE TABLE users(
    user_id TEXT PRIMARY KEY,
    username TEXT);


--przykladowe dane w tabeli 
INSERT INTO messages(sender_id, receiver_id, message, sending_date) VALUES('rsSq5E8EvdaV5d9MdryrfwzB0eQ2','itxZoi2PeuU00cr0gai7oek5kKu1','Hello 1-2', '2023-01-25T15:40:47Z');
INSERT INTO messages(sender_id, receiver_id, message, sending_date) VALUES('itxZoi2PeuU00cr0gai7oek5kKu1','rsSq5E8EvdaV5d9MdryrfwzB0eQ2','Whats updog?', '2023-01-25T15:41:47Z');
INSERT INTO messages(sender_id, receiver_id, message, sending_date) VALUES('rsSq5E8EvdaV5d9MdryrfwzB0eQ2','itxZoi2PeuU00cr0gai7oek5kKu1','Nothing much...', '2023-01-25T15:42:47Z');

INSERT INTO users(user_id, username) VALUES('rsSq5E8EvdaV5d9MdryrfwzB0eQ2', 'j@j.com');
INSERT INTO users(user_id, username) VALUES('itxZoi2PeuU00cr0gai7oek5kKu1', 'm@m.com');

