#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#define DBG 1

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

/*
 * TODO next
 *  - implement ctx dump with pretty printing for call stack too
 *  - covers other exception paths like ctx_ip_set
 *  - debug i_call/i_ret
 *  - implement factorial with call/ret
 *  - add load/store and change factorial
 *  - add fibonacci, both exponential and linear version
 *  - add recur
 *  - replace static ctx.ip_end by a dynamic method end point set when calling/returning method
 */

typedef uint8_t instr;

static const int instr_multibyte_mask = 3 << 6;
static const int instr_codepoint_mask = 0x3f;

#define multi(i, nadd) (((nadd & 3) << 6) | (i & 0x3f));

static int instr_nbyte(instr i) {
  return (i & instr_multibyte_mask) >> 6;
}

static instr instr_codepoint(instr i) {
  return (i & instr_codepoint_mask);
}

const int instr_max = 25;    // next instr id
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

// TODO:
const instr i_load    =  22;
const instr i_store   =  23;
const instr i_recur   =  24;

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
};
static int instr_names_len = sizeof(instr_names);
static char* instr_unknown = "unknown";

char* instr_name(instr i) {
  i = instr_codepoint(i);
  if (0 <= i && i < instr_names_len) {
    return instr_names[i];
  }
  return instr_unknown;
}

