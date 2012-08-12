#include <stdio.h>
#include "referenced_images.h"
void add_referenced_image(referenced_images_t *images, size_t offset) {
    if (images==NULL) return;
    if (images->count >= MAX_IMAGES) { printf("referenced_images: error, too many images :)\n"); return; }
    images->arr[images->count] = offset;
    images->count ++;
}
int compare_size_t(const void *a, const void *b) {
    return *((size_t *)a) - *((size_t *)b);
}

void sort_referenced_images(referenced_images_t *images) {
    qsort(images->arr, images->count, sizeof(size_t), compare_size_t);
}

void sort_referenced_images_skip(referenced_images_t *images, int skip) {
    qsort(images->arr + skip, images->count - skip, sizeof(size_t), compare_size_t);
}
