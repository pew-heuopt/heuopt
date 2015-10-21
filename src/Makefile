

LIBDIR =  lib
LIBSTAT = $(LIBDIR)/libkpmp.a 

ASSIGNMENT1_1= assignment1_1
ASSIGNMENT1_2= assignment1_2

ASSIGNMENTS= $(ASSIGNMENT1_1) $(ASSIGNMENT1_2)

INC = $(LIBDIR)


CXXFLAGS= -Wall -std=c++11
CXX = g++

release: CXX += -O3
debug: CXX += -g

all: runtests $(ASSIGNMENTS)

release: all
debug: all	

$(LIBSTAT): $(LIBDIR)/kpmp_instance.o  $(LIBDIR)/kpmp_solution.o  $(LIBDIR)/kpmp_solution_writer.o  $(LIBDIR)/assignment_helpers.o  $(LIBDIR)/spine.o 
	ar rvs $(LIBSTAT) lib/*.o

$(LIBDIR)/%.o : $(LIBDIR)/%.cpp $(LIBDIR)/%.h
	$(CXX) -c $(CXXFLAGS) $< -o $@


unittests: $(LIBSTAT) unittests.cpp
	$(CXX) -o unittests -I $(INC) $(CXXFLAGS) unittests.cpp $(LIBSTAT) -lboost_unit_test_framework

runtests: unittests
	./unittests


$(ASSIGNMENT1_1): $(LIBSTAT) $(ASSIGNMENT1_1).cpp
	$(CXX) -o $(ASSIGNMENT1_1) -I $(INC) $(CXXFLAGS) $(ASSIGNMENT1_1).cpp $(LIBSTAT) -lboost_program_options

$(ASSIGNMENT1_2): $(LIBSTAT) $(ASSIGNMENT1_2).cpp
	$(CXX) -o $(ASSIGNMENT1_2) -I $(INC) $(CXXFLAGS) $(ASSIGNMENT1_2).cpp $(LIBSTAT) -lboost_program_options



clean: 
	rm -f $(LIBSTAT) $(ASSIGNMENTS) unittests $(LIBDIR)/*.o
