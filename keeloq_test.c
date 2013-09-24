#include <stdio.h>

#include "keeloq.h"

int main (int argc, char **argv)
{
  uint64_t key = 0x00000000deadbeef;
  uint32_t in = 0x12345678;

  printf ("key=%08x%08x\n", key>>32, key&0xffffffff);
  printf ("plaintext = 0x%08x\n", in);
  keeloq_enc (&in, key, 528);
  printf ("ciphertext = 0x%08x\n", in);
  keeloq_dec (&in, key, 528);
  printf ("plaintext = 0x%08x\n", in);
}
