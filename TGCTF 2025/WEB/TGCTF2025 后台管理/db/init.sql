USE tgctf;

DROP TABLE IF EXISTS flag;
CREATE TABLE IF NOT EXISTS flag (
    value VARCHAR(128) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

-- On the remote server, a real flag is inserted.
INSERT INTO flag (value) VALUES ('TGCTF{ac4ca16f-f1508c-000342}');

DROP TABLE IF EXISTS users;
CREATE TABLE IF NOT EXISTS users (
    username VARCHAR(255) PRIMARY KEY,
    password VARCHAR(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

INSERT INTO users (username, password) VALUES ('admin', 'ashdjkashdjsj');
INSERT INTO users (username, password) VALUES ('tg', 'tg123');
