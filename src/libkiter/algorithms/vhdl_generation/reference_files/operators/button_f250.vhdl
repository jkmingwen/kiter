library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity button_f250 is
    port (clk : in std_logic;
          X : in  std_logic;
          R : out  std_logic   );
end entity;

architecture arch of button_f250 is
begin
  R <= X;
end architecture;
