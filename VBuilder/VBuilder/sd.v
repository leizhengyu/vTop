// =====HEAD BEGIN ===============
// FileName    : sd.v
// Author      : VBuilder
// Create Time : 2015-04-21 00:37:01
// =====HEAD END =================

module SD
(
// =====PORTS BEGIN ==============
input         [1739151:0]  rst_n               ,
input            [  7:0]  op1                 ,
// =====PORTS END ================

);
// =====WIRES BEGIN ==============
wire             [ 31:0]  addr                ;
wire             [  8:0]  asdf                ;
wire             [ 72:0]  rlt                 ;
wire             [ 69:0]  asdfas              ;
wire             [  7:0]  data                ;
wire             [ 72:0]  clk                 ;
wire             [ 65:0]  op2                 ;
wire       [842150452:0]  1                   ;
// =====WIRES END ================

// =====USERDEFINED BEGIN ========
// =====USERDEFINED END ==========

// =====INSTANCES BEGIN ==========
// INSTANCE(TESTGEN,U0_TESTGEN,E:\testgen.v)
// CONNECT(                 rlt ,  73 , o ,                 rlt , e)
// CONNECT(              asdfas ,  70 , o ,              asdfas , w)
// CONNECT(               rst_n ,   8 , i ,               rst_n , i)
// CONNECT(                 clk ,   8 , i ,                 clk , e)
// CONNECT(                 op1 ,   8 , i ,                 op1 , i)
// CONNECT(                 op2 ,  66 , i ,                 op2 , e)

// INSTANCE(REG_MODULE,U1_REG_MODULE,E:\NewTest - ¸±±¾.v)
// PARAMETER(               WIDTH ,                   0)
// PARAMETER(               DEPTH ,                   0)
// CONNECT(                addr ,  32 , o ,                addr , w)
// CONNECT(                data ,   8 , i ,                data , w)
// CONNECT(                 clk ,   8 , i ,                 clk , e)

// INSTANCE(TEST2,U0_TEST2,E:\tes2.v)
// PARAMETER(               WIDTH ,                   8)
// PARAMETER(               DEPTH ,                  65)
// CONNECT(                 rlt ,  73 , o ,                 rlt , e)
// CONNECT(               rst_n ,   1 , i ,               rst_n , e)
// CONNECT(                 clk ,   1 , i ,                 clk , e)
// CONNECT(                 op1 ,   8 , i ,                 op1 , e)
// CONNECT(                 op2 ,  66 , i ,                 op2 , e)

// =====INSTANCES END ============

// =====DEFINES BEGIN ============
TESTGEN U0_TESTGEN
(
    .rlt                  ( rlt                 ),
    .asdfas               ( asdfas              ),
    .rst_n                ( rst_n               ),
    .clk                  ( clk                 ),
    .op1                  ( op1                 ),
    .op2                  ( op2                 )
);

REG_MODULE U1_REG_MODULE
#(
    .WIDTH     (0  ),
    .DEPTH     (0  )
)
(
    .addr                 ( addr                ),
    .data                 ( data                ),
    .clk                  ( clk                 )
);

TEST2 U0_TEST2
#(
    .WIDTH     (8  ),
    .DEPTH     (65 )
)
(
    .rlt                  ( rlt                 ),
    .rst_n                ( rst_n               ),
    .clk                  ( clk                 ),
    .op1                  ( op1                 ),
    .op2                  ( op2                 )
);

// =====DEFINES END ==============

// =====UI BEGIN =================
// U0_TESTGEN;535;74;635;174
// U1_REG_MODULE;415;73;515;173
// U0_TEST2;299;75;399;175
// =====UI END ===================

endmodule
