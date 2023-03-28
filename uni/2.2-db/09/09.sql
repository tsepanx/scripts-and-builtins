
CREATE TYPE Currency AS ENUM ('RUB', 'DOL', 'EUR');

CREATE TABLE accounts (
    pk SERIAL PRIMARY KEY,
    name VARCHAR(50) NOT NULL,
    credit INTEGER,
    currency Currency
);

INSERT INTO accounts (name, credit, currency) VALUES
    ('name1', 1000, 'RUB'),
    ('name2', 1000, 'RUB'),
    ('name3', 1000, 'RUB')
;

CREATE OR REPLACE FUNCTION make_transaction(pk1 INTEGER, pk2 INTEGER, amount INTEGER)
RETURNS void AS $$
BEGIN
UPDATE accounts SET credit = credit - amount WHERE pk = pk1;
UPDATE accounts SET credit = credit + amount WHERE pk = pk2;
END
$$ LANGUAGE plpgsql;

BEGIN;
    SELECT make_transaction(1, 3, 500);
    SAVEPOINT tr1;

    SELECT make_transaction(2, 1, 700);
    SAVEPOINT tr2;

    SELECT make_transaction(2, 3, 100);
    SAVEPOINT tr3;

    ROLLBACK transaction TO tr1;
    SELECT * FROM accounts;
--     ROLLBACK transaction TO tr2;
--     ROLLBACK transaction TO tr3;
COMMIT;


--- Part B

CREATE TYPE BankName AS ENUM ('SberBank', 'Tinkoff');
ALTER TABLE accounts ADD COLUMN bank_name BankName;

DELETE FROM accounts WHERE pk in (1, 2, 3, 4);
INSERT INTO accounts (pk, name, credit, currency) VALUES
    (1, 'name1', 1000, 'RUB'),
    (2, 'name2', 1000, 'RUB'),
    (3, 'name3', 1000, 'RUB')
;

UPDATE accounts
    SET bank_name = 'SberBank'
    WHERE pk IN (1, 3);
UPDATE accounts
    SET bank_name = 'Tinkoff'
    WHERE pk IN (2);

INSERT INTO accounts (pk, name, credit, currency) VALUES (4, 'fee_get', 0, 'RUB');


-- SELECT * FROM accounts ORDER BY pk;

CREATE OR REPLACE FUNCTION make_transaction(pk1 INTEGER, pk2 INTEGER, amount INTEGER)
RETURNS void AS $$
BEGIN
    UPDATE accounts SET credit = credit - amount - 30 WHERE pk = pk1;
    UPDATE accounts SET credit = credit + amount WHERE pk = pk2;
    UPDATE accounts SET credit = credit + 30 WHERE pk = 4;
END
$$ LANGUAGE plpgsql;


BEGIN;
    SELECT make_transaction(1, 3, 500);
    SAVEPOINT tr1;

    SELECT make_transaction(2, 1, 700);
    SAVEPOINT tr2;

    SELECT make_transaction(2, 3, 100);
    SAVEPOINT tr3;

    SELECT * FROM accounts;
    ROLLBACK transaction TO tr3;
    ROLLBACK transaction TO tr2;
    ROLLBACK transaction TO tr1;
COMMIT;

-- Part C

CREATE TABLE IF NOT EXISTS Ledger (
    pk SERIAL PRIMARY KEY,
    from_pk INTEGER REFERENCES accounts(pk),
    to_pk INTEGER REFERENCES accounts(pk),
    fee NUMERIC,
    amount NUMERIC,
    datetime TIMESTAMP
);

CREATE OR REPLACE FUNCTION make_transaction(pk1 INTEGER, pk2 INTEGER, amount INTEGER)
RETURNS void AS $$
BEGIN
    UPDATE accounts SET credit = credit - amount - 30 WHERE pk = pk1;
    UPDATE accounts SET credit = credit + amount WHERE pk = pk2;
    UPDATE accounts SET credit = credit + 30 WHERE pk = 4;

    INSERT INTO Ledger (from_pk, to_pk, fee, amount, datetime) VALUES
        (pk1, pk2, 30, amount, now());
END
$$ LANGUAGE plpgsql;

BEGIN;
    SELECT make_transaction(1, 3, 500);
    SAVEPOINT tr1;

    SELECT make_transaction(2, 1, 700);
    SAVEPOINT tr2;

    SELECT make_transaction(2, 3, 100);
    SAVEPOINT tr3;

    SELECT * FROM accounts;
--     ROLLBACK transaction TO tr3;
--     ROLLBACK transaction TO tr2;
--     ROLLBACK transaction TO tr1;
COMMIT;

SELECT * FROM Ledger;