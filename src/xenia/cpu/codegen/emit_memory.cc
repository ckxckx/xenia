/*
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2013 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include <xenia/cpu/codegen/emit.h>

#include <xenia/cpu/codegen/function_generator.h>


using namespace llvm;
using namespace xe::cpu::codegen;
using namespace xe::cpu::ppc;


namespace xe {
namespace cpu {
namespace codegen {


// Integer load (A-13)

XEEMITTER(lbz,          0x88000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + EXTS(D)
  // RT <- i56.0 || MEM(EA, 1)

  Value* ea = b.getInt64(XEEXTS16(i.D.DS));
  if (i.D.RA) {
    ea = b.CreateAdd(g.gpr_value(i.D.RA), ea);
  }
  Value* v = g.ReadMemory(i.address, ea, 1, false);
  g.update_gpr_value(i.D.RT, v);

  return 0;
}

XEEMITTER(lbzu,         0x8C000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + EXTS(D)
  // RT <- i56.0 || MEM(EA, 1)
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.D.RA), b.getInt64(XEEXTS16(i.D.DS)));
  Value* v = g.ReadMemory(i.address, ea, 1, false);
  g.update_gpr_value(i.D.RT, v);
  g.update_gpr_value(i.D.RA, ea);

  return 0;
}

XEEMITTER(lbzux,        0x7C0000EE, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + (RB)
  // RT <- i56.0 || MEM(EA, 1)
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.X.RA), g.gpr_value(i.X.RB));
  Value* v = g.ReadMemory(i.address, ea, 1, false);
  g.update_gpr_value(i.X.RT, v);
  g.update_gpr_value(i.X.RA, ea);

  return 0;
}

XEEMITTER(lbzx,         0x7C0000AE, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + (RB)
  // RT <- i56.0 || MEM(EA, 1)

  Value* ea = g.gpr_value(i.X.RB);
  if (i.X.RA) {
    ea = b.CreateAdd(g.gpr_value(i.X.RA), ea);
  }
  Value* v = g.ReadMemory(i.address, ea, 1, false);
  g.update_gpr_value(i.X.RT, v);

  return 0;
}

XEEMITTER(ld,           0xE8000000, DS )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + EXTS(DS || 0b00)
  // RT <- MEM(EA, 8)

  Value* ea = b.getInt64(XEEXTS16(i.DS.DS << 2));
  if (i.DS.RA) {
    ea = b.CreateAdd(g.gpr_value(i.DS.RA), ea);
  }
  Value* v = g.ReadMemory(i.address, ea, 8, false);
  g.update_gpr_value(i.DS.RT, v);

  return 0;
}

XEEMITTER(ldu,          0xE8000001, DS )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + EXTS(DS || 0b00)
  // RT <- MEM(EA, 8)
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.DS.RA),
                          b.getInt64(XEEXTS16(i.DS.DS << 2)));
  Value* v = g.ReadMemory(i.address, ea, 8, false);
  g.update_gpr_value(i.DS.RT, v);
  g.update_gpr_value(i.DS.RA, ea);

  return 0;
}

XEEMITTER(ldux,         0x7C00006A, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}

XEEMITTER(ldx,          0x7C00002A, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}

XEEMITTER(lha,          0xA8000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + EXTS(D)
  // RT <- EXTS(MEM(EA, 2))

  Value* ea = b.getInt64(XEEXTS16(i.D.DS));
  if (i.D.RA) {
    ea = b.CreateAdd(g.gpr_value(i.D.RA), ea);
  }
  Value* v = b.CreateSExt(g.ReadMemory(i.address, ea, 2, false),
                          b.getInt64Ty());
  g.update_gpr_value(i.D.RT, v);

  return 0;
}

XEEMITTER(lhau,         0xAC000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}

XEEMITTER(lhaux,        0x7C0002EE, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}

XEEMITTER(lhax,         0x7C0002AE, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + (RB)
  // RT <- EXTS(MEM(EA, 2))

  Value* ea = g.gpr_value(i.X.RB);
  if (i.X.RA) {
    ea = b.CreateAdd(g.gpr_value(i.X.RA), ea);
  }
  Value* v = b.CreateSExt(g.ReadMemory(i.address, ea, 2, false),
                          b.getInt64Ty());
  g.update_gpr_value(i.X.RT, v);

  return 0;
}

XEEMITTER(lhz,          0xA0000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + EXTS(D)
  // RT <- i48.0 || MEM(EA, 2)

  Value* ea = b.getInt64(XEEXTS16(i.D.DS));
  if (i.D.RA) {
    ea = b.CreateAdd(g.gpr_value(i.D.RA), ea);
  }
  Value* v = g.ReadMemory(i.address, ea, 2, false);
  g.update_gpr_value(i.D.RT, v);

  return 0;
}

XEEMITTER(lhzu,         0xA4000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + EXTS(D)
  // RT <- i48.0 || MEM(EA, 2)
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.D.RA), b.getInt64(XEEXTS16(i.D.DS)));
  Value* v = g.ReadMemory(i.address, ea, 2, false);
  g.update_gpr_value(i.D.RT, v);
  g.update_gpr_value(i.D.RA, ea);

  return 0;
}

XEEMITTER(lhzux,        0x7C00026E, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + (RB)
  // RT <- i48.0 || MEM(EA, 2)
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.X.RA), g.gpr_value(i.X.RB));
  Value* v = g.ReadMemory(i.address, ea, 2, false);
  g.update_gpr_value(i.X.RT, v);
  g.update_gpr_value(i.X.RA, ea);

  return 0;
}

XEEMITTER(lhzx,         0x7C00022E, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + (RB)
  // RT <- i48.0 || MEM(EA, 2)

  Value* ea = g.gpr_value(i.X.RB);
  if (i.X.RA) {
    ea = b.CreateAdd(g.gpr_value(i.X.RA), ea);
  }
  Value* v = g.ReadMemory(i.address, ea, 2, false);
  g.update_gpr_value(i.X.RT, v);

  return 0;
}

XEEMITTER(lwa,          0xE8000002, DS )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + EXTS(D || 00)
  // RT <- EXTS(MEM(EA, 4))

  Value* ea = b.getInt64(XEEXTS16(i.DS.DS << 2));
  if (i.DS.RA) {
    ea = b.CreateAdd(g.gpr_value(i.DS.RA), ea);
  }
  Value* v = b.CreateSExt(g.ReadMemory(i.address, ea, 4, false),
                          b.getInt64Ty());
  g.update_gpr_value(i.DS.RT, v);

  return 0;
}

XEEMITTER(lwaux,        0x7C0002EA, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + (RB)
  // RT <- EXTS(MEM(EA, 4))
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.X.RA), g.gpr_value(i.X.RB));
  Value* v = b.CreateSExt(g.ReadMemory(i.address, ea, 4, false),
                          b.getInt64Ty());
  g.update_gpr_value(i.X.RT, v);
  g.update_gpr_value(i.X.RA, ea);

  return 0;
}

XEEMITTER(lwax,         0x7C0002AA, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + (RB)
  // RT <- EXTS(MEM(EA, 4))

  Value* ea = g.gpr_value(i.X.RB);
  if (i.X.RA) {
    ea = b.CreateAdd(g.gpr_value(i.X.RA), ea);
  }
  Value* v = b.CreateSExt(g.ReadMemory(i.address, ea, 4, false),
                          b.getInt64Ty());
  g.update_gpr_value(i.X.RT, v);

  return 0;
}

XEEMITTER(lwz,          0x80000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + EXTS(D)
  // RT <- i32.0 || MEM(EA, 4)

  Value* ea = b.getInt64(XEEXTS16(i.D.DS));
  if (i.D.RA) {
    ea = b.CreateAdd(g.gpr_value(i.D.RA), ea);
  }
  Value* v = g.ReadMemory(i.address, ea, 4, false);
  g.update_gpr_value(i.D.RT, v);

  return 0;
}

XEEMITTER(lwzu,         0x84000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + EXTS(D)
  // RT <- i32.0 || MEM(EA, 4)
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.D.RA), b.getInt64(XEEXTS16(i.D.DS)));
  Value* v = g.ReadMemory(i.address, ea, 4, false);
  g.update_gpr_value(i.D.RT, v);
  g.update_gpr_value(i.D.RA, ea);

  return 0;
}

XEEMITTER(lwzux,        0x7C00006E, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + (RB)
  // RT <- i32.0 || MEM(EA, 4)
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.X.RA), g.gpr_value(i.X.RB));
  Value* v = g.ReadMemory(i.address, ea, 4, false);
  g.update_gpr_value(i.X.RT, v);
  g.update_gpr_value(i.X.RA, ea);

  return 0;
}

XEEMITTER(lwzx,         0x7C00002E, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + (RB)
  // RT <- i32.0 || MEM(EA, 4)

  Value* ea = g.gpr_value(i.X.RB);
  if (i.X.RA) {
    ea = b.CreateAdd(g.gpr_value(i.X.RA), ea);
  }
  Value* v = g.ReadMemory(i.address, ea, 4, false);
  g.update_gpr_value(i.X.RT, v);

  return 0;
}


// Integer store (A-14)

XEEMITTER(stb,          0x98000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + EXTS(D)
  // MEM(EA, 1) <- (RS)[56:63]

  Value* ea = b.getInt64(XEEXTS16(i.D.DS));
  if (i.D.RA) {
    ea = b.CreateAdd(g.gpr_value(i.D.RA), ea);
  }
  Value* v = g.gpr_value(i.D.RT);
  g.WriteMemory(i.address, ea, 1, v);

  return 0;
}

XEEMITTER(stbu,         0x9C000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + EXTS(D)
  // MEM(EA, 1) <- (RS)[56:63]
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.D.RA), b.getInt64(XEEXTS16(i.D.DS)));
  Value* v = g.gpr_value(i.D.RT);
  g.WriteMemory(i.address, ea, 1, v);
  g.update_gpr_value(i.D.RA, ea);

  return 0;
}

XEEMITTER(stbux,        0x7C0001EE, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + (RB)
  // MEM(EA, 1) <- (RS)[56:63]
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.X.RA), g.gpr_value(i.X.RB));
  Value* v = g.gpr_value(i.X.RT);
  g.WriteMemory(i.address, ea, 1, v);
  g.update_gpr_value(i.X.RA, ea);

  return 0;
}

XEEMITTER(stbx,         0x7C0001AE, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + (RB)
  // MEM(EA, 1) <- (RS)[56:63]

  Value* ea = g.gpr_value(i.X.RB);
  if (i.D.RA) {
    ea = b.CreateAdd(g.gpr_value(i.X.RA), ea);
  }
  Value* v = g.gpr_value(i.X.RT);
  g.WriteMemory(i.address, ea, 1, v);

  return 0;
}

XEEMITTER(std,          0xF8000000, DS )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + EXTS(DS || 0b00)
  // MEM(EA, 8) <- (RS)

  Value* ea = b.getInt64(XEEXTS16(i.DS.DS << 2));
  if (i.DS.RA) {
    ea = b.CreateAdd(g.gpr_value(i.DS.RA), ea);
  }
  Value* v = g.gpr_value(i.DS.RT);
  g.WriteMemory(i.address, ea, 8, v);

  return 0;
}

XEEMITTER(stdu,         0xF8000001, DS )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + EXTS(DS || 0b00)
  // MEM(EA, 8) <- (RS)
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.DS.RA),
                          b.getInt64(XEEXTS16(i.DS.DS << 2)));
  Value* v = g.gpr_value(i.DS.RT);
  g.WriteMemory(i.address, ea, 8, v);
  g.update_gpr_value(i.DS.RA, ea);

  return 0;
}

XEEMITTER(stdux,        0x7C00016A, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + (RB)
  // MEM(EA, 8) <- (RS)

  Value* ea = g.gpr_value(i.X.RB);
  if (i.X.RA) {
    ea = b.CreateAdd(g.gpr_value(i.X.RA), ea);
  }
  Value* v = g.gpr_value(i.X.RT);
  g.WriteMemory(i.address, ea, 8, v);

  return 0;
}

XEEMITTER(stdx,         0x7C00012A, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + (RB)
  // MEM(EA, 8) <- (RS)
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.X.RA), g.gpr_value(i.X.RB));
  Value* v = g.gpr_value(i.X.RT);
  g.WriteMemory(i.address, ea, 8, v);
  g.update_gpr_value(i.X.RA, ea);

  return 0;
}

XEEMITTER(sth,          0xB0000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + EXTS(D)
  // MEM(EA, 2) <- (RS)[48:63]

  Value* ea = b.getInt64(XEEXTS16(i.D.DS));
  if (i.D.RA) {
    ea = b.CreateAdd(g.gpr_value(i.D.RA), ea);
  }
  Value* v = g.gpr_value(i.D.RT);
  g.WriteMemory(i.address, ea, 2, v);

  return 0;
}

XEEMITTER(sthu,         0xB4000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + EXTS(D)
  // MEM(EA, 2) <- (RS)[48:63]
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.D.RA),
                          b.getInt64(XEEXTS16(i.D.DS)));
  Value* v = g.gpr_value(i.D.RT);
  g.WriteMemory(i.address, ea, 2, v);
  g.update_gpr_value(i.D.RA, ea);

  return 0;
}

XEEMITTER(sthux,        0x7C00036E, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + (RB)
  // MEM(EA, 2) <- (RS)[48:63]
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.X.RA), g.gpr_value(i.X.RB));
  Value* v = g.gpr_value(i.X.RT);
  g.WriteMemory(i.address, ea, 2, v);
  g.update_gpr_value(i.X.RA, ea);

  return 0;
}

XEEMITTER(sthx,         0x7C00032E, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + (RB)
  // MEM(EA, 2) <- (RS)[48:63]

  Value* ea = g.gpr_value(i.X.RB);
  if (i.D.RA) {
    ea = b.CreateAdd(g.gpr_value(i.X.RA), ea);
  }
  Value* v = g.gpr_value(i.X.RT);
  g.WriteMemory(i.address, ea, 2, v);

  return 0;
}

XEEMITTER(stw,          0x90000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + EXTS(D)
  // MEM(EA, 4) <- (RS)[32:63]

  Value* ea = b.getInt64(XEEXTS16(i.D.DS));
  if (i.D.RA) {
    ea = b.CreateAdd(g.gpr_value(i.D.RA), ea);
  }
  Value* v = g.gpr_value(i.D.RT);
  g.WriteMemory(i.address, ea, 4, v);

  return 0;
}

XEEMITTER(stwu,         0x94000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + EXTS(D)
  // MEM(EA, 4) <- (RS)[32:63]
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.D.RA),
                          b.getInt64(XEEXTS16(i.D.DS)));
  Value* v = g.gpr_value(i.D.RT);
  g.WriteMemory(i.address, ea, 4, v);
  g.update_gpr_value(i.D.RA, ea);

  return 0;
}

XEEMITTER(stwux,        0x7C00016E, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + (RB)
  // MEM(EA, 4) <- (RS)[32:63]
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.X.RA), g.gpr_value(i.X.RB));
  Value* v = g.gpr_value(i.X.RT);
  g.WriteMemory(i.address, ea, 4, v);
  g.update_gpr_value(i.X.RA, ea);

  return 0;
}

XEEMITTER(stwx,         0x7C00012E, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + (RB)
  // MEM(EA, 4) <- (RS)[32:63]

  Value* ea = g.gpr_value(i.X.RB);
  if (i.X.RA) {
    ea = b.CreateAdd(g.gpr_value(i.X.RA), ea);
  }
  Value* v = g.gpr_value(i.X.RT);
  g.WriteMemory(i.address, ea, 4, v);

  return 0;
}


// Integer load and store with byte reverse (A-1

XEEMITTER(lhbrx,        0x7C00062C, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}

XEEMITTER(lwbrx,        0x7C00042C, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}

XEEMITTER(ldbrx,        0x7C000428, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}

XEEMITTER(sthbrx,       0x7C00072C, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}

XEEMITTER(stwbrx,       0x7C00052C, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}

XEEMITTER(stdbrx,       0x7C000528, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}


// Integer load and store multiple (A-16)

XEEMITTER(lmw,          0xB8000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}

XEEMITTER(stmw,         0xBC000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}


// Integer load and store string (A-17)

XEEMITTER(lswi,         0x7C0004AA, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}

XEEMITTER(lswx,         0x7C00042A, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}

XEEMITTER(stswi,        0x7C0005AA, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}

XEEMITTER(stswx,        0x7C00052A, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}


// Memory synchronization (A-18)

XEEMITTER(eieio,        0x7C0006AC, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}

XEEMITTER(isync,        0x4C00012C, XL )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}

XEEMITTER(ldarx,        0x7C0000A8, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}

XEEMITTER(lwarx,        0x7C000028, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + (RB)
  // RESERVE <- 1
  // RESERVE_LENGTH <- 4
  // RESERVE_ADDR <- real_addr(EA)
  // RT <- i32.0 || MEM(EA, 4)

  // TODO(benvanik): make this right

  Value* ea = g.gpr_value(i.X.RB);
  if (i.X.RA) {
    ea = b.CreateAdd(g.gpr_value(i.X.RA), ea);
  }
  Value* v = g.ReadMemory(i.address, ea, 4, /* acquire */ true);
  g.update_gpr_value(i.X.RT, v);

  return 0;
}

