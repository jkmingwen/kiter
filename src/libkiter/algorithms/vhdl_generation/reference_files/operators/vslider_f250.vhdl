library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity vslider_f250 is
    port (clk : in std_logic;
          X : in  std_logic_vector(33 downto 0);
          R : out  std_logic_vector(33 downto 0)   );
end entity;

architecture arch of vslider_f250 is
begin
  R <= X;
end architecture;
