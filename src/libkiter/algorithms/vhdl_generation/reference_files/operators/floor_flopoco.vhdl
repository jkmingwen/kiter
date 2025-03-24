--------------------------------------------------------------------------------
--                     LeftShifter24_by_max_36_F400_uid4
-- VHDL generated for Zynq7000 @ 400MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Bogdan Pasca (2008-2011), Florent de Dinechin (2008-2019)
--------------------------------------------------------------------------------
-- Pipeline depth: 2 cycles
-- Clock period (ns): 2.5
-- Target frequency (MHz): 400
-- Input signals: X S
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity LeftShifter24_by_max_36_F400_uid4 is
    port (clk : in std_logic;
          X : in  std_logic_vector(23 downto 0);
          S : in  std_logic_vector(5 downto 0);
          R : out  std_logic_vector(59 downto 0)   );
end entity;

architecture arch of LeftShifter24_by_max_36_F400_uid4 is
signal ps, ps_d1, ps_d2 :  std_logic_vector(5 downto 0);
signal level0 :  std_logic_vector(23 downto 0);
signal level1, level1_d1 :  std_logic_vector(24 downto 0);
signal level2 :  std_logic_vector(26 downto 0);
signal level3 :  std_logic_vector(30 downto 0);
signal level4 :  std_logic_vector(38 downto 0);
signal level5, level5_d1 :  std_logic_vector(54 downto 0);
signal level6 :  std_logic_vector(86 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            ps_d1 <=  ps;
            ps_d2 <=  ps_d1;
            level1_d1 <=  level1;
            level5_d1 <=  level5;
         end if;
      end process;
   ps<= S;
   level0<= X;
   level1<= level0 & (0 downto 0 => '0') when ps(0)= '1' else     (0 downto 0 => '0') & level0;
   level2<= level1_d1 & (1 downto 0 => '0') when ps_d1(1)= '1' else     (1 downto 0 => '0') & level1_d1;
   level3<= level2 & (3 downto 0 => '0') when ps_d1(2)= '1' else     (3 downto 0 => '0') & level2;
   level4<= level3 & (7 downto 0 => '0') when ps_d1(3)= '1' else     (7 downto 0 => '0') & level3;
   level5<= level4 & (15 downto 0 => '0') when ps_d1(4)= '1' else     (15 downto 0 => '0') & level4;
   level6<= level5_d1 & (31 downto 0 => '0') when ps_d2(5)= '1' else     (31 downto 0 => '0') & level5_d1;
   R <= level6(59 downto 0);
end architecture;

--------------------------------------------------------------------------------
--                       FP2Fix_8_23_0_33_S_T_F400_uid2
-- VHDL generated for Zynq7000 @ 400MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Fabrizio Ferrandi (2012)
--------------------------------------------------------------------------------
-- Pipeline depth: 4 cycles
-- Clock period (ns): 2.5
-- Target frequency (MHz): 400
-- Input signals: I
-- Output signals: O

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity FP2Fix_8_23_0_33_S_T_F400_uid2 is
    port (clk : in std_logic;
          I : in  std_logic_vector(8+23+2 downto 0);
          O : out  std_logic_vector(33 downto 0)   );
end entity;

architecture arch of FP2Fix_8_23_0_33_S_T_F400_uid2 is
   component LeftShifter24_by_max_36_F400_uid4 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(23 downto 0);
             S : in  std_logic_vector(5 downto 0);
             R : out  std_logic_vector(59 downto 0)   );
   end component;

signal eA0 :  std_logic_vector(7 downto 0);
signal fA0 :  std_logic_vector(23 downto 0);
signal eA1 :  std_logic_vector(7 downto 0);
signal shiftedby :  std_logic_vector(5 downto 0);
signal fA1 :  std_logic_vector(59 downto 0);
signal fA2, fA2_d1 :  std_logic_vector(33 downto 0);
signal fA4, fA4_d1 :  std_logic_vector(33 downto 0);
signal overFl0, overFl0_d1, overFl0_d2, overFl0_d3, overFl0_d4 :  std_logic;
signal notZeroTest, notZeroTest_d1 :  std_logic;
signal overFl1 :  std_logic;
signal eTest :  std_logic;
signal I_d1, I_d2, I_d3, I_d4 :  std_logic_vector(8+23+2 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            fA2_d1 <=  fA2;
            fA4_d1 <=  fA4;
            overFl0_d1 <=  overFl0;
            overFl0_d2 <=  overFl0_d1;
            overFl0_d3 <=  overFl0_d2;
            overFl0_d4 <=  overFl0_d3;
            notZeroTest_d1 <=  notZeroTest;
            I_d1 <=  I;
            I_d2 <=  I_d1;
            I_d3 <=  I_d2;
            I_d4 <=  I_d3;
         end if;
      end process;
   eA0 <= I(30 downto 23);
   fA0 <= "1" & I(22 downto 0);
   eA1 <= eA0 - conv_std_logic_vector(126, 8);
   shiftedby <= eA1(5 downto 0) when eA1(7) = '0' else (5 downto 0 => '0');
   FXP_shifter: LeftShifter24_by_max_36_F400_uid4
      port map ( clk  => clk,
                 S => shiftedby,
                 X => fA0,
                 R => fA1);
   fA2<= fA1(57 downto 24);
   fA4<= fA2_d1 when I_d3(31) = '0' else -signed(fA2_d1);
   overFl0<= '1' when I(30 downto 23) > conv_std_logic_vector(160,8) else I(33);
   notZeroTest <= '1' when fA4 /= conv_std_logic_vector(0,34) else '0';
   overFl1 <= (fA4_d1(33) xor I_d4(31)) and notZeroTest_d1;
   eTest <= (overFl0_d4 or overFl1);
   O <= fA4_d1 when eTest = '0' else
      I_d4(31) & (32 downto 0 => not I_d4(31));
end architecture;

