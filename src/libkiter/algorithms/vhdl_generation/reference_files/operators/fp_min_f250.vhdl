library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity fp_min_f250 is
    port (clk : in std_logic;
          X : in  std_logic_vector(8+23+2 downto 0);
          Y : in  std_logic_vector(8+23+2 downto 0);
          R : out  std_logic_vector(8+23+2 downto 0)   );
end entity;


architecture Behavioral of fp_min_f250 is

  signal X_sign, Y_sign : std_logic;
  signal X_exp, Y_exp : std_logic_vector(8 downto 0);
  signal X_man, Y_man : std_logic_vector(22 downto 0);

begin

  Y_sign <= Y(31);
  X_sign <= X(31);
  X_exp <= X(30 downto 22);
  Y_exp <= Y(30 downto 22);
  X_man <= X(22 downto 0);
  Y_man <= Y(22 downto 0);


  process(clk)
  begin
    if rising_edge(clk) then
      if X_sign < Y_sign then  -- compare sign bits
        R <= X;
      elsif X_sign > Y_sign then
        R <= Y;
      else  -- sign bits are equal, so compare exponents
        if X_exp < Y_exp then
          R <= X;
        elsif X_exp > Y_exp then
          R <= Y;
        else  -- exponents are equal, so compare mantissas
          if X_man <= Y_man then
            R <= X;
          else
            R <= Y;
          end if;
        end if;
      end if;
    end if;
  end process;
end Behavioral;
