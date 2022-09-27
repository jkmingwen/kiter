--------------------------------------------------------------------------------
--             FP2Fix_8_23_0_33_S_NT_F400_uid2Exponent_difference
--                           (IntAdder_8_f400_uid4)
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved
-- Authors: Bogdan Pasca, Florent de Dinechin (2008-2010)
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity FP2Fix_8_23_0_33_S_NT_F400_uid2Exponent_difference is
   port ( clk : in std_logic;
          X : in  std_logic_vector(7 downto 0);
          Y : in  std_logic_vector(7 downto 0);
          Cin : in  std_logic;
          R : out  std_logic_vector(7 downto 0)   );
end entity;

architecture arch of FP2Fix_8_23_0_33_S_NT_F400_uid2Exponent_difference is
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
         end if;
      end process;
   --Classical
    R <= X + Y + Cin;
end architecture;

--------------------------------------------------------------------------------
--                    LeftShifter_24_by_max_36_F400_uid12
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved
-- Authors: Bogdan Pasca, Florent de Dinechin (2008-2011)
--------------------------------------------------------------------------------
-- Pipeline depth: 2 cycles

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity LeftShifter_24_by_max_36_F400_uid12 is
   port ( clk : in std_logic;
          X : in  std_logic_vector(23 downto 0);
          S : in  std_logic_vector(5 downto 0);
          R : out  std_logic_vector(59 downto 0)   );
end entity;

architecture arch of LeftShifter_24_by_max_36_F400_uid12 is
signal level0 :  std_logic_vector(23 downto 0);
signal ps, ps_d1, ps_d2 :  std_logic_vector(5 downto 0);
signal level1 :  std_logic_vector(24 downto 0);
signal level2, level2_d1 :  std_logic_vector(26 downto 0);
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
            level2_d1 <=  level2;
            level5_d1 <=  level5;
         end if;
      end process;
   level0<= X;
   ps<= S;
   level1<= level0 & (0 downto 0 => '0') when ps(0)= '1' else     (0 downto 0 => '0') & level0;
   level2<= level1 & (1 downto 0 => '0') when ps(1)= '1' else     (1 downto 0 => '0') & level1;
   ----------------Synchro barrier, entering cycle 1----------------
   level3<= level2_d1 & (3 downto 0 => '0') when ps_d1(2)= '1' else     (3 downto 0 => '0') & level2_d1;
   level4<= level3 & (7 downto 0 => '0') when ps_d1(3)= '1' else     (7 downto 0 => '0') & level3;
   level5<= level4 & (15 downto 0 => '0') when ps_d1(4)= '1' else     (15 downto 0 => '0') & level4;
   ----------------Synchro barrier, entering cycle 2----------------
   level6<= level5_d1 & (31 downto 0 => '0') when ps_d2(5)= '1' else     (31 downto 0 => '0') & level5_d1;
   R <= level6(59 downto 0);
end architecture;

--------------------------------------------------------------------------------
--                   FP2Fix_8_23_0_33_S_NT_F400_uid2MantSum
--                          (IntAdder_35_f400_uid16)
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved
-- Authors: Bogdan Pasca, Florent de Dinechin (2008-2010)
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity FP2Fix_8_23_0_33_S_NT_F400_uid2MantSum is
   port ( clk : in std_logic;
          X : in  std_logic_vector(34 downto 0);
          Y : in  std_logic_vector(34 downto 0);
          Cin : in  std_logic;
          R : out  std_logic_vector(34 downto 0)   );
end entity;

architecture arch of FP2Fix_8_23_0_33_S_NT_F400_uid2MantSum is
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
         end if;
      end process;
   --Classical
    R <= X + Y + Cin;
end architecture;

--------------------------------------------------------------------------------
--                      FP2Fix_8_23_0_33_S_NT_F400_uid2
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved
-- Authors: Fabrizio Ferrandi (2012)
--------------------------------------------------------------------------------
-- Pipeline depth: 4 cycles

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity FP2Fix_8_23_0_33_S_NT_F400_uid2 is
   port ( clk : in std_logic;
          X : in  std_logic_vector(8+23+2 downto 0);
          R : out  std_logic_vector(33 downto 0)   );
end entity;

