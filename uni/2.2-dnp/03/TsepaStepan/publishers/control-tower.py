from utils import create_publisher_channel_connection

EXCHANGE_NAME = 'amq.topic'

channel, connection = create_publisher_channel_connection(EXCHANGE_NAME)

try:
    while True:
        query = input("Enter a query (current/average): ")

        if query not in ["current", "average"]:
            print("Wrong input, try again")
            continue

        channel.basic_publish(
            EXCHANGE_NAME,
            routing_key=f"rep.{query}",
            body=query.encode()
        )
except KeyboardInterrupt:
    pass

connection.close()
