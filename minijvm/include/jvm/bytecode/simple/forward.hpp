#pragma once

#include <variant>
#include <vector>

#include <jvm/execution/stack.hpp>
#include <jvm/object/object.hpp>
#include <jvm/utils/ref.hpp>

#define JVM_INSTRUCTION_INVOKE_PARAMS \
    [[maybe_unused]] jvm::Stack& stack,\
    [[maybe_unused]] std::vector<jvm::Object::Value>& local,\
    [[maybe_unused]] jvm::Object& retValue

namespace jvm::bytecode {
    class nop;
    
    class aconst_null;
    
    class iconst_m1;
    class iconst_0;
    class iconst_1;
    class iconst_2;
    class iconst_3;
    class iconst_4;
    class iconst_5;

    class lconst_0;
    class lconst_1;

    class fconst_0;
    class fconst_1;
    class fconst_2;

    class dconst_0;
    class dconst_1;

    class bipush;
    class sipush;

    class ldc;
    class ldc_w;
    class ldc2_w;

    class iload;
    class lload;
    class fload;
    class dload;
    class aload;

    class iload_0;
    class iload_1;
    class iload_2;
    class iload_3;

    class lload_0;
    class lload_1;
    class lload_2;
    class lload_3;

    class fload_0;
    class fload_1;
    class fload_2;
    class fload_3;

    class dload_0;
    class dload_1;
    class dload_2;
    class dload_3;

    class aload_0;
    class aload_1;
    class aload_2;
    class aload_3;

    class iaload;
    class laload;
    class faload;
    class daload;
    class aaload;
    class baload;
    class caload;
    class saload;

    class istore;
    class lstore;
    class fstore;
    class dstore;
    class astore;

    class istore_0;
    class istore_1;
    class istore_2;
    class istore_3;

    class lstore_0;
    class lstore_1;
    class lstore_2;
    class lstore_3;

    class fstore_0;
    class fstore_1;
    class fstore_2;
    class fstore_3;
    
    class dstore_0;
    class dstore_1;
    class dstore_2;
    class dstore_3;

    class astore_0;
    class astore_1;
    class astore_2;
    class astore_3;

    class iastore;
    class lastore;
    class fastore;
    class dastore;
    class aastore;
    class bastore;
    class castore;
    class sastore;

    class pop;
    class pop2;

    class dup;
    class dup_x1;
    class dup_x2;
    class dup2;
    class dup2_x1;
    class dup2_x2;

    class swap;

    class iadd;
    class ladd;
    class fadd;
    class dadd;

    class isub;
    class lsub;
    class fsub;
    class dsub;

    class imul;
    class lmul;
    class fmul;
    class dmul;

    class idiv;
    class ldiv;
    class fdiv;
    class ddiv;

    class irem;
    class lrem;
    class frem;
    class drem;

    class ineg;
    class lneg;
    class fneg;
    class dneg;

    class ishl;
    class lshl;
    class ishr;
    class lshr;
    class iushr;
    class lushr;

    class iand;
    class land;
    class ior;
    class lor;
    class ixor;
    class lxor;

    class iinc;
    
    class i2l;
    class i2f;
    class i2d;

    class l2i;
    class l2f;
    class l2d;

    class f2i;
    class f2l;
    class f2d;

    class d2i;
    class d2l;
    class d2f;

    class i2b;
    class i2c;
    class i2s;

    class lcmp;
    class fcmpl;
    class fcmpg;
    class dcmpl;
    class dcmpg;

    class ifeq;
    class ifne;
    class iflt;
    class ifgt;
    class ifge;
    class ifle;

    class if_icmpeq;
    class if_icmpne;
    class if_icmplt;
    class if_icmpge;
    class if_icmpgt;
    class if_icmple;
    class if_acmpeq;
    class if_acmpne;

    class b_goto;

    class jsr;
    class ret;

    class tableswitch;
    class lookupswitch;

    class ireturn;
    class lreturn;
    class freturn;
    class dreturn;
    class areturn;
    class b_return;
    
    class getstatic;
    class putstatic;
    class getfield;
    class putfield;

    class invokevirtual;
    class invokespecial;
    class invokestatic;
    class invokeinterface;
    class invokedynamic;

    class b_new;
    class newarray;
    class anewarray;
    class arraylength;
    
    class athrow;
    
    class checkcast;

    class instanceof;

    class monitorenter;
    class monitorexit;

    class wide;
    
    class multianewarray;

    class ifnull;
    class ifnonnull;

    class b_goto_w;

    class jsr_w;

    class breakpoint;

    class impdep1;
    class impdep2;

    class BaseInstruction;

    using Instruction = jvm::Ref<BaseInstruction>; 
    using Instructions = std::vector<Instruction>;
}

