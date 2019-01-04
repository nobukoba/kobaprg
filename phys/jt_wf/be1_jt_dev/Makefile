# Makefile for vpcalc
#---user defined variables --------------------------------------
#
# Modified to compile by g77. koba 2013/04/10 02:10:17
#
FC = gfortran
FFLAGS = -std=legacy
#
TARGET1 = be1
OBJECT1 = be1.o
#
TARGET2 = coulex
OBJECT2 = coulex.o
#
all:	$(TARGET1) $(TARGET2)
#
$(TARGET1): $(OBJECT1)
	$(FC) $(FFLAGS) -o $(TARGET1) $(OBJECT1)
#
$(TARGET2): $(OBJECT2)
	$(FC) $(FFLAGS) -o $(TARGET2) $(OBJECT2)
#
clean:
	$(RM) $(TARGET) *.o *~
#
