
link:: http://blog.sina.com.cn/s/blog_54384df80100w0f9.html



# -*- coding: utf-8 -*-

from time import time

# A*地图可达数据管理接口
class AMAP:
	def __init__(self, width, height, maze):
		self._width = width
		self._height = height
		self._mask = -1
		self._maze = maze #直接指向内存结构
		self._nodeparentx = []
		self._nodeparenty = []
		self._nodegcost = []
		self._nodehcost = []
		self._size = width * height
		for i in range(self._size):
			self._nodeparentx.append(-1)
			self._nodeparenty.append(-1)
			self._nodegcost.append(0)
			self._nodehcost.append(0)
		self.reset = True

		self._left = 0
		self._top = 0
		self.right = 0
		self.bottom = 0
		self.setRectAll()

	# reset重置地图
	def resetMap(self):
		if self.reset == True:
			return #已经设置过了
		self.reset = True
		for i in range(self._size):
			self._nodeparentx[i] = -1
			self._nodeparenty[i] = -1
			self._nodegcost[i] = 0
			self._nodehcost[i] = 0
			if self._maze[i] != self._mask:
				self._maze[i] = 0


	# 搜索参数设置，每次寻路前初始化
	# 获取可达逻辑判断掩码
	def getMask(self):
		return self._mask

	# 设置搜索范围，范围之外被看成看不见
	# (left,top)为rect左上角坐标 （right,bottom)为Rect右小角坐标 单位是格子
	def setRect(self, left, top, right, bottom):
		self._left = left
		self._top = top
		self._right = right
		self._bottom = bottom


	# 搜索范围为整个地图
	def setRectAll(self):
		self._left = 0
		self._top = 0
		self._right = self._width
		self._bottom = self._height

	def left(self):
		return self._left

	def top(self):
		return self._top

	def right(self):
		return self._right

	def bottom(self):
		return self._bottom
	
	def width(self):
		return self._width

	def height(self):
		return self._height

	def walkable(self, x, y):
		# in rect。搜索范围限制
		if (x < self._left or x >= self._right or y < self._top or y >= self._bottom ):
		return False
		return self._maze[x+y*self._width] != self._mask


# A*类

