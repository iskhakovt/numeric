# Copyright (c) Timur Iskhakov.
# Distributed under the terms of the MIT License.


import numeric


class ProblemArgument:
    def __init__(self, description, isFunction):
        self.description = description
        self.isFunction = isFunction

    def get_dict(self):
        return {
            'description': self.description,
            'isFunction': self.isFunction
        }


class Problem:
    def __init__(self, short_description, description, args, func):
        self.short_description = short_description
        self.description = description
        self.args = args
        self.func = func

    def get_dict(self):
        return {
            'short_description': self.short_description,
            'description': self.description,
            'args': {arg: self.args[arg].get_dict() for arg in self.args}
        }


def tabulate(args):
    a = float(args['1-a'])
    b = float(args['2-b'])
    n = int(args['3-n'])

    func = lambda x: a * x * (b - x)
    ret = numeric.tabulate(func, n)

    return [
        {
            'type': 'text',
            'data': '\\text{Задание функции }\\rho(\\omega) = ' + \
                str(round(a, 2)) + \
                '\\omega(' + str(round(b, 2)) + \
                ' - \\omega)'
        },
        {
            'type': 'function',
            'description': '\\text{Загрузить }\\rho(\\omega)',
            'filename': 'rho',
            'data': ret
        },
        {
            'type': 'graph',
            'description': 'График',
            'data': ret
        },
    ]


def tabulate_integral(args):
    ret = numeric.tabulate_integral(args['1-rho'])

    return [
        {
            'type': 'text',
            'data': '\\text{Табулирование интеграла}'
        },
        {'type': 'function', 'description': '\\rho(\\omega)', 'data': ret}
    ]


def cauchy(args):
    ret = numeric.cauchy(
        args['1-U'], args['2-S'], args['3-z'],
        args['4-x0'], args['5-y0'], args['6-beta'], args['7-T']
    )

    return [
        {
            'type': 'text',
            'data': '\\text{Решение задачи Коши}'
        },
        {'type': 'function', 'description': 'f', 'data': ret}
    ]


PROBLEMS = {
    '1-tabulate': Problem(
        '\\text{Задание функции }\\rho(\\omega)',
        '\\text{Задание функции }\\rho(\\omega) = a\\omega(b - \\omega)',
        {
            '1-a': ProblemArgument('a', False),
            '2-b': ProblemArgument('b', False),
            '3-n': ProblemArgument('\\text{Количество точек}', False)
        },
        tabulate
    ),
    '2-tabulate-integral': Problem(
        '\\text{Табулирование интеграла}',
        '\\text{Табулирование интеграла }\\int\\limits_y^1 \\rho(\\omega) \\operatorname{d}\\!{\\omega} = U(y)',
        {
            '1-rho': ProblemArgument('\\rho(\\omega)', True),
        },
        tabulate_integral
    ),
    '3-cauchy': Problem(
        '\\text{Решение задачи Коши}',
        '\\text{Решение задачи Коши}',
        {
            '1-U': ProblemArgument('U(y)', True),
            '2-S': ProblemArgument('S(t)', True),
            '3-z': ProblemArgument('z(t)', True),
            '4-x0': ProblemArgument('x_0', False),
            '5-y0': ProblemArgument('y_0', False),
            '6-beta': ProblemArgument('\\beta', False),
            '7-T': ProblemArgument('T', False),
        },
        cauchy
    )
}
