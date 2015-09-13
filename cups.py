import sys

MAX_VOL_X = int(sys.argv[1]) if len(sys.argv) >= 2 else 9
MAX_VOL_Y = int(sys.argv[2]) if len(sys.argv) >= 3 else 4
TARGET_VOLUME = int(sys.argv[3]) if len(sys.argv) >= 4 else 2

class CupState:
	volume_x = 0
	volume_y = 0
	parent = False

	def __init__(self , vol_x , vol_y , parent_state):
		self.volume_x = vol_x
		self.volume_y = vol_y
		self.parent = parent_state

	def empty_x(self):
		return CupState(0 , self.volume_y , self)

	def empty_y(self):
		return CupState(self.volume_x , 0 , self)

	def fill_x(self):
		return CupState(MAX_VOL_X , self.volume_y , self)

	def fill_y(self):
		return CupState(self.volume_x , MAX_VOL_Y , self)

	def pour_x_to_y(self):
		y = min(MAX_VOL_Y , self.volume_y + self.volume_x)
		x = self.volume_x - (y-self.volume_y)
		return CupState(x, y, self)

	def pour_y_to_x(self):
		x = min(MAX_VOL_X , self.volume_y + self.volume_x)
		y = self.volume_y - (x-self.volume_x)
		return CupState(x, y, self)

	def isTargetVolume(self):
		return self.volume_y == TARGET_VOLUME or self.volume_x == TARGET_VOLUME

bfs_queue = [];
initial_state = CupState(0 , 0 , False)
bfs_queue.append(initial_state)
visited_states = [[0 for x in range(MAX_VOL_X + 1)] for y in range(MAX_VOL_Y + 1)]

while len(bfs_queue) > 0:
	current_state = bfs_queue.pop(0)
	if visited_states[current_state.volume_y][current_state.volume_x]:
		continue

	if current_state.isTargetVolume():
		num_x = []
		num_y = []
		while True:
			num_x.append(current_state.volume_x)
			num_y.append(current_state.volume_y)
			current_state = current_state.parent
			if current_state == False:
				break

		print("Found a solution with %s steps!" % (len(num_x) - 1))
		while len(num_x):
			print("(%s , %s)" % (num_x.pop() , num_y.pop()))
		exit()

	visited_states[current_state.volume_y][current_state.volume_x] = True
	for mutation in range(0,6):

		if mutation == 0: next_state = current_state.empty_x()
		elif mutation == 1: next_state = current_state.empty_y()
		elif mutation == 2: next_state = current_state.pour_x_to_y()
		elif mutation == 3: next_state = current_state.pour_y_to_x()
		elif mutation == 4: next_state = current_state.fill_x()
		elif mutation == 5: next_state = current_state.fill_y()
		#print next_state
		#print len(visited_states), len(visited_states[0]), next_state.volume_y, next_state.volume_x
		if visited_states[next_state.volume_y][next_state.volume_x] == False:
			bfs_queue.append(next_state)

print("No possible solutions!")
