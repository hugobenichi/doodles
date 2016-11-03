#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#define DBG 1

#define BOOM do { printf("boom\n"); exit(0); } while(0)

static char buf[256];

static void _fatal(const char* msg, int ln) {
  printf("%s:%d %s\n", __FILE__, ln, msg);
  exit(-1);
}
#define fatal(msg) _fatal((msg), __LINE__)

static void* _must_malloc(size_t nbytes, int ln) {
  void* p = malloc(nbytes);
  if (!p) {
    _fatal("malloc failed", ln);
  }
  return p;
}
#define must_malloc(nbytes) _must_malloc((nbytes), __LINE__)

typedef uint8_t instr;

static const int instr_multibyte_mask = 3 << 6;
static const int instr_codepoint_mask = 0x3f;

#define multi(i, nadd) (((nadd & 3) << 6) | (i & 0x3f));

// TODO: consider returniung length of instr instead of number of extra bytes
static int instr_nbyte(instr i) {
  return (i & instr_multibyte_mask) >> 6;
}

static instr instr_codepoint(instr i) {
  return (i & instr_codepoint_mask);
}

const int instr_max = 26;    // next instr id
static char* instr_names[instr_max];

const instr i_noop    =   0; // nothing
const instr i_push_32 =   1; // push next 4B as a LE int32 (next byte not 4B aligned !)
const instr i_push_u8 =   multi(2, 1); // push next 1B as uint8
const instr i_push_s8 =   multi(3, 1); // push next 1B as int8 with sign expansion
const instr i_32add   =   4;
const instr i_32mul   =   5;
const instr i_32neg   =   6; // flip int sign
const instr i_32inc   =   7;
const instr i_32dec   =   8;
const instr i_not     =   9; // 0 -> 1, any other -> 0
const instr i_eq      =  10 ;
const instr i_leq     =  11;
const instr i_geq     =  12;
const instr i_dup     =  13;
const instr i_dupbis  =  14;
const instr i_swap    =  15;
const instr i_goto    =  multi(16, 1); // TODO: change to forward goto with relative offset
const instr i_jump_if =  multi(17, 1); // TODO: change to forward goto with relative offset
const instr i_skip_if =  multi(18, 1);
const instr i_do_if   =  multi(19, 1);
const instr i_call    =  multi(20, 1); // pop top of stack as programm addr and start subroutine there.
                                       // Folowwing byte indicate number of args to take from stack for fp.
const instr i_ret     =  multi(21, 1); // return to caller. Following byte indicate number of words to return.
const instr i_load    =  multi(22, 1);
const instr i_store   =  multi(23, 1);
const instr i_recur   =  24;
const instr i_panic   =  25;

static char* instr_names[] = {
  "i_noop",
  "i_push_32",
  "i_push_u8",
  "i_push_s8",
  "i_32add",
  "i_32mul",
  "i_32neg",
  "i_32inc",
  "i_32dec",
  "i_not",
  "i_eq",
  "i_leq",
  "i_geq",
  "i_dup",
  "i_dupbis",
  "i_swap",
  "i_goto",
  "i_jump_if",
  "i_skip_if",
  "i_do_if",
  "i_call",
  "i_ret",
  "i_load",
  "i_store",
  "i_recur",
  "i_panic",
};
static int instr_names_len = sizeof(instr_names);
static char* instr_unknown = "unknown";

struct instr {
  instr i;
  uint8_t has_data;
  uint32_t data;
};

int  decode_instr(struct instr* d, instr* i_addr) {
  d -> i = instr_codepoint(*i_addr);
  int nbytes = instr_nbyte(*i_addr);
  if (nbytes > 0) {
    d -> data = *(i_addr + 1);
    d -> has_data = 1;
  }
  return nbytes + 1;
}

int decode_program(struct instr* out, size_t outlen, instr* in, size_t inlen) {
  struct instr* outstart = out;
  while (inlen && outlen) {
    int n = decode_instr(out, in);
    in += n;
    inlen -= n;
    out++;
    outlen--;
  }
  if (inlen) {
    return -inlen;
  }
  return out - outstart;
}

