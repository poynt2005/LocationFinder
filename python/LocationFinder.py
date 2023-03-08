import PyLocationFinder as lf
from enum import Enum
from typing import List


class finer_type(Enum):
    VISUAL_STUDIO = 0
    PYTHON = 1
    NODE_JS = 2


def finder(input_type: finer_type) -> List:
    finder_type_str = ''
    if input_type == finer_type.VISUAL_STUDIO:
        finder_type_str = 'visual_studio'
    elif input_type == finer_type.PYTHON:
        finder_type_str = 'python'
    elif input_type == finer_type.NODE_JS:
        finder_type_str = 'node_js'

    return lf.Finder(finder_type_str)
