# Copyright (c) Timur Iskhakov.
# Distributed under the terms of the MIT License.


import numpy as np

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


def func_to_tabulated(data, a=-1.0, b=1.0, n=1000):
    if data['type'] != 'eval':
        return data['value']

    def func(x):
        return float(eval(
            data['value'],
            {'abs': np.abs, 'sin': np.sin, 'cos': np.cos, 'pow': np.power, 'log': np.log, 'e': np.e, 'pi': np.pi},
            {'x': x})
        )

    return numeric.tabulate_chebyshev(func, a, b, n)


def tabulate(args):
    a = float(args['1-a'])
    b = float(args['2-b'])
    n = int(args['3-n'])

    ret = numeric.tabulate_chebyshev(lambda x: a * x * (b - x), -1.0, 1.0, n)

    return [
        {
            'type': 'title',
            'data': '\\text{Задание функции }\\rho(\\omega) = ' +
                    str(round(a, 2)) +
                    '\\omega(' + str(round(b, 2)) +
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
    func = func_to_tabulated(args['1-rho'])
    ret = numeric.tabulate_integral(func)

    return [
        {
            'type': 'title',
            'data': '\\text{Табулирование интеграла}'
        },
        {
            'type': 'function',
            'description': 'result',
            'filename': 'result',
            'data': ret
        }
    ]


def cauchy(args):
    ret = numeric.cauchy(
        args['1-U'], args['2-S'], args['3-z'],
        args['4-x0'], args['5-y0'], args['6-T'], args['7-beta']
    )

    return [
        {
            'type': 'title',
            'data': '\\text{Решение задачи Коши}'
        },
        {
            'type': 'function',
            'description': 'f',
            'filename': 'result',
            'data': ret
        }
    ]


def beta_search(args):
    beta = numeric.beta_search(
        args['1-U'], args['2-S'], args['3-z'],
        args['4-x0'], args['5-y0'], args['6-T']
    )

    model = numeric.cauchy(
        args['1-U'], args['2-S'], args['3-z'],
        args['4-x0'], args['5-y0'], args['6-T'], beta
    )

    return [
        {
            'type': 'title',
            'data': '\\beta Search'
        },
        {
            'type': 'text',
            'data': '\\beta=' + str(beta)
        },
        {
            'type': 'function',
            'description': 'f',
            'filename': 'result',
            'data': model
        }
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
            '6-T': ProblemArgument('T', False),
            '7-beta': ProblemArgument('\\beta', False),
        },
        cauchy
    ),
    '4-beta-search': Problem(
        '\\beta Search',
        '\\beta Search',
        {
            '1-U': ProblemArgument('U(y)', True),
            '2-S': ProblemArgument('S(t)', True),
            '3-z': ProblemArgument('z(t)', True),
            '4-x0': ProblemArgument('x_0', False),
            '5-y0': ProblemArgument('y_0', False),
            '6-T': ProblemArgument('T', False),
        },
        beta_search
    )
}
