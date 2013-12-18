class Lines
  attr_accessor :sample, :size, :moves, :fields, :lines
  
  DIRECTIONS = [
    [[0, 1], [0, -1]],
    [[1, 0], [-1, 0]],
    [[1, 1], [-1, -1]],
    [[1, -1], [-1, 1]]
  ]
  
  class Move
    COORDS = [:from_x, :from_y, :to_x, :to_y]
    attr_accessor *COORDS
    
    def initialize(*args)
      COORDS.each_with_index do |v, k|
        self.send("#{v}=", args[k].to_i)
      end
    end
    
    def from
      [from_x, from_y]
    end
    
    def to
      [to_x, to_y]
    end
    
    def invalid?(lines)
      (from_x < 0 || from_y < 0 || from_x >= lines.size || from_y >= lines.size) ||
      (to_x < 0 || to_y < 0 || to_x >= lines.size || to_y >= lines.size) ||
      (!to_x.between?(from_x-1, from_x+1) || !to_y.between?(from_y-1, from_y+1))
    end
  end
  
  class Ball
    attr_accessor :x, :y, :color
    
    def initialize(x, y, color)
      @x = x.to_i
      @y = y.to_i
      @color = color
    end
    
    def invalid?(lines)
      x < 0 || y < 0 || x >= lines.size || y >= lines.size
    end
  end
  
  def initialize(sample)
    @sample = sample.split("\n")
    
    # SIZE
    @size = @sample.find { |x| x.start_with?("SIZE") }.gsub(/^SIZE\s/, "").to_i
    @lines = 0 and return self if @size < 5
    
    # BALLS
    @balls = @sample.select { |x| x.start_with?("BALL") }.map { |x| Ball.new(*x.gsub(/^BALL\s/,"").split(",")) }
    @lines = 0 and return self if @balls.find { |x| x.invalid?(self) }
    
    # MOVES
    @moves = @sample.select { |x| x.start_with?("MOVE") }.map { |x| Move.new(*x.gsub(/^MOVE\s/,"").split(",")) }
    @lines = 0 and return self if @moves.find { |x| x.invalid?(self) }
    
    @fields = @size.times.map do |y|
      size.times.map do |x|
        @balls.find { |ball| ball.x == x && ball.y == y } || false
      end
    end
  end
  
  def result
    return @lines if @lines
    @lines = 0
    
    @moves.each do |move|
      # Check for ball which will be moved
      move_ball = get(*move.from)
      return @lines unless move_ball
      
      # Move
      set(*move.to, move_ball)
      clear(*move.from)
      
      # Check each direction pair for a line
      DIRECTIONS.each do |directions|
        balls = 1
        
        directions.each do |direction|
          current_point = move.to
          
          while
            current_point[0] += direction[0]
            current_point[1] += direction[1]
            
            current_ball = get(*current_point)
            break if !current_ball || (current_ball.color != move_ball.color)
            
            balls += 1
          end
        end
        
        if balls >= 5
          @lines += 1
        end
      end
    end
    
    @lines
  end
  
  private
  
  def get(x, y)
    @fields && @fields[y] ? @fields[y][x] : false
  end
  
  def set(x, y, ball)
    if ball
      ball.x = x
      ball.y = y
    end
    @fields[y][x] = ball
  end
  
  def clear(x, y)
    set(x, y, false)
  end
end

puts Lines.new(STDIN.read).result
