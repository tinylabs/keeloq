/*
 * slow keeloq implementation header file
 *
 * Elliot Buller 2013
 */
#ifndef _KEELOQ_H_
#define _KEELOQ_H_

#include <stdint.h>

void keeloq_enc (uint32_t *in, uint64_t key, uint16_t rounds);
void keeloq_dec (uint32_t *in, uint64_t key, uint16_t rounds);

#endif /* _KEELOQ_H_ */
