/* Copyright (c) 2012, Movidius Ltd.
   Released under the BSD 3-clause license.
   Author: Purdea Andrei <andrei -at- purdea.ro>
                         <andrei.purdea -at- movidius.com> */

#ifndef __MPOTOOLS_REFERENCED_IMAGES_H
#define __MPOTOOLS_REFERENCED_IMAGES_H
#include <stdlib.h>
#define MAX_IMAGES 100

typedef struct referenced_images_s {
    int count;
    size_t arr[MAX_IMAGES]; // contains absolute positions!!!
} referenced_images_t;

void add_referenced_image(referenced_images_t *images, size_t offset);
void sort_referenced_images(referenced_images_t *images);
void sort_referenced_images_skip(referenced_images_t *image, int skip);

#endif
