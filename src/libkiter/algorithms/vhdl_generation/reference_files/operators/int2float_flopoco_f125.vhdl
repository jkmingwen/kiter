--------------------------------------------------------------------------------
--                     Fix2FP_0_33_S_8_23_F125_uid2_LZOCS
--                (LZOCShifter_33_to_34_counting_64_F125_uid4)
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved
-- Authors: Florent de Dinechin, Bogdan Pasca (2007)
--------------------------------------------------------------------------------
-- Pipeline depth: 1 cycles

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity Fix2FP_0_33_S_8_23_F125_uid2_LZOCS is
   port ( clk, rst : in std_logic;
          I : in  std_logic_vector(32 downto 0);
          OZb : in  std_logic;
          Count : out  std_logic_vector(5 downto 0);
          O : out  std_logic_vector(33 downto 0)   );
end entity;

architecture arch of Fix2FP_0_33_S_8_23_F125_uid2_LZOCS is
signal level6 :  std_logic_vector(32 downto 0);
signal sozb, sozb_d1 :  std_logic;
signal count5, count5_d1 :  std_logic;
signal level5 :  std_logic_vector(32 downto 0);
signal count4, count4_d1 :  std_logic;
signal level4 :  std_logic_vector(32 downto 0);
signal count3, count3_d1 :  std_logic;
signal level3 :  std_logic_vector(32 downto 0);
signal count2, count2_d1 :  std_logic;
signal level2 :  std_logic_vector(32 downto 0);
signal count1, count1_d1 :  std_logic;
signal level1, level1_d1 :  std_logic_vector(32 downto 0);
signal count0 :  std_logic;
signal level0 :  std_logic_vector(32 downto 0);
signal sCount :  std_logic_vector(5 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            sozb_d1 <=  sozb;
            count5_d1 <=  count5;
            count4_d1 <=  count4;
            count3_d1 <=  count3;
            count2_d1 <=  count2;
            count1_d1 <=  count1;
            level1_d1 <=  level1;
         end if;
      end process;
   level6 <= I ;
   sozb<= OZb;
   count5<= '1' when level6(32 downto 1) = (32 downto 1=>sozb) else '0';
   level5<= level6(32 downto 0) when count5='0' else level6(0 downto 0) & (31 downto 0 => '0');

   count4<= '1' when level5(32 downto 17) = (32 downto 17=>sozb) else '0';
   level4<= level5(32 downto 0) when count4='0' else level5(16 downto 0) & (15 downto 0 => '0');

   count3<= '1' when level4(32 downto 25) = (32 downto 25=>sozb) else '0';
   level3<= level4(32 downto 0) when count3='0' else level4(24 downto 0) & (7 downto 0 => '0');

   count2<= '1' when level3(32 downto 29) = (32 downto 29=>sozb) else '0';
   level2<= level3(32 downto 0) when count2='0' else level3(28 downto 0) & (3 downto 0 => '0');

   count1<= '1' when level2(32 downto 31) = (32 downto 31=>sozb) else '0';
   level1<= level2(32 downto 0) when count1='0' else level2(30 downto 0) & (1 downto 0 => '0');

   ----------------Synchro barrier, entering cycle 1----------------
   count0<= '1' when level1_d1(32 downto 32) = (32 downto 32=>sozb_d1) else '0';
   level0<= level1_d1(32 downto 0) when count0='0' else level1_d1(31 downto 0) & (0 downto 0 => '0');

   O <= level0&(0 downto 0 => '0');
   sCount <= count5_d1 & count4_d1 & count3_d1 & count2_d1 & count1_d1 & count0;
   Count <= sCount;
end architecture;

--------------------------------------------------------------------------------
--               Fix2FP_0_33_S_8_23_F125_uid2exponentConversion
--                           (IntAdder_8_f125_uid8)
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

entity Fix2FP_0_33_S_8_23_F125_uid2exponentConversion is
   port ( clk, rst : in std_logic;
          X : in  std_logic_vector(7 downto 0);
          Y : in  std_logic_vector(7 downto 0);
          Cin : in  std_logic;
          R : out  std_logic_vector(7 downto 0)   );
end entity;

architecture arch of Fix2FP_0_33_S_8_23_F125_uid2exponentConversion is
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
--                 Fix2FP_0_33_S_8_23_F125_uid2exponentFinal
--                          (IntAdder_9_f125_uid16)
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

entity Fix2FP_0_33_S_8_23_F125_uid2exponentFinal is
   port ( clk, rst : in std_logic;
          X : in  std_logic_vector(8 downto 0);
          Y : in  std_logic_vector(8 downto 0);
          Cin : in  std_logic;
          R : out  std_logic_vector(8 downto 0)   );
end entity;

architecture arch of Fix2FP_0_33_S_8_23_F125_uid2exponentFinal is
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
--                     Fix2FP_0_33_S_8_23_F125_uid2zeroD
--                          (IntAdder_34_f125_uid24)
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

entity Fix2FP_0_33_S_8_23_F125_uid2zeroD is
   port ( clk, rst : in std_logic;
          X : in  std_logic_vector(33 downto 0);
          Y : in  std_logic_vector(33 downto 0);
          Cin : in  std_logic;
          R : out  std_logic_vector(33 downto 0)   );
end entity;

architecture arch of Fix2FP_0_33_S_8_23_F125_uid2zeroD is
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
--                Fix2FP_0_33_S_8_23_F125_uid2_fractionConvert
--                          (IntAdder_35_f125_uid32)
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

entity Fix2FP_0_33_S_8_23_F125_uid2_fractionConvert is
   port ( clk, rst : in std_logic;
          X : in  std_logic_vector(34 downto 0);
          Y : in  std_logic_vector(34 downto 0);
          Cin : in  std_logic;
          R : out  std_logic_vector(34 downto 0)   );
end entity;

architecture arch of Fix2FP_0_33_S_8_23_F125_uid2_fractionConvert is
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
--                Fix2FP_0_33_S_8_23_F125_uid2_oneSubstracter
--                          (IntAdder_9_f125_uid40)
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

entity Fix2FP_0_33_S_8_23_F125_uid2_oneSubstracter is
   port ( clk, rst : in std_logic;
          X : in  std_logic_vector(8 downto 0);
          Y : in  std_logic_vector(8 downto 0);
          Cin : in  std_logic;
          R : out  std_logic_vector(8 downto 0)   );
end entity;

architecture arch of Fix2FP_0_33_S_8_23_F125_uid2_oneSubstracter is
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
--                 Fix2FP_0_33_S_8_23_F125_uid2roundingAdder
--                          (IntAdder_32_f125_uid48)
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

entity Fix2FP_0_33_S_8_23_F125_uid2roundingAdder is
   port ( clk, rst : in std_logic;
          X : in  std_logic_vector(31 downto 0);
          Y : in  std_logic_vector(31 downto 0);
          Cin : in  std_logic;
          R : out  std_logic_vector(31 downto 0)   );
end entity;

architecture arch of Fix2FP_0_33_S_8_23_F125_uid2roundingAdder is
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
--                           int2float_flopoco_f125
--                       (Fix2FP_0_33_S_8_23_F125_uid2)
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved
-- Authors: Radu Tudoran, Bogdan Pasca (2009)
--------------------------------------------------------------------------------
-- Pipeline depth: 1 cycles

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity int2float_flopoco_f125 is
   port ( clk, rst : in std_logic;
          I : in  std_logic_vector(33 downto 0);
          O : out  std_logic_vector(8+23+2 downto 0)   );
end entity;

architecture arch of int2float_flopoco_f125 is
   component Fix2FP_0_33_S_8_23_F125_uid2_LZOCS is
      port ( clk, rst : in std_logic;
             I : in  std_logic_vector(32 downto 0);
             OZb : in  std_logic;
             Count : out  std_logic_vector(5 downto 0);
             O : out  std_logic_vector(33 downto 0)   );
   end component;

   component Fix2FP_0_33_S_8_23_F125_uid2exponentConversion is
      port ( clk, rst : in std_logic;
             X : in  std_logic_vector(7 downto 0);
             Y : in  std_logic_vector(7 downto 0);
             Cin : in  std_logic;
             R : out  std_logic_vector(7 downto 0)   );
   end component;

   component Fix2FP_0_33_S_8_23_F125_uid2exponentFinal is
      port ( clk, rst : in std_logic;
             X : in  std_logic_vector(8 downto 0);
             Y : in  std_logic_vector(8 downto 0);
             Cin : in  std_logic;
             R : out  std_logic_vector(8 downto 0)   );
   end component;

   component Fix2FP_0_33_S_8_23_F125_uid2zeroD is
      port ( clk, rst : in std_logic;
             X : in  std_logic_vector(33 downto 0);
             Y : in  std_logic_vector(33 downto 0);
             Cin : in  std_logic;
             R : out  std_logic_vector(33 downto 0)   );
   end component;

   component Fix2FP_0_33_S_8_23_F125_uid2_fractionConvert is
      port ( clk, rst : in std_logic;
             X : in  std_logic_vector(34 downto 0);
             Y : in  std_logic_vector(34 downto 0);
             Cin : in  std_logic;
             R : out  std_logic_vector(34 downto 0)   );
   end component;

   component Fix2FP_0_33_S_8_23_F125_uid2_oneSubstracter is
      port ( clk, rst : in std_logic;
             X : in  std_logic_vector(8 downto 0);
             Y : in  std_logic_vector(8 downto 0);
             Cin : in  std_logic;
             R : out  std_logic_vector(8 downto 0)   );
   end component;

   component Fix2FP_0_33_S_8_23_F125_uid2roundingAdder is
      port ( clk, rst : in std_logic;
             X : in  std_logic_vector(31 downto 0);
             Y : in  std_logic_vector(31 downto 0);
             Cin : in  std_logic;
             R : out  std_logic_vector(31 downto 0)   );
   end component;

signal input :  std_logic_vector(33 downto 0);
signal signSignal, signSignal_d1 :  std_logic;
signal passedInput :  std_logic_vector(33 downto 0);
signal input2LZOC :  std_logic_vector(32 downto 0);
signal temporalExponent :  std_logic_vector(5 downto 0);
signal temporalFraction :  std_logic_vector(33 downto 0);
signal MSB2Signal :  std_logic_vector(7 downto 0);
signal zeroPadding4Exponent :  std_logic_vector(1 downto 0);
signal valueExponent :  std_logic_vector(7 downto 0);
signal partialConvertedExponent :  std_logic_vector(7 downto 0);
signal biassOfOnes :  std_logic_vector(6 downto 0);
signal biassSignal :  std_logic_vector(7 downto 0);
signal biassSignalBit :  std_logic_vector(8 downto 0);
signal partialConvertedExponentBit :  std_logic_vector(8 downto 0);
signal sign4OU :  std_logic;
signal convertedExponentBit :  std_logic_vector(8 downto 0);
signal convertedExponent :  std_logic_vector(7 downto 0);
signal underflowSignal :  std_logic;
signal overflowSignal :  std_logic;
signal minusOne4ZD :  std_logic_vector(33 downto 0);
signal zeroDS :  std_logic_vector(33 downto 0);
signal zeroInput, zeroInput_d1 :  std_logic;
signal sign2vector :  std_logic_vector(33 downto 0);
signal tempConvert :  std_logic_vector(33 downto 0);
signal tempConvert0 :  std_logic_vector(34 downto 0);
signal tempPaddingAddSign :  std_logic_vector(33 downto 0);
signal tempAddSign :  std_logic_vector(34 downto 0);
signal tempFractionResult :  std_logic_vector(34 downto 0);
signal correctingExponent :  std_logic;
signal fractionConverted :  std_logic_vector(22 downto 0);
signal firstBitofRest :  std_logic;
signal lastBitOfFraction :  std_logic;
signal minusOne :  std_logic_vector(8 downto 0);
signal fractionRemainder :  std_logic_vector(8 downto 0);
signal zeroFractionResult :  std_logic_vector(8 downto 0);
signal zeroRemainder :  std_logic;
signal outputOfMux3 :  std_logic;
signal outputOfMux2 :  std_logic;
signal outputOfMux1 :  std_logic;
signal possibleCorrector4Rounding :  std_logic_vector(31 downto 0);
signal concatenationForRounding :  std_logic_vector(31 downto 0);
signal testC :  std_logic_vector(31 downto 0);
signal testR :  std_logic_vector(31 downto 0);
signal testM :  std_logic;
signal roundedResult :  std_logic_vector(31 downto 0);
signal convertedExponentAfterRounding :  std_logic_vector(7 downto 0);
signal convertedFractionAfterRounding :  std_logic_vector(22 downto 0);
signal MSBSelection :  std_logic;
signal LSBSelection :  std_logic;
signal Selection :  std_logic_vector(1 downto 0);
signal specialBits :  std_logic_vector(1 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            signSignal_d1 <=  signSignal;
            zeroInput_d1 <=  zeroInput;
         end if;
      end process;
   input <= I;
   signSignal<=input(33);
   passedInput<=input(33 downto 0);
   input2LZOC<=passedInput(32 downto 0);
   LZOC_component: Fix2FP_0_33_S_8_23_F125_uid2_LZOCS  -- pipelineDepth=1 maxInDelay=0
      port map ( clk  => clk,
                 rst  => rst,
                 Count => temporalExponent,
                 I => input2LZOC,
                 O => temporalFraction,
                 OZb => signSignal);
   ----------------Synchro barrier, entering cycle 1----------------
   MSB2Signal<=CONV_STD_LOGIC_VECTOR(32,8);
   zeroPadding4Exponent<=CONV_STD_LOGIC_VECTOR(0,2);
   valueExponent<= not (zeroPadding4Exponent & temporalExponent );
   exponentConversion: Fix2FP_0_33_S_8_23_F125_uid2exponentConversion  -- pipelineDepth=0 maxInDelay=0
      port map ( clk  => clk,
                 rst  => rst,
                 Cin => '1',
                 R => partialConvertedExponent,
                 X => MSB2Signal,
                 Y => valueExponent);
   biassOfOnes<=CONV_STD_LOGIC_VECTOR(255,7);
   biassSignal<='0' & biassOfOnes;
   biassSignalBit<='0' & biassSignal;
   partialConvertedExponentBit<= '0' & partialConvertedExponent;
   sign4OU<=partialConvertedExponent(7);
   exponentFinal: Fix2FP_0_33_S_8_23_F125_uid2exponentFinal  -- pipelineDepth=0 maxInDelay=0
      port map ( clk  => clk,
                 rst  => rst,
                 Cin => '0',
                 R => convertedExponentBit,
                 X => partialConvertedExponentBit,
                 Y => biassSignalBit);
   convertedExponent<= convertedExponentBit(7 downto 0);
   underflowSignal<= '1' when (sign4OU='1' and convertedExponentBit(8 downto 7) = "01" ) else '0' ;
   overflowSignal<= '1' when (sign4OU='0' and convertedExponentBit(8 downto 7) = "10" ) else '0' ;
   ---------------- cycle 0----------------
   minusOne4ZD<=CONV_STD_LOGIC_VECTOR(-1,34);
   zeroD: Fix2FP_0_33_S_8_23_F125_uid2zeroD  -- pipelineDepth=0 maxInDelay=0
      port map ( clk  => clk,
                 rst  => rst,
                 Cin => '0',
                 R => zeroDS,
                 X => passedInput,
                 Y => minusOne4ZD);
   zeroInput<= zeroDS(33) and not(signSignal);
   ---------------- cycle 1----------------
   sign2vector<=(others => signSignal_d1);
   tempConvert<= sign2vector xor temporalFraction;
   tempConvert0<= '0' & tempConvert;
   tempPaddingAddSign<=(others=>'0');
   tempAddSign<=tempPaddingAddSign & signSignal_d1;
   fractionConverter: Fix2FP_0_33_S_8_23_F125_uid2_fractionConvert  -- pipelineDepth=0 maxInDelay=0
      port map ( clk  => clk,
                 rst  => rst,
                 Cin => '0',
                 R => tempFractionResult,
                 X => tempConvert0,
                 Y => tempAddSign);
   correctingExponent<=tempFractionResult(34);
   fractionConverted<=tempFractionResult(32 downto 10);
   firstBitofRest<=tempFractionResult(9);
   lastBitOfFraction<=tempFractionResult(10);
   ---------------- cycle 1----------------
   minusOne<=CONV_STD_LOGIC_VECTOR(-1,9);
   fractionRemainder<= tempFractionResult(8 downto 0);
   oneSubstracter: Fix2FP_0_33_S_8_23_F125_uid2_oneSubstracter  -- pipelineDepth=0 maxInDelay=0
      port map ( clk  => clk,
                 rst  => rst,
                 Cin => '0',
                 R => zeroFractionResult,
                 X => fractionRemainder,
                 Y => minusOne);
   zeroRemainder<= not( not (tempFractionResult(8)) and zeroFractionResult(8));
   outputOfMux3<=lastBitOfFraction;
   with zeroRemainder select
   outputOfMux2 <= outputOfMux3 when '0', '1' when others;
   with firstBitofRest select
   outputOfMux1 <= outputOfMux2 when '1', '0' when others;
   possibleCorrector4Rounding<=CONV_STD_LOGIC_VECTOR(0,8) & correctingExponent & CONV_STD_LOGIC_VECTOR(0,23);
   concatenationForRounding<= '0' & convertedExponent & fractionConverted;
   testC<= concatenationForRounding;
   testR<= possibleCorrector4Rounding;
   testM<= outputOfMux1;
   roundingAdder: Fix2FP_0_33_S_8_23_F125_uid2roundingAdder  -- pipelineDepth=0 maxInDelay=0
      port map ( clk  => clk,
                 rst  => rst,
                 Cin => outputOfMux1,
                 R => roundedResult,
                 X => concatenationForRounding,
                 Y => possibleCorrector4Rounding);
   convertedExponentAfterRounding<= roundedResult(30 downto 23);
   convertedFractionAfterRounding<= roundedResult(22 downto 0);
   MSBSelection<= overflowSignal or roundedResult(31);
   LSBSelection<= not(underflowSignal and not(zeroInput_d1));
   Selection<= MSBSelection & LSBSelection when zeroInput_d1='0' else "00";
   specialBits <= Selection;
   O<= specialBits & signSignal_d1 & convertedExponentAfterRounding & convertedFractionAfterRounding;
end architecture;

