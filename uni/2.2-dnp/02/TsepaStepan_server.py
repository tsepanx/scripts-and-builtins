import socket
from threading import Thread

from PIL import Image
import random
from io import BytesIO

MSS = 1024


def gen_image() -> bytes:
    # Create a new 10x10 image
    img = Image.new('RGB', (10, 10), color='white')
    pixels = img.load()

    # Set random colors for each pixel
    for i in range(img.size[0]):
        for j in range(img.size[1]):
            r = random.randint(0, 255)
            g = random.randint(0, 255)
            b = random.randint(0, 255)
            pixels[i, j] = (r, g, b)

    bio = BytesIO()

    img.save(bio, format="JPEG")
    return bio.getvalue()


def thread_main(client_socket: socket.socket, client_addr: tuple[str, int]):
    print(f"Connection from: {client_addr[0]}:{client_addr[1]}")

    img_bytes: bytes = gen_image()
    client_socket.send(img_bytes)
    client_socket.close()


if __name__ == '__main__':
    threads: list[Thread] = []

    TCP_IP = "127.0.0.1"
    TCP_PORT = 8089

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind((TCP_IP, TCP_PORT))
    print(f"Bind to addr: {TCP_IP}:{TCP_PORT}")

    sock.listen()

    try:
        while True:
            client_sock, addr = sock.accept()

            t = Thread(target=thread_main, args=(client_sock, addr))
            t.start()
            # t.join()
    except KeyboardInterrupt:
        sock.close()
