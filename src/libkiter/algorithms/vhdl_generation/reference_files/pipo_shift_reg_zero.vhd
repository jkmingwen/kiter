library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity pipo_shift_reg_zero is
  port ( in_data : in std_logic_vector(33 downto 0);
         out_data : out std_logic_vector(33 downto 0) );
end pipo_shift_reg_zero;

architecture Behavioral of pipo_shift_reg_zero is

begin

    out_data <= in_data;

end Behavioral;
