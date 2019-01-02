CXX	:= g++
DEBUG	:= -g -pg
OPT	:= -O2 #comment out for debuging
OPTIONS	:= -std=c++11 $(OPT) #$(DEBUG)

OBJ	:= main.o Slicer.o Slicing.o GCode.o STL.o \
	Configurations.o BufferReadBinary.o BufferWrite.o \
	Vec2d.o Vec3d.o Triangle.o Cross.o Loop.o Layer.o \
	DoubleToString.o

all: xbslicer

xbslicer: $(OBJ)
	$(CXX) $(OPTIONS) $(OBJ) -o xbslicer -pthread

.cc.o:
	$(CXX) $(OPTIONS) -c $<

main.o: Slicer.o
Slicer.o: STL.o Slicing.o BufferWrite.o Slicer.hh
STL.o: Configurations.o BufferReadBinary.o Triangle.o STL.hh
Slicing.o: STL.o GCode.o Slicing.hh
GCode.o: Configurations.o Layer.o GCode.hh
Configurations.o: Configurations.hh
BufferReadBinary.o: BufferReadBinary.hh
BufferWrite.o: DoubleToString.o BufferWrite.hh
Vec2d.o: Vec3d.o Vec2d.hh
Vec3d.o: Configurations.o BufferReadBinary.o Vec3d.hh
Triangle.o: Vec3d.o Triangle.hh
Cross.o: Vec2d.o Cross.hh
Loop.o: Loop.hh
Layer.o: Loop.o Cross.o BufferWrite.o Layer.hh
DoubleToString.o: DoubleToString.hh

clean:
	rm *.o xbslicer *~
