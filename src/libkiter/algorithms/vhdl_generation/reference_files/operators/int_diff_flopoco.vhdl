--------------------------------------------------------------------------------
--                           IntAdder_34_F125_uid2
-- VHDL generated for Zynq7000 @ 125MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved
-- Authors: Bogdan Pasca, Florent de Dinechin (2008-2016)
--------------------------------------------------------------------------------
-- Pipeline depth: 0 cycles
-- Clock period (ns): 8
-- Target frequency (MHz): 125
-- Input signals: X Y Cin
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity IntAdder_34_F125_uid2 is
    port (clk : in std_logic;
          X : in  std_logic_vector(33 downto 0);
          Y : in  std_logic_vector(33 downto 0);
          R : out  std_logic_vector(33 downto 0)   );
end entity;

architecture arch of IntAdder_34_F125_uid2 is
signal Rtmp :  std_logic_vector(33 downto 0);
begin
   Rtmp <= X - Y;
   R <= Rtmp;
end architecture;
