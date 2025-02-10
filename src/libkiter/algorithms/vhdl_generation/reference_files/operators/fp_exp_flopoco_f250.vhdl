--------------------------------------------------------------------------------
--                     FixRealKCM_F250_uid6_T0_F250_uid9
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Florent de Dinechin, Bogdan Pasca (2007-2020)
--------------------------------------------------------------------------------
-- combinatorial
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X
-- Output signals: Y

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity FixRealKCM_F250_uid6_T0_F250_uid9 is
    port (X : in  std_logic_vector(5 downto 0);
          Y : out  std_logic_vector(11 downto 0)   );
end entity;

architecture arch of FixRealKCM_F250_uid6_T0_F250_uid9 is
signal Y0 :  std_logic_vector(11 downto 0);
attribute ram_extract: string;
attribute ram_style: string;
attribute ram_extract of Y0: signal is "yes";
attribute ram_style of Y0: signal is "distributed";
signal Y1 :  std_logic_vector(11 downto 0);
begin
   with X  select  Y0 <= 
      "000000001000" when "000000",
      "000000110110" when "000001",
      "000001100100" when "000010",
      "000010010010" when "000011",
      "000011000001" when "000100",
      "000011101111" when "000101",
      "000100011101" when "000110",
      "000101001011" when "000111",
      "000101111001" when "001000",
      "000110100111" when "001001",
      "000111010110" when "001010",
      "001000000100" when "001011",
      "001000110010" when "001100",
      "001001100000" when "001101",
      "001010001110" when "001110",
      "001010111100" when "001111",
      "001011101011" when "010000",
      "001100011001" when "010001",
      "001101000111" when "010010",
      "001101110101" when "010011",
      "001110100011" when "010100",
      "001111010001" when "010101",
      "010000000000" when "010110",
      "010000101110" when "010111",
      "010001011100" when "011000",
      "010010001010" when "011001",
      "010010111000" when "011010",
      "010011100110" when "011011",
      "010100010101" when "011100",
      "010101000011" when "011101",
      "010101110001" when "011110",
      "010110011111" when "011111",
      "010111001101" when "100000",
      "010111111011" when "100001",
      "011000101010" when "100010",
      "011001011000" when "100011",
      "011010000110" when "100100",
      "011010110100" when "100101",
      "011011100010" when "100110",
      "011100010000" when "100111",
      "011100111111" when "101000",
      "011101101101" when "101001",
      "011110011011" when "101010",
      "011111001001" when "101011",
      "011111110111" when "101100",
      "100000100101" when "101101",
      "100001010100" when "101110",
      "100010000010" when "101111",
      "100010110000" when "110000",
      "100011011110" when "110001",
      "100100001100" when "110010",
      "100100111010" when "110011",
      "100101101001" when "110100",
      "100110010111" when "110101",
      "100111000101" when "110110",
      "100111110011" when "110111",
      "101000100001" when "111000",
      "101001001111" when "111001",
      "101001111110" when "111010",
      "101010101100" when "111011",
      "101011011010" when "111100",
      "101100001000" when "111101",
      "101100110110" when "111110",
      "101101100100" when "111111",
      "------------" when others;
   Y1 <= Y0; -- for the possible blockram register
   Y <= Y1;
end architecture;

--------------------------------------------------------------------------------
--                     FixRealKCM_F250_uid6_T1_F250_uid12
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Florent de Dinechin, Bogdan Pasca (2007-2020)
--------------------------------------------------------------------------------
-- combinatorial
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X
-- Output signals: Y

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity FixRealKCM_F250_uid6_T1_F250_uid12 is
    port (X : in  std_logic_vector(3 downto 0);
          Y : out  std_logic_vector(5 downto 0)   );
end entity;

architecture arch of FixRealKCM_F250_uid6_T1_F250_uid12 is
signal Y0 :  std_logic_vector(5 downto 0);
attribute ram_extract: string;
attribute ram_style: string;
attribute ram_extract of Y0: signal is "yes";
attribute ram_style of Y0: signal is "distributed";
signal Y1 :  std_logic_vector(5 downto 0);
begin
   with X  select  Y0 <= 
      "000000" when "0000",
      "000011" when "0001",
      "000110" when "0010",
      "001001" when "0011",
      "001100" when "0100",
      "001110" when "0101",
      "010001" when "0110",
      "010100" when "0111",
      "010111" when "1000",
      "011010" when "1001",
      "011101" when "1010",
      "100000" when "1011",
      "100011" when "1100",
      "100110" when "1101",
      "101000" when "1110",
      "101011" when "1111",
      "------" when others;
   Y1 <= Y0; -- for the possible blockram register
   Y <= Y1;
end architecture;

--------------------------------------------------------------------------------
--                    FixRealKCM_F250_uid18_T0_F250_uid21
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Florent de Dinechin, Bogdan Pasca (2007-2020)
--------------------------------------------------------------------------------
-- combinatorial
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X
-- Output signals: Y

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity FixRealKCM_F250_uid18_T0_F250_uid21 is
    port (X : in  std_logic_vector(5 downto 0);
          Y : out  std_logic_vector(34 downto 0)   );
end entity;

architecture arch of FixRealKCM_F250_uid18_T0_F250_uid21 is
signal Y0 :  std_logic_vector(34 downto 0);
attribute ram_extract: string;
attribute ram_style: string;
attribute ram_extract of Y0: signal is "yes";
attribute ram_style of Y0: signal is "distributed";
signal Y1 :  std_logic_vector(34 downto 0);
begin
   with X  select  Y0 <= 
      "00000000000000000000000000000000000" when "000000",
      "00000010110001011100100001011111111" when "000001",
      "00000101100010111001000010111111110" when "000010",
      "00001000010100010101100100011111101" when "000011",
      "00001011000101110010000101111111100" when "000100",
      "00001101110111001110100111011111011" when "000101",
      "00010000101000101011001000111111010" when "000110",
      "00010011011010000111101010011111001" when "000111",
      "00010110001011100100001011111111000" when "001000",
      "00011000111101000000101101011110111" when "001001",
      "00011011101110011101001110111110110" when "001010",
      "00011110011111111001110000011110101" when "001011",
      "00100001010001010110010001111110100" when "001100",
      "00100100000010110010110011011110011" when "001101",
      "00100110110100001111010100111110010" when "001110",
      "00101001100101101011110110011110001" when "001111",
      "00101100010111001000010111111110000" when "010000",
      "00101111001000100100111001011101111" when "010001",
      "00110001111010000001011010111101110" when "010010",
      "00110100101011011101111100011101101" when "010011",
      "00110111011100111010011101111101100" when "010100",
      "00111010001110010110111111011101011" when "010101",
      "00111100111111110011100000111101010" when "010110",
      "00111111110001010000000010011101000" when "010111",
      "01000010100010101100100011111100111" when "011000",
      "01000101010100001001000101011100110" when "011001",
      "01001000000101100101100110111100101" when "011010",
      "01001010110111000010001000011100100" when "011011",
      "01001101101000011110101001111100011" when "011100",
      "01010000011001111011001011011100010" when "011101",
      "01010011001011010111101100111100001" when "011110",
      "01010101111100110100001110011100000" when "011111",
      "01011000101110010000101111111011111" when "100000",
      "01011011011111101101010001011011110" when "100001",
      "01011110010001001001110010111011101" when "100010",
      "01100001000010100110010100011011100" when "100011",
      "01100011110100000010110101111011011" when "100100",
      "01100110100101011111010111011011010" when "100101",
      "01101001010110111011111000111011001" when "100110",
      "01101100001000011000011010011011000" when "100111",
      "01101110111001110100111011111010111" when "101000",
      "01110001101011010001011101011010110" when "101001",
      "01110100011100101101111110111010101" when "101010",
      "01110111001110001010100000011010100" when "101011",
      "01111001111111100111000001111010011" when "101100",
      "01111100110001000011100011011010010" when "101101",
      "01111111100010100000000100111010001" when "101110",
      "10000010010011111100100110011010000" when "101111",
      "10000101000101011001000111111001111" when "110000",
      "10000111110110110101101001011001110" when "110001",
      "10001010101000010010001010111001101" when "110010",
      "10001101011001101110101100011001100" when "110011",
      "10010000001011001011001101111001011" when "110100",
      "10010010111100100111101111011001010" when "110101",
      "10010101101110000100010000111001001" when "110110",
      "10011000011111100000110010011001000" when "110111",
      "10011011010000111101010011111000111" when "111000",
      "10011110000010011001110101011000110" when "111001",
      "10100000110011110110010110111000101" when "111010",
      "10100011100101010010111000011000100" when "111011",
      "10100110010110101111011001111000011" when "111100",
      "10101001001000001011111011011000010" when "111101",
      "10101011111001101000011100111000001" when "111110",
      "10101110101011000100111110011000000" when "111111",
      "-----------------------------------" when others;
   Y1 <= Y0; -- for the possible blockram register
   Y <= Y1;
end architecture;

--------------------------------------------------------------------------------
--                    FixRealKCM_F250_uid18_T1_F250_uid24
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Florent de Dinechin, Bogdan Pasca (2007-2020)
--------------------------------------------------------------------------------
-- combinatorial
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X
-- Output signals: Y

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity FixRealKCM_F250_uid18_T1_F250_uid24 is
    port (X : in  std_logic_vector(1 downto 0);
          Y : out  std_logic_vector(28 downto 0)   );
end entity;

architecture arch of FixRealKCM_F250_uid18_T1_F250_uid24 is
signal Y0 :  std_logic_vector(28 downto 0);
attribute ram_extract: string;
attribute ram_style: string;
attribute ram_extract of Y0: signal is "yes";
attribute ram_style of Y0: signal is "distributed";
signal Y1 :  std_logic_vector(28 downto 0);
begin
   with X  select  Y0 <= 
      "00000000000000000000000000000" when "00",
      "00101100010111001000011000000" when "01",
      "01011000101110010000101111111" when "10",
      "10000101000101011001000111111" when "11",
      "-----------------------------" when others;
   Y1 <= Y0; -- for the possible blockram register
   Y <= Y1;
end architecture;

--------------------------------------------------------------------------------
--                          ExpZmZm1Table_F250_uid35
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Florent de Dinechin, Bogdan Pasca (2007-2020)
--------------------------------------------------------------------------------
-- combinatorial
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X
-- Output signals: Y

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity ExpZmZm1Table_F250_uid35 is
    port (X : in  std_logic_vector(6 downto 0);
          Y : out  std_logic_vector(5 downto 0)   );
end entity;

architecture arch of ExpZmZm1Table_F250_uid35 is
signal Y0 :  std_logic_vector(5 downto 0);
attribute ram_extract: string;
attribute ram_style: string;
attribute ram_extract of Y0: signal is "yes";
attribute ram_style of Y0: signal is "distributed";
signal Y1 :  std_logic_vector(5 downto 0);
begin
   with X  select  Y0 <= 
      "000000" when "0000000",
      "000000" when "0000001",
      "000000" when "0000010",
      "000000" when "0000011",
      "000000" when "0000100",
      "000000" when "0000101",
      "000000" when "0000110",
      "000000" when "0000111",
      "000000" when "0001000",
      "000000" when "0001001",
      "000000" when "0001010",
      "000000" when "0001011",
      "000001" when "0001100",
      "000001" when "0001101",
      "000001" when "0001110",
      "000001" when "0001111",
      "000001" when "0010000",
      "000001" when "0010001",
      "000001" when "0010010",
      "000001" when "0010011",
      "000010" when "0010100",
      "000010" when "0010101",
      "000010" when "0010110",
      "000010" when "0010111",
      "000010" when "0011000",
      "000010" when "0011001",
      "000011" when "0011010",
      "000011" when "0011011",
      "000011" when "0011100",
      "000011" when "0011101",
      "000100" when "0011110",
      "000100" when "0011111",
      "000100" when "0100000",
      "000100" when "0100001",
      "000101" when "0100010",
      "000101" when "0100011",
      "000101" when "0100100",
      "000101" when "0100101",
      "000110" when "0100110",
      "000110" when "0100111",
      "000110" when "0101000",
      "000111" when "0101001",
      "000111" when "0101010",
      "000111" when "0101011",
      "001000" when "0101100",
      "001000" when "0101101",
      "001000" when "0101110",
      "001001" when "0101111",
      "001001" when "0110000",
      "001001" when "0110001",
      "001010" when "0110010",
      "001010" when "0110011",
      "001011" when "0110100",
      "001011" when "0110101",
      "001011" when "0110110",
      "001100" when "0110111",
      "001100" when "0111000",
      "001101" when "0111001",
      "001101" when "0111010",
      "001110" when "0111011",
      "001110" when "0111100",
      "001111" when "0111101",
      "001111" when "0111110",
      "010000" when "0111111",
      "010000" when "1000000",
      "010001" when "1000001",
      "010001" when "1000010",
      "010010" when "1000011",
      "010010" when "1000100",
      "010011" when "1000101",
      "010011" when "1000110",
      "010100" when "1000111",
      "010100" when "1001000",
      "010101" when "1001001",
      "010101" when "1001010",
      "010110" when "1001011",
      "010111" when "1001100",
      "010111" when "1001101",
      "011000" when "1001110",
      "011000" when "1001111",
      "011001" when "1010000",
      "011010" when "1010001",
      "011010" when "1010010",
      "011011" when "1010011",
      "011100" when "1010100",
      "011100" when "1010101",
      "011101" when "1010110",
      "011110" when "1010111",
      "011110" when "1011000",
      "011111" when "1011001",
      "100000" when "1011010",
      "100000" when "1011011",
      "100001" when "1011100",
      "100010" when "1011101",
      "100011" when "1011110",
      "100011" when "1011111",
      "100100" when "1100000",
      "100101" when "1100001",
      "100110" when "1100010",
      "100110" when "1100011",
      "100111" when "1100100",
      "101000" when "1100101",
      "101001" when "1100110",
      "101001" when "1100111",
      "101010" when "1101000",
      "101011" when "1101001",
      "101100" when "1101010",
      "101101" when "1101011",
      "101110" when "1101100",
      "101110" when "1101101",
      "101111" when "1101110",
      "110000" when "1101111",
      "110001" when "1110000",
      "110010" when "1110001",
      "110011" when "1110010",
      "110100" when "1110011",
      "110101" when "1110100",
      "110101" when "1110101",
      "110110" when "1110110",
      "110111" when "1110111",
      "111000" when "1111000",
      "111001" when "1111001",
      "111010" when "1111010",
      "111011" when "1111011",
      "111100" when "1111100",
      "111101" when "1111101",
      "111110" when "1111110",
      "111111" when "1111111",
      "------" when others;
   Y1 <= Y0; -- for the possible blockram register
   Y <= Y1;
end architecture;

--------------------------------------------------------------------------------
--                         Compressor_6_3_F250_uid76
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: 
--------------------------------------------------------------------------------
-- combinatorial
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X0
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity Compressor_6_3_F250_uid76 is
    port (X0 : in  std_logic_vector(5 downto 0);
          R : out  std_logic_vector(2 downto 0)   );
end entity;

architecture arch of Compressor_6_3_F250_uid76 is
signal X :  std_logic_vector(5 downto 0);
signal R0 :  std_logic_vector(2 downto 0);
begin
   X <= X0 ;

   with X  select  R0 <= 
      "000" when "000000",
      "001" when "000001" | "000010" | "000100" | "001000" | "010000" | "100000",
      "010" when "000011" | "000101" | "000110" | "001001" | "001010" | "001100" | "010001" | "010010" | "010100" | "011000" | "100001" | "100010" | "100100" | "101000" | "110000",
      "011" when "000111" | "001011" | "001101" | "001110" | "010011" | "010101" | "010110" | "011001" | "011010" | "011100" | "100011" | "100101" | "100110" | "101001" | "101010" | "101100" | "110001" | "110010" | "110100" | "111000",
      "100" when "001111" | "010111" | "011011" | "011101" | "011110" | "100111" | "101011" | "101101" | "101110" | "110011" | "110101" | "110110" | "111001" | "111010" | "111100",
      "101" when "011111" | "101111" | "110111" | "111011" | "111101" | "111110",
      "110" when "111111",
      "---" when others;
   R <= R0;
end architecture;

--------------------------------------------------------------------------------
--                         Compressor_14_3_F250_uid80
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: 
--------------------------------------------------------------------------------
-- combinatorial
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X1 X0
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity Compressor_14_3_F250_uid80 is
    port (X1 : in  std_logic_vector(0 downto 0);
          X0 : in  std_logic_vector(3 downto 0);
          R : out  std_logic_vector(2 downto 0)   );
end entity;

architecture arch of Compressor_14_3_F250_uid80 is
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
--                         Compressor_23_3_F250_uid92
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: 
--------------------------------------------------------------------------------
-- combinatorial
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X1 X0
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity Compressor_23_3_F250_uid92 is
    port (X1 : in  std_logic_vector(1 downto 0);
          X0 : in  std_logic_vector(2 downto 0);
          R : out  std_logic_vector(2 downto 0)   );
end entity;

architecture arch of Compressor_23_3_F250_uid92 is
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
--                         Compressor_3_2_F250_uid118
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: 
--------------------------------------------------------------------------------
-- combinatorial
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X0
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity Compressor_3_2_F250_uid118 is
    port (X0 : in  std_logic_vector(2 downto 0);
          R : out  std_logic_vector(1 downto 0)   );
end entity;

architecture arch of Compressor_3_2_F250_uid118 is
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
--                     LeftShifter24_by_max_33_F250_uid4
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

entity LeftShifter24_by_max_33_F250_uid4 is
    port (clk : in std_logic;
          X : in  std_logic_vector(23 downto 0);
          S : in  std_logic_vector(5 downto 0);
          R : out  std_logic_vector(56 downto 0)   );
end entity;

architecture arch of LeftShifter24_by_max_33_F250_uid4 is
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
   R <= level6(56 downto 0);
end architecture;

--------------------------------------------------------------------------------
--                           IntAdder_13_F250_uid16
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Bogdan Pasca, Florent de Dinechin (2008-2016)
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X Y Cin
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity IntAdder_13_F250_uid16 is
    port (clk : in std_logic;
          X : in  std_logic_vector(12 downto 0);
          Y : in  std_logic_vector(12 downto 0);
          Cin : in  std_logic;
          R : out  std_logic_vector(12 downto 0)   );
end entity;

architecture arch of IntAdder_13_F250_uid16 is
signal Rtmp :  std_logic_vector(12 downto 0);
signal Cin_d1 :  std_logic;
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            Cin_d1 <=  Cin;
         end if;
      end process;
   Rtmp <= X + Y + Cin_d1;
   R <= Rtmp;
end architecture;

--------------------------------------------------------------------------------
--                            FixRealKCM_F250_uid6
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Florent de Dinechin (2007-2016)
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity FixRealKCM_F250_uid6 is
    port (clk : in std_logic;
          X : in  std_logic_vector(9 downto 0);
          R : out  std_logic_vector(7 downto 0)   );
end entity;

architecture arch of FixRealKCM_F250_uid6 is
   component FixRealKCM_F250_uid6_T0_F250_uid9 is
      port ( X : in  std_logic_vector(5 downto 0);
             Y : out  std_logic_vector(11 downto 0)   );
   end component;

   component FixRealKCM_F250_uid6_T1_F250_uid12 is
      port ( X : in  std_logic_vector(3 downto 0);
             Y : out  std_logic_vector(5 downto 0)   );
   end component;

   component IntAdder_13_F250_uid16 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(12 downto 0);
             Y : in  std_logic_vector(12 downto 0);
             Cin : in  std_logic;
             R : out  std_logic_vector(12 downto 0)   );
   end component;

signal FixRealKCM_F250_uid6_A0 :  std_logic_vector(5 downto 0);
signal FixRealKCM_F250_uid6_T0 :  std_logic_vector(11 downto 0);
signal FixRealKCM_F250_uid6_T0_copy10 :  std_logic_vector(11 downto 0);
signal bh7_w0_0 :  std_logic;
signal bh7_w1_0 :  std_logic;
signal bh7_w2_0 :  std_logic;
signal bh7_w3_0 :  std_logic;
signal bh7_w4_0 :  std_logic;
signal bh7_w5_0 :  std_logic;
signal bh7_w6_0 :  std_logic;
signal bh7_w7_0 :  std_logic;
signal bh7_w8_0 :  std_logic;
signal bh7_w9_0 :  std_logic;
signal bh7_w10_0 :  std_logic;
signal bh7_w11_0 :  std_logic;
signal FixRealKCM_F250_uid6_A1 :  std_logic_vector(3 downto 0);
signal FixRealKCM_F250_uid6_T1 :  std_logic_vector(5 downto 0);
signal FixRealKCM_F250_uid6_T1_copy13 :  std_logic_vector(5 downto 0);
signal bh7_w0_1 :  std_logic;
signal bh7_w1_1 :  std_logic;
signal bh7_w2_1 :  std_logic;
signal bh7_w3_1 :  std_logic;
signal bh7_w4_1 :  std_logic;
signal bh7_w5_1 :  std_logic;
signal bitheapFinalAdd_bh7_In0 :  std_logic_vector(12 downto 0);
signal bitheapFinalAdd_bh7_In1 :  std_logic_vector(12 downto 0);
signal bitheapFinalAdd_bh7_Cin :  std_logic;
signal bitheapFinalAdd_bh7_Out :  std_logic_vector(12 downto 0);
signal bitheapResult_bh7 :  std_logic_vector(11 downto 0);
signal OutRes :  std_logic_vector(11 downto 0);
begin
-- This operator multiplies by 1/log(2)
   FixRealKCM_F250_uid6_A0 <= X(9 downto 4);-- input address  m=6  l=1
   FixRealKCM_F250_uid6_Table0: FixRealKCM_F250_uid6_T0_F250_uid9
      port map ( X => FixRealKCM_F250_uid6_A0,
                 Y => FixRealKCM_F250_uid6_T0_copy10);
   FixRealKCM_F250_uid6_T0 <= FixRealKCM_F250_uid6_T0_copy10; -- output copy to hold a pipeline register if needed
   bh7_w0_0 <= FixRealKCM_F250_uid6_T0(0);
   bh7_w1_0 <= FixRealKCM_F250_uid6_T0(1);
   bh7_w2_0 <= FixRealKCM_F250_uid6_T0(2);
   bh7_w3_0 <= FixRealKCM_F250_uid6_T0(3);
   bh7_w4_0 <= FixRealKCM_F250_uid6_T0(4);
   bh7_w5_0 <= FixRealKCM_F250_uid6_T0(5);
   bh7_w6_0 <= FixRealKCM_F250_uid6_T0(6);
   bh7_w7_0 <= FixRealKCM_F250_uid6_T0(7);
   bh7_w8_0 <= FixRealKCM_F250_uid6_T0(8);
   bh7_w9_0 <= FixRealKCM_F250_uid6_T0(9);
   bh7_w10_0 <= FixRealKCM_F250_uid6_T0(10);
   bh7_w11_0 <= FixRealKCM_F250_uid6_T0(11);
   FixRealKCM_F250_uid6_A1 <= X(3 downto 0);-- input address  m=0  l=-3
   FixRealKCM_F250_uid6_Table1: FixRealKCM_F250_uid6_T1_F250_uid12
      port map ( X => FixRealKCM_F250_uid6_A1,
                 Y => FixRealKCM_F250_uid6_T1_copy13);
   FixRealKCM_F250_uid6_T1 <= FixRealKCM_F250_uid6_T1_copy13; -- output copy to hold a pipeline register if needed
   bh7_w0_1 <= FixRealKCM_F250_uid6_T1(0);
   bh7_w1_1 <= FixRealKCM_F250_uid6_T1(1);
   bh7_w2_1 <= FixRealKCM_F250_uid6_T1(2);
   bh7_w3_1 <= FixRealKCM_F250_uid6_T1(3);
   bh7_w4_1 <= FixRealKCM_F250_uid6_T1(4);
   bh7_w5_1 <= FixRealKCM_F250_uid6_T1(5);

   -- Adding the constant bits 
      -- All the constant bits are zero, nothing to add


   bitheapFinalAdd_bh7_In0 <= "0" & bh7_w11_0 & bh7_w10_0 & bh7_w9_0 & bh7_w8_0 & bh7_w7_0 & bh7_w6_0 & bh7_w5_1 & bh7_w4_1 & bh7_w3_1 & bh7_w2_1 & bh7_w1_1 & bh7_w0_1;
   bitheapFinalAdd_bh7_In1 <= "0" & "0" & "0" & "0" & "0" & "0" & "0" & bh7_w5_0 & bh7_w4_0 & bh7_w3_0 & bh7_w2_0 & bh7_w1_0 & bh7_w0_0;
   bitheapFinalAdd_bh7_Cin <= '0';

   bitheapFinalAdd_bh7: IntAdder_13_F250_uid16
      port map ( clk  => clk,
                 Cin => bitheapFinalAdd_bh7_Cin,
                 X => bitheapFinalAdd_bh7_In0,
                 Y => bitheapFinalAdd_bh7_In1,
                 R => bitheapFinalAdd_bh7_Out);
   bitheapResult_bh7 <= bitheapFinalAdd_bh7_Out(11 downto 0);
   OutRes <= bitheapResult_bh7(11 downto 0);
   R <= OutRes(11 downto 4);
end architecture;

--------------------------------------------------------------------------------
--                           IntAdder_36_F250_uid28
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Bogdan Pasca, Florent de Dinechin (2008-2016)
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X Y Cin
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity IntAdder_36_F250_uid28 is
    port (clk : in std_logic;
          X : in  std_logic_vector(35 downto 0);
          Y : in  std_logic_vector(35 downto 0);
          Cin : in  std_logic;
          R : out  std_logic_vector(35 downto 0)   );
end entity;

architecture arch of IntAdder_36_F250_uid28 is
signal Rtmp :  std_logic_vector(35 downto 0);
signal Cin_d1, Cin_d2 :  std_logic;
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            Cin_d1 <=  Cin;
            Cin_d2 <=  Cin_d1;
         end if;
      end process;
   Rtmp <= X + Y + Cin_d2;
   R <= Rtmp;
end architecture;

--------------------------------------------------------------------------------
--                           FixRealKCM_F250_uid18
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Florent de Dinechin (2007-2016)
--------------------------------------------------------------------------------
-- Pipeline depth: 1 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity FixRealKCM_F250_uid18 is
    port (clk : in std_logic;
          X : in  std_logic_vector(7 downto 0);
          R : out  std_logic_vector(34 downto 0)   );
end entity;

architecture arch of FixRealKCM_F250_uid18 is
   component FixRealKCM_F250_uid18_T0_F250_uid21 is
      port ( X : in  std_logic_vector(5 downto 0);
             Y : out  std_logic_vector(34 downto 0)   );
   end component;

   component FixRealKCM_F250_uid18_T1_F250_uid24 is
      port ( X : in  std_logic_vector(1 downto 0);
             Y : out  std_logic_vector(28 downto 0)   );
   end component;

   component IntAdder_36_F250_uid28 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(35 downto 0);
             Y : in  std_logic_vector(35 downto 0);
             Cin : in  std_logic;
             R : out  std_logic_vector(35 downto 0)   );
   end component;