void disassembly(FILE* f, instr* program, size_t len) {
  instr* end = program + len;
  while (program < end) {
    instr i = *program++;
    int nbytes = instr_nbyte(i);
    //fprintf(f, "%s\n", instr_name(i));
    //program += nbytes;
    instr a, b, c;
    switch (nbytes) {
      case 0:
        fprintf(f, "%s\n", instr_name(i));
        break;
      case 1:
        a = *program++;
        fprintf(f, "%s, %i\n", instr_name(i), a);
        break;
      case 2:
        a = *program++;
        b = *program++;
        fprintf(f, "%s, %i, %i\n", instr_name(i), a, b);
        break;
      case 3:
        a = *program++;
        b = *program++;
        c = *program++;
        fprintf(f, "%s, %i, %i, %i\n", instr_name(i), a, b, c);
        break;
      default:
        snprintf(buf, sizeof(buf), "unexpected number of bytes %d", nbytes);
        fatal(buf);
    }
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




struct call {
  instr*    ip;
  uint32_t* fp;
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




struct ctx {
  struct datastack data;
  struct callstack call;

  struct call current;
  instr* ip_end;
};

void ctx_new(struct ctx *c,
             size_t depth
             ) {
  datastack_new(&c -> data, depth);
  callstack_new(&c -> call, 256);
}

void ctx_del(struct ctx *c) {
  datastack_del(&c -> data);
  callstack_del(&c -> call);
}

void ctx_data_print(struct ctx *c, const char* indent) {
  int i = 0;
  uint32_t* s = c -> data.bottom;
  while (s < c -> data.top) {
    printf("%s%.2i: 0x%.8x %i\n", indent, i++, *s, *s);
    s++;
  }
}

void ctx_dump(struct ctx *c, FILE* f) {
  // also print call stack,
  // also print current function code and current
  //  add disassembly
  ctx_data_print(c, "| ");
  // also mark data stack with frame pointers from call stack
}

void _ctx_dump_fatal(FILE* f, struct ctx *c, const char *msg, int ln) {
    fprintf(f, "full context dump\n");
    fprintf(f, "-----------------\n");
    ctx_dump(c, f);
    _fatal(msg, ln);
}
#define ctx_dump_fatal(f, c, msg) _ctx_dump_fatal(f, c, msg, __LINE__);

void ctx_ip_next(struct ctx *c) {
  c -> current.ip++;
  if (c -> current.ip >= c -> ip_end) {
    ctx_dump_fatal(stderr, c, "program pointer overflow");
  }
}

instr ctx_ip_get(struct ctx *c) {
  return *(c -> current.ip);
}

void ctx_ip_set(struct ctx *c, instr* ip) {
  if (ip >= c -> ip_end) {
    ctx_dump_fatal(stderr, c, "invalid program address");
  }
  c -> current.ip = ip;
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

// Jumps to function pointed by 'callee' which takes 'input_n' input args.
// It does so by:
//  - pushing the current activation record on the call stack
//  - creating a new activation record
void ctx_call(struct ctx *c, instr* callee, int input_n) {
  if (c-> call.top == c -> call.end) {
    ctx_dump_fatal(stderr, c, "call stack overflow");
  }
  *(c -> call.top)++ = c -> current;
  c -> current = (struct call){
    callee,
    c -> data.top - input_n
  };
}

// Returns from current function and returns 'output_n' args to the caller.
// It does so by:
//  - restoring the data stack top, taking into account the number of output args.
//  - popping the top activation record of the call stack and making it current
void ctx_ret(struct ctx *c, int output_n) {
  if (c-> call.top == c -> call.bottom) {
    ctx_dump_fatal(stderr, c, "call stack underflow");
  }
  c -> data.top = c -> current.fp + output_n;
  c -> current = *--(c -> call.top);
}

void exec(struct ctx *c,        // execution context containing stack area
          instr* program,       // program to execute
          size_t len            // program length
          ) {

  c -> current.ip = program;
  c -> current.fp = c -> data.top;
  c -> ip_end     = program + len;

  int32_t r0, r1;

  while (c -> current.ip < c -> ip_end) {

    if (DBG) {
      ctx_data_print(c, "  :");
      instr i = ctx_ip_get(c);
      printf("instr %s (%i:%i)\n", instr_name(i), instr_codepoint(i), instr_nbyte(i));
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
        r0 = ctx_pop(c);
        r1 = ctx_pop(c);
        ctx_push(c, r0 + r1);
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
        ctx_push(c, ctx_pop(c) + 1);
        break;
      case i_32dec:
        ctx_push(c, ctx_pop(c) - 1);
        break;
      case i_dup:
        // PERF: read top of stack instead of popping
        r0 = ctx_pop(c);
        ctx_push(c, r0);
        ctx_push(c, r0);
        break;
      case i_dupbis:
        // PERF: read top two of stack instead of popping
        r0 = ctx_pop(c);
        r1 = ctx_pop(c);
        ctx_push(c, r1);
        ctx_push(c, r0);
        ctx_push(c, r1);
        break;
      case i_swap:
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
      case i_call:
        ctx_ip_next(c);
        r0 = ctx_pop(c);
        ctx_call(c, program + r0, ctx_ip_get(c));
      case i_ret:
        ctx_ip_next(c);
        ctx_ret(c, ctx_ip_get(c));
      case i_noop:
        break;
      default:
        break;
    }
    c -> current.ip++; // do not use ctx_ip_next() to avoid fatal-ing at program end
  }
}

void run_program(instr* p, size_t len) {
  struct ctx c;
  ctx_new(&c, 256);
  exec(&c, p, len);
  ctx_data_print(&c, "");
  ctx_del(&c);
}

// 2 * (4 + (3 * 5)) - 10 = 28
void p1() {
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

  struct fn fn_p1 = (struct fn){ program, sizeof(program) };
  struct program p1 = (struct program){ &fn_p1, 1, 0 };

  if (DBG) disassembly(stdout, program, sizeof(program));
  run_program(program, sizeof(program));
}

// count until 20
void p2() {
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
  run_program(program, sizeof(program));
}

void p3() {
  // compute 4!
  instr program[] = {
    // initial values
    i_push_u8, 1,
    i_push_u8, 5,
    // factorial begin
    i_dup,
    i_jump_if, 9,         // exit below if top is zero, otherwise jump +2
    i_goto, 100,          // exit
    i_swap,               // ..., acc, n] -> ..., n, acc]
    i_dupbis,             //              -> ..., n, acc, n]
    i_32mul,              //              -> ..., n, n x acc]
    i_swap,               //              -> ..., n x acc, n]
    i_32dec,              //              -> ..., n x acc, n - 1]
    i_goto, 4
  };

  if (DBG) disassembly(stdout, program, sizeof(program));
  run_program(program, sizeof(program));
}

void p4() { // like p3, but with call/ret
  // compute 4!
  instr program[] = {
    // goto main
    i_goto, 18,
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

  if (DBG) disassembly(stdout, program, sizeof(program));
  if (0) {
  run_program(program, sizeof(program));
  }
}

int main(int argc, char *argv[]) {
  puts("p1");
  p1();

  puts("");
  puts("p2");
  p2();

  puts("");
  puts("p3");
  p3();

  puts("");
  puts("p4");
  p4();
}
