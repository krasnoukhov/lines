unless ARGV.any?
  puts "Usage: #{$0} sample" 
  exit
end

sample = File.open(Dir.glob("./samples/#{ARGV.first}*").first).read.split("\n")

size = sample.find { |x| x.start_with?("SIZE") }.gsub(/^SIZE\s/, "").to_i
balls = sample.select { |x| x.start_with?("BALL") }.map { |x| x.gsub(/^BALL\s/,"").split(",") }
moves = sample.select { |x| x.start_with?("MOVE") }.map { |x| x.gsub(/^MOVE\s/,"").split(",").map(&:to_i) }

([nil] + moves).each do |move|
  # Move
  if move
    curr = balls.find { |ball| ball[0].to_i == move[0] && ball[1].to_i == move[1] }
    balls << [move[2], move[3], curr[2]]
    balls.delete(curr)
    puts "MOVE: #{move}"
  end
  
  # Render
  ascii = ["+" + size.times.map { "---" }.join("+") + "+"]
  size.times do |y|
    row = size.times.map do |x|
      ball = balls.find { |ball| ball[0].to_i == x && ball[1].to_i == y }
      ball ? " #{ball[2][0]} " : "   "
    end.join("|")
    
    ascii << ["|#{row}|"]
    ascii << ["+" + size.times.map { "---" }.join("+") + "+"]
  end
  puts ascii.join("\n")
end
