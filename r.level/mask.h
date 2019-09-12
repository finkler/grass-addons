#ifndef _MASK_H
#define _MASK_H

typedef char *mask_t;

mask_t *Mask_allocate(void);
unsigned long Mask_area(mask_t *);
void Mask_clear(mask_t *);
void Mask_copy(mask_t *, mask_t *);
void Mask_fill(mask_t *);
void Mask_filter(mask_t *, int);
void Mask_free(mask_t *);
int Mask_isset(mask_t *, int, int);
void Mask_set(mask_t *, int, float);

#endif /* _MASK_H */
