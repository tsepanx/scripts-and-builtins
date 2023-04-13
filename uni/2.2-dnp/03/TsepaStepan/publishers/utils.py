import pika

RMQ_HOST = 'localhost'
RMQ_USER = 'rabbit'
RMQ_PASS = '1234'


def create_publisher_channel_connection(exchange_name: str):
    credentials = pika.PlainCredentials(RMQ_USER, RMQ_PASS)
    connection = pika.BlockingConnection(
        pika.ConnectionParameters(
            RMQ_HOST,
            credentials=credentials
        )
    )
    channel = connection.channel()

    channel.exchange_declare(exchange=exchange_name, exchange_type='topic', durable=True)
    return channel, connection