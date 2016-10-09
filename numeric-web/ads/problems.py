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
    func = lambda x: args['a'] * x * (args['b'] - x)
    return numeric.tabulate_chebyshev(func, args['n'])


def tabulate_integral(args):
    return numeric.tabulate_integral(args['rho'], args['y'])


PROBLEMS = {
    '1-tabulate': Problem(
        '\\text{Задание функции }\\rho(\\omega)',
        '\\text{Задание функции }\\rho(\\omega) = a\\omega(b - \\omega)',
        {
            'a': ProblemArgument('a', False),
            'b': ProblemArgument('b', False),
            'n': ProblemArgument('\text{Количество точек}', False)
        },
        tabulate
    ),
    '2-tabulate-integral': Problem(
        '\\text{Табулирование интеграла}',
        '\\text{Табулирование интеграла }\\int\\limits_y^1 \\rho(\\omega) \\operatorname{d}\\!{\\omega} = U(y)',
        {
            'rho': ProblemArgument('\\rho(\\omega)', True),
            'y': ProblemArgument('y', False)
        },
        tabulate_integral
    ),
    '3-cauchy': Problem(
        '\\text{Решение задачи Коши}',
        '\\text{Решение задачи Коши}',
        {
            'U': ProblemArgument('U(y)', True),
            'S': ProblemArgument('S(t)', True),
            'z': ProblemArgument('z(t)', True),
            'x0': ProblemArgument('x_0', False),
            'y0': ProblemArgument('x_0', False),
            'beta': ProblemArgument('\\beta', False),
            'T': ProblemArgument('T', False),
        },
        None
    )
}
