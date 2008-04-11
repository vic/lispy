require 'module_uninclude'

module Lispy

  module Mutation
    class << self
      def apply(list)
        
      end
    end
  end

  class Name
    def initialize(name)
      @name = name.to_sym
    end

    def to_sym
      @name
    end

    def to_s
      @name.to_s
    end

    def ==(other)
      self.to_sym == other.to_sym
    end
  end

  class Nil
    def self.instance
      @instance ||= new
    end
  end
  
  # (cons <car> <cdr>)
  class Cons
    include Enumerable
    
    def initialize(car = nil, cdr = nil)
      self.car, self.cdr = car, cdr
    end

    attr_accessor :car, :cdr

    def empty?
      !(car && cdr)
    end
    
    def size
      (car.nil? ? 0 : 1) + (cdr.nil? ? 0 : 1)
    end

    def each
      if car
        yield car
        yield cdr if cdr
      end
    end
  end

  # (list <items...>)
  class List
    include Enumerable
    
    def initialize(*items)
      @cons = Cons.new(items.shift)
      items.inject(@cons) do |cons, item|
        cons.cdr = List.new(item)
      end
    end
    
    def car
      @cons.car
    end

    def cdr
      @cons.cdr
    end

    def car=(car)
      @cons.car = car
    end

    def cdr=(cdr)
      @cons.cdr = cdr
    end

    def empty?
      cons.emtpy?
    end

    def each
      current = @cons
      while current
        head = current.car
        yield head if head
        current = current.cdr
      end
    end

    # Obtain element at index @n@ on this list
    def nth(n)
      if n <= 0
        car
      elsif cdr
        cdr.nth(n - 1)
      end
    end

    # Obtain the list whose head is element at index @n@ on this list
    def [](n)
      if n <= 0
        self
      elsif @cons.cdr
        @cons.cdr[n - 1]
      end
    end

    def to_s
      '(' + to_a.join(' ') + ')'
    end

    def to_a
      super.map { |a| List === a ? a.to_a : a }
    end

    alias_method :inspect, :to_s
  end

  # (lambda <arguments> <body...>)
  module Lambda
    def arguments
      self[1]
    end
    
    def body
      self[2]
    end
  end

end
