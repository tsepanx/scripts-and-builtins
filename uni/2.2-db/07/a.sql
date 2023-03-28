

EXPLAIN ANALYZE SELECT * FROM customer WHERE name LIKE 'Ma%';

--                                               QUERY PLAN
-- -------------------------------------------------------------------------------------------------------
--  Seq Scan on customer  (cost=0.00..43.50 rows=30 width=210) (actual time=0.019..0.335 rows=35 loops=1)
--    Filter: (name ~~ 'Ma%'::text)
--    Rows Removed by Filter: 965
--  Planning Time: 0.218 ms
--  Execution Time: 0.360 ms
-- (5 rows)


CREATE INDEX my_index_1 ON customer (id);
CREATE INDEX my_index_2 ON customer (name);


EXPLAIN ANALYZE SELECT * FROM customer WHERE name LIKE 'Ma%';

--                                               QUERY PLAN
-- -------------------------------------------------------------------------------------------------------
--  Seq Scan on customer  (cost=0.00..43.50 rows=30 width=210) (actual time=0.023..0.331 rows=35 loops=1)
--    Filter: (name ~~ 'Ma%'::text)
--    Rows Removed by Filter: 965
--  Planning Time: 0.135 ms
--  Execution Time: 0.358 ms
-- (5 rows)
