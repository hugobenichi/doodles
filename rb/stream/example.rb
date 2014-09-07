

module Flowing



end




f = Flowing::Future.new do nantoka end

f = Flowing.future do nantoka end

s = Flowing::Stream.new do nantoka end

s = Flowing.stream

s = Flowing.from_file :by_line, :buffered, :read =>  do |file|


end


s = Flowing.from_
