import typing

import pika

RMQ_HOST = 'localhost'
RMQ_USER = 'rabbit'
RMQ_PASS = '1234'

EXCHANGE_NAME = 'amq.topic'


def create_subscriber_exchange(callback: typing.Callable, routing_key: str):
    credentials = pika.PlainCredentials(RMQ_USER, RMQ_PASS)
    connection = pika.BlockingConnection(pika.ConnectionParameters(RMQ_HOST, credentials=credentials))

    channel = connection.channel()

    result = channel.queue_declare(queue='', exclusive=True)
    queue_name = result.method.queue

    channel.queue_bind(
        exchange=EXCHANGE_NAME,
        queue=queue_name,
        routing_key=routing_key
    )

    channel.basic_consume(
        queue=queue_name,
        on_message_callback=callback,
        auto_ack=True
    )

    print("Waiting for messages...")
    channel.start_consuming()