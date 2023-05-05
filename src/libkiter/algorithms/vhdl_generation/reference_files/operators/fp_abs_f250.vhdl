library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity fp_abs_f250 is
    port (clk : in std_logic;
          X : in  std_logic_vector(8+23+2 downto 0);
          R : out  std_logic_vector(8+23+2 downto 0)   );
end entity;

architecture arch of fp_abs_f250 is
  signal Rtmp : std_logic_vector(8+23+2 downto 0);
begin
  Rtmp <= X(33 downto 32) & '0' & X(30 downto 0); -- just set sign bit to 0 to get positive value
  R <= Rtmp;
end architecture;
