--------------------------------------------------------------------------------
--                         Compressor_23_3_F400_uid15
-- VHDL generated for Zynq7000 @ 400MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved
-- Authors:
--------------------------------------------------------------------------------
-- combinatorial
-- Clock period (ns): 2.5
-- Target frequency (MHz): 400
-- Input signals: X1 X0
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity Compressor_23_3_F400_uid15 is
    port (X1 : in  std_logic_vector(1 downto 0);
          X0 : in  std_logic_vector(2 downto 0);
          R : out  std_logic_vector(2 downto 0)   );
end entity;

architecture arch of Compressor_23_3_F400_uid15 is
signal X :  std_logic_vector(4 downto 0);
signal R0 :  std_logic_vector(2 downto 0);
begin
   X <= X1 & X0 ;

   with X  select  R0 <=
      "000" when "00000",
      "001" when "00001" | "00010" | "00100",
      "010" when "00011" | "00101" | "00110" | "01000" | "10000",
      "011" when "00111" | "01001" | "01010" | "01100" | "10001" | "10010" | "10100",
      "100" when "01011" | "01101" | "01110" | "10011" | "10101" | "10110" | "11000",
      "101" when "01111" | "10111" | "11001" | "11010" | "11100",
      "110" when "11011" | "11101" | "11110",
      "111" when "11111",
      "---" when others;
   R <= R0;
end architecture;

--------------------------------------------------------------------------------
--                         Compressor_3_2_F400_uid21
-- VHDL generated for Zynq7000 @ 400MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved
-- Authors:
--------------------------------------------------------------------------------
-- combinatorial
-- Clock period (ns): 2.5
-- Target frequency (MHz): 400
-- Input signals: X0
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity Compressor_3_2_F400_uid21 is
    port (X0 : in  std_logic_vector(2 downto 0);
          R : out  std_logic_vector(1 downto 0)   );
end entity;

architecture arch of Compressor_3_2_F400_uid21 is
signal X :  std_logic_vector(2 downto 0);
signal R0 :  std_logic_vector(1 downto 0);
begin
   X <= X0 ;

   with X  select  R0 <=
      "00" when "000",
      "01" when "001" | "010" | "100",
      "10" when "011" | "101" | "110",
      "11" when "111",
      "--" when others;
   R <= R0;
end architecture;

--------------------------------------------------------------------------------
--                         Compressor_14_3_F400_uid25
-- VHDL generated for Zynq7000 @ 400MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved
-- Authors:
--------------------------------------------------------------------------------
-- combinatorial
-- Clock period (ns): 2.5
-- Target frequency (MHz): 400
-- Input signals: X1 X0
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity Compressor_14_3_F400_uid25 is
    port (X1 : in  std_logic_vector(0 downto 0);
          X0 : in  std_logic_vector(3 downto 0);
          R : out  std_logic_vector(2 downto 0)   );
end entity;

architecture arch of Compressor_14_3_F400_uid25 is
signal X :  std_logic_vector(4 downto 0);
signal R0 :  std_logic_vector(2 downto 0);
begin
   X <= X1 & X0 ;

   with X  select  R0 <=
      "000" when "00000",
      "001" when "00001" | "00010" | "00100" | "01000",
      "010" when "00011" | "00101" | "00110" | "01001" | "01010" | "01100" | "10000",
      "011" when "00111" | "01011" | "01101" | "01110" | "10001" | "10010" | "10100" | "11000",
      "100" when "01111" | "10011" | "10101" | "10110" | "11001" | "11010" | "11100",
      "101" when "10111" | "11011" | "11101" | "11110",
      "110" when "11111",
      "---" when others;
   R <= R0;
end architecture;

--------------------------------------------------------------------------------
--                          DSPBlock_17x24_F400_uid6
-- VHDL generated for Zynq7000 @ 400MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved
-- Authors:
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 2.5
-- Target frequency (MHz): 400
-- Input signals: X Y
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library std;
use std.textio.all;
library work;

entity DSPBlock_17x24_F400_uid6 is
    port (clk : in std_logic;
          X : in  std_logic_vector(16 downto 0);
          Y : in  std_logic_vector(23 downto 0);
          R : out  std_logic_vector(40 downto 0)   );
end entity;

architecture arch of DSPBlock_17x24_F400_uid6 is
signal Mint :  std_logic_vector(40 downto 0);
signal M :  std_logic_vector(40 downto 0);
signal Rtmp :  std_logic_vector(40 downto 0);
begin
   Mint <= std_logic_vector(unsigned(X) * unsigned(Y)); -- multiplier
   M <= Mint(40 downto 0);
   Rtmp <= M;
   R <= Rtmp;
end architecture;

--------------------------------------------------------------------------------
--                          DSPBlock_12x12_F400_uid8
-- VHDL generated for Zynq7000 @ 400MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved
-- Authors:
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 2.5
-- Target frequency (MHz): 400
-- Input signals: X Y
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library std;
use std.textio.all;
library work;

entity DSPBlock_12x12_F400_uid8 is
    port (clk : in std_logic;
          X : in  std_logic_vector(11 downto 0);
          Y : in  std_logic_vector(11 downto 0);
          R : out  std_logic_vector(23 downto 0)   );
end entity;

architecture arch of DSPBlock_12x12_F400_uid8 is
signal Mint :  std_logic_vector(23 downto 0);
signal M :  std_logic_vector(23 downto 0);
signal Rtmp :  std_logic_vector(23 downto 0);
begin
   Mint <= std_logic_vector(unsigned(X) * unsigned(Y)); -- multiplier
   M <= Mint(23 downto 0);
   Rtmp <= M;
   R <= Rtmp;
end architecture;

--------------------------------------------------------------------------------
--                         DSPBlock_17x10_F400_uid10
-- VHDL generated for Zynq7000 @ 400MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved
-- Authors:
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 2.5
-- Target frequency (MHz): 400
-- Input signals: X Y
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library std;
use std.textio.all;
library work;

entity DSPBlock_17x10_F400_uid10 is
    port (clk : in std_logic;
          X : in  std_logic_vector(16 downto 0);
          Y : in  std_logic_vector(9 downto 0);
          R : out  std_logic_vector(26 downto 0)   );
end entity;

architecture arch of DSPBlock_17x10_F400_uid10 is
signal Mint :  std_logic_vector(26 downto 0);
signal M :  std_logic_vector(26 downto 0);
signal Rtmp :  std_logic_vector(26 downto 0);
begin
   Mint <= std_logic_vector(unsigned(X) * unsigned(Y)); -- multiplier
   M <= Mint(26 downto 0);
   Rtmp <= M;
   R <= Rtmp;
end architecture;

--------------------------------------------------------------------------------
--                         DSPBlock_17x10_F400_uid12
-- VHDL generated for Zynq7000 @ 400MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved
-- Authors:
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 2.5
-- Target frequency (MHz): 400
-- Input signals: X Y
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library std;
use std.textio.all;
library work;

entity DSPBlock_17x10_F400_uid12 is
    port (clk : in std_logic;
          X : in  std_logic_vector(16 downto 0);
          Y : in  std_logic_vector(9 downto 0);
          R : out  std_logic_vector(26 downto 0)   );
end entity;

architecture arch of DSPBlock_17x10_F400_uid12 is
signal Mint :  std_logic_vector(26 downto 0);
signal M :  std_logic_vector(26 downto 0);
signal Rtmp :  std_logic_vector(26 downto 0);
begin
   Mint <= std_logic_vector(unsigned(X) * unsigned(Y)); -- multiplier
   M <= Mint(26 downto 0);
   Rtmp <= M;
   R <= Rtmp;
end architecture;

--------------------------------------------------------------------------------
--                           IntAdder_38_F400_uid87
-- VHDL generated for Zynq7000 @ 400MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved
-- Authors: Bogdan Pasca, Florent de Dinechin (2008-2016)
--------------------------------------------------------------------------------
-- Pipeline depth: 2 cycles
-- Clock period (ns): 2.5
-- Target frequency (MHz): 400
-- Input signals: X Y Cin
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity IntAdder_38_F400_uid87 is
    port (clk : in std_logic;
          X : in  std_logic_vector(37 downto 0);
          Y : in  std_logic_vector(37 downto 0);
          Cin : in  std_logic;
          R : out  std_logic_vector(37 downto 0)   );
end entity;