XEEMITTER(stdcx,        0x7C0001AD, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}

XEEMITTER(stwcx,        0x7C00012D, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + (RB)
  // RESERVE stuff...
  // MEM(EA, 4) <- (RS)[32:63]
  // n <- 1 if store performed
  // CR0[LT GT EQ SO] = 0b00 || n || XER[SO]

  // TODO(benvanik): make this right

  Value* ea = b.getInt64(XEEXTS16(i.D.DS));
  if (i.D.RA) {
    ea = b.CreateAdd(g.gpr_value(i.D.RA), ea);
  }
  Value* v = g.gpr_value(i.D.RT);
  g.WriteMemory(i.address, ea, 4, v, /* release */ true);

  // We always succeed.
  g.update_cr_value(0, b.getInt64(1 << 2));

  return 0;
}

XEEMITTER(sync,         0x7C0004AC, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}


// Floating-point load (A-19)

XEEMITTER(lfd,          0xC8000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + EXTS(D)
  // FRT <- MEM(EA, 8)

  Value* ea = b.getInt64(XEEXTS16(i.D.DS));
  if (i.D.RA) {
    ea = b.CreateAdd(g.gpr_value(i.D.RA), ea);
  }
  Value* v = g.ReadMemory(i.address, ea, 8, false);
  v = b.CreateBitCast(v, b.getDoubleTy());
  g.update_fpr_value(i.D.RT, v);

  return 0;
}