char* instr_name(instr i) {
  i = instr_codepoint(i);
  if (0 <= i && i < instr_names_len) {
    return instr_names[i];
  }
  return instr_unknown;
}

void instr_print(char* buf, size_t n, struct instr* i_addr) {
  instr i = i_addr -> i;
  uint8_t d = i_addr -> data;
  int w = snprintf(buf, n, "%.2i:%s", i, instr_name(i));
  if (!i_addr -> has_data) {
    return;
  }
  // TODO: safer string concatenation please, do not ignore w error values.
  buf += w;
  buf += snprintf(buf, n - w, ", 0x%.2x (%i)", d, d);
}

void program_fprint(FILE* f, struct instr* program, size_t len) {
  char b[64];
  int i = 0;
  while (i < len) {
    instr_print(b, sizeof(b), program++);
    fprintf(f, "0x%.2x: %s\n", i++, b);
  }
}

// TODO: replace with print_instr(struct instr i)
int instr_disassembly(char* buf, size_t n, instr* i_addr) {
  instr i = *i_addr;
  int nbytes = instr_nbyte(i);
  if (nbytes < 0 || 3 < nbytes) {
    return -1;
  }
  // TODO: safer string concatenation please, do not ignore w error values.
  int w = snprintf(buf, n, "%.2i:%s", instr_codepoint(i), instr_name(i));
  buf += w;
  n -= w;
  int r = nbytes + 1;
  while (nbytes--) {
    i = *++i_addr;
    buf += snprintf(buf, n, ", 0x%.2x (%i)", i, i);
  }
  return r;
}

// TODO: replace with decode_program
void disassembly(FILE* f, instr* program, size_t len) {
  instr* end = program + len;
  char b[64];
  int i = 0;
  while (program < end) {
    int nbytes = instr_disassembly(b, sizeof(b), program);
    fprintf(f, "0x%.2x: %s\n", i, b);
    i += nbytes;
    if (nbytes < 0) {
      snprintf(buf, sizeof(buf), "unexpected number of bytes %d", nbytes);
      fatal(buf);
    }
    program += nbytes;
  }
}

/* other intructions to consider:
 * ------------------------------
 *  BE/LE conversion
 *    reverse bytes as short/int/long
 *  push usual constants: 0,1,2,-1
 *  u64 arithmetic
 *  float/double arithmetic
 *  int32 and uint32 div and mod
 *  bitsize and shift operations
 *  truncation and widening between integer types
 */

struct fn {
  instr* code;
  size_t len;
};

struct program {
  struct fn* fns; // index implicitly identifies functions
  size_t len;
  int main;             // index of main function
};

struct datastack {
  uint32_t* bottom;
  uint32_t* top;
  uint32_t* end;
};
struct datastack datastack_empty;

void datastack_new(struct datastack * s,
                   size_t depth
                  ) {
  void * mem = must_malloc(depth * sizeof(uint32_t));
  s -> bottom = mem;
  s -> top    = mem;
  s -> end    = mem + depth;
}

void datastack_del(struct datastack *s) {
  free(s -> bottom);
  *s = datastack_empty;
}

int datastack_ensure_capacity(struct datastack* d, int n) {
  return ((n < 0) && (d -> top - d -> bottom) >= n)
    ||  ((n > 0) && (d -> end - d -> top) >= n);
  return 0;
}

#define datastack_overflow(c, n)  if (!datastack_ensure_capacity((c) -> data, n) \
                                    ctx_dump_fatal(stderr, (c), "data stack overflow")
#define datastack_underflow(c, n) if (!datastack_ensure_capacity((c) -> data, n) \
                                    ctx_dump_fatal(stderr, (c), "data stack underflow")


struct call {
  instr*    start;
  instr*    ip;
  uint32_t* fp;
  int       n_args;
};

struct callstack {
  struct call* bottom;
  struct call* top;
  struct call* end;
};
struct callstack callstack_empty;

