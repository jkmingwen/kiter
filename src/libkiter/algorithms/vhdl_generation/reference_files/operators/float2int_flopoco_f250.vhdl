--------------------------------------------------------------------------------
--                     LeftShifter24_by_max_36_F250_uid4
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved
-- Authors: Bogdan Pasca (2008-2011), Florent de Dinechin (2008-2019)
--------------------------------------------------------------------------------
-- Pipeline depth: 1 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X S
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity LeftShifter24_by_max_36_F250_uid4 is
    port (clk : in std_logic;
          X : in  std_logic_vector(23 downto 0);
          S : in  std_logic_vector(5 downto 0);
          R : out  std_logic_vector(59 downto 0)   );
end entity;

architecture arch of LeftShifter24_by_max_36_F250_uid4 is
signal ps, ps_d1 :  std_logic_vector(5 downto 0);
signal level0 :  std_logic_vector(23 downto 0);
signal level1 :  std_logic_vector(24 downto 0);
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
            level5_d1 <=  level5;
         end if;
      end process;
   ps<= S;
   level0<= X;
   level1<= level0 & (0 downto 0 => '0') when ps(0)= '1' else     (0 downto 0 => '0') & level0;
   level2<= level1 & (1 downto 0 => '0') when ps(1)= '1' else     (1 downto 0 => '0') & level1;
   level3<= level2 & (3 downto 0 => '0') when ps(2)= '1' else     (3 downto 0 => '0') & level2;
   level4<= level3 & (7 downto 0 => '0') when ps(3)= '1' else     (7 downto 0 => '0') & level3;
   level5<= level4 & (15 downto 0 => '0') when ps(4)= '1' else     (15 downto 0 => '0') & level4;
   level6<= level5_d1 & (31 downto 0 => '0') when ps_d1(5)= '1' else     (31 downto 0 => '0') & level5_d1;
   R <= level6(59 downto 0);
end architecture;

--------------------------------------------------------------------------------
--                           float2int_flopoco_f250
--                      (FP2Fix_8_23_0_33_S_T_F250_uid2)
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved
-- Authors: Fabrizio Ferrandi (2012)
--------------------------------------------------------------------------------
-- Pipeline depth: 2 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: I
-- Output signals: O

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity float2int_flopoco_f250 is
    port (clk : in std_logic;
          X : in  std_logic_vector(8+23+2 downto 0);
          R : out  std_logic_vector(33 downto 0)   );
end entity;

architecture arch of float2int_flopoco_f250 is
   component LeftShifter24_by_max_36_F250_uid4 is
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
signal fA2 :  std_logic_vector(33 downto 0);
signal fA4, fA4_d1 :  std_logic_vector(33 downto 0);
signal overFl0, overFl0_d1, overFl0_d2 :  std_logic;
signal notZeroTest :  std_logic;
signal overFl1 :  std_logic;
signal eTest :  std_logic;
signal I_d1, I_d2 :  std_logic_vector(8+23+2 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            fA4_d1 <=  fA4;
            overFl0_d1 <=  overFl0;
            overFl0_d2 <=  overFl0_d1;
            I_d1 <=  X;
            I_d2 <=  I_d1;
         end if;
      end process;
   eA0 <= X(30 downto 23);
   fA0 <= "1" & X(22 downto 0);
   eA1 <= eA0 - conv_std_logic_vector(126, 8);
   shiftedby <= eA1(5 downto 0) when eA1(7) = '0' else (5 downto 0 => '0');
   FXP_shifter: LeftShifter24_by_max_36_F250_uid4
      port map ( clk  => clk,
                 S => shiftedby,
                 X => fA0,
                 R => fA1);
   fA2<= fA1(57 downto 24);
   fA4<= fA2 when I_d1(31) = '0' else -signed(fA2);
   overFl0<= '1' when X(30 downto 23) > conv_std_logic_vector(160,8) else X(33);
   notZeroTest <= '1' when fA4_d1 /= conv_std_logic_vector(0,34) else '0';
   overFl1 <= (fA4_d1(33) xor I_d2(31)) and notZeroTest;
   eTest <= (overFl0_d2 or overFl1);
   R <= fA4_d1 when eTest = '0' else
      I_d2(31) & (32 downto 0 => not I_d2(31));
end architecture;
