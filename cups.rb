VOLUME_X = ARGV[0] ? ARGV[0].to_i : 4
VOLUME_Y = ARGV[1] ? ARGV[1].to_i : 9
TARGET_VOLUME = ARGV[2] ? ARGV[2].to_i : 2

class NilClass
  def clone
    nil
  end
end

class Cups
  attr_reader :x, :y

  def initialize(x = 0, y = 0)
    @x = x
    @y = y
  end

  def ==(another)
    x == another.x and y == another.y
  end

  def eql?(another)
    self == another
  end

  def hash
    [x, y].hash
  end

  def qualify?
    @x == TARGET_VOLUME or @y == TARGET_VOLUME
  end

  def fill_x
    self.class.new(VOLUME_X, y)
  end

  def fill_y
    self.class.new(x, VOLUME_Y)
  end

  def empty_x
    self.class.new(0, y)
  end

  def empty_y
    self.class.new(x, 0)
  end

  def pour_x_y
    new_y = [VOLUME_Y, x+y].min
    new_x = x - (new_y - y)
    self.class.new(new_x, new_y)
  end

  def pour_y_x
    new_x = [VOLUME_X, x+y].min
    new_y = y - (new_x - x)
    self.class.new(new_x, new_y)
  end
end

class CupState
  attr_reader :step, :parent

  def initialize(step, parent)
    @step = step
    @parent = parent.clone
  end
end

cup_states = {}
cup_states[Cups.new(0, 0)] = CupState.new(0, nil)
bfs_queue = [Cups.new(0, 0)]
mutation_methods = [:fill_x, :fill_y, :empty_x, :empty_y, :pour_x_y, :pour_y_x]

cup_states.define_singleton_method(:child) do |cup|
  cup_state = self[cup]
  cup_state.class.new(cup_state.step + 1, cup)
end

until bfs_queue.empty?
  cur_cup = bfs_queue.shift

  if cur_cup.qualify?
    now = cur_cup
    seq = [now]
    until cup_states[now].parent.nil?
      now = cup_states[now].parent
      seq.push now
    end
    seq.reverse!
    puts "Found an answer with #{seq.length - 1} steps!"
    seq.each { |s| puts "(#{s.x}, #{s.y})"}
    break
  end

  mutation_methods.each do |sym|
    new_cup = cur_cup.send(sym)
    next unless cup_states[new_cup].nil?
    cup_states[new_cup] = cup_states.child(cur_cup)
    bfs_queue.push(new_cup)
  end
end