XEEMITTER(lfdu,         0xCC000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + EXTS(D)
  // FRT <- MEM(EA, 8)
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.D.RA), b.getInt64(XEEXTS16(i.D.DS)));
  Value* v = g.ReadMemory(i.address, ea, 8, false);
  v = b.CreateBitCast(v, b.getDoubleTy());
  g.update_fpr_value(i.D.RT, v);
  g.update_gpr_value(i.D.RA, ea);

  return 0;
}

XEEMITTER(lfdux,        0x7C0004EE, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + (RB)
  // FRT <- MEM(EA, 8)
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.X.RA), g.gpr_value(i.X.RB));
  Value* v = g.ReadMemory(i.address, ea, 8, false);
  v = b.CreateBitCast(v, b.getDoubleTy());
  g.update_fpr_value(i.X.RT, v);
  g.update_gpr_value(i.X.RA, ea);

  return 0;
}

XEEMITTER(lfdx,         0x7C0004AE, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + (RB)
  // FRT <- MEM(EA, 8)

  Value* ea = g.gpr_value(i.X.RB);
  if (i.X.RA) {
    ea = b.CreateAdd(g.gpr_value(i.X.RA), ea);
  }
  Value* v = g.ReadMemory(i.address, ea, 8, false);
  v = b.CreateBitCast(v, b.getDoubleTy());
  g.update_fpr_value(i.X.RT, v);

  return 0;
}

