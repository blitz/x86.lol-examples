# Get rid of default rules
.SUFFIXES:

.PHONY: all
all: main-secret main-plain

CXXFLAGS=-Os -std=c++14

%.s: %.cpp
	$(CXX) $(CXXFLAGS) -S -c $< -o $@

%.se: %.s secret.txt
	./embed.py "$$(cat secret.txt)" < $< > $@

%-plain.o: %.s
	as $< -o $@

%-secret.o: %.se
	as $< -o $@

main-plain: main-plain.o
	$(CXX) $(CXXFLAGS) -o $@ $^

main-secret: main-secret.o
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f *.s *.se *.o main-plain main-secret
