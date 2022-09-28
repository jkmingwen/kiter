library ieee;
use ieee.std_logic_1164.ALL;

entity select2_op is
    Port ( clk : in std_logic;
           X : in std_logic_vector (33 downto 0);
           Y : in std_logic_vector (33 downto 0);
           Z : in std_logic_vector (33 downto 0);
           R : out std_logic_vector (33 downto 0));
end select2_op;

architecture Behavioral of select2_op is

begin
-- expecting values of X to either be the integer 0 or 1
-- for any other value, we just output 0
R <= Y when (X = "0000000000000000000000000000000000") else
     Z when (X = "0000000000000000000000000000000001") else
     "0000000000000000000000000000000000";
end Behavioral;