void callstack_new(struct callstack * s,
                   size_t depth
                  ) {
  void * mem = must_malloc(depth * sizeof(struct call));
  s -> bottom = mem;
  s -> top    = mem;
  s -> end    = mem + depth;
}

void callstack_del(struct callstack *s) {
  free(s -> bottom);
  *s = callstack_empty;
}

int call_args(char* buf, size_t n, struct call* call) {
  *buf++ = '(';
  int nargs = (int) call -> n_args;
  uint32_t* w = call -> fp;
  if (!nargs) {
      buf += 2;
  }
  while (nargs--) {
    buf += snprintf(buf, 14, "%u, ", *w++);
  }
  *(buf - 2) = ')';
  *(buf - 1) = 0;
  return 0;
}



struct ctx {
  struct datastack data;
  struct callstack call;
  struct call* current;
  instr* ip_end;
};

void ctx_new(struct ctx *c,
             size_t depth
             ) {
  datastack_new(&c -> data, depth);
  callstack_new(&c -> call, depth);
}

void ctx_del(struct ctx *c) {
  datastack_del(&c -> data);
  callstack_del(&c -> call);
}

void ctx_reset(struct ctx *c) {
  c -> current = 0;
  c -> data.top = c -> data.bottom;
  c -> call.top = c -> call.bottom;
}

void ctx_datastack_print(struct ctx *c, FILE* f, const char* indent) {
  int i = 0;
  uint32_t* s = c -> data.bottom;
  while (s < c -> data.top) {
    fprintf(f, "%s%.2i: 0x%.8x %i\n", indent, i++, *s, *s);
    s++;
  }
}

void ctx_callstack_print(struct ctx* c, FILE* f, const char* indent) {
  // TODO: when calling function with argument area overlapping caller argument area,
  //       the caller arguments are lost when dumping the stack.
  char b[64];
  int n = (c -> current) - (c -> call.bottom);
  instr* base = c -> call.bottom -> start;
  struct call* call = c -> current;
  while (n >= 0) {
    instr* s = call -> start;
    instr* i = call -> ip;
    if (n > 0) {
      s++; // match +1 at end of exec's switch for non-first call. TODO: clean this hack
    }
    if (call < c -> current) {
      i--; // similar hack to the one above
    }
    // BUG: why is it not printing multibyte instruction correctly ?
    instr_disassembly(b, sizeof(b), i);
    fprintf(f, "%s%.2i: at +%.2ld: %s\n", indent, n, (long)(i - base), b);
    instr_disassembly(b, sizeof(b), s);
    fprintf(f, "%s    in +%.2ld: %s", indent, (long)(s - base), b);
    call_args(b, sizeof(b), call);
    fprintf(f, " with %s\n", b);
    call--;
    n--;
  }
}

void ctx_dump(struct ctx *c, FILE* f) {
  fprintf(f, "\nfull context dump\n");
  fprintf(f,   "-----------------\n");
  fprintf(f, "\ndata stack dump\n");
  fprintf(f,   "---------------\n");
  ctx_datastack_print(c, f, "| ");
  fprintf(f, "\ncall stack dump\n");
  fprintf(f,   "---------------\n");
  ctx_callstack_print(c, f, "| ");
  fprintf(f, "\n");
}

void _ctx_dump_fatal(FILE* f, struct ctx *c, const char *msg, int ln) {
  ctx_dump(c, f);
  _fatal(msg, ln);
}
#define ctx_dump_fatal(f, c, msg) _ctx_dump_fatal(f, c, msg, __LINE__);

void ctx_ip_next(struct ctx *c) {
  c -> current -> ip++;
  if (c -> current -> ip >= c -> ip_end) {
    ctx_dump_fatal(stderr, c, "program pointer overflow");
  }
}

instr ctx_ip_get(struct ctx *c) {
  return *(c -> current -> ip);
}

