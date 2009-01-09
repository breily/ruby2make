require 'singleton'

# -=- Classes -=-

class Rule
    attr_reader :name, :dependencies, :compilations, :commands
    def initialize(name)
        @name = name
        @dependencies = []  # list of strings
        @compilations = []  # list of hashes
        @commands = []      # list of strings
    end
    def depend(arg)
        @dependencies.push arg
    end
    def compile(params={})
        # Find the input files for the compile command (usually dependencies)
        # Why are there nils in @dependencies?
        deps = @dependencies.find_all { |d| d.split('.').last != 'h' unless d.nil? }
        params[:input] = deps if params[:input].nil?
        # Make it a list again and then flatten it in case it was just one string,
        # instead of a list
        params[:input] = [params[:input]].flatten.join " "
        # Find the output file, if any
        if params[:output].nil?
            params[:output] = ""
        else
            params[:output] = "-o #{params[:output]}"
        end
        params[:flags] = params[:flags].join " "
        @compilations.push params
    end
    def command(arg)
        @commands.push arg
    end
end

class Makefile
    include Singleton
    attr_accessor :variables, :suffixes, :rules, :current_rule
    def initialize
        @variables = { :CC => "gcc", :FLAGS => "", :SHELL => "/bin/sh" }
        @suffixes = []
        @rules = []
        @current_rule = nil
    end
    def render
        fp = File.new("Makefile", "w")
        fp.write "# Generated by Ruby2Make\n\n"
        @variables.each_pair do |k, v|
            v = v.collect { |i| symbol2macro i }.join " " if v.respond_to? :join
            fp.write "#{k} = #{v}\n"
        end
        @suffixes.each do |s|
            fp.write "\n"
            fp.write ".SUFFIXES: #{s[0]} #{s[1]}\n"
        end    
        @rules.each do |r| 
            fp.write "\n"
            fp.write "#{r.name}: #{r.dependencies.join ' '}\n"
            r.compilations.each do |d|
                fp.write "\t$(CC) $(FLAGS) #{d[:flags]} #{d[:input]} #{d[:output]}\n"
            end
            r.commands.each do |cmd|
                fp.write "\t#{cmd}\n"
            end
        end
        fp.close
    end
end

# -=- DSL Methods -=-

def vars var_dict
    var_dict.each_pair { |k, v| Makefile.instance.variables[k] = v }
end

def rule(name, params={}, &block)
    r = Rule.new name
    Makefile.instance.rules << r
    Makefile.instance.current_rule = r
    # Combine dependency hashes so different keys can be used
    deps = [params[:depend], params[:depends], params[:d]].flatten
    deps.each { |dep| depend dep }
    # Give control to the user's block
    yield if block_given?
    Makefile.instance.current_rule = nil
end

# Add a dependency (or list of them) to the rule
def depend(*args)
    args.each { |arg| Makefile.instance.current_rule.depend(symbol2macro(arg)) }
end

# Compilation method, uses val of :CC and :FLAGS
def compile(*args)
    # List of user-added flags
    params = { :flags => [] }
    args.each do |arg|
        if arg.class == Hash
            arg.each_pair { |k, v| params[k] = symbol2macro v }
        elsif [String, Symbol].include? arg.class
            case arg
            # Compile to object code
            when :to_obj, "-c"
                params[:flags].push "-c"
            when
                # Transform args like :MACRO into $(MACRO)
                if arg.class == Symbol
                    params[:flags].push "$(#{arg})"
                end
            end
        end
    end
    Makefile.instance.current_rule.compile params
end

# Add shell commands
def command(*args)
    args.each { |arg| Makefile.instance.current_rule.command arg }
end
# Shortcut for 'echo' command
def echo message; command "@echo '#{message}'"; end

# Shortcut to create a 'clean: ' rule => clean "*o ~" or clean "*o", "~"
def clean(*cmds)
    if cmds.respond_to? :join
        tmp_cmds = cmds.collect { |c| symbol2macro c }
        cmds = tmp_cmds.join " "
    end
    rule "clean" do
        command "-rm #{cmds}"
    end
end

# Add a suffix rule (.SUFFIXES)  Necessary?
def suffix ext1, ext2
    Makefile.instance.suffixes.push [ext1, ext2]
end

#  -=- Utility -=-

# Transforms a symbol into Make's macro form
def symbol2macro sym
    if sym.class == Symbol
        "$(#{sym})"
    else
        sym
    end
end

# -=- Run -=-
if ARGV.length == 0
    load 'Makefile.rb'
else
    load ARGV[0]
end
Makefile.instance.render
