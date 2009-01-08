require 'singleton'

# Classes

class Rule
    attr_reader :name, :dependencies, :compilations, :commands
    def initialize(name)
        @name = name
        @dependencies = []
        @compilations = []
        @commands = []
    end
    def depend(arg)
        @dependencies << arg
    end
    def compile(params={})
        puts "adding compile (#{params})"
        @compilations << params
        puts
    end
    def command(arg)
        @commands << arg
    end
end

class Makefile
    include Singleton
    attr_accessor :variables, :rules, :current_rule
    def initialize
        @variables = { :CC => "gcc" }
        @rules = []
        @current_rule = nil
    end
    def status
        puts
        @variables.each_pair { |k, v| puts "#{k} = #{v}" }
        puts
        @rules.each do |r| 
            puts "#{r.name}: #{r.dependencies.join ' '}"
            r.compilations.each do |d|
                puts "\t#{d[:input]} => #{d[:output]}"
            end
            r.commands.each do |cmd|
                puts "\t#{cmd}"
            end
        end    
    end
end

# DSL Methods

def vars var_dict
    var_dict.each_pair { |k, v| Makefile.instance.variables[k] = v }
end

def rule(name, params={}, &block)
    r = Rule.new name
    Makefile.instance.rules << r
    Makefile.instance.current_rule = r
    # Combine dependency hashes so either key can be used
    deps = [params[:depend], params[:depends]].flatten
    deps.each { |dep| depend dep }
    # Cede control to the user's block
    yield
    Makefile.instance.current_rule = nil
end

def depend(*args)
    args.each { |arg| Makefile.instance.current_rule.depend arg }
end

# compilation method, will use val of :CC
def compile(*args)
    params = { :flags => [] }
    args.each do |arg|
        case arg.class
        when Hash
            arg.each_pair { |k, v| params[k] = v }
        when Symbol, String
            case arg
            when :to_obj, "-c"
                params[:flags] << "-c"
            end
        end
    end
    Makefile.instance.current_rule.compile params
end

# shell command
def command(*args)
    args.each { |arg| Makefile.instance.current_rule.command arg }
end

def echo message
    command "echo '#{message}'"
end

# Shortcut to create a 'clean: ' rule => clean "*o ~" or clean "*o", "~"
def clean(*cmds)
    if cmds.respond_to? :join
        cmds = cmds.join " "
    end
    rule "clean" do
        command "-rm #{cmds}"
    end
end

# Load and test a file
load 'file1.rb'
Makefile.instance.status
