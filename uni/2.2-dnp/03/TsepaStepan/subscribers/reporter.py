from utils import create_subscriber_exchange

ROUTING_KEY = 'rep.*'


def callback(_, __, ___, body: bytes):
    with open("receiver.log", 'r') as f:
        lines = f.readlines()
        values = [int(line.strip().split(',')[1]) for line in lines]
        if body == b"current":
            print(f"Current CO2 level: {values[-1]}")
        elif body == b"average":
            avg = sum(values) / len(values)
            print(f"Average CO2 level: {avg}")


create_subscriber_exchange(callback, ROUTING_KEY)
