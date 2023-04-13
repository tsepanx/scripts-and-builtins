import json
from utils import create_subscriber_exchange

ROUTING_KEY = 'co2.*'


def callback(_, __, ___, body: bytes):
    data: dict = json.loads(body)
    time = data.get("time", None)
    value = data.get("value", None)

    assert time is not None
    assert value is not None

    with open('receiver.log', 'a+') as f:
        f.write(f"{time},{value}\n")

    if value > 500:
        print("WARNING: CO2 level is high!")
    else:
        print("OK")


create_subscriber_exchange(callback, ROUTING_KEY)
