library ieee;
use ieee.std_logic_1164.all;

entity const_value is
    generic ( value : std_logic_vector (33 downto 0) );
    port ( clk : in std_logic;
           rst : in std_logic;
           out_ready : in std_logic;
           out_valid : out std_logic;
           out_data  : out std_logic_vector (33 downto 0) := (others => '0') );
end const_value;

architecture Behavioral of const_value is
begin

out_valid <= '1';
out_data <= value;

end Behavioral;
