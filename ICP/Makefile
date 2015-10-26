RM = rm -f
RMD = rm -rf
sub-make:
	$(MAKE) -C src

all: sub-make doxygen

clean:
	$(MAKE) clean -C src

doxygen:
	$(MAKE) doxygen -C src


pack:
	$(MAKE) pack -C src

run:
	$(MAKE) run -C src
