from collections import deque
import operator


def infix_to_postfix(s: list):
    d = deque()
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


def solve(rpn: list):
    d = deque()
    for ch in rpn:
        if ch in ['+', '-', '*', '/']:
            a, b = int(d.pop()), int(d.pop())

            if ch == '+':
                d.append(a + b)
            elif ch == '-':
                d.append(b - a)
            elif ch == '*':
                d.append(a * b)
            elif ch == '/':
                d.append(b / a)
        else:
            d.append(ch)

    return int(d[0])


s = input().split()

l = infix_to_postfix(s)

print(solve(l))

