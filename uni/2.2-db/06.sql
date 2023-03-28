

-- SELECT DISTINCT f.film_id FROM inventory
--     JOIN film f on f.film_id = inventory.film_id
--     JOIN rental r on inventory.inventory_id = r.inventory_id
-- ORDER BY f.film_id;

SELECT * FROM film
    JOIN film_category fc on film.film_id = fc.film_id
    JOIN category c on c.category_id = fc.category_id

    WHERE film.film_id NOT IN (
        SELECT DISTINCT f.film_id FROM inventory
            JOIN film f on f.film_id = inventory.film_id
            JOIN rental r on inventory.inventory_id = r.inventory_id
        ORDER BY f.film_id
    )

    AND rating IN ('R', 'PG-13')
    AND c.name IN ('Horror', 'Sci-fi');



SELECT staff_id, SUM(amount) FROM payment
    JOIN staff s on s.staff_id = payment.staff_id
    JOIN address a on a.address_id = s.address_id
GROUP BY staff_id;
-- SELECT * FROM payment;

-- SELECT * FROM address
--     JOIN city c on c.city_id = address.city_id
--     LEFT JOIN store s on address.address_id = s.address_id;