XEEMITTER(lfs,          0xC0000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + EXTS(D)
  // FRT <- DOUBLE(MEM(EA, 4))

  Value* ea = b.getInt64(XEEXTS16(i.D.DS));
  if (i.D.RA) {
    ea = b.CreateAdd(g.gpr_value(i.D.RA), ea);
  }
  Value* v = g.ReadMemory(i.address, ea, 4, false);
  v = b.CreateFPExt(b.CreateBitCast(v, b.getFloatTy()), b.getDoubleTy());
  g.update_fpr_value(i.D.RT, v);

  return 0;
}

XEEMITTER(lfsu,         0xC4000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + EXTS(D)
  // FRT <- DOUBLE(MEM(EA, 4))
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.D.RA), b.getInt64(XEEXTS16(i.D.DS)));
  Value* v = g.ReadMemory(i.address, ea, 4, false);
  v = b.CreateFPExt(b.CreateBitCast(v, b.getFloatTy()), b.getDoubleTy());
  g.update_fpr_value(i.D.RT, v);
  g.update_gpr_value(i.D.RA, ea);

  return 0;
}

XEEMITTER(lfsux,        0x7C00046E, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + (RB)
  // FRT <- DOUBLE(MEM(EA, 4))
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.X.RA), g.gpr_value(i.X.RB));
  Value* v = g.ReadMemory(i.address, ea, 4, false);
  v = b.CreateFPExt(b.CreateBitCast(v, b.getFloatTy()), b.getDoubleTy());
  g.update_fpr_value(i.X.RT, v);
  g.update_gpr_value(i.X.RA, ea);

  return 0;
}

