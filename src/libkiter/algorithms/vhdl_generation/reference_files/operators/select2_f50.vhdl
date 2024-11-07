library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity select2_f50 is
    port (clk : in std_logic;
          S : in  std_logic; -- selector signal
          X : in  std_logic_vector(8+23+2 downto 0);
          Y : in  std_logic_vector(8+23+2 downto 0);
          R : out  std_logic_vector(8+23+2 downto 0)   );
end entity;

architecture arch of select2_f50 is
begin
  R <= X when (S = '0') else
       Y ;
end architecture;