void ctx_ip_set(struct ctx *c, instr* ip) {
  if (ip >= c -> ip_end) {
    snprintf(buf, sizeof(buf), "invalid program address %ld", ip - c -> current -> ip);
    ctx_dump_fatal(stderr, c, buf);
  }
  c -> current -> ip = ip;
}

void ctx_push(struct ctx *c, int32_t x) {
  if (c-> data.top == c -> data.end) {
    ctx_dump_fatal(stderr, c, "data stack overflow");
  }
  *(c -> data.top)++ = x;
}

int32_t ctx_pop(struct ctx *c) {
  if (c-> data.top == c -> data.bottom) {
    ctx_dump_fatal(stderr, c, "data stack underflow");
  }
  return *--(c -> data.top);
}

// Jumps to function pointed by 'callee' which takes 'n_args' input args.
// It does so by:
//  - pushing a new activation record on the call stack.
//  - pointing current activation record to new top of stack.
void ctx_call(struct ctx *c, instr* callee, int n_args) {
  if (c-> call.top == c -> call.end) {
    ctx_dump_fatal(stderr, c, "call stack overflow");
  }
  *(c -> call.top) = (struct call){
    .start  = callee,
    .ip     = callee,
    .fp     = c -> data.top - n_args,
    .n_args = n_args,
  };
  c -> current = (c -> call.top)++;
}

// Returns from current function and returns 'output_n' args to the caller.
// It does so by:
//  - restoring the data stack top, taking into account the number of output args.
//  - popping the top activation record of the call stack and making it current
void ctx_ret(struct ctx *c, int output_n) {
  (c -> call.top)--;
  (c -> current)--;
  //c -> current -> fp += output_n;
  if (c-> call.top == c -> call.bottom) {
    ctx_dump_fatal(stderr, c, "call stack underflow");
  }
}

