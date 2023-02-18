import os, time, json
from urllib.parse import unquote

is_json_bookmark = lambda fname: os.path.isfile(fname) and fname.startswith('bookmarks') and fname.endswith('.json')

files = list(filter(is_json_bookmark, os.listdir('.')))
fname = files[0]

print(fname)
# time.sleep(5)

fin = open(fname)

d = json.loads(fin.readline())


result = []

def main(d):
    CHILDREN = 'children'
    TITLE = 'title'
    URL = 'uri'

    if CHILDREN in d:
        for c in d[CHILDREN]:
            main(c)

    elif TITLE in d and URL in d:
            title = d[TITLE]
            url = d[URL]

            result.append([title, url])

main(d)

print(result)

result.sort(key=lambda x: x[0])

fout = open('result123.md', 'w')

def convert_links_to_markdown(arr):
    link_to_md = lambda x: unquote(f'[{x[0]}]({x[1]})')

    md_links = list(map(link_to_md, arr))

    return md_links

res = convert_links_to_markdown(result)

res = '\n'.join(sorted(set(res)))

fout.write(res)
