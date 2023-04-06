from collections import deque

s = input().split()
d = deque()

def infix_to_postfix(s):
    op_dict = {
        '_': (-1, 'left'),
        # '(': (0.5, 'left'),
        '+': (0, 'left'),
        '-': (0, 'left'),
        '*': (5, 'left'),
        '/': (5, 'left'),
        '^': (10, 'right'),
    }

    operators = op_dict.keys()

    out = []

    for ch in s:
        if ch in operators:
            # if ch == '/':
            #     import pdb; pdb.set_trace()
            y = d[-1] if len(d) > 0 else '_'
            x = ch
            # print(x, y, operators, y in operators)

            while len(d) > 0 and d[-1] in operators and (op_dict[d[-1]][0] >= op_dict[x][0]):
                f = True
                y = d.pop()
                out.append(y)

            # if (len(d) > 0 and d[-1] in operators and op_dict[x][0] > op_dict[d[-1]][0]) or len(d) == 0:
            d.append(x)

        elif ch == '(':
            d.append(ch)

        elif ch == ')':
            a = d.pop()
            while a != '(':
                out.append(a)
                a = d.pop()

        else:
            out.append(ch)

        # print(d, out)

    if len(d) > 0:
        out += reversed(list(d))

    return out
