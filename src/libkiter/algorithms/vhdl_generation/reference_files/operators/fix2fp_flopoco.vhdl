--------------------------------------------------------------------------------
--                     Fix2FP_0_23_S_8_23_F125_uid2_LZOCS
--                (LZOCShifter_23_to_27_counting_32_F125_uid4)
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved
-- Authors: Florent de Dinechin, Bogdan Pasca (2007)
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity Fix2FP_0_23_S_8_23_F125_uid2_LZOCS is
   port ( clk, rst : in std_logic;
          I : in  std_logic_vector(22 downto 0);
          OZb : in  std_logic;
          Count : out  std_logic_vector(4 downto 0);
          O : out  std_logic_vector(26 downto 0)   );
end entity;

architecture arch of Fix2FP_0_23_S_8_23_F125_uid2_LZOCS is
signal level5 :  std_logic_vector(22 downto 0);
signal sozb :  std_logic;
signal count4 :  std_logic;
signal level4 :  std_logic_vector(22 downto 0);
signal count3 :  std_logic;
signal level3 :  std_logic_vector(22 downto 0);
signal count2 :  std_logic;
signal level2 :  std_logic_vector(22 downto 0);
signal count1 :  std_logic;
signal level1 :  std_logic_vector(22 downto 0);
signal count0 :  std_logic;
signal level0 :  std_logic_vector(22 downto 0);
signal sCount :  std_logic_vector(4 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
         end if;
      end process;
   level5 <= I ;
   sozb<= OZb;
   count4<= '1' when level5(22 downto 7) = (22 downto 7=>sozb) else '0';
   level4<= level5(22 downto 0) when count4='0' else level5(6 downto 0) & (15 downto 0 => '0');

   count3<= '1' when level4(22 downto 15) = (22 downto 15=>sozb) else '0';
   level3<= level4(22 downto 0) when count3='0' else level4(14 downto 0) & (7 downto 0 => '0');

   count2<= '1' when level3(22 downto 19) = (22 downto 19=>sozb) else '0';
   level2<= level3(22 downto 0) when count2='0' else level3(18 downto 0) & (3 downto 0 => '0');

   count1<= '1' when level2(22 downto 21) = (22 downto 21=>sozb) else '0';
   level1<= level2(22 downto 0) when count1='0' else level2(20 downto 0) & (1 downto 0 => '0');

   count0<= '1' when level1(22 downto 22) = (22 downto 22=>sozb) else '0';
   level0<= level1(22 downto 0) when count0='0' else level1(21 downto 0) & (0 downto 0 => '0');

   O <= level0&(3 downto 0 => '0');
   sCount <= count4 & count3 & count2 & count1 & count0;
   Count <= sCount;
end architecture;

--------------------------------------------------------------------------------
--               Fix2FP_0_23_S_8_23_F125_uid2exponentConversion
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

entity Fix2FP_0_23_S_8_23_F125_uid2exponentConversion is
   port ( clk, rst : in std_logic;
          X : in  std_logic_vector(7 downto 0);
          Y : in  std_logic_vector(7 downto 0);
          Cin : in  std_logic;
          R : out  std_logic_vector(7 downto 0)   );
end entity;

architecture arch of Fix2FP_0_23_S_8_23_F125_uid2exponentConversion is
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
--                 Fix2FP_0_23_S_8_23_F125_uid2exponentFinal
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

entity Fix2FP_0_23_S_8_23_F125_uid2exponentFinal is
   port ( clk, rst : in std_logic;
          X : in  std_logic_vector(8 downto 0);
          Y : in  std_logic_vector(8 downto 0);
          Cin : in  std_logic;
          R : out  std_logic_vector(8 downto 0)   );
end entity;

architecture arch of Fix2FP_0_23_S_8_23_F125_uid2exponentFinal is
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
--                     Fix2FP_0_23_S_8_23_F125_uid2zeroD
--                          (IntAdder_24_f125_uid24)
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

entity Fix2FP_0_23_S_8_23_F125_uid2zeroD is
   port ( clk, rst : in std_logic;
          X : in  std_logic_vector(23 downto 0);
          Y : in  std_logic_vector(23 downto 0);
          Cin : in  std_logic;
          R : out  std_logic_vector(23 downto 0)   );
end entity;

architecture arch of Fix2FP_0_23_S_8_23_F125_uid2zeroD is
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
--                Fix2FP_0_23_S_8_23_F125_uid2_fractionConvert
--                          (IntAdder_28_f125_uid32)
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

entity Fix2FP_0_23_S_8_23_F125_uid2_fractionConvert is
   port ( clk, rst : in std_logic;
          X : in  std_logic_vector(27 downto 0);
          Y : in  std_logic_vector(27 downto 0);
          Cin : in  std_logic;
          R : out  std_logic_vector(27 downto 0)   );
end entity;

architecture arch of Fix2FP_0_23_S_8_23_F125_uid2_fractionConvert is
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
--                Fix2FP_0_23_S_8_23_F125_uid2_oneSubstracter
--                          (IntAdder_2_f125_uid40)
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

entity Fix2FP_0_23_S_8_23_F125_uid2_oneSubstracter is
   port ( clk, rst : in std_logic;
          X : in  std_logic_vector(1 downto 0);
          Y : in  std_logic_vector(1 downto 0);
          Cin : in  std_logic;
          R : out  std_logic_vector(1 downto 0)   );
end entity;

architecture arch of Fix2FP_0_23_S_8_23_F125_uid2_oneSubstracter is
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
--                 Fix2FP_0_23_S_8_23_F125_uid2roundingAdder
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

entity Fix2FP_0_23_S_8_23_F125_uid2roundingAdder is
   port ( clk, rst : in std_logic;
          X : in  std_logic_vector(31 downto 0);
          Y : in  std_logic_vector(31 downto 0);
          Cin : in  std_logic;
          R : out  std_logic_vector(31 downto 0)   );
end entity;

architecture arch of Fix2FP_0_23_S_8_23_F125_uid2roundingAdder is
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
--                        Fix2FP_0_23_S_8_23_F125_uid2
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved
-- Authors: Radu Tudoran, Bogdan Pasca (2009)
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity Fix2FP_0_23_S_8_23_F125_uid2 is
   port ( clk, rst : in std_logic;
          I : in  std_logic_vector(23 downto 0);
          O : out  std_logic_vector(8+23+2 downto 0)   );
end entity;

architecture arch of Fix2FP_0_23_S_8_23_F125_uid2 is
   component Fix2FP_0_23_S_8_23_F125_uid2_LZOCS is
      port ( clk, rst : in std_logic;
             I : in  std_logic_vector(22 downto 0);
             OZb : in  std_logic;
             Count : out  std_logic_vector(4 downto 0);
             O : out  std_logic_vector(26 downto 0)   );
   end component;

   component Fix2FP_0_23_S_8_23_F125_uid2exponentConversion is
      port ( clk, rst : in std_logic;
             X : in  std_logic_vector(7 downto 0);
             Y : in  std_logic_vector(7 downto 0);
             Cin : in  std_logic;
             R : out  std_logic_vector(7 downto 0)   );
   end component;

   component Fix2FP_0_23_S_8_23_F125_uid2exponentFinal is
      port ( clk, rst : in std_logic;
             X : in  std_logic_vector(8 downto 0);
             Y : in  std_logic_vector(8 downto 0);
             Cin : in  std_logic;
             R : out  std_logic_vector(8 downto 0)   );
   end component;

   component Fix2FP_0_23_S_8_23_F125_uid2zeroD is
      port ( clk, rst : in std_logic;
             X : in  std_logic_vector(23 downto 0);
             Y : in  std_logic_vector(23 downto 0);
             Cin : in  std_logic;
             R : out  std_logic_vector(23 downto 0)   );
   end component;

   component Fix2FP_0_23_S_8_23_F125_uid2_fractionConvert is
      port ( clk, rst : in std_logic;
             X : in  std_logic_vector(27 downto 0);
             Y : in  std_logic_vector(27 downto 0);
             Cin : in  std_logic;
             R : out  std_logic_vector(27 downto 0)   );
   end component;

   component Fix2FP_0_23_S_8_23_F125_uid2_oneSubstracter is
      port ( clk, rst : in std_logic;
             X : in  std_logic_vector(1 downto 0);
             Y : in  std_logic_vector(1 downto 0);
             Cin : in  std_logic;
             R : out  std_logic_vector(1 downto 0)   );
   end component;

   component Fix2FP_0_23_S_8_23_F125_uid2roundingAdder is
      port ( clk, rst : in std_logic;
             X : in  std_logic_vector(31 downto 0);
             Y : in  std_logic_vector(31 downto 0);
             Cin : in  std_logic;
             R : out  std_logic_vector(31 downto 0)   );
   end component;

signal input :  std_logic_vector(23 downto 0);
signal signSignal :  std_logic;
signal passedInput :  std_logic_vector(23 downto 0);
signal input2LZOC :  std_logic_vector(22 downto 0);
signal temporalExponent :  std_logic_vector(4 downto 0);
signal temporalFraction :  std_logic_vector(26 downto 0);
signal MSB2Signal :  std_logic_vector(7 downto 0);
signal zeroPadding4Exponent :  std_logic_vector(2 downto 0);
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
signal minusOne4ZD :  std_logic_vector(23 downto 0);
signal zeroDS :  std_logic_vector(23 downto 0);
signal zeroInput :  std_logic;
signal sign2vector :  std_logic_vector(26 downto 0);
signal tempConvert :  std_logic_vector(26 downto 0);
signal tempConvert0 :  std_logic_vector(27 downto 0);
signal tempPaddingAddSign :  std_logic_vector(26 downto 0);
signal tempAddSign :  std_logic_vector(27 downto 0);
signal tempFractionResult :  std_logic_vector(27 downto 0);
signal correctingExponent :  std_logic;
signal fractionConverted :  std_logic_vector(22 downto 0);
signal firstBitofRest :  std_logic;
signal lastBitOfFraction :  std_logic;
signal minusOne :  std_logic_vector(1 downto 0);
signal fractionRemainder :  std_logic_vector(1 downto 0);
signal zeroFractionResult :  std_logic_vector(1 downto 0);
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
         end if;
      end process;
   input <= I;
   signSignal<=input(23);
   passedInput<=input(23 downto 0);
   input2LZOC<=passedInput(22 downto 0);
   LZOC_component: Fix2FP_0_23_S_8_23_F125_uid2_LZOCS  -- pipelineDepth=0 maxInDelay=0
      port map ( clk  => clk,
                 rst  => rst,
                 Count => temporalExponent,
                 I => input2LZOC,
                 O => temporalFraction,
                 OZb => signSignal);
   MSB2Signal<=CONV_STD_LOGIC_VECTOR(22,8);
   zeroPadding4Exponent<=CONV_STD_LOGIC_VECTOR(0,3);
   valueExponent<= not (zeroPadding4Exponent & temporalExponent );
   exponentConversion: Fix2FP_0_23_S_8_23_F125_uid2exponentConversion  -- pipelineDepth=0 maxInDelay=0
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
   exponentFinal: Fix2FP_0_23_S_8_23_F125_uid2exponentFinal  -- pipelineDepth=0 maxInDelay=0
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
   minusOne4ZD<=CONV_STD_LOGIC_VECTOR(-1,24);
   zeroD: Fix2FP_0_23_S_8_23_F125_uid2zeroD  -- pipelineDepth=0 maxInDelay=0
      port map ( clk  => clk,
                 rst  => rst,
                 Cin => '0',
                 R => zeroDS,
                 X => passedInput,
                 Y => minusOne4ZD);
   zeroInput<= zeroDS(23) and not(signSignal);
   ---------------- cycle 0----------------
   sign2vector<=(others => signSignal);
   tempConvert<= sign2vector xor temporalFraction;
   tempConvert0<= '0' & tempConvert;
   tempPaddingAddSign<=(others=>'0');
   tempAddSign<=tempPaddingAddSign & signSignal;
   fractionConverter: Fix2FP_0_23_S_8_23_F125_uid2_fractionConvert  -- pipelineDepth=0 maxInDelay=0
      port map ( clk  => clk,
                 rst  => rst,
                 Cin => '0',
                 R => tempFractionResult,
                 X => tempConvert0,
                 Y => tempAddSign);
   correctingExponent<=tempFractionResult(27);
   fractionConverted<=tempFractionResult(25 downto 3);
   firstBitofRest<=tempFractionResult(2);
   lastBitOfFraction<=tempFractionResult(3);
   ---------------- cycle 0----------------
   minusOne<=CONV_STD_LOGIC_VECTOR(-1,2);
   fractionRemainder<= tempFractionResult(1 downto 0);
   oneSubstracter: Fix2FP_0_23_S_8_23_F125_uid2_oneSubstracter  -- pipelineDepth=0 maxInDelay=0
      port map ( clk  => clk,
                 rst  => rst,
                 Cin => '0',
                 R => zeroFractionResult,
                 X => fractionRemainder,
                 Y => minusOne);
   zeroRemainder<= not( not (tempFractionResult(1)) and zeroFractionResult(1));
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
   roundingAdder: Fix2FP_0_23_S_8_23_F125_uid2roundingAdder  -- pipelineDepth=0 maxInDelay=0
      port map ( clk  => clk,
                 rst  => rst,
                 Cin => outputOfMux1,
                 R => roundedResult,
                 X => concatenationForRounding,
                 Y => possibleCorrector4Rounding);
   convertedExponentAfterRounding<= roundedResult(30 downto 23);
   convertedFractionAfterRounding<= roundedResult(22 downto 0);
   MSBSelection<= overflowSignal or roundedResult(31);
   LSBSelection<= not(underflowSignal and not(zeroInput));
   Selection<= MSBSelection & LSBSelection when zeroInput='0' else "00";
   specialBits <= Selection;
   O<= specialBits & signSignal & convertedExponentAfterRounding & convertedFractionAfterRounding;
end architecture;