void exec(struct ctx *c, instr* program, size_t len) {
  int32_t r0, r1;

  struct instr dec[128] = {};
  int declen = decode_program(dec, 128, program, len);
  if (declen < 0) {
    snprintf(buf, sizeof(buf), "program too big, could not decode %d bytes", -declen);
    fatal(buf);
  }

  printf("declen = %d\n" ,declen);
  program_fprint(stdout, dec, declen);
  BOOM;

  ctx_call(c, program, 0);
  c -> ip_end     = program + len;
  while (c -> current -> ip < c -> ip_end) {

    if (DBG) {
      ctx_datastack_print(c, stdout, "  :");
      instr_disassembly(buf, sizeof(buf), c -> current -> ip);
      printf("%s\n", buf);
    }

    switch(ctx_ip_get(c)) {
      case i_push_32:
        break;
      case i_push_u8: // TODO: fix broken widening
        ctx_ip_next(c);
        ctx_push(c, (ctx_ip_get(c)) & 0xff);
        break;
      case i_push_s8:
        ctx_ip_next(c);
        ctx_push(c, ctx_ip_get(c));
        break;
      case i_32add:
        // TODO: check datastack len > 1
        r0 = ctx_pop(c);
        (*(c -> data.top - 1)) += r0;
        break;
      case i_32mul:
        r0 = ctx_pop(c);
        r1 = ctx_pop(c);
        ctx_push(c, r0 * r1);
        break;
      case i_32neg:
        r0 = ctx_pop(c);
        ctx_push(c, -r0);
        break;
      case i_not:
        r0 = ctx_pop(c);
        r1 = r0 ? 0 : 1;
        ctx_push(c, r1);
        break;
      case i_eq:
        r0 = ctx_pop(c);
        r1 = ctx_pop(c);
        ctx_push(c, r1);
        ctx_push(c, r0);
        ctx_push(c, r0 == r1);
        break;
      case i_leq:
        r0 = ctx_pop(c);
        r1 = ctx_pop(c);
        ctx_push(c, r1);
        ctx_push(c, r0);
        ctx_push(c, r0 <= r1);
        break;
      case i_geq:
        r0 = ctx_pop(c);
        r1 = ctx_pop(c);
        ctx_push(c, r1);
        ctx_push(c, r0);
        ctx_push(c, r0 >= r1);
        break;
      case i_32inc:
        // TODO: check datastack len > 0
        ctx_push(c, ctx_pop(c) + 1);
        (*(c -> data.top - 1))++;
        break;
      case i_32dec:
        // TODO: check datastack len > 0
        (*(c -> data.top - 1))--;
        break;
      case i_dup:
        // TODO: check datastack len > 0
        ctx_push(c, *(c -> data.top - 1));
        break;
      case i_dupbis:
        // TODO: check datastack len > 1
        ctx_push(c, *(c -> data.top - 2));
        break;
      case i_swap:
        // TODO: check datastack len > 1
        r0 = ctx_pop(c);
        r1 = ctx_pop(c);
        ctx_push(c, r0);
        ctx_push(c, r1);
        break;
      case i_goto:
        ctx_ip_next(c);
        ctx_ip_set(c, program + ctx_ip_get(c) - 1);
        break;
      case i_jump_if:
        ctx_ip_next(c);
        if (ctx_pop(c)) {
          ctx_ip_set(c, program + ctx_ip_get(c) - 1);
        }
        break;
      case i_skip_if:
        if (ctx_pop(c)) {
          ctx_ip_next(c);
        }
        break;
      case i_do_if:
        if (!ctx_pop(c)) {
          ctx_ip_next(c);
        }
        break;
      case i_load:
        ctx_ip_next(c);
        r0 = ctx_ip_get(c);
        // TODO: check 0 <= r0 < n_args
        ctx_push(c, *(c -> current -> fp + r0));
        break;
      case i_store:
        ctx_ip_next(c);
        r0 = ctx_ip_get(c);
        // TODO: check 0 <= r0 < n_args
        *(c -> current -> fp + r0) = ctx_pop(c);
        break;
      case i_recur:
        c -> current -> ip = c -> current -> start;
        // TODO: reset stack pointer
        break;
      case i_call:
        ctx_ip_next(c);
        r0 = ctx_pop(c);
        ctx_call(c, program + r0 - 1, ctx_ip_get(c));
        break;
      case i_ret:
        ctx_ip_next(c);
        ctx_ret(c, ctx_ip_get(c));
        break;
      case i_panic:
        ctx_dump_fatal(stderr, c, "panic");
        break;
      case i_noop:
        break;
      default:
        break;
    }
    (c -> current -> ip)++; // do not use ctx_ip_next() to avoid fatal-ing at program end
  }
}

