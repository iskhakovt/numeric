# numeric: ad relevance prediction model [![Build Status](https://travis-ci.org/iskhakovt/numeric.svg?branch=master)](https://travis-ci.org/iskhakovt/numeric)
### Coursework [«Численные методы в анализе данных» \[RU\]](http://wiki.cs.hse.ru/%D0%A7%D0%B8%D1%81%D0%BB%D0%B5%D0%BD%D0%BD%D1%8B%D0%B5_%D0%BC%D0%B5%D1%82%D0%BE%D0%B4%D1%8B_%D0%B2_%D0%B0%D0%BD%D0%B0%D0%BB%D0%B8%D0%B7%D0%B5_%D0%B4%D0%B0%D0%BD%D0%BD%D1%8B%D1%85)

*Timur Iskhakov, 141*

## Modules:

* `numeric-core` — computation implemented in C++
* `numeric-web` — UI implemented in Django and ReactJS

## Deploy:

* Module deploy is explained in corresponding `README` files
* `numeric-web` requires built dynamic lib `numeric`, which location is set up in [settings.py](numeric-web/numeric_web/settings.py) config file, `BASE_DIR/../numeric-core` is used by default, where `BASE_DIR` stands for `numeric-web` deploy path
