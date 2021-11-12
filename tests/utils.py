from functools import partial
from typing import (Callable,
                    Iterable,
                    List, Tuple,
                    TypeVar)

_T1 = TypeVar('_T1')
_T2 = TypeVar('_T2')


def apply(function: Callable[..., _T2], args: Iterable[_T1]) -> _T2:
    return function(*args)


def equivalence(left_statement: bool, right_statement: bool) -> bool:
    return left_statement is right_statement


def implication(antecedent: bool, consequent: bool) -> bool:
    return not antecedent or consequent


def pack(function: Callable[..., _T2]) -> Callable[[Iterable[_T1]], _T2]:
    return partial(apply, function)


def sort_pair(pair: Tuple[_T1, _T1]) -> Tuple[_T1, _T1]:
    first, second = pair
    return pair if first < second else (second, first)


def transpose(value: List[Tuple[_T1, _T2]]) -> Tuple[List[_T1], List[_T2]]:
    if not value:
        return [], []
    first_coordinates, second_coordinates = zip(*value)
    return list(first_coordinates), list(second_coordinates)
