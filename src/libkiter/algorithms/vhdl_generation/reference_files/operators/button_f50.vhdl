library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity button_f50 is
    port (clk : in std_logic;
          X : in  std_logic;
          R : out  std_logic_vector(33 downto 0));
end entity;

architecture arch of button_f50 is
begin
  convert_input :  process (clk)
    begin
        if rising_edge(clk) then
            case (X) is
                when '1' => R <= "0100111111100000000000000000000000";
                when '0' => R <= "0000000000000000000000000000000000";
            end case;
        end if;
  end process convert_input;
end architecture;
