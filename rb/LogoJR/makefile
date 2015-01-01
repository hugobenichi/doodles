

RUBY=./ruby
LIB=./lib


JAR=/usr/lib/jvm/java-6-sun/bin/jar



#ruby_main=$(RUBY)/logo.rb
ruby_main=debug.rb

#jar_entry=$(RUBY)/jar-bootstrap.rb
jar_entry=jar-bootstrap.rb

jruby_jar=$(LIB)/jruby.jar


scripts=screen.rb turtle.rb turtle/behavior.rb turtle/history.rb
#ruby_source=$(addprefix $(RUBY)/,$(scripts))

ruby_source=screen.rb turtle.rb behavior.rb history.rb


all: turtle.jar $(jar_entry) $(ruby_source)
	$(JAR) ufe turtle.jar org.jruby.JarBoostrapMain $(jar_entry)

turtle.jar: $(jruby_jar)
	cp $(jruby_jar) turtle.jar

$(jar_entry): $(ruby_main)
	cp $(ruby_main) $(jar_entry)

