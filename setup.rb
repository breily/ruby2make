#!/usr/bin/env ruby

require 'find'
require 'ftools'

COMMAND = 'rbmake'
BIN_DIR = "/usr/bin/#{COMMAND}"

puts "> copying make.rb to #{BIN_DIR}..."
puts "> overwriting..." if File.exists? BIN_DIR
File.copy 'make.rb', BIN_DIR

puts "> setup complete; use Ruby2Make as #{COMMAND}"
