/* Copyright (c) 2012, Movidius Ltd.
   Released under the BSD 3-clause license.
   Author: Purdea Andrei <andrei -at- purdea.ro>
                         <andrei.purdea -at- movidius.com> */

#include <stdio.h>
#include <string.h>
#include "markers.h"


/* 'get_marker_name()' - returns a string representing the name of the marker.
sometimes the returned string doesn't always stay constant. */
char *get_marker_name(unsigned char marker) {
    switch(marker) {
        
        case MARKER_JPG: return "JPG";
        case MARKER_DHT: return "DHT";
        case MARKER_DAC: return "DAC";
        
        
        case MARKER_SOI: return "SOI";
        case MARKER_EOI: return "EOI";
        case MARKER_SOS: return "SOS";
        case MARKER_DQT: return "DQT";
        case MARKER_DNL: return "DNL";
        case MARKER_DRI: return "DRI";
        case MARKER_DHP: return "DHP";
        case MARKER_EXP: return "EXP";
        
        case MARKER_COM: return "COM";
        case MARKER_TEM: return "TEM";
        default:
            if (MARKER_IS_SOF(marker)) {
                static char name[]="SOF\0\0";
                int nr = marker & MARKER_SOF_NR_MASK;
                if (nr<10) { name[3] = '0' + nr; name[4] = '\0'; }
                else { name[3] = '1'; name[4] = '0' + nr - 10; }
                return name;
            } else if (MARKER_IS_APP(marker)) {
                static char name[]="APP\0\0";
                int nr = marker & MARKER_APP_NR_MASK;
                if (nr<10) { name[3] = '0' + nr; name[4] = '\0'; }
                else { name[3] = '1'; name[4] = '0' + nr - 10; }
                return name;
            } else if (MARKER_IS_RST(marker)) {
                static char name[]="RST\0";
                int nr = marker & MARKER_RST_NR_MASK;
                name[3] = '0' + nr;
                return name;
            } else if (MARKER_IS_JPG(marker)) {
                static char name[]="JPG\0\0";
                int nr = marker & MARKER_JPG_NR_MASK;
                if (nr<10) { name[3] = '0' + nr; name[4] = '\0'; }
                else {name[3] = '1'; name[4] = '0' + nr - 10; }
                return name;
            } else {
                static char name[] = "RES_\0\0";
                snprintf(name + 4, 3, "%02x", marker);
                return name;
            }
    }
}

/* 'get_marker_number()' - returns the number value of a marker name given as a string.
 Or it returns -1 when not given a marker name. */
int get_marker_number(char *name) {
    int id;
    switch (name[0]) {
        case 'A':
            if ((strlen(name)==4 || strlen(name)==5) && sscanf(name, "APP%d", &id)==1) return id + MARKER_APP_BASE;
            break;
        case 'C':
            if (0==strcmp(name, "COM")) return MARKER_COM;
            break;
        case 'D':
            if (0==strcmp(name, "DHT")) return MARKER_DHT;
            if (0==strcmp(name, "DAC")) return MARKER_DAC;
            if (0==strcmp(name, "DQT")) return MARKER_DQT;
            if (0==strcmp(name, "DNL")) return MARKER_DNL;
            if (0==strcmp(name, "DRI")) return MARKER_DRI;
            if (0==strcmp(name, "DHP")) return MARKER_DHP;
            break;
        case 'E':
            if (0==strcmp(name, "EOI")) return MARKER_EOI;
            if (0==strcmp(name, "EXP")) return MARKER_EXP;
            break;
        case 'J':
            if (0==strcmp(name, "JPG")) return MARKER_JPG;
            if ((strlen(name)==4 || strlen(name)==5) && sscanf(name, "JPG%d", &id)==1) return id + MARKER_JPG_BASE;
            break;
        case 'R':
            if (strlen(name)==6 && sscanf(name, "RES_%x", &id)==1) return id;
            if (strlen(name)==4 && sscanf(name, "RST%d", &id)==1) return id + MARKER_RST_BASE;
            break;
        case 'S':
            if (0==strcmp(name, "SOI")) return MARKER_SOI;
            if (0==strcmp(name, "SOS")) return MARKER_SOS;
            if ((strlen(name)==4 || strlen(name)==5) && sscanf(name, "SOF%d", &id)==1) return id + MARKER_SOF_BASE;
            break;
        case 'T':
            if (0==strcmp(name, "TEM")) return MARKER_TEM;
            break;
        default:
            return -1;
            break;
    }
    return -1;
}

