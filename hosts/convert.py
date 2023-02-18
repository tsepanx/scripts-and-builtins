#!/bin/python3

# Converts hosts_raw.txt -> hosts

import sys
import requests

def list_by_yield(func):
    def wrapper(*args, **kwargs):
        gen = func(*args, **kwargs)
        return list(gen)

    return wrapper

def many_not(arr, s):
    return True not in [arr[i] in s for i in range(len(arr))]

@list_by_yield
def lines_strip(arr):
    blacklist = ['#']
    for i in arr:
        l = i.strip()
        if l and many_not(blacklist, l):
            yield l

@list_by_yield
def hosts_strip(lines):
    blacklist = ['#', 'localhost']
    # import pdb; pdb.set_trace()

    for l in lines:
        if not many_not(blacklist, l):
            # yield l
            continue

        l = l[7:] if l.startswith('0.0.0.0') else l
        l = l[9:] if l.startswith('127.0.0.1') else l
        l = l.strip()

        yield f'0.0.0.0 {l}'


@list_by_yield
def main(hosts_urls):
    global_cnt = 0

    for hosts_url in hosts_urls:
        hosts_text = requests.get(hosts_url).text.split('\n')
        hosts_text = lines_strip(hosts_text)

        hosts_arr = hosts_strip(hosts_text)
        cnt = len(hosts_arr)

        hosts_info = f'{hosts_url}: {cnt}'
        print(hosts_info)

        global_cnt += cnt

        yield f'# {hosts_info}\n'
        for i in hosts_arr:
            yield i

        yield '\n\n\n'

    global_info = f'Global count: {global_cnt}'
    print(global_info)

    yield '# ' + global_info


filename = sys.argv[-1]
print(filename)

fin = open(filename, 'r')

hosts_urls = lines_strip(fin.readlines())
res = main(hosts_urls)

ask = input('Write to file?')
if ask.lower() != 'n':
    fout = open(f'res_hosts.txt', 'w')
    print(len(res))
    for i in res:
        fout.write(i + '\n')
