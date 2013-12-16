class Solution
  attr_accessor :cmd, :failed, :passed, :runtime
  
  class << self
    def all
      @all ||= []
      
      Dir.glob("./solutions/*.rb").each do |rb|
        @all << new("ruby #{rb}")
      end
      
      Dir.glob("./solutions/*.sh").each do |sh|
        @all << new("bash #{sh}")
      end
      
      Dir.glob("./solutions/*.c").each do |c|
        `gcc #{c} -o #{c}.out && chmod +x #{c}.out`
        @all << new("#{c}.out")
      end
      
      @all
    end
    
    def samples
      @samples ||= Dir.glob("./samples/*.{0,1,2}").map do |sample|
        {
          file: sample,
          in: File.open(sample).read,
          out: File.extname(sample).gsub(/^\./, "")
        }
      end
    end
  end
  
  def initialize(cmd)
    @cmd = cmd
    @failed = @passed = @runtime = 0
  end
  
  def run
    puts "Name: #{name}"
    runtimes = []
    
    self.class.samples.each do |sample|
      result = ""
      
      3.times do
        start = Time.new
        result = `#{cmd} "#{sample[:in]}"`
        runtimes << Time.new - start
      end
      
      if result.strip == sample[:out]
        @passed += 1
        print "."
      else
        @failed += 1
        print "F"
      end
    end
    
    @runtime = runtimes.instance_eval { reduce(:+) / size.to_f }
    puts "", "Failed: #{failed}, passed #{passed}, avg runtime #{runtime}"
    puts "="*60, ""
    
    self
  end
  
  def name
    File.basename(cmd)
  end
  
  def passed?
    @failed == 0
  end
end

if ARGV.any?
  results = [Solution.new(ARGV.first).run]
else
  results = Solution.all.map(&:run)
end

winners = results.select(&:passed?).sort_by(&:runtime)
puts "WINNERS!"
puts winners.any? ? winners.each_with_index.map { |v, k| "#{k+1}: #{v.name}" } : "No one :("
exit winners.any? ? 0 : 1