XEEMITTER(lfsx,         0x7C00042E, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + (RB)
  // FRT <- DOUBLE(MEM(EA, 4))

  Value* ea = g.gpr_value(i.X.RB);
  if (i.X.RA) {
    ea = b.CreateAdd(g.gpr_value(i.X.RA), ea);
  }
  Value* v = g.ReadMemory(i.address, ea, 4, false);
  v = b.CreateFPExt(b.CreateBitCast(v, b.getFloatTy()), b.getDoubleTy());
  g.update_fpr_value(i.X.RT, v);

  return 0;
}


// Floating-point store (A-20)

XEEMITTER(stfd,         0xD8000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + EXTS(D)
  // MEM(EA, 8) <- (FRS)

  Value* ea = b.getInt64(XEEXTS16(i.D.DS));
  if (i.D.RA) {
    ea = b.CreateAdd(g.gpr_value(i.D.RA), ea);
  }
  Value* v = g.fpr_value(i.D.RT);
  v = b.CreateBitCast(v, b.getInt64Ty());
  g.WriteMemory(i.address, ea, 8, v);

  return 0;
}

XEEMITTER(stfdu,        0xDC000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + EXTS(D)
  // MEM(EA, 8) <- (FRS)
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.D.RA),
                          b.getInt64(XEEXTS16(i.D.DS)));
  Value* v = g.fpr_value(i.D.RT);
  v = b.CreateBitCast(v, b.getInt64Ty());
  g.WriteMemory(i.address, ea, 8, v);
  g.update_gpr_value(i.D.RA, ea);

  return 0;
}