architecture arch of IntAdder_38_F400_uid87 is
signal Cin_1, Cin_1_d1 :  std_logic;
signal X_1, X_1_d1 :  std_logic_vector(31 downto 0);
signal Y_1, Y_1_d1 :  std_logic_vector(31 downto 0);
signal S_1 :  std_logic_vector(31 downto 0);
signal R_1, R_1_d1 :  std_logic_vector(30 downto 0);
signal Cin_2, Cin_2_d1 :  std_logic;
signal X_2, X_2_d1, X_2_d2 :  std_logic_vector(7 downto 0);
signal Y_2, Y_2_d1, Y_2_d2 :  std_logic_vector(7 downto 0);
signal S_2 :  std_logic_vector(7 downto 0);
signal R_2 :  std_logic_vector(6 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            Cin_1_d1 <=  Cin_1;
            X_1_d1 <=  X_1;
            Y_1_d1 <=  Y_1;
            R_1_d1 <=  R_1;
            Cin_2_d1 <=  Cin_2;
            X_2_d1 <=  X_2;
            X_2_d2 <=  X_2_d1;
            Y_2_d1 <=  Y_2;
            Y_2_d2 <=  Y_2_d1;
         end if;
      end process;
   Cin_1 <= Cin;
   X_1 <= '0' & X(30 downto 0);
   Y_1 <= '0' & Y(30 downto 0);
   S_1 <= X_1_d1 + Y_1_d1 + Cin_1_d1;
   R_1 <= S_1(30 downto 0);
   Cin_2 <= S_1(31);
   X_2 <= '0' & X(37 downto 31);
   Y_2 <= '0' & Y(37 downto 31);
   S_2 <= X_2_d2 + Y_2_d2 + Cin_2_d1;
   R_2 <= S_2(6 downto 0);
   R <= R_2 & R_1_d1 ;
end architecture;

--------------------------------------------------------------------------------
--                          IntMultiplier_F400_uid2
-- VHDL generated for Zynq7000 @ 400MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved
-- Authors: Martin Kumm, Florent de Dinechin, Kinga Illyes, Bogdan Popa, Bogdan Pasca, 2012
--------------------------------------------------------------------------------
-- Pipeline depth: 2 cycles
-- Clock period (ns): 2.5
-- Target frequency (MHz): 400
-- Input signals: X Y
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library std;
use std.textio.all;
library work;

entity IntMultiplier_F400_uid2 is
    port (clk : in std_logic;
          X : in  std_logic_vector(33 downto 0);
          Y : in  std_logic_vector(33 downto 0);
          R : out  std_logic_vector(33 downto 0)   );
end entity;

architecture arch of IntMultiplier_F400_uid2 is
   component DSPBlock_17x24_F400_uid6 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(16 downto 0);
             Y : in  std_logic_vector(23 downto 0);
             R : out  std_logic_vector(40 downto 0)   );
   end component;

   component DSPBlock_12x12_F400_uid8 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(11 downto 0);
             Y : in  std_logic_vector(11 downto 0);
             R : out  std_logic_vector(23 downto 0)   );
   end component;

   component DSPBlock_17x10_F400_uid10 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(16 downto 0);
             Y : in  std_logic_vector(9 downto 0);
             R : out  std_logic_vector(26 downto 0)   );
   end component;

   component DSPBlock_17x10_F400_uid12 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(16 downto 0);
             Y : in  std_logic_vector(9 downto 0);
             R : out  std_logic_vector(26 downto 0)   );
   end component;

   component Compressor_23_3_F400_uid15 is
      port ( X1 : in  std_logic_vector(1 downto 0);
             X0 : in  std_logic_vector(2 downto 0);
             R : out  std_logic_vector(2 downto 0)   );
   end component;

   component Compressor_3_2_F400_uid21 is
      port ( X0 : in  std_logic_vector(2 downto 0);
             R : out  std_logic_vector(1 downto 0)   );
   end component;

   component Compressor_14_3_F400_uid25 is
      port ( X1 : in  std_logic_vector(0 downto 0);
             X0 : in  std_logic_vector(3 downto 0);
             R : out  std_logic_vector(2 downto 0)   );
   end component;

   component IntAdder_38_F400_uid87 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(37 downto 0);
             Y : in  std_logic_vector(37 downto 0);
             Cin : in  std_logic;
             R : out  std_logic_vector(37 downto 0)   );
   end component;

