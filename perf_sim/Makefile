
# specifying relative path to the TRUNK
TRUNK= ../

# paths to look for headers
vpath %.h $(TRUNK)/common
vpath %.h $(TRUNK)/perf_sim/
vpath %.h $(TRUNK)/func_sim/elf_parser/
vpath %.h $(TRUNK)/func_sim/func_instr/
vpath %.h $(TRUNK)/func_sim/func_memory/
vpath %.cpp $(TRUNK)/perf_sim/
vpath %.cpp $(TRUNK)/func_sim/elf_parser/
vpath %.cpp $(TRUNK)/func_sim/func_instr/
vpath %.cpp $(TRUNK)/func_sim/func_memory/

# option for C++ compiler specifying directories
# to search for headers
INCL= -I ./ -I $(TRUNK)/common/ -I $(TRUNK)/func_sim/elf_parser/ -I $(TRUNK)/func_sim/func_memory/ -I $(TRUNK)/func_sim/func_instr

#
# Enter for building func_memory stand alone program
#
Debug: perf_sim

perf_sim: func_memory.o elf_parser.o func_instr.o log.o perf_sim.o main.o
	@# don't forget to link ELF library using "-l elf"
	$(CXX) -g -o $@ $^ -l elf
	@echo "---------------------------------"
	@echo "$@ is built SUCCESSFULLY"

main.o: main.cpp perf_sim.h
	$(CXX) -g -c $< $(INCL)

perf_sim.o: perf_sim.cpp perf_sim.h types.h func_instr.h func_memory.h rf.h ports.h
	$(CXX) -g -c $< $(INCL)

func_instr.o: func_instr.cpp func_instr.h types.h
	$(CXX) -g -c $< $(INCL)

func_memory.o: func_memory.cpp func_memory.h types.h
	$(CXX) -g -c $< $(INCL)

elf_parser.o: elf_parser.cpp elf_parser.h types.h
	$(CXX) -g -c $< $(INCL)

log.o: log.cpp log.h
	$(CXX) -g -c $< $(INCL)
clean:
	@-rm *.o
	@-rm perf_sim