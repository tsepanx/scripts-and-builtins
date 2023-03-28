import argparse
import socket

RECV_SIZE = 20480


class MessageType:
    START = "s"
    DATA = "d"


def my_send(sock: socket.SocketType, addr: str, msg: str):
    print(f"SEND: {msg}")
    sock.sendto(msg.encode("utf-8"), addr)


def main_server(server_sock: socket.SocketType):
    recv_buff = b""
    cur_seq_num = 1  # 0 was received as start

    fname, fsize = None, None
    cur_fsize = 0

    while True:
        data, addr = server_sock.recvfrom(RECV_SIZE)

        print(f"RECV: {data}")

        msg_type, seq_num_recv = data[:3].decode("utf-8").split("|")
        seq_num_recv = int(seq_num_recv)
        other_data: bytes = data[4:]

        if msg_type == MessageType.START:
            fname, fsize_str = other_data.decode("utf-8").split("|")
            fsize = int(fsize_str)
            my_send(server_sock, addr, "a|1")

        elif msg_type == MessageType.DATA:
            assert fsize is not None

            if cur_seq_num % 2 == seq_num_recv:
                cur_seq_num += 1

                recv_buff += other_data
                cur_fsize += len(other_data)

            seq_num_send = cur_seq_num % 2
            resp_msg = f"a|{seq_num_send}"
            my_send(server_sock, addr, resp_msg)
        else:
            raise Exception(f"Unknown msg_type: {msg_type}")

        if cur_fsize >= fsize:
            with open(fname, 'wb') as fout:
                fout.write(recv_buff)
            return


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("server_port", type=str)
    args = parser.parse_args()

    UDP_IP = "127.0.0.1"
    UDP_PORT = int(args.server_port)

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server_socket.bind((UDP_IP, UDP_PORT))

    print(f"Bind to addr: {UDP_IP}:{UDP_PORT}")

    try:
        main_server(server_socket)
    except Exception as e:
        print(e)

    server_socket.close()