XEEMITTER(stfdux,       0x7C0005EE, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + (RB)
  // MEM(EA, 8) <- (FRS)
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.X.RA), g.gpr_value(i.X.RB));
  Value* v = g.fpr_value(i.X.RT);
  v = b.CreateBitCast(v, b.getInt64Ty());
  g.WriteMemory(i.address, ea, 8, v);
  g.update_gpr_value(i.X.RA, ea);

  return 0;
}

XEEMITTER(stfdx,        0x7C0005AE, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + (RB)
  // MEM(EA, 8) <- (FRS)

  Value* ea = g.gpr_value(i.X.RB);
  if (i.X.RA) {
    ea = b.CreateAdd(g.gpr_value(i.X.RA), ea);
  }
  Value* v = g.fpr_value(i.X.RT);
  v = b.CreateBitCast(v, b.getInt64Ty());
  g.WriteMemory(i.address, ea, 8, v);

  return 0;
}

XEEMITTER(stfiwx,       0x7C0007AE, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + (RB)
  // MEM(EA, 4) <- (FRS)[32:63]

  Value* ea = g.gpr_value(i.X.RB);
  if (i.X.RA) {
    ea = b.CreateAdd(g.gpr_value(i.D.RA), ea);
  }
  Value* v = g.fpr_value(i.X.RT);
  v = b.CreateBitCast(v, b.getInt64Ty());
  g.WriteMemory(i.address, ea, 4, v);

  return 0;
}

XEEMITTER(stfs,         0xD0000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + EXTS(D)
  // MEM(EA, 4) <- SINGLE(FRS)

  Value* ea = b.getInt64(XEEXTS16(i.D.DS));
  if (i.D.RA) {
    ea = b.CreateAdd(g.gpr_value(i.D.RA), ea);
  }
  Value* v = g.fpr_value(i.D.RT);
  v = b.CreateBitCast(b.CreateFPTrunc(v, b.getFloatTy()), b.getInt32Ty());
  g.WriteMemory(i.address, ea, 4, v);

  return 0;
}