void exec2(struct ctx *c, instr* program, size_t len) {
  int32_t r0, r1;

  static char* instr_labels[] = {
    &&do_i_noop,
    &&do_i_push_32,
    &&do_i_push_u8,
    &&do_i_push_s8,
    &&do_i_32add,
    &&do_i_32mul,
    &&do_i_32neg,
    &&do_i_32inc,
    &&do_i_32dec,
    &&do_i_not,
    &&do_i_eq,
    &&do_i_leq,
    &&do_i_geq,
    &&do_i_dup,
    &&do_i_dupbis,
    &&do_i_swap,
    &&do_i_goto,
    &&do_i_jump_if,
    &&do_i_skip_if,
    &&do_i_do_if,
    &&do_i_call,
    &&do_i_ret,
    &&do_i_load,
    &&do_i_store,
    &&do_i_recur,
    &&do_i_panic,
  };

  ctx_call(c, program, 0);
  c -> ip_end     = program + len;

  dispatch:
    if (c -> current -> ip >= c -> ip_end) return;

    if (DBG) {
      ctx_datastack_print(c, stdout, "  :");
      instr_disassembly(buf, sizeof(buf), c -> current -> ip);
      printf("%s\n", buf);
    }

    instr i = instr_codepoint(ctx_ip_get(c));
    // TODO: check bounds
    void* label = instr_labels[i];
    goto *label;

  do_i_push_u8: // TODO: fix broken widening
    ctx_ip_next(c);
    ctx_push(c, (ctx_ip_get(c)) & 0xff);
    goto next;
  do_i_push_s8:
    ctx_ip_next(c);
    ctx_push(c, ctx_ip_get(c));
    goto next;
  do_i_32add:
    // TODO: check datastack len > 1
    r0 = ctx_pop(c);
    (*(c -> data.top - 1)) += r0;
    goto next;
  do_i_32mul:
    r0 = ctx_pop(c);
    r1 = ctx_pop(c);
    ctx_push(c, r0 * r1);
    goto next;
  do_i_32neg:
    r0 = ctx_pop(c);
    ctx_push(c, -r0);
    goto next;
  do_i_not:
    r0 = ctx_pop(c);
    r1 = r0 ? 0 : 1;
    ctx_push(c, r1);
    goto next;
  do_i_eq:
    r0 = ctx_pop(c);
    r1 = ctx_pop(c);
    ctx_push(c, r1);
    ctx_push(c, r0);
    ctx_push(c, r0 == r1);
    goto next;
  do_i_leq:
    r0 = ctx_pop(c);
    r1 = ctx_pop(c);
    ctx_push(c, r1);
    ctx_push(c, r0);
    ctx_push(c, r0 <= r1);
    goto next;
  do_i_geq:
    r0 = ctx_pop(c);
    r1 = ctx_pop(c);
    ctx_push(c, r1);
    ctx_push(c, r0);
    ctx_push(c, r0 >= r1);
    goto next;
  do_i_32inc:
    // TODO: check datastack len > 0
    ctx_push(c, ctx_pop(c) + 1);
    (*(c -> data.top - 1))++;
    goto next;
  do_i_32dec:
    // TODO: check datastack len > 0
    (*(c -> data.top - 1))--;
    goto next;
  do_i_dup:
    // TODO: check datastack len > 0
    ctx_push(c, *(c -> data.top - 1));
    goto next;
  do_i_dupbis:
    // TODO: check datastack len > 1
    ctx_push(c, *(c -> data.top - 2));
    goto next;
  do_i_swap:
    // TODO: check datastack len > 1
    r0 = ctx_pop(c);
    r1 = ctx_pop(c);
    ctx_push(c, r0);
    ctx_push(c, r1);
    goto next;
  do_i_goto:
    ctx_ip_next(c);
    ctx_ip_set(c, program + ctx_ip_get(c) - 1);
    goto next;
  do_i_jump_if:
    ctx_ip_next(c);
    if (ctx_pop(c)) {
      ctx_ip_set(c, program + ctx_ip_get(c) - 1);
    }
    goto next;
  do_i_skip_if:
    if (ctx_pop(c)) {
      ctx_ip_next(c);
    }
    goto next;
  do_i_do_if:
    if (!ctx_pop(c)) {
      ctx_ip_next(c);
    }
    goto next;
  do_i_load:
    ctx_ip_next(c);
    r0 = ctx_ip_get(c);
    // TODO: check 0 <= r0 < n_args
    ctx_push(c, *(c -> current -> fp + r0));
    goto next;
  do_i_store:
    ctx_ip_next(c);
    r0 = ctx_ip_get(c);
    // TODO: check 0 <= r0 < n_args
    *(c -> current -> fp + r0) = ctx_pop(c);
    goto next;
  do_i_recur:
    c -> current -> ip = c -> current -> start;
    // TODO: reset stack pointer
    goto next;
  do_i_call:
    ctx_ip_next(c);
    r0 = ctx_pop(c);
    ctx_call(c, program + r0 - 1, ctx_ip_get(c));
    goto next;
  do_i_ret:
    ctx_ip_next(c);
    ctx_ret(c, ctx_ip_get(c));
    goto next;
  do_i_panic:
    ctx_dump_fatal(stderr, c, "panic");
    goto next;
  do_i_push_32:
  do_i_noop:
  next:
    (c -> current -> ip)++; // do not use ctx_ip_next() to avoid fatal-ing at program end
    goto dispatch;
}

#include <time.h>