signal FixRealKCM_F250_uid18_A0 :  std_logic_vector(5 downto 0);
signal FixRealKCM_F250_uid18_T0 :  std_logic_vector(34 downto 0);
signal FixRealKCM_F250_uid18_T0_copy22, FixRealKCM_F250_uid18_T0_copy22_d1 :  std_logic_vector(34 downto 0);
signal bh19_w0_0 :  std_logic;
signal bh19_w1_0 :  std_logic;
signal bh19_w2_0 :  std_logic;
signal bh19_w3_0 :  std_logic;
signal bh19_w4_0 :  std_logic;
signal bh19_w5_0 :  std_logic;
signal bh19_w6_0 :  std_logic;
signal bh19_w7_0 :  std_logic;
signal bh19_w8_0 :  std_logic;
signal bh19_w9_0 :  std_logic;
signal bh19_w10_0 :  std_logic;
signal bh19_w11_0 :  std_logic;
signal bh19_w12_0 :  std_logic;
signal bh19_w13_0 :  std_logic;
signal bh19_w14_0 :  std_logic;
signal bh19_w15_0 :  std_logic;
signal bh19_w16_0 :  std_logic;
signal bh19_w17_0 :  std_logic;
signal bh19_w18_0 :  std_logic;
signal bh19_w19_0 :  std_logic;
signal bh19_w20_0 :  std_logic;
signal bh19_w21_0 :  std_logic;
signal bh19_w22_0 :  std_logic;
signal bh19_w23_0 :  std_logic;
signal bh19_w24_0 :  std_logic;
signal bh19_w25_0 :  std_logic;
signal bh19_w26_0 :  std_logic;
signal bh19_w27_0 :  std_logic;
signal bh19_w28_0 :  std_logic;
signal bh19_w29_0 :  std_logic;
signal bh19_w30_0 :  std_logic;
signal bh19_w31_0 :  std_logic;
signal bh19_w32_0 :  std_logic;
signal bh19_w33_0 :  std_logic;
signal bh19_w34_0 :  std_logic;
signal FixRealKCM_F250_uid18_A1 :  std_logic_vector(1 downto 0);
signal FixRealKCM_F250_uid18_T1 :  std_logic_vector(28 downto 0);
signal FixRealKCM_F250_uid18_T1_copy25, FixRealKCM_F250_uid18_T1_copy25_d1 :  std_logic_vector(28 downto 0);
signal bh19_w0_1 :  std_logic;
signal bh19_w1_1 :  std_logic;
signal bh19_w2_1 :  std_logic;
signal bh19_w3_1 :  std_logic;
signal bh19_w4_1 :  std_logic;
signal bh19_w5_1 :  std_logic;
signal bh19_w6_1 :  std_logic;
signal bh19_w7_1 :  std_logic;
signal bh19_w8_1 :  std_logic;
signal bh19_w9_1 :  std_logic;
signal bh19_w10_1 :  std_logic;
signal bh19_w11_1 :  std_logic;
signal bh19_w12_1 :  std_logic;
signal bh19_w13_1 :  std_logic;
signal bh19_w14_1 :  std_logic;
signal bh19_w15_1 :  std_logic;
signal bh19_w16_1 :  std_logic;
signal bh19_w17_1 :  std_logic;
signal bh19_w18_1 :  std_logic;
signal bh19_w19_1 :  std_logic;
signal bh19_w20_1 :  std_logic;
signal bh19_w21_1 :  std_logic;
signal bh19_w22_1 :  std_logic;
signal bh19_w23_1 :  std_logic;
signal bh19_w24_1 :  std_logic;
signal bh19_w25_1 :  std_logic;
signal bh19_w26_1 :  std_logic;
signal bh19_w27_1 :  std_logic;
signal bh19_w28_1 :  std_logic;
signal bitheapFinalAdd_bh19_In0 :  std_logic_vector(35 downto 0);
signal bitheapFinalAdd_bh19_In1 :  std_logic_vector(35 downto 0);
signal bitheapFinalAdd_bh19_Cin :  std_logic;
signal bitheapFinalAdd_bh19_Out :  std_logic_vector(35 downto 0);
signal bitheapResult_bh19 :  std_logic_vector(34 downto 0);
signal OutRes :  std_logic_vector(34 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            FixRealKCM_F250_uid18_T0_copy22_d1 <=  FixRealKCM_F250_uid18_T0_copy22;
            FixRealKCM_F250_uid18_T1_copy25_d1 <=  FixRealKCM_F250_uid18_T1_copy25;
         end if;
      end process;
-- This operator multiplies by log(2)
   FixRealKCM_F250_uid18_A0 <= X(7 downto 2);-- input address  m=7  l=2
   FixRealKCM_F250_uid18_Table0: FixRealKCM_F250_uid18_T0_F250_uid21
      port map ( X => FixRealKCM_F250_uid18_A0,
                 Y => FixRealKCM_F250_uid18_T0_copy22);
   FixRealKCM_F250_uid18_T0 <= FixRealKCM_F250_uid18_T0_copy22_d1; -- output copy to hold a pipeline register if needed
   bh19_w0_0 <= FixRealKCM_F250_uid18_T0(0);
   bh19_w1_0 <= FixRealKCM_F250_uid18_T0(1);
   bh19_w2_0 <= FixRealKCM_F250_uid18_T0(2);
   bh19_w3_0 <= FixRealKCM_F250_uid18_T0(3);
   bh19_w4_0 <= FixRealKCM_F250_uid18_T0(4);
   bh19_w5_0 <= FixRealKCM_F250_uid18_T0(5);
   bh19_w6_0 <= FixRealKCM_F250_uid18_T0(6);
   bh19_w7_0 <= FixRealKCM_F250_uid18_T0(7);
   bh19_w8_0 <= FixRealKCM_F250_uid18_T0(8);
   bh19_w9_0 <= FixRealKCM_F250_uid18_T0(9);
   bh19_w10_0 <= FixRealKCM_F250_uid18_T0(10);
   bh19_w11_0 <= FixRealKCM_F250_uid18_T0(11);
   bh19_w12_0 <= FixRealKCM_F250_uid18_T0(12);
   bh19_w13_0 <= FixRealKCM_F250_uid18_T0(13);
   bh19_w14_0 <= FixRealKCM_F250_uid18_T0(14);
   bh19_w15_0 <= FixRealKCM_F250_uid18_T0(15);
   bh19_w16_0 <= FixRealKCM_F250_uid18_T0(16);
   bh19_w17_0 <= FixRealKCM_F250_uid18_T0(17);
   bh19_w18_0 <= FixRealKCM_F250_uid18_T0(18);
   bh19_w19_0 <= FixRealKCM_F250_uid18_T0(19);
   bh19_w20_0 <= FixRealKCM_F250_uid18_T0(20);
   bh19_w21_0 <= FixRealKCM_F250_uid18_T0(21);
   bh19_w22_0 <= FixRealKCM_F250_uid18_T0(22);
   bh19_w23_0 <= FixRealKCM_F250_uid18_T0(23);
   bh19_w24_0 <= FixRealKCM_F250_uid18_T0(24);
   bh19_w25_0 <= FixRealKCM_F250_uid18_T0(25);
   bh19_w26_0 <= FixRealKCM_F250_uid18_T0(26);
   bh19_w27_0 <= FixRealKCM_F250_uid18_T0(27);
   bh19_w28_0 <= FixRealKCM_F250_uid18_T0(28);
   bh19_w29_0 <= FixRealKCM_F250_uid18_T0(29);
   bh19_w30_0 <= FixRealKCM_F250_uid18_T0(30);
   bh19_w31_0 <= FixRealKCM_F250_uid18_T0(31);
   bh19_w32_0 <= FixRealKCM_F250_uid18_T0(32);
   bh19_w33_0 <= FixRealKCM_F250_uid18_T0(33);
   bh19_w34_0 <= FixRealKCM_F250_uid18_T0(34);
   FixRealKCM_F250_uid18_A1 <= X(1 downto 0);-- input address  m=1  l=0
   FixRealKCM_F250_uid18_Table1: FixRealKCM_F250_uid18_T1_F250_uid24
      port map ( X => FixRealKCM_F250_uid18_A1,
                 Y => FixRealKCM_F250_uid18_T1_copy25);
   FixRealKCM_F250_uid18_T1 <= FixRealKCM_F250_uid18_T1_copy25_d1; -- output copy to hold a pipeline register if needed
   bh19_w0_1 <= FixRealKCM_F250_uid18_T1(0);
   bh19_w1_1 <= FixRealKCM_F250_uid18_T1(1);
   bh19_w2_1 <= FixRealKCM_F250_uid18_T1(2);
   bh19_w3_1 <= FixRealKCM_F250_uid18_T1(3);
   bh19_w4_1 <= FixRealKCM_F250_uid18_T1(4);
   bh19_w5_1 <= FixRealKCM_F250_uid18_T1(5);
   bh19_w6_1 <= FixRealKCM_F250_uid18_T1(6);
   bh19_w7_1 <= FixRealKCM_F250_uid18_T1(7);
   bh19_w8_1 <= FixRealKCM_F250_uid18_T1(8);
   bh19_w9_1 <= FixRealKCM_F250_uid18_T1(9);
   bh19_w10_1 <= FixRealKCM_F250_uid18_T1(10);
   bh19_w11_1 <= FixRealKCM_F250_uid18_T1(11);
   bh19_w12_1 <= FixRealKCM_F250_uid18_T1(12);
   bh19_w13_1 <= FixRealKCM_F250_uid18_T1(13);
   bh19_w14_1 <= FixRealKCM_F250_uid18_T1(14);
   bh19_w15_1 <= FixRealKCM_F250_uid18_T1(15);
   bh19_w16_1 <= FixRealKCM_F250_uid18_T1(16);
   bh19_w17_1 <= FixRealKCM_F250_uid18_T1(17);
   bh19_w18_1 <= FixRealKCM_F250_uid18_T1(18);
   bh19_w19_1 <= FixRealKCM_F250_uid18_T1(19);
   bh19_w20_1 <= FixRealKCM_F250_uid18_T1(20);
   bh19_w21_1 <= FixRealKCM_F250_uid18_T1(21);
   bh19_w22_1 <= FixRealKCM_F250_uid18_T1(22);
   bh19_w23_1 <= FixRealKCM_F250_uid18_T1(23);
   bh19_w24_1 <= FixRealKCM_F250_uid18_T1(24);
   bh19_w25_1 <= FixRealKCM_F250_uid18_T1(25);
   bh19_w26_1 <= FixRealKCM_F250_uid18_T1(26);
   bh19_w27_1 <= FixRealKCM_F250_uid18_T1(27);
   bh19_w28_1 <= FixRealKCM_F250_uid18_T1(28);

   -- Adding the constant bits 
      -- All the constant bits are zero, nothing to add


   bitheapFinalAdd_bh19_In0 <= "0" & bh19_w34_0 & bh19_w33_0 & bh19_w32_0 & bh19_w31_0 & bh19_w30_0 & bh19_w29_0 & bh19_w28_1 & bh19_w27_1 & bh19_w26_1 & bh19_w25_1 & bh19_w24_1 & bh19_w23_1 & bh19_w22_1 & bh19_w21_1 & bh19_w20_1 & bh19_w19_1 & bh19_w18_1 & bh19_w17_1 & bh19_w16_1 & bh19_w15_1 & bh19_w14_1 & bh19_w13_1 & bh19_w12_1 & bh19_w11_1 & bh19_w10_1 & bh19_w9_1 & bh19_w8_1 & bh19_w7_1 & bh19_w6_1 & bh19_w5_1 & bh19_w4_1 & bh19_w3_1 & bh19_w2_1 & bh19_w1_1 & bh19_w0_1;
   bitheapFinalAdd_bh19_In1 <= "0" & "0" & "0" & "0" & "0" & "0" & "0" & bh19_w28_0 & bh19_w27_0 & bh19_w26_0 & bh19_w25_0 & bh19_w24_0 & bh19_w23_0 & bh19_w22_0 & bh19_w21_0 & bh19_w20_0 & bh19_w19_0 & bh19_w18_0 & bh19_w17_0 & bh19_w16_0 & bh19_w15_0 & bh19_w14_0 & bh19_w13_0 & bh19_w12_0 & bh19_w11_0 & bh19_w10_0 & bh19_w9_0 & bh19_w8_0 & bh19_w7_0 & bh19_w6_0 & bh19_w5_0 & bh19_w4_0 & bh19_w3_0 & bh19_w2_0 & bh19_w1_0 & bh19_w0_0;
   bitheapFinalAdd_bh19_Cin <= '0';

   bitheapFinalAdd_bh19: IntAdder_36_F250_uid28
      port map ( clk  => clk,
                 Cin => bitheapFinalAdd_bh19_Cin,
                 X => bitheapFinalAdd_bh19_In0,
                 Y => bitheapFinalAdd_bh19_In1,
                 R => bitheapFinalAdd_bh19_Out);
   bitheapResult_bh19 <= bitheapFinalAdd_bh19_Out(34 downto 0);
   OutRes <= bitheapResult_bh19(34 downto 0);
   R <= OutRes(34 downto 0);
end architecture;

--------------------------------------------------------------------------------
--                           IntAdder_27_F250_uid31
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Bogdan Pasca, Florent de Dinechin (2008-2016)
--------------------------------------------------------------------------------
-- Pipeline depth: 1 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X Y Cin
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity IntAdder_27_F250_uid31 is
    port (clk : in std_logic;
          X : in  std_logic_vector(26 downto 0);
          Y : in  std_logic_vector(26 downto 0);
          Cin : in  std_logic;
          R : out  std_logic_vector(26 downto 0)   );
end entity;

architecture arch of IntAdder_27_F250_uid31 is
signal Cin_1, Cin_1_d1, Cin_1_d2, Cin_1_d3 :  std_logic;
signal X_1, X_1_d1, X_1_d2 :  std_logic_vector(27 downto 0);
signal Y_1, Y_1_d1 :  std_logic_vector(27 downto 0);
signal S_1 :  std_logic_vector(27 downto 0);
signal R_1 :  std_logic_vector(26 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            Cin_1_d1 <=  Cin_1;
            Cin_1_d2 <=  Cin_1_d1;
            Cin_1_d3 <=  Cin_1_d2;
            X_1_d1 <=  X_1;
            X_1_d2 <=  X_1_d1;
            Y_1_d1 <=  Y_1;
         end if;
      end process;
   Cin_1 <= Cin;
   X_1 <= '0' & X(26 downto 0);
   Y_1 <= '0' & Y(26 downto 0);
   S_1 <= X_1_d2 + Y_1_d1 + Cin_1_d3;
   R_1 <= S_1(26 downto 0);
   R <= R_1 ;
end architecture;

--------------------------------------------------------------------------------
--                            ExpATable_F250_uid33
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Florent de Dinechin, Bogdan Pasca (2007-2020)
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X
-- Output signals: Y

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity ExpATable_F250_uid33 is
    port (clk : in std_logic;
          X : in  std_logic_vector(9 downto 0);
          Y : out  std_logic_vector(27 downto 0)   );
end entity;

architecture arch of ExpATable_F250_uid33 is
signal Y0 :  std_logic_vector(27 downto 0);
attribute ram_extract: string;
attribute ram_style: string;
attribute ram_extract of Y0: signal is "yes";
attribute ram_style of Y0: signal is "block";
signal Y1 :  std_logic_vector(27 downto 0);
begin
   with X  select  Y0 <= 
      "1000000000000000000000000000" when "0000000000",
      "1000000000100000000001000000" when "0000000001",
      "1000000001000000000100000000" when "0000000010",
      "1000000001100000001001000001" when "0000000011",
      "1000000010000000010000000001" when "0000000100",
      "1000000010100000011001000011" when "0000000101",
      "1000000011000000100100000101" when "0000000110",
      "1000000011100000110001000111" when "0000000111",
      "1000000100000001000000001011" when "0000001000",
      "1000000100100001010001001111" when "0000001001",
      "1000000101000001100100010101" when "0000001010",
      "1000000101100001111001011100" when "0000001011",
      "1000000110000010010000100100" when "0000001100",
      "1000000110100010101001101110" when "0000001101",
      "1000000111000011000100111001" when "0000001110",
      "1000000111100011100010000111" when "0000001111",
      "1000001000000100000001010110" when "0000010000",
      "1000001000100100100010100111" when "0000010001",
      "1000001001000101000101111010" when "0000010010",
      "1000001001100101101011010000" when "0000010011",
      "1000001010000110010010100111" when "0000010100",
      "1000001010100110111100000010" when "0000010101",
      "1000001011000111100111011111" when "0000010110",
      "1000001011101000010100111111" when "0000010111",
      "1000001100001001000100100010" when "0000011000",
      "1000001100101001110110001000" when "0000011001",
      "1000001101001010101001110001" when "0000011010",
      "1000001101101011011111011101" when "0000011011",
      "1000001110001100010111001100" when "0000011100",
      "1000001110101101010001000000" when "0000011101",
      "1000001111001110001100110111" when "0000011110",
      "1000001111101111001010110001" when "0000011111",
      "1000010000010000001010110000" when "0000100000",
      "1000010000110001001100110011" when "0000100001",
      "1000010001010010010000111010" when "0000100010",
      "1000010001110011010111000101" when "0000100011",
      "1000010010010100011111010101" when "0000100100",
      "1000010010110101101001101001" when "0000100101",
      "1000010011010110110110000010" when "0000100110",
      "1000010011111000000100100000" when "0000100111",
      "1000010100011001010101000010" when "0000101000",
      "1000010100111010100111101010" when "0000101001",
      "1000010101011011111100010111" when "0000101010",
      "1000010101111101010011001010" when "0000101011",
      "1000010110011110101100000010" when "0000101100",
      "1000010111000000000110111111" when "0000101101",
      "1000010111100001100100000011" when "0000101110",
      "1000011000000011000011001100" when "0000101111",
      "1000011000100100100100011011" when "0000110000",
      "1000011001000110000111110001" when "0000110001",
      "1000011001100111101101001100" when "0000110010",
      "1000011010001001010100101110" when "0000110011",
      "1000011010101010111110010111" when "0000110100",
      "1000011011001100101010000110" when "0000110101",
      "1000011011101110010111111100" when "0000110110",
      "1000011100010000000111111001" when "0000110111",
      "1000011100110001111001111101" when "0000111000",
      "1000011101010011101110001000" when "0000111001",
      "1000011101110101100100011011" when "0000111010",
      "1000011110010111011100110101" when "0000111011",
      "1000011110111001010111010111" when "0000111100",
      "1000011111011011010100000000" when "0000111101",
      "1000011111111101010010110001" when "0000111110",
      "1000100000011111010011101010" when "0000111111",
      "1000100001000001010110101100" when "0001000000",
      "1000100001100011011011110101" when "0001000001",
      "1000100010000101100011000111" when "0001000010",
      "1000100010100111101100100010" when "0001000011",
      "1000100011001001111000000101" when "0001000100",
      "1000100011101100000101110001" when "0001000101",
      "1000100100001110010101100110" when "0001000110",
      "1000100100110000100111100100" when "0001000111",
      "1000100101010010111011101011" when "0001001000",
      "1000100101110101010001111011" when "0001001001",
      "1000100110010111101010010101" when "0001001010",
      "1000100110111010000100111000" when "0001001011",
      "1000100111011100100001100110" when "0001001100",
      "1000100111111111000000011101" when "0001001101",
      "1000101000100001100001011110" when "0001001110",
      "1000101001000100000100101001" when "0001001111",
      "1000101001100110101001111110" when "0001010000",
      "1000101010001001010001011110" when "0001010001",
      "1000101010101011111011001001" when "0001010010",
      "1000101011001110100110111110" when "0001010011",
      "1000101011110001010100111101" when "0001010100",
      "1000101100010100000101001000" when "0001010101",
      "1000101100110110110111011110" when "0001010110",
      "1000101101011001101011111111" when "0001010111",
      "1000101101111100100010101100" when "0001011000",
      "1000101110011111011011100100" when "0001011001",
      "1000101111000010010110100111" when "0001011010",
      "1000101111100101010011110110" when "0001011011",
      "1000110000001000010011010010" when "0001011100",
      "1000110000101011010100111001" when "0001011101",
      "1000110001001110011000101100" when "0001011110",
      "1000110001110001011110101100" when "0001011111",
      "1000110010010100100110111000" when "0001100000",
      "1000110010110111110001010001" when "0001100001",
      "1000110011011010111101110110" when "0001100010",
      "1000110011111110001100101001" when "0001100011",
      "1000110100100001011101101000" when "0001100100",
      "1000110101000100110000110100" when "0001100101",
      "1000110101101000000110001110" when "0001100110",
      "1000110110001011011101110101" when "0001100111",
      "1000110110101110110111101010" when "0001101000",
      "1000110111010010010011101100" when "0001101001",
      "1000110111110101110001111100" when "0001101010",
      "1000111000011001010010011011" when "0001101011",
      "1000111000111100110101000111" when "0001101100",
      "1000111001100000011010000001" when "0001101101",
      "1000111010000100000001001010" when "0001101110",
      "1000111010100111101010100001" when "0001101111",
      "1000111011001011010110000111" when "0001110000",
      "1000111011101111000011111100" when "0001110001",
      "1000111100010010110100000000" when "0001110010",
      "1000111100110110100110010011" when "0001110011",
      "1000111101011010011010110101" when "0001110100",
      "1000111101111110010001100110" when "0001110101",
      "1000111110100010001010100111" when "0001110110",
      "1000111111000110000101111000" when "0001110111",
      "1000111111101010000011011000" when "0001111000",
      "1001000000001110000011001000" when "0001111001",
      "1001000000110010000101001000" when "0001111010",
      "1001000001010110001001011001" when "0001111011",
      "1001000001111010001111111010" when "0001111100",
      "1001000010011110011000101011" when "0001111101",
      "1001000011000010100011101101" when "0001111110",
      "1001000011100110110000111111" when "0001111111",
      "1001000100001011000000100011" when "0010000000",
      "1001000100101111010010010111" when "0010000001",
      "1001000101010011100110011101" when "0010000010",
      "1001000101110111111100110100" when "0010000011",
      "1001000110011100010101011101" when "0010000100",
      "1001000111000000110000010111" when "0010000101",
      "1001000111100101001101100011" when "0010000110",
      "1001001000001001101101000001" when "0010000111",
      "1001001000101110001110110001" when "0010001000",
      "1001001001010010110010110011" when "0010001001",
      "1001001001110111011001000111" when "0010001010",
      "1001001010011100000001101110" when "0010001011",
      "1001001011000000101100100111" when "0010001100",
      "1001001011100101011001110011" when "0010001101",
      "1001001100001010001001010011" when "0010001110",
      "1001001100101110111011000101" when "0010001111",
      "1001001101010011101111001010" when "0010010000",
      "1001001101111000100101100011" when "0010010001",
      "1001001110011101011110001111" when "0010010010",
      "1001001111000010011001001110" when "0010010011",
      "1001001111100111010110100010" when "0010010100",
      "1001010000001100010110001001" when "0010010101",
      "1001010000110001011000000101" when "0010010110",
      "1001010001010110011100010100" when "0010010111",
      "1001010001111011100010111000" when "0010011000",
      "1001010010100000101011110001" when "0010011001",
      "1001010011000101110110111110" when "0010011010",
      "1001010011101011000100100000" when "0010011011",
      "1001010100010000010100010110" when "0010011100",
      "1001010100110101100110100010" when "0010011101",
      "1001010101011010111011000011" when "0010011110",
      "1001010110000000010001111010" when "0010011111",
      "1001010110100101101011000110" when "0010100000",
      "1001010111001011000110100111" when "0010100001",
      "1001010111110000100100011110" when "0010100010",
      "1001011000010110000100101100" when "0010100011",
      "1001011000111011100111001111" when "0010100100",
      "1001011001100001001100001001" when "0010100101",
      "1001011010000110110011011001" when "0010100110",
      "1001011010101100011100111111" when "0010100111",
      "1001011011010010001000111100" when "0010101000",
      "1001011011110111110111010000" when "0010101001",
      "1001011100011101100111111011" when "0010101010",
      "1001011101000011011010111101" when "0010101011",
      "1001011101101001010000010111" when "0010101100",
      "1001011110001111001000000111" when "0010101101",
      "1001011110110101000010010000" when "0010101110",
      "1001011111011010111110110000" when "0010101111",
      "1001100000000000111101101000" when "0010110000",
      "1001100000100110111110111000" when "0010110001",
      "1001100001001101000010100000" when "0010110010",
      "1001100001110011001000100000" when "0010110011",
      "1001100010011001010000111001" when "0010110100",
      "1001100010111111011011101010" when "0010110101",
      "1001100011100101101000110100" when "0010110110",
      "1001100100001011111000010111" when "0010110111",
      "1001100100110010001010010011" when "0010111000",
      "1001100101011000011110101001" when "0010111001",
      "1001100101111110110101010111" when "0010111010",
      "1001100110100101001110011111" when "0010111011",
      "1001100111001011101010000001" when "0010111100",
      "1001100111110010000111111101" when "0010111101",
      "1001101000011000101000010010" when "0010111110",
      "1001101000111111001011000010" when "0010111111",
      "1001101001100101110000001100" when "0011000000",
      "1001101010001100010111110000" when "0011000001",
      "1001101010110011000001101111" when "0011000010",
      "1001101011011001101110001000" when "0011000011",
      "1001101100000000011100111100" when "0011000100",
      "1001101100100111001110001100" when "0011000101",
      "1001101101001110000001110110" when "0011000110",
      "1001101101110100110111111100" when "0011000111",
      "1001101110011011110000011101" when "0011001000",
      "1001101111000010101011011010" when "0011001001",
      "1001101111101001101000110011" when "0011001010",
      "1001110000010000101000100111" when "0011001011",
      "1001110000110111101010111000" when "0011001100",
      "1001110001011110101111100101" when "0011001101",
      "1001110010000101110110101110" when "0011001110",
      "1001110010101101000000010100" when "0011001111",
      "1001110011010100001100010110" when "0011010000",
      "1001110011111011011010110101" when "0011010001",
      "1001110100100010101011110001" when "0011010010",
      "1001110101001001111111001011" when "0011010011",
      "1001110101110001010101000001" when "0011010100",
      "1001110110011000101101010101" when "0011010101",
      "1001110111000000001000000111" when "0011010110",
      "1001110111100111100101010110" when "0011010111",
      "1001111000001111000101000100" when "0011011000",
      "1001111000110110100111001111" when "0011011001",
      "1001111001011110001011111001" when "0011011010",
      "1001111010000101110011000001" when "0011011011",
      "1001111010101101011100100111" when "0011011100",
      "1001111011010101001000101100" when "0011011101",
      "1001111011111100110111010000" when "0011011110",
      "1001111100100100101000010011" when "0011011111",
      "1001111101001100011011110101" when "0011100000",
      "1001111101110100010001110111" when "0011100001",
      "1001111110011100001010011000" when "0011100010",
      "1001111111000100000101011000" when "0011100011",
      "1001111111101100000010111000" when "0011100100",
      "1010000000010100000010111000" when "0011100101",
      "1010000000111100000101011001" when "0011100110",
      "1010000001100100001010011001" when "0011100111",
      "1010000010001100010001111010" when "0011101000",
      "1010000010110100011011111100" when "0011101001",
      "1010000011011100101000011110" when "0011101010",
      "1010000100000100110111100001" when "0011101011",
      "1010000100101101001001000101" when "0011101100",
      "1010000101010101011101001010" when "0011101101",
      "1010000101111101110011110000" when "0011101110",
      "1010000110100110001100111000" when "0011101111",
      "1010000111001110101000100010" when "0011110000",
      "1010000111110111000110101101" when "0011110001",
      "1010001000011111100111011011" when "0011110010",
      "1010001001001000001010101010" when "0011110011",
      "1010001001110000110000011100" when "0011110100",
      "1010001010011001011000110001" when "0011110101",
      "1010001011000010000011100111" when "0011110110",
      "1010001011101010110001000001" when "0011110111",
      "1010001100010011100000111110" when "0011111000",
      "1010001100111100010011011101" when "0011111001",
      "1010001101100101001000100000" when "0011111010",
      "1010001110001110000000000110" when "0011111011",
      "1010001110110110111010010000" when "0011111100",
      "1010001111011111110110111110" when "0011111101",
      "1010010000001000110110001111" when "0011111110",
      "1010010000110001111000000100" when "0011111111",
      "1010010001011010111100011110" when "0100000000",
      "1010010010000100000011011100" when "0100000001",
      "1010010010101101001100111111" when "0100000010",
      "1010010011010110011001000110" when "0100000011",
      "1010010011111111100111110010" when "0100000100",
      "1010010100101000111001000011" when "0100000101",
      "1010010101010010001100111001" when "0100000110",
      "1010010101111011100011010100" when "0100000111",
      "1010010110100100111100010101" when "0100001000",
      "1010010111001110010111111100" when "0100001001",
      "1010010111110111110110001000" when "0100001010",
      "1010011000100001010110111011" when "0100001011",
      "1010011001001010111010010011" when "0100001100",
      "1010011001110100100000010010" when "0100001101",
      "1010011010011110001000111000" when "0100001110",
      "1010011011000111110100000011" when "0100001111",
      "1010011011110001100001110110" when "0100010000",
      "1010011100011011010010010000" when "0100010001",
      "1010011101000101000101010000" when "0100010010",
      "1010011101101110111010111000" when "0100010011",
      "1010011110011000110011001000" when "0100010100",
      "1010011111000010101101111111" when "0100010101",
      "1010011111101100101011011110" when "0100010110",
      "1010100000010110101011100100" when "0100010111",
      "1010100001000000101110010011" when "0100011000",
      "1010100001101010110011101010" when "0100011001",
      "1010100010010100111011101010" when "0100011010",
      "1010100010111111000110010010" when "0100011011",
      "1010100011101001010011100010" when "0100011100",
      "1010100100010011100011011100" when "0100011101",
      "1010100100111101110101111111" when "0100011110",
      "1010100101101000001011001011" when "0100011111",
      "1010100110010010100011000000" when "0100100000",
      "1010100110111100111101011111" when "0100100001",
      "1010100111100111011010101000" when "0100100010",
      "1010101000010001111010011011" when "0100100011",
      "1010101000111100011100111000" when "0100100100",
      "1010101001100111000001111110" when "0100100101",
      "1010101010010001101001110000" when "0100100110",
      "1010101010111100010100001100" when "0100100111",
      "1010101011100111000001010010" when "0100101000",
      "1010101100010001110001000100" when "0100101001",
      "1010101100111100100011100001" when "0100101010",
      "1010101101100111011000101000" when "0100101011",
      "1010101110010010010000011100" when "0100101100",
      "1010101110111101001010111011" when "0100101101",
      "1010101111101000001000000101" when "0100101110",
      "1010110000010011000111111100" when "0100101111",
      "1010110000111110001010011110" when "0100110000",
      "1010110001101001001111101101" when "0100110001",
      "1010110010010100010111101000" when "0100110010",
      "1010110010111111100010010000" when "0100110011",
      "1010110011101010101111100101" when "0100110100",
      "1010110100010101111111100110" when "0100110101",
      "1010110101000001010010010101" when "0100110110",
      "1010110101101100100111110000" when "0100110111",
      "1010110110010111111111111010" when "0100111000",
      "1010110111000011011010110000" when "0100111001",
      "1010110111101110111000010101" when "0100111010",
      "1010111000011010011000100111" when "0100111011",
      "1010111001000101111011101000" when "0100111100",
      "1010111001110001100001010111" when "0100111101",
      "1010111010011101001001110100" when "0100111110",
      "1010111011001000110101000000" when "0100111111",
      "1010111011110100100010111011" when "0101000000",
      "1010111100100000010011100101" when "0101000001",
      "1010111101001100000110111110" when "0101000010",
      "1010111101110111111101000110" when "0101000011",
      "1010111110100011110101111101" when "0101000100",
      "1010111111001111110001100100" when "0101000101",
      "1010111111111011101111111011" when "0101000110",
      "1011000000100111110001000010" when "0101000111",
      "1011000001010011110100111010" when "0101001000",
      "1011000001111111111011100001" when "0101001001",
      "1011000010101100000100111001" when "0101001010",
      "1011000011011000010001000010" when "0101001011",
      "1011000100000100011111111011" when "0101001100",
      "1011000100110000110001100110" when "0101001101",
      "1011000101011101000110000010" when "0101001110",
      "1011000110001001011101001111" when "0101001111",
      "1011000110110101110111001101" when "0101010000",
      "1011000111100010010011111110" when "0101010001",
      "1011001000001110110011100000" when "0101010010",
      "1011001000111011010101110100" when "0101010011",
      "1011001001100111111010111011" when "0101010100",
      "1011001010010100100010110100" when "0101010101",
      "1011001011000001001101011111" when "0101010110",
      "1011001011101101111010111101" when "0101010111",
      "1011001100011010101011001110" when "0101011000",
      "1011001101000111011110010011" when "0101011001",
      "1011001101110100010100001010" when "0101011010",
      "1011001110100001001100110101" when "0101011011",
      "1011001111001110001000010100" when "0101011100",
      "1011001111111011000110100110" when "0101011101",
      "1011010000101000000111101101" when "0101011110",
      "1011010001010101001011100111" when "0101011111",
      "1011010010000010010010010110" when "0101100000",
      "1011010010101111011011111010" when "0101100001",
      "1011010011011100101000010010" when "0101100010",
      "1011010100001001110111011111" when "0101100011",
      "1011010100110111001001100001" when "0101100100",
      "1011010101100100011110011000" when "0101100101",
      "1011010110010001110110000101" when "0101100110",
      "1011010110111111010000100111" when "0101100111",
      "1011010111101100101101111111" when "0101101000",
      "1011011000011010001110001101" when "0101101001",
      "1011011001000111110001010001" when "0101101010",
      "1011011001110101010111001011" when "0101101011",
      "1011011010100010111111111100" when "0101101100",
      "1011011011010000101011100011" when "0101101101",
      "1011011011111110011010000001" when "0101101110",
      "1011011100101100001011010110" when "0101101111",
      "1011011101011001111111100011" when "0101110000",
      "1011011110000111110110100110" when "0101110001",
      "1011011110110101110000100010" when "0101110010",
      "1011011111100011101101010100" when "0101110011",
      "1011100000010001101100111111" when "0101110100",
      "1011100000111111101111100010" when "0101110101",
      "1011100001101101110100111101" when "0101110110",
      "1011100010011011111101010001" when "0101110111",
      "1011100011001010001000011101" when "0101111000",
      "1011100011111000010110100010" when "0101111001",
      "1011100100100110100111100000" when "0101111010",
      "1011100101010100111011010111" when "0101111011",
      "1011100110000011010010000111" when "0101111100",
      "1011100110110001101011110001" when "0101111101",
      "1011100111100000001000010101" when "0101111110",
      "1011101000001110100111110010" when "0101111111",
      "1011101000111101001010001010" when "0110000000",
      "1011101001101011101111011100" when "0110000001",
      "1011101010011010010111101000" when "0110000010",
      "1011101011001001000010101111" when "0110000011",
      "1011101011110111110000110000" when "0110000100",
      "1011101100100110100001101101" when "0110000101",
      "1011101101010101010101100101" when "0110000110",
      "1011101110000100001100011000" when "0110000111",
      "1011101110110011000110000110" when "0110001000",
      "1011101111100010000010110000" when "0110001001",
      "1011110000010001000010010111" when "0110001010",
      "1011110001000000000100111001" when "0110001011",
      "1011110001101111001010010111" when "0110001100",
      "1011110010011110010010110010" when "0110001101",
      "1011110011001101011110001010" when "0110001110",
      "1011110011111100101100011110" when "0110001111",
      "1011110100101011111101101111" when "0110010000",
      "1011110101011011010001111110" when "0110010001",
      "1011110110001010101001001010" when "0110010010",
      "1011110110111010000011010011" when "0110010011",
      "1011110111101001100000011010" when "0110010100",
      "1011111000011001000000011111" when "0110010101",
      "1011111001001000100011100010" when "0110010110",
      "1011111001111000001001100100" when "0110010111",
      "1011111010100111110010100011" when "0110011000",
      "1011111011010111011110100010" when "0110011001",
      "1011111100000111001101011111" when "0110011010",
      "1011111100110110111111011100" when "0110011011",
      "1011111101100110110100010111" when "0110011100",
      "1011111110010110101100010010" when "0110011101",
      "1011111111000110100111001101" when "0110011110",
      "1011111111110110100101000111" when "0110011111",
      "1100000000100110100110000010" when "0110100000",
      "1100000001010110101001111100" when "0110100001",
      "1100000010000110110000110111" when "0110100010",
      "1100000010110110111010110010" when "0110100011",
      "1100000011100111000111101110" when "0110100100",
      "1100000100010111010111101011" when "0110100101",
      "1100000101000111101010101001" when "0110100110",
      "1100000101111000000000101001" when "0110100111",
      "1100000110101000011001101010" when "0110101000",
      "1100000111011000110101101100" when "0110101001",
      "1100001000001001010100110000" when "0110101010",
      "1100001000111001110110110111" when "0110101011",
      "1100001001101010011011111111" when "0110101100",
      "1100001010011011000100001010" when "0110101101",
      "1100001011001011101111011000" when "0110101110",
      "1100001011111100011101101000" when "0110101111",
      "1100001100101101001110111100" when "0110110000",
      "1100001101011110000011010010" when "0110110001",
      "1100001110001110111010101100" when "0110110010",
      "1100001110111111110101001010" when "0110110011",
      "1100001111110000110010101011" when "0110110100",
      "1100010000100001110011010000" when "0110110101",
      "1100010001010010110110111001" when "0110110110",
      "1100010010000011111101100111" when "0110110111",
      "1100010010110101000111011001" when "0110111000",
      "1100010011100110010100010000" when "0110111001",
      "1100010100010111100100001100" when "0110111010",
      "1100010101001000110111001100" when "0110111011",
      "1100010101111010001101010011" when "0110111100",
      "1100010110101011100110011110" when "0110111101",
      "1100010111011101000010101111" when "0110111110",
      "1100011000001110100010000111" when "0110111111",
      "1100011001000000000100100100" when "0111000000",
      "1100011001110001101010000111" when "0111000001",
      "1100011010100011010010110001" when "0111000010",
      "1100011011010100111110100010" when "0111000011",
      "1100011100000110101101011001" when "0111000100",
      "1100011100111000011111010111" when "0111000101",
      "1100011101101010010100011101" when "0111000110",
      "1100011110011100001100101010" when "0111000111",
      "1100011111001110000111111111" when "0111001000",
      "1100100000000000000110011011" when "0111001001",
      "1100100000110010000111111111" when "0111001010",
      "1100100001100100001100101100" when "0111001011",
      "1100100010010110010100100001" when "0111001100",
      "1100100011001000011111011111" when "0111001101",
      "1100100011111010101101100101" when "0111001110",
      "1100100100101100111110110100" when "0111001111",
      "1100100101011111010011001101" when "0111010000",
      "1100100110010001101010101111" when "0111010001",
      "1100100111000100000101011010" when "0111010010",
      "1100100111110110100011010000" when "0111010011",
      "1100101000101001000100001111" when "0111010100",
      "1100101001011011101000011000" when "0111010101",
      "1100101010001110001111101100" when "0111010110",
      "1100101011000000111010001010" when "0111010111",
      "1100101011110011100111110011" when "0111011000",
      "1100101100100110011000100111" when "0111011001",
      "1100101101011001001100100110" when "0111011010",
      "1100101110001100000011110001" when "0111011011",
      "1100101110111110111110000111" when "0111011100",
      "1100101111110001111011101001" when "0111011101",
      "1100110000100100111100010111" when "0111011110",
      "1100110001011000000000010000" when "0111011111",
      "1100110010001011000111010111" when "0111100000",
      "1100110010111110010001101001" when "0111100001",
      "1100110011110001011111001001" when "0111100010",
      "1100110100100100101111110101" when "0111100011",
      "1100110101011000000011101111" when "0111100100",
      "1100110110001011011010110110" when "0111100101",
      "1100110110111110110101001010" when "0111100110",
      "1100110111110010010010101101" when "0111100111",
      "1100111000100101110011011101" when "0111101000",
      "1100111001011001010111011011" when "0111101001",
      "1100111010001100111110101000" when "0111101010",
      "1100111011000000101001000011" when "0111101011",
      "1100111011110100010110101101" when "0111101100",
      "1100111100101000000111100110" when "0111101101",
      "1100111101011011111011101110" when "0111101110",
      "1100111110001111110011000101" when "0111101111",
      "1100111111000011101101101100" when "0111110000",
      "1100111111110111101011100011" when "0111110001",
      "1101000000101011101100101010" when "0111110010",
      "1101000001011111110001000001" when "0111110011",
      "1101000010010011111000101000" when "0111110100",
      "1101000011001000000011100000" when "0111110101",
      "1101000011111100010001101001" when "0111110110",
      "1101000100110000100011000010" when "0111110111",
      "1101000101100100110111101101" when "0111111000",
      "1101000110011001001111101001" when "0111111001",
      "1101000111001101101010110111" when "0111111010",
      "1101001000000010001001010111" when "0111111011",
      "1101001000110110101011001000" when "0111111100",
      "1101001001101011010000001100" when "0111111101",
      "1101001010011111111000100010" when "0111111110",
      "1101001011010100100100001011" when "0111111111",
      "0100110110100010110010111111" when "1000000000",
      "0100110110110110001101110001" when "1000000001",
      "0100110111001001101001110001" when "1000000010",
      "0100110111011101000110111110" when "1000000011",
      "0100110111110000100101011010" when "1000000100",
      "0100111000000100000101000011" when "1000000101",
      "0100111000010111100101111010" when "1000000110",
      "0100111000101011001000000000" when "1000000111",
      "0100111000111110101011010011" when "1000001000",
      "0100111001010010001111110101" when "1000001001",
      "0100111001100101110101100101" when "1000001010",
      "0100111001111001011100100100" when "1000001011",
      "0100111010001101000100110001" when "1000001100",
      "0100111010100000101110001101" when "1000001101",
      "0100111010110100011000110111" when "1000001110",
      "0100111011001000000100110000" when "1000001111",
      "0100111011011011110001110111" when "1000010000",
      "0100111011101111100000001110" when "1000010001",
      "0100111100000011001111110100" when "1000010010",
      "0100111100010111000000101000" when "1000010011",
      "0100111100101010110010101100" when "1000010100",
      "0100111100111110100101111110" when "1000010101",
      "0100111101010010011010100000" when "1000010110",
      "0100111101100110010000010010" when "1000010111",
      "0100111101111010000111010010" when "1000011000",
      "0100111110001101111111100011" when "1000011001",
      "0100111110100001111001000010" when "1000011010",
      "0100111110110101110011110010" when "1000011011",
      "0100111111001001101111110001" when "1000011100",
      "0100111111011101101101000000" when "1000011101",
      "0100111111110001101011011111" when "1000011110",
      "0101000000000101101011001101" when "1000011111",
      "0101000000011001101100001100" when "1000100000",
      "0101000000101101101110011011" when "1000100001",
      "0101000001000001110001111010" when "1000100010",
      "0101000001010101110110101001" when "1000100011",
      "0101000001101001111100101001" when "1000100100",
      "0101000001111110000011111001" when "1000100101",
      "0101000010010010001100011001" when "1000100110",
      "0101000010100110010110001010" when "1000100111",
      "0101000010111010100001001100" when "1000101000",
      "0101000011001110101101011110" when "1000101001",
      "0101000011100010111011000010" when "1000101010",
      "0101000011110111001001110110" when "1000101011",
      "0101000100001011011001111011" when "1000101100",
      "0101000100011111101011010001" when "1000101101",
      "0101000100110011111101111000" when "1000101110",
      "0101000101001000010001110001" when "1000101111",
      "0101000101011100100110111011" when "1000110000",
      "0101000101110000111101010110" when "1000110001",
      "0101000110000101010101000010" when "1000110010",
      "0101000110011001101110000000" when "1000110011",
      "0101000110101110001000010000" when "1000110100",
      "0101000111000010100011110001" when "1000110101",
      "0101000111010111000000100101" when "1000110110",
      "0101000111101011011110101010" when "1000110111",
      "0101000111111111111110000000" when "1000111000",
      "0101001000010100011110101001" when "1000111001",
      "0101001000101001000000100100" when "1000111010",
      "0101001000111101100011110001" when "1000111011",
      "0101001001010010001000010001" when "1000111100",
      "0101001001100110101110000011" when "1000111101",
      "0101001001111011010101000111" when "1000111110",
      "0101001010001111111101011101" when "1000111111",
      "0101001010100100100111000110" when "1001000000",
      "0101001010111001010010000010" when "1001000001",
      "0101001011001101111110010001" when "1001000010",
      "0101001011100010101011110010" when "1001000011",
      "0101001011110111011010100110" when "1001000100",
      "0101001100001100001010101101" when "1001000101",
      "0101001100100000111100000111" when "1001000110",
      "0101001100110101101110110101" when "1001000111",
      "0101001101001010100010110101" when "1001001000",
      "0101001101011111011000001001" when "1001001001",
      "0101001101110100001110110000" when "1001001010",
      "0101001110001001000110101011" when "1001001011",
      "0101001110011101111111111001" when "1001001100",
      "0101001110110010111010011011" when "1001001101",
      "0101001111000111110110010001" when "1001001110",
      "0101001111011100110011011010" when "1001001111",
      "0101001111110001110001110111" when "1001010000",
      "0101010000000110110001101000" when "1001010001",
      "0101010000011011110010101101" when "1001010010",
      "0101010000110000110101000110" when "1001010011",
      "0101010001000101111000110100" when "1001010100",
      "0101010001011010111101110110" when "1001010101",
      "0101010001110000000100001100" when "1001010110",
      "0101010010000101001011110110" when "1001010111",
      "0101010010011010010100110101" when "1001011000",
      "0101010010101111011111001001" when "1001011001",
      "0101010011000100101010110001" when "1001011010",
      "0101010011011001110111101110" when "1001011011",
      "0101010011101111000110000000" when "1001011100",
      "0101010100000100010101100111" when "1001011101",
      "0101010100011001100110100011" when "1001011110",
      "0101010100101110111000110100" when "1001011111",
      "0101010101000100001100011010" when "1001100000",
      "0101010101011001100001010101" when "1001100001",
      "0101010101101110110111100110" when "1001100010",
      "0101010110000100001111001100" when "1001100011",
      "0101010110011001101000001000" when "1001100100",
      "0101010110101111000010011001" when "1001100101",
      "0101010111000100011110000000" when "1001100110",
      "0101010111011001111010111101" when "1001100111",
      "0101010111101111011001010000" when "1001101000",
      "0101011000000100111000111000" when "1001101001",
      "0101011000011010011001110111" when "1001101010",
      "0101011000101111111100001100" when "1001101011",
      "0101011001000101011111110110" when "1001101100",
      "0101011001011011000100111000" when "1001101101",
      "0101011001110000101011001111" when "1001101110",
      "0101011010000110010010111101" when "1001101111",
      "0101011010011011111100000010" when "1001110000",
      "0101011010110001100110011101" when "1001110001",
      "0101011011000111010010001110" when "1001110010",
      "0101011011011100111111010111" when "1001110011",
      "0101011011110010101101110110" when "1001110100",
      "0101011100001000011101101101" when "1001110101",
      "0101011100011110001110111010" when "1001110110",
      "0101011100110100000001011111" when "1001110111",
      "0101011101001001110101011010" when "1001111000",
      "0101011101011111101010101101" when "1001111001",
      "0101011101110101100001011000" when "1001111010",
      "0101011110001011011001011001" when "1001111011",
      "0101011110100001010010110011" when "1001111100",
      "0101011110110111001101100100" when "1001111101",
      "0101011111001101001001101101" when "1001111110",
      "0101011111100011000111001101" when "1001111111",
      "0101011111111001000110000101" when "1010000000",
      "0101100000001111000110010110" when "1010000001",
      "0101100000100101000111111110" when "1010000010",
      "0101100000111011001010111111" when "1010000011",
      "0101100001010001001111011000" when "1010000100",
      "0101100001100111010101001001" when "1010000101",
      "0101100001111101011100010010" when "1010000110",
      "0101100010010011100100110100" when "1010000111",
      "0101100010101001101110101111" when "1010001000",
      "0101100010111111111010000010" when "1010001001",
      "0101100011010110000110101110" when "1010001010",
      "0101100011101100010100110011" when "1010001011",
      "0101100100000010100100010001" when "1010001100",
      "0101100100011000110101001000" when "1010001101",
      "0101100100101111000111011000" when "1010001110",
      "0101100101000101011011000001" when "1010001111",
      "0101100101011011110000000011" when "1010010000",
      "0101100101110010000110011111" when "1010010001",
      "0101100110001000011110010100" when "1010010010",
      "0101100110011110110111100011" when "1010010011",
      "0101100110110101010010001011" when "1010010100",
      "0101100111001011101110001101" when "1010010101",
      "0101100111100010001011101001" when "1010010110",
      "0101100111111000101010011110" when "1010010111",
      "0101101000001111001010101110" when "1010011000",
      "0101101000100101101100011000" when "1010011001",
      "0101101000111100001111011100" when "1010011010",
      "0101101001010010110011111010" when "1010011011",
      "0101101001101001011001110010" when "1010011100",
      "0101101010000000000001000101" when "1010011101",
      "0101101010010110101001110010" when "1010011110",
      "0101101010101101010011111010" when "1010011111",
      "0101101011000011111111011101" when "1010100000",
      "0101101011011010101100011010" when "1010100001",
      "0101101011110001011010110010" when "1010100010",
      "0101101100001000001010100101" when "1010100011",
      "0101101100011110111011110100" when "1010100100",
      "0101101100110101101110011101" when "1010100101",
      "0101101101001100100010100001" when "1010100110",
      "0101101101100011011000000001" when "1010100111",
      "0101101101111010001110111101" when "1010101000",
      "0101101110010001000111010011" when "1010101001",
      "0101101110101000000001000101" when "1010101010",
      "0101101110111110111100010011" when "1010101011",
      "0101101111010101111000111101" when "1010101100",
      "0101101111101100110111000011" when "1010101101",
      "0101110000000011110110100100" when "1010101110",
      "0101110000011010110111100001" when "1010101111",
      "0101110000110001111001111011" when "1010110000",
      "0101110001001000111101110001" when "1010110001",
      "0101110001100000000011000011" when "1010110010",
      "0101110001110111001001110001" when "1010110011",
      "0101110010001110010001111100" when "1010110100",
      "0101110010100101011011100011" when "1010110101",
      "0101110010111100100110100111" when "1010110110",
      "0101110011010011110011001000" when "1010110111",
      "0101110011101011000001000110" when "1010111000",
      "0101110100000010010000100000" when "1010111001",
      "0101110100011001100001011000" when "1010111010",
      "0101110100110000110011101101" when "1010111011",
      "0101110101001000000111011110" when "1010111100",
      "0101110101011111011100101110" when "1010111101",
      "0101110101110110110011011010" when "1010111110",
      "0101110110001110001011100100" when "1010111111",
      "0101110110100101100101001011" when "1011000000",
      "0101110110111101000000010001" when "1011000001",
      "0101110111010100011100110100" when "1011000010",
      "0101110111101011111010110100" when "1011000011",
      "0101111000000011011010010011" when "1011000100",
      "0101111000011010111011010000" when "1011000101",
      "0101111000110010011101101010" when "1011000110",
      "0101111001001010000001100011" when "1011000111",
      "0101111001100001100110111011" when "1011001000",
      "0101111001111001001101110000" when "1011001001",
      "0101111010010000110110000100" when "1011001010",
      "0101111010101000011111110111" when "1011001011",
      "0101111011000000001011001000" when "1011001100",
      "0101111011010111110111111000" when "1011001101",
      "0101111011101111100110000111" when "1011001110",
      "0101111100000111010101110101" when "1011001111",
      "0101111100011111000111000010" when "1011010000",
      "0101111100110110111001101110" when "1011010001",
      "0101111101001110101101111001" when "1011010010",
      "0101111101100110100011100100" when "1011010011",
      "0101111101111110011010101110" when "1011010100",
      "0101111110010110010011010111" when "1011010101",
      "0101111110101110001101100000" when "1011010110",
      "0101111111000110001001001001" when "1011010111",
      "0101111111011110000110010001" when "1011011000",
      "0101111111110110000100111010" when "1011011001",
      "0110000000001110000101000010" when "1011011010",
      "0110000000100110000110101010" when "1011011011",
      "0110000000111110001001110011" when "1011011100",
      "0110000001010110001110011100" when "1011011101",
      "0110000001101110010100100101" when "1011011110",
      "0110000010000110011100001110" when "1011011111",
      "0110000010011110100101011000" when "1011100000",
      "0110000010110110110000000011" when "1011100001",
      "0110000011001110111100001110" when "1011100010",
      "0110000011100111001001111011" when "1011100011",
      "0110000011111111011001001000" when "1011100100",
      "0110000100010111101001110110" when "1011100101",
      "0110000100101111111100000101" when "1011100110",
      "0110000101001000001111110101" when "1011100111",
      "0110000101100000100101000111" when "1011101000",
      "0110000101111000111011111010" when "1011101001",
      "0110000110010001010100001110" when "1011101010",
      "0110000110101001101110000101" when "1011101011",
      "0110000111000010001001011100" when "1011101100",
      "0110000111011010100110010110" when "1011101101",
      "0110000111110011000100110001" when "1011101110",
      "0110001000001011100100101110" when "1011101111",
      "0110001000100100000110001110" when "1011110000",
      "0110001000111100101001001111" when "1011110001",
      "0110001001010101001101110011" when "1011110010",
      "0110001001101101110011111001" when "1011110011",
      "0110001010000110011011100001" when "1011110100",
      "0110001010011111000100101100" when "1011110101",
      "0110001010110111101111011010" when "1011110110",
      "0110001011010000011011101010" when "1011110111",
      "0110001011101001001001011110" when "1011111000",
      "0110001100000001111000110100" when "1011111001",
      "0110001100011010101001101101" when "1011111010",
      "0110001100110011011100001001" when "1011111011",
      "0110001101001100010000001000" when "1011111100",
      "0110001101100101000101101011" when "1011111101",
      "0110001101111101111100110001" when "1011111110",
      "0110001110010110110101011011" when "1011111111",
      "0110001110101111101111101000" when "1100000000",
      "0110001111001000101011011001" when "1100000001",
      "0110001111100001101000101101" when "1100000010",
      "0110001111111010100111100110" when "1100000011",
      "0110010000010011101000000010" when "1100000100",
      "0110010000101100101010000011" when "1100000101",
      "0110010001000101101101100111" when "1100000110",
      "0110010001011110110010110000" when "1100000111",
      "0110010001110111111001011110" when "1100001000",
      "0110010010010001000001110000" when "1100001001",
      "0110010010101010001011100110" when "1100001010",
      "0110010011000011010111000001" when "1100001011",
      "0110010011011100100100000001" when "1100001100",
      "0110010011110101110010100110" when "1100001101",
      "0110010100001111000010101111" when "1100001110",
      "0110010100101000010100011110" when "1100001111",
      "0110010101000001100111110010" when "1100010000",
      "0110010101011010111100101011" when "1100010001",
      "0110010101110100010011001001" when "1100010010",
      "0110010110001101101011001101" when "1100010011",
      "0110010110100111000100110111" when "1100010100",
      "0110010111000000100000000110" when "1100010101",
      "0110010111011001111100111011" when "1100010110",
      "0110010111110011011011010110" when "1100010111",
      "0110011000001100111011010110" when "1100011000",
      "0110011000100110011100111101" when "1100011001",
      "0110011001000000000000001010" when "1100011010",
      "0110011001011001100100111101" when "1100011011",
      "0110011001110011001011010111" when "1100011100",
      "0110011010001100110011010111" when "1100011101",
      "0110011010100110011100111101" when "1100011110",
      "0110011011000000001000001010" when "1100011111",
      "0110011011011001110100111110" when "1100100000",
      "0110011011110011100011011001" when "1100100001",
      "0110011100001101010011011011" when "1100100010",
      "0110011100100111000101000011" when "1100100011",
      "0110011101000000111000010011" when "1100100100",
      "0110011101011010101101001011" when "1100100101",
      "0110011101110100100011101001" when "1100100110",
      "0110011110001110011011101111" when "1100100111",
      "0110011110101000010101011101" when "1100101000",
      "0110011111000010010000110010" when "1100101001",
      "0110011111011100001101101111" when "1100101010",
      "0110011111110110001100010011" when "1100101011",
      "0110100000010000001100100000" when "1100101100",
      "0110100000101010001110010101" when "1100101101",
      "0110100001000100010001110010" when "1100101110",
      "0110100001011110010110110111" when "1100101111",
      "0110100001111000011101100101" when "1100110000",
      "0110100010010010100101111011" when "1100110001",
      "0110100010101100101111111010" when "1100110010",
      "0110100011000110111011100001" when "1100110011",
      "0110100011100001001000110001" when "1100110100",
      "0110100011111011010111101010" when "1100110101",
      "0110100100010101101000001100" when "1100110110",
      "0110100100101111111010010111" when "1100110111",
      "0110100101001010001110001100" when "1100111000",
      "0110100101100100100011101001" when "1100111001",
      "0110100101111110111010110000" when "1100111010",
      "0110100110011001010011100000" when "1100111011",
      "0110100110110011101101111010" when "1100111100",
      "0110100111001110001001111110" when "1100111101",
      "0110100111101000100111101100" when "1100111110",
      "0110101000000011000111000011" when "1100111111",
      "0110101000011101101000000101" when "1101000000",
      "0110101000111000001010110000" when "1101000001",
      "0110101001010010101111000110" when "1101000010",
      "0110101001101101010101000110" when "1101000011",
      "0110101010000111111100110001" when "1101000100",
      "0110101010100010100110000110" when "1101000101",
      "0110101010111101010001000110" when "1101000110",
      "0110101011010111111101110000" when "1101000111",
      "0110101011110010101100000101" when "1101001000",
      "0110101100001101011100000110" when "1101001001",
      "0110101100101000001101110001" when "1101001010",
      "0110101101000011000001000111" when "1101001011",
      "0110101101011101110110001001" when "1101001100",
      "0110101101111000101100110110" when "1101001101",
      "0110101110010011100101001111" when "1101001110",
      "0110101110101110011111010011" when "1101001111",
      "0110101111001001011011000011" when "1101010000",
      "0110101111100100011000011110" when "1101010001",
      "0110101111111111010111100110" when "1101010010",
      "0110110000011010011000011001" when "1101010011",
      "0110110000110101011010111001" when "1101010100",
      "0110110001010000011111000101" when "1101010101",
      "0110110001101011100100111101" when "1101010110",
      "0110110010000110101100100001" when "1101010111",
      "0110110010100001110101110010" when "1101011000",
      "0110110010111101000000110000" when "1101011001",
      "0110110011011000001101011010" when "1101011010",
      "0110110011110011011011110010" when "1101011011",
      "0110110100001110101011110110" when "1101011100",
      "0110110100101001111101100111" when "1101011101",
      "0110110101000101010001000110" when "1101011110",
      "0110110101100000100110010001" when "1101011111",
      "0110110101111011111101001010" when "1101100000",
      "0110110110010111010101110001" when "1101100001",
      "0110110110110010110000000101" when "1101100010",
      "0110110111001110001100000111" when "1101100011",
      "0110110111101001101001110111" when "1101100100",
      "0110111000000101001001010100" when "1101100101",
      "0110111000100000101010100000" when "1101100110",
      "0110111000111100001101011010" when "1101100111",
      "0110111001010111110010000010" when "1101101000",
      "0110111001110011011000011000" when "1101101001",
      "0110111010001111000000011101" when "1101101010",
      "0110111010101010101010010000" when "1101101011",
      "0110111011000110010101110010" when "1101101100",
      "0110111011100010000011000011" when "1101101101",
      "0110111011111101110010000011" when "1101101110",
      "0110111100011001100010110001" when "1101101111",
      "0110111100110101010101001111" when "1101110000",
      "0110111101010001001001011100" when "1101110001",
      "0110111101101100111111011000" when "1101110010",
      "0110111110001000110111000100" when "1101110011",
      "0110111110100100110000011111" when "1101110100",
      "0110111111000000101011101010" when "1101110101",
      "0110111111011100101000100101" when "1101110110",
      "0110111111111000100111001111" when "1101110111",
      "0111000000010100100111101001" when "1101111000",
      "0111000000110000101001110100" when "1101111001",
      "0111000001001100101101101111" when "1101111010",
      "0111000001101000110011011010" when "1101111011",
      "0111000010000100111010110101" when "1101111100",
      "0111000010100001000100000001" when "1101111101",
      "0111000010111101001110111110" when "1101111110",
      "0111000011011001011011101011" when "1101111111",
      "0111000011110101101010001001" when "1110000000",
      "0111000100010001111010011000" when "1110000001",
      "0111000100101110001100011001" when "1110000010",
      "0111000101001010100000001010" when "1110000011",
      "0111000101100110110101101101" when "1110000100",
      "0111000110000011001101000001" when "1110000101",
      "0111000110011111100110000110" when "1110000110",
      "0111000110111100000000111101" when "1110000111",
      "0111000111011000011101100110" when "1110001000",
      "0111000111110100111100000001" when "1110001001",
      "0111001000010001011100001110" when "1110001010",
      "0111001000101101111110001101" when "1110001011",
      "0111001001001010100001111110" when "1110001100",
      "0111001001100111000111100001" when "1110001101",
      "0111001010000011101110110111" when "1110001110",
      "0111001010100000010111111111" when "1110001111",
      "0111001010111101000010111010" when "1110010000",
      "0111001011011001101111100111" when "1110010001",
      "0111001011110110011110001000" when "1110010010",
      "0111001100010011001110011011" when "1110010011",
      "0111001100110000000000100010" when "1110010100",
      "0111001101001100110100011011" when "1110010101",
      "0111001101101001101010001000" when "1110010110",
      "0111001110000110100001101001" when "1110010111",
      "0111001110100011011010111100" when "1110011000",
      "0111001111000000010110000100" when "1110011001",
      "0111001111011101010010111111" when "1110011010",
      "0111001111111010010001101110" when "1110011011",
      "0111010000010111010010010010" when "1110011100",
      "0111010000110100010100101001" when "1110011101",
      "0111010001010001011000110100" when "1110011110",
      "0111010001101110011110110100" when "1110011111",
      "0111010010001011100110101000" when "1110100000",
      "0111010010101000110000010001" when "1110100001",
      "0111010011000101111011101110" when "1110100010",
      "0111010011100011001001000000" when "1110100011",
      "0111010100000000011000000111" when "1110100100",
      "0111010100011101101001000011" when "1110100101",
      "0111010100111010111011110100" when "1110100110",
      "0111010101011000010000011011" when "1110100111",
      "0111010101110101100110110111" when "1110101000",
      "0111010110010010111111001000" when "1110101001",
      "0111010110110000011001001110" when "1110101010",
      "0111010111001101110101001011" when "1110101011",
      "0111010111101011010010111101" when "1110101100",
      "0111011000001000110010100101" when "1110101101",
      "0111011000100110010100000011" when "1110101110",
      "0111011001000011110111011000" when "1110101111",
      "0111011001100001011100100010" when "1110110000",
      "0111011001111111000011100011" when "1110110001",
      "0111011010011100101100011011" when "1110110010",
      "0111011010111010010111001001" when "1110110011",
      "0111011011011000000011101110" when "1110110100",
      "0111011011110101110010001010" when "1110110101",
      "0111011100010011100010011100" when "1110110110",
      "0111011100110001010100100110" when "1110110111",
      "0111011101001111001000100111" when "1110111000",
      "0111011101101100111110011111" when "1110111001",
      "0111011110001010110110001111" when "1110111010",
      "0111011110101000101111110110" when "1110111011",
      "0111011111000110101011010101" when "1110111100",
      "0111011111100100101000101011" when "1110111101",
      "0111100000000010100111111010" when "1110111110",
      "0111100000100000101001000000" when "1110111111",
      "0111100000111110101011111111" when "1111000000",
      "0111100001011100110000110110" when "1111000001",
      "0111100001111010110111100101" when "1111000010",
      "0111100010011001000000001101" when "1111000011",
      "0111100010110111001010101101" when "1111000100",
      "0111100011010101010111000110" when "1111000101",
      "0111100011110011100101011000" when "1111000110",
      "0111100100010001110101100011" when "1111000111",
      "0111100100110000000111100111" when "1111001000",
      "0111100101001110011011100100" when "1111001001",
      "0111100101101100110001011010" when "1111001010",
      "0111100110001011001001001010" when "1111001011",
      "0111100110101001100010110011" when "1111001100",
      "0111100111000111111110010111" when "1111001101",
      "0111100111100110011011110011" when "1111001110",
      "0111101000000100111011001010" when "1111001111",
      "0111101000100011011100011011" when "1111010000",
      "0111101001000001111111100110" when "1111010001",
      "0111101001100000100100101011" when "1111010010",
      "0111101001111111001011101010" when "1111010011",
      "0111101010011101110100100100" when "1111010100",
      "0111101010111100011111011001" when "1111010101",
      "0111101011011011001100001000" when "1111010110",
      "0111101011111001111010110010" when "1111010111",
      "0111101100011000101011011000" when "1111011000",
      "0111101100110111011101111000" when "1111011001",
      "0111101101010110010010010011" when "1111011010",
      "0111101101110101001000101010" when "1111011011",
      "0111101110010100000000111100" when "1111011100",
      "0111101110110010111011001010" when "1111011101",
      "0111101111010001110111010100" when "1111011110",
      "0111101111110000110101011001" when "1111011111",
      "0111110000001111110101011011" when "1111100000",
      "0111110000101110110111011000" when "1111100001",
      "0111110001001101111011010010" when "1111100010",
      "0111110001101101000001000111" when "1111100011",
      "0111110010001100001000111010" when "1111100100",
      "0111110010101011010010101001" when "1111100101",
      "0111110011001010011110010100" when "1111100110",
      "0111110011101001101011111100" when "1111100111",
      "0111110100001000111011100010" when "1111101000",
      "0111110100101000001101000100" when "1111101001",
      "0111110101000111100000100011" when "1111101010",
      "0111110101100110110110000000" when "1111101011",
      "0111110110000110001101011010" when "1111101100",
      "0111110110100101100110110010" when "1111101101",
      "0111110111000101000010000111" when "1111101110",
      "0111110111100100011111011010" when "1111101111",
      "0111111000000011111110101011" when "1111110000",
      "0111111000100011011111111010" when "1111110001",
      "0111111001000011000011000111" when "1111110010",
      "0111111001100010101000010010" when "1111110011",
      "0111111010000010001111011100" when "1111110100",
      "0111111010100001111000100100" when "1111110101",
      "0111111011000001100011101011" when "1111110110",
      "0111111011100001010000110001" when "1111110111",
      "0111111100000000111111110101" when "1111111000",
      "0111111100100000110000111001" when "1111111001",
      "0111111101000000100011111100" when "1111111010",
      "0111111101100000011000111101" when "1111111011",
      "0111111110000000001111111111" when "1111111100",
      "0111111110100000001000111111" when "1111111101",
      "0111111111000000000100000000" when "1111111110",
      "0111111111100000000001000000" when "1111111111",
      "----------------------------" when others;
   Y1 <= Y0; -- for the possible blockram register
   Y <= Y1;
end architecture;

--------------------------------------------------------------------------------
--                           IntAdder_18_F250_uid39
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Bogdan Pasca, Florent de Dinechin (2008-2016)
--------------------------------------------------------------------------------
-- Pipeline depth: 1 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X Y Cin
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity IntAdder_18_F250_uid39 is
    port (clk : in std_logic;
          X : in  std_logic_vector(17 downto 0);
          Y : in  std_logic_vector(17 downto 0);
          Cin : in  std_logic;
          R : out  std_logic_vector(17 downto 0)   );
end entity;

architecture arch of IntAdder_18_F250_uid39 is
signal Cin_1, Cin_1_d1, Cin_1_d2, Cin_1_d3, Cin_1_d4 :  std_logic;
signal X_1, X_1_d1 :  std_logic_vector(18 downto 0);
signal Y_1, Y_1_d1 :  std_logic_vector(18 downto 0);
signal S_1 :  std_logic_vector(18 downto 0);
signal R_1 :  std_logic_vector(17 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            Cin_1_d1 <=  Cin_1;
            Cin_1_d2 <=  Cin_1_d1;
            Cin_1_d3 <=  Cin_1_d2;
            Cin_1_d4 <=  Cin_1_d3;
            X_1_d1 <=  X_1;
            Y_1_d1 <=  Y_1;
         end if;
      end process;
   Cin_1 <= Cin;
   X_1 <= '0' & X(17 downto 0);
   Y_1 <= '0' & Y(17 downto 0);
   S_1 <= X_1_d1 + Y_1_d1 + Cin_1_d4;
   R_1 <= S_1(17 downto 0);
   R <= R_1 ;
end architecture;

--------------------------------------------------------------------------------
--                           IntAdder_18_F250_uid43
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Bogdan Pasca, Florent de Dinechin (2008-2016)
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X Y Cin
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity IntAdder_18_F250_uid43 is
    port (clk : in std_logic;
          X : in  std_logic_vector(17 downto 0);
          Y : in  std_logic_vector(17 downto 0);
          Cin : in  std_logic;
          R : out  std_logic_vector(17 downto 0)   );
end entity;

architecture arch of IntAdder_18_F250_uid43 is
signal Rtmp :  std_logic_vector(17 downto 0);
signal Y_d1, Y_d2, Y_d3 :  std_logic_vector(17 downto 0);
signal Cin_d1, Cin_d2, Cin_d3 :  std_logic;
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            Y_d1 <=  Y;
            Y_d2 <=  Y_d1;
            Y_d3 <=  Y_d2;
            Cin_d1 <=  Cin;
            Cin_d2 <=  Cin_d1;
            Cin_d3 <=  Cin_d2;
         end if;
      end process;
   Rtmp <= X + Y_d3 + Cin_d3;
   R <= Rtmp;
end architecture;

--------------------------------------------------------------------------------
--                         DSPBlock_11x12_F250_uid49
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: 
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X Y
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library std;
use std.textio.all;
library work;

entity DSPBlock_11x12_F250_uid49 is
    port (clk : in std_logic;
          X : in  std_logic_vector(10 downto 0);
          Y : in  std_logic_vector(11 downto 0);
          R : out  std_logic_vector(22 downto 0)   );
end entity;

architecture arch of DSPBlock_11x12_F250_uid49 is
signal Mint :  std_logic_vector(22 downto 0);
signal M :  std_logic_vector(22 downto 0);
signal Rtmp :  std_logic_vector(22 downto 0);
signal X_d1 :  std_logic_vector(10 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            X_d1 <=  X;
         end if;
      end process;
   Mint <= std_logic_vector(unsigned(X_d1) * unsigned(Y)); -- multiplier
   M <= Mint(22 downto 0);
   Rtmp <= M;
   R <= Rtmp;
end architecture;

--------------------------------------------------------------------------------
--                          DSPBlock_1x11_F250_uid51
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: 
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X Y
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library std;
use std.textio.all;
library work;

entity DSPBlock_1x11_F250_uid51 is
    port (clk : in std_logic;
          X : in  std_logic_vector(0 downto 0);
          Y : in  std_logic_vector(10 downto 0);
          R : out  std_logic_vector(10 downto 0)   );
end entity;

architecture arch of DSPBlock_1x11_F250_uid51 is
signal Mint :  std_logic_vector(11 downto 0);
signal M :  std_logic_vector(10 downto 0);
signal Rtmp :  std_logic_vector(10 downto 0);
signal X_d1 :  std_logic_vector(0 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            X_d1 <=  X;
         end if;
      end process;
   Mint <= std_logic_vector(unsigned(X_d1) * unsigned(Y)); -- multiplier
   M <= Mint(10 downto 0);
   Rtmp <= M;
   R <= Rtmp;
end architecture;

--------------------------------------------------------------------------------
--                          DSPBlock_10x1_F250_uid53
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: 
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X Y
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library std;
use std.textio.all;
library work;

entity DSPBlock_10x1_F250_uid53 is
    port (clk : in std_logic;
          X : in  std_logic_vector(9 downto 0);
          Y : in  std_logic_vector(0 downto 0);
          R : out  std_logic_vector(9 downto 0)   );
end entity;

architecture arch of DSPBlock_10x1_F250_uid53 is
signal Mint :  std_logic_vector(10 downto 0);
signal M :  std_logic_vector(9 downto 0);
signal Rtmp :  std_logic_vector(9 downto 0);
signal X_d1 :  std_logic_vector(9 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            X_d1 <=  X;
         end if;
      end process;
   Mint <= std_logic_vector(unsigned(X_d1) * unsigned(Y)); -- multiplier
   M <= Mint(9 downto 0);
   Rtmp <= M;
   R <= Rtmp;
end architecture;

--------------------------------------------------------------------------------
--                          DSPBlock_9x1_F250_uid55
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: 
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X Y
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library std;
use std.textio.all;
library work;

entity DSPBlock_9x1_F250_uid55 is
    port (clk : in std_logic;
          X : in  std_logic_vector(8 downto 0);
          Y : in  std_logic_vector(0 downto 0);
          R : out  std_logic_vector(8 downto 0)   );
end entity;

architecture arch of DSPBlock_9x1_F250_uid55 is
signal Mint :  std_logic_vector(9 downto 0);
signal M :  std_logic_vector(8 downto 0);
signal Rtmp :  std_logic_vector(8 downto 0);
signal X_d1 :  std_logic_vector(8 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            X_d1 <=  X;
         end if;
      end process;
   Mint <= std_logic_vector(unsigned(X_d1) * unsigned(Y)); -- multiplier
   M <= Mint(8 downto 0);
   Rtmp <= M;
   R <= Rtmp;
end architecture;

--------------------------------------------------------------------------------
--                          DSPBlock_1x10_F250_uid57
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: 
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X Y
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library std;
use std.textio.all;
library work;

entity DSPBlock_1x10_F250_uid57 is
    port (clk : in std_logic;
          X : in  std_logic_vector(0 downto 0);
          Y : in  std_logic_vector(9 downto 0);
          R : out  std_logic_vector(9 downto 0)   );
end entity;

architecture arch of DSPBlock_1x10_F250_uid57 is
signal Mint :  std_logic_vector(10 downto 0);
signal M :  std_logic_vector(9 downto 0);
signal Rtmp :  std_logic_vector(9 downto 0);
signal X_d1 :  std_logic_vector(0 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            X_d1 <=  X;
         end if;
      end process;
   Mint <= std_logic_vector(unsigned(X_d1) * unsigned(Y)); -- multiplier
   M <= Mint(9 downto 0);
   Rtmp <= M;
   R <= Rtmp;
end architecture;

--------------------------------------------------------------------------------
--                          DSPBlock_1x9_F250_uid59
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: 
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X Y
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library std;
use std.textio.all;
library work;

entity DSPBlock_1x9_F250_uid59 is
    port (clk : in std_logic;
          X : in  std_logic_vector(0 downto 0);
          Y : in  std_logic_vector(8 downto 0);
          R : out  std_logic_vector(8 downto 0)   );
end entity;

architecture arch of DSPBlock_1x9_F250_uid59 is
signal Mint :  std_logic_vector(9 downto 0);
signal M :  std_logic_vector(8 downto 0);
signal Rtmp :  std_logic_vector(8 downto 0);
signal X_d1 :  std_logic_vector(0 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            X_d1 <=  X;
         end if;
      end process;
   Mint <= std_logic_vector(unsigned(X_d1) * unsigned(Y)); -- multiplier
   M <= Mint(8 downto 0);
   Rtmp <= M;
   R <= Rtmp;
end architecture;

--------------------------------------------------------------------------------
--                          DSPBlock_8x1_F250_uid61
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: 
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X Y
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library std;
use std.textio.all;
library work;

entity DSPBlock_8x1_F250_uid61 is
    port (clk : in std_logic;
          X : in  std_logic_vector(7 downto 0);
          Y : in  std_logic_vector(0 downto 0);
          R : out  std_logic_vector(7 downto 0)   );
end entity;

architecture arch of DSPBlock_8x1_F250_uid61 is
signal Mint :  std_logic_vector(8 downto 0);
signal M :  std_logic_vector(7 downto 0);
signal Rtmp :  std_logic_vector(7 downto 0);
signal X_d1 :  std_logic_vector(7 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            X_d1 <=  X;
         end if;
      end process;
   Mint <= std_logic_vector(unsigned(X_d1) * unsigned(Y)); -- multiplier
   M <= Mint(7 downto 0);
   Rtmp <= M;
   R <= Rtmp;
end architecture;

--------------------------------------------------------------------------------
--                          DSPBlock_1x8_F250_uid63
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: 
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X Y
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library std;
use std.textio.all;
library work;

entity DSPBlock_1x8_F250_uid63 is
    port (clk : in std_logic;
          X : in  std_logic_vector(0 downto 0);
          Y : in  std_logic_vector(7 downto 0);
          R : out  std_logic_vector(7 downto 0)   );
end entity;

architecture arch of DSPBlock_1x8_F250_uid63 is
signal Mint :  std_logic_vector(8 downto 0);
signal M :  std_logic_vector(7 downto 0);
signal Rtmp :  std_logic_vector(7 downto 0);
signal X_d1 :  std_logic_vector(0 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            X_d1 <=  X;
         end if;
      end process;
   Mint <= std_logic_vector(unsigned(X_d1) * unsigned(Y)); -- multiplier
   M <= Mint(7 downto 0);
   Rtmp <= M;
   R <= Rtmp;
end architecture;

--------------------------------------------------------------------------------
--                          DSPBlock_1x7_F250_uid65
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: 
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X Y
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library std;
use std.textio.all;
library work;

entity DSPBlock_1x7_F250_uid65 is
    port (clk : in std_logic;
          X : in  std_logic_vector(0 downto 0);
          Y : in  std_logic_vector(6 downto 0);
          R : out  std_logic_vector(6 downto 0)   );
end entity;

architecture arch of DSPBlock_1x7_F250_uid65 is
signal Mint :  std_logic_vector(7 downto 0);
signal M :  std_logic_vector(6 downto 0);
signal Rtmp :  std_logic_vector(6 downto 0);
signal X_d1 :  std_logic_vector(0 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            X_d1 <=  X;
         end if;
      end process;
   Mint <= std_logic_vector(unsigned(X_d1) * unsigned(Y)); -- multiplier
   M <= Mint(6 downto 0);
   Rtmp <= M;
   R <= Rtmp;
end architecture;

--------------------------------------------------------------------------------
--                          DSPBlock_6x1_F250_uid67
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: 
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X Y
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library std;
use std.textio.all;
library work;

entity DSPBlock_6x1_F250_uid67 is
    port (clk : in std_logic;
          X : in  std_logic_vector(5 downto 0);
          Y : in  std_logic_vector(0 downto 0);
          R : out  std_logic_vector(5 downto 0)   );
end entity;

architecture arch of DSPBlock_6x1_F250_uid67 is
signal Mint :  std_logic_vector(6 downto 0);
signal M :  std_logic_vector(5 downto 0);
signal Rtmp :  std_logic_vector(5 downto 0);
signal X_d1 :  std_logic_vector(5 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            X_d1 <=  X;
         end if;
      end process;
   Mint <= std_logic_vector(unsigned(X_d1) * unsigned(Y)); -- multiplier
   M <= Mint(5 downto 0);
   Rtmp <= M;
   R <= Rtmp;
end architecture;

--------------------------------------------------------------------------------
--                          DSPBlock_1x6_F250_uid69
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: 
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X Y
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library std;
use std.textio.all;
library work;

entity DSPBlock_1x6_F250_uid69 is
    port (clk : in std_logic;
          X : in  std_logic_vector(0 downto 0);
          Y : in  std_logic_vector(5 downto 0);
          R : out  std_logic_vector(5 downto 0)   );
end entity;

architecture arch of DSPBlock_1x6_F250_uid69 is
signal Mint :  std_logic_vector(6 downto 0);
signal M :  std_logic_vector(5 downto 0);
signal Rtmp :  std_logic_vector(5 downto 0);
signal X_d1 :  std_logic_vector(0 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            X_d1 <=  X;
         end if;
      end process;
   Mint <= std_logic_vector(unsigned(X_d1) * unsigned(Y)); -- multiplier
   M <= Mint(5 downto 0);
   Rtmp <= M;
   R <= Rtmp;
end architecture;

--------------------------------------------------------------------------------
--                          DSPBlock_5x1_F250_uid71
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: 
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X Y
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library std;
use std.textio.all;
library work;

entity DSPBlock_5x1_F250_uid71 is
    port (clk : in std_logic;
          X : in  std_logic_vector(4 downto 0);
          Y : in  std_logic_vector(0 downto 0);
          R : out  std_logic_vector(4 downto 0)   );
end entity;

architecture arch of DSPBlock_5x1_F250_uid71 is
signal Mint :  std_logic_vector(5 downto 0);
signal M :  std_logic_vector(4 downto 0);
signal Rtmp :  std_logic_vector(4 downto 0);
signal X_d1 :  std_logic_vector(4 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            X_d1 <=  X;
         end if;
      end process;
   Mint <= std_logic_vector(unsigned(X_d1) * unsigned(Y)); -- multiplier
   M <= Mint(4 downto 0);
   Rtmp <= M;
   R <= Rtmp;
end architecture;

--------------------------------------------------------------------------------
--                          DSPBlock_4x1_F250_uid73
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: 
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X Y
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library std;
use std.textio.all;
library work;

entity DSPBlock_4x1_F250_uid73 is
    port (clk : in std_logic;
          X : in  std_logic_vector(3 downto 0);
          Y : in  std_logic_vector(0 downto 0);
          R : out  std_logic_vector(3 downto 0)   );
end entity;

architecture arch of DSPBlock_4x1_F250_uid73 is
signal Mint :  std_logic_vector(4 downto 0);
signal M :  std_logic_vector(3 downto 0);
signal Rtmp :  std_logic_vector(3 downto 0);
signal X_d1 :  std_logic_vector(3 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            X_d1 <=  X;
         end if;
      end process;
   Mint <= std_logic_vector(unsigned(X_d1) * unsigned(Y)); -- multiplier
   M <= Mint(3 downto 0);
   Rtmp <= M;
   R <= Rtmp;
end architecture;

--------------------------------------------------------------------------------
--                          IntAdder_31_F250_uid172
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Bogdan Pasca, Florent de Dinechin (2008-2016)
--------------------------------------------------------------------------------
-- Pipeline depth: 1 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X Y Cin
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity IntAdder_31_F250_uid172 is
    port (clk : in std_logic;
          X : in  std_logic_vector(30 downto 0);
          Y : in  std_logic_vector(30 downto 0);
          Cin : in  std_logic;
          R : out  std_logic_vector(30 downto 0)   );
end entity;

architecture arch of IntAdder_31_F250_uid172 is
signal Rtmp :  std_logic_vector(30 downto 0);
signal X_d1 :  std_logic_vector(30 downto 0);
signal Y_d1 :  std_logic_vector(30 downto 0);
signal Cin_d1, Cin_d2, Cin_d3, Cin_d4, Cin_d5 :  std_logic;
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            X_d1 <=  X;
            Y_d1 <=  Y;
            Cin_d1 <=  Cin;
            Cin_d2 <=  Cin_d1;
            Cin_d3 <=  Cin_d2;
            Cin_d4 <=  Cin_d3;
            Cin_d5 <=  Cin_d4;
         end if;
      end process;
   Rtmp <= X_d1 + Y_d1 + Cin_d5;
   R <= Rtmp;
end architecture;

--------------------------------------------------------------------------------
--                          IntMultiplier_F250_uid45
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Martin Kumm, Florent de Dinechin, Kinga Illyes, Bogdan Popa, Bogdan Pasca, 2012
--------------------------------------------------------------------------------
-- Pipeline depth: 1 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X Y
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library std;
use std.textio.all;
library work;

entity IntMultiplier_F250_uid45 is
    port (clk : in std_logic;
          X : in  std_logic_vector(16 downto 0);
          Y : in  std_logic_vector(17 downto 0);
          R : out  std_logic_vector(18 downto 0)   );
end entity;

architecture arch of IntMultiplier_F250_uid45 is
   component DSPBlock_11x12_F250_uid49 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(10 downto 0);
             Y : in  std_logic_vector(11 downto 0);
             R : out  std_logic_vector(22 downto 0)   );
   end component;

   component DSPBlock_1x11_F250_uid51 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(0 downto 0);
             Y : in  std_logic_vector(10 downto 0);
             R : out  std_logic_vector(10 downto 0)   );
   end component;

   component DSPBlock_10x1_F250_uid53 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(9 downto 0);
             Y : in  std_logic_vector(0 downto 0);
             R : out  std_logic_vector(9 downto 0)   );
   end component;

   component DSPBlock_9x1_F250_uid55 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(8 downto 0);
             Y : in  std_logic_vector(0 downto 0);
             R : out  std_logic_vector(8 downto 0)   );
   end component;

   component DSPBlock_1x10_F250_uid57 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(0 downto 0);
             Y : in  std_logic_vector(9 downto 0);
             R : out  std_logic_vector(9 downto 0)   );
   end component;

   component DSPBlock_1x9_F250_uid59 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(0 downto 0);
             Y : in  std_logic_vector(8 downto 0);
             R : out  std_logic_vector(8 downto 0)   );
   end component;

   component DSPBlock_8x1_F250_uid61 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(7 downto 0);
             Y : in  std_logic_vector(0 downto 0);
             R : out  std_logic_vector(7 downto 0)   );
   end component;

   component DSPBlock_1x8_F250_uid63 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(0 downto 0);
             Y : in  std_logic_vector(7 downto 0);
             R : out  std_logic_vector(7 downto 0)   );
   end component;

   component DSPBlock_1x7_F250_uid65 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(0 downto 0);
             Y : in  std_logic_vector(6 downto 0);
             R : out  std_logic_vector(6 downto 0)   );
   end component;

   component DSPBlock_6x1_F250_uid67 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(5 downto 0);
             Y : in  std_logic_vector(0 downto 0);
             R : out  std_logic_vector(5 downto 0)   );
   end component;

   component DSPBlock_1x6_F250_uid69 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(0 downto 0);
             Y : in  std_logic_vector(5 downto 0);
             R : out  std_logic_vector(5 downto 0)   );
   end component;

   component DSPBlock_5x1_F250_uid71 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(4 downto 0);
             Y : in  std_logic_vector(0 downto 0);
             R : out  std_logic_vector(4 downto 0)   );
   end component;

   component DSPBlock_4x1_F250_uid73 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(3 downto 0);
             Y : in  std_logic_vector(0 downto 0);
             R : out  std_logic_vector(3 downto 0)   );
   end component;

   component Compressor_6_3_F250_uid76 is
      port ( X0 : in  std_logic_vector(5 downto 0);
             R : out  std_logic_vector(2 downto 0)   );
   end component;

   component Compressor_14_3_F250_uid80 is
      port ( X1 : in  std_logic_vector(0 downto 0);
             X0 : in  std_logic_vector(3 downto 0);
             R : out  std_logic_vector(2 downto 0)   );
   end component;

   component Compressor_23_3_F250_uid92 is
      port ( X1 : in  std_logic_vector(1 downto 0);
             X0 : in  std_logic_vector(2 downto 0);
             R : out  std_logic_vector(2 downto 0)   );
   end component;

   component Compressor_3_2_F250_uid118 is
      port ( X0 : in  std_logic_vector(2 downto 0);
             R : out  std_logic_vector(1 downto 0)   );
   end component;

   component IntAdder_31_F250_uid172 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(30 downto 0);
             Y : in  std_logic_vector(30 downto 0);
             Cin : in  std_logic;
             R : out  std_logic_vector(30 downto 0)   );
   end component;

signal XX_m46 :  std_logic_vector(16 downto 0);
signal YY_m46 :  std_logic_vector(17 downto 0);
signal tile_0_X :  std_logic_vector(10 downto 0);
signal tile_0_Y :  std_logic_vector(11 downto 0);
signal tile_0_output :  std_logic_vector(22 downto 0);
signal tile_0_filtered_output :  unsigned(22-0 downto 0);
signal bh47_w0_0 :  std_logic;
signal bh47_w1_0 :  std_logic;
signal bh47_w2_0 :  std_logic;
signal bh47_w3_0 :  std_logic;
signal bh47_w4_0 :  std_logic;
signal bh47_w5_0 :  std_logic;
signal bh47_w6_0 :  std_logic;
signal bh47_w7_0 :  std_logic;
signal bh47_w8_0 :  std_logic;
signal bh47_w9_0 :  std_logic;
signal bh47_w10_0 :  std_logic;
signal bh47_w11_0 :  std_logic;
signal bh47_w12_0 :  std_logic;
signal bh47_w13_0 :  std_logic;
signal bh47_w14_0 :  std_logic;
signal bh47_w15_0 :  std_logic;
signal bh47_w16_0 :  std_logic;
signal bh47_w17_0 :  std_logic;
signal bh47_w18_0 :  std_logic;
signal bh47_w19_0 :  std_logic;
signal bh47_w20_0 :  std_logic;
signal bh47_w21_0 :  std_logic;
signal bh47_w22_0 :  std_logic;
signal tile_1_X :  std_logic_vector(0 downto 0);
signal tile_1_Y :  std_logic_vector(10 downto 0);
signal tile_1_output :  std_logic_vector(10 downto 0);
signal tile_1_filtered_output :  unsigned(10-0 downto 0);
signal bh47_w0_1 :  std_logic;
signal bh47_w1_1 :  std_logic;
signal bh47_w2_1 :  std_logic;
signal bh47_w3_1 :  std_logic;
signal bh47_w4_1 :  std_logic;
signal bh47_w5_1 :  std_logic;
signal bh47_w6_1 :  std_logic;
signal bh47_w7_1 :  std_logic;
signal bh47_w8_1 :  std_logic;
signal bh47_w9_1 :  std_logic;
signal bh47_w10_1 :  std_logic;
signal tile_2_X :  std_logic_vector(9 downto 0);
signal tile_2_Y :  std_logic_vector(0 downto 0);
signal tile_2_output :  std_logic_vector(9 downto 0);
signal tile_2_filtered_output :  unsigned(9-0 downto 0);
signal bh47_w0_2 :  std_logic;
signal bh47_w1_2 :  std_logic;
signal bh47_w2_2 :  std_logic;
signal bh47_w3_2 :  std_logic;
signal bh47_w4_2 :  std_logic;
signal bh47_w5_2 :  std_logic;
signal bh47_w6_2 :  std_logic;
signal bh47_w7_2 :  std_logic;
signal bh47_w8_2 :  std_logic;
signal bh47_w9_2 :  std_logic;
signal tile_3_X :  std_logic_vector(8 downto 0);
signal tile_3_Y :  std_logic_vector(0 downto 0);
signal tile_3_output :  std_logic_vector(8 downto 0);
signal tile_3_filtered_output :  unsigned(8-0 downto 0);
signal bh47_w0_3 :  std_logic;
signal bh47_w1_3 :  std_logic;
signal bh47_w2_3 :  std_logic;
signal bh47_w3_3 :  std_logic;
signal bh47_w4_3 :  std_logic;
signal bh47_w5_3 :  std_logic;
signal bh47_w6_3 :  std_logic;
signal bh47_w7_3 :  std_logic;
signal bh47_w8_3 :  std_logic;
signal tile_4_X :  std_logic_vector(0 downto 0);
signal tile_4_Y :  std_logic_vector(9 downto 0);
signal tile_4_output :  std_logic_vector(9 downto 0);
signal tile_4_filtered_output :  unsigned(9-0 downto 0);
signal bh47_w0_4 :  std_logic;
signal bh47_w1_4 :  std_logic;
signal bh47_w2_4 :  std_logic;
signal bh47_w3_4 :  std_logic;
signal bh47_w4_4 :  std_logic;
signal bh47_w5_4 :  std_logic;
signal bh47_w6_4 :  std_logic;
signal bh47_w7_4 :  std_logic;
signal bh47_w8_4 :  std_logic;
signal bh47_w9_3 :  std_logic;
signal tile_5_X :  std_logic_vector(0 downto 0);
signal tile_5_Y :  std_logic_vector(8 downto 0);
signal tile_5_output :  std_logic_vector(8 downto 0);
signal tile_5_filtered_output :  unsigned(8-0 downto 0);
signal bh47_w0_5 :  std_logic;
signal bh47_w1_5 :  std_logic;
signal bh47_w2_5 :  std_logic;
signal bh47_w3_5 :  std_logic;
signal bh47_w4_5 :  std_logic;
signal bh47_w5_5 :  std_logic;
signal bh47_w6_5 :  std_logic;
signal bh47_w7_5 :  std_logic;
signal bh47_w8_5 :  std_logic;
signal tile_6_X :  std_logic_vector(7 downto 0);
signal tile_6_Y :  std_logic_vector(0 downto 0);
signal tile_6_output :  std_logic_vector(7 downto 0);
signal tile_6_filtered_output :  unsigned(7-0 downto 0);
signal bh47_w0_6 :  std_logic;
signal bh47_w1_6 :  std_logic;
signal bh47_w2_6 :  std_logic;
signal bh47_w3_6 :  std_logic;
signal bh47_w4_6 :  std_logic;
signal bh47_w5_6 :  std_logic;
signal bh47_w6_6 :  std_logic;
signal bh47_w7_6 :  std_logic;
signal tile_7_X :  std_logic_vector(0 downto 0);
signal tile_7_Y :  std_logic_vector(7 downto 0);
signal tile_7_output :  std_logic_vector(7 downto 0);
signal tile_7_filtered_output :  unsigned(7-0 downto 0);
signal bh47_w0_7 :  std_logic;
signal bh47_w1_7 :  std_logic;
signal bh47_w2_7 :  std_logic;
signal bh47_w3_7 :  std_logic;
signal bh47_w4_7 :  std_logic;
signal bh47_w5_7 :  std_logic;
signal bh47_w6_7 :  std_logic;
signal bh47_w7_7 :  std_logic;
signal tile_8_X :  std_logic_vector(0 downto 0);
signal tile_8_Y :  std_logic_vector(6 downto 0);
signal tile_8_output :  std_logic_vector(6 downto 0);
signal tile_8_filtered_output :  unsigned(6-0 downto 0);
signal bh47_w0_8 :  std_logic;
signal bh47_w1_8 :  std_logic;
signal bh47_w2_8 :  std_logic;
signal bh47_w3_8 :  std_logic;
signal bh47_w4_8 :  std_logic;
signal bh47_w5_8 :  std_logic;
signal bh47_w6_8 :  std_logic;
signal tile_9_X :  std_logic_vector(5 downto 0);
signal tile_9_Y :  std_logic_vector(0 downto 0);
signal tile_9_output :  std_logic_vector(5 downto 0);
signal tile_9_filtered_output :  unsigned(5-0 downto 0);
signal bh47_w1_9 :  std_logic;
signal bh47_w2_9 :  std_logic;
signal bh47_w3_9 :  std_logic;
signal bh47_w4_9 :  std_logic;
signal bh47_w5_9 :  std_logic;
signal bh47_w6_9 :  std_logic;
signal tile_10_X :  std_logic_vector(0 downto 0);
signal tile_10_Y :  std_logic_vector(5 downto 0);
signal tile_10_output :  std_logic_vector(5 downto 0);
signal tile_10_filtered_output :  unsigned(5-0 downto 0);
signal bh47_w0_9 :  std_logic;
signal bh47_w1_10 :  std_logic;
signal bh47_w2_10 :  std_logic;
signal bh47_w3_10 :  std_logic;
signal bh47_w4_10 :  std_logic;
signal bh47_w5_10 :  std_logic;
signal tile_11_X :  std_logic_vector(4 downto 0);
signal tile_11_Y :  std_logic_vector(0 downto 0);
signal tile_11_output :  std_logic_vector(4 downto 0);
signal tile_11_filtered_output :  unsigned(4-0 downto 0);
signal bh47_w1_11 :  std_logic;
signal bh47_w2_11 :  std_logic;
signal bh47_w3_11 :  std_logic;
signal bh47_w4_11 :  std_logic;
signal bh47_w5_11 :  std_logic;
signal tile_12_X :  std_logic_vector(3 downto 0);
signal tile_12_Y :  std_logic_vector(0 downto 0);
signal tile_12_output :  std_logic_vector(3 downto 0);
signal tile_12_filtered_output :  unsigned(3-0 downto 0);
signal bh47_w1_12 :  std_logic;
signal bh47_w2_12 :  std_logic;
signal bh47_w3_12 :  std_logic;
signal bh47_w4_12 :  std_logic;
signal bh47_w0_10, bh47_w0_10_d1, bh47_w0_10_d2, bh47_w0_10_d3, bh47_w0_10_d4 :  std_logic;
signal bh47_w1_13 :  std_logic;
signal bh47_w2_13, bh47_w2_13_d1, bh47_w2_13_d2, bh47_w2_13_d3, bh47_w2_13_d4 :  std_logic;
signal bh47_w3_13, bh47_w3_13_d1, bh47_w3_13_d2, bh47_w3_13_d3, bh47_w3_13_d4 :  std_logic;
signal Compressor_6_3_F250_uid76_bh47_uid77_In0 :  std_logic_vector(5 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid77_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w0_11 :  std_logic;
signal bh47_w1_14 :  std_logic;
signal bh47_w2_14 :  std_logic;
signal Compressor_6_3_F250_uid76_bh47_uid77_Out0_copy78 :  std_logic_vector(2 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid81_In0 :  std_logic_vector(3 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid81_In1, Compressor_14_3_F250_uid80_bh47_uid81_In1_d1, Compressor_14_3_F250_uid80_bh47_uid81_In1_d2, Compressor_14_3_F250_uid80_bh47_uid81_In1_d3, Compressor_14_3_F250_uid80_bh47_uid81_In1_d4 :  std_logic_vector(0 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid81_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w0_12 :  std_logic;
signal bh47_w1_15 :  std_logic;
signal bh47_w2_15 :  std_logic;
signal Compressor_14_3_F250_uid80_bh47_uid81_Out0_copy82 :  std_logic_vector(2 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid83_In0 :  std_logic_vector(5 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid83_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w1_16 :  std_logic;
signal bh47_w2_16 :  std_logic;
signal bh47_w3_14 :  std_logic;
signal Compressor_6_3_F250_uid76_bh47_uid83_Out0_copy84 :  std_logic_vector(2 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid85_In0 :  std_logic_vector(5 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid85_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w1_17 :  std_logic;
signal bh47_w2_17 :  std_logic;
signal bh47_w3_15 :  std_logic;
signal Compressor_6_3_F250_uid76_bh47_uid85_Out0_copy86 :  std_logic_vector(2 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid87_In0 :  std_logic_vector(5 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid87_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w2_18 :  std_logic;
signal bh47_w3_16 :  std_logic;
signal bh47_w4_13 :  std_logic;
signal Compressor_6_3_F250_uid76_bh47_uid87_Out0_copy88 :  std_logic_vector(2 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid89_In0 :  std_logic_vector(5 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid89_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w2_19 :  std_logic;
signal bh47_w3_17 :  std_logic;
signal bh47_w4_14 :  std_logic;
signal Compressor_6_3_F250_uid76_bh47_uid89_Out0_copy90 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid93_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid93_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid93_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w2_20 :  std_logic;
signal bh47_w3_18 :  std_logic;
signal bh47_w4_15 :  std_logic;
signal Compressor_23_3_F250_uid92_bh47_uid93_Out0_copy94 :  std_logic_vector(2 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid95_In0 :  std_logic_vector(5 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid95_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w3_19 :  std_logic;
signal bh47_w4_16 :  std_logic;
signal bh47_w5_12 :  std_logic;
signal Compressor_6_3_F250_uid76_bh47_uid95_Out0_copy96 :  std_logic_vector(2 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid97_In0 :  std_logic_vector(5 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid97_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w3_20 :  std_logic;
signal bh47_w4_17 :  std_logic;
signal bh47_w5_13 :  std_logic;
signal Compressor_6_3_F250_uid76_bh47_uid97_Out0_copy98 :  std_logic_vector(2 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid99_In0 :  std_logic_vector(5 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid99_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w4_18 :  std_logic;
signal bh47_w5_14 :  std_logic;
signal bh47_w6_10 :  std_logic;
signal Compressor_6_3_F250_uid76_bh47_uid99_Out0_copy100 :  std_logic_vector(2 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid101_In0 :  std_logic_vector(5 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid101_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w4_19 :  std_logic;
signal bh47_w5_15 :  std_logic;
signal bh47_w6_11 :  std_logic;
signal Compressor_6_3_F250_uid76_bh47_uid101_Out0_copy102 :  std_logic_vector(2 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid103_In0 :  std_logic_vector(5 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid103_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w5_16 :  std_logic;
signal bh47_w6_12 :  std_logic;
signal bh47_w7_8 :  std_logic;
signal Compressor_6_3_F250_uid76_bh47_uid103_Out0_copy104 :  std_logic_vector(2 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid105_In0 :  std_logic_vector(5 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid105_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w5_17 :  std_logic;
signal bh47_w6_13 :  std_logic;
signal bh47_w7_9 :  std_logic;
signal Compressor_6_3_F250_uid76_bh47_uid105_Out0_copy106 :  std_logic_vector(2 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid107_In0 :  std_logic_vector(5 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid107_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w6_14 :  std_logic;
signal bh47_w7_10 :  std_logic;
signal bh47_w8_6 :  std_logic;
signal Compressor_6_3_F250_uid76_bh47_uid107_Out0_copy108 :  std_logic_vector(2 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid109_In0 :  std_logic_vector(3 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid109_In1 :  std_logic_vector(0 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid109_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w6_15 :  std_logic;
signal bh47_w7_11 :  std_logic;
signal bh47_w8_7 :  std_logic;
signal Compressor_14_3_F250_uid80_bh47_uid109_Out0_copy110 :  std_logic_vector(2 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid111_In0 :  std_logic_vector(5 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid111_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w7_12 :  std_logic;
signal bh47_w8_8 :  std_logic;
signal bh47_w9_4 :  std_logic;
signal Compressor_6_3_F250_uid76_bh47_uid111_Out0_copy112 :  std_logic_vector(2 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid113_In0 :  std_logic_vector(5 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid113_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w8_9 :  std_logic;
signal bh47_w9_5 :  std_logic;
signal bh47_w10_2 :  std_logic;
signal Compressor_6_3_F250_uid76_bh47_uid113_Out0_copy114 :  std_logic_vector(2 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid115_In0 :  std_logic_vector(3 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid115_In1 :  std_logic_vector(0 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid115_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w9_6 :  std_logic;
signal bh47_w10_3 :  std_logic;
signal bh47_w11_1 :  std_logic;
signal Compressor_14_3_F250_uid80_bh47_uid115_Out0_copy116 :  std_logic_vector(2 downto 0);
signal Compressor_3_2_F250_uid118_bh47_uid119_In0 :  std_logic_vector(2 downto 0);
signal Compressor_3_2_F250_uid118_bh47_uid119_Out0 :  std_logic_vector(1 downto 0);
signal bh47_w0_13 :  std_logic;
signal bh47_w1_18 :  std_logic;
signal Compressor_3_2_F250_uid118_bh47_uid119_Out0_copy120 :  std_logic_vector(1 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid121_In0 :  std_logic_vector(3 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid121_In1 :  std_logic_vector(0 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid121_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w1_19 :  std_logic;
signal bh47_w2_21 :  std_logic;
signal bh47_w3_21 :  std_logic;
signal Compressor_14_3_F250_uid80_bh47_uid121_Out0_copy122 :  std_logic_vector(2 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid123_In0 :  std_logic_vector(5 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid123_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w2_22 :  std_logic;
signal bh47_w3_22 :  std_logic;
signal bh47_w4_20 :  std_logic;
signal Compressor_6_3_F250_uid76_bh47_uid123_Out0_copy124 :  std_logic_vector(2 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid125_In0 :  std_logic_vector(5 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid125_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w3_23 :  std_logic;
signal bh47_w4_21 :  std_logic;
signal bh47_w5_18 :  std_logic;
signal Compressor_6_3_F250_uid76_bh47_uid125_Out0_copy126 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid127_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid127_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid127_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w3_24 :  std_logic;
signal bh47_w4_22 :  std_logic;
signal bh47_w5_19 :  std_logic;
signal Compressor_23_3_F250_uid92_bh47_uid127_Out0_copy128 :  std_logic_vector(2 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid129_In0 :  std_logic_vector(5 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid129_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w4_23 :  std_logic;
signal bh47_w5_20 :  std_logic;
signal bh47_w6_16 :  std_logic;
signal Compressor_6_3_F250_uid76_bh47_uid129_Out0_copy130 :  std_logic_vector(2 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid131_In0 :  std_logic_vector(5 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid131_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w5_21 :  std_logic;
signal bh47_w6_17 :  std_logic;
signal bh47_w7_13 :  std_logic;
signal Compressor_6_3_F250_uid76_bh47_uid131_Out0_copy132 :  std_logic_vector(2 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid133_In0 :  std_logic_vector(5 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid133_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w6_18 :  std_logic;
signal bh47_w7_14 :  std_logic;
signal bh47_w8_10 :  std_logic;
signal Compressor_6_3_F250_uid76_bh47_uid133_Out0_copy134 :  std_logic_vector(2 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid135_In0 :  std_logic_vector(5 downto 0);
signal Compressor_6_3_F250_uid76_bh47_uid135_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w7_15 :  std_logic;
signal bh47_w8_11 :  std_logic;
signal bh47_w9_7 :  std_logic;
signal Compressor_6_3_F250_uid76_bh47_uid135_Out0_copy136 :  std_logic_vector(2 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid137_In0 :  std_logic_vector(3 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid137_In1, Compressor_14_3_F250_uid80_bh47_uid137_In1_d1, Compressor_14_3_F250_uid80_bh47_uid137_In1_d2, Compressor_14_3_F250_uid80_bh47_uid137_In1_d3, Compressor_14_3_F250_uid80_bh47_uid137_In1_d4 :  std_logic_vector(0 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid137_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w8_12 :  std_logic;
signal bh47_w9_8 :  std_logic;
signal bh47_w10_4 :  std_logic;
signal Compressor_14_3_F250_uid80_bh47_uid137_Out0_copy138 :  std_logic_vector(2 downto 0);
signal Compressor_3_2_F250_uid118_bh47_uid139_In0 :  std_logic_vector(2 downto 0);
signal Compressor_3_2_F250_uid118_bh47_uid139_Out0 :  std_logic_vector(1 downto 0);
signal bh47_w9_9 :  std_logic;
signal bh47_w10_5 :  std_logic;
signal Compressor_3_2_F250_uid118_bh47_uid139_Out0_copy140 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid141_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid141_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid141_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w10_6 :  std_logic;
signal bh47_w11_2 :  std_logic;
signal bh47_w12_1 :  std_logic;
signal Compressor_23_3_F250_uid92_bh47_uid141_Out0_copy142 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid143_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid143_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid143_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w1_20 :  std_logic;
signal bh47_w2_23 :  std_logic;
signal bh47_w3_25 :  std_logic;
signal Compressor_23_3_F250_uid92_bh47_uid143_Out0_copy144 :  std_logic_vector(2 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid145_In0 :  std_logic_vector(3 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid145_In1 :  std_logic_vector(0 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid145_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w3_26 :  std_logic;
signal bh47_w4_24 :  std_logic;
signal bh47_w5_22 :  std_logic;
signal Compressor_14_3_F250_uid80_bh47_uid145_Out0_copy146 :  std_logic_vector(2 downto 0);
signal Compressor_3_2_F250_uid118_bh47_uid147_In0 :  std_logic_vector(2 downto 0);
signal Compressor_3_2_F250_uid118_bh47_uid147_Out0 :  std_logic_vector(1 downto 0);
signal bh47_w4_25 :  std_logic;
signal bh47_w5_23 :  std_logic;
signal Compressor_3_2_F250_uid118_bh47_uid147_Out0_copy148 :  std_logic_vector(1 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid149_In0 :  std_logic_vector(3 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid149_In1, Compressor_14_3_F250_uid80_bh47_uid149_In1_d1, Compressor_14_3_F250_uid80_bh47_uid149_In1_d2, Compressor_14_3_F250_uid80_bh47_uid149_In1_d3, Compressor_14_3_F250_uid80_bh47_uid149_In1_d4 :  std_logic_vector(0 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid149_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w5_24 :  std_logic;
signal bh47_w6_19 :  std_logic;
signal bh47_w7_16 :  std_logic;
signal Compressor_14_3_F250_uid80_bh47_uid149_Out0_copy150 :  std_logic_vector(2 downto 0);
signal Compressor_3_2_F250_uid118_bh47_uid151_In0 :  std_logic_vector(2 downto 0);
signal Compressor_3_2_F250_uid118_bh47_uid151_Out0 :  std_logic_vector(1 downto 0);
signal bh47_w6_20 :  std_logic;
signal bh47_w7_17 :  std_logic;
signal Compressor_3_2_F250_uid118_bh47_uid151_Out0_copy152 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid153_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid153_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid153_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w7_18 :  std_logic;
signal bh47_w8_13 :  std_logic;
signal bh47_w9_10 :  std_logic;
signal Compressor_23_3_F250_uid92_bh47_uid153_Out0_copy154 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid155_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid155_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid155_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w9_11 :  std_logic;
signal bh47_w10_7 :  std_logic;
signal bh47_w11_3 :  std_logic;
signal Compressor_23_3_F250_uid92_bh47_uid155_Out0_copy156 :  std_logic_vector(2 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid157_In0 :  std_logic_vector(3 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid157_In1 :  std_logic_vector(0 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid157_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w12_2 :  std_logic;
signal bh47_w13_1 :  std_logic;
signal bh47_w14_1 :  std_logic;
signal Compressor_14_3_F250_uid80_bh47_uid157_Out0_copy158 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid159_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid159_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid159_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w3_27 :  std_logic;
signal bh47_w4_26 :  std_logic;
signal bh47_w5_25 :  std_logic;
signal Compressor_23_3_F250_uid92_bh47_uid159_Out0_copy160 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid161_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid161_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid161_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w5_26 :  std_logic;
signal bh47_w6_21 :  std_logic;
signal bh47_w7_19 :  std_logic;
signal Compressor_23_3_F250_uid92_bh47_uid161_Out0_copy162 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid163_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid163_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid163_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w7_20 :  std_logic;
signal bh47_w8_14 :  std_logic;
signal bh47_w9_12 :  std_logic;
signal Compressor_23_3_F250_uid92_bh47_uid163_Out0_copy164 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid165_In0 :  std_logic_vector(2 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid165_In1 :  std_logic_vector(1 downto 0);
signal Compressor_23_3_F250_uid92_bh47_uid165_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w9_13 :  std_logic;
signal bh47_w10_8 :  std_logic;
signal bh47_w11_4 :  std_logic;
signal Compressor_23_3_F250_uid92_bh47_uid165_Out0_copy166 :  std_logic_vector(2 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid167_In0 :  std_logic_vector(3 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid167_In1 :  std_logic_vector(0 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid167_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w11_5 :  std_logic;
signal bh47_w12_3 :  std_logic;
signal bh47_w13_2 :  std_logic;
signal Compressor_14_3_F250_uid80_bh47_uid167_Out0_copy168 :  std_logic_vector(2 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid169_In0 :  std_logic_vector(3 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid169_In1 :  std_logic_vector(0 downto 0);
signal Compressor_14_3_F250_uid80_bh47_uid169_Out0 :  std_logic_vector(2 downto 0);
signal bh47_w14_2 :  std_logic;
signal bh47_w15_1 :  std_logic;
signal bh47_w16_1 :  std_logic;
signal Compressor_14_3_F250_uid80_bh47_uid169_Out0_copy170 :  std_logic_vector(2 downto 0);
signal tmp_bitheapResult_bh47_4, tmp_bitheapResult_bh47_4_d1 :  std_logic_vector(4 downto 0);
signal bitheapFinalAdd_bh47_In0 :  std_logic_vector(30 downto 0);
signal bitheapFinalAdd_bh47_In1 :  std_logic_vector(30 downto 0);
signal bitheapFinalAdd_bh47_Cin :  std_logic;
signal bitheapFinalAdd_bh47_Out :  std_logic_vector(30 downto 0);
signal bitheapResult_bh47 :  std_logic_vector(34 downto 0);
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            bh47_w0_10_d1 <=  bh47_w0_10;
            bh47_w0_10_d2 <=  bh47_w0_10_d1;
            bh47_w0_10_d3 <=  bh47_w0_10_d2;
            bh47_w0_10_d4 <=  bh47_w0_10_d3;
            bh47_w2_13_d1 <=  bh47_w2_13;
            bh47_w2_13_d2 <=  bh47_w2_13_d1;
            bh47_w2_13_d3 <=  bh47_w2_13_d2;
            bh47_w2_13_d4 <=  bh47_w2_13_d3;
            bh47_w3_13_d1 <=  bh47_w3_13;
            bh47_w3_13_d2 <=  bh47_w3_13_d1;
            bh47_w3_13_d3 <=  bh47_w3_13_d2;
            bh47_w3_13_d4 <=  bh47_w3_13_d3;
            Compressor_14_3_F250_uid80_bh47_uid81_In1_d1 <=  Compressor_14_3_F250_uid80_bh47_uid81_In1;
            Compressor_14_3_F250_uid80_bh47_uid81_In1_d2 <=  Compressor_14_3_F250_uid80_bh47_uid81_In1_d1;
            Compressor_14_3_F250_uid80_bh47_uid81_In1_d3 <=  Compressor_14_3_F250_uid80_bh47_uid81_In1_d2;
            Compressor_14_3_F250_uid80_bh47_uid81_In1_d4 <=  Compressor_14_3_F250_uid80_bh47_uid81_In1_d3;
            Compressor_14_3_F250_uid80_bh47_uid137_In1_d1 <=  Compressor_14_3_F250_uid80_bh47_uid137_In1;
            Compressor_14_3_F250_uid80_bh47_uid137_In1_d2 <=  Compressor_14_3_F250_uid80_bh47_uid137_In1_d1;
            Compressor_14_3_F250_uid80_bh47_uid137_In1_d3 <=  Compressor_14_3_F250_uid80_bh47_uid137_In1_d2;
            Compressor_14_3_F250_uid80_bh47_uid137_In1_d4 <=  Compressor_14_3_F250_uid80_bh47_uid137_In1_d3;
            Compressor_14_3_F250_uid80_bh47_uid149_In1_d1 <=  Compressor_14_3_F250_uid80_bh47_uid149_In1;
            Compressor_14_3_F250_uid80_bh47_uid149_In1_d2 <=  Compressor_14_3_F250_uid80_bh47_uid149_In1_d1;
            Compressor_14_3_F250_uid80_bh47_uid149_In1_d3 <=  Compressor_14_3_F250_uid80_bh47_uid149_In1_d2;
            Compressor_14_3_F250_uid80_bh47_uid149_In1_d4 <=  Compressor_14_3_F250_uid80_bh47_uid149_In1_d3;
            tmp_bitheapResult_bh47_4_d1 <=  tmp_bitheapResult_bh47_4;
         end if;
      end process;
   XX_m46 <= X ;
   YY_m46 <= Y ;
   tile_0_X <= X(16 downto 6);
   tile_0_Y <= Y(17 downto 6);
   tile_0_mult: DSPBlock_11x12_F250_uid49
      port map ( clk  => clk,
                 X => tile_0_X,
                 Y => tile_0_Y,
                 R => tile_0_output);

   tile_0_filtered_output <= unsigned(tile_0_output(22 downto 0));
   bh47_w0_0 <= tile_0_filtered_output(0);
   bh47_w1_0 <= tile_0_filtered_output(1);
   bh47_w2_0 <= tile_0_filtered_output(2);
   bh47_w3_0 <= tile_0_filtered_output(3);
   bh47_w4_0 <= tile_0_filtered_output(4);
   bh47_w5_0 <= tile_0_filtered_output(5);
   bh47_w6_0 <= tile_0_filtered_output(6);
   bh47_w7_0 <= tile_0_filtered_output(7);
   bh47_w8_0 <= tile_0_filtered_output(8);
   bh47_w9_0 <= tile_0_filtered_output(9);
   bh47_w10_0 <= tile_0_filtered_output(10);
   bh47_w11_0 <= tile_0_filtered_output(11);
   bh47_w12_0 <= tile_0_filtered_output(12);
   bh47_w13_0 <= tile_0_filtered_output(13);
   bh47_w14_0 <= tile_0_filtered_output(14);
   bh47_w15_0 <= tile_0_filtered_output(15);
   bh47_w16_0 <= tile_0_filtered_output(16);
   bh47_w17_0 <= tile_0_filtered_output(17);
   bh47_w18_0 <= tile_0_filtered_output(18);
   bh47_w19_0 <= tile_0_filtered_output(19);
   bh47_w20_0 <= tile_0_filtered_output(20);
   bh47_w21_0 <= tile_0_filtered_output(21);
   bh47_w22_0 <= tile_0_filtered_output(22);
   tile_1_X <= X(5 downto 5);
   tile_1_Y <= Y(17 downto 7);
   tile_1_mult: DSPBlock_1x11_F250_uid51
      port map ( clk  => clk,
                 X => tile_1_X,
                 Y => tile_1_Y,
                 R => tile_1_output);

   tile_1_filtered_output <= unsigned(tile_1_output(10 downto 0));
   bh47_w0_1 <= tile_1_filtered_output(0);
   bh47_w1_1 <= tile_1_filtered_output(1);
   bh47_w2_1 <= tile_1_filtered_output(2);
   bh47_w3_1 <= tile_1_filtered_output(3);
   bh47_w4_1 <= tile_1_filtered_output(4);
   bh47_w5_1 <= tile_1_filtered_output(5);
   bh47_w6_1 <= tile_1_filtered_output(6);
   bh47_w7_1 <= tile_1_filtered_output(7);
   bh47_w8_1 <= tile_1_filtered_output(8);
   bh47_w9_1 <= tile_1_filtered_output(9);
   bh47_w10_1 <= tile_1_filtered_output(10);
   tile_2_X <= X(16 downto 7);
   tile_2_Y <= Y(5 downto 5);
   tile_2_mult: DSPBlock_10x1_F250_uid53
      port map ( clk  => clk,
                 X => tile_2_X,
                 Y => tile_2_Y,
                 R => tile_2_output);

   tile_2_filtered_output <= unsigned(tile_2_output(9 downto 0));
   bh47_w0_2 <= tile_2_filtered_output(0);
   bh47_w1_2 <= tile_2_filtered_output(1);
   bh47_w2_2 <= tile_2_filtered_output(2);
   bh47_w3_2 <= tile_2_filtered_output(3);
   bh47_w4_2 <= tile_2_filtered_output(4);
   bh47_w5_2 <= tile_2_filtered_output(5);
   bh47_w6_2 <= tile_2_filtered_output(6);
   bh47_w7_2 <= tile_2_filtered_output(7);
   bh47_w8_2 <= tile_2_filtered_output(8);
   bh47_w9_2 <= tile_2_filtered_output(9);
   tile_3_X <= X(16 downto 8);
   tile_3_Y <= Y(4 downto 4);
   tile_3_mult: DSPBlock_9x1_F250_uid55
      port map ( clk  => clk,
                 X => tile_3_X,
                 Y => tile_3_Y,
                 R => tile_3_output);

   tile_3_filtered_output <= unsigned(tile_3_output(8 downto 0));
   bh47_w0_3 <= tile_3_filtered_output(0);
   bh47_w1_3 <= tile_3_filtered_output(1);
   bh47_w2_3 <= tile_3_filtered_output(2);
   bh47_w3_3 <= tile_3_filtered_output(3);
   bh47_w4_3 <= tile_3_filtered_output(4);
   bh47_w5_3 <= tile_3_filtered_output(5);
   bh47_w6_3 <= tile_3_filtered_output(6);
   bh47_w7_3 <= tile_3_filtered_output(7);
   bh47_w8_3 <= tile_3_filtered_output(8);
   tile_4_X <= X(4 downto 4);
   tile_4_Y <= Y(17 downto 8);
   tile_4_mult: DSPBlock_1x10_F250_uid57
      port map ( clk  => clk,
                 X => tile_4_X,
                 Y => tile_4_Y,
                 R => tile_4_output);

   tile_4_filtered_output <= unsigned(tile_4_output(9 downto 0));
   bh47_w0_4 <= tile_4_filtered_output(0);
   bh47_w1_4 <= tile_4_filtered_output(1);
   bh47_w2_4 <= tile_4_filtered_output(2);
   bh47_w3_4 <= tile_4_filtered_output(3);
   bh47_w4_4 <= tile_4_filtered_output(4);
   bh47_w5_4 <= tile_4_filtered_output(5);
   bh47_w6_4 <= tile_4_filtered_output(6);
   bh47_w7_4 <= tile_4_filtered_output(7);
   bh47_w8_4 <= tile_4_filtered_output(8);
   bh47_w9_3 <= tile_4_filtered_output(9);
   tile_5_X <= X(3 downto 3);
   tile_5_Y <= Y(17 downto 9);
   tile_5_mult: DSPBlock_1x9_F250_uid59
      port map ( clk  => clk,
                 X => tile_5_X,
                 Y => tile_5_Y,
                 R => tile_5_output);

   tile_5_filtered_output <= unsigned(tile_5_output(8 downto 0));
   bh47_w0_5 <= tile_5_filtered_output(0);
   bh47_w1_5 <= tile_5_filtered_output(1);
   bh47_w2_5 <= tile_5_filtered_output(2);
   bh47_w3_5 <= tile_5_filtered_output(3);
   bh47_w4_5 <= tile_5_filtered_output(4);
   bh47_w5_5 <= tile_5_filtered_output(5);
   bh47_w6_5 <= tile_5_filtered_output(6);
   bh47_w7_5 <= tile_5_filtered_output(7);
   bh47_w8_5 <= tile_5_filtered_output(8);
   tile_6_X <= X(16 downto 9);
   tile_6_Y <= Y(3 downto 3);
   tile_6_mult: DSPBlock_8x1_F250_uid61
      port map ( clk  => clk,
                 X => tile_6_X,
                 Y => tile_6_Y,
                 R => tile_6_output);

   tile_6_filtered_output <= unsigned(tile_6_output(7 downto 0));
   bh47_w0_6 <= tile_6_filtered_output(0);
   bh47_w1_6 <= tile_6_filtered_output(1);
   bh47_w2_6 <= tile_6_filtered_output(2);
   bh47_w3_6 <= tile_6_filtered_output(3);
   bh47_w4_6 <= tile_6_filtered_output(4);
   bh47_w5_6 <= tile_6_filtered_output(5);
   bh47_w6_6 <= tile_6_filtered_output(6);
   bh47_w7_6 <= tile_6_filtered_output(7);
   tile_7_X <= X(2 downto 2);
   tile_7_Y <= Y(17 downto 10);
   tile_7_mult: DSPBlock_1x8_F250_uid63
      port map ( clk  => clk,
                 X => tile_7_X,
                 Y => tile_7_Y,
                 R => tile_7_output);

   tile_7_filtered_output <= unsigned(tile_7_output(7 downto 0));
   bh47_w0_7 <= tile_7_filtered_output(0);
   bh47_w1_7 <= tile_7_filtered_output(1);
   bh47_w2_7 <= tile_7_filtered_output(2);
   bh47_w3_7 <= tile_7_filtered_output(3);
   bh47_w4_7 <= tile_7_filtered_output(4);
   bh47_w5_7 <= tile_7_filtered_output(5);
   bh47_w6_7 <= tile_7_filtered_output(6);
   bh47_w7_7 <= tile_7_filtered_output(7);
   tile_8_X <= X(1 downto 1);
   tile_8_Y <= Y(17 downto 11);
   tile_8_mult: DSPBlock_1x7_F250_uid65
      port map ( clk  => clk,
                 X => tile_8_X,
                 Y => tile_8_Y,
                 R => tile_8_output);

   tile_8_filtered_output <= unsigned(tile_8_output(6 downto 0));
   bh47_w0_8 <= tile_8_filtered_output(0);
   bh47_w1_8 <= tile_8_filtered_output(1);
   bh47_w2_8 <= tile_8_filtered_output(2);
   bh47_w3_8 <= tile_8_filtered_output(3);
   bh47_w4_8 <= tile_8_filtered_output(4);
   bh47_w5_8 <= tile_8_filtered_output(5);
   bh47_w6_8 <= tile_8_filtered_output(6);
   tile_9_X <= X(16 downto 11);
   tile_9_Y <= Y(2 downto 2);
   tile_9_mult: DSPBlock_6x1_F250_uid67
      port map ( clk  => clk,
                 X => tile_9_X,
                 Y => tile_9_Y,
                 R => tile_9_output);

   tile_9_filtered_output <= unsigned(tile_9_output(5 downto 0));
   bh47_w1_9 <= tile_9_filtered_output(0);
   bh47_w2_9 <= tile_9_filtered_output(1);
   bh47_w3_9 <= tile_9_filtered_output(2);
   bh47_w4_9 <= tile_9_filtered_output(3);
   bh47_w5_9 <= tile_9_filtered_output(4);
   bh47_w6_9 <= tile_9_filtered_output(5);
   tile_10_X <= X(0 downto 0);
   tile_10_Y <= Y(17 downto 12);
   tile_10_mult: DSPBlock_1x6_F250_uid69
      port map ( clk  => clk,
                 X => tile_10_X,
                 Y => tile_10_Y,
                 R => tile_10_output);

   tile_10_filtered_output <= unsigned(tile_10_output(5 downto 0));
   bh47_w0_9 <= tile_10_filtered_output(0);
   bh47_w1_10 <= tile_10_filtered_output(1);
   bh47_w2_10 <= tile_10_filtered_output(2);
   bh47_w3_10 <= tile_10_filtered_output(3);
   bh47_w4_10 <= tile_10_filtered_output(4);
   bh47_w5_10 <= tile_10_filtered_output(5);
   tile_11_X <= X(16 downto 12);
   tile_11_Y <= Y(1 downto 1);
   tile_11_mult: DSPBlock_5x1_F250_uid71
      port map ( clk  => clk,
                 X => tile_11_X,
                 Y => tile_11_Y,
                 R => tile_11_output);

   tile_11_filtered_output <= unsigned(tile_11_output(4 downto 0));
   bh47_w1_11 <= tile_11_filtered_output(0);
   bh47_w2_11 <= tile_11_filtered_output(1);
   bh47_w3_11 <= tile_11_filtered_output(2);
   bh47_w4_11 <= tile_11_filtered_output(3);
   bh47_w5_11 <= tile_11_filtered_output(4);
   tile_12_X <= X(16 downto 13);
   tile_12_Y <= Y(0 downto 0);
   tile_12_mult: DSPBlock_4x1_F250_uid73
      port map ( clk  => clk,
                 X => tile_12_X,
                 Y => tile_12_Y,
                 R => tile_12_output);

   tile_12_filtered_output <= unsigned(tile_12_output(3 downto 0));
   bh47_w1_12 <= tile_12_filtered_output(0);
   bh47_w2_12 <= tile_12_filtered_output(1);
   bh47_w3_12 <= tile_12_filtered_output(2);
   bh47_w4_12 <= tile_12_filtered_output(3);

   -- Adding the constant bits 
   bh47_w0_10 <= '1';
   bh47_w1_13 <= '1';
   bh47_w2_13 <= '1';
   bh47_w3_13 <= '1';


   Compressor_6_3_F250_uid76_bh47_uid77_In0 <= "" & bh47_w0_10_d4 & bh47_w0_0 & bh47_w0_1 & bh47_w0_2 & bh47_w0_3 & bh47_w0_4;
   bh47_w0_11 <= Compressor_6_3_F250_uid76_bh47_uid77_Out0(0);
   bh47_w1_14 <= Compressor_6_3_F250_uid76_bh47_uid77_Out0(1);
   bh47_w2_14 <= Compressor_6_3_F250_uid76_bh47_uid77_Out0(2);
   Compressor_6_3_F250_uid76_uid77: Compressor_6_3_F250_uid76
      port map ( X0 => Compressor_6_3_F250_uid76_bh47_uid77_In0,
                 R => Compressor_6_3_F250_uid76_bh47_uid77_Out0_copy78);
   Compressor_6_3_F250_uid76_bh47_uid77_Out0 <= Compressor_6_3_F250_uid76_bh47_uid77_Out0_copy78; -- output copy to hold a pipeline register if needed


   Compressor_14_3_F250_uid80_bh47_uid81_In0 <= "" & bh47_w0_5 & bh47_w0_6 & bh47_w0_7 & bh47_w0_8;
   Compressor_14_3_F250_uid80_bh47_uid81_In1 <= "" & bh47_w1_13;
   bh47_w0_12 <= Compressor_14_3_F250_uid80_bh47_uid81_Out0(0);
   bh47_w1_15 <= Compressor_14_3_F250_uid80_bh47_uid81_Out0(1);
   bh47_w2_15 <= Compressor_14_3_F250_uid80_bh47_uid81_Out0(2);
   Compressor_14_3_F250_uid80_uid81: Compressor_14_3_F250_uid80
      port map ( X0 => Compressor_14_3_F250_uid80_bh47_uid81_In0,
                 X1 => Compressor_14_3_F250_uid80_bh47_uid81_In1_d4,
                 R => Compressor_14_3_F250_uid80_bh47_uid81_Out0_copy82);
   Compressor_14_3_F250_uid80_bh47_uid81_Out0 <= Compressor_14_3_F250_uid80_bh47_uid81_Out0_copy82; -- output copy to hold a pipeline register if needed


   Compressor_6_3_F250_uid76_bh47_uid83_In0 <= "" & bh47_w1_0 & bh47_w1_1 & bh47_w1_2 & bh47_w1_3 & bh47_w1_4 & bh47_w1_5;
   bh47_w1_16 <= Compressor_6_3_F250_uid76_bh47_uid83_Out0(0);
   bh47_w2_16 <= Compressor_6_3_F250_uid76_bh47_uid83_Out0(1);
   bh47_w3_14 <= Compressor_6_3_F250_uid76_bh47_uid83_Out0(2);
   Compressor_6_3_F250_uid76_uid83: Compressor_6_3_F250_uid76
      port map ( X0 => Compressor_6_3_F250_uid76_bh47_uid83_In0,
                 R => Compressor_6_3_F250_uid76_bh47_uid83_Out0_copy84);
   Compressor_6_3_F250_uid76_bh47_uid83_Out0 <= Compressor_6_3_F250_uid76_bh47_uid83_Out0_copy84; -- output copy to hold a pipeline register if needed


   Compressor_6_3_F250_uid76_bh47_uid85_In0 <= "" & bh47_w1_12 & bh47_w1_11 & bh47_w1_10 & bh47_w1_9 & bh47_w1_8 & bh47_w1_7;
   bh47_w1_17 <= Compressor_6_3_F250_uid76_bh47_uid85_Out0(0);
   bh47_w2_17 <= Compressor_6_3_F250_uid76_bh47_uid85_Out0(1);
   bh47_w3_15 <= Compressor_6_3_F250_uid76_bh47_uid85_Out0(2);
   Compressor_6_3_F250_uid76_uid85: Compressor_6_3_F250_uid76
      port map ( X0 => Compressor_6_3_F250_uid76_bh47_uid85_In0,
                 R => Compressor_6_3_F250_uid76_bh47_uid85_Out0_copy86);
   Compressor_6_3_F250_uid76_bh47_uid85_Out0 <= Compressor_6_3_F250_uid76_bh47_uid85_Out0_copy86; -- output copy to hold a pipeline register if needed


   Compressor_6_3_F250_uid76_bh47_uid87_In0 <= "" & bh47_w2_13_d4 & bh47_w2_0 & bh47_w2_1 & bh47_w2_2 & bh47_w2_3 & "0";
   bh47_w2_18 <= Compressor_6_3_F250_uid76_bh47_uid87_Out0(0);
   bh47_w3_16 <= Compressor_6_3_F250_uid76_bh47_uid87_Out0(1);
   bh47_w4_13 <= Compressor_6_3_F250_uid76_bh47_uid87_Out0(2);
   Compressor_6_3_F250_uid76_uid87: Compressor_6_3_F250_uid76
      port map ( X0 => Compressor_6_3_F250_uid76_bh47_uid87_In0,
                 R => Compressor_6_3_F250_uid76_bh47_uid87_Out0_copy88);
   Compressor_6_3_F250_uid76_bh47_uid87_Out0 <= Compressor_6_3_F250_uid76_bh47_uid87_Out0_copy88; -- output copy to hold a pipeline register if needed


   Compressor_6_3_F250_uid76_bh47_uid89_In0 <= "" & bh47_w2_12 & bh47_w2_11 & bh47_w2_10 & bh47_w2_9 & bh47_w2_8 & bh47_w2_7;
   bh47_w2_19 <= Compressor_6_3_F250_uid76_bh47_uid89_Out0(0);
   bh47_w3_17 <= Compressor_6_3_F250_uid76_bh47_uid89_Out0(1);
   bh47_w4_14 <= Compressor_6_3_F250_uid76_bh47_uid89_Out0(2);
   Compressor_6_3_F250_uid76_uid89: Compressor_6_3_F250_uid76
      port map ( X0 => Compressor_6_3_F250_uid76_bh47_uid89_In0,
                 R => Compressor_6_3_F250_uid76_bh47_uid89_Out0_copy90);
   Compressor_6_3_F250_uid76_bh47_uid89_Out0 <= Compressor_6_3_F250_uid76_bh47_uid89_Out0_copy90; -- output copy to hold a pipeline register if needed


   Compressor_23_3_F250_uid92_bh47_uid93_In0 <= "" & bh47_w2_4 & bh47_w2_5 & bh47_w2_6;
   Compressor_23_3_F250_uid92_bh47_uid93_In1 <= "" & bh47_w3_13_d4 & bh47_w3_6;
   bh47_w2_20 <= Compressor_23_3_F250_uid92_bh47_uid93_Out0(0);
   bh47_w3_18 <= Compressor_23_3_F250_uid92_bh47_uid93_Out0(1);
   bh47_w4_15 <= Compressor_23_3_F250_uid92_bh47_uid93_Out0(2);
   Compressor_23_3_F250_uid92_uid93: Compressor_23_3_F250_uid92
      port map ( X0 => Compressor_23_3_F250_uid92_bh47_uid93_In0,
                 X1 => Compressor_23_3_F250_uid92_bh47_uid93_In1,
                 R => Compressor_23_3_F250_uid92_bh47_uid93_Out0_copy94);
   Compressor_23_3_F250_uid92_bh47_uid93_Out0 <= Compressor_23_3_F250_uid92_bh47_uid93_Out0_copy94; -- output copy to hold a pipeline register if needed


   Compressor_6_3_F250_uid76_bh47_uid95_In0 <= "" & bh47_w3_3 & bh47_w3_4 & bh47_w3_12 & bh47_w3_11 & bh47_w3_10 & bh47_w3_9;
   bh47_w3_19 <= Compressor_6_3_F250_uid76_bh47_uid95_Out0(0);
   bh47_w4_16 <= Compressor_6_3_F250_uid76_bh47_uid95_Out0(1);
   bh47_w5_12 <= Compressor_6_3_F250_uid76_bh47_uid95_Out0(2);
   Compressor_6_3_F250_uid76_uid95: Compressor_6_3_F250_uid76
      port map ( X0 => Compressor_6_3_F250_uid76_bh47_uid95_In0,
                 R => Compressor_6_3_F250_uid76_bh47_uid95_Out0_copy96);
   Compressor_6_3_F250_uid76_bh47_uid95_Out0 <= Compressor_6_3_F250_uid76_bh47_uid95_Out0_copy96; -- output copy to hold a pipeline register if needed


   Compressor_6_3_F250_uid76_bh47_uid97_In0 <= "" & bh47_w3_7 & bh47_w3_0 & bh47_w3_1 & bh47_w3_2 & bh47_w3_5 & bh47_w3_8;
   bh47_w3_20 <= Compressor_6_3_F250_uid76_bh47_uid97_Out0(0);
   bh47_w4_17 <= Compressor_6_3_F250_uid76_bh47_uid97_Out0(1);
   bh47_w5_13 <= Compressor_6_3_F250_uid76_bh47_uid97_Out0(2);
   Compressor_6_3_F250_uid76_uid97: Compressor_6_3_F250_uid76
      port map ( X0 => Compressor_6_3_F250_uid76_bh47_uid97_In0,
                 R => Compressor_6_3_F250_uid76_bh47_uid97_Out0_copy98);
   Compressor_6_3_F250_uid76_bh47_uid97_Out0 <= Compressor_6_3_F250_uid76_bh47_uid97_Out0_copy98; -- output copy to hold a pipeline register if needed


   Compressor_6_3_F250_uid76_bh47_uid99_In0 <= "" & bh47_w4_5 & bh47_w4_0 & bh47_w4_1 & bh47_w4_2 & bh47_w4_3 & bh47_w4_6;
   bh47_w4_18 <= Compressor_6_3_F250_uid76_bh47_uid99_Out0(0);
   bh47_w5_14 <= Compressor_6_3_F250_uid76_bh47_uid99_Out0(1);
   bh47_w6_10 <= Compressor_6_3_F250_uid76_bh47_uid99_Out0(2);
   Compressor_6_3_F250_uid76_uid99: Compressor_6_3_F250_uid76
      port map ( X0 => Compressor_6_3_F250_uid76_bh47_uid99_In0,
                 R => Compressor_6_3_F250_uid76_bh47_uid99_Out0_copy100);
   Compressor_6_3_F250_uid76_bh47_uid99_Out0 <= Compressor_6_3_F250_uid76_bh47_uid99_Out0_copy100; -- output copy to hold a pipeline register if needed


   Compressor_6_3_F250_uid76_bh47_uid101_In0 <= "" & bh47_w4_4 & bh47_w4_12 & bh47_w4_11 & bh47_w4_10 & bh47_w4_9 & bh47_w4_8;
   bh47_w4_19 <= Compressor_6_3_F250_uid76_bh47_uid101_Out0(0);
   bh47_w5_15 <= Compressor_6_3_F250_uid76_bh47_uid101_Out0(1);
   bh47_w6_11 <= Compressor_6_3_F250_uid76_bh47_uid101_Out0(2);
   Compressor_6_3_F250_uid76_uid101: Compressor_6_3_F250_uid76
      port map ( X0 => Compressor_6_3_F250_uid76_bh47_uid101_In0,
                 R => Compressor_6_3_F250_uid76_bh47_uid101_Out0_copy102);
   Compressor_6_3_F250_uid76_bh47_uid101_Out0 <= Compressor_6_3_F250_uid76_bh47_uid101_Out0_copy102; -- output copy to hold a pipeline register if needed


   Compressor_6_3_F250_uid76_bh47_uid103_In0 <= "" & bh47_w5_0 & bh47_w5_1 & bh47_w5_2 & bh47_w5_3 & bh47_w5_4 & bh47_w5_5;
   bh47_w5_16 <= Compressor_6_3_F250_uid76_bh47_uid103_Out0(0);
   bh47_w6_12 <= Compressor_6_3_F250_uid76_bh47_uid103_Out0(1);
   bh47_w7_8 <= Compressor_6_3_F250_uid76_bh47_uid103_Out0(2);
   Compressor_6_3_F250_uid76_uid103: Compressor_6_3_F250_uid76
      port map ( X0 => Compressor_6_3_F250_uid76_bh47_uid103_In0,
                 R => Compressor_6_3_F250_uid76_bh47_uid103_Out0_copy104);
   Compressor_6_3_F250_uid76_bh47_uid103_Out0 <= Compressor_6_3_F250_uid76_bh47_uid103_Out0_copy104; -- output copy to hold a pipeline register if needed


   Compressor_6_3_F250_uid76_bh47_uid105_In0 <= "" & bh47_w5_11 & bh47_w5_10 & bh47_w5_9 & bh47_w5_8 & bh47_w5_7 & bh47_w5_6;
   bh47_w5_17 <= Compressor_6_3_F250_uid76_bh47_uid105_Out0(0);
   bh47_w6_13 <= Compressor_6_3_F250_uid76_bh47_uid105_Out0(1);
   bh47_w7_9 <= Compressor_6_3_F250_uid76_bh47_uid105_Out0(2);
   Compressor_6_3_F250_uid76_uid105: Compressor_6_3_F250_uid76
      port map ( X0 => Compressor_6_3_F250_uid76_bh47_uid105_In0,
                 R => Compressor_6_3_F250_uid76_bh47_uid105_Out0_copy106);
   Compressor_6_3_F250_uid76_bh47_uid105_Out0 <= Compressor_6_3_F250_uid76_bh47_uid105_Out0_copy106; -- output copy to hold a pipeline register if needed


   Compressor_6_3_F250_uid76_bh47_uid107_In0 <= "" & bh47_w6_0 & bh47_w6_1 & bh47_w6_2 & bh47_w6_3 & bh47_w6_4 & bh47_w6_5;
   bh47_w6_14 <= Compressor_6_3_F250_uid76_bh47_uid107_Out0(0);
   bh47_w7_10 <= Compressor_6_3_F250_uid76_bh47_uid107_Out0(1);
   bh47_w8_6 <= Compressor_6_3_F250_uid76_bh47_uid107_Out0(2);
   Compressor_6_3_F250_uid76_uid107: Compressor_6_3_F250_uid76
      port map ( X0 => Compressor_6_3_F250_uid76_bh47_uid107_In0,
                 R => Compressor_6_3_F250_uid76_bh47_uid107_Out0_copy108);
   Compressor_6_3_F250_uid76_bh47_uid107_Out0 <= Compressor_6_3_F250_uid76_bh47_uid107_Out0_copy108; -- output copy to hold a pipeline register if needed


   Compressor_14_3_F250_uid80_bh47_uid109_In0 <= "" & bh47_w6_6 & bh47_w6_7 & bh47_w6_8 & bh47_w6_9;
   Compressor_14_3_F250_uid80_bh47_uid109_In1 <= "" & bh47_w7_0;
   bh47_w6_15 <= Compressor_14_3_F250_uid80_bh47_uid109_Out0(0);
   bh47_w7_11 <= Compressor_14_3_F250_uid80_bh47_uid109_Out0(1);
   bh47_w8_7 <= Compressor_14_3_F250_uid80_bh47_uid109_Out0(2);
   Compressor_14_3_F250_uid80_uid109: Compressor_14_3_F250_uid80
      port map ( X0 => Compressor_14_3_F250_uid80_bh47_uid109_In0,
                 X1 => Compressor_14_3_F250_uid80_bh47_uid109_In1,
                 R => Compressor_14_3_F250_uid80_bh47_uid109_Out0_copy110);
   Compressor_14_3_F250_uid80_bh47_uid109_Out0 <= Compressor_14_3_F250_uid80_bh47_uid109_Out0_copy110; -- output copy to hold a pipeline register if needed


   Compressor_6_3_F250_uid76_bh47_uid111_In0 <= "" & bh47_w7_1 & bh47_w7_2 & bh47_w7_3 & bh47_w7_4 & bh47_w7_5 & bh47_w7_6;
   bh47_w7_12 <= Compressor_6_3_F250_uid76_bh47_uid111_Out0(0);
   bh47_w8_8 <= Compressor_6_3_F250_uid76_bh47_uid111_Out0(1);
   bh47_w9_4 <= Compressor_6_3_F250_uid76_bh47_uid111_Out0(2);
   Compressor_6_3_F250_uid76_uid111: Compressor_6_3_F250_uid76
      port map ( X0 => Compressor_6_3_F250_uid76_bh47_uid111_In0,
                 R => Compressor_6_3_F250_uid76_bh47_uid111_Out0_copy112);
   Compressor_6_3_F250_uid76_bh47_uid111_Out0 <= Compressor_6_3_F250_uid76_bh47_uid111_Out0_copy112; -- output copy to hold a pipeline register if needed


   Compressor_6_3_F250_uid76_bh47_uid113_In0 <= "" & bh47_w8_0 & bh47_w8_1 & bh47_w8_2 & bh47_w8_3 & bh47_w8_4 & bh47_w8_5;
   bh47_w8_9 <= Compressor_6_3_F250_uid76_bh47_uid113_Out0(0);
   bh47_w9_5 <= Compressor_6_3_F250_uid76_bh47_uid113_Out0(1);
   bh47_w10_2 <= Compressor_6_3_F250_uid76_bh47_uid113_Out0(2);
   Compressor_6_3_F250_uid76_uid113: Compressor_6_3_F250_uid76
      port map ( X0 => Compressor_6_3_F250_uid76_bh47_uid113_In0,
                 R => Compressor_6_3_F250_uid76_bh47_uid113_Out0_copy114);
   Compressor_6_3_F250_uid76_bh47_uid113_Out0 <= Compressor_6_3_F250_uid76_bh47_uid113_Out0_copy114; -- output copy to hold a pipeline register if needed


   Compressor_14_3_F250_uid80_bh47_uid115_In0 <= "" & bh47_w9_0 & bh47_w9_1 & bh47_w9_2 & bh47_w9_3;
   Compressor_14_3_F250_uid80_bh47_uid115_In1 <= "" & bh47_w10_0;
   bh47_w9_6 <= Compressor_14_3_F250_uid80_bh47_uid115_Out0(0);
   bh47_w10_3 <= Compressor_14_3_F250_uid80_bh47_uid115_Out0(1);
   bh47_w11_1 <= Compressor_14_3_F250_uid80_bh47_uid115_Out0(2);
   Compressor_14_3_F250_uid80_uid115: Compressor_14_3_F250_uid80
      port map ( X0 => Compressor_14_3_F250_uid80_bh47_uid115_In0,
                 X1 => Compressor_14_3_F250_uid80_bh47_uid115_In1,
                 R => Compressor_14_3_F250_uid80_bh47_uid115_Out0_copy116);
   Compressor_14_3_F250_uid80_bh47_uid115_Out0 <= Compressor_14_3_F250_uid80_bh47_uid115_Out0_copy116; -- output copy to hold a pipeline register if needed


   Compressor_3_2_F250_uid118_bh47_uid119_In0 <= "" & bh47_w0_12 & bh47_w0_11 & bh47_w0_9;
   bh47_w0_13 <= Compressor_3_2_F250_uid118_bh47_uid119_Out0(0);
   bh47_w1_18 <= Compressor_3_2_F250_uid118_bh47_uid119_Out0(1);
   Compressor_3_2_F250_uid118_uid119: Compressor_3_2_F250_uid118
      port map ( X0 => Compressor_3_2_F250_uid118_bh47_uid119_In0,
                 R => Compressor_3_2_F250_uid118_bh47_uid119_Out0_copy120);
   Compressor_3_2_F250_uid118_bh47_uid119_Out0 <= Compressor_3_2_F250_uid118_bh47_uid119_Out0_copy120; -- output copy to hold a pipeline register if needed


   Compressor_14_3_F250_uid80_bh47_uid121_In0 <= "" & bh47_w1_15 & bh47_w1_16 & bh47_w1_14 & bh47_w1_6;
   Compressor_14_3_F250_uid80_bh47_uid121_In1 <= "" & bh47_w2_15;
   bh47_w1_19 <= Compressor_14_3_F250_uid80_bh47_uid121_Out0(0);
   bh47_w2_21 <= Compressor_14_3_F250_uid80_bh47_uid121_Out0(1);
   bh47_w3_21 <= Compressor_14_3_F250_uid80_bh47_uid121_Out0(2);
   Compressor_14_3_F250_uid80_uid121: Compressor_14_3_F250_uid80
      port map ( X0 => Compressor_14_3_F250_uid80_bh47_uid121_In0,
                 X1 => Compressor_14_3_F250_uid80_bh47_uid121_In1,
                 R => Compressor_14_3_F250_uid80_bh47_uid121_Out0_copy122);
   Compressor_14_3_F250_uid80_bh47_uid121_Out0 <= Compressor_14_3_F250_uid80_bh47_uid121_Out0_copy122; -- output copy to hold a pipeline register if needed


   Compressor_6_3_F250_uid76_bh47_uid123_In0 <= "" & bh47_w2_20 & bh47_w2_18 & bh47_w2_16 & bh47_w2_14 & bh47_w2_17 & bh47_w2_19;
   bh47_w2_22 <= Compressor_6_3_F250_uid76_bh47_uid123_Out0(0);
   bh47_w3_22 <= Compressor_6_3_F250_uid76_bh47_uid123_Out0(1);
   bh47_w4_20 <= Compressor_6_3_F250_uid76_bh47_uid123_Out0(2);
   Compressor_6_3_F250_uid76_uid123: Compressor_6_3_F250_uid76
      port map ( X0 => Compressor_6_3_F250_uid76_bh47_uid123_In0,
                 R => Compressor_6_3_F250_uid76_bh47_uid123_Out0_copy124);
   Compressor_6_3_F250_uid76_bh47_uid123_Out0 <= Compressor_6_3_F250_uid76_bh47_uid123_Out0_copy124; -- output copy to hold a pipeline register if needed


   Compressor_6_3_F250_uid76_bh47_uid125_In0 <= "" & bh47_w3_20 & bh47_w3_18 & bh47_w3_16 & bh47_w3_17 & "0" & "0";
   bh47_w3_23 <= Compressor_6_3_F250_uid76_bh47_uid125_Out0(0);
   bh47_w4_21 <= Compressor_6_3_F250_uid76_bh47_uid125_Out0(1);
   bh47_w5_18 <= Compressor_6_3_F250_uid76_bh47_uid125_Out0(2);
   Compressor_6_3_F250_uid76_uid125: Compressor_6_3_F250_uid76
      port map ( X0 => Compressor_6_3_F250_uid76_bh47_uid125_In0,
                 R => Compressor_6_3_F250_uid76_bh47_uid125_Out0_copy126);
   Compressor_6_3_F250_uid76_bh47_uid125_Out0 <= Compressor_6_3_F250_uid76_bh47_uid125_Out0_copy126; -- output copy to hold a pipeline register if needed


   Compressor_23_3_F250_uid92_bh47_uid127_In0 <= "" & bh47_w3_19 & bh47_w3_14 & bh47_w3_15;
   Compressor_23_3_F250_uid92_bh47_uid127_In1 <= "" & bh47_w4_19 & bh47_w4_16;
   bh47_w3_24 <= Compressor_23_3_F250_uid92_bh47_uid127_Out0(0);
   bh47_w4_22 <= Compressor_23_3_F250_uid92_bh47_uid127_Out0(1);
   bh47_w5_19 <= Compressor_23_3_F250_uid92_bh47_uid127_Out0(2);
   Compressor_23_3_F250_uid92_uid127: Compressor_23_3_F250_uid92
      port map ( X0 => Compressor_23_3_F250_uid92_bh47_uid127_In0,
                 X1 => Compressor_23_3_F250_uid92_bh47_uid127_In1,
                 R => Compressor_23_3_F250_uid92_bh47_uid127_Out0_copy128);
   Compressor_23_3_F250_uid92_bh47_uid127_Out0 <= Compressor_23_3_F250_uid92_bh47_uid127_Out0_copy128; -- output copy to hold a pipeline register if needed


   Compressor_6_3_F250_uid76_bh47_uid129_In0 <= "" & bh47_w4_7 & bh47_w4_17 & bh47_w4_18 & bh47_w4_13 & bh47_w4_14 & bh47_w4_15;
   bh47_w4_23 <= Compressor_6_3_F250_uid76_bh47_uid129_Out0(0);
   bh47_w5_20 <= Compressor_6_3_F250_uid76_bh47_uid129_Out0(1);
   bh47_w6_16 <= Compressor_6_3_F250_uid76_bh47_uid129_Out0(2);
   Compressor_6_3_F250_uid76_uid129: Compressor_6_3_F250_uid76
      port map ( X0 => Compressor_6_3_F250_uid76_bh47_uid129_In0,
                 R => Compressor_6_3_F250_uid76_bh47_uid129_Out0_copy130);
   Compressor_6_3_F250_uid76_bh47_uid129_Out0 <= Compressor_6_3_F250_uid76_bh47_uid129_Out0_copy130; -- output copy to hold a pipeline register if needed


   Compressor_6_3_F250_uid76_bh47_uid131_In0 <= "" & bh47_w5_17 & bh47_w5_16 & bh47_w5_12 & bh47_w5_13 & bh47_w5_14 & bh47_w5_15;
   bh47_w5_21 <= Compressor_6_3_F250_uid76_bh47_uid131_Out0(0);
   bh47_w6_17 <= Compressor_6_3_F250_uid76_bh47_uid131_Out0(1);
   bh47_w7_13 <= Compressor_6_3_F250_uid76_bh47_uid131_Out0(2);
   Compressor_6_3_F250_uid76_uid131: Compressor_6_3_F250_uid76
      port map ( X0 => Compressor_6_3_F250_uid76_bh47_uid131_In0,
                 R => Compressor_6_3_F250_uid76_bh47_uid131_Out0_copy132);
   Compressor_6_3_F250_uid76_bh47_uid131_Out0 <= Compressor_6_3_F250_uid76_bh47_uid131_Out0_copy132; -- output copy to hold a pipeline register if needed


   Compressor_6_3_F250_uid76_bh47_uid133_In0 <= "" & bh47_w6_15 & bh47_w6_14 & bh47_w6_13 & bh47_w6_12 & bh47_w6_11 & bh47_w6_10;
   bh47_w6_18 <= Compressor_6_3_F250_uid76_bh47_uid133_Out0(0);
   bh47_w7_14 <= Compressor_6_3_F250_uid76_bh47_uid133_Out0(1);
   bh47_w8_10 <= Compressor_6_3_F250_uid76_bh47_uid133_Out0(2);
   Compressor_6_3_F250_uid76_uid133: Compressor_6_3_F250_uid76
      port map ( X0 => Compressor_6_3_F250_uid76_bh47_uid133_In0,
                 R => Compressor_6_3_F250_uid76_bh47_uid133_Out0_copy134);
   Compressor_6_3_F250_uid76_bh47_uid133_Out0 <= Compressor_6_3_F250_uid76_bh47_uid133_Out0_copy134; -- output copy to hold a pipeline register if needed


   Compressor_6_3_F250_uid76_bh47_uid135_In0 <= "" & bh47_w7_12 & bh47_w7_11 & bh47_w7_10 & bh47_w7_9 & bh47_w7_8 & bh47_w7_7;
   bh47_w7_15 <= Compressor_6_3_F250_uid76_bh47_uid135_Out0(0);
   bh47_w8_11 <= Compressor_6_3_F250_uid76_bh47_uid135_Out0(1);
   bh47_w9_7 <= Compressor_6_3_F250_uid76_bh47_uid135_Out0(2);
   Compressor_6_3_F250_uid76_uid135: Compressor_6_3_F250_uid76
      port map ( X0 => Compressor_6_3_F250_uid76_bh47_uid135_In0,
                 R => Compressor_6_3_F250_uid76_bh47_uid135_Out0_copy136);
   Compressor_6_3_F250_uid76_bh47_uid135_Out0 <= Compressor_6_3_F250_uid76_bh47_uid135_Out0_copy136; -- output copy to hold a pipeline register if needed


   Compressor_14_3_F250_uid80_bh47_uid137_In0 <= "" & bh47_w8_9 & bh47_w8_8 & bh47_w8_7 & bh47_w8_6;
   Compressor_14_3_F250_uid80_bh47_uid137_In1 <= "" & "0";
   bh47_w8_12 <= Compressor_14_3_F250_uid80_bh47_uid137_Out0(0);
   bh47_w9_8 <= Compressor_14_3_F250_uid80_bh47_uid137_Out0(1);
   bh47_w10_4 <= Compressor_14_3_F250_uid80_bh47_uid137_Out0(2);
   Compressor_14_3_F250_uid80_uid137: Compressor_14_3_F250_uid80
      port map ( X0 => Compressor_14_3_F250_uid80_bh47_uid137_In0,
                 X1 => Compressor_14_3_F250_uid80_bh47_uid137_In1_d4,
                 R => Compressor_14_3_F250_uid80_bh47_uid137_Out0_copy138);
   Compressor_14_3_F250_uid80_bh47_uid137_Out0 <= Compressor_14_3_F250_uid80_bh47_uid137_Out0_copy138; -- output copy to hold a pipeline register if needed


   Compressor_3_2_F250_uid118_bh47_uid139_In0 <= "" & bh47_w9_6 & bh47_w9_5 & bh47_w9_4;
   bh47_w9_9 <= Compressor_3_2_F250_uid118_bh47_uid139_Out0(0);
   bh47_w10_5 <= Compressor_3_2_F250_uid118_bh47_uid139_Out0(1);
   Compressor_3_2_F250_uid118_uid139: Compressor_3_2_F250_uid118
      port map ( X0 => Compressor_3_2_F250_uid118_bh47_uid139_In0,
                 R => Compressor_3_2_F250_uid118_bh47_uid139_Out0_copy140);
   Compressor_3_2_F250_uid118_bh47_uid139_Out0 <= Compressor_3_2_F250_uid118_bh47_uid139_Out0_copy140; -- output copy to hold a pipeline register if needed


   Compressor_23_3_F250_uid92_bh47_uid141_In0 <= "" & bh47_w10_3 & bh47_w10_2 & bh47_w10_1;
   Compressor_23_3_F250_uid92_bh47_uid141_In1 <= "" & bh47_w11_1 & bh47_w11_0;
   bh47_w10_6 <= Compressor_23_3_F250_uid92_bh47_uid141_Out0(0);
   bh47_w11_2 <= Compressor_23_3_F250_uid92_bh47_uid141_Out0(1);
   bh47_w12_1 <= Compressor_23_3_F250_uid92_bh47_uid141_Out0(2);
   Compressor_23_3_F250_uid92_uid141: Compressor_23_3_F250_uid92
      port map ( X0 => Compressor_23_3_F250_uid92_bh47_uid141_In0,
                 X1 => Compressor_23_3_F250_uid92_bh47_uid141_In1,
                 R => Compressor_23_3_F250_uid92_bh47_uid141_Out0_copy142);
   Compressor_23_3_F250_uid92_bh47_uid141_Out0 <= Compressor_23_3_F250_uid92_bh47_uid141_Out0_copy142; -- output copy to hold a pipeline register if needed


   Compressor_23_3_F250_uid92_bh47_uid143_In0 <= "" & bh47_w1_18 & bh47_w1_19 & bh47_w1_17;
   Compressor_23_3_F250_uid92_bh47_uid143_In1 <= "" & bh47_w2_21 & bh47_w2_22;
   bh47_w1_20 <= Compressor_23_3_F250_uid92_bh47_uid143_Out0(0);
   bh47_w2_23 <= Compressor_23_3_F250_uid92_bh47_uid143_Out0(1);
   bh47_w3_25 <= Compressor_23_3_F250_uid92_bh47_uid143_Out0(2);
   Compressor_23_3_F250_uid92_uid143: Compressor_23_3_F250_uid92
      port map ( X0 => Compressor_23_3_F250_uid92_bh47_uid143_In0,
                 X1 => Compressor_23_3_F250_uid92_bh47_uid143_In1,
                 R => Compressor_23_3_F250_uid92_bh47_uid143_Out0_copy144);
   Compressor_23_3_F250_uid92_bh47_uid143_Out0 <= Compressor_23_3_F250_uid92_bh47_uid143_Out0_copy144; -- output copy to hold a pipeline register if needed


   Compressor_14_3_F250_uid80_bh47_uid145_In0 <= "" & bh47_w3_21 & bh47_w3_24 & bh47_w3_23 & bh47_w3_22;
   Compressor_14_3_F250_uid80_bh47_uid145_In1 <= "" & bh47_w4_23;
   bh47_w3_26 <= Compressor_14_3_F250_uid80_bh47_uid145_Out0(0);
   bh47_w4_24 <= Compressor_14_3_F250_uid80_bh47_uid145_Out0(1);
   bh47_w5_22 <= Compressor_14_3_F250_uid80_bh47_uid145_Out0(2);
   Compressor_14_3_F250_uid80_uid145: Compressor_14_3_F250_uid80
      port map ( X0 => Compressor_14_3_F250_uid80_bh47_uid145_In0,
                 X1 => Compressor_14_3_F250_uid80_bh47_uid145_In1,
                 R => Compressor_14_3_F250_uid80_bh47_uid145_Out0_copy146);
   Compressor_14_3_F250_uid80_bh47_uid145_Out0 <= Compressor_14_3_F250_uid80_bh47_uid145_Out0_copy146; -- output copy to hold a pipeline register if needed


   Compressor_3_2_F250_uid118_bh47_uid147_In0 <= "" & bh47_w4_22 & bh47_w4_21 & bh47_w4_20;
   bh47_w4_25 <= Compressor_3_2_F250_uid118_bh47_uid147_Out0(0);
   bh47_w5_23 <= Compressor_3_2_F250_uid118_bh47_uid147_Out0(1);
   Compressor_3_2_F250_uid118_uid147: Compressor_3_2_F250_uid118
      port map ( X0 => Compressor_3_2_F250_uid118_bh47_uid147_In0,
                 R => Compressor_3_2_F250_uid118_bh47_uid147_Out0_copy148);
   Compressor_3_2_F250_uid118_bh47_uid147_Out0 <= Compressor_3_2_F250_uid118_bh47_uid147_Out0_copy148; -- output copy to hold a pipeline register if needed


   Compressor_14_3_F250_uid80_bh47_uid149_In0 <= "" & bh47_w5_21 & bh47_w5_20 & bh47_w5_19 & bh47_w5_18;
   Compressor_14_3_F250_uid80_bh47_uid149_In1 <= "" & "0";
   bh47_w5_24 <= Compressor_14_3_F250_uid80_bh47_uid149_Out0(0);
   bh47_w6_19 <= Compressor_14_3_F250_uid80_bh47_uid149_Out0(1);
   bh47_w7_16 <= Compressor_14_3_F250_uid80_bh47_uid149_Out0(2);
   Compressor_14_3_F250_uid80_uid149: Compressor_14_3_F250_uid80
      port map ( X0 => Compressor_14_3_F250_uid80_bh47_uid149_In0,
                 X1 => Compressor_14_3_F250_uid80_bh47_uid149_In1_d4,
                 R => Compressor_14_3_F250_uid80_bh47_uid149_Out0_copy150);
   Compressor_14_3_F250_uid80_bh47_uid149_Out0 <= Compressor_14_3_F250_uid80_bh47_uid149_Out0_copy150; -- output copy to hold a pipeline register if needed


   Compressor_3_2_F250_uid118_bh47_uid151_In0 <= "" & bh47_w6_18 & bh47_w6_17 & bh47_w6_16;
   bh47_w6_20 <= Compressor_3_2_F250_uid118_bh47_uid151_Out0(0);
   bh47_w7_17 <= Compressor_3_2_F250_uid118_bh47_uid151_Out0(1);
   Compressor_3_2_F250_uid118_uid151: Compressor_3_2_F250_uid118
      port map ( X0 => Compressor_3_2_F250_uid118_bh47_uid151_In0,
                 R => Compressor_3_2_F250_uid118_bh47_uid151_Out0_copy152);
   Compressor_3_2_F250_uid118_bh47_uid151_Out0 <= Compressor_3_2_F250_uid118_bh47_uid151_Out0_copy152; -- output copy to hold a pipeline register if needed


   Compressor_23_3_F250_uid92_bh47_uid153_In0 <= "" & bh47_w7_15 & bh47_w7_14 & bh47_w7_13;
   Compressor_23_3_F250_uid92_bh47_uid153_In1 <= "" & bh47_w8_12 & bh47_w8_11;
   bh47_w7_18 <= Compressor_23_3_F250_uid92_bh47_uid153_Out0(0);
   bh47_w8_13 <= Compressor_23_3_F250_uid92_bh47_uid153_Out0(1);
   bh47_w9_10 <= Compressor_23_3_F250_uid92_bh47_uid153_Out0(2);
   Compressor_23_3_F250_uid92_uid153: Compressor_23_3_F250_uid92
      port map ( X0 => Compressor_23_3_F250_uid92_bh47_uid153_In0,
                 X1 => Compressor_23_3_F250_uid92_bh47_uid153_In1,
                 R => Compressor_23_3_F250_uid92_bh47_uid153_Out0_copy154);
   Compressor_23_3_F250_uid92_bh47_uid153_Out0 <= Compressor_23_3_F250_uid92_bh47_uid153_Out0_copy154; -- output copy to hold a pipeline register if needed


   Compressor_23_3_F250_uid92_bh47_uid155_In0 <= "" & bh47_w9_8 & bh47_w9_9 & bh47_w9_7;
   Compressor_23_3_F250_uid92_bh47_uid155_In1 <= "" & bh47_w10_4 & bh47_w10_6;
   bh47_w9_11 <= Compressor_23_3_F250_uid92_bh47_uid155_Out0(0);
   bh47_w10_7 <= Compressor_23_3_F250_uid92_bh47_uid155_Out0(1);
   bh47_w11_3 <= Compressor_23_3_F250_uid92_bh47_uid155_Out0(2);
   Compressor_23_3_F250_uid92_uid155: Compressor_23_3_F250_uid92
      port map ( X0 => Compressor_23_3_F250_uid92_bh47_uid155_In0,
                 X1 => Compressor_23_3_F250_uid92_bh47_uid155_In1,
                 R => Compressor_23_3_F250_uid92_bh47_uid155_Out0_copy156);
   Compressor_23_3_F250_uid92_bh47_uid155_Out0 <= Compressor_23_3_F250_uid92_bh47_uid155_Out0_copy156; -- output copy to hold a pipeline register if needed


   Compressor_14_3_F250_uid80_bh47_uid157_In0 <= "" & bh47_w12_1 & bh47_w12_0 & "0" & "0";
   Compressor_14_3_F250_uid80_bh47_uid157_In1 <= "" & bh47_w13_0;
   bh47_w12_2 <= Compressor_14_3_F250_uid80_bh47_uid157_Out0(0);
   bh47_w13_1 <= Compressor_14_3_F250_uid80_bh47_uid157_Out0(1);
   bh47_w14_1 <= Compressor_14_3_F250_uid80_bh47_uid157_Out0(2);
   Compressor_14_3_F250_uid80_uid157: Compressor_14_3_F250_uid80
      port map ( X0 => Compressor_14_3_F250_uid80_bh47_uid157_In0,
                 X1 => Compressor_14_3_F250_uid80_bh47_uid157_In1,
                 R => Compressor_14_3_F250_uid80_bh47_uid157_Out0_copy158);
   Compressor_14_3_F250_uid80_bh47_uid157_Out0 <= Compressor_14_3_F250_uid80_bh47_uid157_Out0_copy158; -- output copy to hold a pipeline register if needed


   Compressor_23_3_F250_uid92_bh47_uid159_In0 <= "" & bh47_w3_25 & bh47_w3_26 & "0";
   Compressor_23_3_F250_uid92_bh47_uid159_In1 <= "" & bh47_w4_24 & bh47_w4_25;
   bh47_w3_27 <= Compressor_23_3_F250_uid92_bh47_uid159_Out0(0);
   bh47_w4_26 <= Compressor_23_3_F250_uid92_bh47_uid159_Out0(1);
   bh47_w5_25 <= Compressor_23_3_F250_uid92_bh47_uid159_Out0(2);
   Compressor_23_3_F250_uid92_uid159: Compressor_23_3_F250_uid92
      port map ( X0 => Compressor_23_3_F250_uid92_bh47_uid159_In0,
                 X1 => Compressor_23_3_F250_uid92_bh47_uid159_In1,
                 R => Compressor_23_3_F250_uid92_bh47_uid159_Out0_copy160);
   Compressor_23_3_F250_uid92_bh47_uid159_Out0 <= Compressor_23_3_F250_uid92_bh47_uid159_Out0_copy160; -- output copy to hold a pipeline register if needed


   Compressor_23_3_F250_uid92_bh47_uid161_In0 <= "" & bh47_w5_22 & bh47_w5_24 & bh47_w5_23;
   Compressor_23_3_F250_uid92_bh47_uid161_In1 <= "" & bh47_w6_19 & bh47_w6_20;
   bh47_w5_26 <= Compressor_23_3_F250_uid92_bh47_uid161_Out0(0);
   bh47_w6_21 <= Compressor_23_3_F250_uid92_bh47_uid161_Out0(1);
   bh47_w7_19 <= Compressor_23_3_F250_uid92_bh47_uid161_Out0(2);
   Compressor_23_3_F250_uid92_uid161: Compressor_23_3_F250_uid92
      port map ( X0 => Compressor_23_3_F250_uid92_bh47_uid161_In0,
                 X1 => Compressor_23_3_F250_uid92_bh47_uid161_In1,
                 R => Compressor_23_3_F250_uid92_bh47_uid161_Out0_copy162);
   Compressor_23_3_F250_uid92_bh47_uid161_Out0 <= Compressor_23_3_F250_uid92_bh47_uid161_Out0_copy162; -- output copy to hold a pipeline register if needed


   Compressor_23_3_F250_uid92_bh47_uid163_In0 <= "" & bh47_w7_16 & bh47_w7_18 & bh47_w7_17;
   Compressor_23_3_F250_uid92_bh47_uid163_In1 <= "" & bh47_w8_13 & bh47_w8_10;
   bh47_w7_20 <= Compressor_23_3_F250_uid92_bh47_uid163_Out0(0);
   bh47_w8_14 <= Compressor_23_3_F250_uid92_bh47_uid163_Out0(1);
   bh47_w9_12 <= Compressor_23_3_F250_uid92_bh47_uid163_Out0(2);
   Compressor_23_3_F250_uid92_uid163: Compressor_23_3_F250_uid92
      port map ( X0 => Compressor_23_3_F250_uid92_bh47_uid163_In0,
                 X1 => Compressor_23_3_F250_uid92_bh47_uid163_In1,
                 R => Compressor_23_3_F250_uid92_bh47_uid163_Out0_copy164);
   Compressor_23_3_F250_uid92_bh47_uid163_Out0 <= Compressor_23_3_F250_uid92_bh47_uid163_Out0_copy164; -- output copy to hold a pipeline register if needed


   Compressor_23_3_F250_uid92_bh47_uid165_In0 <= "" & bh47_w9_10 & bh47_w9_11 & "0";
   Compressor_23_3_F250_uid92_bh47_uid165_In1 <= "" & bh47_w10_7 & bh47_w10_5;
   bh47_w9_13 <= Compressor_23_3_F250_uid92_bh47_uid165_Out0(0);
   bh47_w10_8 <= Compressor_23_3_F250_uid92_bh47_uid165_Out0(1);
   bh47_w11_4 <= Compressor_23_3_F250_uid92_bh47_uid165_Out0(2);
   Compressor_23_3_F250_uid92_uid165: Compressor_23_3_F250_uid92
      port map ( X0 => Compressor_23_3_F250_uid92_bh47_uid165_In0,
                 X1 => Compressor_23_3_F250_uid92_bh47_uid165_In1,
                 R => Compressor_23_3_F250_uid92_bh47_uid165_Out0_copy166);
   Compressor_23_3_F250_uid92_bh47_uid165_Out0 <= Compressor_23_3_F250_uid92_bh47_uid165_Out0_copy166; -- output copy to hold a pipeline register if needed


   Compressor_14_3_F250_uid80_bh47_uid167_In0 <= "" & bh47_w11_3 & bh47_w11_2 & "0" & "0";
   Compressor_14_3_F250_uid80_bh47_uid167_In1 <= "" & bh47_w12_2;
   bh47_w11_5 <= Compressor_14_3_F250_uid80_bh47_uid167_Out0(0);
   bh47_w12_3 <= Compressor_14_3_F250_uid80_bh47_uid167_Out0(1);
   bh47_w13_2 <= Compressor_14_3_F250_uid80_bh47_uid167_Out0(2);
   Compressor_14_3_F250_uid80_uid167: Compressor_14_3_F250_uid80
      port map ( X0 => Compressor_14_3_F250_uid80_bh47_uid167_In0,
                 X1 => Compressor_14_3_F250_uid80_bh47_uid167_In1,
                 R => Compressor_14_3_F250_uid80_bh47_uid167_Out0_copy168);
   Compressor_14_3_F250_uid80_bh47_uid167_Out0 <= Compressor_14_3_F250_uid80_bh47_uid167_Out0_copy168; -- output copy to hold a pipeline register if needed


   Compressor_14_3_F250_uid80_bh47_uid169_In0 <= "" & bh47_w14_1 & bh47_w14_0 & "0" & "0";
   Compressor_14_3_F250_uid80_bh47_uid169_In1 <= "" & bh47_w15_0;
   bh47_w14_2 <= Compressor_14_3_F250_uid80_bh47_uid169_Out0(0);
   bh47_w15_1 <= Compressor_14_3_F250_uid80_bh47_uid169_Out0(1);
   bh47_w16_1 <= Compressor_14_3_F250_uid80_bh47_uid169_Out0(2);
   Compressor_14_3_F250_uid80_uid169: Compressor_14_3_F250_uid80
      port map ( X0 => Compressor_14_3_F250_uid80_bh47_uid169_In0,
                 X1 => Compressor_14_3_F250_uid80_bh47_uid169_In1,
                 R => Compressor_14_3_F250_uid80_bh47_uid169_Out0_copy170);
   Compressor_14_3_F250_uid80_bh47_uid169_Out0 <= Compressor_14_3_F250_uid80_bh47_uid169_Out0_copy170; -- output copy to hold a pipeline register if needed

   tmp_bitheapResult_bh47_4 <= bh47_w4_26 & bh47_w3_27 & bh47_w2_23 & bh47_w1_20 & bh47_w0_13;

   bitheapFinalAdd_bh47_In0 <= "0" & "0" & "0" & "0" & "0" & "0" & "0" & "0" & "0" & "0" & "0" & "0" & "0" & bh47_w22_0 & bh47_w21_0 & bh47_w20_0 & bh47_w19_0 & bh47_w18_0 & bh47_w17_0 & bh47_w16_1 & bh47_w15_1 & bh47_w14_2 & bh47_w13_2 & bh47_w12_3 & bh47_w11_4 & bh47_w10_8 & bh47_w9_12 & bh47_w8_14 & bh47_w7_19 & bh47_w6_21 & bh47_w5_25;
   bitheapFinalAdd_bh47_In1 <= "0" & "0" & "0" & "0" & "0" & "0" & "0" & "0" & "0" & "0" & "0" & "0" & "0" & "0" & "0" & "0" & "0" & "0" & "0" & bh47_w16_0 & "0" & "0" & bh47_w13_1 & "0" & bh47_w11_5 & "0" & bh47_w9_13 & "0" & bh47_w7_20 & "0" & bh47_w5_26;
   bitheapFinalAdd_bh47_Cin <= '0';

   bitheapFinalAdd_bh47: IntAdder_31_F250_uid172
      port map ( clk  => clk,
                 Cin => bitheapFinalAdd_bh47_Cin,
                 X => bitheapFinalAdd_bh47_In0,
                 Y => bitheapFinalAdd_bh47_In1,
                 R => bitheapFinalAdd_bh47_Out);
   bitheapResult_bh47 <= bitheapFinalAdd_bh47_Out(29 downto 0) & tmp_bitheapResult_bh47_4_d1;
   R <= bitheapResult_bh47(22 downto 4);
end architecture;

--------------------------------------------------------------------------------
--                          IntAdder_28_F250_uid175
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Bogdan Pasca, Florent de Dinechin (2008-2016)
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X Y Cin
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity IntAdder_28_F250_uid175 is
    port (clk : in std_logic;
          X : in  std_logic_vector(27 downto 0);
          Y : in  std_logic_vector(27 downto 0);
          Cin : in  std_logic;
          R : out  std_logic_vector(27 downto 0)   );
end entity;

architecture arch of IntAdder_28_F250_uid175 is
signal Rtmp :  std_logic_vector(27 downto 0);
signal X_d1, X_d2 :  std_logic_vector(27 downto 0);
signal Cin_d1, Cin_d2, Cin_d3, Cin_d4, Cin_d5 :  std_logic;
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            X_d1 <=  X;
            X_d2 <=  X_d1;
            Cin_d1 <=  Cin;
            Cin_d2 <=  Cin_d1;
            Cin_d3 <=  Cin_d2;
            Cin_d4 <=  Cin_d3;
            Cin_d5 <=  Cin_d4;
         end if;
      end process;
   Rtmp <= X_d2 + Y + Cin_d5;
   R <= Rtmp;
end architecture;

--------------------------------------------------------------------------------
--                          IntAdder_33_F250_uid178
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Bogdan Pasca, Florent de Dinechin (2008-2016)
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X Y Cin
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity IntAdder_33_F250_uid178 is
    port (clk : in std_logic;
          X : in  std_logic_vector(32 downto 0);
          Y : in  std_logic_vector(32 downto 0);
          Cin : in  std_logic;
          R : out  std_logic_vector(32 downto 0)   );
end entity;

architecture arch of IntAdder_33_F250_uid178 is
signal Rtmp :  std_logic_vector(32 downto 0);
signal Y_d1 :  std_logic_vector(32 downto 0);
signal Cin_d1, Cin_d2, Cin_d3, Cin_d4, Cin_d5, Cin_d6 :  std_logic;
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            Y_d1 <=  Y;
            Cin_d1 <=  Cin;
            Cin_d2 <=  Cin_d1;
            Cin_d3 <=  Cin_d2;
            Cin_d4 <=  Cin_d3;
            Cin_d5 <=  Cin_d4;
            Cin_d6 <=  Cin_d5;
         end if;
      end process;
   Rtmp <= X + Y_d1 + Cin_d6;
   R <= Rtmp;
end architecture;

--------------------------------------------------------------------------------
--                            fp_exp_flopoco_f250
--                           (FPExp_8_23_F250_uid2)
-- VHDL generated for Zynq7000 @ 250MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: F. de Dinechin, Bogdan Pasca (2008-2021)
--------------------------------------------------------------------------------
-- Pipeline depth: 6 cycles
-- Clock period (ns): 4
-- Target frequency (MHz): 250
-- Input signals: X
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity fp_exp_flopoco_f250 is
    port (clk : in std_logic;
          X : in  std_logic_vector(8+23+2 downto 0);
          R : out  std_logic_vector(8+23+2 downto 0)   );
end entity;

architecture arch of fp_exp_flopoco_f250 is
   component LeftShifter24_by_max_33_F250_uid4 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(23 downto 0);
             S : in  std_logic_vector(5 downto 0);
             R : out  std_logic_vector(56 downto 0)   );
   end component;

   component FixRealKCM_F250_uid6 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(9 downto 0);
             R : out  std_logic_vector(7 downto 0)   );
   end component;

   component FixRealKCM_F250_uid18 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(7 downto 0);
             R : out  std_logic_vector(34 downto 0)   );
   end component;

   component IntAdder_27_F250_uid31 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(26 downto 0);
             Y : in  std_logic_vector(26 downto 0);
             Cin : in  std_logic;
             R : out  std_logic_vector(26 downto 0)   );
   end component;

   component ExpATable_F250_uid33 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(9 downto 0);
             Y : out  std_logic_vector(27 downto 0)   );
   end component;

   component ExpZmZm1Table_F250_uid35 is
      port ( X : in  std_logic_vector(6 downto 0);
             Y : out  std_logic_vector(5 downto 0)   );
   end component;

   component IntAdder_18_F250_uid39 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(17 downto 0);
             Y : in  std_logic_vector(17 downto 0);
             Cin : in  std_logic;
             R : out  std_logic_vector(17 downto 0)   );
   end component;

   component IntAdder_18_F250_uid43 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(17 downto 0);
             Y : in  std_logic_vector(17 downto 0);
             Cin : in  std_logic;
             R : out  std_logic_vector(17 downto 0)   );
   end component;

   component IntMultiplier_F250_uid45 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(16 downto 0);
             Y : in  std_logic_vector(17 downto 0);
             R : out  std_logic_vector(18 downto 0)   );
   end component;

   component IntAdder_28_F250_uid175 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(27 downto 0);
             Y : in  std_logic_vector(27 downto 0);
             Cin : in  std_logic;
             R : out  std_logic_vector(27 downto 0)   );
   end component;

   component IntAdder_33_F250_uid178 is
      port ( clk : in std_logic;
             X : in  std_logic_vector(32 downto 0);
             Y : in  std_logic_vector(32 downto 0);
             Cin : in  std_logic;
             R : out  std_logic_vector(32 downto 0)   );
   end component;

signal Xexn, Xexn_d1, Xexn_d2, Xexn_d3, Xexn_d4, Xexn_d5, Xexn_d6 :  std_logic_vector(1 downto 0);
signal XSign, XSign_d1, XSign_d2, XSign_d3, XSign_d4, XSign_d5, XSign_d6 :  std_logic;
signal XexpField :  std_logic_vector(7 downto 0);
signal Xfrac :  unsigned(-1+23 downto 0);
signal e0 :  std_logic_vector(9 downto 0);
signal shiftVal :  std_logic_vector(9 downto 0);
signal resultWillBeOne, resultWillBeOne_d1 :  std_logic;
signal mXu :  unsigned(0+23 downto 0);
signal maxShift :  std_logic_vector(8 downto 0);
signal overflow0 :  std_logic;
signal shiftValIn :  std_logic_vector(5 downto 0);
signal fixX0 :  std_logic_vector(56 downto 0);
signal ufixX :  unsigned(6+27 downto 0);
signal xMulIn :  unsigned(6+3 downto 0);
signal absK, absK_d1 :  std_logic_vector(7 downto 0);
signal minusAbsK :  std_logic_vector(8 downto 0);
signal K, K_d1, K_d2, K_d3 :  std_logic_vector(8 downto 0);
signal absKLog2 :  std_logic_vector(34 downto 0);
signal subOp1 :  std_logic_vector(26 downto 0);
signal subOp2 :  std_logic_vector(26 downto 0);
signal Y :  std_logic_vector(26 downto 0);
signal A :  std_logic_vector(9 downto 0);
signal Z :  std_logic_vector(16 downto 0);
signal expA :  std_logic_vector(27 downto 0);
signal Ztrunc :  std_logic_vector(6 downto 0);
signal expZmZm1 :  std_logic_vector(5 downto 0);
signal expZmZm1_copy36 :  std_logic_vector(5 downto 0);
signal expZm1adderX :  std_logic_vector(17 downto 0);
signal expZm1adderY :  std_logic_vector(17 downto 0);
signal expZm1 :  std_logic_vector(17 downto 0);
signal expA_T :  std_logic_vector(17 downto 0);
signal expArounded0 :  std_logic_vector(17 downto 0);
signal expArounded :  std_logic_vector(16 downto 0);
signal lowerProduct :  std_logic_vector(18 downto 0);
signal extendedLowerProduct :  std_logic_vector(27 downto 0);
signal expY, expY_d1 :  std_logic_vector(27 downto 0);
signal needNoNorm, needNoNorm_d1 :  std_logic;
signal preRoundBiasSig :  std_logic_vector(32 downto 0);
signal roundBit :  std_logic;
signal roundNormAddend :  std_logic_vector(32 downto 0);
signal roundedExpSigRes :  std_logic_vector(32 downto 0);
signal roundedExpSig :  std_logic_vector(32 downto 0);
signal ofl1, ofl1_d1, ofl1_d2, ofl1_d3, ofl1_d4, ofl1_d5, ofl1_d6 :  std_logic;
signal ofl2 :  std_logic;
signal ofl3, ofl3_d1, ofl3_d2, ofl3_d3, ofl3_d4, ofl3_d5, ofl3_d6 :  std_logic;
signal ofl :  std_logic;
signal ufl1 :  std_logic;
signal ufl2, ufl2_d1, ufl2_d2, ufl2_d3, ufl2_d4, ufl2_d5, ufl2_d6 :  std_logic;
signal ufl3, ufl3_d1, ufl3_d2, ufl3_d3, ufl3_d4, ufl3_d5, ufl3_d6 :  std_logic;
signal ufl :  std_logic;
signal Rexn :  std_logic_vector(1 downto 0);
constant g: positive := 4;
constant wE: positive := 8;
constant wF: positive := 23;
constant wFIn: positive := 23;
begin
   process(clk)
      begin
         if clk'event and clk = '1' then
            Xexn_d1 <=  Xexn;
            Xexn_d2 <=  Xexn_d1;
            Xexn_d3 <=  Xexn_d2;
            Xexn_d4 <=  Xexn_d3;
            Xexn_d5 <=  Xexn_d4;
            Xexn_d6 <=  Xexn_d5;
            XSign_d1 <=  XSign;
            XSign_d2 <=  XSign_d1;
            XSign_d3 <=  XSign_d2;
            XSign_d4 <=  XSign_d3;
            XSign_d5 <=  XSign_d4;
            XSign_d6 <=  XSign_d5;
            resultWillBeOne_d1 <=  resultWillBeOne;
            absK_d1 <=  absK;
            K_d1 <=  K;
            K_d2 <=  K_d1;
            K_d3 <=  K_d2;
            expY_d1 <=  expY;
            needNoNorm_d1 <=  needNoNorm;
            ofl1_d1 <=  ofl1;
            ofl1_d2 <=  ofl1_d1;
            ofl1_d3 <=  ofl1_d2;
            ofl1_d4 <=  ofl1_d3;
            ofl1_d5 <=  ofl1_d4;
            ofl1_d6 <=  ofl1_d5;
            ofl3_d1 <=  ofl3;
            ofl3_d2 <=  ofl3_d1;
            ofl3_d3 <=  ofl3_d2;
            ofl3_d4 <=  ofl3_d3;
            ofl3_d5 <=  ofl3_d4;
            ofl3_d6 <=  ofl3_d5;
            ufl2_d1 <=  ufl2;
            ufl2_d2 <=  ufl2_d1;
            ufl2_d3 <=  ufl2_d2;
            ufl2_d4 <=  ufl2_d3;
            ufl2_d5 <=  ufl2_d4;
            ufl2_d6 <=  ufl2_d5;
            ufl3_d1 <=  ufl3;
            ufl3_d2 <=  ufl3_d1;
            ufl3_d3 <=  ufl3_d2;
            ufl3_d4 <=  ufl3_d3;
            ufl3_d5 <=  ufl3_d4;
            ufl3_d6 <=  ufl3_d5;
         end if;
      end process;
   Xexn <= X(wE+wFIn+2 downto wE+wFIn+1);
   XSign <= X(wE+wFIn);
   XexpField <= X(wE+wFIn-1 downto wFIn);
   Xfrac <= unsigned(X(wFIn-1 downto 0));
   e0 <= conv_std_logic_vector(100, wE+2);  -- bias - (wF+g)
   shiftVal <= ("00" & XexpField) - e0; -- for a left shift
   -- underflow when input is shifted to zero (shiftval<0), in which case exp = 1
   resultWillBeOne <= shiftVal(wE+1);
   --  mantissa with implicit bit
   mXu <= "1" & Xfrac;
   -- Partial overflow detection
   maxShift <= conv_std_logic_vector(33, wE+1);  -- wE-2 + wF+g
   overflow0 <= not shiftVal(wE+1) when shiftVal(wE downto 0) > maxShift else '0';
   shiftValIn <= shiftVal(5 downto 0);
   mantissa_shift: LeftShifter24_by_max_33_F250_uid4
      port map ( clk  => clk,
                 S => shiftValIn,
                 X => std_logic_vector(mXu),
                 R => fixX0);
   ufixX <=  unsigned(fixX0(56 downto 23)) when resultWillBeOne_d1='0' else "0000000000000000000000000000000000";
   xMulIn <= ufixX(33 downto 24); -- fix resize from (6, -27) to (6, -3)
   MulInvLog2: FixRealKCM_F250_uid6
      port map ( clk  => clk,
                 X => std_logic_vector(xMulIn),
                 R => absK);
   minusAbsK <= (8 downto 0 => '0') - ('0' & absK_d1);
   K <= minusAbsK when  XSign_d2='1'   else ('0' & absK_d1);
   MulLog2: FixRealKCM_F250_uid18
      port map ( clk  => clk,
                 X => absK,
                 R => absKLog2);
   subOp1 <= std_logic_vector(ufixX(26 downto 0)) when XSign_d1='0' else not (std_logic_vector(ufixX(26 downto 0)));
   subOp2 <= absKLog2(26 downto 0) when XSign_d2='1' else not (absKLog2(26 downto 0));
   theYAdder: IntAdder_27_F250_uid31
      port map ( clk  => clk,
                 Cin => '1',
                 X => subOp1,
                 Y => subOp2,
                 R => Y);
   -- Now compute the exp of this fixed-point value
   A <= Y(26 downto 17);
   Z <= Y(16 downto 0);
   ExpATable: ExpATable_F250_uid33
      port map ( clk  => clk,
                 X => A,
                 Y => expA);
   Ztrunc <= Z(16 downto 10);
   ExpZmZm1Table: ExpZmZm1Table_F250_uid35
      port map ( X => Ztrunc,
                 Y => expZmZm1_copy36);
   expZmZm1 <= expZmZm1_copy36; -- output copy to hold a pipeline register if needed
   -- Computing Z + (exp(Z)-1-Z)
   expZm1adderX <= '0' & Z;
   expZm1adderY <= (11 downto 0 => '0') & expZmZm1 ;
   Adder_expZm1: IntAdder_18_F250_uid39
      port map ( clk  => clk,
                 Cin => '0',
                 X => expZm1adderX,
                 Y => expZm1adderY,
                 R => expZm1);
   -- Rounding expA to the same accuracy as expZm1
   --   (truncation would not be accurate enough and require one more guard bit)
   expA_T <= expA(27 downto 10);
   Adder_expArounded0: IntAdder_18_F250_uid43
      port map ( clk  => clk,
                 Cin => '1',
                 X => expA_T,
                 Y => "000000000000000000",
                 R => expArounded0);
   expArounded <= expArounded0(17 downto 1);
   TheLowerProduct: IntMultiplier_F250_uid45
      port map ( clk  => clk,
                 X => expArounded,
                 Y => expZm1,
                 R => lowerProduct);
   extendedLowerProduct <= ((27 downto 19 => '0') & lowerProduct(18 downto 0));
   -- Final addition -- the product MSB bit weight is -k+2 = -8
   TheFinalAdder: IntAdder_28_F250_uid175
      port map ( clk  => clk,
                 Cin => '0',
                 X => expA,
                 Y => extendedLowerProduct,
                 R => expY);
   needNoNorm <= expY(27);
   -- Rounding: all this should consume one row of LUTs
   preRoundBiasSig <= conv_std_logic_vector(127, wE+2)  & expY_d1(26 downto 4) when needNoNorm_d1 = '1'
      else conv_std_logic_vector(126, wE+2)  & expY_d1(25 downto 3) ;
   roundBit <= expY(3)  when needNoNorm = '1'    else expY(2) ;
   roundNormAddend <= K_d3(8) & K_d3 & (22 downto 1 => '0') & roundBit;
   roundedExpSigOperandAdder: IntAdder_33_F250_uid178
      port map ( clk  => clk,
                 Cin => '0',
                 X => preRoundBiasSig,
                 Y => roundNormAddend,
                 R => roundedExpSigRes);
   roundedExpSig <= roundedExpSigRes when Xexn_d6="01" else  "000" & (wE-2 downto 0 => '1') & (wF-1 downto 0 => '0');
   ofl1 <= not XSign and overflow0 and (not Xexn(1) and Xexn(0)); -- input positive, normal,  very large
   ofl2 <= not XSign_d6 and (roundedExpSig(wE+wF) and not roundedExpSig(wE+wF+1)) and (not Xexn_d6(1) and Xexn_d6(0)); -- input positive, normal, overflowed
   ofl3 <= not XSign and Xexn(1) and not Xexn(0);  -- input was -infty
   ofl <= ofl1_d6 or ofl2 or ofl3_d6;
   ufl1 <= (roundedExpSig(wE+wF) and roundedExpSig(wE+wF+1))  and (not Xexn_d6(1) and Xexn_d6(0)); -- input normal
   ufl2 <= XSign and Xexn(1) and not Xexn(0);  -- input was -infty
   ufl3 <= XSign and overflow0  and (not Xexn(1) and Xexn(0)); -- input negative, normal,  very large
   ufl <= ufl1 or ufl2_d6 or ufl3_d6;
   Rexn <= "11" when Xexn_d6 = "11"
      else "10" when ofl='1'
      else "00" when ufl='1'
      else "01";
   R <= Rexn & '0' & roundedExpSig(30 downto 0);
end architecture;