XEEMITTER(stfsu,        0xD4000000, D  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + EXTS(D)
  // MEM(EA, 4) <- SINGLE(FRS)
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.D.RA),
                          b.getInt64(XEEXTS16(i.D.DS)));
  Value* v = g.fpr_value(i.D.RT);
  v = b.CreateBitCast(b.CreateFPTrunc(v, b.getFloatTy()), b.getInt32Ty());
  g.WriteMemory(i.address, ea, 4, v);
  g.update_gpr_value(i.D.RA, ea);

  return 0;
}

XEEMITTER(stfsux,       0x7C00056E, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // EA <- (RA) + (RB)
  // MEM(EA, 4) <- SINGLE(FRS)
  // RA <- EA

  Value* ea = b.CreateAdd(g.gpr_value(i.X.RA), g.gpr_value(i.X.RB));
  Value* v = g.fpr_value(i.X.RT);
  v = b.CreateBitCast(b.CreateFPTrunc(v, b.getFloatTy()), b.getInt32Ty());
  g.WriteMemory(i.address, ea, 4, v);
  g.update_gpr_value(i.X.RA, ea);

  return 0;
}

XEEMITTER(stfsx,        0x7C00052E, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // if RA = 0 then
  //   b <- 0
  // else
  //   b <- (RA)
  // EA <- b + (RB)
  // MEM(EA, 4) <- SINGLE(FRS)

  Value* ea = g.gpr_value(i.X.RB);
  if (i.X.RA) {
    ea = b.CreateAdd(g.gpr_value(i.X.RA), ea);
  }
  Value* v = g.fpr_value(i.X.RT);
  v = b.CreateBitCast(b.CreateFPTrunc(v, b.getFloatTy()), b.getInt32Ty());
  g.WriteMemory(i.address, ea, 4, v);

  return 0;
}


// Cache management (A-27)

XEEMITTER(dcbf,         0x7C0000AC, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}

XEEMITTER(dcbst,        0x7C00006C, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}

XEEMITTER(dcbt,         0x7C00022C, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // No-op for now.
  // TODO(benvanik): use @llvm.prefetch
  return 0;
}

XEEMITTER(dcbtst,       0x7C0001EC, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // No-op for now.
  // TODO(benvanik): use @llvm.prefetch
  return 0;
}

XEEMITTER(dcbz,         0x7C0007EC, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  // or dcbz128 0x7C2007EC
  XEINSTRNOTIMPLEMENTED();
  return 1;
}

XEEMITTER(icbi,         0x7C0007AC, X  )(FunctionGenerator& g, IRBuilder<>& b, InstrData& i) {
  XEINSTRNOTIMPLEMENTED();
  return 1;
}


