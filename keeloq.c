/* 
 * Slow keeloq implementation
 *
 * Elliot Buller 2013
 */
#include <stdint.h>

// Is bit y set in x
#define BS(x,y)   (x & (1 << y))

/* Non-linear feedback functions */
const uint8_t NLF[4] = {0x2e, 0x74, 0x5c, 0x3a};

static inline uint8_t nlf_decrypt (uint32_t in)
{
  uint8_t sel = 0, shift = 0;

  // Select uses bits 30 and 25
  if (BS(in, 30))
    sel |= 2;  
  if (BS(in, 25))
    sel |= 1;

  // Shift uses bits 19, 8, 0
  if (BS(in, 19))
    shift |= 4;
  if (BS(in, 8))
    shift |= 2;
  if (BS(in, 0))
    shift |= 1;

  return ((NLF[sel] >> shift) & 1);
}

static inline uint8_t nlf_encrypt (uint32_t in)
{
  uint8_t sel = 0, shift = 0;

  // Select uses bits 31, 26
  if (BS(in, 31))
    sel |= 2;  
  if (BS(in, 26))
    sel |= 1;

  // Shift uses bits 20, 9, 1
  if (BS(in, 20))
    shift |= 4;
  if (BS(in, 9))
    shift |= 2;
  if (BS(in, 1))
    shift |= 1;

  return ((NLF[sel] >> shift) & 1);
}

static inline void shift_key_right (uint64_t *key, uint8_t shift)
{
  uint64_t tmp;

  tmp = *key;
  (*key) >>= shift;
  *key |= (tmp << (64 - shift));
}

static inline void shift_key_left (uint64_t *key, uint8_t shift)
{
  uint64_t tmp;

  tmp = *key;
  (*key) <<= shift;
  *key |= (tmp >> (64 - shift));
}

static inline void encrypt_round (uint32_t *in, uint64_t *key)
{
  uint8_t bit;

  // calculate the bit shifted in
  bit = ((*in & 1) ^ ((*in >> 16) & 1) ^ nlf_encrypt(*in) ^ (*key & 1));

  // Shift input reg
  (*in) >>= 1;
  if (bit)
    *in |= (1 << 31);

  // Shift key
  shift_key_right (key, 1);
}

static inline void decrypt_round (uint32_t *in, uint64_t *key)
{
  uint8_t bit;

  // calculate the bit shifted in
  bit = ((*in >> 31) & 1) ^ ((*in >> 15) & 1) ^ nlf_decrypt(*in) ^ ((*key >> 15) & 1);

  // Shift input reg
  (*in) <<= 1;
  if (bit)
    *in |= 1;

  // Shift key
  shift_key_left (key, 1);
}

void keeloq_enc (uint32_t *in, uint64_t key, uint16_t rounds)
{
  int i;

  for (i = 0; i < rounds; i++)
    encrypt_round (in, &key);
}

void keeloq_dec (uint32_t *in, uint64_t key, uint16_t rounds)
{
  int i;

  for (i = 0; i < rounds; i++)
    decrypt_round (in, &key);
}

