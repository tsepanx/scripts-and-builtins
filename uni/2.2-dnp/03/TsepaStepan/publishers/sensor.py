import json
from datetime import datetime

from utils import create_publisher_channel_connection

EXCHANGE_NAME = 'amq.topic'
ROUTING_KEY = 'co2.sensor'

channel, connection = create_publisher_channel_connection(EXCHANGE_NAME)

try:
    while True:
        co2_level = int(input("Enter CO2 level: "))

        data = {
            "time": datetime.now().isoformat(),
            "value": co2_level
        }
        message = json.dumps(data).encode()

        channel.basic_publish(
            EXCHANGE_NAME,
            routing_key=ROUTING_KEY,
            body=message
        )
except KeyboardInterrupt:
    pass

connection.close()
