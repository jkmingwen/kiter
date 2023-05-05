library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity int_min_f50 is
    port (clk : in std_logic;
          X : in  std_logic_vector(8+23+2 downto 0);
          Y : in  std_logic_vector(8+23+2 downto 0);
          R : out  std_logic_vector(8+23+2 downto 0)   );
end entity;

architecture Behavioral of int_min_f50 is
  signal X_signed, Y_signed : signed(31 downto 0);
begin
  process(clk)
  begin
    if rising_edge(clk) then
      X_signed <= signed(X(31 downto 0));
      Y_signed <= signed(Y(31 downto 0));
      if X_signed < Y_signed then
        R <= X;
      else
        R <= Y;
      end if;
    end if;
  end process;
end Behavioral;
