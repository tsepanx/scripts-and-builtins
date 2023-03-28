
### Task1

1) 1
```
a0=# EXPLAIN ANALYZE SELECT * FROM customer ;
```

```
                                                QUERY PLAN
-----------------------------------------------------------------------------------------------------------
 Seq Scan on customer  (cost=0.00..41.00 rows=1000 width=210) (actual time=0.023..0.360 rows=1000 loops=1)
 Planning Time: 0.097 ms
 Execution Time: 0.502 ms
(3 rows)
```
