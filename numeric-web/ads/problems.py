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
    return numeric.tabulate(func, args['n'])


def tabulate_integral(args):
    return numeric.tabulate_integral(args['rho'], args['y'])


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
            '2-y': ProblemArgument('y', False)
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
        None
    )
}
