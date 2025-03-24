library ieee;
use ieee.std_logic_1164.all;

entity sbuffer_bypass is
    generic (ram_width : integer);
        port (in_data   : in  std_logic_vector(ram_width-1 downto 0);
              out_data    : out std_logic_vector(ram_width-1 downto 0));
end sbuffer_bypass;

architecture Behavioral of sbuffer_bypass is

begin

    out_data <= in_data;

end Behavioral;
