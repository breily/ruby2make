require 'singleton'

#
# # Classes
#

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
        @variables = { :CC => "gcc" }
        @rules = []
    end
    def status
        puts "variables: "
        @variables.each_pair { |k, v| puts "\t#{k} \t=> \t#{v}" }
        puts "rules: "
        @rules.each { |r| puts "\t#{r.name}" }
    end    
end

#
# # DSL Methods
#

def vars(var_dict)
    var_dict.each_pair { |k, v| Makefile.instance.variables[k] = v }
end

def rule(name, &block)
    Makefile.instance.rules << Rule.new(name, &block)
end

def depend(*args)
    puts "adding dependencies: #{args}"
end

# compilation method, will use val of :CC
def compile(*args)
    puts "compiling: #{args}"
end

# shell command
def command(*args)
    puts "executing: #{args}"
end

# Shortcut to create a 'clean: ' rule
def clean(cmd)
    puts "cleaning: #{cmd}" 
end

#
# # Load and test a file
#

load 'file1.rb'
Makefile.instance.status