void run_program(instr* p, size_t len) {
  struct ctx c;
  struct timespec start, stop;

  ctx_new(&c, 256);
  int iter = 1;

  clock_gettime(CLOCK_REALTIME, &start);
  for (int i = 0; i < iter; i++) {
    exec(&c, p, len);
    if (DBG) ctx_datastack_print(&c, stdout, "");
    ctx_reset(&c);
  }
  clock_gettime(CLOCK_REALTIME, &stop);
  long elapsed = (stop.tv_sec - start.tv_sec) * 1000 * 1000 + (stop.tv_nsec - start.tv_nsec) / 1000;

  printf("duration: %ld\n", elapsed);

  ctx_del(&c);
}

void p1() {
  puts("p1: compute 2 * (4 + (3 * 5)) - 10 = 28");
  instr program[] = {
    i_push_u8, 4,
    i_push_u8, 3,
    i_push_u8, 5,
    i_32mul,
    i_32add,
    i_push_u8, 2,
    i_32mul,
    i_push_u8, 10,
    i_32neg,
    i_32add,
  };

  struct fn fn_p1 = {
    .code = program,
    .len = sizeof(program)
  };
  struct program p1 = {
    .fns = &fn_p1,
    .len = 1,
    .main = 0
  };
  fn_p1 = *p1.fns; // suppress warnings

  if (DBG) disassembly(stdout, program, sizeof(program));
  run_program(program, sizeof(program));
}

void p2() {
  puts("p2: count until 20");
  instr program[] = {
    i_push_u8, 20,
    i_push_u8, 0,
    i_geq,
    i_jump_if, 10,
    i_32inc,
    i_goto, 4,
    i_noop
  };
  if (DBG) disassembly(stdout, program, sizeof(program));
  puts("");
  run_program(program, sizeof(program));
}

void p3() {
  puts("p3: compute 4! using inner goto loop only");
  instr program[] = {
    // initial values
    i_push_u8, 1,
    i_push_u8, 5,
    // factorial begin
    i_dup,
    i_jump_if, 9,         // exit below if top is zero, otherwise jump +2
    i_goto, 16,           // exit
    i_swap,               // ..., acc, n] -> ..., n, acc]
    i_dupbis,             //              -> ..., n, acc, n]
    i_32mul,              //              -> ..., n, n x acc]
    i_swap,               //              -> ..., n x acc, n]
    i_32dec,              //              -> ..., n x acc, n - 1]
    i_goto, 4
  };
  if (DBG) disassembly(stdout, program, sizeof(program));
  puts("");
  run_program(program, sizeof(program));
}

void p4() {
  puts("p4: compute 4! using one layer of i_call/i_ret plus inner goto loop");
  instr program[] = {
    // goto main
    i_goto, 14,
    // factorial
    i_dup,
    i_jump_if, 7,         // exit below if top is zero, otherwise jump +2
    i_ret, 1,             // return
    i_swap,               // ..., acc, n] -> ..., n, acc]
    i_dupbis,             //              -> ..., n, acc, n]
    i_32mul,              //              -> ..., n, n x acc]
    i_swap,               //              -> ..., n x acc, n]
    i_32dec,              //              -> ..., n x acc, n - 1]
    i_goto, 2,
    // push initial values
    i_push_u8, 1,
    i_push_u8, 5,
    // call factorial
    i_push_u8, 2,         // factorial address
    i_call, 2
  };
  if (DBG) { disassembly(stdout, program, sizeof(program)); puts(""); }
  puts("");
  run_program(program, sizeof(program));
}

