int leading_ones(unsigned char x)
{
  x = ~x;
  int count = 0;
  if (x & 0xf0) x >>= 4; else count += 4;
  if (x & 0x0c) x >>= 2; else count += 2;
  if (!(x & 0x02)) count++;
  return count;
}

int next_codepoint(char* bytes)
{
  while ((*bytes & 0xc0) == 0x80) bytes++;    /* move to next sequence */

  if (!(*bytes & 0x80)) return *bytes & 0x7f; /* ascii codepoint */

  int len = leading_ones(*bytes);
  int codepoint = *bytes & ((1<< (8-len)) -1);

  while ((*bytes & 0xc0) == 0x80) {
    codepoint = (codepoint << 6) + (*bytes & 0x3f);
    bytes++;
  }

  return codepoint;
}
