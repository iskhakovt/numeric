# Отчёт

## Используемые программные средства:
* Численные вычисления: C++, Python API для связи C++ и Python
* UI: Django, Celery, ReactJS, plotly.js

## Сигнатуры функций, доступных в Python API:
* `tabulate_linspace(функция, a, b, количество точек)`
* `tabulate_chebyshev(функция, a, b, количество точек)`
* `tabulate_polynomial(табулированная ункция, a, b, количество точек)`
* `tabulate_spline(табулированная ункция, a, b, количество точек)`
* `integral_gauss_kronrod(функция, a, b, количество точек)`
* `integral_simpson(функция, a, b, количество точек)`
* `tabulate_integral(табулированная функция)`
* `gaussian_elimination(матрица, вектор)`
* `lu_decomposition(матрица, вектор)`
* `tridiagonal_thomas(вектор, вектор, вектор)`
* `runge_kutta(функция от 3 аргументов, функция от 3 аргументов, x0, y0, T, количество точек)`
* `solve_mode(аргументы модели, beta)`,
* `beta search(аргументы модели, beta_start, beta_end)`

## Модель:
* [Модель показа рекламы](model.pdf)

## Тестирование:
* [Численное интегрирование](integral.ipynb)
* [Решение СЛАУ и сплайн](linear system and spline.ipynb)
* [Решение задачи Коши](differential equation.ipynb)
