--------------------------------------------------------------------------------
--                     LeftShifter24_by_max_26_F250_uid4
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved
-- Authors: Bogdan Pasca (2008-2011), Florent de Dinechin (2008-2019)
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
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

entity LeftShifter24_by_max_26_F250_uid4 is
    port (clk : in std_logic;
          X : in  std_logic_vector(23 downto 0);
          S : in  std_logic_vector(4 downto 0);
          R : out  std_logic_vector(49 downto 0)   );
end entity;

architecture arch of LeftShifter24_by_max_26_F250_uid4 is
signal ps :  std_logic_vector(4 downto 0);
signal level0 :  std_logic_vector(23 downto 0);
signal level1 :  std_logic_vector(24 downto 0);
signal level2 :  std_logic_vector(26 downto 0);
signal level3 :  std_logic_vector(30 downto 0);
signal level4 :  std_logic_vector(38 downto 0);
signal level5 :  std_logic_vector(54 downto 0);
begin
   ps<= S;
   level0<= X;
   level1<= level0 & (0 downto 0 => '0') when ps(0)= '1' else     (0 downto 0 => '0') & level0;
   level2<= level1 & (1 downto 0 => '0') when ps(1)= '1' else     (1 downto 0 => '0') & level1;
   level3<= level2 & (3 downto 0 => '0') when ps(2)= '1' else     (3 downto 0 => '0') & level2;
   level4<= level3 & (7 downto 0 => '0') when ps(3)= '1' else     (7 downto 0 => '0') & level3;
   level5<= level4 & (15 downto 0 => '0') when ps(4)= '1' else     (15 downto 0 => '0') & level4;
   R <= level5(49 downto 0);
end architecture;

--------------------------------------------------------------------------------
--                            fp2fix_flopoco_f250
--                      (FP2Fix_8_23_0_23_S_T_F250_uid2)
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

entity fp2fix_flopoco_f250 is
    port (clk : in std_logic;
          I : in  std_logic_vector(8+23+2 downto 0);
          O : out  std_logic_vector(23 downto 0)   );
end entity;

architecture arch of fp2fix_flopoco_f250 is
   component LeftShifter24_by_max_26_F250_uid4 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(23 downto 0);
             S : in  std_logic_vector(4 downto 0);
             R : out  std_logic_vector(49 downto 0)   );
   end component;

signal eA0 :  std_logic_vector(7 downto 0);
signal fA0 :  std_logic_vector(23 downto 0);
signal eA1 :  std_logic_vector(7 downto 0);
signal shiftedby :  std_logic_vector(4 downto 0);
signal fA1 :  std_logic_vector(49 downto 0);
signal fA2, fA2_d1 :  std_logic_vector(23 downto 0);
signal fA4, fA4_d1 :  std_logic_vector(23 downto 0);
signal overFl0, overFl0_d1, overFl0_d2 :  std_logic;
signal notZeroTest :  std_logic;
signal overFl1, overFl1_d1 :  std_logic;
signal eTest :  std_logic;
signal I_d1, I_d2 :  std_logic_vector(8+23+2 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            fA2_d1 <=  fA2;
            fA4_d1 <=  fA4;
            overFl0_d1 <=  overFl0;
            overFl0_d2 <=  overFl0_d1;
            overFl1_d1 <=  overFl1;
            I_d1 <=  I;
            I_d2 <=  I_d1;
         end if;
      end process;
   eA0 <= I(30 downto 23);
   fA0 <= "1" & I(22 downto 0);
   eA1 <= eA0 - conv_std_logic_vector(126, 8);
   shiftedby <= eA1(4 downto 0) when eA1(7) = '0' else (4 downto 0 => '0');
   FXP_shifter: LeftShifter24_by_max_26_F250_uid4
      port map ( clk  => clk,
                 S => shiftedby,
                 X => fA0,
                 R => fA1);
   fA2<= fA1(47 downto 24);
   fA4<= fA2_d1 when I_d1(31) = '0' else -signed(fA2_d1);
   overFl0<= '1' when I(30 downto 23) > conv_std_logic_vector(150,8) else I(33);
   notZeroTest <= '1' when fA4 /= conv_std_logic_vector(0,24) else '0';
   overFl1 <= (fA4(23) xor I_d1(31)) and notZeroTest;
   eTest <= (overFl0_d2 or overFl1_d1);
   O <= fA4_d1 when eTest = '0' else
      I_d2(31) & (22 downto 0 => not I_d2(31));
end architecture;
