local VOLUMES = {4, 9}
for i = 1, 2 do
	VOLUMES[i] = arg[i] and tonumber(arg[i]) or VOLUMES[i]
end
local TARGET_VOLUME = arg[3] and tonumber(arg[3]) or 2

local Cups = {}
Cups.mt= {__index = Cups}

function Cups.new(t)
	local new_cups = t
	setmetatable(new_cups, Cups.mt)
	return new_cups
end

function Cups:fill(idx)
	local new_cups = Cups.new{unpack(self)}
	new_cups[idx] = VOLUMES[idx]
	return new_cups
end

function Cups:empty(idx)
	local new_cups = Cups.new{unpack(self)}
	new_cups[idx] = 0
	return new_cups
end

function Cups:pour_to(to)
	local total_volume = self[1]+self[2]
	local volume_to = math.min(VOLUMES[to], total_volume)
	return Cups.new{[3-to] = total_volume-volume_to, [to] = volume_to}
end

function Cups:fulfill_target()
	return self[1] == TARGET_VOLUME or self[2] == TARGET_VOLUME
end

function Cups:hash()
	return table.concat(self, ' ')
end

function Cups:to_string()
	return '('..table.concat(self, ', ')..')'
end

local Queue = {}
Queue.mt = {__index = Queue}

function Queue.new()
	local new_queue = {front = 1, back = 0}
	setmetatable(new_queue, Queue.mt)
	return new_queue
end

function Queue:push(x)
	self.back = self.back+1
	self[self.back] = x
end

function Queue:pop()
	local res = self[self.front]
	self[self.front] = nil
	self.front = self.front+1
	return res
end

function Queue:empty()
	return self.back-self.front == -1
end

local mutation_methods = {'fill', 'empty', 'pour_to'}
local start_state = Cups.new{0, 0}
local cups_states = {[start_state:hash()] = true}
local bfs_queue = Queue.new()
bfs_queue:push(start_state)

repeat
	local cur_cups = bfs_queue:pop()
	if cur_cups:fulfill_target() then
		local ans_arr = {}
		while cur_cups ~= start_state do
			ans_arr[#ans_arr+1] = cur_cups
			cur_cups = cups_states[cur_cups:hash()]
		end
		print("Found an answer with "..#ans_arr.." steps!")
		print(start_state:to_string())
		for i = #ans_arr, 1, -1 do
			print(ans_arr[i]:to_string())
		end
		break
	end
	for _, sym in ipairs(mutation_methods) do
		for i = 1, 2 do
			local new_cups = cur_cups[sym](cur_cups, i)
			if not cups_states[new_cups:hash()] then
				cups_states[new_cups:hash()] = cur_cups
				bfs_queue:push(new_cups)
			end
		end
	end
until bfs_queue:empty()

if bfs_queue:empty() then
	print"No possible solution!"
end

