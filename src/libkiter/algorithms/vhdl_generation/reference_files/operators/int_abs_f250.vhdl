library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity int_abs_f250 is
    port (clk : in std_logic;
          X : in  std_logic_vector(8+23+2 downto 0);
          R : out  std_logic_vector(8+23+2 downto 0)   );
end entity;

architecture arch of int_abs_f250 is
begin
  R <= X when (X(33) = '0') else
       not X + "0000000000000000000000000000000001" when (X(33) = '1');
end architecture;
