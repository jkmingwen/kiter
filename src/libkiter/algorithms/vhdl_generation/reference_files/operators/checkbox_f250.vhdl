library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity checkbox_f250 is
    port (clk : in std_logic;
          X : in  std_logic;
          R : out  std_logic   );
end entity;

architecture arch of checkbox_f250 is
begin
  R <= X;
end architecture;
