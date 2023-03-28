import psycopg2
from faker import Faker


# cur.callproc('function_name', (value1,value2))

con = psycopg2.connect(
    database="a0",
    user="postgres",
    password="",
    host="127.0.0.1", 
    port="5432"
)

cur = con.cursor()
# fake = Faker()
# for i in range(1000):
#     print(i)
#     cur.execute("INSERT INTO CUSTOMER (ID,Name,Address,review) VALUES ('"+ str(i)+"','"+fake.name()+"','"+fake.address()+"','"+fake.text()+"')")
#     con.commit()



# cur.execute("SELECT * FROM address a WHERE a.address LIKE '%11%' AND a.city_id BETWEEN 400 AND 600;")
cur.callproc('get_addresses')

# results = list(i[0] for i in cur.fetchall())
results = cur.fetchall()
# print(*results, sep='\n')

from geopy.geocoders import Nominatim
geolocator = Nominatim(user_agent="asdfasdf", timeout=100)

# coords: list[tuple[int, int]] = []

for i, (addr, city_id) in enumerate(results):
    location = geolocator.geocode(f"{addr}, {city_id}")

    if location:
        lat, lon = location.latitude, location.longitude
    else:
        lat, lon = 0, 0

    print(addr, city_id, lat, lon)
    results[i] = (addr, lat, lon)

    cur.execute(f"""
        UPDATE  address
        SET (lat, lon) = ({lat}, {lon})
        WHERE address = '{addr}';
    """)
    con.commit()


# query = """
# UPDATE address
# SET lat = CASE
# """
#
# from psycopg2.sql import SQL, Literal
#
# #         WHEN address = '1411 Lillydale Drive' THEN 12
# #         WHEN address = '1411 Lillydale Drive' THEN 23
# #         ELSE lat
# for i, (addr, lat, lon) in enumerate(results):
#     template_query = "WHEN address = {} THEN {}"
#
#     query += SQL(template_query).format([
#         Literal(addr),
#         Literal(lat),
#     ]).as_string(con)
#
#     # cur.execute(query)
#     # cur.fetchall()
#
#
# query += """
#         ELSE lat
#     END,
#     lon = CASE
# """
#
# #         WHEN pk_column = pk_value1 THEN new_value3
# #         WHEN pk_column = pk_value2 THEN new_value4
# for i, (addr, lat, lon) in enumerate(results):
#     template_query = "WHEN address = {} THEN {}"
#
#     query += SQL(template_query).format([
#         # SQL(", ").join()
#         Literal(addr),
#         Literal(lon)
#     ]).as_string(con)
#
# query += """
#         ELSE lon
#     END
# """
#
# import sqlparse
# print(sqlparse.format(query, 'utf-8'))
#
# #         ELSE column2
# #     END
# # WHERE address IN ('1411 Lillydale Drive');
# """