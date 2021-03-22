from functools import partial
from typing import (Callable,
                    Iterable,
                    Tuple,
                    TypeVar)

_T1 = TypeVar('_T1')
_T2 = TypeVar('_T2')


def sort_pair(pair: Tuple[_T1, _T1]) -> Tuple[_T1, _T1]:
    first, second = pair
    return pair if first < second else (second, first)


def pack(function: Callable[..., _T2]) -> Callable[[Iterable[_T1]], _T2]:
    return partial(apply, function)


def apply(function: Callable[..., _T2], args: Iterable[_T1]) -> _T2:
    return function(*args)