void RegisterEmitCategoryMemory() {
  XEREGISTERINSTR(lbz,          0x88000000);
  XEREGISTERINSTR(lbzu,         0x8C000000);
  XEREGISTERINSTR(lbzux,        0x7C0000EE);
  XEREGISTERINSTR(lbzx,         0x7C0000AE);
  XEREGISTERINSTR(ld,           0xE8000000);
  XEREGISTERINSTR(ldu,          0xE8000001);
  XEREGISTERINSTR(ldux,         0x7C00006A);
  XEREGISTERINSTR(ldx,          0x7C00002A);
  XEREGISTERINSTR(lha,          0xA8000000);
  XEREGISTERINSTR(lhau,         0xAC000000);
  XEREGISTERINSTR(lhaux,        0x7C0002EE);
  XEREGISTERINSTR(lhax,         0x7C0002AE);
  XEREGISTERINSTR(lhz,          0xA0000000);
  XEREGISTERINSTR(lhzu,         0xA4000000);
  XEREGISTERINSTR(lhzux,        0x7C00026E);
  XEREGISTERINSTR(lhzx,         0x7C00022E);
  XEREGISTERINSTR(lwa,          0xE8000002);
  XEREGISTERINSTR(lwaux,        0x7C0002EA);
  XEREGISTERINSTR(lwax,         0x7C0002AA);
  XEREGISTERINSTR(lwz,          0x80000000);
  XEREGISTERINSTR(lwzu,         0x84000000);
  XEREGISTERINSTR(lwzux,        0x7C00006E);
  XEREGISTERINSTR(lwzx,         0x7C00002E);
  XEREGISTERINSTR(stb,          0x98000000);
  XEREGISTERINSTR(stbu,         0x9C000000);
  XEREGISTERINSTR(stbux,        0x7C0001EE);
  XEREGISTERINSTR(stbx,         0x7C0001AE);
  XEREGISTERINSTR(std,          0xF8000000);
  XEREGISTERINSTR(stdu,         0xF8000001);
  XEREGISTERINSTR(stdux,        0x7C00016A);
  XEREGISTERINSTR(stdx,         0x7C00012A);
  XEREGISTERINSTR(sth,          0xB0000000);
  XEREGISTERINSTR(sthu,         0xB4000000);
  XEREGISTERINSTR(sthux,        0x7C00036E);
  XEREGISTERINSTR(sthx,         0x7C00032E);
  XEREGISTERINSTR(stw,          0x90000000);
  XEREGISTERINSTR(stwu,         0x94000000);
  XEREGISTERINSTR(stwux,        0x7C00016E);
  XEREGISTERINSTR(stwx,         0x7C00012E);
  XEREGISTERINSTR(lhbrx,        0x7C00062C);
  XEREGISTERINSTR(lwbrx,        0x7C00042C);
  XEREGISTERINSTR(ldbrx,        0x7C000428);
  XEREGISTERINSTR(sthbrx,       0x7C00072C);
  XEREGISTERINSTR(stwbrx,       0x7C00052C);
  XEREGISTERINSTR(stdbrx,       0x7C000528);
  XEREGISTERINSTR(lmw,          0xB8000000);
  XEREGISTERINSTR(stmw,         0xBC000000);
  XEREGISTERINSTR(lswi,         0x7C0004AA);
  XEREGISTERINSTR(lswx,         0x7C00042A);
  XEREGISTERINSTR(stswi,        0x7C0005AA);
  XEREGISTERINSTR(stswx,        0x7C00052A);
  XEREGISTERINSTR(eieio,        0x7C0006AC);
  XEREGISTERINSTR(isync,        0x4C00012C);
  XEREGISTERINSTR(ldarx,        0x7C0000A8);
  XEREGISTERINSTR(lwarx,        0x7C000028);
  XEREGISTERINSTR(stdcx,        0x7C0001AD);
  XEREGISTERINSTR(stwcx,        0x7C00012D);
  XEREGISTERINSTR(sync,         0x7C0004AC);
  XEREGISTERINSTR(lfd,          0xC8000000);
  XEREGISTERINSTR(lfdu,         0xCC000000);
  XEREGISTERINSTR(lfdux,        0x7C0004EE);
  XEREGISTERINSTR(lfdx,         0x7C0004AE);
  XEREGISTERINSTR(lfs,          0xC0000000);
  XEREGISTERINSTR(lfsu,         0xC4000000);
  XEREGISTERINSTR(lfsux,        0x7C00046E);
  XEREGISTERINSTR(lfsx,         0x7C00042E);
  XEREGISTERINSTR(stfd,         0xD8000000);
  XEREGISTERINSTR(stfdu,        0xDC000000);
  XEREGISTERINSTR(stfdux,       0x7C0005EE);
  XEREGISTERINSTR(stfdx,        0x7C0005AE);
  XEREGISTERINSTR(stfiwx,       0x7C0007AE);
  XEREGISTERINSTR(stfs,         0xD0000000);
  XEREGISTERINSTR(stfsu,        0xD4000000);
  XEREGISTERINSTR(stfsux,       0x7C00056E);
  XEREGISTERINSTR(stfsx,        0x7C00052E);
  XEREGISTERINSTR(dcbf,         0x7C0000AC);
  XEREGISTERINSTR(dcbst,        0x7C00006C);
  XEREGISTERINSTR(dcbt,         0x7C00022C);
  XEREGISTERINSTR(dcbtst,       0x7C0001EC);
  XEREGISTERINSTR(dcbz,         0x7C0007EC);
  XEREGISTERINSTR(icbi,         0x7C0007AC);
}


}  // namespace codegen
}  // namespace cpu
}  // namespace xe
