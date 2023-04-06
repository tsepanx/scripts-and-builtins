
DROP FUNCTION q1();
CREATE OR REPLACE FUNCTION q1()
RETURNS TABLE (
    cname VARCHAR(50),
    total_sales numeric
) AS $$
BEGIN
RETURN QUERY
    SELECT ct.name AS cname, SUM(p.amount) AS total_sales
        FROM payment p
    INNER JOIN rental r ON p.rental_id = r.rental_id
    INNER JOIN inventory i ON r.inventory_id = i.inventory_id
    INNER JOIN film f ON i.film_id = f.film_id
    INNER JOIN film_category fc ON f.film_id = fc.film_id
    INNER JOIN category ct ON fc.category_id = ct.category_id
    WHERE NOT EXISTS(
            SELECT cs.first_name, count(*)
            FROM customer cs,
                 rental r2,
                 inventory i1,
                 film f1,
                 film_actor fa,
                 actor a
            WHERE cs.customer_id = r2.customer_id
              AND r2.inventory_id = i1.inventory_id
              AND i1.film_id = f1.film_id
              and f1.rating in ('PG-13', 'NC-17')
              AND f1.film_id = fa.film_id
              AND f1.film_id = f.film_id
              AND fa.actor_id = a.actor_id
              and a.first_name = cs.first_name
            GROUP BY cs.first_name
            HAVING count(*) > 2
        )
    GROUP BY ct.name;
END
$$ LANGUAGE plpgsql;

EXPLAIN ANALYZE SELECT q1();

DROP INDEX index_p;
DROP INDEX index_r;
DROP INDEX index_i;
DROP INDEX index_f;
DROP INDEX index_fc;
DROP INDEX index_c;
DROP INDEX index_cs;

CREATE INDEX index_p ON payment(rental_id, amount);
CREATE INDEX index_r ON rental(rental_id, inventory_id);
CREATE INDEX index_i ON inventory(inventory_id, film_id);
CREATE INDEX index_f ON film(film_id);
CREATE INDEX index_fc ON film_category(film_id, category_id);
CREATE INDEX index_c ON category(category_id, name);
CREATE INDEX index_cs ON customer(customer_id, first_name);