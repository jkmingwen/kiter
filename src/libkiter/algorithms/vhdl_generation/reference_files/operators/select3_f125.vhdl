library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity select3_f125 is
    port (clk : in std_logic;
          S : in  std_logic_vector(1 downto 0); -- selector signal
          X : in  std_logic_vector(8+23+2 downto 0);
          Y : in  std_logic_vector(8+23+2 downto 0);
          Z : in  std_logic_vector(8+23+2 downto 0);
          R : out  std_logic_vector(8+23+2 downto 0)   );
end entity;

architecture arch of select3_f125 is
begin
  R <= X when (S = "00") else
       Y when (S = "01") else
       Z when (S = "10") else
       (others => ('0'));
end architecture;