signal XX_m3 :  std_logic_vector(33 downto 0);
signal YY_m3 :  std_logic_vector(33 downto 0);
signal tile_0_X :  std_logic_vector(16 downto 0);
signal tile_0_Y :  std_logic_vector(23 downto 0);
signal tile_0_output :  std_logic_vector(40 downto 0);
signal tile_0_filtered_output :  std_logic_vector(29 downto 0);
signal bh4_w0_0 :  std_logic;
signal bh4_w1_0 :  std_logic;
signal bh4_w2_0 :  std_logic;
signal bh4_w3_0 :  std_logic;
signal bh4_w4_0 :  std_logic;
signal bh4_w5_0 :  std_logic;
signal bh4_w6_0 :  std_logic;
signal bh4_w7_0 :  std_logic;
signal bh4_w8_0 :  std_logic;
signal bh4_w9_0 :  std_logic;
signal bh4_w10_0 :  std_logic;
signal bh4_w11_0 :  std_logic;
signal bh4_w12_0 :  std_logic;
signal bh4_w13_0 :  std_logic;
signal bh4_w14_0 :  std_logic;
signal bh4_w15_0 :  std_logic;
signal bh4_w16_0 :  std_logic;
signal bh4_w17_0 :  std_logic;
signal bh4_w18_0 :  std_logic;
signal bh4_w19_0 :  std_logic;
signal bh4_w20_0 :  std_logic;
signal bh4_w21_0 :  std_logic;
signal bh4_w22_0 :  std_logic;
signal bh4_w23_0 :  std_logic;
signal bh4_w24_0 :  std_logic;
signal bh4_w25_0 :  std_logic;
signal bh4_w26_0 :  std_logic;
signal bh4_w27_0 :  std_logic;
signal bh4_w28_0 :  std_logic;
signal bh4_w29_0 :  std_logic;
signal tile_1_X :  std_logic_vector(11 downto 0);
signal tile_1_Y :  std_logic_vector(11 downto 0);
signal tile_1_output :  std_logic_vector(23 downto 0);
signal tile_1_filtered_output :  std_logic_vector(12 downto 0);
signal bh4_w0_1 :  std_logic;
signal bh4_w1_1 :  std_logic;
signal bh4_w2_1 :  std_logic;
signal bh4_w3_1 :  std_logic;
signal bh4_w4_1 :  std_logic;
signal bh4_w5_1 :  std_logic;
signal bh4_w6_1 :  std_logic;
signal bh4_w7_1 :  std_logic;
signal bh4_w8_1 :  std_logic;
signal bh4_w9_1 :  std_logic;
signal bh4_w10_1 :  std_logic;
signal bh4_w11_1 :  std_logic;
signal bh4_w12_1 :  std_logic;
signal tile_2_X :  std_logic_vector(16 downto 0);
signal tile_2_Y :  std_logic_vector(9 downto 0);
signal tile_2_output :  std_logic_vector(26 downto 0);
signal tile_2_filtered_output :  std_logic_vector(26 downto 0);
signal bh4_w13_1 :  std_logic;
signal bh4_w14_1 :  std_logic;
signal bh4_w15_1 :  std_logic;
signal bh4_w16_1 :  std_logic;
signal bh4_w17_1 :  std_logic;
signal bh4_w18_1 :  std_logic;
signal bh4_w19_1 :  std_logic;
signal bh4_w20_1 :  std_logic;
signal bh4_w21_1 :  std_logic;
signal bh4_w22_1 :  std_logic;
signal bh4_w23_1 :  std_logic;
signal bh4_w24_1 :  std_logic;
signal bh4_w25_1 :  std_logic;
signal bh4_w26_1 :  std_logic;
signal bh4_w27_1 :  std_logic;
signal bh4_w28_1 :  std_logic;
signal bh4_w29_1 :  std_logic;
signal bh4_w30_0 :  std_logic;
signal bh4_w31_0 :  std_logic;
signal bh4_w32_0 :  std_logic;
signal bh4_w33_0 :  std_logic;
signal bh4_w34_0 :  std_logic;
signal bh4_w35_0 :  std_logic;
signal bh4_w36_0 :  std_logic;
signal bh4_w37_0 :  std_logic;
signal bh4_w38_0 :  std_logic;
signal bh4_w39_0 :  std_logic;
signal tile_3_X :  std_logic_vector(16 downto 0);
signal tile_3_Y :  std_logic_vector(9 downto 0);
signal tile_3_output :  std_logic_vector(26 downto 0);
signal tile_3_filtered_output :  std_logic_vector(22 downto 0);
signal bh4_w0_2 :  std_logic;
signal bh4_w1_2 :  std_logic;
signal bh4_w2_2 :  std_logic;
signal bh4_w3_2 :  std_logic;
signal bh4_w4_2 :  std_logic;
signal bh4_w5_2 :  std_logic;
signal bh4_w6_2 :  std_logic;
signal bh4_w7_2 :  std_logic;
signal bh4_w8_2 :  std_logic;
signal bh4_w9_2 :  std_logic;
signal bh4_w10_2 :  std_logic;
signal bh4_w11_2 :  std_logic;
signal bh4_w12_2 :  std_logic;
signal bh4_w13_2 :  std_logic;
signal bh4_w14_2 :  std_logic;
signal bh4_w15_2 :  std_logic;
signal bh4_w16_2 :  std_logic;
signal bh4_w17_2 :  std_logic;
signal bh4_w18_2 :  std_logic;
signal bh4_w19_2 :  std_logic;
signal bh4_w20_2 :  std_logic;
signal bh4_w21_2 :  std_logic;
signal bh4_w22_2 :  std_logic;
signal bh4_w5_3 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid16_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid16_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid16_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid16_Out0_copy17 :  std_logic_vector(2 downto 0);
signal bh4_w0_3 :  std_logic;
signal bh4_w1_3 :  std_logic;
signal bh4_w2_3 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid18_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid18_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid18_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid18_Out0_copy19 :  std_logic_vector(2 downto 0);
signal bh4_w2_4 :  std_logic;
signal bh4_w3_3 :  std_logic;
signal bh4_w4_3 :  std_logic;
signal Compressor_3_2_F400_uid21_bh4_uid22_In0 :  std_logic_vector(2 downto 0);
signal Compressor_3_2_F400_uid21_bh4_uid22_Out0 :  std_logic_vector(1 downto 0);
signal Compressor_3_2_F400_uid21_bh4_uid22_Out0_copy23 :  std_logic_vector(1 downto 0);
signal bh4_w4_4 :  std_logic;
signal bh4_w5_4 :  std_logic;
signal Compressor_14_3_F400_uid25_bh4_uid26_In0 :  std_logic_vector(3 downto 0);
signal Compressor_14_3_F400_uid25_bh4_uid26_In1 :  std_logic_vector(0 downto 0);
signal Compressor_14_3_F400_uid25_bh4_uid26_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_14_3_F400_uid25_bh4_uid26_Out0_copy27 :  std_logic_vector(2 downto 0);
signal bh4_w5_5 :  std_logic;
signal bh4_w6_3 :  std_logic;
signal bh4_w7_3 :  std_logic;
signal Compressor_3_2_F400_uid21_bh4_uid28_In0 :  std_logic_vector(2 downto 0);
signal Compressor_3_2_F400_uid21_bh4_uid28_Out0 :  std_logic_vector(1 downto 0);
signal Compressor_3_2_F400_uid21_bh4_uid28_Out0_copy29 :  std_logic_vector(1 downto 0);
signal bh4_w6_4 :  std_logic;
signal bh4_w7_4 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid30_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid30_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid30_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid30_Out0_copy31 :  std_logic_vector(2 downto 0);
signal bh4_w7_5 :  std_logic;
signal bh4_w8_3 :  std_logic;
signal bh4_w9_3 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid32_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid32_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid32_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid32_Out0_copy33 :  std_logic_vector(2 downto 0);
signal bh4_w9_4 :  std_logic;
signal bh4_w10_3 :  std_logic;
signal bh4_w11_3 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid34_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid34_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid34_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid34_Out0_copy35 :  std_logic_vector(2 downto 0);
signal bh4_w11_4 :  std_logic;
signal bh4_w12_3 :  std_logic;
signal bh4_w13_3 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid36_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid36_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid36_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid36_Out0_copy37 :  std_logic_vector(2 downto 0);
signal bh4_w13_4 :  std_logic;
signal bh4_w14_3 :  std_logic;
signal bh4_w15_3 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid38_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid38_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid38_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid38_Out0_copy39 :  std_logic_vector(2 downto 0);
signal bh4_w15_4 :  std_logic;
signal bh4_w16_3 :  std_logic;
signal bh4_w17_3 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid40_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid40_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid40_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid40_Out0_copy41 :  std_logic_vector(2 downto 0);
signal bh4_w17_4 :  std_logic;
signal bh4_w18_3 :  std_logic;
signal bh4_w19_3 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid42_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid42_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid42_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid42_Out0_copy43 :  std_logic_vector(2 downto 0);
signal bh4_w19_4 :  std_logic;
signal bh4_w20_3 :  std_logic;
signal bh4_w21_3 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid44_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid44_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid44_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid44_Out0_copy45 :  std_logic_vector(2 downto 0);
signal bh4_w21_4 :  std_logic;
signal bh4_w22_3 :  std_logic;
signal bh4_w23_2 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid46_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid46_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid46_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid46_Out0_copy47 :  std_logic_vector(2 downto 0);
signal bh4_w23_3 :  std_logic;
signal bh4_w24_2 :  std_logic;
signal bh4_w25_2 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid48_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid48_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid48_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid48_Out0_copy49 :  std_logic_vector(2 downto 0);
signal bh4_w25_3 :  std_logic;
signal bh4_w26_2 :  std_logic;
signal bh4_w27_2 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid50_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid50_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid50_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid50_Out0_copy51 :  std_logic_vector(2 downto 0);
signal bh4_w27_3 :  std_logic;
signal bh4_w28_2 :  std_logic;
signal bh4_w29_2 :  std_logic;
signal Compressor_14_3_F400_uid25_bh4_uid52_In0 :  std_logic_vector(3 downto 0);
signal Compressor_14_3_F400_uid25_bh4_uid52_In1 :  std_logic_vector(0 downto 0);
signal Compressor_14_3_F400_uid25_bh4_uid52_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_14_3_F400_uid25_bh4_uid52_Out0_copy53 :  std_logic_vector(2 downto 0);
signal bh4_w29_3 :  std_logic;
signal bh4_w30_1 :  std_logic;
signal bh4_w31_1 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid54_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid54_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid54_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid54_Out0_copy55 :  std_logic_vector(2 downto 0);
signal bh4_w1_4 :  std_logic;
signal bh4_w2_5 :  std_logic;
signal bh4_w3_4 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid56_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid56_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid56_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid56_Out0_copy57 :  std_logic_vector(2 downto 0);
signal bh4_w3_5 :  std_logic;
signal bh4_w4_5 :  std_logic;
signal bh4_w5_6 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid58_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid58_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid58_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid58_Out0_copy59 :  std_logic_vector(2 downto 0);
signal bh4_w5_7 :  std_logic;
signal bh4_w6_5 :  std_logic;
signal bh4_w7_6 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid60_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid60_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid60_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid60_Out0_copy61 :  std_logic_vector(2 downto 0);
signal bh4_w7_7 :  std_logic;
signal bh4_w8_4 :  std_logic;
signal bh4_w9_5 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid62_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid62_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid62_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid62_Out0_copy63 :  std_logic_vector(2 downto 0);
signal bh4_w9_6 :  std_logic;
signal bh4_w10_4 :  std_logic;
signal bh4_w11_5 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid64_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid64_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid64_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid64_Out0_copy65 :  std_logic_vector(2 downto 0);
signal bh4_w11_6 :  std_logic;
signal bh4_w12_4 :  std_logic;
signal bh4_w13_5 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid66_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid66_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid66_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid66_Out0_copy67 :  std_logic_vector(2 downto 0);
signal bh4_w13_6 :  std_logic;
signal bh4_w14_4 :  std_logic;
signal bh4_w15_5 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid68_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid68_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid68_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid68_Out0_copy69 :  std_logic_vector(2 downto 0);
signal bh4_w15_6 :  std_logic;
signal bh4_w16_4 :  std_logic;
signal bh4_w17_5 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid70_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid70_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid70_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid70_Out0_copy71 :  std_logic_vector(2 downto 0);
signal bh4_w17_6 :  std_logic;
signal bh4_w18_4 :  std_logic;
signal bh4_w19_5 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid72_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid72_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid72_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid72_Out0_copy73 :  std_logic_vector(2 downto 0);
signal bh4_w19_6 :  std_logic;
signal bh4_w20_4 :  std_logic;
signal bh4_w21_5 :  std_logic;
signal Compressor_23_3_F400_uid15_bh4_uid74_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid74_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid74_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F400_uid15_bh4_uid74_Out0_copy75 :  std_logic_vector(2 downto 0);
signal bh4_w21_6 :  std_logic;
signal bh4_w22_4 :  std_logic;
signal bh4_w23_4 :  std_logic;
signal Compressor_14_3_F400_uid25_bh4_uid76_In0 :  std_logic_vector(3 downto 0);
signal Compressor_14_3_F400_uid25_bh4_uid76_In1 :  std_logic_vector(0 downto 0);
signal Compressor_14_3_F400_uid25_bh4_uid76_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_14_3_F400_uid25_bh4_uid76_Out0_copy77 :  std_logic_vector(2 downto 0);
signal bh4_w23_5 :  std_logic;
signal bh4_w24_3 :  std_logic;
signal bh4_w25_4 :  std_logic;
signal Compressor_14_3_F400_uid25_bh4_uid78_In0 :  std_logic_vector(3 downto 0);
signal Compressor_14_3_F400_uid25_bh4_uid78_In1 :  std_logic_vector(0 downto 0);
signal Compressor_14_3_F400_uid25_bh4_uid78_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_14_3_F400_uid25_bh4_uid78_Out0_copy79 :  std_logic_vector(2 downto 0);
signal bh4_w25_5 :  std_logic;
signal bh4_w26_3 :  std_logic;
signal bh4_w27_4 :  std_logic;
signal Compressor_14_3_F400_uid25_bh4_uid80_In0 :  std_logic_vector(3 downto 0);
signal Compressor_14_3_F400_uid25_bh4_uid80_In1 :  std_logic_vector(0 downto 0);
signal Compressor_14_3_F400_uid25_bh4_uid80_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_14_3_F400_uid25_bh4_uid80_Out0_copy81 :  std_logic_vector(2 downto 0);
signal bh4_w27_5 :  std_logic;
signal bh4_w28_3 :  std_logic;
signal bh4_w29_4 :  std_logic;
signal Compressor_14_3_F400_uid25_bh4_uid82_In0 :  std_logic_vector(3 downto 0);
signal Compressor_14_3_F400_uid25_bh4_uid82_In1 :  std_logic_vector(0 downto 0);
signal Compressor_14_3_F400_uid25_bh4_uid82_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_14_3_F400_uid25_bh4_uid82_Out0_copy83 :  std_logic_vector(2 downto 0);
signal bh4_w29_5 :  std_logic;
signal bh4_w30_2 :  std_logic;
signal bh4_w31_2 :  std_logic;
signal Compressor_14_3_F400_uid25_bh4_uid84_In0 :  std_logic_vector(3 downto 0);
signal Compressor_14_3_F400_uid25_bh4_uid84_In1 :  std_logic_vector(0 downto 0);
signal Compressor_14_3_F400_uid25_bh4_uid84_Out0 :  std_logic_vector(2 downto 0);
signal Compressor_14_3_F400_uid25_bh4_uid84_Out0_copy85 :  std_logic_vector(2 downto 0);
signal bh4_w31_3 :  std_logic;
signal bh4_w32_1 :  std_logic;
signal bh4_w33_1 :  std_logic;
signal tmp_bitheapResult_bh4_2, tmp_bitheapResult_bh4_2_d1, tmp_bitheapResult_bh4_2_d2 :  std_logic_vector(2 downto 0);
signal bitheapFinalAdd_bh4_In0 :  std_logic_vector(37 downto 0);
signal bitheapFinalAdd_bh4_In1 :  std_logic_vector(37 downto 0);
signal bitheapFinalAdd_bh4_Cin :  std_logic;
signal bitheapFinalAdd_bh4_Out :  std_logic_vector(37 downto 0);
signal bitheapResult_bh4 :  std_logic_vector(39 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            tmp_bitheapResult_bh4_2_d1 <=  tmp_bitheapResult_bh4_2;
            tmp_bitheapResult_bh4_2_d2 <=  tmp_bitheapResult_bh4_2_d1;
         end if;
      end process;
   XX_m3 <= X ;
   YY_m3 <= Y ;
   tile_0_X <= X(33 downto 17);
   tile_0_Y <= Y(23 downto 0);
   tile_0_mult: DSPBlock_17x24_F400_uid6
      port map ( clk  => clk,
                 X => tile_0_X,
                 Y => tile_0_Y,
                 R => tile_0_output);

tile_0_filtered_output <= tile_0_output(40 downto 11);
   bh4_w0_0 <= tile_0_filtered_output(0);
   bh4_w1_0 <= tile_0_filtered_output(1);
   bh4_w2_0 <= tile_0_filtered_output(2);
   bh4_w3_0 <= tile_0_filtered_output(3);
   bh4_w4_0 <= tile_0_filtered_output(4);
   bh4_w5_0 <= tile_0_filtered_output(5);
   bh4_w6_0 <= tile_0_filtered_output(6);
   bh4_w7_0 <= tile_0_filtered_output(7);
   bh4_w8_0 <= tile_0_filtered_output(8);
   bh4_w9_0 <= tile_0_filtered_output(9);
   bh4_w10_0 <= tile_0_filtered_output(10);
   bh4_w11_0 <= tile_0_filtered_output(11);
   bh4_w12_0 <= tile_0_filtered_output(12);
   bh4_w13_0 <= tile_0_filtered_output(13);
   bh4_w14_0 <= tile_0_filtered_output(14);
   bh4_w15_0 <= tile_0_filtered_output(15);
   bh4_w16_0 <= tile_0_filtered_output(16);
   bh4_w17_0 <= tile_0_filtered_output(17);
   bh4_w18_0 <= tile_0_filtered_output(18);
   bh4_w19_0 <= tile_0_filtered_output(19);
   bh4_w20_0 <= tile_0_filtered_output(20);
   bh4_w21_0 <= tile_0_filtered_output(21);
   bh4_w22_0 <= tile_0_filtered_output(22);
   bh4_w23_0 <= tile_0_filtered_output(23);
   bh4_w24_0 <= tile_0_filtered_output(24);
   bh4_w25_0 <= tile_0_filtered_output(25);
   bh4_w26_0 <= tile_0_filtered_output(26);
   bh4_w27_0 <= tile_0_filtered_output(27);
   bh4_w28_0 <= tile_0_filtered_output(28);
   bh4_w29_0 <= tile_0_filtered_output(29);
   tile_1_X <= X(16 downto 5);
   tile_1_Y <= Y(23 downto 12);
   tile_1_mult: DSPBlock_12x12_F400_uid8
      port map ( clk  => clk,
                 X => tile_1_X,
                 Y => tile_1_Y,
                 R => tile_1_output);

tile_1_filtered_output <= tile_1_output(23 downto 11);
   bh4_w0_1 <= tile_1_filtered_output(0);
   bh4_w1_1 <= tile_1_filtered_output(1);
   bh4_w2_1 <= tile_1_filtered_output(2);
   bh4_w3_1 <= tile_1_filtered_output(3);
   bh4_w4_1 <= tile_1_filtered_output(4);
   bh4_w5_1 <= tile_1_filtered_output(5);
   bh4_w6_1 <= tile_1_filtered_output(6);
   bh4_w7_1 <= tile_1_filtered_output(7);
   bh4_w8_1 <= tile_1_filtered_output(8);
   bh4_w9_1 <= tile_1_filtered_output(9);
   bh4_w10_1 <= tile_1_filtered_output(10);
   bh4_w11_1 <= tile_1_filtered_output(11);
   bh4_w12_1 <= tile_1_filtered_output(12);
   tile_2_X <= X(33 downto 17);
   tile_2_Y <= Y(33 downto 24);
   tile_2_mult: DSPBlock_17x10_F400_uid10
      port map ( clk  => clk,
                 X => tile_2_X,
                 Y => tile_2_Y,
                 R => tile_2_output);

tile_2_filtered_output <= tile_2_output(26 downto 0);
   bh4_w13_1 <= tile_2_filtered_output(0);
   bh4_w14_1 <= tile_2_filtered_output(1);
   bh4_w15_1 <= tile_2_filtered_output(2);
   bh4_w16_1 <= tile_2_filtered_output(3);
   bh4_w17_1 <= tile_2_filtered_output(4);
   bh4_w18_1 <= tile_2_filtered_output(5);
   bh4_w19_1 <= tile_2_filtered_output(6);
   bh4_w20_1 <= tile_2_filtered_output(7);
   bh4_w21_1 <= tile_2_filtered_output(8);
   bh4_w22_1 <= tile_2_filtered_output(9);
   bh4_w23_1 <= tile_2_filtered_output(10);
   bh4_w24_1 <= tile_2_filtered_output(11);
   bh4_w25_1 <= tile_2_filtered_output(12);
   bh4_w26_1 <= tile_2_filtered_output(13);
   bh4_w27_1 <= tile_2_filtered_output(14);
   bh4_w28_1 <= tile_2_filtered_output(15);
   bh4_w29_1 <= tile_2_filtered_output(16);
   bh4_w30_0 <= tile_2_filtered_output(17);
   bh4_w31_0 <= tile_2_filtered_output(18);
   bh4_w32_0 <= tile_2_filtered_output(19);
   bh4_w33_0 <= tile_2_filtered_output(20);
   bh4_w34_0 <= tile_2_filtered_output(21);
   bh4_w35_0 <= tile_2_filtered_output(22);
   bh4_w36_0 <= tile_2_filtered_output(23);
   bh4_w37_0 <= tile_2_filtered_output(24);
   bh4_w38_0 <= tile_2_filtered_output(25);
   bh4_w39_0 <= tile_2_filtered_output(26);
   tile_3_X <= X(16 downto 0);
   tile_3_Y <= Y(33 downto 24);
   tile_3_mult: DSPBlock_17x10_F400_uid12
      port map ( clk  => clk,
                 X => tile_3_X,
                 Y => tile_3_Y,
                 R => tile_3_output);

tile_3_filtered_output <= tile_3_output(26 downto 4);
   bh4_w0_2 <= tile_3_filtered_output(0);
   bh4_w1_2 <= tile_3_filtered_output(1);
   bh4_w2_2 <= tile_3_filtered_output(2);
   bh4_w3_2 <= tile_3_filtered_output(3);
   bh4_w4_2 <= tile_3_filtered_output(4);
   bh4_w5_2 <= tile_3_filtered_output(5);
   bh4_w6_2 <= tile_3_filtered_output(6);
   bh4_w7_2 <= tile_3_filtered_output(7);
   bh4_w8_2 <= tile_3_filtered_output(8);
   bh4_w9_2 <= tile_3_filtered_output(9);
   bh4_w10_2 <= tile_3_filtered_output(10);
   bh4_w11_2 <= tile_3_filtered_output(11);
   bh4_w12_2 <= tile_3_filtered_output(12);
   bh4_w13_2 <= tile_3_filtered_output(13);
   bh4_w14_2 <= tile_3_filtered_output(14);
   bh4_w15_2 <= tile_3_filtered_output(15);
   bh4_w16_2 <= tile_3_filtered_output(16);
   bh4_w17_2 <= tile_3_filtered_output(17);
   bh4_w18_2 <= tile_3_filtered_output(18);
   bh4_w19_2 <= tile_3_filtered_output(19);
   bh4_w20_2 <= tile_3_filtered_output(20);
   bh4_w21_2 <= tile_3_filtered_output(21);
   bh4_w22_2 <= tile_3_filtered_output(22);

   -- Adding the constant bits
   bh4_w5_3 <= '1';


   Compressor_23_3_F400_uid15_bh4_uid16_In0 <= "" & bh4_w0_0 & bh4_w0_1 & bh4_w0_2;
   Compressor_23_3_F400_uid15_bh4_uid16_In1 <= "" & bh4_w1_0 & bh4_w1_1;
   Compressor_23_3_F400_uid15_uid16: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid16_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid16_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid16_Out0_copy17);
   Compressor_23_3_F400_uid15_bh4_uid16_Out0 <= Compressor_23_3_F400_uid15_bh4_uid16_Out0_copy17; -- output copy to hold a pipeline register if needed

   bh4_w0_3 <= Compressor_23_3_F400_uid15_bh4_uid16_Out0(0);
   bh4_w1_3 <= Compressor_23_3_F400_uid15_bh4_uid16_Out0(1);
   bh4_w2_3 <= Compressor_23_3_F400_uid15_bh4_uid16_Out0(2);

   Compressor_23_3_F400_uid15_bh4_uid18_In0 <= "" & bh4_w2_0 & bh4_w2_1 & bh4_w2_2;
   Compressor_23_3_F400_uid15_bh4_uid18_In1 <= "" & bh4_w3_0 & bh4_w3_1;
   Compressor_23_3_F400_uid15_uid18: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid18_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid18_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid18_Out0_copy19);
   Compressor_23_3_F400_uid15_bh4_uid18_Out0 <= Compressor_23_3_F400_uid15_bh4_uid18_Out0_copy19; -- output copy to hold a pipeline register if needed

   bh4_w2_4 <= Compressor_23_3_F400_uid15_bh4_uid18_Out0(0);
   bh4_w3_3 <= Compressor_23_3_F400_uid15_bh4_uid18_Out0(1);
   bh4_w4_3 <= Compressor_23_3_F400_uid15_bh4_uid18_Out0(2);

   Compressor_3_2_F400_uid21_bh4_uid22_In0 <= "" & bh4_w4_0 & bh4_w4_1 & bh4_w4_2;
   Compressor_3_2_F400_uid21_uid22: Compressor_3_2_F400_uid21
      port map ( X0 => Compressor_3_2_F400_uid21_bh4_uid22_In0,
                 R => Compressor_3_2_F400_uid21_bh4_uid22_Out0_copy23);
   Compressor_3_2_F400_uid21_bh4_uid22_Out0 <= Compressor_3_2_F400_uid21_bh4_uid22_Out0_copy23; -- output copy to hold a pipeline register if needed

   bh4_w4_4 <= Compressor_3_2_F400_uid21_bh4_uid22_Out0(0);
   bh4_w5_4 <= Compressor_3_2_F400_uid21_bh4_uid22_Out0(1);

   Compressor_14_3_F400_uid25_bh4_uid26_In0 <= "" & bh4_w5_0 & bh4_w5_1 & bh4_w5_2 & bh4_w5_3;
   Compressor_14_3_F400_uid25_bh4_uid26_In1 <= "" & "0";
   Compressor_14_3_F400_uid25_uid26: Compressor_14_3_F400_uid25
      port map ( X0 => Compressor_14_3_F400_uid25_bh4_uid26_In0,
                 X1 => Compressor_14_3_F400_uid25_bh4_uid26_In1,
                 R => Compressor_14_3_F400_uid25_bh4_uid26_Out0_copy27);
   Compressor_14_3_F400_uid25_bh4_uid26_Out0 <= Compressor_14_3_F400_uid25_bh4_uid26_Out0_copy27; -- output copy to hold a pipeline register if needed

   bh4_w5_5 <= Compressor_14_3_F400_uid25_bh4_uid26_Out0(0);
   bh4_w6_3 <= Compressor_14_3_F400_uid25_bh4_uid26_Out0(1);
   bh4_w7_3 <= Compressor_14_3_F400_uid25_bh4_uid26_Out0(2);

   Compressor_3_2_F400_uid21_bh4_uid28_In0 <= "" & bh4_w6_0 & bh4_w6_1 & bh4_w6_2;
   Compressor_3_2_F400_uid21_uid28: Compressor_3_2_F400_uid21
      port map ( X0 => Compressor_3_2_F400_uid21_bh4_uid28_In0,
                 R => Compressor_3_2_F400_uid21_bh4_uid28_Out0_copy29);
   Compressor_3_2_F400_uid21_bh4_uid28_Out0 <= Compressor_3_2_F400_uid21_bh4_uid28_Out0_copy29; -- output copy to hold a pipeline register if needed

   bh4_w6_4 <= Compressor_3_2_F400_uid21_bh4_uid28_Out0(0);
   bh4_w7_4 <= Compressor_3_2_F400_uid21_bh4_uid28_Out0(1);

   Compressor_23_3_F400_uid15_bh4_uid30_In0 <= "" & bh4_w7_0 & bh4_w7_1 & bh4_w7_2;
   Compressor_23_3_F400_uid15_bh4_uid30_In1 <= "" & bh4_w8_0 & bh4_w8_1;
   Compressor_23_3_F400_uid15_uid30: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid30_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid30_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid30_Out0_copy31);
   Compressor_23_3_F400_uid15_bh4_uid30_Out0 <= Compressor_23_3_F400_uid15_bh4_uid30_Out0_copy31; -- output copy to hold a pipeline register if needed

   bh4_w7_5 <= Compressor_23_3_F400_uid15_bh4_uid30_Out0(0);
   bh4_w8_3 <= Compressor_23_3_F400_uid15_bh4_uid30_Out0(1);
   bh4_w9_3 <= Compressor_23_3_F400_uid15_bh4_uid30_Out0(2);

   Compressor_23_3_F400_uid15_bh4_uid32_In0 <= "" & bh4_w9_0 & bh4_w9_1 & bh4_w9_2;
   Compressor_23_3_F400_uid15_bh4_uid32_In1 <= "" & bh4_w10_0 & bh4_w10_1;
   Compressor_23_3_F400_uid15_uid32: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid32_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid32_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid32_Out0_copy33);
   Compressor_23_3_F400_uid15_bh4_uid32_Out0 <= Compressor_23_3_F400_uid15_bh4_uid32_Out0_copy33; -- output copy to hold a pipeline register if needed

   bh4_w9_4 <= Compressor_23_3_F400_uid15_bh4_uid32_Out0(0);
   bh4_w10_3 <= Compressor_23_3_F400_uid15_bh4_uid32_Out0(1);
   bh4_w11_3 <= Compressor_23_3_F400_uid15_bh4_uid32_Out0(2);

   Compressor_23_3_F400_uid15_bh4_uid34_In0 <= "" & bh4_w11_0 & bh4_w11_1 & bh4_w11_2;
   Compressor_23_3_F400_uid15_bh4_uid34_In1 <= "" & bh4_w12_0 & bh4_w12_1;
   Compressor_23_3_F400_uid15_uid34: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid34_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid34_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid34_Out0_copy35);
   Compressor_23_3_F400_uid15_bh4_uid34_Out0 <= Compressor_23_3_F400_uid15_bh4_uid34_Out0_copy35; -- output copy to hold a pipeline register if needed

   bh4_w11_4 <= Compressor_23_3_F400_uid15_bh4_uid34_Out0(0);
   bh4_w12_3 <= Compressor_23_3_F400_uid15_bh4_uid34_Out0(1);
   bh4_w13_3 <= Compressor_23_3_F400_uid15_bh4_uid34_Out0(2);

   Compressor_23_3_F400_uid15_bh4_uid36_In0 <= "" & bh4_w13_0 & bh4_w13_1 & bh4_w13_2;
   Compressor_23_3_F400_uid15_bh4_uid36_In1 <= "" & bh4_w14_0 & bh4_w14_1;
   Compressor_23_3_F400_uid15_uid36: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid36_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid36_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid36_Out0_copy37);
   Compressor_23_3_F400_uid15_bh4_uid36_Out0 <= Compressor_23_3_F400_uid15_bh4_uid36_Out0_copy37; -- output copy to hold a pipeline register if needed

   bh4_w13_4 <= Compressor_23_3_F400_uid15_bh4_uid36_Out0(0);
   bh4_w14_3 <= Compressor_23_3_F400_uid15_bh4_uid36_Out0(1);
   bh4_w15_3 <= Compressor_23_3_F400_uid15_bh4_uid36_Out0(2);

   Compressor_23_3_F400_uid15_bh4_uid38_In0 <= "" & bh4_w15_0 & bh4_w15_1 & bh4_w15_2;
   Compressor_23_3_F400_uid15_bh4_uid38_In1 <= "" & bh4_w16_0 & bh4_w16_1;
   Compressor_23_3_F400_uid15_uid38: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid38_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid38_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid38_Out0_copy39);
   Compressor_23_3_F400_uid15_bh4_uid38_Out0 <= Compressor_23_3_F400_uid15_bh4_uid38_Out0_copy39; -- output copy to hold a pipeline register if needed

   bh4_w15_4 <= Compressor_23_3_F400_uid15_bh4_uid38_Out0(0);
   bh4_w16_3 <= Compressor_23_3_F400_uid15_bh4_uid38_Out0(1);
   bh4_w17_3 <= Compressor_23_3_F400_uid15_bh4_uid38_Out0(2);

   Compressor_23_3_F400_uid15_bh4_uid40_In0 <= "" & bh4_w17_0 & bh4_w17_1 & bh4_w17_2;
   Compressor_23_3_F400_uid15_bh4_uid40_In1 <= "" & bh4_w18_0 & bh4_w18_1;
   Compressor_23_3_F400_uid15_uid40: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid40_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid40_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid40_Out0_copy41);
   Compressor_23_3_F400_uid15_bh4_uid40_Out0 <= Compressor_23_3_F400_uid15_bh4_uid40_Out0_copy41; -- output copy to hold a pipeline register if needed

   bh4_w17_4 <= Compressor_23_3_F400_uid15_bh4_uid40_Out0(0);
   bh4_w18_3 <= Compressor_23_3_F400_uid15_bh4_uid40_Out0(1);
   bh4_w19_3 <= Compressor_23_3_F400_uid15_bh4_uid40_Out0(2);

   Compressor_23_3_F400_uid15_bh4_uid42_In0 <= "" & bh4_w19_0 & bh4_w19_1 & bh4_w19_2;
   Compressor_23_3_F400_uid15_bh4_uid42_In1 <= "" & bh4_w20_0 & bh4_w20_1;
   Compressor_23_3_F400_uid15_uid42: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid42_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid42_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid42_Out0_copy43);
   Compressor_23_3_F400_uid15_bh4_uid42_Out0 <= Compressor_23_3_F400_uid15_bh4_uid42_Out0_copy43; -- output copy to hold a pipeline register if needed

   bh4_w19_4 <= Compressor_23_3_F400_uid15_bh4_uid42_Out0(0);
   bh4_w20_3 <= Compressor_23_3_F400_uid15_bh4_uid42_Out0(1);
   bh4_w21_3 <= Compressor_23_3_F400_uid15_bh4_uid42_Out0(2);

   Compressor_23_3_F400_uid15_bh4_uid44_In0 <= "" & bh4_w21_0 & bh4_w21_1 & bh4_w21_2;
   Compressor_23_3_F400_uid15_bh4_uid44_In1 <= "" & bh4_w22_0 & bh4_w22_1;
   Compressor_23_3_F400_uid15_uid44: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid44_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid44_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid44_Out0_copy45);
   Compressor_23_3_F400_uid15_bh4_uid44_Out0 <= Compressor_23_3_F400_uid15_bh4_uid44_Out0_copy45; -- output copy to hold a pipeline register if needed

   bh4_w21_4 <= Compressor_23_3_F400_uid15_bh4_uid44_Out0(0);
   bh4_w22_3 <= Compressor_23_3_F400_uid15_bh4_uid44_Out0(1);
   bh4_w23_2 <= Compressor_23_3_F400_uid15_bh4_uid44_Out0(2);

   Compressor_23_3_F400_uid15_bh4_uid46_In0 <= "" & bh4_w23_0 & bh4_w23_1 & "0";
   Compressor_23_3_F400_uid15_bh4_uid46_In1 <= "" & bh4_w24_0 & bh4_w24_1;
   Compressor_23_3_F400_uid15_uid46: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid46_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid46_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid46_Out0_copy47);
   Compressor_23_3_F400_uid15_bh4_uid46_Out0 <= Compressor_23_3_F400_uid15_bh4_uid46_Out0_copy47; -- output copy to hold a pipeline register if needed

   bh4_w23_3 <= Compressor_23_3_F400_uid15_bh4_uid46_Out0(0);
   bh4_w24_2 <= Compressor_23_3_F400_uid15_bh4_uid46_Out0(1);
   bh4_w25_2 <= Compressor_23_3_F400_uid15_bh4_uid46_Out0(2);

   Compressor_23_3_F400_uid15_bh4_uid48_In0 <= "" & bh4_w25_0 & bh4_w25_1 & "0";
   Compressor_23_3_F400_uid15_bh4_uid48_In1 <= "" & bh4_w26_0 & bh4_w26_1;
   Compressor_23_3_F400_uid15_uid48: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid48_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid48_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid48_Out0_copy49);
   Compressor_23_3_F400_uid15_bh4_uid48_Out0 <= Compressor_23_3_F400_uid15_bh4_uid48_Out0_copy49; -- output copy to hold a pipeline register if needed

   bh4_w25_3 <= Compressor_23_3_F400_uid15_bh4_uid48_Out0(0);
   bh4_w26_2 <= Compressor_23_3_F400_uid15_bh4_uid48_Out0(1);
   bh4_w27_2 <= Compressor_23_3_F400_uid15_bh4_uid48_Out0(2);

   Compressor_23_3_F400_uid15_bh4_uid50_In0 <= "" & bh4_w27_0 & bh4_w27_1 & "0";
   Compressor_23_3_F400_uid15_bh4_uid50_In1 <= "" & bh4_w28_0 & bh4_w28_1;
   Compressor_23_3_F400_uid15_uid50: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid50_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid50_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid50_Out0_copy51);
   Compressor_23_3_F400_uid15_bh4_uid50_Out0 <= Compressor_23_3_F400_uid15_bh4_uid50_Out0_copy51; -- output copy to hold a pipeline register if needed

   bh4_w27_3 <= Compressor_23_3_F400_uid15_bh4_uid50_Out0(0);
   bh4_w28_2 <= Compressor_23_3_F400_uid15_bh4_uid50_Out0(1);
   bh4_w29_2 <= Compressor_23_3_F400_uid15_bh4_uid50_Out0(2);

   Compressor_14_3_F400_uid25_bh4_uid52_In0 <= "" & bh4_w29_0 & bh4_w29_1 & "0" & "0";
   Compressor_14_3_F400_uid25_bh4_uid52_In1 <= "" & bh4_w30_0;
   Compressor_14_3_F400_uid25_uid52: Compressor_14_3_F400_uid25
      port map ( X0 => Compressor_14_3_F400_uid25_bh4_uid52_In0,
                 X1 => Compressor_14_3_F400_uid25_bh4_uid52_In1,
                 R => Compressor_14_3_F400_uid25_bh4_uid52_Out0_copy53);
   Compressor_14_3_F400_uid25_bh4_uid52_Out0 <= Compressor_14_3_F400_uid25_bh4_uid52_Out0_copy53; -- output copy to hold a pipeline register if needed

   bh4_w29_3 <= Compressor_14_3_F400_uid25_bh4_uid52_Out0(0);
   bh4_w30_1 <= Compressor_14_3_F400_uid25_bh4_uid52_Out0(1);
   bh4_w31_1 <= Compressor_14_3_F400_uid25_bh4_uid52_Out0(2);

   Compressor_23_3_F400_uid15_bh4_uid54_In0 <= "" & bh4_w1_2 & bh4_w1_3 & "0";
   Compressor_23_3_F400_uid15_bh4_uid54_In1 <= "" & bh4_w2_4 & bh4_w2_3;
   Compressor_23_3_F400_uid15_uid54: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid54_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid54_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid54_Out0_copy55);
   Compressor_23_3_F400_uid15_bh4_uid54_Out0 <= Compressor_23_3_F400_uid15_bh4_uid54_Out0_copy55; -- output copy to hold a pipeline register if needed

   bh4_w1_4 <= Compressor_23_3_F400_uid15_bh4_uid54_Out0(0);
   bh4_w2_5 <= Compressor_23_3_F400_uid15_bh4_uid54_Out0(1);
   bh4_w3_4 <= Compressor_23_3_F400_uid15_bh4_uid54_Out0(2);

   Compressor_23_3_F400_uid15_bh4_uid56_In0 <= "" & bh4_w3_2 & bh4_w3_3 & "0";
   Compressor_23_3_F400_uid15_bh4_uid56_In1 <= "" & bh4_w4_4 & bh4_w4_3;
   Compressor_23_3_F400_uid15_uid56: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid56_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid56_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid56_Out0_copy57);
   Compressor_23_3_F400_uid15_bh4_uid56_Out0 <= Compressor_23_3_F400_uid15_bh4_uid56_Out0_copy57; -- output copy to hold a pipeline register if needed

   bh4_w3_5 <= Compressor_23_3_F400_uid15_bh4_uid56_Out0(0);
   bh4_w4_5 <= Compressor_23_3_F400_uid15_bh4_uid56_Out0(1);
   bh4_w5_6 <= Compressor_23_3_F400_uid15_bh4_uid56_Out0(2);

   Compressor_23_3_F400_uid15_bh4_uid58_In0 <= "" & bh4_w5_5 & bh4_w5_4 & "0";
   Compressor_23_3_F400_uid15_bh4_uid58_In1 <= "" & bh4_w6_4 & bh4_w6_3;
   Compressor_23_3_F400_uid15_uid58: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid58_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid58_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid58_Out0_copy59);
   Compressor_23_3_F400_uid15_bh4_uid58_Out0 <= Compressor_23_3_F400_uid15_bh4_uid58_Out0_copy59; -- output copy to hold a pipeline register if needed

   bh4_w5_7 <= Compressor_23_3_F400_uid15_bh4_uid58_Out0(0);
   bh4_w6_5 <= Compressor_23_3_F400_uid15_bh4_uid58_Out0(1);
   bh4_w7_6 <= Compressor_23_3_F400_uid15_bh4_uid58_Out0(2);

   Compressor_23_3_F400_uid15_bh4_uid60_In0 <= "" & bh4_w7_5 & bh4_w7_4 & bh4_w7_3;
   Compressor_23_3_F400_uid15_bh4_uid60_In1 <= "" & bh4_w8_2 & bh4_w8_3;
   Compressor_23_3_F400_uid15_uid60: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid60_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid60_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid60_Out0_copy61);
   Compressor_23_3_F400_uid15_bh4_uid60_Out0 <= Compressor_23_3_F400_uid15_bh4_uid60_Out0_copy61; -- output copy to hold a pipeline register if needed

   bh4_w7_7 <= Compressor_23_3_F400_uid15_bh4_uid60_Out0(0);
   bh4_w8_4 <= Compressor_23_3_F400_uid15_bh4_uid60_Out0(1);
   bh4_w9_5 <= Compressor_23_3_F400_uid15_bh4_uid60_Out0(2);

   Compressor_23_3_F400_uid15_bh4_uid62_In0 <= "" & bh4_w9_4 & bh4_w9_3 & "0";
   Compressor_23_3_F400_uid15_bh4_uid62_In1 <= "" & bh4_w10_2 & bh4_w10_3;
   Compressor_23_3_F400_uid15_uid62: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid62_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid62_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid62_Out0_copy63);
   Compressor_23_3_F400_uid15_bh4_uid62_Out0 <= Compressor_23_3_F400_uid15_bh4_uid62_Out0_copy63; -- output copy to hold a pipeline register if needed

   bh4_w9_6 <= Compressor_23_3_F400_uid15_bh4_uid62_Out0(0);
   bh4_w10_4 <= Compressor_23_3_F400_uid15_bh4_uid62_Out0(1);
   bh4_w11_5 <= Compressor_23_3_F400_uid15_bh4_uid62_Out0(2);

   Compressor_23_3_F400_uid15_bh4_uid64_In0 <= "" & bh4_w11_4 & bh4_w11_3 & "0";
   Compressor_23_3_F400_uid15_bh4_uid64_In1 <= "" & bh4_w12_2 & bh4_w12_3;
   Compressor_23_3_F400_uid15_uid64: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid64_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid64_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid64_Out0_copy65);
   Compressor_23_3_F400_uid15_bh4_uid64_Out0 <= Compressor_23_3_F400_uid15_bh4_uid64_Out0_copy65; -- output copy to hold a pipeline register if needed

   bh4_w11_6 <= Compressor_23_3_F400_uid15_bh4_uid64_Out0(0);
   bh4_w12_4 <= Compressor_23_3_F400_uid15_bh4_uid64_Out0(1);
   bh4_w13_5 <= Compressor_23_3_F400_uid15_bh4_uid64_Out0(2);

   Compressor_23_3_F400_uid15_bh4_uid66_In0 <= "" & bh4_w13_4 & bh4_w13_3 & "0";
   Compressor_23_3_F400_uid15_bh4_uid66_In1 <= "" & bh4_w14_2 & bh4_w14_3;
   Compressor_23_3_F400_uid15_uid66: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid66_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid66_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid66_Out0_copy67);
   Compressor_23_3_F400_uid15_bh4_uid66_Out0 <= Compressor_23_3_F400_uid15_bh4_uid66_Out0_copy67; -- output copy to hold a pipeline register if needed

   bh4_w13_6 <= Compressor_23_3_F400_uid15_bh4_uid66_Out0(0);
   bh4_w14_4 <= Compressor_23_3_F400_uid15_bh4_uid66_Out0(1);
   bh4_w15_5 <= Compressor_23_3_F400_uid15_bh4_uid66_Out0(2);

   Compressor_23_3_F400_uid15_bh4_uid68_In0 <= "" & bh4_w15_4 & bh4_w15_3 & "0";
   Compressor_23_3_F400_uid15_bh4_uid68_In1 <= "" & bh4_w16_2 & bh4_w16_3;
   Compressor_23_3_F400_uid15_uid68: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid68_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid68_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid68_Out0_copy69);
   Compressor_23_3_F400_uid15_bh4_uid68_Out0 <= Compressor_23_3_F400_uid15_bh4_uid68_Out0_copy69; -- output copy to hold a pipeline register if needed

   bh4_w15_6 <= Compressor_23_3_F400_uid15_bh4_uid68_Out0(0);
   bh4_w16_4 <= Compressor_23_3_F400_uid15_bh4_uid68_Out0(1);
   bh4_w17_5 <= Compressor_23_3_F400_uid15_bh4_uid68_Out0(2);

   Compressor_23_3_F400_uid15_bh4_uid70_In0 <= "" & bh4_w17_4 & bh4_w17_3 & "0";
   Compressor_23_3_F400_uid15_bh4_uid70_In1 <= "" & bh4_w18_2 & bh4_w18_3;
   Compressor_23_3_F400_uid15_uid70: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid70_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid70_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid70_Out0_copy71);
   Compressor_23_3_F400_uid15_bh4_uid70_Out0 <= Compressor_23_3_F400_uid15_bh4_uid70_Out0_copy71; -- output copy to hold a pipeline register if needed

   bh4_w17_6 <= Compressor_23_3_F400_uid15_bh4_uid70_Out0(0);
   bh4_w18_4 <= Compressor_23_3_F400_uid15_bh4_uid70_Out0(1);
   bh4_w19_5 <= Compressor_23_3_F400_uid15_bh4_uid70_Out0(2);

   Compressor_23_3_F400_uid15_bh4_uid72_In0 <= "" & bh4_w19_4 & bh4_w19_3 & "0";
   Compressor_23_3_F400_uid15_bh4_uid72_In1 <= "" & bh4_w20_2 & bh4_w20_3;
   Compressor_23_3_F400_uid15_uid72: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid72_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid72_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid72_Out0_copy73);
   Compressor_23_3_F400_uid15_bh4_uid72_Out0 <= Compressor_23_3_F400_uid15_bh4_uid72_Out0_copy73; -- output copy to hold a pipeline register if needed

   bh4_w19_6 <= Compressor_23_3_F400_uid15_bh4_uid72_Out0(0);
   bh4_w20_4 <= Compressor_23_3_F400_uid15_bh4_uid72_Out0(1);
   bh4_w21_5 <= Compressor_23_3_F400_uid15_bh4_uid72_Out0(2);

   Compressor_23_3_F400_uid15_bh4_uid74_In0 <= "" & bh4_w21_4 & bh4_w21_3 & "0";
   Compressor_23_3_F400_uid15_bh4_uid74_In1 <= "" & bh4_w22_2 & bh4_w22_3;
   Compressor_23_3_F400_uid15_uid74: Compressor_23_3_F400_uid15
      port map ( X0 => Compressor_23_3_F400_uid15_bh4_uid74_In0,
                 X1 => Compressor_23_3_F400_uid15_bh4_uid74_In1,
                 R => Compressor_23_3_F400_uid15_bh4_uid74_Out0_copy75);
   Compressor_23_3_F400_uid15_bh4_uid74_Out0 <= Compressor_23_3_F400_uid15_bh4_uid74_Out0_copy75; -- output copy to hold a pipeline register if needed

   bh4_w21_6 <= Compressor_23_3_F400_uid15_bh4_uid74_Out0(0);
   bh4_w22_4 <= Compressor_23_3_F400_uid15_bh4_uid74_Out0(1);
   bh4_w23_4 <= Compressor_23_3_F400_uid15_bh4_uid74_Out0(2);

   Compressor_14_3_F400_uid25_bh4_uid76_In0 <= "" & bh4_w23_3 & bh4_w23_2 & "0" & "0";
   Compressor_14_3_F400_uid25_bh4_uid76_In1 <= "" & bh4_w24_2;
   Compressor_14_3_F400_uid25_uid76: Compressor_14_3_F400_uid25
      port map ( X0 => Compressor_14_3_F400_uid25_bh4_uid76_In0,
                 X1 => Compressor_14_3_F400_uid25_bh4_uid76_In1,
                 R => Compressor_14_3_F400_uid25_bh4_uid76_Out0_copy77);
   Compressor_14_3_F400_uid25_bh4_uid76_Out0 <= Compressor_14_3_F400_uid25_bh4_uid76_Out0_copy77; -- output copy to hold a pipeline register if needed

   bh4_w23_5 <= Compressor_14_3_F400_uid25_bh4_uid76_Out0(0);
   bh4_w24_3 <= Compressor_14_3_F400_uid25_bh4_uid76_Out0(1);
   bh4_w25_4 <= Compressor_14_3_F400_uid25_bh4_uid76_Out0(2);

   Compressor_14_3_F400_uid25_bh4_uid78_In0 <= "" & bh4_w25_3 & bh4_w25_2 & "0" & "0";
   Compressor_14_3_F400_uid25_bh4_uid78_In1 <= "" & bh4_w26_2;
   Compressor_14_3_F400_uid25_uid78: Compressor_14_3_F400_uid25
      port map ( X0 => Compressor_14_3_F400_uid25_bh4_uid78_In0,
                 X1 => Compressor_14_3_F400_uid25_bh4_uid78_In1,
                 R => Compressor_14_3_F400_uid25_bh4_uid78_Out0_copy79);
   Compressor_14_3_F400_uid25_bh4_uid78_Out0 <= Compressor_14_3_F400_uid25_bh4_uid78_Out0_copy79; -- output copy to hold a pipeline register if needed

   bh4_w25_5 <= Compressor_14_3_F400_uid25_bh4_uid78_Out0(0);
   bh4_w26_3 <= Compressor_14_3_F400_uid25_bh4_uid78_Out0(1);
   bh4_w27_4 <= Compressor_14_3_F400_uid25_bh4_uid78_Out0(2);

   Compressor_14_3_F400_uid25_bh4_uid80_In0 <= "" & bh4_w27_3 & bh4_w27_2 & "0" & "0";
   Compressor_14_3_F400_uid25_bh4_uid80_In1 <= "" & bh4_w28_2;
   Compressor_14_3_F400_uid25_uid80: Compressor_14_3_F400_uid25
      port map ( X0 => Compressor_14_3_F400_uid25_bh4_uid80_In0,
                 X1 => Compressor_14_3_F400_uid25_bh4_uid80_In1,
                 R => Compressor_14_3_F400_uid25_bh4_uid80_Out0_copy81);
   Compressor_14_3_F400_uid25_bh4_uid80_Out0 <= Compressor_14_3_F400_uid25_bh4_uid80_Out0_copy81; -- output copy to hold a pipeline register if needed

   bh4_w27_5 <= Compressor_14_3_F400_uid25_bh4_uid80_Out0(0);
   bh4_w28_3 <= Compressor_14_3_F400_uid25_bh4_uid80_Out0(1);
   bh4_w29_4 <= Compressor_14_3_F400_uid25_bh4_uid80_Out0(2);

   Compressor_14_3_F400_uid25_bh4_uid82_In0 <= "" & bh4_w29_3 & bh4_w29_2 & "0" & "0";
   Compressor_14_3_F400_uid25_bh4_uid82_In1 <= "" & bh4_w30_1;
   Compressor_14_3_F400_uid25_uid82: Compressor_14_3_F400_uid25
      port map ( X0 => Compressor_14_3_F400_uid25_bh4_uid82_In0,
                 X1 => Compressor_14_3_F400_uid25_bh4_uid82_In1,
                 R => Compressor_14_3_F400_uid25_bh4_uid82_Out0_copy83);
   Compressor_14_3_F400_uid25_bh4_uid82_Out0 <= Compressor_14_3_F400_uid25_bh4_uid82_Out0_copy83; -- output copy to hold a pipeline register if needed

   bh4_w29_5 <= Compressor_14_3_F400_uid25_bh4_uid82_Out0(0);
   bh4_w30_2 <= Compressor_14_3_F400_uid25_bh4_uid82_Out0(1);
   bh4_w31_2 <= Compressor_14_3_F400_uid25_bh4_uid82_Out0(2);

   Compressor_14_3_F400_uid25_bh4_uid84_In0 <= "" & bh4_w31_0 & bh4_w31_1 & "0" & "0";
   Compressor_14_3_F400_uid25_bh4_uid84_In1 <= "" & bh4_w32_0;
   Compressor_14_3_F400_uid25_uid84: Compressor_14_3_F400_uid25
      port map ( X0 => Compressor_14_3_F400_uid25_bh4_uid84_In0,
                 X1 => Compressor_14_3_F400_uid25_bh4_uid84_In1,
                 R => Compressor_14_3_F400_uid25_bh4_uid84_Out0_copy85);
   Compressor_14_3_F400_uid25_bh4_uid84_Out0 <= Compressor_14_3_F400_uid25_bh4_uid84_Out0_copy85; -- output copy to hold a pipeline register if needed

   bh4_w31_3 <= Compressor_14_3_F400_uid25_bh4_uid84_Out0(0);
   bh4_w32_1 <= Compressor_14_3_F400_uid25_bh4_uid84_Out0(1);
   bh4_w33_1 <= Compressor_14_3_F400_uid25_bh4_uid84_Out0(2);
   tmp_bitheapResult_bh4_2 <= bh4_w2_5 & bh4_w1_4 & bh4_w0_3;

   bitheapFinalAdd_bh4_In0 <= "0" & bh4_w39_0 & bh4_w38_0 & bh4_w37_0 & bh4_w36_0 & bh4_w35_0 & bh4_w34_0 & bh4_w33_0 & bh4_w32_1 & bh4_w31_3 & bh4_w30_2 & bh4_w29_5 & bh4_w28_3 & bh4_w27_5 & bh4_w26_3 & bh4_w25_5 & bh4_w24_3 & bh4_w23_5 & bh4_w22_4 & bh4_w21_6 & bh4_w20_4 & bh4_w19_6 & bh4_w18_4 & bh4_w17_6 & bh4_w16_4 & bh4_w15_6 & bh4_w14_4 & bh4_w13_6 & bh4_w12_4 & bh4_w11_6 & bh4_w10_4 & bh4_w9_6 & bh4_w8_4 & bh4_w7_7 & bh4_w6_5 & bh4_w5_7 & bh4_w4_5 & bh4_w3_5;
   bitheapFinalAdd_bh4_In1 <= "0" & "0" & "0" & "0" & "0" & "0" & "0" & bh4_w33_1 & "0" & bh4_w31_2 & "0" & bh4_w29_4 & "0" & bh4_w27_4 & "0" & bh4_w25_4 & "0" & bh4_w23_4 & "0" & bh4_w21_5 & "0" & bh4_w19_5 & "0" & bh4_w17_5 & "0" & bh4_w15_5 & "0" & bh4_w13_5 & "0" & bh4_w11_5 & "0" & bh4_w9_5 & "0" & bh4_w7_6 & "0" & bh4_w5_6 & "0" & bh4_w3_4;
   bitheapFinalAdd_bh4_Cin <= '0';

   bitheapFinalAdd_bh4: IntAdder_38_F400_uid87
      port map ( clk  => clk,
                 Cin => bitheapFinalAdd_bh4_Cin,
                 X => bitheapFinalAdd_bh4_In0,
                 Y => bitheapFinalAdd_bh4_In1,
                 R => bitheapFinalAdd_bh4_Out);
   bitheapResult_bh4 <= bitheapFinalAdd_bh4_Out(36 downto 0) & tmp_bitheapResult_bh4_2_d2;
   R <= bitheapResult_bh4(39 downto 6);
end architecture;
