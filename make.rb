require 'singleton'

class Rule
    attr_reader :name, :block
    def initialize(name, &block)
        @name = name
        @block = block
    end
    def command(text)
        puts "executing command: #{text}"
    end
end

class Makefile
    include Singleton
    attr_accessor :variables, :rules
    def initialize
        @variables = {}
        @rules = []
    end
    def status
        puts "variables: "
        @variables.each_pair { |k, v| puts "\t#{k} \t=> \t#{v}" }
        puts "rules: "
        @rules.each { |r| puts "\t#{r.name}" }
    end    
end

def variables(var_dict)
    var_dict.each_pair do |k, v|
        Makefile.instance.variables[k] = v
    end
end

def rule(name, &block)
    Makefile.instance.rules << Rule.new(name, &block)
end

def depend(*args)
    puts "adding dependencies: #{args}"
end

def compile(*args)
    puts "compiling: #{args}"
end

def command(*args)
    puts "executing: #{args}"
end

def clean(cmd)
    puts "cleaning: #{cmd}" 
end

load 'file1.rb'
Makefile.instance.status

puts "-----------------------------"

load 'file2.rb'
Makefile.instance.status

puts "-----------------------------"

load 'file3.rb'
Makefile.instance.status

