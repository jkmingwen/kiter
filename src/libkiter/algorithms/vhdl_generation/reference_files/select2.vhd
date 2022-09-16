library ieee;
use ieee.std_logic_1164.ALL;

entity select2 is
    Port ( s : in std_logic_vector (33 downto 0);
           x1 : in std_logic_vector (33 downto 0);
           x2 : in std_logic_vector (33 downto 0);
           y : out std_logic_vector (33 downto 0));
end select2;

architecture Behavioral of select2 is

begin
-- expecting values of s to either be the integer 0 or 1
-- for any other value, we just output 0
y <= x1 when (s = "0000000000000000000000000000000000") else
     x2 when (s = "0000000000000000000000000000000001") else
     "0000000000000000000000000000000000";
end Behavioral;
