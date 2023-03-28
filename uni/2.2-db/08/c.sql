

SELECT * FROM address a
    WHERE
        a.address LIKE '%11%'
    AND
        a.city_id BETWEEN 400 AND 600;


-- DROP FUNCTION get_addresses();
CREATE FUNCTION get_addresses()
RETURNS TABLE (
    address VARCHAR(50),
    city_id smallint
) AS $$
BEGIN
    RETURN QUERY
        SELECT a.address, a.city_id
        FROM address a
        WHERE a.address LIKE '%11%' AND a.city_id BETWEEN 400 AND 600;
END;
$$ LANGUAGE plpgsql;

SELECT * FROM get_addresses();

-- UPDATE address SET

ALTER TABLE address ADD COLUMN
    lat float;
ALTER TABLE address ADD COLUMN
    lon float;

-- UPDATE  address
--     SET (lat, lon) = (10, 10)
--     WHERE address = '1411 Lillydale Drive';


-- UPDATE address
-- SET lat = CASE
--         WHEN address = '1411 Lillydale Drive' THEN 12
--         WHEN address = '1411 Lillydale Drive' THEN 23
--         ELSE lat
--     END,
--     lon = CASE
--         WHEN pk_column = pk_value1 THEN new_value3
--         WHEN pk_column = pk_value2 THEN new_value4
--         ELSE column2
--     END
-- WHERE address IN ('1411 Lillydale Drive');