architecture arch of FP2Fix_8_23_0_33_S_NT_F400_uid2 is
   component FP2Fix_8_23_0_33_S_NT_F400_uid2Exponent_difference is
      port ( clk : in std_logic;
             X : in  std_logic_vector(7 downto 0);
             Y : in  std_logic_vector(7 downto 0);
             Cin : in  std_logic;
             R : out  std_logic_vector(7 downto 0)   );
   end component;

   component LeftShifter_24_by_max_36_F400_uid12 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(23 downto 0);
             S : in  std_logic_vector(5 downto 0);
             R : out  std_logic_vector(59 downto 0)   );
   end component;

   component FP2Fix_8_23_0_33_S_NT_F400_uid2MantSum is
      port ( clk : in std_logic;
             X : in  std_logic_vector(34 downto 0);
             Y : in  std_logic_vector(34 downto 0);
             Cin : in  std_logic;
             R : out  std_logic_vector(34 downto 0)   );
   end component;

signal eA0 :  std_logic_vector(7 downto 0);
signal fA0, fA0_d1 :  std_logic_vector(23 downto 0);
signal bias :  std_logic_vector(7 downto 0);
signal eA1, eA1_d1 :  std_logic_vector(7 downto 0);
signal shiftedby :  std_logic_vector(5 downto 0);
signal fA1 :  std_logic_vector(59 downto 0);
signal fA2a :  std_logic_vector(34 downto 0);
signal notallzero :  std_logic;
signal round :  std_logic;
signal fA2b :  std_logic_vector(34 downto 0);
signal fA3, fA3_d1 :  std_logic_vector(34 downto 0);
signal fA3b :  std_logic_vector(34 downto 0);
signal fA4 :  std_logic_vector(33 downto 0);
signal overFl0 :  std_logic;
signal overFl1 :  std_logic;
signal eTest :  std_logic;
signal I_d1, I_d2, I_d3, I_d4 :  std_logic_vector(8+23+2 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            fA0_d1 <=  fA0;
            eA1_d1 <=  eA1;
            fA3_d1 <=  fA3;
            I_d1 <=  X;
            I_d2 <=  I_d1;
            I_d3 <=  I_d2;
            I_d4 <=  I_d3;
         end if;
      end process;
   eA0 <= X(30 downto 23);
   fA0 <= "1" & X(22 downto 0);
   bias <= not conv_std_logic_vector(126, 8);
   Exponent_difference: FP2Fix_8_23_0_33_S_NT_F400_uid2Exponent_difference  -- pipelineDepth=0 maxInDelay=0
      port map ( clk  => clk,
                 Cin => '1',
                 R => eA1,
                 X => bias,
                 Y => eA0);
   ---------------- cycle 0----------------
   ----------------Synchro barrier, entering cycle 1----------------
   shiftedby <= eA1_d1(5 downto 0) when eA1_d1(7) = '0' else (5 downto 0 => '0');
   FXP_shifter: LeftShifter_24_by_max_36_F400_uid12  -- pipelineDepth=2 maxInDelay=0
      port map ( clk  => clk,
                 R => fA1,
                 S => shiftedby,
                 X => fA0_d1);
   ----------------Synchro barrier, entering cycle 3----------------
   fA2a<= '0' & fA1(57 downto 24);
   notallzero <= '0' when fA1(22 downto 0) = (22 downto 0 => '0') else '1';
   round <= (fA1(23) and I_d3(31)) or (fA1(23) and notallzero and not I_d3(31));
   fA2b<= '0' & (33 downto 1 => '0') & round;
   MantSum: FP2Fix_8_23_0_33_S_NT_F400_uid2MantSum  -- pipelineDepth=0 maxInDelay=0
      port map ( clk  => clk,
                 Cin => '0',
                 R => fA3,
                 X => fA2a,
                 Y => fA2b);
   ---------------- cycle 3----------------
   ----------------Synchro barrier, entering cycle 4----------------
   fA3b<= -signed(fA3_d1);
   fA4<= fA3_d1(33 downto 0) when I_d4(31) = '0' else fA3b(33 downto 0);
   overFl0<= '1' when I_d4(30 downto 23) > conv_std_logic_vector(160,8) else I_d4(33);
   overFl1 <= fA3_d1(34);
   eTest <= (overFl0 or overFl1);
   R <= fA4 when eTest = '0' else
      I_d4(31) & (32 downto 0 => not I_d4(31));
end architecture;
