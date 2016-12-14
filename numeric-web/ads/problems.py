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


def func_to_tabulated(data, a=0.0, b=1.0, n=1000):
    if data['type'] != 'eval':
        return data['value']

    def func(x):
        return float(eval(
            data['value'],
            {
                'abs': np.abs, 'sin': np.sin, 'cos': np.cos,
                'pow': np.power, 'log': np.log, 'e': np.e, 'pi': np.pi
            },
            {'x': x})
        )

    return numeric.tabulate_linspace(func, a, b, n)


def tabulate(args):
    a = float(args['1-a'])
    b = float(args['2-b'])
    n = int(args['3-n'])

    ret = numeric.tabulate_chebyshev(lambda x: a * x * (b - x), -1.0, 1.0, n)

    return [
        {
            'type': 'title',
            'data': '\\text{{Задание функции }}\\rho(\\omega) = {:.2f}\\omega({:.2f} - \\omega)'.format(a, b)
        },
        {
            'type': 'function',
            'description': '\\rho(\\omega)',
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
            'description': 'integral',
            'filename': 'integral',
            'data': ret
        },
        {
            'type': 'graph',
            'description': 'График ρ',
            'data': func
        },
        {
            'type': 'graph',
            'description': 'График интеграла',
            'data': ret
        },
    ]


def solve_cauchy(rho, S, z, x0, y0, T, beta):
    ret = numeric.solve_model(rho, S, z, x0, y0, T, beta)

    spline_S = numeric.tabulate_spline(S, 0.0, T, 1000)
    spline_x = numeric.tabulate_spline(ret[0], 0.0, T, 1000)
    diff = (spline_S[0], (np.array(spline_x[1]) - np.array(spline_S[1])).tolist())

    return [
        {
            'type': 'title',
            'data': '\\Phi = 1.0 \cdot {:.2f} + 10.0 \cdot {:.2f} = {:.2f}'.format(
                ret[2], ret[3], ret[4]
            )
        },
        {
            'type': 'function',
            'description': 'x',
            'filename': 'x',
            'data': ret[0]
        },
        {
            'type': 'function',
            'description': 'y',
            'filename': 'y',
            'data': ret[1]
        },
        {
            'type': 'graph',
            'description': 'График ρ',
            'data': rho
        },
        {
            'type': 'graph',
            'description': 'График x',
            'data': ret[0]
        },
        {
            'type': 'graph',
            'description': 'График S',
            'data': S
        },
        {
            'type': 'graph',
            'description': 'График z',
            'data': z
        },
        {
            'type': 'graph',
            'description': 'График x(t) - S(t)',
            'data': diff
        },
        {
            'type': 'graph',
            'description': 'График y',
            'data': ret[1]
        },
    ]


def cauchy(args):
    ret = solve_cauchy(
        func_to_tabulated(args['1-rho']),
        func_to_tabulated(args['2-S'], 0.0, args['6-T']),
        func_to_tabulated(args['3-z'], 0.0, args['6-T']),
        args['4-x0'], args['5-y0'], args['6-T'], args['7-beta']
    )

    return [
        {
            'type': 'title',
            'data': '\\text{Решение задачи Коши}'
        },
    ] + ret


def beta_search(args):
    rho = func_to_tabulated(args['1-rho'])
    S = func_to_tabulated(args['2-s'], 0.0, args['6-T'])
    z = func_to_tabulated(args['3-z'], 0.0, args['6-T'])

    beta = numeric.beta_search(
        rho, S, z,
        args['4-x0'], args['5-y0'], args['6-T'],
        args['8-beta_begin'], args['9-beta_end']
    )

    ret = solve_cauchy(
        rho, S, z,
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
    ] + ret


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
            '1-rho': ProblemArgument('\\rho(\\omega)', True),
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
        '\\beta Search \in \[beta_{begin}, beta_{end}\]',
        {
            '1-rho': ProblemArgument('\\rho(\\omega)', True),
            '2-S': ProblemArgument('S(t)', True),
            '3-z': ProblemArgument('z(t)', True),
            '4-x0': ProblemArgument('x_0', False),
            '5-y0': ProblemArgument('y_0', False),
            '6-T': ProblemArgument('T', False),
            '7-beta_begin': ProblemArgument('beta_{begin}', False),
            '8-beta_end': ProblemArgument('beta_{end}', False),
        },
        beta_search
    )
}
