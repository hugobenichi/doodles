#!/usr/bin/env python3


class Square(object):

  def __init__(self,side):
    self.side = side

  def __get_area(self):
    print("foo")
    return self.side ** 2

  area = property(__get_area)

  @property
  def perimeter(self):
    return self.side * 4

s = Square(3)

print(s, s.side, s.area, s.perimeter)