void p5() {
  puts("p5: compute 4! using non-tail recursive i_call/i_ret");
  instr program[] = {
    // goto main
    i_goto, 18,
    // factorial
    i_dup,
    i_jump_if, 7,         // exit below if top is zero, otherwise jump +2
    i_panic, i_noop,
    //i_ret, 1,             // return
    i_swap,               // ..., acc, n] -> ..., n, acc]
    i_dupbis,             //              -> ..., n, acc, n]
    i_32mul,              //              -> ..., n, n x acc]
    i_swap,               //              -> ..., n x acc, n]
    i_32dec,              //              -> ..., n x acc, n - 1]
    i_push_u8, 2,         // factorial address
    i_call, 2,            // recur
    i_ret, 1,
    // push initial values
    i_push_u8, 1,
    i_push_u8, 5,
    // call factorial
    i_push_u8, 2,         // factorial address
    i_call, 2
  };
  if (DBG) { disassembly(stdout, program, sizeof(program)); puts(""); }
  puts("");
  run_program(program, sizeof(program));
}

void p6() {
  puts("p6: calling panic after some number of i_call");
  instr program[] = {
    i_push_u8, 1,
    i_push_u8, 2,
    i_push_u8, 10,  // &f1
    i_call, 2,
    i_ret, 1,
    i_noop,         // f1, addr = 10
    i_32add,
    i_push_u8, 3,
    i_push_u8, 20,  // &f2
    i_call, 2,
    i_ret, 1,
    i_noop,         // f2, addr = 20
    i_32mul,
    i_push_u8, 4,
    i_32mul,
    i_push_u8, 31,  // &f3
    i_call, 2,
    i_ret, 1,
    i_noop,         // f3, addr = 31
    i_panic,
  };
  if (DBG) { disassembly(stdout, program, sizeof(program)); puts(""); }
  puts("");
  run_program(program, sizeof(program));
}

void p7() {
  puts("p7: sum of square with an sub function ");
  instr program[] = {
    i_goto, 26,     // goto main
    // f1: square top of stack
    i_dup,
    i_32mul,
    i_ret, 1,
    // f2: sum of square
    i_dup,
    i_jump_if, 11,  // swap top of stack and exit below if top is zero
    i_ret, 1,
    i_swap,
    i_dupbis,
    i_push_u8, 2,   // &f1
    i_call, 1,
    i_32add,
    i_swap,
    i_32dec,
    i_push_u8, 6,   // &f2
    i_call, 2,
    i_ret, 1,
    // main: prepare sack and call f2
    i_push_u8, 0,
    i_push_u8, 4,
    i_push_u8, 6,   // &f2,
    i_call, 2,
  };
  if (DBG) { disassembly(stdout, program, sizeof(program)); puts(""); }
  puts("");
  run_program(program, sizeof(program));
}

void p8() {
  puts("p8: compute fibonacci using i_load/i_store");
  instr program[] = {
    // goto main
    i_goto, 18,
    // fib
    i_dup,
    i_jump_if, 7,         // exit below if top is zero, otherwise jump +2
    i_ret, 1,             // return
    i_dupbis,             // [f_n-1, f_n, n] -> [f_n-1, f_n, n, f_n]
    i_dup,                // [f_n-1, f_n, n] -> [f_n-1, f_n, n, f_n, f_n]
    i_load, 0,            //                 -> [f_n-1, f_n, n, f_n, f_n, f_n-1]
    i_32add,              //                 -> [f_n-1, f_n, n, f_n, f_n+1]
    i_store, 1,           //                 -> [f_n-1, f_n+1, n, f_n]
    i_store, 0,           //                 -> [f_n, f_n+1, n]
    i_32dec,
    i_recur,
    // push initial values
    i_push_u8, 0,         // f0
    i_push_u8, 1,         // f1
    i_push_u8, 40,        // fib(n)
    i_push_u8, 2,         // &fib
    i_call, 3
  };
  if (DBG) { disassembly(stdout, program, sizeof(program)); puts(""); }
  puts("");
  run_program(program, sizeof(program));
}

typedef void (*void_fn)();

int main(int argc, char *argv[]) {
  void_fn programs[] = {
    //p1,
    //p2,
    //p3,
    //p4,
    //p5,
    //p6,
    //p7,
    p8,
  };

  size_t len = sizeof(programs) / sizeof(programs[0]);
  for (int i = 0; i < len; i++) {
      programs[i]();
      if (i < len - 1) {
        puts("");
        puts("");
      }
  }
}
