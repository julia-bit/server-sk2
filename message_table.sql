--sqlite3 chat-db.db -init message_table.sql  
--komenda do inicjalizacji pustej bazy danych

DROP TABLE IF EXISTS messages;

CREATE TABLE messages(
    sender_id INTEGER,
    receiver_id INTEGER,
    message TEXT;
    sending_date DATE;
)


--przykladowe dane w tabeli 
INSERT INTO messages(sender_id, receiver_id, message, sending_date)
VALUES(1,2,"Hello 1-2", DATE '2015-12-17');
INSERT INTO messages(sender_id, receiver_id, message, sending_date)
VALUES(2,3,"What's updog?", DATE '2023-01-19');
INSERT INTO messages(sender_id, receiver_id, message, sending_date)
VALUES(3,2,"Nothing much...", DATE '2023-01-19');