$:.unshift File.expand_path('../lib', File.dirname(__FILE__))
$:.unshift File.expand_path('../ext', File.dirname(__FILE__))
require 'lispy/core_types'

module Lispy
  list = List.new(1, 2, 3, List.new(4, 6, 7), 5)
  list.each { |n| p n }
  puts list[1]
  p list[2].to_a
end
