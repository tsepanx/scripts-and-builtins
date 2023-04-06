import os
import socket
import time

from PIL import Image
from threading import Lock
from multiprocessing import Pool, cpu_count
from concurrent.futures import ThreadPoolExecutor

SERVER_URL = '127.0.0.1:8089'
FILE_NAME = 'NameSurname.gif'
CLIENT_BUFFER = 1024
FRAME_COUNT = 5000

frames_dir_lock = Lock()


def download_frames():
    t0 = time.time()
    if not os.path.exists('frames'):
        os.mkdir('frames')

    # threads: list[Thread] = []
    # for i in range(FRAME_COUNT):
    #     thread = Thread(target=thread_main, args=(i,), name=f"T{i}")
    #     thread.start()
    #     threads.append(thread)
    #
    # for t in threads:
    #     print("Joining", t.name)
    #     t.join()

    # Use threads pool, because threads starvation occurs on FRAME_COUNT > 200
    with ThreadPoolExecutor(max_workers=100) as executor:
        for i in range(FRAME_COUNT):
            executor.submit(thread_main, i)

    return round(time.time() - t0, 2)


def thread_main(i: int):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    ip, port = SERVER_URL.split(':')
    sock.connect((ip, int(port)))
    image = b''
    while True:
        packet = sock.recv(CLIENT_BUFFER)
        if not packet:
            break
        image += packet
    with frames_dir_lock:
        with open(f'frames/{i}.png', 'wb') as f:
            f.write(image)


def process_frame(frame_id):
    with frames_dir_lock:
        return Image.open(f"frames/{frame_id}.png").convert("RGBA")


def create_gif():
    t0 = time.time()
    # frames = []

    with Pool(processes=cpu_count()) as pool:
        frames = pool.map(process_frame, range(FRAME_COUNT))

    frames[0].save(FILE_NAME, format="GIF", append_images=frames[1:], save_all=True, duration=500, loop=0)

    return round(time.time() - t0, 2)


if __name__ == '__main__':
    print(f"Frames download time: {download_frames()}")
    print(f"GIF creation time: {create_gif()}")
