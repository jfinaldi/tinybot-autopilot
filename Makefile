ROOTNAME=mainDriver
HW=
FOPTION=
RUNOPTIONS=/home/pi/Desktop/Tinybot/lidardata
CC=gcc
CFLAGS= -g -I./include/
LIBA=wiringPi
LIBB=m
LIBC=pthread
DEPS=
SRCDIR=./src/
ADDOBJ= PCA9685.o DEV_Config.o Motor.o Lidar.o VehicleControl.o Speedometer.o Sensors.o Globals.o
OBJ = $(addprefix $(SRCDIR), $(ROOTNAME)$(HW)$(FOPTION).o $(ADDOBJ))

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

$(ROOTNAME)$(HW)$(FOPTION): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -l $(LIBA) -l $(LIBB) -l $(LIBC)

clean:
	rm $(OBJ)
	rm mainDriver 

run: $(ROOTNAME)$(HW)$(FOPTION)
	./$(ROOTNAME)$(HW)$(FOPTION) $(RUNOPTIONS) 