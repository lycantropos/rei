from reprit.base import generate_repr

from .status_code import StatusCode


class Status:
    __slots__ = 'code', 'error_arg'

    def __init__(self,
                 code: StatusCode = StatusCode.SUCCESS,
                 error_arg: str = '') -> None:
        self.code = code
        self.error_arg = error_arg

    __repr__ = generate_repr(__init__)

    def __str__(self) -> str:
        return f'{self.code}: {self.error_arg}'
