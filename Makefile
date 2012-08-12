common_objs := common.o image2xmlmem.o xmlmem2xmlfile.o markers.o tags.o data_placement.o referenced_images.o mp_entry.o writers.o xmlmem2image.o

image2xml_objs := $(common_objs) image2xml.o
xml2image_objs := $(common_objs) xml2image.o xmlfile2xmlmem.o
mpo2jps_objs := $(common_objs) mpo2jps.o xmlmem_data_source_manager.o

all_objs := $(common_objs) mpo2jps.o xmlmem_data_source_manager.o xml2image.o xmlfile2xmlmem.oimage2xml.o

CFLAGS:=-Iexternal/mxml/ -Iexternal/jpeg-8d/ -Lexternal/mxml/ -Lexternal/jpeg-8d/.libs/
LIBS:=-lmxml -lpthread
CC:=gcc

all: image2xml xml2image mpo2jps
#$ xml2image mpo2jps

.PHONY: all

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

image2xml: $(image2xml_objs)
	$(CC) $(CFLAGS) -o $@ $(image2xml_objs) $(LIBS)

xml2image: $(xml2image_objs)
	$(CC) $(CFLAGS) -o $@ $(xml2image_objs) $(LIBS)

mpo2jps: $(mpo2jps_objs)
	$(CC) $(CFLAGS) -o $@ $(mpo2jps_objs) $(LIBS) -ljpeg

%.d: %.c
	$(CC) -MM -MG -MP $(CFLAGS) $*.c > $@

-include $(all_objs:.o=.d)

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf *.d
	rm -rf image2xml xml2image mpo2jps

.PHONY: cleanall
cleanall: clean
	cd external/mxml; make clean; cd ../../
	cd external/jpeg-8d; make clean; cd ../../

