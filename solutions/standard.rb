class Lines
  attr_accessor :sample, :size, :moves, :fields, :lines
  
  DIRECTIONS = [
    [[0, 1], [0, -1]],
    [[1, 0], [-1, 0]],
    [[1, 1], [-1, -1]],
    [[1, -1], [-1, 1]]
  ]
  
  class Move
    attr_accessor :from_x, :from_y, :to_x, :to_y
    
    def initialize(*args)
      [:from_x, :from_y, :to_x, :to_y].each_with_index do |v, k|
        self.send("#{v}=", args[k].to_i)
      end
    end
    
    def destination
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
    @size = @sample.find { |x| x.start_with?("SIZE") }.gsub(/^SIZE\s/, "").to_i
    @balls = @sample.select { |x| x.start_with?("BALL") }.map { |x| Ball.new(*x.gsub(/^BALL\s/,"").split(",")) }
    @moves = @sample.select { |x| x.start_with?("MOVE") }.map { |x| Move.new(*x.gsub(/^MOVE\s/,"").split(",")) }
    
    @fields = @size.times.map do |y|
      size.times.map do |x|
        @balls.find { |ball| ball.x == x && ball.y == y } || false
      end
    end
  end
  
  def result
    # Validate
    return 0 if @balls.find { |x| x.invalid?(self) }
    return 0 if @moves.find { |x| x.invalid?(self) }
    
    @lines = 0
    @moves.each do |move|
      current = get(move.from_x, move.from_y)
      set(move.to_x, move.to_y, current)
      clear(move.from_x, move.from_y)
      
      DIRECTIONS.each do |directions|
        count = 1
        
        directions.each do |direction|
          coords = move.destination
          begin
            coords[0] += direction[0]
            coords[1] += direction[1]
            
            ball = get(*coords)
            break if !ball || !current || (ball.color != current.color)
            
            count += 1
          end while get(*coords)
        end
        
        if count >= 5
          @lines += 1
        end
      end
    end
    
    @lines
  end
  
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

puts Lines.new(ARGV.first).result
